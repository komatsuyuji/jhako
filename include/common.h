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

#ifndef JHAKO_COMMON_H
#define JHAKO_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <apr_portable.h>
#include <apr_signal.h>
#include "version.h"

#define JHAKO_ERROR -1
#define JHAKO_TRUE 0
#define JHAKO_FALSE 1

#define MAX_STRING_LEN 4096
#define ONE_DAY_SECOND 86400

extern apr_pool_t *jx_pool;

// common.c
void jhk_free(void *p);
apr_size_t jhk_strncat(char *desc, char *src, apr_size_t s);

char *jhk_timestamp(void);
char *jhk_time(void);
char *jhk_today(void);
char *jhk_time2str(const time_t t);
time_t jhk_midnight(time_t t);
int jhk_time2day(time_t t);

char *jhk_csv(const char *str);

char *jhk_dformat(const char *fmt, va_list va);
char *jhk_trim(const char *str);
int jhk_aoti(const char *str);

int jhk_on_range(const int num, const char *range);
int jhk_on_cron(const int value, const char *cron);
int jhk_regexp(const char *str, const char *pattern);

// thread.c
pid_t jhk_fork(void);
void jhk_set_siginal_handler(void);

#endif
