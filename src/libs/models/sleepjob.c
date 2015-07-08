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
sleepjob_t *sleepjob_new(void)
{
    sleepjob_t *obj;

    jhklog_trace("In %s()", __func__);
    obj = (sleepjob_t *) malloc(sizeof(sleepjob_t));
    sleepjob_init(obj);

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
void sleepjob_destroy(sleepjob_t * obj)
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
int sleepjob_init(sleepjob_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    obj->id = 0;
    obj->jobunit_id = 0;
    obj->proc_jobunit_id = 0;
    obj->hist_jobunit_id = 0;
    obj->hour = 0;
    obj->minute = 0;
    obj->second = 0;

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
int sleepjob_load(sleepjob_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    sleepjob_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->jobunit_id = dbi_result_get_ulonglong(res, "jobunit_id");
    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");
    obj->hist_jobunit_id =
        dbi_result_get_ulonglong(res, "hist_jobunit_id");

    obj->hour = dbi_result_get_int(res, "hour");
    obj->minute = dbi_result_get_int(res, "minute");
    obj->second = dbi_result_get_int(res, "second");

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
int sleepjob_load_one(sleepjob_t * obj, dbi_result res)
{
    apr_uint64_t num;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    num = dbi_result_get_numrows(res);
    if (num != 1) {
        jhklog_warn("In %s() can not find sleepjob. numrows: %llu",
                    __func__, num);
        return 1;
    }

    while (dbi_result_next_row(res)) {
        sleepjob_load(obj, res);
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
int sleepjob_select(sleepjob_t * obj, const apr_uint64_t jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() jobunit_id: %llu", __func__, jobunit_id);

    result =
        jhkdb_select("SELECT * FROM sleepjobs WHERE jobunit_id = %llu",
                     jobunit_id);
    if (result == NULL)
        return -1;

    rc = sleepjob_load_one(obj, result);

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
int proc_sleepjob_select(sleepjob_t * obj,
                         const apr_uint64_t proc_jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu", __func__,
                 proc_jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM proc_sleepjobs WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    rc = sleepjob_load_one(obj, result);

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
int hist_sleepjob_select(sleepjob_t * obj,
                         const apr_uint64_t hist_jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() hist_jobunit_id: %llu", __func__,
                 hist_jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM hist_sleepjobs WHERE hist_jobunit_id = %llu",
         hist_jobunit_id);
    if (result == NULL)
        return -1;

    rc = sleepjob_load_one(obj, result);

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
apr_uint64_t proc_sleepjob_insert(sleepjob_t * obj)
{
    apr_uint64_t id;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    id = jhkdb_insert
        ("INSERT INTO proc_sleepjobs(proc_jobunit_id, hour, minute, second) VALUES(%llu, %d, %d, %d)",
         obj->proc_jobunit_id, obj->hour, obj->minute, obj->second);
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
apr_uint64_t hist_sleepjob_insert(sleepjob_t * obj)
{
    apr_uint64_t id;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    id = jhkdb_insert
        ("INSERT INTO hist_sleepjobs(hist_jobunit_id, hour, minute, second) VALUES(%llu, %d, %d, %d)",
         obj->hist_jobunit_id, obj->hour, obj->minute, obj->second);
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
int sleepjob_put_process(const apr_uint64_t jobunit_id,
                         const apr_uint64_t proc_jobunit_id)
{
    int rc;
    sleepjob_t *obj = NULL;

    jhklog_debug("In %s() jobunit_id: %llu, proc_jobunit_id: %llu",
                 __func__, jobunit_id, proc_jobunit_id);
    rc = -1;
    obj = sleepjob_new();

    if (sleepjob_select(obj, jobunit_id) != 0)
        goto error;

    obj->proc_jobunit_id = proc_jobunit_id;
    if (proc_sleepjob_insert(obj) == 0)
        goto error;

    rc = 0;
  error:
    sleepjob_destroy(obj);
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
int sleepjob_put_history(const apr_uint64_t proc_jobunit_id,
                         const apr_uint64_t hist_jobunit_id)
{
    int rc;
    sleepjob_t *obj = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu, hist_jobunit_id: %llu",
                 __func__, proc_jobunit_id, hist_jobunit_id);
    rc = -1;
    obj = sleepjob_new();

    if (proc_sleepjob_select(obj, proc_jobunit_id) != 0)
        goto error;

    obj->hist_jobunit_id = hist_jobunit_id;
    if (hist_sleepjob_insert(obj) == 0)
        goto error;

    rc = 0;
  error:
    sleepjob_destroy(obj);
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
int proc_sleepjob_delete(const apr_uint64_t proc_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM proc_sleepjobs WHERE proc_jobunit_id = %llu",
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
int hist_sleepjob_delete(const apr_uint64_t hist_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM hist_sleepjobs WHERE hist_jobunit_id = %llu",
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
int sleepjob_execute(jobunit_t * proc_jobunit)
{
    jhklog_trace("In %s()", __func__);

    return proc_jobunit_set_running(proc_jobunit);
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
int sleepjob_monitor(jobunit_t * proc_jobunit)
{
    int rc;
    time_t now, sec;
    sleepjob_t *obj = NULL;

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
    obj = sleepjob_new();

    if (proc_sleepjob_select(obj, proc_jobunit->id) != 0)
        goto error;
    jhklog_debug("In %s() hour: %d, minute: %d, second: %d",
                 __func__, obj->hour, obj->minute, obj->second);

    sec = obj->hour * 60 * 60 + obj->minute * 60 + obj->second;
    now = time(NULL);
    if (now >= (proc_jobunit->start_time + sec)) {
        if (proc_jobunit_set_finish(proc_jobunit, CONNECTOR_KIND_NORMAL) !=
            0)
            goto error;
    }

    rc = 0;
  error:
    sleepjob_destroy(obj);
    return rc;
}
