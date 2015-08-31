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
execlog_t *exelog_new(void)
{
    execlog_t *obj;

    jhklog_trace("In %s()", __func__);
    obj = (execlog_t *) malloc(sizeof(execlog_t));
    obj->message = NULL;
    execlog_init(obj);

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
void execlog_destroy(execlog_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return;

    execlog_init(obj);
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
int execlog_init(execlog_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    jhk_free(obj->message);
    obj->id = 0;
    obj->proc_jobunit_id = 0;
    obj->kind = 0;
    obj->code = 0;
    obj->message = NULL;

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
int execlog_load(execlog_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    execlog_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");

    obj->kind = dbi_result_get_int(res, "kind");
    obj->code = dbi_result_get_int(res, "code");
    obj->message = jhkdb_get_string(res, "message");

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
int proc_execlogs_delete(const apr_uint64_t proc_jobunit_id)
{
    jhklog_trace("In %s()", __func__);

    return
        jhkdb_execute
        ("DELETE FROM proc_execlogs WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
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
apr_uint64_t execlog_append(const apr_uint64_t proc_jobunit_id,
                            const int kind, const int code,
                            const char *fmt, va_list va)
{
    apr_uint64_t id;
    char *ts;
    char buf[MAX_STRING_LEN];
    char *esc_message = NULL;

    jhklog_trace("In %s()", __func__);
    vsnprintf(buf, sizeof(buf), fmt, va);
    esc_message = jhkdb_escape_string(buf);

    ts = jhk_time();
    id = jhkdb_insert
        ("INSERT INTO proc_execlogs(proc_jobunit_id, kind, code, message, created_at, updated_at) VALUES(%llu, %d, %d, '%s', '%s', '%s')",
         proc_jobunit_id, kind, code, esc_message, ts, ts);

    jhk_free(esc_message);
    return id;
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
apr_uint64_t execlog_info(const apr_uint64_t proc_jobunit_id,
                          const char *fmt, ...)
{
    apr_uint64_t id = 0;
    va_list va;

    jhklog_trace("In %s()", __func__);
    va_start(va, fmt);
    id = execlog_append(proc_jobunit_id, EXECLOG_KIND_INFO, 0, fmt, va);
    va_end(va);

    return id;
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
apr_uint64_t execlog_warn(const apr_uint64_t proc_jobunit_id,
                          const char *fmt, ...)
{
    apr_uint64_t id = 0;
    va_list va;

    jhklog_trace("In %s()", __func__);
    va_start(va, fmt);
    id = execlog_append(proc_jobunit_id, EXECLOG_KIND_WARN, 0, fmt, va);
    va_end(va);

    return id;
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
apr_uint64_t execlog_error(const apr_uint64_t proc_jobunit_id,
                           const char *fmt, ...)
{
    apr_uint64_t id = 0;
    va_list va;

    jhklog_trace("In %s()", __func__);
    va_start(va, fmt);
    id = execlog_append(proc_jobunit_id, EXECLOG_KIND_ERROR, 0, fmt, va);
    va_end(va);

    return id;
}
