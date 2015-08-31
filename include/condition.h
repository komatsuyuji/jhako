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

#ifndef JHAKO_CONDITION_H
#define JHAKO_CONDITION_H

#include <apr_portable.h>
#include <dbi/dbi.h>
#include "jobresult.h"

typedef struct {
    apr_uint64_t id;
    apr_uint64_t jobunit_id;
    apr_uint64_t proc_jobunit_id;
    apr_uint64_t hist_jobunit_id;
    int method;
    int kind;
    char *cond;
    int negative;
} condition_t;

condition_t *condition_new(void);
void condition_destroy(condition_t * obj);
int condition_init(condition_t * obj);
int condition_load(condition_t * obj, dbi_result res);

apr_uint64_t proc_condition_insert(condition_t * obj);
apr_uint64_t hist_condition_insert(condition_t * obj);

int conditions_put_process(const apr_uint64_t jobunit_id,
                           const apr_uint64_t proc_jobunit_id);
int conditions_put_history(const apr_uint64_t proc_jobunit_id,
                           const apr_uint64_t hist_jobunit_id);

int proc_conditions_delete(const apr_uint64_t proc_jobunit_id);
int hist_conditions_delete(const apr_uint64_t hist_jobunit_id);

int condition_execute(condition_t * obj, jobresult_t * obj_res);
int conditions_execute(const apr_uint64_t proc_jobunit_id,
                       jobresult_t * obj_res);

#endif
