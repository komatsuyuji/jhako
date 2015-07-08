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
jobunit_t *jobunit_new(void)
{
    jobunit_t *obj;

    jhklog_trace("In %s()", __func__);

    obj = (jobunit_t *) malloc(sizeof(jobunit_t));
    obj->name = NULL;
    obj->description = NULL;
    obj->path = NULL;

    jobunit_init(obj);

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
void jobunit_destroy(jobunit_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return;

    jobunit_init(obj);
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
int jobunit_init(jobunit_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    jhk_free(obj->name);
    jhk_free(obj->description);
    jhk_free(obj->path);

    obj->id = 0;
    obj->name = NULL;
    obj->description = NULL;
    obj->kind = 0;
    obj->parent_id = 0;
    obj->x = 0;
    obj->y = 0;
    obj->hold = 0;
    obj->skip = 0;
    obj->timeout = 0;
    obj->created_at = 0;
    obj->updated_at = 0;

    obj->jobunit_id = 0;
    obj->status = 0;
    obj->start_time = 0;
    obj->end_time = 0;
    obj->timeout_occur = 0;
    obj->force_start = 0;
    obj->force_stop = 0;

    obj->boot = 0;
    obj->finish = 0;
    obj->pid = 0;

    obj->proc_jobunit_id = 0;
    obj->mode = 0;
    obj->schedule_time = 0;
    obj->run_type = 0;
    obj->delay_limit = 1;

    obj->path = NULL;

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
int jobunit_load(jobunit_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);

    if (obj == NULL || res == NULL)
        return -1;

    jobunit_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->name = jhkdb_get_string(res, "name");
    obj->description = jhkdb_get_string(res, "description");
    obj->kind = dbi_result_get_int(res, "kind");
    obj->parent_id = dbi_result_get_ulonglong(res, "parent_id");
    obj->x = dbi_result_get_int(res, "x");
    obj->y = dbi_result_get_int(res, "y");
    obj->hold = dbi_result_get_int(res, "hold");
    obj->skip = dbi_result_get_int(res, "skip");
    obj->timeout = dbi_result_get_int(res, "timeout");
    obj->created_at = jhkdb_get_datetime(res, "created_at");
    obj->updated_at = jhkdb_get_datetime(res, "updated_at");

    obj->jobunit_id = dbi_result_get_ulonglong(res, "jobunit_id");
    obj->status = dbi_result_get_int(res, "status");
    obj->start_time = jhkdb_get_datetime(res, "start_time");
    obj->end_time = jhkdb_get_datetime(res, "end_time");
    obj->timeout_occur = dbi_result_get_int(res, "timeout_occur");
    obj->force_start = dbi_result_get_int(res, "force_start");
    obj->force_stop = dbi_result_get_int(res, "force_stop");

    obj->boot = dbi_result_get_int(res, "boot");
    obj->finish = dbi_result_get_int(res, "finish");
    obj->pid = (pid_t) dbi_result_get_int(res, "pid");

    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");
    obj->mode = dbi_result_get_int(res, "mode");
    obj->schedule_time = jhkdb_get_datetime(res, "schedule_time");
    obj->run_type = dbi_result_get_int(res, "run_type");
    obj->delay_limit = dbi_result_get_int(res, "delay_limit");

    obj->path = jhkdb_get_string(res, "path");

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
int jobunit_load_one(jobunit_t * obj, dbi_result res)
{
    apr_uint64_t num;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    num = dbi_result_get_numrows(res);
    if (num != 1) {
        jhklog_warn("In %s() can not find jobunit. numrows: %llu",
                    __func__, num);
        return 1;
    }

    while (dbi_result_next_row(res)) {
        jobunit_load(obj, res);
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
int jobunit_select(jobunit_t * obj, const apr_uint64_t id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_trace("In %s() id: %llu", __func__, id);

    result = jhkdb_select("SELECT * FROM jobunits WHERE id = %llu", id);
    if (result == NULL)
        return -1;

    rc = jobunit_load_one(obj, result);

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
int proc_jobunit_select(jobunit_t * obj, const apr_uint64_t id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_trace("In %s() id: %llu", __func__, id);
    result =
        jhkdb_select
        ("SELECT * FROM proc_jobunits WHERE id = %llu FOR UPDATE", id);
    if (result == NULL)
        return -1;

    rc = jobunit_load_one(obj, result);

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
int proc_topjobnet_select(jobunit_t * obj, const apr_uint64_t id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_trace("In %s() id: %llu", __func__, id);
    result =
        jhkdb_select
        ("SELECT * FROM proc_topjobnets WHERE id = %llu FOR UPDATE", id);
    if (result == NULL)
        return -1;

    rc = jobunit_load_one(obj, result);

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
apr_uint64_t proc_topjobnet_insert(jobunit_t * obj)
{
    apr_uint64_t id;
    dbi_result result = NULL;
    apr_uint64_t num;
    char *esc_name = NULL;
    char *esc_description = NULL;
    char *ts, *schedule_time;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    // escape string
    id = 0;
    esc_name = jhkdb_escape_string(obj->name);
    esc_description = jhkdb_escape_string(obj->description);
    ts = jhk_time();
    schedule_time = jhkdb_escape_datetime(obj->schedule_time);


    // check inserted data
    result =
        jhkdb_select
        ("SELECT id FROM proc_topjobnets WHERE jobunit_id = %llu AND schedule_time = %s AND mode = %d",
         obj->jobunit_id, schedule_time, TOPJOBNET_MODE_SCHEDULE);
    if (result == NULL)
        goto error;

    num = dbi_result_get_numrows(result);
    dbi_result_free(result);
    if (num > 0)
        goto error;

    id = jhkdb_insert("INSERT INTO proc_topjobnets(name, description, kind, hold, skip, timeout, created_at, updated_at, \
jobunit_id, proc_jobunit_id, mode, schedule_time, run_type, delay_limit) \
VALUES ('%s', '%s', %d, %d, %d, %d, '%s', '%s', %llu, %d, %d, %s, %d, %d)",
                      esc_name, esc_description, obj->kind, obj->hold, obj->skip, obj->timeout, ts, ts, obj->jobunit_id, 0, TOPJOBNET_MODE_SCHEDULE, schedule_time, obj->run_type, obj->delay_limit);

    if (id > 0)
        obj->id = id;

  error:
    jhk_free(schedule_time);
    jhk_free(esc_name);
    jhk_free(esc_description);
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
apr_uint64_t proc_jobunit_insert(jobunit_t * obj)
{
    apr_uint64_t id;
    char *esc_name = NULL;
    char *esc_description = NULL;
    char *ts;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    // escape string
    esc_name = jhkdb_escape_string(obj->name);
    esc_description = jhkdb_escape_string(obj->description);

    ts = jhk_time();
    id = jhkdb_insert
        ("INSERT INTO proc_jobunits(name, description, kind, parent_id, x, y, hold, skip, timeout, \
created_at, updated_at, jobunit_id, boot, path) \
VALUES('%s', '%s', %d, %llu, %d, %d, %d, %d, %d, '%s', '%s', %llu, %d, '%s')",
         esc_name, esc_description, obj->kind, obj->parent_id, obj->x, obj->y, obj->hold, obj->skip, obj->timeout, ts, ts, obj->jobunit_id, obj->boot, obj->path);
    obj->id = id;

    jhk_free(esc_name);
    jhk_free(esc_description);
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
apr_uint64_t hist_jobunit_insert(jobunit_t * obj)
{
    apr_uint64_t id;
    char *esc_name = NULL;
    char *esc_description = NULL;
    char *ts, *schedule_time, *start_time, *end_time;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    // escape string
    esc_name = jhkdb_escape_string(obj->name);
    esc_description = jhkdb_escape_string(obj->description);

    ts = jhk_time();
    schedule_time = jhkdb_escape_datetime(obj->schedule_time);
    start_time = jhkdb_escape_datetime(obj->start_time);
    end_time = jhkdb_escape_datetime(obj->end_time);

    id = jhkdb_insert
        ("INSERT INTO hist_jobunits(name, description, kind, parent_id, x, y, hold, skip, timeout, \
created_at, updated_at, jobunit_id, status, start_time, end_time, timeout_occur, mode, schedule_time, path) \
VALUES('%s', '%s', %d, %llu, %d, %d, %d, %d, %d, '%s', '%s', %llu, %d, %s, %s, %d, %d, %s, '%s')",
         esc_name, esc_description, obj->kind, obj->parent_id, obj->x, obj->y, obj->hold, obj->skip, obj->timeout, ts, ts, obj->jobunit_id, obj->status, start_time, end_time, obj->timeout_occur, obj->mode, schedule_time, obj->path);
    obj->id = id;

    jhk_free(schedule_time);
    jhk_free(start_time);
    jhk_free(end_time);
    jhk_free(esc_name);
    jhk_free(esc_description);
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
int proc_topjobnet_delete(const apr_uint64_t id)
{
    jhklog_trace("In %s()", __func__);

    return jhkdb_execute("DELETE FROM proc_topjobnets WHERE id = %llu",
                         id);
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
int proc_jobunit_delete(const apr_uint64_t id)
{
    jhklog_trace("In %s()", __func__);

    return jhkdb_execute("DELETE FROM proc_jobunits WHERE id = %llu", id);
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
int hist_jobunit_delete(const apr_uint64_t id)
{
    jhklog_trace("In %s()", __func__);

    return jhkdb_execute("DELETE FROM hist_jobunits WHERE id = %llu", id);
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
int proc_jobunit_delete_parent_id(const apr_uint64_t parent_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute("DELETE FROM proc_jobunits WHERE parent_id = %llu",
                      parent_id);
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
int jobunit_get_path(const apr_uint64_t id, char *path, const apr_size_t s,
                     const int parent)
{
    int rc;
    jobunit_t *obj;

    jhklog_trace("In %s() id: %llu, size: %d, parent: %d", __func__, id, s,
                 parent);
    if (path == NULL || s <= 0)
        return -1;

    // root path
    if (id == 0) {
        snprintf(path, s, "");
        return 0;
    }

    rc = -1;
    obj = jobunit_new();

    if (jobunit_select(obj, id) != 0)
        goto error;

    if (jobunit_get_path(obj->parent_id, path, s, 0) != 0)
        goto error;

    if (parent == 0) {
        jhk_strncat(path, "/", s);
        jhk_strncat(path, obj->name, s);
    }

    rc = 0;
  error:
    jobunit_destroy(obj);
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
time_t proc_jobunit_get_start_time(const apr_uint64_t id)
{
    time_t start_time;
    dbi_result result = NULL;

    jhklog_trace("In %s() id: %llu", __func__, id);

    start_time = 0;
    result =
        jhkdb_select
        ("SELECT start_time FROM proc_jobunits WHERE id = %llu", id);
    if (result == NULL)
        return 0;

    while (dbi_result_next_row(result)) {
        start_time = jhkdb_get_datetime(result, "start_time");
        break;
    }

    dbi_result_free(result);
    return start_time;
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
time_t proc_jobunit_get_rootjobnet_starttime(const apr_uint64_t id)
{
    apr_uint64_t parent_id;
    time_t start_time;
    dbi_result result = NULL;

    jhklog_trace("In %s() id: %llu", __func__, id);

    start_time = 0;
    result =
        jhkdb_select
        ("SELECT start_time, parent_id FROM proc_jobunits WHERE id = %llu",
         id);
    if (result == NULL)
        return 0;

    while (dbi_result_next_row(result)) {
        start_time = jhkdb_get_datetime(result, "start_time");
        parent_id = dbi_result_get_ulonglong(result, "parent_id");
        break;
    }
    dbi_result_free(result);

    if (parent_id == 0)
        return start_time;

    return proc_jobunit_get_rootjobnet_starttime(parent_id);
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
int proc_jobunit_timeout_occur(jobunit_t * obj)
{
    int rc_db;

    jhklog_trace("In %s() id: %llu", __func__, obj->id);
    execlog_warn(obj->id, "The job '%s' is timeout", obj->name);

    rc_db = jhkdb_execute
        ("UPDATE proc_jobunits SET timeout_occur = true WHERE id = %llu AND status = %d AND timeout_occur = false",
         obj->id, JOBUNIT_STATUS_RUNNING);
    if (rc_db != 0)
        return -1;

    // joblog
    obj->timeout_occur = 1;
    jhkjoblog_jobunit(obj);

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
int proc_jobunit_set_running(jobunit_t * obj)
{
    int rc_db;
    char *ts;

    jhklog_trace("In %s() id: %llu", __func__, obj->id);
    execlog_info(obj->id, "The job '%s' started", obj->name);

    // update status
    ts = jhk_time();
    rc_db = jhkdb_execute
        ("UPDATE proc_jobunits SET status = %d, start_time = '%s', end_time = NULL, updated_at = '%s' WHERE id = %llu",
         JOBUNIT_STATUS_RUNNING, ts, ts, obj->id);
    if (rc_db != 0)
        return -1;

    // joblog
    obj->status = JOBUNIT_STATUS_RUNNING;
    jhkjoblog_jobunit(obj);

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
int proc_jobunit_set_error(jobunit_t * obj)
{
    int rc_db;
    char *ts;

    jhklog_trace("In %s() id: %llu", __func__, obj->id);
    execlog_error(obj->id, "The job '%s' is error", obj->name);

    // update status
    ts = jhk_time();
    rc_db = jhkdb_execute
        ("UPDATE proc_jobunits SET status = %d, start_time = '%s', end_time = '%s', updated_at = '%s' WHERE id = %llu",
         JOBUNIT_STATUS_ERROR, ts, ts, ts, obj->id);
    if (rc_db != 0)
        return -1;

    // joblog
    obj->status = JOBUNIT_STATUS_ERROR;
    jhkjoblog_jobunit(obj);

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
int proc_jobunit_set_end(jobunit_t * obj, const int conn_kind)
{
    int rc_db, rc_flowing;
    int status;
    char *ts;

    jhklog_trace("In %s() id: %llu", __func__, obj->id);
    execlog_info(obj->id, "The job '%s' is completed", obj->name);

    // init
    status = JOBUNIT_STATUS_END;

    // flow next job
    if (!(obj->parent_id == 0 || obj->kind == JOBUNIT_KIND_ENDJOB)) {
        rc_flowing = jobunit_flowing(obj, conn_kind);
        if (rc_flowing < 0) {
            return -1;
        } else if (rc_flowing == 0) {
            status = JOBUNIT_STATUS_ERROR;
        }
    }
    // update status
    ts = jhk_time();
    rc_db = jhkdb_execute
        ("UPDATE proc_jobunits SET status = %d, start_time = '%s', end_time = '%s', updated_at = '%s' WHERE id = %llu",
         status, ts, ts, ts, obj->id);
    if (rc_db != 0)
        return -1;

    // update topjobnet
    if (obj->parent_id == 0) {
        rc_db = jhkdb_execute
            ("UPDATE proc_topjobnets SET status = %d, end_time = '%s', updated_at = '%s' WHERE proc_jobunit_id = %llu",
             status, ts, ts, obj->id);
        if (rc_db != 0)
            return -1;
    }
    // joblog
    obj->status = status;
    jhkjoblog_jobunit(obj);

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
int proc_jobunit_set_success(jobunit_t * obj, const int conn_kind)
{
    int rc_db, rc_flowing;
    int status;
    char *ts;

    jhklog_trace("In %s() id: %llu", __func__, obj->id);
    execlog_info(obj->id, "The job '%s' successfully completed",
                 obj->name);

    // init
    status = JOBUNIT_STATUS_END;

    // flow next job
    rc_flowing = jobunit_flowing(obj, conn_kind);
    if (rc_flowing < 0) {
        return -1;
    } else if (rc_flowing == 0) {
        status = JOBUNIT_STATUS_ERROR;
    }
    // update status
    ts = jhk_time();
    rc_db = jhkdb_execute
        ("UPDATE proc_jobunits SET status = %d, end_time = '%s', updated_at = '%s' WHERE id = %llu",
         status, ts, ts, obj->id);
    if (rc_db != 0)
        return -1;

    // joblog
    obj->status = status;
    jhkjoblog_jobunit(obj);

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
int proc_jobunit_set_failure(jobunit_t * obj)
{
    int rc_db;
    char *ts;

    jhklog_trace("In %s() id: %llu", __func__, obj->id);
    execlog_error(obj->id, "The job '%s' failed", obj->name);

    // update status
    ts = jhk_time();
    rc_db = jhkdb_execute
        ("UPDATE proc_jobunits SET status = %d, end_time = '%s', updated_at = '%s' WHERE id = %llu",
         JOBUNIT_STATUS_ERROR, ts, ts, obj->id);
    if (rc_db != 0)
        return -1;

    // joblog
    obj->status = JOBUNIT_STATUS_ERROR;
    jhkjoblog_jobunit(obj);

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
int proc_jobunit_set_finish(jobunit_t * obj, const int conn_kind)
{
    jhklog_trace("In %s() id: %llu", __func__, obj->id);

    if (conn_kind == CONNECTOR_KIND_NORMAL
        || conn_kind == CONNECTOR_KIND_BRANCH) {
        return proc_jobunit_set_success(obj, conn_kind);
    }

    return proc_jobunit_set_failure(obj);
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
int jobunit_flowing(jobunit_t * obj, const int kind)
{
    int rc, rc_db;
    int num;
    connector_t *obj_conn;
    jobunit_t *obj_next_job;
    dbi_result result = NULL;

    jhklog_trace("In %s() id: %llu, kind: %d", __func__, obj->id, kind);

    // search all connectors of jobunit
    result =
        jhkdb_select
        ("SELECT * FROM proc_connectors WHERE prev_jobid = %llu AND kind = %d",
         obj->id, kind);
    if (result == NULL)
        return -1;

    rc = -1;
    num = 0;
    obj_conn = connector_new();
    obj_next_job = jobunit_new();
    while (dbi_result_next_row(result)) {
        connector_load(obj_conn, result);
        if (proc_jobunit_select(obj_next_job, obj_conn->next_jobid) != 0)
            goto error;

        // calc finished connectors of next_job
        obj_next_job->finish++;
        if (obj_next_job->finish == obj_next_job->boot) {
            obj_next_job->status = JOBUNIT_STATUS_READY;
        } else if (obj_next_job->finish > obj_next_job->boot) {
            jhklog_warn
                ("In %s() connectors too much. next_jobid: %llu, finish: %d, boot: %d",
                 __func__, obj_next_job->id, obj_next_job->finish,
                 obj_next_job->boot);
            obj_next_job->finish = obj_next_job->boot;
        }
        // update next_job data
        rc_db =
            jhkdb_execute
            ("UPDATE proc_jobunits SET status = %d, finish = %d, updated_at = '%s' WHERE id = %llu",
             obj_next_job->status, obj_next_job->finish, jhk_time(),
             obj_next_job->id);
        if (rc_db != 0)
            goto error;
        num++;
    }

    if (num == 0) {
        execlog_error(obj->id, "Can not find next jobs. '%s'", obj->name);
    }

    rc = num;
  error:
    connector_destroy(obj_conn);
    jobunit_destroy(obj_next_job);
    dbi_result_free(result);
    return rc;
}
