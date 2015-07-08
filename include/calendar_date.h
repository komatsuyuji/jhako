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

#ifndef JHAKO_CALENDAR_DATE_H
#define JHAKO_CALENDAR_DATE_H

#include <time.h>
#include <apr_portable.h>
#include <dbi/dbi.h>

typedef struct {
    apr_uint64_t id;
    apr_uint64_t calendar_id;
    time_t caledate;
} calendar_date_t;

calendar_date_t *calendar_date_new(void);
void calendar_date_destroy(calendar_date_t * obj);
int calendar_date_init(calendar_date_t * obj);
int calendar_date_load(calendar_date_t * obj, dbi_result res);

int calendar_date_ondate(const apr_uint64_t calendar_id,
                         const time_t cale_time);

#endif
