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

#ifndef JHAKO_BOOTSTRAP_H
#define JHAKO_BOOTSTRAP_H

#include <apr_portable.h>
#include "models.h"

void bootstrap_mainloop(void);
int bootstrap_exec(void);

int bootstrap_skip(void);
int bootstrap_forcestart(void);
int bootstrap_immediate(void);

int bootstrap_topjobnets(void);
int bootstrap_topjobnet(jobunit_t * obj);
int bootstrap_running_num(const apr_uint64_t jobunit_id);
int bootstrap_running_error_num(const apr_uint64_t jobunit_id);

int bootstrap_set_ready(const apr_uint64_t proc_rootjobnet_id);
int bootstrap_set_outschedule(jobunit_t * obj);
int bootstrap_set_carryover(jobunit_t * obj);

int bootstrap_ready(void);

#endif
