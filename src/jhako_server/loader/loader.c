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

#include "common.h"
#include "jhklog.h"
#include "jhkdb.h"
#include "models.h"
#include "loader.h"

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
void loader_mainloop(void)
{
    pid_t ppid;
    int cnt;

    jhklog_info("loader start");

    if (jhkdb_connect() != 0) {
        jhklog_crit("In %s() can not connect database", __func__);
        return;
    }

    cnt = 0;
    while (1) {
        ppid = getppid();
        if (ppid <= 1) {
            jhklog_debug("In %s() ppid: %d", __func__, ppid);
            break;
        }
        if (cnt >= 10) {
            cnt = 0;
            loader_exec();
        } else {
            cnt++;
        }
        sleep(1);
    }

    jhkdb_disconnect();
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
int loader_exec(void)
{
    apr_uint64_t id;
    dbi_result result = NULL;
    dbi_result res_rootjobnet = NULL;
    rootjobnet_t *obj = NULL;

    jhklog_trace("In %s()", __func__);
    result =
        jhkdb_select
        ("SELECT id FROM rootjobnets WHERE user_id = 0 AND disabled = FALSE");
    if (result == NULL)
        return -1;

    obj = rootjobnet_new();
    while (dbi_result_next_row(result)) {
        jhkdb_begin();
        id = dbi_result_get_ulonglong(result, "id");
        res_rootjobnet =
            jhkdb_select
            ("SELECT * FROM rootjobnets WHERE id = %llu AND user_id = 0 AND disabled = FALSE FOR UPDATE",
             id);
        if (res_rootjobnet == NULL) {
            jhkdb_rollback();
            continue;
        }

        rootjobnet_load_one(obj, res_rootjobnet);
        dbi_result_free(res_rootjobnet);

        if (loader_rootjobnet(obj) < 0) {
            jhkdb_rollback();
            continue;
        }

        jhkdb_commit();
    }

    rootjobnet_destroy(obj);
    dbi_result_free(result);
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function:
//
// Purpose:
//
// Parameters:
//
// Return value:
//
// Author: Komatsu Yuji(Zheng Chuyu)
//
/////////////////////////////////////////////////////////////////////////////////
int loader_rootjobnet(rootjobnet_t * obj)
{
    time_t now;

    if (obj == NULL)
        return -1;

    jhklog_trace("In %s() id: %llu", __func__, obj->id);

    if (!(obj->user_id == 0 && obj->disabled == 0))
        return 0;

    now = time(NULL);

    // check ative period
    if (rootjobnet_on_active(obj, now) != JHAKO_TRUE) {
        jhklog_trace
            ("In %s() rootjobnet is not active. rootjobnet_id: %llu, now: %ld",
             __func__, obj->id, now);
        return 0;
    }
    // check calendar date
    if (calendar_date_ondate(obj->calendar_id, now) != JHAKO_TRUE) {
        jhklog_trace
            ("In %s() rootjobnet is not on calendar. rootjobnet_id: %llu, now: %ld",
             __func__, obj->id, now);
        return 0;
    }
    // create schedule
    return rootjobnet_scheduling(obj, now);
}
