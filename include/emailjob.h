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

#ifndef JHAKO_EMAILJOB_H
#define JHAKO_EMAILJOB_H

#include <apr_portable.h>
#include <dbi/dbi.h>
#include "models.h"

typedef struct {
    apr_uint64_t id;
    apr_uint64_t jobunit_id;
    apr_uint64_t proc_jobunit_id;
    apr_uint64_t hist_jobunit_id;
    char *host;
    int port;
    int auth;
    char *username;
    char *password;
    char *mail_from;
    char *mail_to;
    char *subject;
    char *body;
    int lines_read;
} emailjob_t;

emailjob_t *emailjob_new(void);
void emailjob_destroy(emailjob_t * obj);
int emailjob_init(emailjob_t * obj);
int emailjob_load(emailjob_t * obj, dbi_result res);
int emailjob_load_one(emailjob_t * obj, dbi_result res);

int emailjob_select(emailjob_t * obj, const apr_uint64_t jobunit_id);
int proc_emailjob_select(emailjob_t * obj,
                         const apr_uint64_t proc_jobunit_id);
int hist_emailjob_select(emailjob_t * obj,
                         const apr_uint64_t hist_jobunit_id);

apr_uint64_t proc_emailjob_insert(emailjob_t * obj);
apr_uint64_t hist_emailjob_insert(emailjob_t * obj);

int emailjob_put_process(const apr_uint64_t jobunit_id,
                         const apr_uint64_t proc_jobunit_id);
int emailjob_put_history(const apr_uint64_t proc_jobunit_id,
                         const apr_uint64_t hist_jobunit_id);

int proc_emailjob_delete(const apr_uint64_t proc_jobunit_id);
int hist_emailjob_delete(const apr_uint64_t hist_jobunit_id);

int emailjob_execute(jobunit_t * proc_jobunit);

#endif
