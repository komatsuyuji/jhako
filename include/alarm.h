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

#ifndef JHAKO_ALARM_H
#define JHAKO_ALARM_H

#include <time.h>
#include <apr_portable.h>
#include <dbi/dbi.h>

typedef struct {
    apr_uint64_t id;
    apr_uint64_t jobunit_id;
    apr_uint64_t proc_jobunit_id;
    apr_uint64_t hist_jobunit_id;
    int status;
    apr_uint64_t jobnet_id;
} alarm_t;

alarm_t *alarm_new(void);
void alarm_destroy(alarm_t * obj);
int alarm_init(alarm_t * obj);
int alarm_load(alarm_t * obj, dbi_result res);
int alarm_load_one(alarm_t * obj, dbi_result res);
int proc_alarm_select(alarm_t * obj, const apr_uint64_t id);

apr_uint64_t proc_alarm_insert(alarm_t * obj);
apr_uint64_t hist_alarm_insert(alarm_t * obj);

int alarms_put_process(const apr_uint64_t jobunit_id,
                       const apr_uint64_t proc_jobunit_id);
int alarms_put_history(const apr_uint64_t proc_jobunit_id,
                       const apr_uint64_t hist_jobunit_id);

int proc_alarms_delete(const apr_uint64_t proc_jobunit_id);
int hist_alarms_delete(const apr_uint64_t hist_jobunit_id);

int alarms_execute(const apr_uint64_t proc_jobunit_id, const int status);
int alarms_set_variables(const apr_uint64_t proc_alarm_id,
                         const apr_uint64_t proc_jobunit_id);

#endif
