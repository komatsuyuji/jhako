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
jobresult_t *jobresult_new(void)
{
    jobresult_t *obj;

    jhklog_trace("In %s()", __func__);
    obj = (jobresult_t *) malloc(sizeof(jobresult_t));
    obj->exitsignal = (char *) malloc(EXIT_SIGNAL_SIZE);
    obj->stdout = (char *) malloc(JOB_RESULT_SIZE);
    obj->stderr = (char *) malloc(JOB_RESULT_SIZE);
    jobresult_init(obj);

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
void jobresult_destroy(jobresult_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return;

    jhk_free(obj->exitsignal);
    jhk_free(obj->stdout);
    jhk_free(obj->stderr);
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
int jobresult_init(jobresult_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    obj->id = 0;
    obj->proc_jobunit_id = 0;
    obj->hist_jobunit_id = 0;
    obj->exitcode = -1;
    memset(obj->exitsignal, 0, EXIT_SIGNAL_SIZE);
    memset(obj->stdout, 0, JOB_RESULT_SIZE);
    memset(obj->stderr, 0, JOB_RESULT_SIZE);
    obj->created_at = 0;
    obj->updated_at = 0;

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
int jobresult_load(jobresult_t * obj, dbi_result res)
{
    const char *str;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    jobresult_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");
    obj->hist_jobunit_id =
        dbi_result_get_ulonglong(res, "hist_jobunit_id");
    obj->exitcode = dbi_result_get_int(res, "exitcode");

    str = dbi_result_get_string(res, "exitsignal");
    if (str) {
        snprintf(obj->exitsignal, EXIT_SIGNAL_SIZE, "%s", str);
    }

    str = dbi_result_get_string(res, "stdout");
    if (str) {
        snprintf(obj->stdout, JOB_RESULT_SIZE, "%s", str);
    }

    str = dbi_result_get_string(res, "stderr");
    if (str) {
        snprintf(obj->stderr, JOB_RESULT_SIZE, "%s", str);
    }

    obj->created_at = jhkdb_get_datetime(res, "created_at");
    obj->updated_at = jhkdb_get_datetime(res, "updated_at");

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
int proc_jobresult_select(jobresult_t * obj,
                          const apr_uint64_t proc_jobunit_id)
{
    apr_uint64_t num;
    dbi_result result = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    jhklog_debug("In %s() proc_jobunit_id: %llu", __func__,
                 proc_jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM proc_jobresults WHERE proc_jobunit_id = %llu ORDER BY id DESC",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    num = dbi_result_get_numrows(result);
    if (num == 0) {
        dbi_result_free(result);
        return 1;
    }

    while (dbi_result_next_row(result)) {
        jobresult_load(obj, result);
        break;
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
apr_uint64_t proc_jobresult_insert(jobresult_t * obj)
{
    apr_uint64_t id;
    char *esc_exitsignal = NULL;
    char *esc_stdout = NULL;
    char *esc_stderr = NULL;
    char *ts;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    jhklog_debug("In %s() proc_jobunit_id: %llu", __func__,
                 obj->proc_jobunit_id);

    // escape strinq
    esc_exitsignal = jhkdb_escape_string(obj->exitsignal);
    esc_stdout = jhkdb_escape_string(obj->stdout);
    esc_stderr = jhkdb_escape_string(obj->stderr);

    ts = jhk_time();
    id = jhkdb_insert
        ("INSERT INTO proc_jobresults(proc_jobunit_id, exitcode, exitsignal, stdout, stderr, created_at, updated_at) VALUES(%llu, %d, '%s', '%s', '%s', '%s', '%s')",
         obj->proc_jobunit_id, obj->exitcode, esc_exitsignal, esc_stdout,
         esc_stderr, ts, ts);

    if (id > 0)
        obj->id = id;

    jhk_free(esc_exitsignal);
    jhk_free(esc_stdout);
    jhk_free(esc_stderr);
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
apr_uint64_t hist_jobresult_insert(jobresult_t * obj)
{
    apr_uint64_t id;
    char *esc_exitsignal = NULL;
    char *esc_stdout = NULL;
    char *esc_stderr = NULL;
    char *created_at = NULL;
    char *updated_at = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    jhklog_debug("In %s() hist_jobunit_id: %llu", __func__,
                 obj->hist_jobunit_id);

    // escape strinq
    esc_exitsignal = jhkdb_escape_string(obj->exitsignal);
    esc_stdout = jhkdb_escape_string(obj->stdout);
    esc_stderr = jhkdb_escape_string(obj->stderr);

    created_at = jhkdb_escape_datetime(obj->created_at);
    updated_at = jhkdb_escape_datetime(obj->updated_at);

    id = jhkdb_insert
        ("INSERT INTO hist_jobresults(hist_jobunit_id, exitcode, exitsignal, stdout, stderr, created_at, updated_at) VALUES(%llu, %d, '%s', '%s', '%s', %s, %s)",
         obj->hist_jobunit_id, obj->exitcode, esc_exitsignal, esc_stdout,
         esc_stderr, created_at, updated_at);

    if (id > 0)
        obj->id = id;

    jhk_free(created_at);
    jhk_free(updated_at);
    jhk_free(esc_exitsignal);
    jhk_free(esc_stdout);
    jhk_free(esc_stderr);
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
int jobresults_put_history(const apr_uint64_t proc_jobunit_id,
                           const apr_uint64_t hist_jobunit_id)
{
    int rc;
    jobresult_t *obj;
    dbi_result result;

    jhklog_debug("In %s() proc_jobunit_id: %llu, hist_jobunit_id: %llu",
                 __func__, proc_jobunit_id, hist_jobunit_id);

    result =
        jhkdb_select
        ("SELECT * FROM proc_jobresults WHERE proc_jobunit_id = %llu ORDER BY id ASC",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    rc = -1;
    obj = jobresult_new();
    while (dbi_result_next_row(result)) {
        jobresult_load(obj, result);
        obj->hist_jobunit_id = hist_jobunit_id;
        if (hist_jobresult_insert(obj) == 0)
            goto error;
    }

    rc = 0;
  error:
    jobresult_destroy(obj);
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
int proc_jobresults_delete(const apr_uint64_t proc_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM proc_jobresults WHERE proc_jobunit_id = %llu",
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
int hist_jobresults_delete(const apr_uint64_t hist_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM hist_jobresults WHERE hist_jobunit_id = %llu",
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
int jobresult_execute(jobresult_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    jhklog_debug("In %s() proc_jobunit_id: %llu", __func__,
                 obj->proc_jobunit_id);

    // exitsignal
    if (obj->exitcode < 0) {
        jhklog_warn("In %s() get a exitsignal: %s, proc_jobunit_id: %llu",
                    __func__, obj->exitsignal, obj->proc_jobunit_id);
        return CONNECTOR_KIND_ERROR;
    }

    return conditions_execute(obj->proc_jobunit_id, obj);
}
