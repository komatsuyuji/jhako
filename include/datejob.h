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

#ifndef JHAKO_DATEJOB_H
#define JHAKO_DATEJOB_H

#include <apr_portable.h>
#include <dbi/dbi.h>
#include "models.h"

typedef struct {
    apr_uint64_t id;
    apr_uint64_t jobunit_id;
    apr_uint64_t proc_jobunit_id;
    apr_uint64_t hist_jobunit_id;
    int baseday;
    int beginning_of_week;
} datejob_t;

datejob_t *datejob_new(void);
void datejob_destroy(datejob_t * obj);
int datejob_init(datejob_t * obj);
int datejob_load(datejob_t * obj, dbi_result res);
int datejob_load_one(datejob_t * obj, dbi_result res);

int datejob_select(datejob_t * obj, const apr_uint64_t jobunit_id);
int proc_datejob_select(datejob_t * obj,
                        const apr_uint64_t proc_jobunit_id);
int hist_datejob_select(datejob_t * obj,
                        const apr_uint64_t hist_jobunit_id);

apr_uint64_t proc_datejob_insert(datejob_t * obj);
apr_uint64_t hist_datejob_insert(datejob_t * obj);

int datejob_put_process(const apr_uint64_t jobunit_id,
                        const apr_uint64_t proc_jobunit_id);
int datejob_put_history(const apr_uint64_t proc_jobunit_id,
                        const apr_uint64_t hist_jobunit_id);

int proc_datejob_delete(const apr_uint64_t proc_jobunit_id);
int hist_datejob_delete(const apr_uint64_t hist_jobunit_id);

int datejob_execute(jobunit_t * obj);

#endif
