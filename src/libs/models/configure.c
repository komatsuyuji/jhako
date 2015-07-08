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
configure_t *configure_new(void)
{
    configure_t *obj;

    jhklog_trace("In %s()", __func__);
    obj = (configure_t *) malloc(sizeof(configure_t));

    return obj;
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
void configure_destroy(configure_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return;

    configure_init(obj);
    jhk_free(obj);

    obj = NULL;
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
int configure_init(configure_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    obj->id = 0;
    obj->end_period = 1;
    obj->error_period = 24;
    obj->outschedule_period = 1;
    obj->carryover_period = 1;
    obj->forcestop_period = 1;
    obj->history_period = 30;

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
int configure_load(configure_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    configure_init(obj);
    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->end_period = dbi_result_get_int(res, "end_period");
    obj->error_period = dbi_result_get_int(res, "error_period");
    obj->outschedule_period =
        dbi_result_get_int(res, "outschedule_period");
    obj->carryover_period = dbi_result_get_int(res, "carryover_period");
    obj->forcestop_period = dbi_result_get_int(res, "forcestop_period");
    obj->history_period = dbi_result_get_int(res, "history_period");

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
int configure_load_one(configure_t * obj, dbi_result res)
{
    apr_uint64_t num;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    num = dbi_result_get_numrows(res);
    if (num != 1) {
        jhklog_warn("In %s() can not find configure. numrows: %llu",
                    __func__, num);
        return 1;
    }

    while (dbi_result_next_row(res)) {
        configure_load(obj, res);
        break;
    }

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
int configure_select(configure_t * obj, const apr_uint64_t id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_trace("In %s() id: %llu", __func__, id);

    result = jhkdb_select("SELECT * FROM configures WHERE id = %llu", id);
    if (result == NULL)
        return -1;

    rc = configure_load_one(obj, result);

    dbi_result_free(result);
    return rc;
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
int configure_get(configure_t * obj)
{
    jhklog_trace("In %s()", __func__);
    return configure_select(obj, 1);
}
