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

#ifndef JHAKO_VARDATUM_H
#define JHAKO_VARDATUM_H

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
    int overwrite;
} vardatum_t;

vardatum_t *vardatum_new(void);
void vardatum_destroy(vardatum_t * obj);
int vardatum_init(vardatum_t * obj);
int vardatum_load(vardatum_t * obj, dbi_result res);

apr_uint64_t proc_vardatum_insert(vardatum_t * obj);
apr_uint64_t hist_vardatum_insert(vardatum_t * obj);

int vardata_put_process(const apr_uint64_t jobunit_id,
                        const apr_uint64_t proc_jobunit_id);
int vardata_put_history(const apr_uint64_t proc_jobunit_id,
                        const apr_uint64_t hist_jobunit_id);

int proc_vardata_delete(const apr_uint64_t proc_jobunit_id);
int hist_vardata_delete(const apr_uint64_t hist_jobunit_id);

int vardata_execute(jobunit_t * proc_jobunit);

#endif
