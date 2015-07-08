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

/////////////////////////////////////////////////////////////////////////////////
static apr_file_t *jx_pidfile_fp = NULL;

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
int jhkpid_create(const char *pidfile)
{
    apr_status_t s;
    char errmsg[100];

    if (jx_pidfile_fp != NULL || pidfile == NULL)
        return -1;

    // open pidfile
    s = apr_file_open(&jx_pidfile_fp, pidfile, APR_WRITE | APR_CREATE,
                      APR_OS_DEFAULT, jx_pool);
    if (s != APR_SUCCESS) {
        apr_strerror(s, errmsg, sizeof(errmsg));
        jhklog_error("In %s() can not open the file [%s]. %s", __func__,
                     pidfile, errmsg);
        return -1;
    }
    // lock pidfile
    s = apr_file_lock(jx_pidfile_fp,
                      APR_FLOCK_EXCLUSIVE | APR_FLOCK_NONBLOCK);
    if (s != APR_SUCCESS) {
        apr_strerror(s, errmsg, sizeof(errmsg));
        jhklog_error("In %s() can not lock the file [%s]. %s", __func__,
                     pidfile, errmsg);
        return -1;
    }
    // write pid to pidile
    if (apr_file_printf(jx_pidfile_fp, "%d", getpid()) <= 0) {
        apr_strerror(s, errmsg, sizeof(errmsg));
        jhklog_error("In %s() can not write to the file [%s]. %s",
                     __func__, pidfile, errmsg);
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
void jhkpid_destroy(void)
{
    const char *pidfile = NULL;

    if (jx_pidfile_fp == NULL)
        return;

    apr_file_unlock(jx_pidfile_fp);
    apr_file_close(jx_pidfile_fp);
    apr_file_name_get(&pidfile, jx_pidfile_fp);
    apr_file_remove(pidfile, jx_pool);

    jx_pidfile_fp = NULL;
}
