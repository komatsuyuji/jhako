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
calendar_t *calendar_new(void)
{
    calendar_t *obj;

    jhklog_trace("In %s()", __func__);

    obj = (calendar_t *) malloc(sizeof(calendar_t));
    obj->name = NULL;
    obj->description = NULL;

    calendar_init(obj);

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
void calendar_destroy(calendar_t * obj)
{
    jhklog_trace("In %s()", __func__);

    if (obj == NULL)
        return;

    calendar_init(obj);
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
int calendar_init(calendar_t * obj)
{
    jhklog_trace("In %s()", __func__);

    if (obj == NULL)
        return -1;

    jhk_free(obj->name);
    jhk_free(obj->description);

    obj->id = 0;
    obj->name = NULL;
    obj->description = NULL;
    obj->kind = 0;
    obj->created_at = 0;
    obj->updated_at = 0;

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
int calendar_load(calendar_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);

    if (obj == NULL || res == NULL)
        return -1;

    calendar_init(obj);
    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->name = jhkdb_get_string(res, "name");
    obj->description = jhkdb_get_string(res, "description");
    obj->kind = dbi_result_get_int(res, "kind");
    obj->created_at = jhkdb_get_datetime(res, "created_at");
    obj->updated_at = jhkdb_get_datetime(res, "updated_at");

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
int calendar_load_one(calendar_t * obj, dbi_result res)
{
    apr_uint64_t num;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    num = dbi_result_get_numrows(res);
    if (num != 1) {
        jhklog_warn("In %s() can not find calendar. numrows: %llu",
                    __func__, num);
        return 1;
    }

    while (dbi_result_next_row(res)) {
        calendar_load(obj, res);
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
int calendar_select(calendar_t * obj, const apr_uint64_t id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_trace("In %s() id: %llu", __func__, id);

    result = jhkdb_select("SELECT * FROM calendars WHERE id = %llu", id);
    if (result == NULL)
        return -1;

    rc = calendar_load_one(obj, result);

    dbi_result_free(result);
    return rc;
}
