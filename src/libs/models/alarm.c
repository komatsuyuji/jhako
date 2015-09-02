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
alarm_t *alarm_new(void)
{
    alarm_t *obj;

    jhklog_trace("In %s()", __func__);
    obj = (alarm_t *) malloc(sizeof(alarm_t));
    alarm_init(obj);

    return obj;
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
void alarm_destroy(alarm_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return;

    alarm_init(obj);
    jhk_free(obj);

    obj = NULL;
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
int alarm_init(alarm_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    obj->id = 0;
    obj->jobunit_id = 0;
    obj->proc_jobunit_id = 0;
    obj->hist_jobunit_id = 0;
    obj->status = 0;
    obj->jobnet_id = 0;

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
int alarm_load(alarm_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    alarm_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->jobunit_id = dbi_result_get_ulonglong(res, "jobunit_id");
    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");
    obj->hist_jobunit_id =
        dbi_result_get_ulonglong(res, "hist_jobunit_id");

    obj->status = dbi_result_get_int(res, "status");
    obj->jobnet_id = dbi_result_get_ulonglong(res, "jobnet_id");

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
int alarm_load_one(alarm_t * obj, dbi_result res)
{
    apr_uint64_t num;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    num = dbi_result_get_numrows(res);
    if (num != 1) {
        jhklog_warn("In %s() can not find alarm. numrows: %llu",
                    __func__, num);
        return 1;
    }

    while (dbi_result_next_row(res)) {
        alarm_load(obj, res);
        break;
    }

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
int proc_alarm_select(alarm_t * obj, const apr_uint64_t id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_trace("In %s() id: %llu", __func__, id);
    result = jhkdb_select("SELECT * FROM proc_alarms WHERE id = %llu", id);
    if (result == NULL)
        return -1;

    rc = alarm_load_one(obj, result);

    dbi_result_free(result);
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
apr_uint64_t proc_alarm_insert(alarm_t * obj)
{
    apr_uint64_t id;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    id = jhkdb_insert
        ("INSERT INTO proc_alarms(proc_jobunit_id, status, jobnet_id) VALUES(%llu, %d, %llu)",
         obj->proc_jobunit_id, obj->status, obj->jobnet_id);
    obj->id = id;

    return id;
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
apr_uint64_t hist_alarm_insert(alarm_t * obj)
{
    apr_uint64_t id;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    id = jhkdb_insert
        ("INSERT INTO hist_alarms(hist_jobunit_id, status, jobnet_id) VALUES(%llu, %d, %llu)",
         obj->hist_jobunit_id, obj->status, obj->jobnet_id);
    obj->id = id;

    return id;
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
int alarms_put_process(const apr_uint64_t jobunit_id,
                       const apr_uint64_t proc_jobunit_id)
{
    int rc;
    alarm_t *obj = NULL;
    dbi_result result = NULL;

    jhklog_debug("In %s() jobunit_id: %llu, proc_jobunit_id: %llu",
                 __func__, jobunit_id, proc_jobunit_id);

    result =
        jhkdb_select("SELECT * FROM alarms WHERE jobunit_id = %llu",
                     jobunit_id);
    if (result == NULL)
        return -1;

    rc = -1;
    obj = alarm_new();
    while (dbi_result_next_row(result)) {
        alarm_load(obj, result);
        obj->proc_jobunit_id = proc_jobunit_id;
        if (proc_alarm_insert(obj) == 0)
            goto error;
    }

    rc = 0;
  error:
    alarm_destroy(obj);
    dbi_result_free(result);
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
int alarms_put_history(const apr_uint64_t proc_jobunit_id,
                       const apr_uint64_t hist_jobunit_id)
{
    int rc;
    alarm_t *obj = NULL;
    dbi_result result = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu, hist_jobunit_id: %llu",
                 __func__, proc_jobunit_id, hist_jobunit_id);

    result =
        jhkdb_select
        ("SELECT * FROM proc_alarms WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    rc = -1;
    obj = alarm_new();
    while (dbi_result_next_row(result)) {
        alarm_load(obj, result);
        obj->hist_jobunit_id = hist_jobunit_id;
        if (hist_alarm_insert(obj) == 0)
            goto error;
    }

    rc = 0;
  error:
    alarm_destroy(obj);
    dbi_result_free(result);
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
int proc_alarms_delete(const apr_uint64_t proc_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM proc_alarms WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
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
int hist_alarms_delete(const apr_uint64_t hist_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM hist_alarms WHERE hist_jobunit_id = %llu",
         hist_jobunit_id);
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
int alarms_execute(const apr_uint64_t proc_jobunit_id, const int status)
{
    alarm_t *obj = NULL;
    jobunit_t *obj_topjobnet = NULL;
    dbi_result result = NULL;

    jhklog_trace("In %s() proc_jobunit_id: %llu, status: %d", __func__,
                 proc_jobunit_id, status);

    result =
        jhkdb_select
        ("SELECT * FROM proc_alarms WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    obj = alarm_new();
    obj_topjobnet = jobunit_new();

    while (dbi_result_next_row(result)) {
        alarm_load(obj, result);
        if (obj->status == status) {
            // can not call alarm again
            proc_topjobnet_select2(obj_topjobnet, proc_jobunit_id);
            if (obj_topjobnet->mode == TOPJOBNET_MODE_ALARM) {
                jhklog_warn
                    ("In %s() cat not call alarm again. proc_jobunit_id: %llu, mode: %d",
                     __func__, proc_jobunit_id, obj_topjobnet->mode);
                goto finish;
            }

            execlog_info(proc_jobunit_id,
                         "Alarm is called. status: %d, jobnet_id: %llu",
                         status, obj->jobnet_id);
            proc_topjobnet_insert_alarm(obj->jobnet_id, obj->id);
        }
    }

  finish:
    alarm_destroy(obj);
    jobunit_destroy(obj_topjobnet);
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
int alarms_set_variables(const apr_uint64_t proc_alarm_id,
                         const apr_uint64_t proc_jobunit_id)
{
    int rc;
    alarm_t *obj = NULL;
    jobunit_t *obj_jobunit = NULL;
    char path[MAX_STRING_LEN];

    jhklog_trace("In %s() proc_alarm_id: %llu, proc_jobunit_id: %llu",
                 __func__, proc_alarm_id, proc_jobunit_id);

    rc = -1;
    obj = alarm_new();
    obj_jobunit = jobunit_new();

    // select alarm
    if (proc_alarm_select(obj, proc_alarm_id) != 0)
        goto error;

    // select alarm jobnet
    if (proc_jobunit_select(obj_jobunit, obj->proc_jobunit_id) != 0)
        goto error;

    // dup variables
    if (proc_variables_dup(obj->proc_jobunit_id, proc_jobunit_id) != 0)
        goto error;

    // set alarm variables
    snprintf(path, MAX_STRING_LEN, "%s/%s", obj_jobunit->path,
             obj_jobunit->name);
    proc_variable_overwrite(proc_jobunit_id, "ALARM_JOBUNIT_PATH", path,
                            VARDATUM_OVERWRITE_TRUE);

    rc = 0;
  error:
    alarm_destroy(obj);
    jobunit_destroy(obj_jobunit);
    return rc;
}
