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

#ifndef JHAKO_CONFIGURE_H
#define JHAKO_CONFIGURE_H

#include <apr_portable.h>
#include <dbi/dbi.h>

typedef struct {
    apr_uint64_t id;
    int end_period;
    int error_period;
    int outschedule_period;
    int carryover_period;
    int forcestop_period;
    int history_period;
} configure_t;

configure_t *configure_new(void);
void configure_destroy(configure_t * obj);
int configure_init(configure_t * obj);
int configure_load(configure_t * obj, dbi_result res);
int configure_load_one(configure_t * obj, dbi_result res);

int configure_select(configure_t * obj, const apr_uint64_t id);
int configure_get(configure_t * obj);

#endif
