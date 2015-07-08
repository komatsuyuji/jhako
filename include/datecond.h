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

#ifndef JHAKO_DATECOND_H
#define JHAKO_DATECOND_H

#include <apr_portable.h>
#include <dbi/dbi.h>

typedef struct {
    apr_uint64_t id;
    apr_uint64_t jobunit_id;
    apr_uint64_t proc_jobunit_id;
    apr_uint64_t hist_jobunit_id;
    int kind;
    int cond;
    apr_uint64_t calendar_id;
    int negative;
} datecond_t;

datecond_t *datecond_new(void);
void datecond_destroy(datecond_t * obj);
int datecond_init(datecond_t * obj);
int datecond_load(datecond_t * obj, dbi_result res);

apr_uint64_t proc_datecond_insert(datecond_t * obj);
apr_uint64_t hist_datecond_insert(datecond_t * obj);

int dateconds_put_process(const apr_uint64_t jobunit_id,
                          const apr_uint64_t proc_jobunit_id);
int dateconds_put_history(const apr_uint64_t proc_jobunit_id,
                          const apr_uint64_t hist_jobunit_id);

int proc_datecond_delete(const apr_uint64_t proc_jobunit_id);
int hist_datecond_delete(const apr_uint64_t hist_jobunit_id);

int datecond_execute(datecond_t * obj, const time_t base_time,
                     const int beginning_of_week);
int dateconds_execute(const apr_uint64_t proc_jobunit_id,
                      const time_t base_time, const int beginning_of_week);

#endif
