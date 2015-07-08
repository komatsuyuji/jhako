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
clockjob_t *clockjob_new(void)
{
    clockjob_t *obj;

    jhklog_trace("In %s()", __func__);
    obj = (clockjob_t *) malloc(sizeof(clockjob_t));
    clockjob_init(obj);

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
void clockjob_destroy(clockjob_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return;

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
int clockjob_init(clockjob_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    obj->id = 0;
    obj->jobunit_id = 0;
    obj->proc_jobunit_id = 0;
    obj->hist_jobunit_id = 0;
    obj->basetime = 0;
    obj->predetermined_time = 0;
    obj->day = 0;
    obj->hour = 0;
    obj->minute = 0;
    obj->delay_limit = 0;
    obj->timeover = 0;

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
int clockjob_load(clockjob_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    clockjob_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->jobunit_id = dbi_result_get_ulonglong(res, "jobunit_id");
    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");
    obj->hist_jobunit_id =
        dbi_result_get_ulonglong(res, "hist_jobunit_id");

    obj->basetime = dbi_result_get_int(res, "basetime");
    obj->predetermined_time =
        jhkdb_get_datetime(res, "predetermined_time");
    obj->day = dbi_result_get_int(res, "day");
    obj->hour = dbi_result_get_int(res, "hour");
    obj->minute = dbi_result_get_int(res, "minute");
    obj->delay_limit = dbi_result_get_int(res, "delay_limit");
    obj->timeover = dbi_result_get_int(res, "timeover");

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
int clockjob_load_one(clockjob_t * obj, dbi_result res)
{
    apr_uint64_t num;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    num = dbi_result_get_numrows(res);
    if (num != 1) {
        jhklog_warn("In %s() can not find clockjob. numrows: %llu",
                    __func__, num);
        return 1;
    }

    while (dbi_result_next_row(res)) {
        clockjob_load(obj, res);
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
int clockjob_select(clockjob_t * obj, const apr_uint64_t jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() jobunit_id: %llu", __func__, jobunit_id);

    result =
        jhkdb_select("SELECT * FROM clockjobs WHERE jobunit_id = %llu",
                     jobunit_id);
    if (result == NULL)
        return -1;

    rc = clockjob_load_one(obj, result);

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
int proc_clockjob_select(clockjob_t * obj,
                         const apr_uint64_t proc_jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu", __func__,
                 proc_jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM proc_clockjobs WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    rc = clockjob_load_one(obj, result);

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
int hist_clockjob_select(clockjob_t * obj,
                         const apr_uint64_t hist_jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() hist_jobunit_id: %llu", __func__,
                 hist_jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM hist_clockjobs WHERE hist_jobunit_id = %llu",
         hist_jobunit_id);
    if (result == NULL)
        return -1;

    rc = clockjob_load_one(obj, result);

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
apr_uint64_t proc_clockjob_insert(clockjob_t * obj)
{
    apr_uint64_t id;
    char *predetermined_time;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    predetermined_time = jhkdb_escape_datetime(obj->predetermined_time);

    id = jhkdb_insert
        ("INSERT INTO proc_clockjobs(proc_jobunit_id, basetime, predetermined_time, day, hour, minute, delay_limit, timeover) VALUES(%llu, %d, %s, %d, %d, %d, %d, %d)",
         obj->proc_jobunit_id, obj->basetime, predetermined_time,
         obj->day, obj->hour, obj->minute, obj->delay_limit,
         obj->timeover);
    obj->id = id;

    jhk_free(predetermined_time);
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
apr_uint64_t hist_clockjob_insert(clockjob_t * obj)
{
    apr_uint64_t id;
    char *predetermined_time;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    predetermined_time = jhkdb_escape_datetime(obj->predetermined_time);

    id = jhkdb_insert
        ("INSERT INTO hist_clockjobs(hist_jobunit_id, basetime, predetermined_time, day, hour, minute, delay_limit, timeover) VALUES(%llu, %d, %s, %d, %d, %d, %d, %d)",
         obj->hist_jobunit_id, obj->basetime, predetermined_time,
         obj->day, obj->hour, obj->minute, obj->delay_limit,
         obj->timeover);
    obj->id = id;

    jhk_free(predetermined_time);
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
int clockjob_put_process(const apr_uint64_t jobunit_id,
                         const apr_uint64_t proc_jobunit_id)
{
    int rc;
    clockjob_t *obj = NULL;

    jhklog_debug("In %s() jobunit_id: %llu, proc_jobunit_id: %llu",
                 __func__, jobunit_id, proc_jobunit_id);
    rc = -1;
    obj = clockjob_new();

    if (clockjob_select(obj, jobunit_id) != 0)
        goto error;

    obj->proc_jobunit_id = proc_jobunit_id;
    if (proc_clockjob_insert(obj) == 0)
        goto error;

    rc = 0;
  error:
    clockjob_destroy(obj);
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
int clockjob_put_history(const apr_uint64_t proc_jobunit_id,
                         const apr_uint64_t hist_jobunit_id)
{
    int rc;
    clockjob_t *obj = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu, hist_jobunit_id: %llu",
                 __func__, proc_jobunit_id, hist_jobunit_id);
    rc = -1;
    obj = clockjob_new();

    if (proc_clockjob_select(obj, proc_jobunit_id) != 0)
        goto error;

    obj->hist_jobunit_id = hist_jobunit_id;
    if (hist_clockjob_insert(obj) == 0)
        goto error;

    rc = 0;
  error:
    clockjob_destroy(obj);
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
int proc_clockjob_delete(const apr_uint64_t proc_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM proc_clockjobs WHERE proc_jobunit_id = %llu",
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
int hist_clockjob_delete(const apr_uint64_t hist_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM hist_clockjobs WHERE hist_jobunit_id = %llu",
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
int clockjob_execute(jobunit_t * proc_jobunit)
{
    int rc, rc_db;
    clockjob_t *obj = NULL;
    int day;
    time_t basetime;
    struct tm *tm = NULL;
    char *predetermined_time;

    if (proc_jobunit == NULL)
        return -1;

    jhklog_trace("In %s() proc_jobunit_id: %llu, parent_id: %llu",
                 __func__, proc_jobunit->id, proc_jobunit);
    // init
    rc = -1;
    predetermined_time = NULL;
    obj = clockjob_new();

    if (proc_clockjob_select(obj, proc_jobunit->id) != 0)
        goto error;

    switch (obj->basetime) {
    case CLOCKJOB_BASETIME_JOB_STARTDAY:
        basetime = jhk_midnight(time(NULL));
        break;
    case CLOCKJOB_BASETIME_JOB_STARTTIME:
        basetime = time(NULL);
        break;
    case CLOCKJOB_BASETIME_JOBNET_STARTDAY:
        basetime =
            jhk_midnight(proc_jobunit_get_start_time
                         (proc_jobunit->parent_id));
        break;
    case CLOCKJOB_BASETIME_JOBNET_STARTTIME:
        basetime = proc_jobunit_get_start_time(proc_jobunit->parent_id);
        break;
    case CLOCKJOB_BASETIME_ROOTJOBNET_STARTDAY:
        basetime =
            jhk_midnight(proc_jobunit_get_rootjobnet_starttime
                         (proc_jobunit->parent_id));
        break;
    case CLOCKJOB_BASETIME_ROOTJOBNET_STARTTIME:
        basetime =
            proc_jobunit_get_rootjobnet_starttime(proc_jobunit->parent_id);
        break;
    default:
        basetime = jhk_midnight(time(NULL));
        break;
    }

    // calc predetermined_time
    tm = localtime(&basetime);
    day = obj->day;
    if (obj->hour < tm->tm_hour) {
        day++;
    } else if (obj->hour == tm->tm_hour && obj->minute < tm->tm_min) {
        day++;
    }
    tm->tm_mday += day;
    tm->tm_hour = obj->hour;
    tm->tm_min = obj->minute;
    tm->tm_sec = 0;

    predetermined_time = jhkdb_escape_datetime(mktime(tm));
    jhklog_debug
        ("In %s() proc_jobunit_id: %llu, basetime: %d, predetermined_time: %s",
         __func__, proc_jobunit->id, obj->basetime, predetermined_time);

    // set predetermined_time
    rc_db =
        jhkdb_execute
        ("UPDATE proc_clockjobs SET predetermined_time = %s WHERE proc_jobunit_id = %lld",
         predetermined_time, proc_jobunit->id);
    if (rc_db != 0)
        goto error;

    // run clockjob
    rc = proc_jobunit_set_running(proc_jobunit);
  error:
    clockjob_destroy(obj);
    jhk_free(predetermined_time);
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
int clockjob_monitor(jobunit_t * proc_jobunit)
{
    int rc;
    time_t now;
    clockjob_t *obj = NULL;
    int conn_kind;

    if (proc_jobunit == NULL)
        return -1;

    jhklog_trace("In %s() id: %llu, status: %d, force_stop: %d", __func__,
                 proc_jobunit->id, proc_jobunit->status,
                 proc_jobunit->force_stop);

    // force stop
    if (proc_jobunit->force_stop == 1) {
        return proc_jobunit_set_error(proc_jobunit);
    }
    // normal monitor
    rc = -1;
    obj = clockjob_new();

    if (proc_clockjob_select(obj, proc_jobunit->id) != 0)
        goto error;
    jhklog_debug
        ("In %s() predetermined_time: %ld, day: %d, hour: %d, minute: %d, delay_limit: %d, timeover: %d",
         __func__, obj->predetermined_time, obj->day, obj->hour,
         obj->minute, obj->delay_limit, obj->timeover);

    now = time(NULL);
    if (now >= obj->predetermined_time) {
        if (now < (obj->predetermined_time + obj->delay_limit * 60)) {
            conn_kind = CONNECTOR_KIND_NORMAL;
        } else {
            execlog_warn(proc_jobunit->id, "time over");
            if (obj->timeover == CLOCKJOB_TIMEOVER_NORMAL) {
                conn_kind = CONNECTOR_KIND_NORMAL;
            } else if (obj->timeover == CLOCKJOB_TIMEOVER_BRANCH) {
                conn_kind = CONNECTOR_KIND_BRANCH;
            } else {
                conn_kind = CONNECTOR_KIND_ERROR;
            }
        }
        if (proc_jobunit_set_finish(proc_jobunit, conn_kind) != 0)
            goto error;
    }

    rc = 0;
  error:
    clockjob_destroy(obj);
    return rc;
}
