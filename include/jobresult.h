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

#ifndef JHAKO_JOBRESULT_H
#define JHAKO_JOBRESULT_H

#include <time.h>
#include <apr_portable.h>
#include <dbi/dbi.h>

#define EXIT_SIGNAL_SIZE 128
#define JOB_RESULT_SIZE 4096

typedef struct {
    apr_uint64_t id;
    apr_uint64_t proc_jobunit_id;
    apr_uint64_t hist_jobunit_id;
    int exitcode;
    char *exitsignal;
    char *stdout;
    char *stderr;
    time_t created_at;
    time_t updated_at;
} jobresult_t;

jobresult_t *jobresult_new(void);
void jobresult_destroy(jobresult_t * obj);
int jobresult_init(jobresult_t * obj);
int jobresult_load(jobresult_t * obj, dbi_result res);

int proc_jobresult_select(jobresult_t * obj,
                          const apr_uint64_t proc_jobunit_id);

apr_uint64_t proc_jobresult_insert(jobresult_t * obj);
apr_uint64_t hist_jobresult_insert(jobresult_t * obj);

int jobresult_put_history(const apr_uint64_t proc_jobunit_id,
                          const apr_uint64_t hist_jobunit_id);

int proc_jobresult_delete(const apr_uint64_t proc_jobunit_id);
int hist_jobresult_delete(const apr_uint64_t hist_jobunit_id);

int jobresult_execute(jobresult_t * obj);

#endif
