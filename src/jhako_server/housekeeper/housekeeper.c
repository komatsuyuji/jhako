/////////////////////////////////////////////////////////////////////////////////
//
// jHako
// Copyright (C) 2014-2015 Komatsu Yuji(Zheng Chuyu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
/////////////////////////////////////////////////////////////////////////////////

#include "common.h"
#include "jhklog.h"
#include "jhkdb.h"
#include "models.h"
#include "housekeeper.h"

configure_t *housekeeper_conf = NULL;

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
void housekeeper_mainloop(void)
{
    pid_t ppid;
    int cnt;

    jhklog_info("housekeeper start");

    if (jhkdb_connect() != 0) {
        jhklog_crit("In %s() can not connect database", __func__);
        return;
    }

    cnt = 0;
    housekeeper_conf = configure_new();
    while (1) {
        ppid = getppid();
        if (ppid <= 1) {
            jhklog_debug("In %s() ppid: %d", __func__, ppid);
            break;
        }

        housekeeper_forcestart();
        if (cnt >= 60) {
            cnt = 0;
            configure_get(housekeeper_conf);
            housekeeper_exec();
            housekeeper_history();
        } else {
            cnt++;
        }
        sleep(1);
    }

    configure_destroy(housekeeper_conf);
    jhkdb_disconnect();
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
int housekeeper_forcestart(void)
{
    int rc;

    jhklog_trace("In %s()", __func__);

    jhkdb_begin();
    rc = jhkdb_execute
        ("UPDATE proc_topjobnets SET status = %d, force_start = false, start_time = NULL, end_time = NULL, updated_at = '%s' WHERE status in (%d, %d) AND force_start = true",
         JOBUNIT_STATUS_READY, jhk_time(), JOBUNIT_STATUS_OUTSCHEDULE,
         JOBUNIT_STATUS_CARRYOVER);
    jhkdb_commit();

    return rc;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
int housekeeper_exec(void)
{
    apr_uint64_t id;
    time_t now, t;
    int flag;
    dbi_result result = NULL;
    jobunit_t *obj = NULL;

    jhklog_trace("In %s()", __func__);
    result =
        jhkdb_select
        ("SELECT id FROM proc_topjobnets WHERE status in (%d, %d, %d, %d, %d)",
         JOBUNIT_STATUS_END, JOBUNIT_STATUS_ERROR,
         JOBUNIT_STATUS_OUTSCHEDULE, JOBUNIT_STATUS_CARRYOVER,
         JOBUNIT_STATUS_FORCESTOP);
    if (result == NULL)
        return -1;

    now = time(NULL);
    obj = jobunit_new();
    while (dbi_result_next_row(result)) {
        jhkdb_begin();
        id = dbi_result_get_ulonglong(result, "id");
        if (proc_topjobnet_select(obj, id) != 0) {
            jhkdb_rollback();
            continue;
        }

        flag = 0;
        t = now - obj->end_time;
        if (t < 0) {
            flag = 1;
        }
        switch (obj->status) {
        case JOBUNIT_STATUS_END:
            if (t > (housekeeper_conf->end_period * 3600)) {
                flag = 1;
            }
            break;
        case JOBUNIT_STATUS_ERROR:
            if (t > (housekeeper_conf->error_period * 3600)) {
                flag = 1;
            }
            break;
        case JOBUNIT_STATUS_OUTSCHEDULE:
            if (t > (housekeeper_conf->outschedule_period * 3600)) {
                flag = 1;
            }
            break;
        case JOBUNIT_STATUS_CARRYOVER:
            if (t > (housekeeper_conf->carryover_period * 3600)) {
                flag = 1;
            }
            break;
        case JOBUNIT_STATUS_FORCESTOP:
            if (t > (housekeeper_conf->forcestop_period * 3600)) {
                flag = 1;
            }
            break;
        default:
            break;
        }

        if (flag == 1) {
            if (housekeeper_topjobnet(obj) != 0) {
                jhkdb_rollback();
                continue;
            }
        }
        jhkdb_commit();
    }

    jobunit_destroy(obj);
    dbi_result_free(result);
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
int housekeeper_topjobnet(jobunit_t * obj)
{
    apr_uint64_t id, proc_jobunit_id, hist_jobunit_id;

    jhklog_trace("In %s()", __func__);

    if (obj == NULL)
        return -1;

    jhklog_debug("In %s() proc_topjobnet_id: %llu, proc_jobunit_id: %llu",
                 __func__, obj->id, obj->proc_jobunit_id);

    id = obj->id;

    // insert rootjobnet
    proc_jobunit_id = obj->proc_jobunit_id;
    hist_jobunit_id = hist_jobunit_insert(obj);
    if (hist_jobunit_id == 0)
        return -1;

    // housekeeper rootjobnet
    if (obj->kind == JOBUNIT_KIND_ROOTJOBNET) {
        // rootjobnet
        if (rootjobnet_put_history(proc_jobunit_id, hist_jobunit_id) != 0)
            return -1;
        if (proc_rootjobnet_delete(proc_jobunit_id) != 0)
            return -1;

        // schedules
        if (schedules_put_history(proc_jobunit_id, hist_jobunit_id) != 0)
            return -1;
        if (proc_schedules_delete(proc_jobunit_id) != 0)
            return -1;
    }
    // variables
    if (variables_put_history(proc_jobunit_id, hist_jobunit_id) != 0)
        return -1;
    if (proc_variables_delete(proc_jobunit_id) != 0)
        return -1;

    // housekeeper jobnet's children
    if (housekeeper_jobnet(proc_jobunit_id, hist_jobunit_id) != 0)
        return -1;

    if (proc_topjobnet_delete(id) != 0)
        return -1;

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
int housekeeper_jobnet(const apr_uint64_t proc_jobnet_id,
                       const apr_uint64_t hist_jobnet_id)
{
    int rc;
    apr_uint64_t proc_jobunit_id, hist_jobunit_id;
    jobunit_t *obj_jobunit = NULL;
    connectors_t *obj_conns = NULL;
    connectors_t *obj_newconns = NULL;
    dbi_result result = NULL;

    jhklog_debug("In %s() proc_jobnet_id: %llu , hist_jobnet_id: %llu",
                 __func__, proc_jobnet_id, hist_jobnet_id);

    result =
        jhkdb_select("SELECT * FROM proc_jobunits WHERE parent_id = %llu",
                     proc_jobnet_id);
    if (result == NULL)
        return -1;

    // init
    rc = -1;
    obj_jobunit = jobunit_new();
    obj_conns = connectors_new();
    obj_newconns = connectors_new();

    // load connectors data
    proc_connectors_select(obj_conns, proc_jobnet_id);
    connectors_create(obj_newconns, obj_conns->num);

    // insert children hist_jobunit
    while (dbi_result_next_row(result)) {
        // load jobunit information
        jobunit_load(obj_jobunit, result);
        proc_jobunit_id = obj_jobunit->id;
        obj_jobunit->parent_id = hist_jobnet_id;

        // insert jobunit information
        hist_jobunit_id = hist_jobunit_insert(obj_jobunit);
        if (hist_jobunit_id == 0)
            goto error;

        // insert jobresult infotmation
        if (jobresult_put_history(proc_jobunit_id, hist_jobunit_id) != 0)
            goto error;
        if (proc_jobresult_delete(proc_jobunit_id) != 0)
            goto error;

        // renew connectors prev_jobid & next_jobid 
        if (connectors_renew_jobid
            (obj_newconns, obj_conns, hist_jobnet_id, proc_jobunit_id,
             hist_jobunit_id) != 0)
            goto error;

        // delete execution log
        if (proc_execlog_delete(proc_jobunit_id) != 0)
            goto error;

        // insert other information
        switch (obj_jobunit->kind) {
        case JOBUNIT_KIND_ROOTJOBNET:
        case JOBUNIT_KIND_JOBNET:
            // jobnet
            if (housekeeper_jobnet(proc_jobunit_id, hist_jobunit_id) != 0)
                goto error;
            // variables
            if (variables_put_history(proc_jobunit_id, hist_jobunit_id) !=
                0)
                goto error;
            if (proc_variables_delete(proc_jobunit_id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_SLEEPJOB:
            // sleepjob
            if (sleepjob_put_history(proc_jobunit_id, hist_jobunit_id) !=
                0)
                goto error;
            if (proc_sleepjob_delete(proc_jobunit_id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_CLOCKJOB:
            // clockjob
            if (clockjob_put_history(proc_jobunit_id, hist_jobunit_id) !=
                0)
                goto error;
            if (proc_clockjob_delete(proc_jobunit_id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_DATEJOB:
            // datejob
            if (datejob_put_history(proc_jobunit_id, hist_jobunit_id) != 0)
                goto error;
            if (dateconds_put_history
                (proc_jobunit_id, hist_jobunit_id) != 0)
                goto error;
            if (proc_datejob_delete(proc_jobunit_id) != 0)
                goto error;
            if (proc_datecond_delete(proc_jobunit_id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_VARJOB:
            // varjob
            if (vardata_put_history(proc_jobunit_id, hist_jobunit_id) != 0)
                goto error;
            if (proc_vardata_delete(proc_jobunit_id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_SSHJOB:
            // sshjob
            if (sshjob_put_history(proc_jobunit_id, hist_jobunit_id)
                != 0)
                goto error;
            if (conditions_put_history
                (proc_jobunit_id, hist_jobunit_id) != 0)
                goto error;
            if (proc_sshjob_delete(proc_jobunit_id) != 0)
                goto error;
            if (proc_condition_delete(proc_jobunit_id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_WINJOB:
            // winjob
            if (winjob_put_history(proc_jobunit_id, hist_jobunit_id) != 0)
                goto error;
            if (conditions_put_history
                (proc_jobunit_id, hist_jobunit_id) != 0)
                goto error;
            if (proc_winjob_delete(proc_jobunit_id) != 0)
                goto error;
            if (proc_condition_delete(proc_jobunit_id) != 0)
                goto error;
            break;
        default:
            break;
        }
    }

    // insert history connectors
    if (hist_connectors_insert(obj_newconns) != 0)
        goto error;

    // delete children jobs
    if (proc_jobunit_delete(proc_jobnet_id) != 0)
        goto error;

    // delete process connectors
    if (proc_connector_delete(proc_jobnet_id) != 0)
        goto error;

    // delete children jobs
    if (proc_jobunit_delete_parent_id(proc_jobnet_id) != 0)
        goto error;

    // delete execution log
    if (proc_execlog_delete(proc_jobnet_id) != 0)
        goto error;

    rc = 0;
  error:
    dbi_result_free(result);
    connectors_destroy(obj_conns);
    connectors_destroy(obj_newconns);
    jobunit_destroy(obj_jobunit);
    return rc;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
int housekeeper_history(void)
{
    int rc;
    apr_uint64_t id;
    int kind;
    time_t hp;
    char *history_period;
    dbi_result result = NULL;

    jhklog_trace("In %s()", __func__);

    // calc history_period
    hp = time(NULL) - housekeeper_conf->history_period * ONE_DAY_SECOND;
    history_period = jhk_time2str(hp);
    jhklog_debug("In %s() history_period: %s", __func__, history_period);

    result =
        jhkdb_select
        ("SELECT id, kind FROM hist_jobunits WHERE created_at <= '%s'",
         history_period);
    jhk_free(history_period);
    if (result == NULL)
        return -1;

    rc = -1;
    while (dbi_result_next_row(result)) {
        jhkdb_begin();
        id = dbi_result_get_ulonglong(result, "id");
        kind = dbi_result_get_int(result, "kind");
        jhklog_debug("In %s() id: %llu, kind %d", __func__, id, kind);

        if (hist_jobunit_delete(id) != 0)
            goto error;
        if (hist_jobresult_delete(id) != 0)
            goto error;

        switch (kind) {
        case JOBUNIT_KIND_ROOTJOBNET:
            if (hist_rootjobnet_delete(id) != 0)
                goto error;
            if (hist_schedules_delete(id) != 0)
                goto error;
            if (hist_variables_delete(id) != 0)
                goto error;
            if (hist_connector_delete(id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_JOBNET:
            if (hist_variables_delete(id) != 0)
                goto error;
            if (hist_connector_delete(id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_SLEEPJOB:
            // sleepjob
            if (hist_sleepjob_delete(id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_CLOCKJOB:
            // clockjob
            if (hist_clockjob_delete(id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_DATEJOB:
            // datejob
            if (hist_datejob_delete(id) != 0)
                goto error;
            if (hist_datecond_delete(id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_VARJOB:
            // varjob
            if (hist_vardata_delete(id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_SSHJOB:
            // sshjob
            if (hist_sshjob_delete(id) != 0)
                goto error;
            if (hist_condition_delete(id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_WINJOB:
            // winjob
            if (hist_winjob_delete(id) != 0)
                goto error;
            if (hist_condition_delete(id) != 0)
                goto error;
            break;
        default:
            break;
        }
        jhkdb_commit();
    }
    rc = 0;

  error:
    dbi_result_free(result);
    return 0;
}
