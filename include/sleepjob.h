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

#ifndef JHAKO_SLEEPJOB_H
#define JHAKO_SLEEPJOB_H

#include <apr_portable.h>
#include <dbi/dbi.h>

typedef struct {
    apr_uint64_t id;
    apr_uint64_t jobunit_id;
    apr_uint64_t proc_jobunit_id;
    apr_uint64_t hist_jobunit_id;
    int hour;
    int minute;
    int second;
} sleepjob_t;

sleepjob_t *sleepjob_new(void);
void sleepjob_destroy(sleepjob_t * obj);
int sleepjob_init(sleepjob_t * obj);
int sleepjob_load(sleepjob_t * obj, dbi_result res);
int sleepjob_load_one(sleepjob_t * obj, dbi_result res);

int sleepjob_select(sleepjob_t * obj, const apr_uint64_t jobunit_id);
int proc_sleepjob_select(sleepjob_t * obj,
                         const apr_uint64_t proc_jobunit_id);
int hist_sleepjob_select(sleepjob_t * obj,
                         const apr_uint64_t hist_jobunit_id);

apr_uint64_t proc_sleepjob_insert(sleepjob_t * obj);
apr_uint64_t hist_sleepjob_insert(sleepjob_t * obj);

int sleepjob_put_process(const apr_uint64_t jobunit_id,
                         const apr_uint64_t proc_jobunit_id);
int sleepjob_put_history(const apr_uint64_t proc_jobunit_id,
                         const apr_uint64_t hist_jobunit_id);

int proc_sleepjob_delete(const apr_uint64_t proc_jobunit_id);
int hist_sleepjob_delete(const apr_uint64_t hist_jobunit_id);

int sleepjob_monitor(jobunit_t * proc_jobunit);

#endif
