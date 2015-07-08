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
datejob_t *datejob_new(void)
{
    datejob_t *obj;

    jhklog_trace("In %s()", __func__);
    obj = (datejob_t *) malloc(sizeof(datejob_t));
    datejob_init(obj);

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
void datejob_destroy(datejob_t * obj)
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
int datejob_init(datejob_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    obj->id = 0;
    obj->jobunit_id = 0;
    obj->proc_jobunit_id = 0;
    obj->hist_jobunit_id = 0;
    obj->baseday = 0;
    obj->beginning_of_week = 0;

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
int datejob_load(datejob_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    datejob_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->jobunit_id = dbi_result_get_ulonglong(res, "jobunit_id");
    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");
    obj->hist_jobunit_id =
        dbi_result_get_ulonglong(res, "hist_jobunit_id");

    obj->baseday = dbi_result_get_int(res, "baseday");
    obj->beginning_of_week = dbi_result_get_int(res, "beginning_of_week");

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
int datejob_load_one(datejob_t * obj, dbi_result res)
{
    apr_uint64_t num;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    num = dbi_result_get_numrows(res);
    if (num != 1) {
        jhklog_warn("In %s() can not find datejob. numrows: %llu",
                    __func__, num);
        return 1;
    }

    while (dbi_result_next_row(res)) {
        datejob_load(obj, res);
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
int datejob_select(datejob_t * obj, const apr_uint64_t jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() jobunit_id: %llu", __func__, jobunit_id);

    result =
        jhkdb_select("SELECT * FROM datejobs WHERE jobunit_id = %llu",
                     jobunit_id);
    if (result == NULL)
        return -1;

    rc = datejob_load_one(obj, result);

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
int proc_datejob_select(datejob_t * obj,
                        const apr_uint64_t proc_jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu", __func__,
                 proc_jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM proc_datejobs WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    rc = datejob_load_one(obj, result);

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
int hist_datejob_select(datejob_t * obj,
                        const apr_uint64_t hist_jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() hist_jobunit_id: %llu", __func__,
                 hist_jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM hist_datejobs WHERE hist_jobunit_id = %llu",
         hist_jobunit_id);
    if (result == NULL)
        return -1;

    rc = datejob_load_one(obj, result);

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
apr_uint64_t proc_datejob_insert(datejob_t * obj)
{
    apr_uint64_t id;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    id = jhkdb_insert
        ("INSERT INTO proc_datejobs(proc_jobunit_id, baseday, beginning_of_week) VALUES(%llu, %d, %d)",
         obj->proc_jobunit_id, obj->baseday, obj->beginning_of_week);
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
apr_uint64_t hist_datejob_insert(datejob_t * obj)
{
    apr_uint64_t id;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    id = jhkdb_insert
        ("INSERT INTO hist_datejobs(hist_jobunit_id, baseday, beginning_of_week) VALUES(%llu, %d, %d)",
         obj->hist_jobunit_id, obj->baseday, obj->beginning_of_week);
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
int datejob_put_process(const apr_uint64_t jobunit_id,
                        const apr_uint64_t proc_jobunit_id)
{
    int rc;
    datejob_t *obj = NULL;

    jhklog_debug("In %s() jobunit_id: %llu, proc_jobunit_id: %llu",
                 __func__, jobunit_id, proc_jobunit_id);
    rc = -1;
    obj = datejob_new();

    if (datejob_select(obj, jobunit_id) != 0)
        goto error;

    obj->proc_jobunit_id = proc_jobunit_id;
    if (proc_datejob_insert(obj) == 0)
        goto error;

    rc = 0;
  error:
    datejob_destroy(obj);
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
int datejob_put_history(const apr_uint64_t proc_jobunit_id,
                        const apr_uint64_t hist_jobunit_id)
{
    int rc;
    datejob_t *obj = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu, hist_jobunit_id: %llu",
                 __func__, proc_jobunit_id, hist_jobunit_id);
    rc = -1;
    obj = datejob_new();

    if (proc_datejob_select(obj, proc_jobunit_id) != 0)
        goto error;

    obj->hist_jobunit_id = hist_jobunit_id;
    if (hist_datejob_insert(obj) == 0)
        goto error;

    rc = 0;
  error:
    datejob_destroy(obj);
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
int proc_datejob_delete(const apr_uint64_t proc_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM proc_datejobs WHERE proc_jobunit_id = %llu",
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
int hist_datejob_delete(const apr_uint64_t hist_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM hist_datejobs WHERE hist_jobunit_id = %llu",
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
int datejob_execute(jobunit_t * proc_jobunit)
{
    int rc, rc_dc;
    datejob_t *obj = NULL;
    time_t base_time;

    if (proc_jobunit == NULL)
        return -1;

    jhklog_trace("In %s() proc_jobunit_id: %llu, parent_id: %llu",
                 __func__, proc_jobunit->id, proc_jobunit->parent_id);
    // init
    rc = -1;
    obj = datejob_new();

    // load
    if (proc_datejob_select(obj, proc_jobunit->id) != 0)
        goto error;

    switch (obj->baseday) {
    case DATEJOB_BASEDAY_JOB:
        base_time = time(NULL);
        break;
    case DATEJOB_BASEDAY_JOBNET:
        base_time = proc_jobunit_get_start_time(proc_jobunit->parent_id);
        break;
    case DATEJOB_BASEDAY_ROOTJOBNET:
        base_time =
            proc_jobunit_get_rootjobnet_starttime(proc_jobunit->parent_id);
        break;
    default:
        base_time = time(NULL);
        break;
    }

    jhklog_debug
        ("In %s() proc_jobunit_id: %llu, base_time: %ld", __func__,
         proc_jobunit->id, base_time);

    rc_dc =
        dateconds_execute(proc_jobunit->id, base_time,
                          obj->beginning_of_week);

    if (rc_dc == CONNECTOR_KIND_NORMAL || rc_dc == CONNECTOR_KIND_BRANCH) {
        rc = proc_jobunit_set_end(proc_jobunit, rc_dc);
    } else if (rc_dc == CONNECTOR_KIND_ERROR) {
        rc = proc_jobunit_set_error(proc_jobunit);
    }
  error:
    datejob_destroy(obj);
    return rc;
}
