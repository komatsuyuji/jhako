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
#include "executive.h"

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
void executive_mainloop(void)
{
    pid_t ppid;

    jhklog_info("executive start");

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
        if (executive_exec() <= 0) {
            sleep(1);
        }
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
int executive_exec(void)
{
    int num;
    apr_uint64_t id;
    dbi_result result = NULL;
    jobunit_t *obj = NULL;

    jhklog_trace("In %s()", __func__);

    result =
        jhkdb_select
        ("SELECT id FROM proc_jobunits WHERE status = %d AND hold = false",
         JOBUNIT_STATUS_READY);
    if (result == NULL) {
        return -1;
    }

    num = 0;
    obj = jobunit_new();
    while (dbi_result_next_row(result)) {
        jhkdb_begin();

        // lock the record
        id = dbi_result_get_ulonglong(result, "id");
        if (proc_jobunit_select(obj, id) != 0) {
            jhkdb_rollback();
            continue;
        }

        if (executive_run(obj) != 0) {
            jhkdb_rollback();
            continue;
        }

        num++;
        jhkdb_commit();
    }
    jhklog_debug("In %s() num: %d", __func__, num);

    jobunit_destroy(obj);
    dbi_result_free(result);
    return num;
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
int executive_run(jobunit_t * obj)
{
    int rc_job;

    jhklog_trace("In %s()", __func__);

    if (!(obj->status == JOBUNIT_STATUS_READY && obj->hold == 0)) {
        return -1;
    }
    // skip the jobunit
    if (obj->skip) {
        execlog_info(obj->id, "The job '%s' is skipped", obj->name);
        return proc_jobunit_set_end(obj, CONNECTOR_KIND_NORMAL);
    }
    // run the jobunit
    rc_job = -1;
    switch (obj->kind) {
    case JOBUNIT_KIND_ROOTJOBNET:
    case JOBUNIT_KIND_JOBNET:
        // jobnet
        return executive_jobnet(obj);
        break;
    case JOBUNIT_KIND_STARTJOB:
    case JOBUNIT_KIND_ENDJOB:
    case JOBUNIT_KIND_MERGEJOB:
        // startjob, endjob, mergejob
        return executive_commjob(obj);
        break;
    case JOBUNIT_KIND_SLEEPJOB:
        // sleepjob
        return sleepjob_execute(obj);
        break;
    case JOBUNIT_KIND_CLOCKJOB:
        // clockjob
        return clockjob_execute(obj);
        break;
    case JOBUNIT_KIND_DATEJOB:
        // datejob
        return datejob_execute(obj);
        break;
    case JOBUNIT_KIND_VARJOB:
        // varjob
        return vardata_execute(obj);
        break;
    case JOBUNIT_KIND_SSHJOB:
    case JOBUNIT_KIND_WINJOB:
        // sshjob, winjob
        return executive_background(obj);
        break;
    default:
        jhklog_error("In %s() unknown job kind. id: %llu, kind: %d",
                     __func__, obj->id, obj->kind);
        return proc_jobunit_set_error(obj);
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
int executive_jobnet(jobunit_t * obj)
{
    int rc;
    char *ts;

    execlog_info(obj->id, "The jobnet '%s' started", obj->name);
    ts = jhk_time();

    // run startjob
    rc = jhkdb_execute
        ("UPDATE proc_jobunits SET status = %d, start_time = NULL, end_time = NULL, updated_at = '%s' WHERE parent_id = %llu AND kind = %d",
         JOBUNIT_STATUS_READY, ts, obj->id, JOBUNIT_KIND_STARTJOB);
    if (rc != 0)
        return rc;


    // inherit variables
    rc = proc_variable_inherit(obj->id, obj->parent_id);
    if (rc != 0)
        return rc;

    // update jobnet status
    rc = jhkdb_execute
        ("UPDATE proc_jobunits SET status = %d, start_time = '%s', end_time = NULL, updated_at = '%s' WHERE id = %llu",
         JOBUNIT_STATUS_RUNNING, ts, ts, obj->id);
    if (rc != 0)
        return rc;

    // jobnet log
    obj->status = JOBUNIT_STATUS_RUNNING;
    jhkjoblog_jobunit(obj);

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
int executive_commjob(jobunit_t * obj)
{
    jhklog_trace("In %s() id: %llu, name: %s, kind: %d", __func__,
                 obj->id, obj->name, obj->kind);

    return proc_jobunit_set_end(obj, CONNECTOR_KIND_NORMAL);
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
int executive_background(jobunit_t * obj)
{
    int rc, ec, rc_db, rc_job;
    pid_t pid;

    jhklog_trace("In %s() id: %llu, name: %s, kind: %d", __func__,
                 obj->id, obj->name, obj->kind);

    // first fork
    pid = jhk_fork();
    if (pid == -1) {
        jhklog_error("In %s() the first fork failed", __func__);
        return -1;
    } else if (pid > 0) {
        ec = -1;
        wait(&ec);
        if (WEXITSTATUS(ec) != 0) {
            jhklog_error("In %s() the second fork failed. exitcode: %d",
                         __func__, WEXITSTATUS(ec));
            return -1;
        }
        return proc_jobunit_set_running(obj);
    }
    setsid();
    jhklog_debug("In %s() the first fork is successfully", __func__);

    // second fork
    pid = jhk_fork();
    if (pid < 0) {
        exit(-1);
    } else if (pid > 0) {
        exit(0);
    }
    jhklog_debug("In %s() the second fork is successfully", __func__);

    // main execute
    rc = -1;
    if (jhkdb_connect() != 0)
        exit(-1);

    // set pid
    rc_db =
        jhkdb_execute("UPDATE proc_jobunits SET pid = %d WHERE id = %llu",
                      getpid(), obj->id);
    if (rc_db != 0)
        goto error;

    // run the job
    rc_job = -1;
    switch (obj->kind) {
    case JOBUNIT_KIND_SSHJOB:
        rc_job = sshjob_execute(obj);
        break;
    case JOBUNIT_KIND_WINJOB:
        rc_job = winjob_execute(obj);
        break;
    default:
        jhklog_error("In %s() unknown the job kind '%d'", __func__,
                     obj->kind);
        break;
    }

    // check the result
    jhklog_debug("In %s() the job return code: %d, id: %llu", __func__,
                 rc_job, obj->id);
    jhkdb_begin();
    if (proc_jobunit_set_finish(obj, rc_job) != 0) {
        jhkdb_rollback();
        goto error;
    }
    jhkdb_commit();

    rc = 0;
  error:
    jhkdb_disconnect();
    exit(rc);
}
