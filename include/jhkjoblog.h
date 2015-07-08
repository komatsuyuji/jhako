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

#ifndef JHAKO_JHKJOBLOG_H
#define JHAKO_JHKJOBLOG_H

#include <apr_portable.h>
#include "models.h"

typedef struct {
    apr_pool_t *pool;
    apr_file_t *fp;
    char *joblogfile;
    char today[9];
    char *logfile;
} jhkjoblog_t;

int jhkjoblog_create(char *joblogfile);
void jhkjoblog_destroy(void);

int jhkjoblog_open(void);
void jhkjoblog_close(void);
void jhkjoblog_lock(void);
void jhkjoblog_unlock(void);
int jhkjoblog_filechk(void);

const char *jhkjoblog_kind(int kind);
const char *jhkjoblog_status(int status);
const char *jhkjoblog_timeout_occur(int timeout_occur);

int jhkjoblog_write(const char *message);
int jhkjoblog_jobunit(jobunit_t * obj);

#endif
