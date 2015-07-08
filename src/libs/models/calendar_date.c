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
calendar_date_t *calendar_date_new(void)
{
    calendar_date_t *obj;

    jhklog_trace("In %s()", __func__);

    obj = (calendar_date_t *) malloc(sizeof(calendar_date_t));
    calendar_date_init(obj);

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
void calendar_date_destroy(calendar_date_t * obj)
{
    jhklog_trace("In %s()", __func__);

    if (obj == NULL)
        return;

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
int calendar_date_init(calendar_date_t * obj)
{
    jhklog_trace("In %s()", __func__);

    if (obj == NULL)
        return -1;

    obj->id = 0;
    obj->calendar_id = 0;
    obj->caledate = 0;

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
int calendar_date_load(calendar_date_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);

    if (obj == NULL || res == NULL)
        return -1;

    calendar_date_init(obj);
    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->calendar_id = dbi_result_get_ulonglong(res, "calendar_id");
    obj->caledate = jhkdb_get_datetime(res, "caledate");

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
int calendar_date_ondate(const apr_uint64_t calendar_id,
                         const time_t cale_time)
{
    int rv;
    char caledate[11];
    struct tm *tm;
    dbi_result result = NULL;

    jhklog_trace("In %s() calendar_id: %llu, cale_time: %ld",
                 __func__, calendar_id, cale_time);
    if (cale_time == 0) {
        jhklog_warn("In %s() cale_time is unset", __func__);
        return JHAKO_ERROR;
    }

    if (calendar_id == 0) {
        jhklog_trace("In %s() not use calendar", __func__);
        return JHAKO_TRUE;
    }

    tm = localtime(&cale_time);
    snprintf(caledate, sizeof(caledate), "%04d-%02d-%02d",
             tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
    jhklog_trace("In %s() caledate: %s", __func__, caledate);

    result =
        jhkdb_select
        ("SELECT id FROM calendar_dates WHERE calendar_id = %llu and caledate = '%s'",
         calendar_id, caledate);
    if (result == NULL)
        return JHAKO_ERROR;

    if (dbi_result_get_numrows(result) > 0) {
        // on calendar
        rv = JHAKO_TRUE;
    } else {
        rv = JHAKO_FALSE;
    }

    dbi_result_free(result);
    return rv;
}
