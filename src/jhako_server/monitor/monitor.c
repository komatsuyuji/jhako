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
#include "monitor.h"

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
void monitor_mainloop(void)
{
    pid_t ppid;

    jhklog_info("monitor start");

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
        monitor_exec();
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
int monitor_exec(void)
{
    jhklog_trace("In %s()", __func__);

    if (monitor_topjobnets() != 0)
        return -1;

    if (monitor_jobnets() != 0)
        return -1;

    if (monitor_jobs() != 0)
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
int monitor_topjobnets(void)
{
    int rc_db;
    apr_uint64_t id, proc_jobunit_id;
    dbi_result result = NULL;
    char *ts;

    jhklog_trace("In %s()", __func__);

    ts = jhk_time();

    // force stop topjobnet. status: ERROR
    jhkdb_begin();
    rc_db =
        jhkdb_execute
        ("UPDATE proc_topjobnets SET status = %d, force_stop = false, end_time = '%s', updated_at = '%s' WHERE status = %d AND force_stop = true",
         JOBUNIT_STATUS_FORCESTOP, ts, ts, JOBUNIT_STATUS_ERROR);
    jhkdb_commit();
    if (rc_db != 0) {
        return -1;
    }
    // force stop topjobnet. status: RUNNING
    result =
        jhkdb_select
        ("SELECT id, proc_jobunit_id FROM proc_topjobnets WHERE status = %d AND force_stop = true",
         JOBUNIT_STATUS_RUNNING);
    if (result == NULL) {
        return -1;
    }

    while (dbi_result_next_row(result)) {
        // get id, proc_jobunit_id
        id = dbi_result_get_ulonglong(result, "id");
        proc_jobunit_id =
            dbi_result_get_ulonglong(result, "proc_jobunit_id");

        // force stop topjobnet
        jhkdb_begin();
        ts = jhk_time();
        rc_db =
            jhkdb_execute
            ("UPDATE proc_topjobnets SET status = %d, force_stop = false, end_time = '%s', updated_at = '%s' WHERE id = %llu AND status = %d AND force_stop = true",
             JOBUNIT_STATUS_ERROR, ts, ts, id, JOBUNIT_STATUS_RUNNING);
        if (rc_db != 0) {
            jhkdb_rollback();
            continue;
        }
        // force stop rootjobnet
        rc_db =
            jhkdb_execute
            ("UPDATE proc_jobunits SET force_stop = true, updated_at = '%s' WHERE id = %llu AND status = %d",
             ts, proc_jobunit_id, JOBUNIT_STATUS_RUNNING);
        if (rc_db != 0) {
            jhkdb_rollback();
            continue;
        }
        jhkdb_commit();
    }

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
int monitor_jobnets(void)
{
    apr_uint64_t id;
    dbi_result result = NULL;
    jobunit_t *obj = NULL;

    jhklog_trace("In %s()", __func__);

    result =
        jhkdb_select
        ("SELECT id FROM proc_jobunits WHERE (status = %d OR status = %d) AND kind < %d",
         JOBUNIT_STATUS_RUNNING, JOBUNIT_STATUS_ERROR,
         JOBUNIT_KIND_STARTJOB);
    if (result == NULL) {
        return -1;
    }

    obj = jobunit_new();
    while (dbi_result_next_row(result)) {
        jhkdb_begin();

        // lock record
        id = dbi_result_get_ulonglong(result, "id");
        if (proc_jobunit_select(obj, id) != 0) {
            jhkdb_rollback();
            continue;
        }
        // monitor jobnet
        if (monitor_jobnet(obj) != 0) {
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
int monitor_jobnet(jobunit_t * obj)
{
    time_t now;
    int rc_db, status, timeout_occur, rc_flowing;
    dbi_result result = NULL;
    jobunit_t *obj_child = NULL;
    char *ts;

    if (obj == NULL)
        return -1;

    jhklog_trace("In %s() id: %llu, kind: %d, status: %d", __func__,
                 obj->id, obj->kind, obj->status);
    if (!
        (obj->status == JOBUNIT_STATUS_RUNNING
         || obj->status == JOBUNIT_STATUS_ERROR))
        return 0;

    // force stop jobnet
    if (obj->status == JOBUNIT_STATUS_RUNNING && obj->force_stop == 1) {
        execlog_info(obj->id, "The jobnet '%s' force stop", obj->name);

        ts = jhk_time();
        rc_db =
            jhkdb_execute
            ("UPDATE proc_jobunits SET force_stop = true, updated_at = '%s' WHERE status = %d AND parent_id = %llu",
             ts, JOBUNIT_STATUS_RUNNING, obj->id);
        if (rc_db != 0)
            return -1;

        rc_db =
            jhkdb_execute
            ("UPDATE proc_jobunits SET force_stop = false, updated_at = '%s' WHERE id = %llu",
             ts, obj->id);
        if (rc_db != 0)
            return -1;

        return 0;
    }
    // normal monitor
    result =
        jhkdb_select("SELECT * FROM proc_jobunits WHERE parent_id = %llu",
                     obj->id);
    if (result == NULL)
        return -1;

    // init
    now = time(NULL);
    status = JOBUNIT_STATUS_RUNNING;
    timeout_occur = 0;

    // check jobnet timeout
    if (obj->timeout > 0) {
        if ((now - obj->start_time) >= (obj->timeout * 60)) {
            timeout_occur = 1;
            if (obj->timeout_occur == 0) {
                execlog_warn(obj->id, "The jobnet '%s' is timeout",
                             obj->name);
            }
        }
    }
    // check child jobs
    obj_child = jobunit_new();
    while (dbi_result_next_row(result)) {
        jobunit_load(obj_child, result);

        // check child job timeout
        if (obj_child->timeout_occur == 1)
            timeout_occur = 1;

        // check child job status
        if (obj_child->status == JOBUNIT_STATUS_ERROR) {
            status = JOBUNIT_STATUS_ERROR;
        }
        // check child endjob
        if (obj_child->kind == JOBUNIT_KIND_ENDJOB
            && obj_child->status == JOBUNIT_STATUS_END) {
            status = JOBUNIT_STATUS_END;
        }
    }
    jobunit_destroy(obj_child);
    dbi_result_free(result);

    // check update
    if (obj->status == status && obj->timeout_occur == timeout_occur) {
        // no change
        return 0;
    }
    // flowing jobnet
    if (status == JOBUNIT_STATUS_END && obj->parent_id > 0) {
        rc_flowing = jobunit_flowing(obj, CONNECTOR_KIND_NORMAL);
        if (rc_flowing < 0) {
            return -1;
        } else if (rc_flowing == 0) {
            status = JOBUNIT_STATUS_ERROR;
        }
    }
    // update jobnet
    ts = jhk_time();
    if (status == JOBUNIT_STATUS_RUNNING) {
        rc_db =
            jhkdb_execute
            ("UPDATE proc_jobunits SET status = %d, timeout_occur = %d, end_time = NULL, updated_at = '%s' WHERE id = %llu",
             status, timeout_occur, ts, obj->id);
    } else {
        rc_db =
            jhkdb_execute
            ("UPDATE proc_jobunits SET status = %d, timeout_occur = %d, end_time = '%s', updated_at = '%s' WHERE id = %llu",
             status, timeout_occur, ts, ts, obj->id);

        if (status == JOBUNIT_STATUS_END) {
            execlog_info(obj->id, "The jobnet '%s' successfully completed",
                         obj->name);
        } else {
            execlog_info(obj->id, "The jobnet '%s' caught ERROR",
                         obj->name);
        }
    }
    if (rc_db != 0)
        return -1;

    // sync proc_topjobnet
    if (obj->parent_id == 0) {
        if (status == JOBUNIT_STATUS_RUNNING) {
            rc_db =
                jhkdb_execute
                ("UPDATE proc_topjobnets SET status = %d, timeout_occur = %d, end_time = NULL, updated_at = '%s' WHERE proc_jobunit_id = %llu",
                 status, timeout_occur, ts, obj->id);

            // only for timeout_occur
            if (obj->timeout_occur == 0 && timeout_occur == 1) {
                // check topjobnet alarm
                alarms_execute(obj->id, JOBUNIT_STATUS_RUNNING);
            }
        } else {
            rc_db =
                jhkdb_execute
                ("UPDATE proc_topjobnets SET status = %d, timeout_occur = %d, end_time = '%s', updated_at = '%s' WHERE proc_jobunit_id = %llu",
                 status, timeout_occur, ts, ts, obj->id);
            // check topjobnet alarm
            alarms_execute(obj->id, status);
        }
        if (rc_db != 0)
            return -1;
    }
    // joblog
    obj->status = status;
    obj->timeout_occur = timeout_occur;
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
int monitor_jobs(void)
{
    int rc_db;
    apr_uint64_t id;
    dbi_result result = NULL;
    jobunit_t *obj = NULL;

    jhklog_trace("In %s()", __func__);

    // force start job
    rc_db =
        jhkdb_execute
        ("UPDATE proc_jobunits SET status = %d, start_time = NULL, end_time = NULL, timeout_occur = false, force_start = false, force_stop = false, pid = 0, updated_at = '%s' \
         WHERE status = %d AND kind >= %d AND force_start = true",
         JOBUNIT_STATUS_READY, jhk_time(), JOBUNIT_STATUS_ERROR, JOBUNIT_KIND_STARTJOB);
    if (rc_db != 0)
        return -1;

    // normal monitor
    result =
        jhkdb_select
        ("SELECT id FROM proc_jobunits WHERE status = %d AND kind >= %d",
         JOBUNIT_STATUS_RUNNING, JOBUNIT_KIND_STARTJOB);
    if (result == NULL) {
        return -1;
    }

    obj = jobunit_new();
    while (dbi_result_next_row(result)) {
        jhkdb_begin();
        // lock the record
        id = dbi_result_get_ulonglong(result, "id");
        if (proc_jobunit_select(obj, id) != 0) {
            jhkdb_rollback();
            continue;
        }
        // monitor job
        if (monitor_job(obj) != 0) {
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
int monitor_job(jobunit_t * obj)
{
    int rc_job;

    if (obj == NULL)
        return -1;
    jhklog_trace("In %s() id: %llu, kind: %d, status: %d", __func__,
                 obj->id, obj->kind, obj->status);
    if (obj->status != JOBUNIT_STATUS_RUNNING)
        return 0;

    // check timemout
    if (obj->timeout_occur == 0 && obj->timeout > 0
        && ((time(NULL) - obj->start_time) >= (obj->timeout * 60))) {
        if (proc_jobunit_timeout_occur(obj) != 0)
            return -1;
    }
    // monitor job
    switch (obj->kind) {
    case JOBUNIT_KIND_SLEEPJOB:
        return sleepjob_monitor(obj);
        break;
    case JOBUNIT_KIND_CLOCKJOB:
        return clockjob_monitor(obj);
        break;
    case JOBUNIT_KIND_SSHJOB:
    case JOBUNIT_KIND_WINJOB:
        return monitor_background(obj);
        break;
    default:
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
int monitor_background(jobunit_t * obj)
{
    if (obj == NULL)
        return -1;

    jhklog_trace("In %s() id: %llu, kind: %d, pid: %d, force_stop: %d",
                 __func__, obj->id, obj->kind, obj->pid, obj->force_stop);

    // wait pid
    if (obj->pid == 0) {
        if (obj->force_stop == 0) {
            execlog_warn(obj->id, "The job '%s' is not started",
                         obj->name);
            return 0;
        } else {
            jhklog_info("Force stop the job. name: %s, id: %llu, pid: %d",
                        obj->name, obj->id, obj->pid);
            execlog_info(obj->id, "The job '%s' is forced to stop",
                         obj->name);
            return proc_jobunit_set_failure(obj);
        }
    }
    // check pid
    if (kill(obj->pid, 0) != 0) {
        jhklog_warn("In %s() pid is not existed. id: %llu, pid: %d",
                    __func__, obj->id, obj->pid);
        execlog_error(obj->id, "The job '%s' is not existed", obj->name);
        return proc_jobunit_set_failure(obj);
    }
    // check force_stop
    if (obj->force_stop == 0)
        return 0;

    // force stop job
    if (kill(obj->pid, SIGTERM) != 0) {
        jhklog_error("In %s() kill failed. id: %llu, pid: %d", __func__,
                     obj->id, obj->pid);
        execlog_error(obj->id, "The job '%s' failed to force stop",
                      obj->name);
        return 0;
    } else {
        jhklog_info("Force stop the job. name: %s, id: %llu, pid: %d",
                    obj->name, obj->id, obj->pid);
        execlog_info(obj->id, "The job '%s' is forced to stop", obj->name);
        return proc_jobunit_set_failure(obj);
    }

    return 0;
}
