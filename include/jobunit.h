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

#ifndef JHAKO_JOBUNIT_H
#define JHAKO_JOBUNIT_H

#include <time.h>
#include <apr_portable.h>
#include <dbi/dbi.h>

typedef struct {
    apr_uint64_t id;
    char *name;
    char *description;
    int kind;
    apr_uint64_t parent_id;
    int x;
    int y;
    int hold;
    int skip;
    int timeout;
    time_t created_at;
    time_t updated_at;

    apr_uint64_t jobunit_id;
    int status;
    time_t start_time;
    time_t end_time;
    int timeout_occur;
    int force_start;
    int force_stop;

    int boot;
    int finish;
    pid_t pid;

    apr_uint64_t proc_jobunit_id;
    int mode;
    time_t schedule_time;
    int run_type;
    int delay_limit;

    char *path;
} jobunit_t;

jobunit_t *jobunit_new(void);
void jobunit_destroy(jobunit_t * obj);
int jobunit_init(jobunit_t * obj);
int jobunit_load(jobunit_t * obj, dbi_result res);

int jobunit_load_one(jobunit_t * obj, dbi_result res);
int jobunit_select(jobunit_t * obj, const apr_uint64_t id);
int proc_jobunit_select(jobunit_t * obj, const apr_uint64_t id);
int proc_topjobnet_select(jobunit_t * obj, const apr_uint64_t id);

apr_uint64_t proc_topjobnet_insert(jobunit_t * obj);
apr_uint64_t proc_jobunit_insert(jobunit_t * obj);
apr_uint64_t hist_jobunit_insert(jobunit_t * obj);

int proc_topjobnet_delete(const apr_uint64_t id);
int proc_jobunit_delete(const apr_uint64_t id);
int hist_jobunit_delete(const apr_uint64_t id);
int proc_jobunit_delete_parent_id(const apr_uint64_t parent_id);

time_t proc_jobunit_get_start_time(const apr_uint64_t id);
time_t proc_jobunit_get_rootjobnet_starttime(const apr_uint64_t id);

int proc_jobunit_timeout_occur(jobunit_t * obj);
int proc_jobunit_set_running(jobunit_t * obj);
int proc_jobunit_set_error(jobunit_t * obj);
int proc_jobunit_set_end(jobunit_t * obj, const int conn_kind);
int proc_jobunit_set_success(jobunit_t * obj, const int conn_kind);
int proc_jobunit_set_failure(jobunit_t * obj);
int proc_jobunit_set_finish(jobunit_t * obj, const int conn_kind);

int jobunit_get_path(const apr_uint64_t id, char *path, const apr_size_t s,
                     const int parent);


int jobunit_flowing(jobunit_t * obj, const int kind);
int jobunit_inherit_variables(jobunit_t * obj);

#endif
