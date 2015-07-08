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

#ifndef JHAKO_JHKLOG_H
#define JHAKO_JHKLOG_H

#include <apr_portable.h>
#include "jhkconf.h"

#define LOG_LEVEL_UNKNOWN 0
#define LOG_LEVEL_TRACE   1
#define LOG_LEVEL_DEBUG   2
#define LOG_LEVEL_INFO    3
#define LOG_LEVEL_NOTICE  4
#define LOG_LEVEL_WARN    5
#define LOG_LEVEL_ERROR   6
#define LOG_LEVEL_CRIT    7
#define LOG_LEVEL_ALERT   8
#define LOG_LEVEL_FATAL   9

typedef struct {
    apr_pool_t *pool;
    apr_file_t *fp;
    char *logfile;
    int loglevel;
    apr_off_t logsize;
} jhklog_t;

int jhklog_create(jhkconf_t * cfg);
void jhklog_destroy(void);
int jhklog_open(void);
void jhklog_close(void);
void jhklog_lock(void);
void jhklog_unlock(void);

int jhklog_rotate(void);
int jhklog_write(const char *message);
int jhklog_append(const int level, const char *fmt, va_list va);
int jhklog_trace(const char *fmt, ...);
int jhklog_debug(const char *fmt, ...);
int jhklog_info(const char *fmt, ...);
int jhklog_warn(const char *fmt, ...);
int jhklog_error(const char *fmt, ...);
int jhklog_crit(const char *fmt, ...);

#endif
