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
schedule_t *schedule_new(void)
{
    schedule_t *obj;

    jhklog_trace("In %s()", __func__);
    obj = (schedule_t *) malloc(sizeof(schedule_t));
    obj->week = NULL;
    obj->month = NULL;
    obj->day = NULL;
    obj->hour = NULL;
    obj->minute = NULL;
    schedule_init(obj);

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
void schedule_destroy(schedule_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return;

    schedule_init(obj);
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
int schedule_init(schedule_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    jhk_free(obj->week);
    jhk_free(obj->month);
    jhk_free(obj->day);
    jhk_free(obj->hour);
    jhk_free(obj->minute);

    obj->id = 0;
    obj->jobunit_id = 0;
    obj->proc_jobunit_id = 0;
    obj->hist_jobunit_id = 0;
    obj->week = NULL;
    obj->month = NULL;
    obj->day = NULL;
    obj->hour = NULL;
    obj->minute = NULL;

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
int schedule_load(schedule_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    schedule_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->jobunit_id = dbi_result_get_ulonglong(res, "jobunit_id");
    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");
    obj->hist_jobunit_id =
        dbi_result_get_ulonglong(res, "hist_jobunit_id");

    obj->week = jhkdb_get_string(res, "week");
    obj->month = jhkdb_get_string(res, "month");
    obj->day = jhkdb_get_string(res, "day");
    obj->hour = jhkdb_get_string(res, "hour");
    obj->minute = jhkdb_get_string(res, "minute");

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
apr_uint64_t proc_schedule_insert(schedule_t * obj)
{
    apr_uint64_t id;
    char *esc_week = NULL;
    char *esc_month = NULL;
    char *esc_day = NULL;
    char *esc_hour = NULL;
    char *esc_minute = NULL;


    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    // escape string
    id = 0;
    esc_week = jhkdb_escape_string(obj->week);
    esc_month = jhkdb_escape_string(obj->month);
    esc_day = jhkdb_escape_string(obj->day);
    esc_hour = jhkdb_escape_string(obj->hour);
    esc_minute = jhkdb_escape_string(obj->minute);

    id = jhkdb_insert
        ("INSERT INTO proc_schedules(proc_jobunit_id, week, month, day, hour, minute) VALUES(%llu, '%s', '%s', '%s', '%s', '%s')",
         obj->proc_jobunit_id, esc_week, esc_month, esc_day, esc_hour,
         esc_minute);
    obj->id = id;

    jhk_free(esc_week);
    jhk_free(esc_month);
    jhk_free(esc_day);
    jhk_free(esc_hour);
    jhk_free(esc_minute);

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
apr_uint64_t hist_schedule_insert(schedule_t * obj)
{
    apr_uint64_t id;
    char *esc_week = NULL;
    char *esc_month = NULL;
    char *esc_day = NULL;
    char *esc_hour = NULL;
    char *esc_minute = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    // escape string
    id = 0;
    esc_week = jhkdb_escape_string(obj->week);
    esc_month = jhkdb_escape_string(obj->month);
    esc_day = jhkdb_escape_string(obj->day);
    esc_hour = jhkdb_escape_string(obj->hour);
    esc_minute = jhkdb_escape_string(obj->minute);

    id = jhkdb_insert
        ("INSERT INTO hist_schedules(hist_jobunit_id, week, month, day, hour, minute) VALUES(%llu, '%s', '%s', '%s', '%s', '%s')",
         obj->hist_jobunit_id, esc_week, esc_month, esc_day, esc_hour,
         esc_minute);
    obj->id = id;

    jhk_free(esc_week);
    jhk_free(esc_month);
    jhk_free(esc_day);
    jhk_free(esc_hour);
    jhk_free(esc_minute);

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
int schedules_put_process(const apr_uint64_t jobunit_id,
                          const apr_uint64_t proc_jobunit_id)
{
    int rc;
    schedule_t *obj = NULL;
    dbi_result result = NULL;

    jhklog_debug("In %s() jobunit_id: %llu, proc_jobunit_id: %llu",
                 __func__, jobunit_id, proc_jobunit_id);

    result =
        jhkdb_select("SELECT * FROM schedules WHERE jobunit_id = %llu",
                     jobunit_id);
    if (result == NULL)
        return -1;

    rc = -1;
    obj = schedule_new();
    while (dbi_result_next_row(result)) {
        schedule_load(obj, result);
        obj->proc_jobunit_id = proc_jobunit_id;
        if (proc_schedule_insert(obj) == 0)
            goto error;
    }

    rc = 0;
  error:
    schedule_destroy(obj);
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
int schedules_put_history(const apr_uint64_t proc_jobunit_id,
                          const apr_uint64_t hist_jobunit_id)
{
    int rc;
    schedule_t *obj = NULL;
    dbi_result result = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu, hist_jobunit_id: %llu",
                 __func__, proc_jobunit_id, hist_jobunit_id);

    result =
        jhkdb_select
        ("SELECT * FROM proc_schedules WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    rc = -1;
    obj = schedule_new();
    while (dbi_result_next_row(result)) {
        schedule_load(obj, result);
        obj->hist_jobunit_id = hist_jobunit_id;
        if (hist_schedule_insert(obj) == 0)
            goto error;
    }

    rc = 0;
  error:
    schedule_destroy(obj);
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
int proc_schedules_delete(const apr_uint64_t proc_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM proc_schedules WHERE proc_jobunit_id = %llu",
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
int hist_schedules_delete(const apr_uint64_t hist_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM hist_schedules WHERE hist_jobunit_id = %llu",
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
int schedule_on_time(schedule_t * obj, const time_t t)
{
    int rv;
    struct tm *tm = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || t == 0)
        return JHAKO_ERROR;

    jhklog_trace("In %s() schedule_id: %llu, time: %ld", __func__,
                 obj->id, t);
    tm = localtime(&t);

    // check week
    jhklog_trace("In %s() week: %s, tw_wday: %d", __func__,
                 obj->week, tm->tm_wday);
    rv = jhk_on_cron(tm->tm_wday, obj->week);
    if (rv != JHAKO_TRUE)
        return rv;

    // check month
    jhklog_trace("In %s() month: %s, tm_mon: %d", __func__,
                 obj->month, tm->tm_mon + 1);
    rv = jhk_on_cron(tm->tm_mon + 1, obj->month);
    if (rv != JHAKO_TRUE)
        return rv;

    // check day
    jhklog_trace("In %s() day: %s, tm_mday: %d", __func__, obj->day,
                 tm->tm_mday);
    rv = jhk_on_cron(tm->tm_mday, obj->day);
    if (rv != JHAKO_TRUE)
        return rv;

    // check hour
    jhklog_trace("In %s() hour: %s, tm_hour: %d", __func__,
                 obj->hour, tm->tm_hour);
    rv = jhk_on_cron(tm->tm_hour, obj->hour);
    if (rv != JHAKO_TRUE)
        return rv;

    // check minute
    jhklog_trace("In %s() minute: %s, tm_min: %d", __func__,
                 obj->minute, tm->tm_min);
    rv = jhk_on_cron(tm->tm_min, obj->minute);
    if (rv != JHAKO_TRUE)
        return rv;

    // matched
    return JHAKO_TRUE;
}
