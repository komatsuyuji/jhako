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

#ifndef JHAKO_SCHEDULE_H
#define JHAKO_SCHEDULE_H

#include <time.h>
#include <apr_portable.h>
#include <dbi/dbi.h>

typedef struct {
    apr_uint64_t id;
    apr_uint64_t jobunit_id;
    apr_uint64_t proc_jobunit_id;
    apr_uint64_t hist_jobunit_id;
    char *week;
    char *month;
    char *day;
    char *hour;
    char *minute;
} schedule_t;

schedule_t *schedule_new(void);
void schedule_destroy(schedule_t * obj);
int schedule_init(schedule_t * obj);
int schedule_load(schedule_t * obj, dbi_result res);

apr_uint64_t proc_schedule_insert(schedule_t * obj);
apr_uint64_t hist_schedule_insert(schedule_t * obj);

int schedules_put_process(const apr_uint64_t jobunit_id,
                          const apr_uint64_t proc_jobunit_id);
int schedules_put_history(const apr_uint64_t proc_jobunit_id,
                          const apr_uint64_t hist_jobunit_id);

int proc_schedules_delete(const apr_uint64_t proc_jobunit_id);
int hist_schedules_delete(const apr_uint64_t hist_jobunit_id);

int schedule_on_time(schedule_t * obj, const time_t t);

#endif
