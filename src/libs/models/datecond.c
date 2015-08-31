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
datecond_t *datecond_new(void)
{
    datecond_t *obj;

    jhklog_trace("In %s()", __func__);

    obj = (datecond_t *) malloc(sizeof(datecond_t));
    datecond_init(obj);

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
void datecond_destroy(datecond_t * obj)
{
    jhklog_trace("In %s()", __func__);

    if (obj == NULL)
        return;

    datecond_init(obj);
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
int datecond_init(datecond_t * obj)
{
    jhklog_trace("In %s()", __func__);

    if (obj == NULL)
        return -1;

    obj->id = 0;
    obj->jobunit_id = 0;
    obj->proc_jobunit_id = 0;
    obj->hist_jobunit_id = 0;
    obj->kind = 0;
    obj->cond = 0;
    obj->calendar_id = 0;
    obj->negative = 0;

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
int datecond_load(datecond_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    datecond_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->jobunit_id = dbi_result_get_ulonglong(res, "jobunit_id");
    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");
    obj->hist_jobunit_id =
        dbi_result_get_ulonglong(res, "hist_jobunit_id");

    obj->kind = dbi_result_get_int(res, "kind");
    obj->cond = dbi_result_get_int(res, "cond");
    obj->calendar_id = dbi_result_get_ulonglong(res, "calendar_id");
    obj->negative = dbi_result_get_int(res, "negative");

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
apr_uint64_t proc_datecond_insert(datecond_t * obj)
{
    apr_uint64_t id;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    id = jhkdb_insert
        ("INSERT INTO proc_dateconds(proc_jobunit_id, kind, cond, calendar_id, negative) VALUES(%llu, %d, %d, %llu, %d)",
         obj->proc_jobunit_id, obj->kind, obj->cond, obj->calendar_id,
         obj->negative);
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
apr_uint64_t hist_datecond_insert(datecond_t * obj)
{
    apr_uint64_t id;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    id = jhkdb_insert
        ("INSERT INTO hist_dateconds(hist_jobunit_id, kind, cond, calendar_id, negative) VALUES(%llu, %d, %d, '%s', %d)",
         obj->hist_jobunit_id, obj->kind, obj->cond, obj->calendar_id,
         obj->negative);
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
int dateconds_put_process(const apr_uint64_t jobunit_id,
                          const apr_uint64_t proc_jobunit_id)
{
    int rc;
    datecond_t *obj;
    dbi_result result;

    jhklog_debug("In %s() jobunit_id: %llu, proc_jobunit_id: %llu",
                 __func__, jobunit_id, proc_jobunit_id);

    result =
        jhkdb_select("SELECT * FROM dateconds WHERE jobunit_id = %llu",
                     jobunit_id);
    if (result == NULL)
        return -1;

    rc = -1;
    obj = datecond_new();
    while (dbi_result_next_row(result)) {
        datecond_load(obj, result);
        obj->proc_jobunit_id = proc_jobunit_id;
        if (proc_datecond_insert(obj) == 0)
            goto error;
    }

    rc = 0;
  error:
    datecond_destroy(obj);
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
int dateconds_put_history(const apr_uint64_t proc_jobunit_id,
                          const apr_uint64_t hist_jobunit_id)
{
    int rc;
    datecond_t *obj;
    dbi_result result;

    jhklog_debug("In %s() proc_jobunit_id: %llu, hist_jobunit_id: %llu",
                 __func__, proc_jobunit_id, hist_jobunit_id);

    result =
        jhkdb_select
        ("SELECT * FROM proc_dateconds WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    rc = -1;
    obj = datecond_new();
    while (dbi_result_next_row(result)) {
        datecond_load(obj, result);
        obj->hist_jobunit_id = hist_jobunit_id;
        if (hist_datecond_insert(obj) == 0)
            goto error;
    }

    rc = 0;
  error:
    datecond_destroy(obj);
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
int proc_dateconds_delete(const apr_uint64_t proc_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM proc_dateconds WHERE proc_jobunit_id = %llu",
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
int hist_dateconds_delete(const apr_uint64_t hist_jobunit_id)
{
    jhklog_trace("In %s()", __func__);
    return
        jhkdb_execute
        ("DELETE FROM hist_dateconds WHERE hist_jobunit_id = %llu",
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
int datecond_execute(datecond_t * obj, const time_t base_time,
                     const int beginning_of_week)
{
    int rv;
    struct tm *tm = NULL;
    int week_no, mday;
    time_t reverse;
    calendar_t *obj_calendar;

    if (obj == NULL)
        return JHAKO_ERROR;
    jhklog_trace("In %s() base_time: %ld, beginning_of_week: %d", __func__,
                 base_time, beginning_of_week);

    // init
    obj_calendar = calendar_new();

    // cals week No.
    tm = localtime(&base_time);
    week_no =
        (int) ((tm->tm_mday - 1 +
                (beginning_of_week + tm->tm_wday + 6) % 7) / 7) + 1;
    jhklog_debug("In %s() id: %llu, week_no: %d", __func__, obj->id,
                 week_no);

    rv = JHAKO_FALSE;
    switch (obj->kind) {
    case DATECOND_KIND_EVERY_WEEK:
        if (obj->cond == tm->tm_wday) {
            execlog_info(obj->proc_jobunit_id,
                         "Date condition is matched. every week: %d",
                         obj->cond);
            rv = JHAKO_TRUE;
        }
        break;
    case DATECOND_KIND_FIRST_WEEK:
        if (obj->cond == tm->tm_wday && week_no == 1) {
            execlog_info(obj->proc_jobunit_id,
                         "Date condition is matched. the first week: %d",
                         obj->cond);
            rv = JHAKO_TRUE;
        }
        break;
    case DATECOND_KIND_SECOND_WEEK:
        if (obj->cond == tm->tm_wday && week_no == 2) {
            execlog_info(obj->proc_jobunit_id,
                         "Date condition is matched. the second week: %d",
                         obj->cond);
            rv = JHAKO_TRUE;
        }
        break;
    case DATECOND_KIND_THIRD_WEEK:
        if (obj->cond == tm->tm_wday && week_no == 3) {
            execlog_info(obj->proc_jobunit_id,
                         "Date condition is matched. the third week: %d",
                         obj->cond);
            rv = JHAKO_TRUE;
        }
        break;
    case DATECOND_KIND_FOURTH_WEEK:
        if (obj->cond == tm->tm_wday && week_no == 4) {
            execlog_info(obj->proc_jobunit_id,
                         "Date condition is matched. the fourth week: %d",
                         obj->cond);
            rv = JHAKO_TRUE;
        }
        break;
    case DATECOND_KIND_FIFTH_WEEK:
        if (obj->cond == tm->tm_wday && week_no == 5) {
            execlog_info(obj->proc_jobunit_id,
                         "Date condition is matched. the fifth week: %d",
                         obj->cond);
            rv = JHAKO_TRUE;
        }
        break;
    case DATECOND_KIND_EVERY_MONTH:
        if (obj->cond == tm->tm_mday) {
            execlog_info(obj->proc_jobunit_id,
                         "Date condition is matched. month: %d",
                         obj->cond);
            rv = JHAKO_TRUE;
        }
        break;
    case DATECOND_KIND_REVERSE_MONTH:
        mday = tm->tm_mday;
        tm->tm_mon++;
        tm->tm_mday = 1;
        reverse = mktime(tm) - obj->cond * 60 * 60 * 24;
        tm = localtime(&reverse);
        if (mday == tm->tm_mday) {
            execlog_info(obj->proc_jobunit_id,
                         "Date condition is matched. reverse month: %d",
                         obj->cond);
            rv = JHAKO_TRUE;
        }
        break;
    case DATECOND_KIND_CALENDAR:
        if (calendar_select(obj_calendar, obj->calendar_id) != 0) {
            execlog_error(obj->proc_jobunit_id,
                          "Can not find the calendar");
            rv = JHAKO_ERROR;
            break;
        }
        rv = calendar_date_ondate(obj->calendar_id, base_time);
        if (rv == JHAKO_TRUE) {
            execlog_info(obj->proc_jobunit_id,
                         "Date condition is matched. calendar: %s",
                         obj_calendar->name);
        }
        break;
    default:
        jhklog_error("In %s() unknown date condition kind. %d",
                     __func__, obj->kind);
        rv = JHAKO_ERROR;
    }

    if (rv == JHAKO_TRUE) {
        rv = (obj->negative) ? JHAKO_FALSE : JHAKO_TRUE;
    } else if (rv == JHAKO_FALSE) {
        rv = (obj->negative) ? JHAKO_TRUE : JHAKO_FALSE;
    }

    calendar_destroy(obj_calendar);
    return rv;
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
int dateconds_execute(const apr_uint64_t proc_jobunit_id,
                      const time_t base_time, const int beginning_of_week)
{
    int rc, rv;
    dbi_result result;
    datecond_t *obj = NULL;
    struct tm *tm = NULL;

    jhklog_trace
        ("In %s() proc_jobunit_id: %llu, base_time: %ld, beginning_of_week: %d",
         __func__, proc_jobunit_id, base_time, beginning_of_week);

    result =
        jhkdb_select
        ("SELECT * FROM proc_dateconds WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    // unset date conditions
    if (dbi_result_get_numrows(result) == 0) {
        dbi_result_free(result);
        return CONNECTOR_KIND_NORMAL;
    }
    // init
    rc = -1;
    obj = datecond_new();

    // date condition
    while (dbi_result_next_row(result)) {
        datecond_load(obj, result);
        rv = datecond_execute(obj, base_time, beginning_of_week);
        if (rv == JHAKO_FALSE)
            continue;

        if (rv == JHAKO_TRUE) {
            rc = CONNECTOR_KIND_NORMAL;
        } else {
            rc = CONNECTOR_KIND_ERROR;
        }
        goto finish;
    }

    rc = CONNECTOR_KIND_BRANCH;
  finish:
    datecond_destroy(obj);
    dbi_result_free(result);
    return rc;
}
