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
variable_t *variable_new(void)
{
    variable_t *obj;

    jhklog_trace("In %s()", __func__);
    obj = (variable_t *) malloc(sizeof(variable_t));
    obj->name = NULL;
    obj->value = NULL;
    variable_init(obj);

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
void variable_destroy(variable_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return;

    variable_init(obj);
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
int variable_init(variable_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    jhk_free(obj->name);
    jhk_free(obj->value);

    obj->id = 0;
    obj->jobunit_id = 0;
    obj->proc_jobunit_id = 0;
    obj->hist_jobunit_id = 0;

    obj->name = NULL;
    obj->value = NULL;

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
int variable_load(variable_t * obj, dbi_result res)
{
    char *value;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    variable_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->jobunit_id = dbi_result_get_ulonglong(res, "jobunit_id");
    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");
    obj->hist_jobunit_id =
        dbi_result_get_ulonglong(res, "hist_jobunit_id");

    obj->name = jhkdb_get_string(res, "name");
    obj->value = jhkdb_get_string(res, "value");

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
int variable_load_one(variable_t * obj, dbi_result res)
{
    int rc;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    rc = 1;
    while (dbi_result_next_row(res)) {
        variable_load(obj, res);
        rc = 0;
        break;
    }

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
int proc_variable_select(variable_t * obj,
                         const apr_uint64_t proc_jobunit_id, char *name)
{
    int rc, rc_db;
    dbi_result result = NULL;
    char *esc_name = NULL;

    if (obj == NULL || name == NULL)
        return -1;

    jhklog_debug("In %s() proc_jobunit_id: %llu, name: %s", __func__,
                 proc_jobunit_id, name);

    esc_name = jhkdb_escape_string(name);
    result =
        jhkdb_select
        ("SELECT * FROM proc_variables WHERE proc_jobunit_id = %llu AND name = '%s'",
         proc_jobunit_id, esc_name);
    jhk_free(esc_name);
    if (result == NULL)
        return -1;

    rc = 0;
    rc_db = variable_load_one(obj, result);

    if (rc_db < 0) {
        rc = -1;
    } else if (rc_db > 0) {
        obj->name = strdup(name);
        obj->value = strdup("");
    }

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
int proc_variable_create(const apr_uint64_t proc_jobunit_id, char *name,
                         char *value)
{
    int rc;
    apr_uint64_t id;
    char *ts;
    char *esc_name = NULL;
    char *esc_value = NULL;

    jhklog_trace("In %s() name: %s, value: %s", __func__, name, value);

    // escape string
    rc = -1;
    esc_name = jhkdb_escape_string(name);
    esc_value = jhkdb_escape_string(value);
    ts = jhk_time();

    id = jhkdb_insert
        ("INSERT INTO proc_variables(proc_jobunit_id, name, value, created_at, updated_at) VALUES(%llu, '%s', '%s', '%s', '%s')",
         proc_jobunit_id, esc_name, esc_value, ts, ts);
    if (id > 0)
        rc = 0;

    jhk_free(esc_name);
    jhk_free(esc_value);
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
int proc_variable_renew(const apr_uint64_t proc_jobunit_id, char *name,
                        char *value)
{
    int rc;
    char *ts;
    char *esc_name = NULL;
    char *esc_value = NULL;

    jhklog_trace("In %s() name: %s, value: %s", __func__, name, value);

    // escape string
    rc = -1;
    esc_name = jhkdb_escape_string(name);
    esc_value = jhkdb_escape_string(value);
    ts = jhk_time();

    rc = jhkdb_execute
        ("UPDATE proc_variables SET value = '%s', updated_at = '%s' WHERE proc_jobunit_id = %llu AND name = '%s'",
         esc_value, ts, proc_jobunit_id, esc_name);

    jhk_free(esc_name);
    jhk_free(esc_value);
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
int proc_variable_overwrite(const apr_uint64_t proc_jobunit_id, char *name,
                            char *value, const int overwrite)
{
    int rc;
    variable_t *obj = NULL;

    jhklog_trace("In %s() name: %s, value: %s, overwrite: %d", __func__,
                 name, value, overwrite);

    // init
    rc = -1;
    obj = variable_new();
    obj->id = 0;

    proc_variable_select(obj, proc_jobunit_id, name);
    if (overwrite == VARDATUM_OVERWRITE_TRUE) {
        if (obj->id == 0) {
            if (proc_variable_create(proc_jobunit_id, name, value) != 0) {
                goto error;
            }
        } else {
            if (proc_variable_renew(proc_jobunit_id, name, value) != 0) {
                goto error;
            }
        }
    } else {
        if (obj->id == 0) {
            if (proc_variable_create(proc_jobunit_id, name, value) != 0) {
                goto error;
            }
        }
    }

    rc = 0;
  error:
    variable_destroy(obj);
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
int proc_variables_dup(const apr_uint64_t proc_jobunit_id,
                       const apr_uint64_t desc_jobunit_id)
{
    int rc;
    variable_t *obj;
    dbi_result result = NULL;

    jhklog_trace("In %s() proc_jobunit_id: %llu, desc_jobunit_id: %llu",
                 __func__, proc_jobunit_id, desc_jobunit_id);

    if (desc_jobunit_id == 0)
        return 0;

    result =
        jhkdb_select
        ("SELECT * FROM proc_variables WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    rc = -1;
    obj = variable_new();
    while (dbi_result_next_row(result)) {
        variable_load(obj, result);
        if (proc_variable_overwrite
            (desc_jobunit_id, obj->name, obj->value,
             VARDATUM_OVERWRITE_TRUE) != 0)
            goto error;
    }

    rc = 0;
  error:
    variable_destroy(obj);
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
apr_uint64_t hist_variable_insert(variable_t * obj)
{
    apr_uint64_t id;
    char *ts;
    char *esc_name = NULL;
    char *esc_value = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    // escape string
    id = 0;
    esc_name = jhkdb_escape_string(obj->name);
    esc_value = jhkdb_escape_string(obj->value);
    ts = jhk_time();

    id = jhkdb_insert
        ("INSERT INTO hist_variables(hist_jobunit_id, name, value, created_at, updated_at) VALUES(%llu, '%s', '%s', '%s', '%s')",
         obj->hist_jobunit_id, esc_name, esc_value, ts, ts);
    obj->id = id;

    jhk_free(esc_name);
    jhk_free(esc_value);

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
int variables_put_history(const apr_uint64_t proc_jobunit_id,
                          const apr_uint64_t hist_jobunit_id)
{
    int rc;
    variable_t *obj = NULL;
    dbi_result result = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu, hist_jobunit_id: %llu",
                 __func__, proc_jobunit_id, hist_jobunit_id);

    result =
        jhkdb_select
        ("SELECT * FROM proc_variables WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    rc = -1;
    obj = variable_new();
    while (dbi_result_next_row(result)) {
        variable_load(obj, result);
        obj->hist_jobunit_id = hist_jobunit_id;
        if (hist_variable_insert(obj) == 0)
            goto error;
    }

    rc = 0;
  error:
    variable_destroy(obj);
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
int proc_variables_delete(const apr_uint64_t proc_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM proc_variables WHERE proc_jobunit_id = %llu",
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
int hist_variables_delete(const apr_uint64_t hist_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM hist_variables WHERE hist_jobunit_id = %llu",
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
char *variable_replace(const apr_uint64_t proc_jobunit_id, char *str)
{
    char *p, *q, *t;
    char name[256];
    char *new_str;
    int flag, len;
    variable_t *obj;

    jhklog_trace("In %s() proc_jobunit_id: %llu", __func__,
                 proc_jobunit_id);

    // init
    new_str = (char *) malloc(MAX_STRING_LEN);
    obj = variable_new();
    flag = 0;
    p = str;
    q = new_str;

    // check input
    if (str == NULL)
        goto finish;

    // main
    while (1) {
        if (*p == '\0')
            break;
        if ((q - new_str + 1) >= MAX_STRING_LEN)
            break;

        // variable start pos: ${
        if (*p == '$' && *(p + 1) == '{') {
            flag = 1;
            t = q;
        }
        // variable end pos: }
        if (*p == '}' && flag == 1) {
            flag = 0;

            // get variable name
            len = q - t - 2;
            if (len < sizeof(name)) {
                strncpy(name, t + 2, len);
            }
            *(name + len) = '\0';

            // get variable value
            len = 0;
            if (proc_variable_select(obj, proc_jobunit_id, name) == 0) {
                len = strlen(obj->value);
                if (t - new_str + len < MAX_STRING_LEN - 1) {
                    strncpy(t, obj->value, len);
                }
            }
            jhklog_debug("In %s() variable. name: %s, value: %s", __func__,
                         name, obj->value);

            q = t + len;
            p++;
            continue;
        }

        *q = *p;
        p++;
        q++;
    }

  finish:
    *q = '\0';
    variable_destroy(obj);
    return new_str;
}
