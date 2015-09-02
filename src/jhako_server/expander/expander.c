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
#include "expander.h"

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
void expander_mainloop(void)
{
    pid_t ppid;

    jhklog_info("expander start");

    if (jhkdb_connect() != 0) {
        jhklog_crit("In %s() can not connect database", __func__);
        return;
    }

    while (1) {
        ppid = getppid();
        if (ppid <= 1) {
            jhklog_debug("In %s() ppid: %d", __func__, ppid);
            break;
        }
        expander_exec();
        sleep(1);
    }

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
int expander_exec(void)
{
    apr_uint64_t id;
    dbi_result result = NULL;
    jobunit_t *obj = NULL;

    jhklog_trace("In %s()", __func__);
    result =
        jhkdb_select
        ("SELECT id FROM proc_topjobnets WHERE status = %d",
         JOBUNIT_STATUS_NONE);
    if (result == NULL) {
        return -1;
    }

    obj = jobunit_new();
    while (dbi_result_next_row(result)) {
        jhkdb_begin();
        id = dbi_result_get_ulonglong(result, "id");
        if (proc_topjobnet_select(obj, id) != 0) {
            jhkdb_rollback();
            continue;
        }

        if (expander_topjobnet(obj) != 0) {
            jhkdb_rollback();
            continue;
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
int expander_topjobnet(jobunit_t * obj)
{
    int rc_db;
    char path[MAX_STRING_LEN];
    apr_uint64_t proc_topjobnet_id, jobunit_id, proc_jobunit_id;

    if (obj == NULL)
        return -1;

    jhklog_debug
        ("In %s() proc_topjobnet id: %llu, status: %d, jobunit_id: %llu",
         __func__, obj->id, obj->status, obj->jobunit_id);
    if (obj->status != JOBUNIT_STATUS_NONE)
        return 0;

    // expand rootjobnet
    proc_topjobnet_id = obj->id;
    jobunit_id = obj->jobunit_id;

    // get jobunit path
    jhk_free(obj->path);
    if (jobunit_get_path(jobunit_id, path, MAX_STRING_LEN, 1) != 0) {
        snprintf(path, MAX_STRING_LEN, "");
    }
    obj->path = strdup(path);

    // insert jobnet
    proc_jobunit_id = proc_jobunit_insert(obj);
    if (proc_jobunit_id == 0)
        return -1;

    // set alarm variables
    if (obj->mode == TOPJOBNET_MODE_ALARM && obj->proc_alarm_id > 0) {
        jhklog_info
            ("The alarm is called. proc_topjobnet_id: %llu, proc_jobunit_id: %llu, proc_alarm_id: %llu",
             proc_topjobnet_id, proc_jobunit_id, obj->proc_alarm_id);
        if (alarms_set_variables(obj->proc_alarm_id, proc_jobunit_id) != 0)
            return -1;
    }
    // expand rootjobnet
    if (obj->kind == JOBUNIT_KIND_ROOTJOBNET) {
        if (rootjobnet_put_process(jobunit_id, proc_jobunit_id) != 0)
            return -1;
        if (schedules_put_process(jobunit_id, proc_jobunit_id) != 0)
            return -1;
        if (alarms_put_process(jobunit_id, proc_jobunit_id) != 0)
            return -1;
    }
    // expand jobnet
    snprintf(path, MAX_STRING_LEN, "%s/%s", obj->path, obj->name);
    if (expander_jobnet(jobunit_id, proc_jobunit_id, path) != 0)
        return -1;

    // set proc_jobunit_id
    rc_db =
        jhkdb_execute
        ("UPDATE proc_topjobnets SET status = %d, proc_jobunit_id = %llu, path = '%s', updated_at = '%s' WHERE id = %llu",
         JOBUNIT_STATUS_STANDBY, proc_jobunit_id, obj->path, jhk_time(),
         proc_topjobnet_id);
    if (rc_db != 0)
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
int expander_jobnet(const apr_uint64_t jobnet_id,
                    const apr_uint64_t proc_jobnet_id,
                    const char *jobnet_path)
{
    int rc;
    apr_uint64_t jobunit_id, proc_jobunit_id;
    jobunit_t *obj_jobunit = NULL;
    connectors_t *obj_conns = NULL;
    connectors_t *obj_newconns = NULL;
    dbi_result result = NULL;
    char path[MAX_STRING_LEN];

    jhklog_debug
        ("In %s() jobnet_id: %llu, proc_jobnet_id: %llu, jobnet_path: %s",
         __func__, jobnet_id, proc_jobnet_id, jobnet_path);

    // search children and connectos of the proc_jobnet
    result =
        jhkdb_select("SELECT * FROM jobunits WHERE parent_id = %llu",
                     jobnet_id);
    if (result == NULL)
        return -1;

    // init
    rc = -1;
    obj_jobunit = jobunit_new();
    obj_conns = connectors_new();
    obj_newconns = connectors_new();

    // load connectors data
    connectors_select(obj_conns, jobnet_id);
    connectors_create(obj_newconns, obj_conns->num);

    // insert children proc_jobunit
    while (dbi_result_next_row(result)) {
        // load jobunit information
        jobunit_load(obj_jobunit, result);
        jobunit_id = obj_jobunit->id;
        obj_jobunit->parent_id = proc_jobnet_id;
        obj_jobunit->jobunit_id = jobunit_id;

        // calc boot
        if (obj_jobunit->kind == JOBUNIT_KIND_MERGEJOB) {
            obj_jobunit->boot = 1;
        } else {
            obj_jobunit->boot =
                connectors_calc_boot(obj_conns, jobunit_id);
        }

        // set jobunit path
        jhk_free(obj_jobunit->path);
        obj_jobunit->path = strdup(jobnet_path);

        // insert jobunit information
        proc_jobunit_id = proc_jobunit_insert(obj_jobunit);
        if (proc_jobunit_id == 0)
            goto error;

        // renew connectors prev_jobid & next_jobid 
        if (connectors_renew_jobid
            (obj_newconns, obj_conns, proc_jobnet_id, jobunit_id,
             proc_jobunit_id) != 0)
            goto error;

        // insert other information
        switch (obj_jobunit->kind) {
        case JOBUNIT_KIND_ROOTJOBNET:
        case JOBUNIT_KIND_JOBNET:
            // jobnet
            snprintf(path, MAX_STRING_LEN, "%s/%s", jobnet_path,
                     obj_jobunit->name);
            if (expander_jobnet(jobunit_id, proc_jobunit_id, path) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_SLEEPJOB:
            // sleepjob
            if (sleepjob_put_process(jobunit_id, proc_jobunit_id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_CLOCKJOB:
            // clockjob
            if (clockjob_put_process(jobunit_id, proc_jobunit_id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_DATEJOB:
            // datejob
            if (datejob_put_process(jobunit_id, proc_jobunit_id) != 0)
                goto error;
            // conditions
            if (dateconds_put_process(jobunit_id, proc_jobunit_id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_VARJOB:
            // varjob
            if (vardata_put_process(jobunit_id, proc_jobunit_id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_SSHJOB:
            // sshjob
            if (sshjob_put_process(jobunit_id, proc_jobunit_id) != 0)
                goto error;
            // conditions
            if (conditions_put_process(jobunit_id, proc_jobunit_id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_WINJOB:
            // winjob
            if (winjob_put_process(jobunit_id, proc_jobunit_id) != 0)
                goto error;
            // conditions
            if (conditions_put_process(jobunit_id, proc_jobunit_id) != 0)
                goto error;
            break;
        case JOBUNIT_KIND_EMAILJOB:
            // emailjob
            if (emailjob_put_process(jobunit_id, proc_jobunit_id) != 0)
                goto error;
            break;
        default:
            break;
        }
    }

    // insert proc_connectors
    if (proc_connectors_insert(obj_newconns) != 0)
        goto error;

    rc = 0;
  error:
    dbi_result_free(result);
    jobunit_destroy(obj_jobunit);
    connectors_destroy(obj_conns);
    connectors_destroy(obj_newconns);
    return rc;
}
