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
#include "jhkssh.h"

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
sshjob_t *sshjob_new(void)
{
    sshjob_t *obj;

    jhklog_trace("In %s()", __func__);
    obj = (sshjob_t *) malloc(sizeof(sshjob_t));
    obj->host = NULL;
    obj->username = NULL;
    obj->password = NULL;
    obj->privatekey = NULL;
    obj->command = NULL;
    sshjob_init(obj);

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
void sshjob_destroy(sshjob_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return;

    sshjob_init(obj);
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
int sshjob_init(sshjob_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    jhk_free(obj->host);
    jhk_free(obj->username);
    jhk_free(obj->password);
    jhk_free(obj->privatekey);
    jhk_free(obj->command);

    obj->id = 0;
    obj->jobunit_id = 0;
    obj->proc_jobunit_id = 0;
    obj->hist_jobunit_id = 0;
    obj->host = NULL;
    obj->port = 0;
    obj->authtype = 0;
    obj->username = NULL;
    obj->password = NULL;
    obj->privatekey = NULL;
    obj->command = NULL;

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
int sshjob_load(sshjob_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    sshjob_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->jobunit_id = dbi_result_get_ulonglong(res, "jobunit_id");
    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");
    obj->hist_jobunit_id =
        dbi_result_get_ulonglong(res, "hist_jobunit_id");

    obj->host = jhkdb_get_string(res, "host");
    obj->port = dbi_result_get_int(res, "port");
    obj->authtype = dbi_result_get_int(res, "authtype");
    obj->username = jhkdb_get_string(res, "username");
    obj->password = jhkdb_get_string(res, "password");
    obj->privatekey = jhkdb_get_string(res, "privatekey");
    obj->command = jhkdb_get_string(res, "command");

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
int sshjob_load_one(sshjob_t * obj, dbi_result res)
{
    apr_uint64_t num;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    num = dbi_result_get_numrows(res);
    if (num != 1) {
        jhklog_warn("In %s() can not find sshjob. numrows: %llu",
                    __func__, num);
        return 1;
    }

    while (dbi_result_next_row(res)) {
        sshjob_load(obj, res);
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
int sshjob_select(sshjob_t * obj, const apr_uint64_t jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() jobunit_id: %llu", __func__, jobunit_id);
    result =
        jhkdb_select("SELECT * FROM sshjobs WHERE jobunit_id = %llu",
                     jobunit_id);
    if (result == NULL)
        return -1;

    rc = sshjob_load_one(obj, result);

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
int proc_sshjob_select(sshjob_t * obj, const apr_uint64_t proc_jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu", __func__,
                 proc_jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM proc_sshjobs WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    rc = sshjob_load_one(obj, result);

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
int hist_sshjob_select(sshjob_t * obj, const apr_uint64_t hist_jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() hist_jobunit_id: %llu", __func__,
                 hist_jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM hist_sshjobs WHERE hist_jobunit_id = %llu",
         hist_jobunit_id);
    if (result == NULL)
        return -1;

    rc = sshjob_load_one(obj, result);

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
apr_uint64_t proc_sshjob_insert(sshjob_t * obj)
{
    apr_uint64_t id;
    char *esc_host = NULL;
    char *esc_username = NULL;
    char *esc_password = NULL;
    char *esc_privatekey = NULL;
    char *esc_command = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    // escape strinq
    esc_host = jhkdb_escape_string(obj->host);
    esc_username = jhkdb_escape_string(obj->username);
    esc_password = jhkdb_escape_string(obj->password);
    esc_privatekey = jhkdb_escape_string(obj->privatekey);
    esc_command = jhkdb_escape_string(obj->command);

    id = jhkdb_insert
        ("INSERT INTO proc_sshjobs(proc_jobunit_id, host, port, authtype, username, password, privatekey, command) VALUES(%llu, '%s', %d, %d, '%s', '%s', '%s', '%s')",
         obj->proc_jobunit_id, esc_host, obj->port, obj->authtype,
         esc_username, esc_password, esc_privatekey, esc_command);
    obj->id = id;

    jhk_free(esc_host);
    jhk_free(esc_username);
    jhk_free(esc_password);
    jhk_free(esc_privatekey);
    jhk_free(esc_command);

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
apr_uint64_t hist_sshjob_insert(sshjob_t * obj)
{
    apr_uint64_t id;
    char *esc_host = NULL;
    char *esc_username = NULL;
    char *esc_password = NULL;
    char *esc_privatekey = NULL;
    char *esc_command = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    // escape strinq
    esc_host = jhkdb_escape_string(obj->host);
    esc_username = jhkdb_escape_string(obj->username);
    esc_password = jhkdb_escape_string(obj->password);
    esc_privatekey = jhkdb_escape_string(obj->privatekey);
    esc_command = jhkdb_escape_string(obj->command);

    id = jhkdb_insert
        ("INSERT INTO hist_sshjobs(hist_jobunit_id, host, port, authtype, username, password, privatekey, command) VALUES(%llu, '%s', %d, %d, '%s', '%s', '%s', '%s')",
         obj->hist_jobunit_id, esc_host, obj->port, obj->authtype,
         esc_username, esc_password, esc_privatekey, esc_command);
    obj->id = id;

    jhk_free(esc_host);
    jhk_free(esc_username);
    jhk_free(esc_password);
    jhk_free(esc_privatekey);
    jhk_free(esc_command);

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
int sshjob_put_process(const apr_uint64_t jobunit_id,
                       const apr_uint64_t proc_jobunit_id)
{
    int rc;
    sshjob_t *obj = NULL;

    jhklog_debug("In %s() jobunit_id: %llu, proc_jobunit_id: %llu",
                 __func__, jobunit_id, proc_jobunit_id);
    rc = -1;
    obj = sshjob_new();

    if (sshjob_select(obj, jobunit_id) != 0)
        goto error;

    obj->proc_jobunit_id = proc_jobunit_id;
    if (proc_sshjob_insert(obj) == 0)
        goto error;

    rc = 0;
  error:
    sshjob_destroy(obj);
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
int sshjob_put_history(const apr_uint64_t proc_jobunit_id,
                       const apr_uint64_t hist_jobunit_id)
{
    int rc;
    sshjob_t *obj = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu, hist_jobunit_id: %llu",
                 __func__, proc_jobunit_id, hist_jobunit_id);
    rc = -1;
    obj = sshjob_new();

    if (proc_sshjob_select(obj, proc_jobunit_id) != 0)
        goto error;

    obj->hist_jobunit_id = hist_jobunit_id;
    if (hist_sshjob_insert(obj) == 0)
        goto error;

    rc = 0;
  error:
    sshjob_destroy(obj);
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
int proc_sshjob_delete(const apr_uint64_t proc_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM proc_sshjobs WHERE proc_jobunit_id = %llu",
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
int hist_sshjob_delete(const apr_uint64_t hist_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM hist_sshjobs WHERE hist_jobunit_id = %llu",
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
int sshjob_execute(jobunit_t * proc_jobunit)
{
    int rc, rc_ssh;
    sshjob_t *obj = NULL;
    jobresult_t *obj_res = NULL;
    jhkssh_t *obj_ssh = NULL;
    char *var_host;

    if (proc_jobunit == NULL)
        return -1;

    jhklog_trace("In %s() proc_jobunit_id: %llu, parent_id: %llu",
                 __func__, proc_jobunit->id, proc_jobunit->parent_id);

    // init
    rc = -1;
    obj = sshjob_new();
    obj_res = jobresult_new();
    obj_ssh = jhkssh_new();
    var_host = NULL;

    // load sshjob data
    if (proc_sshjob_select(obj, proc_jobunit->id)
        != 0)
        goto error;

    jhklog_debug("In %s() host: %s, port: %d, username: %s, command: %s",
                 __func__, obj->host, obj->port, obj->username,
                 obj->command);

    // replace variable
    var_host = variable_replace(proc_jobunit->parent_id, obj->host);
    jhklog_debug("In %s() replace variable. host: %s", __func__, var_host);

    // create ssh session
    if (jhkssh_session(obj_ssh, var_host, obj->port) != 0) {
        execlog_error(proc_jobunit->id,
                      "Failed to connect the host '%s:%d'", var_host,
                      obj->port);
        goto error;
    }
    // ssh userauth
    rc_ssh = -1;
    switch (obj->authtype) {
    case SSHJOB_AUTHTYPE_PASSWORD:
        rc_ssh =
            jhkssh_userauth_password(obj_ssh, obj->username,
                                     obj->password);
        break;
    case SSHJOB_AUTHTYPE_PUBLICKEY:
        rc_ssh =
            jhkssh_userauth_publickey(obj_ssh, obj->username,
                                      obj->privatekey, obj->password);
        break;
    default:
        break;
    }
    if (rc_ssh != 0) {
        execlog_error(proc_jobunit->id,
                      "Authentication failed. username: '%s', authtype: %d",
                      obj->username, obj->authtype);
        goto error;
    }
    // ssh exec command
    jhkdb_disconnect();
    rc_ssh = jhkssh_exec(obj_ssh, obj_res, obj->command);
    jhkdb_connect();
    if (rc_ssh != 0) {
        execlog_error(proc_jobunit->id, "The command is failed");
        goto error;
    }
    // insert job result
    obj_res->proc_jobunit_id = proc_jobunit->id;
    if (proc_jobresult_insert(obj_res) == 0)
        goto error;

    // check conditions
    rc = jobresult_execute(obj_res);

  error:
    jhk_free(var_host);
    jhkssh_destroy(obj_ssh);
    jobresult_destroy(obj_res);
    sshjob_destroy(obj);
    return rc;
}
