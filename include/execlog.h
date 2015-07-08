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

#ifndef JHAKO_EXECLOG_H
#define JHAKO_EXECLOG_H

#include <apr_portable.h>
#include <dbi/dbi.h>

typedef struct {
    apr_uint64_t id;
    apr_uint64_t proc_jobunit_id;
    int kind;
    int code;
    char *message;
    time_t created_at;
    time_t updated_at;
} execlog_t;

execlog_t *execlog_new(void);
void execlog_destroy(execlog_t * obj);
int execlog_init(execlog_t * obj);
int execlog_load(execlog_t * obj, dbi_result res);
int proc_execlog_delete(const apr_uint64_t proc_jobunit_id);

apr_uint64_t execlog_append(const apr_uint64_t proc_jobunit_id,
                            const int kind, const int code,
                            const char *fmt, va_list va);
apr_uint64_t execlog_info(const apr_uint64_t proc_jobunit_id,
                          const char *fmt, ...);
apr_uint64_t execlog_warn(const apr_uint64_t proc_jobunit_id,
                          const char *fmt, ...);
apr_uint64_t execlog_error(const apr_uint64_t proc_jobunit_id,
                           const char *fmt, ...);

#endif
