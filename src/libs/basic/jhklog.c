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
#include "jhkconf.h"
#include "jhklog.h"

/////////////////////////////////////////////////////////////////////////////////
static jhklog_t *jx_log = NULL;

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
int jhklog_create(jhkconf_t * cfg)
{
    if (jx_log != NULL)
        return -1;

    if (cfg == NULL || cfg->logfile == NULL)
        return -1;

    jx_log = (jhklog_t *) malloc(sizeof(jhklog_t));

    apr_pool_create(&(jx_log->pool), jx_pool);
    jx_log->fp = NULL;
    jx_log->logfile = strdup(cfg->logfile);
    jx_log->loglevel = cfg->loglevel;
    jx_log->logsize = cfg->logsize * 1024 * 1024;

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
void jhklog_destroy(void)
{
    if (jx_log == NULL)
        return;

    jhklog_close();
    apr_pool_destroy(jx_log->pool);
    jhk_free(jx_log->logfile);

    jhk_free(jx_log);
    jx_log = NULL;
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
int jhklog_open(void)
{
    apr_status_t rv;
    char errmsg[100];

    if (jx_log == NULL)
        return -1;

    rv = apr_file_open(&(jx_log->fp), jx_log->logfile,
                       APR_WRITE | APR_CREATE | APR_APPEND, APR_OS_DEFAULT,
                       jx_log->pool);
    if (rv != APR_SUCCESS) {
        apr_strerror(rv, errmsg, sizeof(errmsg));
        fprintf(stderr, "can not open the file [%s]. %s\n",
                jx_log->logfile, errmsg);
        return 1;
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
void jhklog_close(void)
{
    if (jx_log == NULL)
        return;

    if (jx_log->fp != NULL)
        apr_file_close(jx_log->fp);
    jx_log->fp = NULL;

    apr_pool_clear(jx_log->pool);
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
void jhklog_lock(void)
{
    if (jx_log == NULL)
        return;

    if (jx_log->fp != NULL)
        apr_file_lock(jx_log->fp, APR_FLOCK_EXCLUSIVE);
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
void jhklog_unlock(void)
{
    if (jx_log == NULL)
        return;

    if (jx_log->fp != NULL)
        apr_file_unlock(jx_log->fp);
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
int jhklog_rotate(void)
{
    char errmsg[100];
    char logfile_old[MAX_STRING_LEN];
    int rename_flag;
    apr_status_t rv;
    apr_finfo_t finfo, finfo_log;

    if (jx_log == NULL)
        return -1;

    rv = apr_file_info_get(&finfo, APR_FINFO_INODE, jx_log->fp);
    if (rv != APR_SUCCESS)
        return -1;

    // no rotate
    if (jx_log->logsize == 0) {
        rv = apr_stat(&finfo_log, jx_log->logfile, APR_FINFO_INODE,
                      jx_log->pool);
        if (rv == APR_SUCCESS) {
            if (finfo.inode == finfo_log.inode)
                return 0;
        }
        jhklog_unlock();
        jhklog_close();
        jhklog_open();
        jhklog_lock();
        return 0;
    }
    // don't rotate
    if (finfo.size < jx_log->logsize)
        return 0;

    // create new log file
    rename_flag = 0;
    snprintf(logfile_old, sizeof(logfile_old), "%s.old", jx_log->logfile);
    rv = apr_stat(&finfo_log, logfile_old, APR_FINFO_INODE, jx_log->pool);
    if (rv == APR_SUCCESS) {
        if (finfo.inode != finfo_log.inode) {
            rename_flag = 1;
        }
    } else {
        rename_flag = 1;
    }

    if (rename_flag == 1) {
        rv = apr_file_rename(jx_log->logfile, logfile_old, jx_log->pool);
        if (rv != APR_SUCCESS) {
            apr_strerror(rv, errmsg, sizeof(errmsg));
            fprintf(stderr, "can not rename [%s] to [%s]. %s\n",
                    jx_log->logfile, logfile_old, errmsg);
            return -1;
        }
    }

    jhklog_unlock();
    jhklog_close();
    jhklog_open();
    jhklog_lock();

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
int jhklog_write(const char *message)
{
    int rc;
    char errmsg[100];
    apr_status_t rv;

    if (jx_log == NULL || message == NULL)
        return -1;

    if (jx_log->fp == NULL) {
        printf("%s", message);
        return 0;
    }

    rc = -1;
    jhklog_lock();

    if (jhklog_rotate() != 0)
        goto error;

    rv = apr_file_puts(message, jx_log->fp);
    if (rv != APR_SUCCESS) {
        apr_strerror(rv, errmsg, sizeof(errmsg));
        fprintf(stderr, "can not write to the file [%s]. %s\n",
                jx_log->logfile, errmsg);
        goto error;
    }
    apr_file_flush(jx_log->fp);

    rc = 0;
  error:
    jhklog_unlock();
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
int jhklog_append(const int level, const char *fmt, va_list va)
{
    char level_string[8];
    char buf[MAX_STRING_LEN];
    char message[MAX_STRING_LEN];

    if (jx_log == NULL)
        return -1;

    if (level < jx_log->loglevel)
        return 0;

    switch (level) {
    case 0:
        sprintf(level_string, "UNKNOWN");
        break;
    case 1:
        sprintf(level_string, "TRACE");
        break;
    case 2:
        sprintf(level_string, "DEBUG");
        break;
    case 3:
        sprintf(level_string, "INFO");
        break;
    case 4:
        sprintf(level_string, "NOTICE");
        break;
    case 5:
        sprintf(level_string, "WARN");
        break;
    case 6:
        sprintf(level_string, "ERROR");
        break;
    case 7:
        sprintf(level_string, "CRIT");
        break;
    case 8:
        sprintf(level_string, "ALERT");
        break;
    case 9:
        sprintf(level_string, "FATAL");
        break;
    default:
        sprintf(level_string, "UNKNOWN");
        break;
    }

    vsnprintf(buf, sizeof(buf), fmt, va);
    snprintf(message, sizeof(message), "%s [%d] [%5s] %s\n",
             jhk_timestamp(), getpid(), level_string, buf);
    if (jhklog_write(message) != 0)
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
int jhklog_trace(const char *fmt, ...)
{
    int rc = -1;
    va_list va;

    va_start(va, fmt);
    rc = jhklog_append(LOG_LEVEL_TRACE, fmt, va);
    va_end(va);

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
int jhklog_debug(const char *fmt, ...)
{
    int rc = -1;
    va_list va;

    va_start(va, fmt);
    rc = jhklog_append(LOG_LEVEL_DEBUG, fmt, va);
    va_end(va);

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
int jhklog_info(const char *fmt, ...)
{
    int rc = -1;
    va_list va;

    va_start(va, fmt);
    rc = jhklog_append(LOG_LEVEL_INFO, fmt, va);
    va_end(va);

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
int jhklog_warn(const char *fmt, ...)
{
    int rc = -1;
    va_list va;

    va_start(va, fmt);
    rc = jhklog_append(LOG_LEVEL_WARN, fmt, va);
    va_end(va);

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
int jhklog_error(const char *fmt, ...)
{
    int rc = -1;
    va_list va;

    va_start(va, fmt);
    rc = jhklog_append(LOG_LEVEL_ERROR, fmt, va);
    va_end(va);

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
int jhklog_crit(const char *fmt, ...)
{
    int rc = -1;
    va_list va;

    va_start(va, fmt);
    rc = jhklog_append(LOG_LEVEL_CRIT, fmt, va);
    va_end(va);

    return rc;
}
