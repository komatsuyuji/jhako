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

#ifndef JHAKO_JHKDB_H
#define JHAKO_JHKDB_H

#include <apr_portable.h>
#include <dbi/dbi.h>
#include "jhkconf.h"

// connection
int jhkdb_create(jhkconf_t * cfg);
void jhkdb_destroy(void);
int jhkdb_connect(void);
void jhkdb_disconnect(void);

// basic db query 
dbi_result jhkdb_query(const char *sql);
dbi_result jhkdb_query_valist(const char *fmt, va_list va);

int jhkdb_begin(void);
int jhkdb_commit(void);
int jhkdb_rollback(void);

dbi_result jhkdb_select(const char *fmt, ...);
apr_uint64_t jhkdb_insert(const char *fmt, ...);
int jhkdb_execute(const char *fmt, ...);

char *jhkdb_get_string(dbi_result result, const char *fieldname);
time_t jhkdb_get_datetime(dbi_result result, const char *fieldname);

char *jhkdb_escape_datetime(const time_t t);
char *jhkdb_escape_string(char *str);

#endif
