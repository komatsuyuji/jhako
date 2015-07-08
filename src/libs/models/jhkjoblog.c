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
#include "models.h"

/////////////////////////////////////////////////////////////////////////////////
static jhkjoblog_t *jx_joblog = NULL;

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
int jhkjoblog_create(char *joblogfile)
{
    if (jx_joblog != NULL)
        return -1;

    jx_joblog = (jhkjoblog_t *) malloc(sizeof(jhkjoblog_t));

    apr_pool_create(&(jx_joblog->pool), jx_pool);
    jx_joblog->fp = NULL;
    jx_joblog->joblogfile = strdup(joblogfile);
    jx_joblog->logfile = (char *) malloc(strlen(joblogfile) + 10);

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
void jhkjoblog_destroy(void)
{
    if (jx_joblog == NULL)
        return;

    jhklog_close();
    apr_pool_destroy(jx_joblog->pool);

    jhk_free(jx_joblog->joblogfile);
    jhk_free(jx_joblog->logfile);

    jhk_free(jx_joblog);
    jx_joblog = NULL;
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
int jhkjoblog_open(void)
{
    apr_status_t s;
    char errmsg[100];

    if (jx_joblog == NULL)
        return -1;

    if (strlen(jx_joblog->joblogfile) == 0)
        return 0;

    sprintf(jx_joblog->today, "%s", jhk_today());
    sprintf(jx_joblog->logfile, "%s-%s", jx_joblog->joblogfile,
            jx_joblog->today);

    s = apr_file_open(&(jx_joblog->fp), jx_joblog->logfile,
                      APR_WRITE | APR_CREATE | APR_APPEND, APR_OS_DEFAULT,
                      jx_joblog->pool);
    if (s != APR_SUCCESS) {
        apr_strerror(s, errmsg, sizeof(errmsg));
        jhklog_error("can not open the file [%s]. %s\n",
                     jx_joblog->logfile, errmsg);
        return -1;
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
void jhkjoblog_close(void)
{
    if (jx_joblog == NULL)
        return;

    if (jx_joblog->fp != NULL)
        apr_file_close(jx_joblog->fp);
    jx_joblog->fp = NULL;

    apr_pool_clear(jx_joblog->pool);
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
void jhkjoblog_lock(void)
{
    if (jx_joblog == NULL)
        return;

    if (jx_joblog->fp != NULL)
        apr_file_lock(jx_joblog->fp, APR_FLOCK_EXCLUSIVE);
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
void jhkjoblog_unlock(void)
{
    if (jx_joblog == NULL)
        return;

    if (jx_joblog->fp != NULL)
        apr_file_unlock(jx_joblog->fp);
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
int jhkjoblog_filechk(void)
{
    apr_status_t rv;
    apr_finfo_t finfo, finfo_log;

    rv = apr_stat(&finfo_log, jx_joblog->logfile, APR_FINFO_INODE,
                  jx_joblog->pool);
    if (rv != APR_SUCCESS)
        return -1;

    rv = apr_file_info_get(&finfo, APR_FINFO_INODE, jx_joblog->fp);
    if (rv != APR_SUCCESS)
        return -1;

    if (finfo.inode != finfo_log.inode)
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
const char *jhkjoblog_kind(int kind)
{
    switch (kind) {
    case JOBUNIT_KIND_JOBGROUP:
        return "JOBUNIT_KIND_JOBGROUP";
        break;
    case JOBUNIT_KIND_ROOTJOBNET:
        return "JOBUNIT_KIND_ROOTJOBNET";
        break;
    case JOBUNIT_KIND_JOBNET:
        return "JOBUNIT_KIND_JOBNET";
        break;
    case JOBUNIT_KIND_STARTJOB:
        return "JOBUNIT_KIND_STARTJOB";
        break;
    case JOBUNIT_KIND_ENDJOB:
        return "JOBUNIT_KIND_ENDJOB";
        break;
    case JOBUNIT_KIND_MERGEJOB:
        return "JOBUNIT_KIND_MERGEJOB";
        break;
    case JOBUNIT_KIND_SLEEPJOB:
        return "JOBUNIT_KIND_SLEEPJOB";
        break;
    case JOBUNIT_KIND_CLOCKJOB:
        return "JOBUNIT_KIND_CLOCKJOB";
        break;
    case JOBUNIT_KIND_SSHJOB:
        return "JOBUNIT_KIND_SSHJOB";
        break;
    case JOBUNIT_KIND_WINJOB:
        return "JOBUNIT_KIND_WINJOB";
        break;
    default:
        return "JOBUNIT_KIND_UNKNOWN";
        break;
    }
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
const char *jhkjoblog_status(int status)
{
    switch (status) {
    case JOBUNIT_STATUS_NONE:
        return "JOBUNIT_STATUS_NONE";
        break;
    case JOBUNIT_STATUS_STANDBY:
        return "JOBUNIT_STATUS_STANDBY";
        break;
    case JOBUNIT_STATUS_READY:
        return "JOBUNIT_STATUS_READY";
        break;
    case JOBUNIT_STATUS_RUNNING:
        return "JOBUNIT_STATUS_RUNNING";
        break;
    case JOBUNIT_STATUS_END:
        return "JOBUNIT_STATUS_END";
        break;
    case JOBUNIT_STATUS_ERROR:
        return "JOBUNIT_STATUS_ERROR";
        break;
    case JOBUNIT_STATUS_OUTSCHEDULE:
        return "JOBUNIT_STATUS_OUTSCHEDULE";
        break;
    case JOBUNIT_STATUS_CARRYOVER:
        return "JOBUNIT_STATUS_CARRYOVER";
        break;
    case JOBUNIT_STATUS_FORCESTOP:
        return "JOBUNIT_STATUS_FORCESTOP";
        break;
    default:
        return "JOBUNIT_STATUS_UNKNOWN";
        break;
    }
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
const char *jhkjoblog_timeout_occur(int timeout_occur)
{
    if (timeout_occur == 1) {
        return "TIMEOUT_OCCUR";
    }

    return "";
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
int jhkjoblog_write(const char *message)
{
    int rc;
    apr_status_t s;
    char errmsg[100];

    if (jx_joblog == NULL || message == NULL)
        return -1;

    if (jx_joblog->fp == NULL)
        return 0;

    // rotate the file
    if (strcmp(jhk_today(), jx_joblog->today) != 0
        || jhkjoblog_filechk() != 0) {
        jhkjoblog_close();
        if (jhkjoblog_open() != 0) {
            return -1;
        }
    }
    // write joblog
    rc = -1;
    jhkjoblog_lock();
    s = apr_file_puts(message, jx_joblog->fp);
    if (s != APR_SUCCESS) {
        apr_strerror(s, errmsg, sizeof(errmsg));
        jhklog_error("can not write to the file [%s]. %s\n",
                     jx_joblog->logfile, errmsg);
        goto error;
    }
    apr_file_flush(jx_joblog->fp);

    rc = 0;
  error:
    jhkjoblog_unlock();
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
int jhkjoblog_jobunit(jobunit_t * obj)
{
    char message[MAX_STRING_LEN];
    char *csv_description;

    if (jx_joblog == NULL || obj == NULL)
        return -1;

    csv_description = jhk_csv(obj->description);
    snprintf(message, sizeof(message), "%s,%llu,%s,%s,%s,%s,%s,%s\n",
             jhk_timestamp(), obj->id, obj->path, obj->name,
             csv_description, jhkjoblog_kind(obj->kind),
             jhkjoblog_status(obj->status),
             jhkjoblog_timeout_occur(obj->timeout_occur));

    jhk_free(csv_description);
    return jhkjoblog_write(message);
}
