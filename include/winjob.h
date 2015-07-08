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

#ifndef JHAKO_WINJOB_H
#define JHAKO_WINJOB_H

#include <apr_portable.h>
#include <dbi/dbi.h>
#include "models.h"

typedef struct {
    apr_uint64_t id;
    apr_uint64_t jobunit_id;
    apr_uint64_t proc_jobunit_id;
    apr_uint64_t hist_jobunit_id;
    char *host;
    char *user;
    char *password;
    char *scheme;
    int port;
    char *path;
    char *auth;
    int codepage;
    char *command;
} winjob_t;

winjob_t *winjob_new(void);
void winjob_destroy(winjob_t * obj);
int winjob_init(winjob_t * obj);
int winjob_load(winjob_t * obj, dbi_result res);
int winjob_load_one(winjob_t * obj, dbi_result res);

int winjob_select(winjob_t * obj, const apr_uint64_t jobunit_id);
int proc_winjob_select(winjob_t * obj, const apr_uint64_t proc_jobunit_id);
int hist_winjob_select(winjob_t * obj, const apr_uint64_t hist_jobunit_id);

apr_uint64_t proc_winjob_insert(winjob_t * obj);
apr_uint64_t hist_winjob_insert(winjob_t * obj);

int winjob_put_process(const apr_uint64_t jobunit_id,
                       const apr_uint64_t proc_jobunit_id);
int winjob_put_history(const apr_uint64_t proc_jobunit_id,
                       const apr_uint64_t hist_jobunit_id);

int proc_winjob_delete(const apr_uint64_t proc_jobunit_id);
int hist_winjob_delete(const apr_uint64_t hist_jobunit_id);

int winjob_execute(jobunit_t * proc_jobunit);

#endif
