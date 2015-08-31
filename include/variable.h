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

#ifndef JHAKO_VARIABLE_H
#define JHAKO_VARIABLE_H

#include <time.h>
#include <apr_portable.h>
#include <dbi/dbi.h>

typedef struct {
    apr_uint64_t id;
    apr_uint64_t jobunit_id;
    apr_uint64_t proc_jobunit_id;
    apr_uint64_t hist_jobunit_id;
    char *name;
    char *value;
    time_t created_at;
    time_t updated_at;
} variable_t;

variable_t *variable_new(void);
void variable_destroy(variable_t * obj);
int variable_init(variable_t * obj);
int variable_load(variable_t * obj, dbi_result res);
int variable_load_one(variable_t * obj, dbi_result res);

int proc_variable_select(variable_t * obj,
                         const apr_uint64_t proc_jobunit_id, char *name);
int proc_variable_create(const apr_uint64_t proc_jobunit_id, char *name,
                         char *value);
int proc_variable_renew(const apr_uint64_t proc_jobunit_id, char *name,
                        char *value);
int proc_variable_overwrite(const apr_uint64_t proc_jobunit_id, char *name,
                            char *value, const int overwrite);
int proc_variables_dup(const apr_uint64_t proc_jobunit_id,
                       const apr_uint64_t desc_jobunit_id);

apr_uint64_t hist_variable_insert(variable_t * obj);
int variables_put_history(const apr_uint64_t proc_jobunit_id,
                          const apr_uint64_t hist_jobunit_id);

int proc_variables_delete(const apr_uint64_t proc_jobunit_id);
int hist_variables_delete(const apr_uint64_t hist_jobunit_id);

char *variable_replace(const apr_uint64_t proc_jobunit_id, char *str);

#endif
