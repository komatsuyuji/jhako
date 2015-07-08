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

#ifndef JHAKO_HOUSEKEEPER_H
#define JHAKO_HOUSEKEEPER_H

#include <apr_portable.h>
#include "models.h"

void housekeeper_mainloop(void);

int housekeeper_forcestart(void);

int housekeeper_exec(void);
int housekeeper_topjobnet(jobunit_t * obj);
int housekeeper_jobnet(const apr_uint64_t proc_jobnet_id,
                       const apr_uint64_t hist_jobnet_id);

int housekeeper_history(void);

#endif
