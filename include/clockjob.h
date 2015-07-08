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

#ifndef JHAKO_CLOCKJOB_H
#define JHAKO_CLOCKJOB_H

#include <apr_portable.h>
#include <dbi/dbi.h>
#include "models.h"

typedef struct {
    apr_uint64_t id;
    apr_uint64_t jobunit_id;
    apr_uint64_t proc_jobunit_id;
    apr_uint64_t hist_jobunit_id;
    int basetime;
    time_t predetermined_time;
    int day;
    int hour;
    int minute;
    int delay_limit;
    int timeover;
} clockjob_t;

clockjob_t *clockjob_new(void);
void clockjob_destroy(clockjob_t * obj);
int clockjob_init(clockjob_t * obj);
int clockjob_load(clockjob_t * obj, dbi_result res);
int clockjob_load_one(clockjob_t * obj, dbi_result res);

int clockjob_select(clockjob_t * obj, const apr_uint64_t jobunit_id);
int proc_clockjob_select(clockjob_t * obj,
                         const apr_uint64_t proc_jobunit_id);
int hist_clockjob_select(clockjob_t * obj,
                         const apr_uint64_t hist_jobunit_id);

apr_uint64_t proc_clockjob_insert(clockjob_t * obj);
apr_uint64_t hist_clockjob_insert(clockjob_t * obj);

int clockjob_put_process(const apr_uint64_t jobunit_id,
                         const apr_uint64_t proc_jobunit_id);
int clockjob_put_history(const apr_uint64_t proc_jobunit_id,
                         const apr_uint64_t hist_jobunit_id);

int proc_clockjob_delete(const apr_uint64_t proc_jobunit_id);
int hist_clockjob_delete(const apr_uint64_t hist_jobunit_id);

int clockjob_execute(jobunit_t * obj);
int clockjob_monitor(jobunit_t * obj);

#endif
