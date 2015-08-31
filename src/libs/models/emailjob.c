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

#include <curl/curl.h>
#include <wsman-api.h>

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
emailjob_t *emailjob_new(void)
{
    emailjob_t *obj;

    jhklog_trace("In %s()", __func__);
    obj = (emailjob_t *) malloc(sizeof(emailjob_t));
    obj->host = NULL;
    obj->username = NULL;
    obj->password = NULL;
    obj->mail_from = NULL;
    obj->mail_to = NULL;
    obj->subject = NULL;
    obj->body = NULL;
    emailjob_init(obj);

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
void emailjob_destroy(emailjob_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return;

    emailjob_init(obj);
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
int emailjob_init(emailjob_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    jhk_free(obj->host);
    jhk_free(obj->username);
    jhk_free(obj->password);
    jhk_free(obj->mail_from);
    jhk_free(obj->mail_to);
    jhk_free(obj->subject);
    jhk_free(obj->body);

    obj->id = 0;
    obj->jobunit_id = 0;
    obj->proc_jobunit_id = 0;
    obj->hist_jobunit_id = 0;
    obj->host = NULL;
    obj->port = 0;
    obj->auth = 0;
    obj->username = NULL;
    obj->password = NULL;
    obj->mail_from = NULL;
    obj->mail_to = NULL;
    obj->subject = NULL;
    obj->body = NULL;
    obj->lines_read = 0;

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
int emailjob_load(emailjob_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    emailjob_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->jobunit_id = dbi_result_get_ulonglong(res, "jobunit_id");
    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");
    obj->hist_jobunit_id =
        dbi_result_get_ulonglong(res, "hist_jobunit_id");

    obj->host = jhkdb_get_string(res, "host");
    obj->port = dbi_result_get_int(res, "port");
    obj->auth = dbi_result_get_int(res, "autht");
    obj->username = jhkdb_get_string(res, "username");
    obj->password = jhkdb_get_string(res, "password");
    obj->mail_from = jhkdb_get_string(res, "mail_from");
    obj->mail_to = jhkdb_get_string(res, "mail_to");
    obj->subject = jhkdb_get_string(res, "subject");
    obj->body = jhkdb_get_string(res, "body");

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
int emailjob_load_one(emailjob_t * obj, dbi_result res)
{
    apr_uint64_t num;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    num = dbi_result_get_numrows(res);
    if (num != 1) {
        jhklog_warn("In %s() can not find emailjob. numrows: %llu",
                    __func__, num);
        return 1;
    }

    while (dbi_result_next_row(res)) {
        emailjob_load(obj, res);
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
int emailjob_select(emailjob_t * obj, const apr_uint64_t jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() jobunit_id: %llu", __func__, jobunit_id);
    result =
        jhkdb_select("SELECT * FROM emailjobs WHERE jobunit_id = %llu",
                     jobunit_id);
    if (result == NULL)
        return -1;

    rc = emailjob_load_one(obj, result);

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
int proc_emailjob_select(emailjob_t * obj,
                         const apr_uint64_t proc_jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu", __func__,
                 proc_jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM proc_emailjobs WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    rc = emailjob_load_one(obj, result);

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
int hist_emailjob_select(emailjob_t * obj,
                         const apr_uint64_t hist_jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_debug("In %s() hist_jobunit_id: %llu", __func__,
                 hist_jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM hist_emailjobs WHERE hist_jobunit_id = %llu",
         hist_jobunit_id);
    if (result == NULL)
        return -1;

    rc = emailjob_load_one(obj, result);

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
apr_uint64_t proc_emailjob_insert(emailjob_t * obj)
{
    apr_uint64_t id;
    char *esc_host = NULL;
    char *esc_username = NULL;
    char *esc_password = NULL;
    char *esc_mail_from = NULL;
    char *esc_mail_to = NULL;
    char *esc_subject = NULL;
    char *esc_body = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    // escape strinq
    esc_host = jhkdb_escape_string(obj->host);
    esc_username = jhkdb_escape_string(obj->username);
    esc_password = jhkdb_escape_string(obj->password);
    esc_mail_from = jhkdb_escape_string(obj->mail_from);
    esc_mail_to = jhkdb_escape_string(obj->mail_to);
    esc_subject = jhkdb_escape_string(obj->subject);
    esc_body = jhkdb_escape_string(obj->body);

    id = jhkdb_insert
        ("INSERT INTO proc_emailjobs(proc_jobunit_id, host, port, auth, username, password, mail_from, mail_to, subject, body) \
            VALUES(%llu, '%s', %d, %d, '%s', '%s', '%s', '%s', '%s', '%s')",
         obj->proc_jobunit_id, esc_host, obj->port, obj->auth, esc_username, esc_password, esc_mail_from, esc_mail_to, esc_subject, esc_body);
    obj->id = id;

    jhk_free(esc_host);
    jhk_free(esc_username);
    jhk_free(esc_password);
    jhk_free(esc_mail_from);
    jhk_free(esc_mail_to);
    jhk_free(esc_subject);
    jhk_free(esc_body);

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
apr_uint64_t hist_emailjob_insert(emailjob_t * obj)
{
    apr_uint64_t id;
    char *esc_host = NULL;
    char *esc_username = NULL;
    char *esc_password = NULL;
    char *esc_mail_from = NULL;
    char *esc_mail_to = NULL;
    char *esc_subject = NULL;
    char *esc_body = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    // escape strinq
    esc_host = jhkdb_escape_string(obj->host);
    esc_username = jhkdb_escape_string(obj->username);
    esc_password = jhkdb_escape_string(obj->password);
    esc_mail_from = jhkdb_escape_string(obj->mail_from);
    esc_mail_to = jhkdb_escape_string(obj->mail_to);
    esc_subject = jhkdb_escape_string(obj->subject);
    esc_body = jhkdb_escape_string(obj->body);

    id = jhkdb_insert
        ("INSERT INTO hist_emailjobs(hist_jobunit_id, host, port, auth, username, password, mail_from, mail_to, subject, body) \
            VALUES(%llu, '%s', %d, %d, '%s', '%s', '%s', '%s', '%s', '%s')",
         obj->hist_jobunit_id, esc_host, obj->port, obj->auth, esc_username, esc_password, esc_mail_from, esc_mail_to, esc_subject, esc_body);
    obj->id = id;

    jhk_free(esc_host);
    jhk_free(esc_username);
    jhk_free(esc_password);
    jhk_free(esc_mail_from);
    jhk_free(esc_mail_to);
    jhk_free(esc_subject);
    jhk_free(esc_body);

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
int emailjob_put_process(const apr_uint64_t jobunit_id,
                         const apr_uint64_t proc_jobunit_id)
{
    int rc;
    emailjob_t *obj = NULL;

    jhklog_debug("In %s() jobunit_id: %llu, proc_jobunit_id: %llu",
                 __func__, jobunit_id, proc_jobunit_id);
    rc = -1;
    obj = emailjob_new();

    if (emailjob_select(obj, jobunit_id) != 0)
        goto error;

    obj->proc_jobunit_id = proc_jobunit_id;
    if (proc_emailjob_insert(obj) == 0)
        goto error;

    rc = 0;
  error:
    emailjob_destroy(obj);
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
int emailjob_put_history(const apr_uint64_t proc_jobunit_id,
                         const apr_uint64_t hist_jobunit_id)
{
    int rc;
    emailjob_t *obj = NULL;

    jhklog_debug("In %s() proc_jobunit_id: %llu, hist_jobunit_id: %llu",
                 __func__, proc_jobunit_id, hist_jobunit_id);
    rc = -1;
    obj = emailjob_new();

    if (proc_emailjob_select(obj, proc_jobunit_id) != 0)
        goto error;

    obj->hist_jobunit_id = hist_jobunit_id;
    if (hist_emailjob_insert(obj) == 0)
        goto error;

    rc = 0;
  error:
    emailjob_destroy(obj);
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
int proc_emailjob_delete(const apr_uint64_t proc_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM proc_emailjobs WHERE proc_jobunit_id = %llu",
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
int hist_emailjob_delete(const apr_uint64_t hist_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM hist_emailjobs WHERE hist_jobunit_id = %llu",
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
static size_t payload_source(void *ptr, size_t size, size_t nmemb,
                             void *userp)
{
    emailjob_t *obj = (emailjob_t *) userp;
    char data[4096];
    size_t len;

    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
        jhklog_debug("In %s() lines_read: %d, size: %d, nmemb: %d\n",
                     obj->lines_read, size, nmemb);
        return 0;
    }

    memset(data, 0, sizeof(data));
    switch (obj->lines_read) {
    case 0:
        snprintf(data, sizeof(data), "MIME-Version: 1.0\r\n");
        break;
    case 1:
        snprintf(data, sizeof(data),
                 "Content-Type: text/plain; charset=\"UTF-8\"\r\n");
        break;
    case 2:
        snprintf(data, sizeof(data), "From: %s\r\n", obj->mail_from);
        break;
    case 3:
        snprintf(data, sizeof(data), "To: %s\r\n", obj->mail_to);
        break;
    case 4:
        snprintf(data, sizeof(data), "Subject: %s\r\n", obj->subject);
        break;
    case 5:
        snprintf(data, sizeof(data), "\r\n");
        break;
    case 6:
        snprintf(data, sizeof(data), "%s\r\n", obj->body);
        break;
    default:
        break;
    }

    len = strlen(data);
    memcpy(ptr, data, len);
    obj->lines_read++;

    return len;
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
int emailjob_execute(jobunit_t * proc_jobunit)
{
    int rc;
    emailjob_t *obj = NULL;
    char *var_subject, *var_body;
    char smtp_url[256];
    CURL *curl;
    CURLcode res;
    struct curl_slist *recipients;

    // check pro_jobunit
    if (proc_jobunit == NULL)
        return -1;
    jhklog_trace("In %s() proc_jobunit_id: %llu, parent_id: %llu",
                 __func__, proc_jobunit->id, proc_jobunit->parent_id);

    // init
    rc = -1;
    obj = emailjob_new();
    var_subject = NULL;
    var_body = NULL;
    curl = curl_easy_init();
    res = CURLE_OK;
    recipients = NULL;

    // check curl
    if (curl == NULL) {
        jhklog_error("In %s() curl_easy_init() failed.", __func__);
        goto error;
    }
    // load emailjob data
    if (proc_emailjob_select(obj, proc_jobunit->id)
        != 0)
        goto error;
    jhklog_debug
        ("In %s() mail_from: %s, mail_to: %s, subject: %s, body: %s",
         __func__, obj->mail_from, obj->mail_to, obj->subject, obj->body);

    // replace variable
    var_subject = variable_replace(proc_jobunit->parent_id, obj->subject);
    var_body = variable_replace(proc_jobunit->parent_id, obj->body);
    jhk_free(obj->subject);
    jhk_free(obj->body);
    obj->subject = strdup(var_subject);
    obj->body = strdup(var_body);
    jhklog_debug("In %s() replace variable. subject: %s, body: %s",
                 __func__, var_subject, var_body);

    // create smtp_url
    snprintf(smtp_url, sizeof(smtp_url), "smtp://%s", obj->host);

    // curl setopt
    curl_easy_setopt(curl, CURLOPT_URL, smtp_url);
    curl_easy_setopt(curl, CURLOPT_PORT, obj->port);
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, obj->mail_from);

    // set mail_rcpt
    recipients = curl_slist_append(recipients, obj->mail_to);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    // set auth
    if (obj->auth == 1) {
        curl_easy_setopt(curl, CURLOPT_USERNAME, obj->username);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, obj->password);
    }
    // set mail_data
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, obj);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    // send mail
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        jhklog_warn("In %s() curl_easy_perform() failed: %s", __func__,
                    curl_easy_strerror(res));
        execlog_error(proc_jobunit->id, "%s:%d [%s]", obj->host, obj->port,
                      curl_easy_strerror(res));
        goto error;
    }

    rc = 0;
  error:
    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
    jhk_free(var_subject);
    jhk_free(var_body);
    emailjob_destroy(obj);
    return rc;
}
