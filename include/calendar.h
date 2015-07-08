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

#ifndef JHAKO_CALENDAR_H
#define JHAKO_CALENDAR_H

#include <time.h>
#include <apr_portable.h>
#include <dbi/dbi.h>

typedef struct {
    apr_uint64_t id;
    char *name;
    char *description;
    int kind;
    time_t created_at;
    time_t updated_at;
} calendar_t;

calendar_t *calendar_new(void);
void calendar_destroy(calendar_t * obj);
int calendar_init(calendar_t * obj);
int calendar_load(calendar_t * obj, dbi_result res);
int calendar_load_one(calendar_t * obj, dbi_result res);
int calendar_select(calendar_t * obj, const apr_uint64_t id);

#endif
