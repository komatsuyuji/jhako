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

#ifndef JHAKO_EXPANDER_H
#define JHAKO_EXPANDER_H

#include <apr_portable.h>
#include "models.h"

void expander_mainloop(void);
int expander_exec(void);

int expander_topjobnet(jobunit_t * obj);
int expander_jobnet(const apr_uint64_t jobnet_id,
                    const apr_uint64_t proc_jobnet_id,
                    const char *jobnet_path);

#endif
