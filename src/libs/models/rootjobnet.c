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
rootjobnet_t *rootjobnet_new(void)
{
    rootjobnet_t *obj;

    jhklog_trace("In %s()", __func__);
    obj = (rootjobnet_t *) malloc(sizeof(rootjobnet_t));
    rootjobnet_init(obj);

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
void rootjobnet_destroy(rootjobnet_t * obj)
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
int rootjobnet_init(rootjobnet_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    obj->id = 0;
    obj->jobunit_id = 0;
    obj->proc_jobunit_id = 0;
    obj->hist_jobunit_id = 0;
    obj->user_id = 0;
    obj->disabled = 0;
    obj->active_since = 0;
    obj->active_till = 0;
    obj->calendar_id = 0;
    obj->run_type = 0;
    obj->delay_limit = 1;
    obj->loader_interval = 5;

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
int rootjobnet_load(rootjobnet_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);

    if (obj == NULL || res == NULL)
        return -1;

    rootjobnet_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->jobunit_id = dbi_result_get_ulonglong(res, "jobunit_id");
    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");
    obj->hist_jobunit_id =
        dbi_result_get_ulonglong(res, "hist_jobunit_id");

    obj->user_id = dbi_result_get_ulonglong(res, "user_id");
    obj->disabled = dbi_result_get_int(res, "disabled");
    obj->active_since = jhkdb_get_datetime(res, "active_since");
    obj->active_till = jhkdb_get_datetime(res, "active_till");
    obj->calendar_id = dbi_result_get_ulonglong(res, "calendar_id");
    obj->run_type = dbi_result_get_int(res, "run_type");
    obj->delay_limit = dbi_result_get_int(res, "delay_limit");
    obj->loader_interval = dbi_result_get_int(res, "loader_interval");

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
int rootjobnet_load_one(rootjobnet_t * obj, dbi_result res)
{
    apr_uint64_t num;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    num = dbi_result_get_numrows(res);
    if (num != 1) {
        jhklog_warn("In %s() can not find rootjobnet. numrows: %llu",
                    __func__, num);
        return 1;
    }

    while (dbi_result_next_row(res)) {
        rootjobnet_load(obj, res);
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
int rootjobnet_select(rootjobnet_t * obj, const apr_uint64_t jobunit_id)
{
    int rc;
    apr_uint64_t num;
    dbi_result result = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    jhklog_trace("In %s() jobunit_id: %llu", __func__, jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM rootjobnets WHERE jobunit_id = %llu", jobunit_id);
    if (result == NULL)
        return -1;

    rc = rootjobnet_load_one(obj, result);

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
int proc_rootjobnet_select(rootjobnet_t * obj,
                           const apr_uint64_t proc_jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu", __func__,
                 proc_jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM proc_rootjobnets WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    rc = rootjobnet_load_one(obj, result);

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
int hist_rootjobnet_select(rootjobnet_t * obj,
                           const apr_uint64_t hist_jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() hist_jobunit_id: %llu", __func__,
                 hist_jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM hist_rootjobnets WHERE hist_jobunit_id = %llu",
         hist_jobunit_id);
    if (result == NULL)
        return -1;

    rc = rootjobnet_load_one(obj, result);

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
apr_uint64_t proc_rootjobnet_insert(rootjobnet_t * obj)
{
    apr_uint64_t id;
    char *active_since, *active_till;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    active_since = jhkdb_escape_datetime(obj->active_since);
    active_till = jhkdb_escape_datetime(obj->active_till);

    id = jhkdb_insert
        ("INSERT INTO proc_rootjobnets(proc_jobunit_id, user_id, disabled, active_since, active_till, calendar_id, run_type, delay_limit, loader_interval) \
 VALUES(%llu, %llu, %d, %s, %s, %llu, %d, %d, %d)",
         obj->proc_jobunit_id, obj->user_id, obj->disabled, active_since, active_till, obj->calendar_id, obj->run_type, obj->delay_limit, obj->loader_interval);
    obj->id = id;

    jhk_free(active_since);
    jhk_free(active_till);
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
apr_uint64_t hist_rootjobnet_insert(rootjobnet_t * obj)
{
    apr_uint64_t id;
    char *active_since, *active_till;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    active_since = jhkdb_escape_datetime(obj->active_since);
    active_till = jhkdb_escape_datetime(obj->active_till);

    id = jhkdb_insert
        ("INSERT INTO hist_rootjobnets(hist_jobunit_id, user_id, disabled, active_since, active_till, calendar_id, run_type, delay_limit, loader_interval) \
 VALUES(%llu, %llu, %d, %s, %s, %llu, %d, %d, %d)",
         obj->hist_jobunit_id, obj->user_id, obj->disabled, active_since, active_till, obj->calendar_id, obj->run_type, obj->delay_limit, obj->loader_interval);
    obj->id = id;

    jhk_free(active_since);
    jhk_free(active_till);
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
int rootjobnet_put_process(const apr_uint64_t jobunit_id,
                           const apr_uint64_t proc_jobunit_id)
{
    int rc;
    rootjobnet_t *obj = NULL;

    jhklog_debug("In %s() jobunit_id: %llu, proc_jobunit_id: %llu",
                 __func__, jobunit_id, proc_jobunit_id);
    rc = -1;
    obj = rootjobnet_new();

    if (rootjobnet_select(obj, jobunit_id) != 0)
        goto error;

    obj->proc_jobunit_id = proc_jobunit_id;
    if (proc_rootjobnet_insert(obj) == 0)
        goto error;

    rc = 0;
  error:
    rootjobnet_destroy(obj);
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
int rootjobnet_put_history(const apr_uint64_t proc_jobunit_id,
                           const apr_uint64_t hist_jobunit_id)
{
    int rc;
    rootjobnet_t *obj = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu, hist_jobunit_id: %llu",
                 __func__, proc_jobunit_id, hist_jobunit_id);
    rc = -1;
    obj = rootjobnet_new();

    if (proc_rootjobnet_select(obj, proc_jobunit_id) != 0)
        goto error;

    obj->hist_jobunit_id = hist_jobunit_id;
    if (hist_rootjobnet_insert(obj) == 0)
        goto error;

    rc = 0;
  error:
    rootjobnet_destroy(obj);
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
int proc_rootjobnet_delete(const apr_uint64_t proc_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM proc_rootjobnets WHERE proc_jobunit_id = %llu",
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
int hist_rootjobnet_delete(const apr_uint64_t hist_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM hist_rootjobnets WHERE hist_jobunit_id = %llu",
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
int rootjobnet_on_active(rootjobnet_t * obj, const time_t t)
{
    int target_date, since_date, till_date;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || t == 0)
        return JHAKO_ERROR;

    jhklog_debug
        ("In %s() rootjobnet_id: %llu, active_since: %ld, active_till: %ld, current time: %ld ",
         __func__, obj->id, obj->active_since, obj->active_till, t);

    if (obj->active_since <= 0 && obj->active_till <= 0) {
        jhklog_trace
            ("In %s() the rootjobnet do not set active dates. id: %llu",
             __func__, obj->id);
        return JHAKO_TRUE;
    }
    // change time to YYYYMMDD
    target_date = jhk_time2day(t);
    since_date = jhk_time2day(obj->active_since);
    till_date = jhk_time2day(obj->active_till);

    // check active date 
    jhklog_trace("In %s() target_date: %d, since_date: %d, till_date: %d",
                 __func__, target_date, since_date, till_date);
    if (target_date >= since_date) {
        if (target_date <= till_date || obj->active_till <= 0) {
            jhklog_trace("In %s() the rootjobnet is active. id: %llu",
                         __func__, obj->id);
            return JHAKO_TRUE;
        } else {
            jhklog_trace("In %s() the rootjobnet is not active. id: %llu",
                         __func__, obj->id);
            return JHAKO_FALSE;
        }
    } else {
        jhklog_trace("In %s() the rootjobnet is not active. id: %llu",
                     __func__, obj->id);
        return JHAKO_FALSE;
    }

    return JHAKO_TRUE;
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
int rootjobnet_scheduling(rootjobnet_t * obj, const time_t t)
{
    int rc, num, sec;
    apr_uint64_t id;
    time_t i, tz, start_time, end_time;
    dbi_result result = NULL;
    jobunit_t *obj_topjobnet = NULL;
    schedule_t *obj_schedule = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    jhklog_trace
        ("In %s() id: %llu, loader_interval: %d, current time: %ld",
         __func__, obj->id, obj->loader_interval, t);
    if (t == 0 || obj->loader_interval < 1) {
        jhklog_warn("In %s() current time or loader_interval is unset",
                    __func__);
        return -1;
    }
    // load jobunit & rootjobnet information to proc_topjobnet
    rc = -1;
    obj_topjobnet = jobunit_new();
    if (jobunit_select(obj_topjobnet, obj->jobunit_id) != 0) {
        jhklog_warn("In %s() can not find jobunit_id: %llu",
                    __func__, obj->jobunit_id);
        goto error;
    }
    obj_topjobnet->jobunit_id = obj->jobunit_id;
    obj_topjobnet->proc_jobunit_id = 0;
    obj_topjobnet->mode = TOPJOBNET_MODE_SCHEDULE;
    obj_topjobnet->run_type = obj->run_type;
    obj_topjobnet->delay_limit = obj->delay_limit;

    // search schedules
    result =
        jhkdb_select("SELECT * FROM schedules WHERE jobunit_id = %llu",
                     obj->jobunit_id);
    if (result == NULL)
        goto error;

    // calc schedule period
    tz = 0;
    tz = mktime(gmtime(&tz));
    sec = obj->loader_interval * 60;
    start_time = ((time_t) ((t - tz) / sec + 1)) * sec;
    start_time = mktime(gmtime(&start_time));
    end_time = start_time + sec;
    jhklog_trace
        ("In %s() current time: %ld, sec: %d, start_time: %ld, end_time: %ld",
         __func__, t, sec, start_time, end_time);

    num = 0;
    obj_schedule = schedule_new();
    while (dbi_result_next_row(result)) {
        schedule_load(obj_schedule, result);
        for (i = start_time; i < end_time; i += 60) {
            // check schedule
            if (schedule_on_time(obj_schedule, i) != JHAKO_TRUE)
                continue;

            // insert topjobnet
            jhklog_trace
                ("In %s() the schedule_time: %ld will be loaded. schedule_id: %llu",
                 __func__, i, obj_schedule->id);

            //  create topjobnet
            obj_topjobnet->schedule_time = i;
            id = proc_topjobnet_insert(obj_topjobnet);
            if (id == 0) {
                continue;
            }
            num++;
        }
    }
    jhklog_trace("In %s() rootjobnet is loaded. id: %llu, num: %d",
                 __func__, obj->id, num);

    rc = 0;
  error:
    jobunit_destroy(obj_topjobnet);
    schedule_destroy(obj_schedule);
    dbi_result_free(result);
    return rc;
}
