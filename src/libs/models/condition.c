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
condition_t *condition_new(void)
{
    condition_t *obj;

    jhklog_trace("In %s()", __func__);

    obj = (condition_t *) malloc(sizeof(condition_t));
    obj->cond = NULL;
    condition_init(obj);

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
void condition_destroy(condition_t * obj)
{
    jhklog_trace("In %s()", __func__);

    if (obj == NULL)
        return;

    condition_init(obj);
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
int condition_init(condition_t * obj)
{
    jhklog_trace("In %s()", __func__);

    if (obj == NULL)
        return -1;

    jhk_free(obj->cond);
    obj->id = 0;
    obj->jobunit_id = 0;
    obj->proc_jobunit_id = 0;
    obj->hist_jobunit_id = 0;
    obj->method = 0;
    obj->kind = 0;
    obj->cond = NULL;
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
int condition_load(condition_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    condition_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->jobunit_id = dbi_result_get_ulonglong(res, "jobunit_id");
    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");
    obj->hist_jobunit_id =
        dbi_result_get_ulonglong(res, "hist_jobunit_id");

    obj->method = dbi_result_get_int(res, "method");
    obj->kind = dbi_result_get_int(res, "kind");
    obj->cond = jhkdb_get_string(res, "cond");
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
apr_uint64_t proc_condition_insert(condition_t * obj)
{
    apr_uint64_t id;
    char *esc_cond = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    // escape strinq
    esc_cond = jhkdb_escape_string(obj->cond);

    id = jhkdb_insert
        ("INSERT INTO proc_conditions(proc_jobunit_id, method, kind, cond, negative) VALUES(%llu, %d, %d, '%s', %d)",
         obj->proc_jobunit_id, obj->method, obj->kind, esc_cond,
         obj->negative);
    obj->id = id;

    jhk_free(esc_cond);
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
apr_uint64_t hist_condition_insert(condition_t * obj)
{
    apr_uint64_t id;
    char *esc_cond = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    // escape strinq
    esc_cond = jhkdb_escape_string(obj->cond);

    id = jhkdb_insert
        ("INSERT INTO hist_conditions(hist_jobunit_id, method, kind, cond, negative) VALUES(%llu, %d, %d, '%s', %d)",
         obj->hist_jobunit_id, obj->method, obj->kind, esc_cond,
         obj->negative);
    obj->id = id;

    jhk_free(esc_cond);
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
int conditions_put_process(const apr_uint64_t jobunit_id,
                           const apr_uint64_t proc_jobunit_id)
{
    int rc;
    condition_t *obj;
    dbi_result result;

    jhklog_debug("In %s() jobunit_id: %llu, proc_jobunit_id: %llu",
                 __func__, jobunit_id, proc_jobunit_id);

    result =
        jhkdb_select("SELECT * FROM conditions WHERE jobunit_id = %llu",
                     jobunit_id);
    if (result == NULL)
        return -1;

    rc = -1;
    obj = condition_new();
    while (dbi_result_next_row(result)) {
        condition_load(obj, result);
        obj->proc_jobunit_id = proc_jobunit_id;
        if (proc_condition_insert(obj) == 0)
            goto error;
    }

    rc = 0;
  error:
    condition_destroy(obj);
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
int conditions_put_history(const apr_uint64_t proc_jobunit_id,
                           const apr_uint64_t hist_jobunit_id)
{
    int rc;
    condition_t *obj;
    dbi_result result;

    jhklog_debug("In %s() proc_jobunit_id: %llu, hist_jobunit_id: %llu",
                 __func__, proc_jobunit_id, hist_jobunit_id);

    result =
        jhkdb_select
        ("SELECT * FROM proc_conditions WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    rc = -1;
    obj = condition_new();
    while (dbi_result_next_row(result)) {
        condition_load(obj, result);
        obj->hist_jobunit_id = hist_jobunit_id;
        if (hist_condition_insert(obj) == 0)
            goto error;
    }

    rc = 0;
  error:
    condition_destroy(obj);
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
int proc_conditions_delete(const apr_uint64_t proc_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM proc_conditions WHERE proc_jobunit_id = %llu",
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
int hist_conditions_delete(const apr_uint64_t hist_jobunit_id)
{
    jhklog_trace("In %s()", __func__);
    return
        jhkdb_execute
        ("DELETE FROM hist_conditions WHERE hist_jobunit_id = %llu",
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
int condition_execute(condition_t * obj, jobresult_t * obj_res)
{
    int rv;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || obj_res == NULL)
        return JHAKO_ERROR;

    jhklog_debug("In %s() id: %llu", __func__, obj->id);

    switch (obj->kind) {
    case CONDITION_KIND_EXITCODE:
        rv = jhk_on_range(obj_res->exitcode, obj->cond);
        break;
    case CONDITION_KIND_EXITSIGNAL:
        rv = jhk_on_regexp(obj_res->exitsignal, obj->cond);
        break;
    case CONDITION_KIND_STDOUT:
        rv = jhk_on_regexp(obj_res->stdout, obj->cond);
        break;
    case CONDITION_KIND_STDERR:
        rv = jhk_on_regexp(obj_res->stderr, obj->cond);
        break;
    default:
        jhklog_error("In %s() unknown condition kind. %d",
                     __func__, obj->kind);
        return JHAKO_ERROR;
    }

    if (rv == JHAKO_TRUE) {
        rv = (obj->negative) ? JHAKO_FALSE : JHAKO_TRUE;
    } else if (rv == JHAKO_FALSE) {
        rv = (obj->negative) ? JHAKO_TRUE : JHAKO_FALSE;
    }

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
int conditions_execute(const apr_uint64_t proc_jobunit_id,
                       jobresult_t * obj_res)
{
    int rc, rv_error, rv_branch;
    dbi_result result;
    condition_t *obj = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj_res == NULL)
        return -1;

    jhklog_debug("In %s() proc_jobunit_id: %llu", __func__,
                 proc_jobunit_id);

    // decide job conditions
    result =
        jhkdb_select
        ("SELECT * FROM proc_conditions WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    // not find condition
    if (dbi_result_get_numrows(result) == 0) {
        dbi_result_free(result);
        return CONNECTOR_KIND_NORMAL;
    }
    // init
    rc = -1;
    obj = condition_new();

    // error conditions
    while (dbi_result_next_row(result)) {
        condition_load(obj, result);
        if (obj->method != CONDITION_METHOD_ERROR)
            continue;

        rv_error = condition_execute(obj, obj_res);
        if (rv_error == JHAKO_FALSE)
            continue;

        if (rv_error == JHAKO_TRUE) {
            execlog_info(obj->proc_jobunit_id,
                         "Error condition '%s' is matched", obj->cond);
        } else {
            execlog_error(obj->proc_jobunit_id,
                          "Error condition '%s' is incorrect", obj->cond);
        }
        rc = CONNECTOR_KIND_ERROR;
        goto finish;
    }

    // branch conditions
    dbi_result_first_row(result);
    do {
        condition_load(obj, result);
        if (obj->method != CONDITION_METHOD_BRANCH)
            continue;

        rv_branch = condition_execute(obj, obj_res);
        if (rv_branch == JHAKO_FALSE)
            continue;

        if (rv_branch == JHAKO_TRUE) {
            execlog_info(obj->proc_jobunit_id,
                         "Branch condition '%s' is matched", obj->cond);
            rc = CONNECTOR_KIND_BRANCH;
        } else {
            execlog_error(obj->proc_jobunit_id,
                          "Branch condition '%s' is incorrect", obj->cond);
            rc = CONNECTOR_KIND_ERROR;
        }
        goto finish;
    } while (dbi_result_next_row(result));

    rc = CONNECTOR_KIND_NORMAL;
  finish:
    condition_destroy(obj);
    dbi_result_free(result);
    return rc;
}
