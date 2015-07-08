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

#ifndef JHAKO_ROOTJOBNET_H
#define JHAKO_ROOTJOBNET_H

#include <time.h>
#include <apr_portable.h>
#include <dbi/dbi.h>

typedef struct {
    apr_uint64_t id;
    apr_uint64_t jobunit_id;
    apr_uint64_t proc_jobunit_id;
    apr_uint64_t hist_jobunit_id;
    apr_uint64_t user_id;
    int disabled;
    time_t active_since;
    time_t active_till;
    apr_uint64_t calendar_id;
    int run_type;
    int delay_limit;
    int loader_interval;
} rootjobnet_t;

rootjobnet_t *rootjobnet_new(void);
void rootjobnet_destroy(rootjobnet_t * obj);
int rootjobnet_init(rootjobnet_t * obj);
int rootjobnet_load(rootjobnet_t * obj, dbi_result res);

int rootjobnet_select(rootjobnet_t * obj, const apr_uint64_t jobunit_id);
int proc_rootjobnet_select(rootjobnet_t * obj,
                           const apr_uint64_t proc_jobunit_id);
int hist_rootjobnet_select(rootjobnet_t * obj,
                           const apr_uint64_t hist_jobunit_id);

apr_uint64_t proc_rootjobnet_insert(rootjobnet_t * obj);
apr_uint64_t hist_rootjobnet_insert(rootjobnet_t * obj);

int rootjobnet_put_process(const apr_uint64_t jobunit_id,
                           const apr_uint64_t proc_jobunit_id);
int rootjobnet_put_history(const apr_uint64_t proc_jobunit_id,
                           const apr_uint64_t hist_jobunit_id);

int proc_rootjobnet_delete(const apr_uint64_t proc_jobunit_id);
int hist_rootjobnet_delete(const apr_uint64_t hist_jobunit_id);

int rootjobnet_on_active(rootjobnet_t * obj, const time_t t);
int rootjobnet_scheduling(rootjobnet_t * obj, const time_t t);

#endif
