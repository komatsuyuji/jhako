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

#ifndef JHAKO_SSHJOB_H
#define JHAKO_SSHJOB_H

#include <apr_portable.h>
#include <dbi/dbi.h>
#include "models.h"

typedef struct {
    apr_uint64_t id;
    apr_uint64_t jobunit_id;
    apr_uint64_t proc_jobunit_id;
    apr_uint64_t hist_jobunit_id;
    int authtype;
    char *host;
    char *user;
    char *password;
    char *privatekey;
    int port;
    char *command;
} sshjob_t;

#define SSHJOB_AUTHTYPE_PASSWORD 0
#define SSHJOB_AUTHTYPE_PUBLICKEY 1

sshjob_t *sshjob_new(void);
void sshjob_destroy(sshjob_t * obj);
int sshjob_init(sshjob_t * obj);
int sshjob_load(sshjob_t * obj, dbi_result res);
int sshjob_load_one(sshjob_t * obj, dbi_result res);

int sshjob_select(sshjob_t * obj, const apr_uint64_t jobunit_id);
int proc_sshjob_select(sshjob_t * obj, const apr_uint64_t proc_jobunit_id);
int hist_sshjob_select(sshjob_t * obj, const apr_uint64_t hist_jobunit_id);

apr_uint64_t proc_sshjob_insert(sshjob_t * obj);
apr_uint64_t hist_sshjob_insert(sshjob_t * obj);

int sshjob_put_process(const apr_uint64_t jobunit_id,
                       const apr_uint64_t proc_jobunit_id);
int sshjob_put_history(const apr_uint64_t proc_jobunit_id,
                       const apr_uint64_t hist_jobunit_id);

int proc_sshjob_delete(const apr_uint64_t proc_jobunit_id);
int hist_sshjob_delete(const apr_uint64_t hist_jobunit_id);

int sshjob_execute(jobunit_t * proc_jobunit);

#endif
