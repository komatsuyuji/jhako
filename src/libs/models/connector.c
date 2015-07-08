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
connector_t *connector_new(void)
{
    connector_t *obj;

    jhklog_trace("In %s()", __func__);
    obj = (connector_t *) malloc(sizeof(connector_t));
    connector_init(obj);

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
void connector_destroy(connector_t * obj)
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
int connector_init(connector_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    obj->id = 0;
    obj->jobunit_id = 0;
    obj->proc_jobunit_id = 0;
    obj->hist_jobunit_id = 0;
    obj->kind = 0;
    obj->prev_jobid = 0;
    obj->next_jobid = 0;

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
int connector_load(connector_t * obj, dbi_result res)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    connector_init(obj);

    obj->id = dbi_result_get_ulonglong(res, "id");
    obj->jobunit_id = dbi_result_get_ulonglong(res, "jobunit_id");
    obj->proc_jobunit_id =
        dbi_result_get_ulonglong(res, "proc_jobunit_id");
    obj->hist_jobunit_id =
        dbi_result_get_ulonglong(res, "hist_jobunit_id");
    obj->kind = dbi_result_get_int(res, "kind");
    obj->prev_jobid = dbi_result_get_ulonglong(res, "prev_jobid");
    obj->next_jobid = dbi_result_get_ulonglong(res, "next_jobid");

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
apr_uint64_t proc_connector_insert(connector_t * obj)
{
    apr_uint64_t id;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    id = jhkdb_insert
        ("INSERT INTO proc_connectors(proc_jobunit_id, kind, prev_jobid, next_jobid) VALUES(%llu, %d, %llu, %llu)",
         obj->proc_jobunit_id, obj->kind, obj->prev_jobid,
         obj->next_jobid);
    obj->id = id;

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
apr_uint64_t hist_connector_insert(connector_t * obj)
{
    apr_uint64_t id;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return 0;

    id = jhkdb_insert
        ("INSERT INTO hist_connectors(hist_jobunit_id, kind, prev_jobid, next_jobid) VALUES(%llu, %d, %llu, %llu)",
         obj->hist_jobunit_id, obj->kind, obj->prev_jobid,
         obj->next_jobid);
    obj->id = id;

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
int proc_connector_delete(const apr_uint64_t proc_jobunit_id)
{
    jhklog_trace("In %s()", __func__);
    return
        jhkdb_execute
        ("DELETE FROM proc_connectors WHERE proc_jobunit_id = %llu",
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
int hist_connector_delete(const apr_uint64_t hist_jobunit_id)
{
    jhklog_trace("In %s()", __func__);
    return
        jhkdb_execute
        ("DELETE FROM hist_connectors WHERE hist_jobunit_id = %llu",
         hist_jobunit_id);
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
connectors_t *connectors_new(void)
{
    connectors_t *obj;

    jhklog_trace("In %s()", __func__);
    obj = (connectors_t *) malloc(sizeof(connectors_t));
    obj->num = 0;
    obj->conns = NULL;
    connectors_init(obj);

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
void connectors_destroy(connectors_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return;

    connectors_init(obj);
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
int connectors_init(connectors_t * obj)
{
    int i;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    for (i = 0; i < obj->num; i++) {
        connector_destroy(obj->conns[i]);
        obj->conns[i] = NULL;
    }
    jhk_free(obj->conns);
    obj->conns = NULL;
    obj->num = 0;

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
int connectors_create(connectors_t * obj, const int num)
{
    int i;

    jhklog_trace("In %s() num: %d", __func__, num);
    if (obj == NULL)
        return -1;

    connectors_init(obj);
    obj->conns = (connector_t **) malloc(sizeof(connector_t) * num);
    for (i = 0; i < num; i++) {
        obj->conns[i] = connector_new();
    }
    obj->num = num;

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
int connectors_load(connectors_t * obj, dbi_result res)
{
    int i;
    apr_uint64_t num;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || res == NULL)
        return -1;

    connectors_init(obj);
    num = dbi_result_get_numrows(res);
    if (num == 0 || dbi_result_first_row(res) != 1)
        return 0;

    connectors_create(obj, (int) num);
    i = 0;
    do {
        connector_load(obj->conns[i], res);
        i++;
    } while (dbi_result_next_row(res));

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
int connectors_select(connectors_t * obj, const apr_uint64_t jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;
    jhklog_debug("In %s() jobunit_id: %llu", __func__, jobunit_id);

    result =
        jhkdb_select("SELECT * FROM connectors WHERE jobunit_id = %llu",
                     jobunit_id);
    if (result == NULL)
        return -1;

    rc = connectors_load(obj, result);

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
int proc_connectors_select(connectors_t * obj,
                           const apr_uint64_t proc_jobunit_id)
{
    int rc;
    dbi_result result = NULL;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    jhklog_debug("In %s() proc_jobunit_id: %llu", __func__,
                 proc_jobunit_id);
    result =
        jhkdb_select
        ("SELECT * FROM proc_connectors WHERE proc_jobunit_id = %llu",
         proc_jobunit_id);
    if (result == NULL)
        return -1;

    rc = connectors_load(obj, result);

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
int proc_connectors_insert(connectors_t * obj)
{
    int i;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    jhklog_debug("In %s() num: %d", __func__, obj->num);

    for (i = 0; i < obj->num; i++) {
        if (proc_connector_insert(obj->conns[i]) == 0)
            return -1;
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
int hist_connectors_insert(connectors_t * obj)
{
    int i;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    jhklog_debug("In %s() num: %d", __func__, obj->num);
    for (i = 0; i < obj->num; i++) {
        if (hist_connector_insert(obj->conns[i]) == 0)
            return -1;
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
int connectors_calc_boot(connectors_t * obj, const apr_uint64_t next_jobid)
{
    int i, boot;

    jhklog_trace("In %s() next_jobid: %llu", __func__, next_jobid);
    if (obj == NULL || next_jobid == 0)
        return -1;

    boot = 0;
    for (i = 0; i < obj->num; i++) {
        if (obj->conns[i]->next_jobid == next_jobid)
            boot++;
    }

    jhklog_debug("In %s() next_jobid: %llu, boot: %d", __func__,
                 next_jobid, boot);

    return boot;
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
int connectors_renew_jobid(connectors_t * obj, connectors_t * obj_oldconns,
                           const apr_uint64_t jobnet_id,
                           const apr_uint64_t old_jobid,
                           const apr_uint64_t new_jobid)
{
    int i;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || obj_oldconns == NULL)
        return -1;

    jhklog_trace
        ("In %s() jobnet_id: %llu, old_jobid: %llu, new_jobid: %llu",
         __func__, jobnet_id, old_jobid, new_jobid);

    for (i = 0; i < obj->num; i++) {
        obj->conns[i]->proc_jobunit_id = jobnet_id;
        obj->conns[i]->hist_jobunit_id = jobnet_id;
        obj->conns[i]->kind = obj_oldconns->conns[i]->kind;

        if (obj_oldconns->conns[i]->prev_jobid == old_jobid)
            obj->conns[i]->prev_jobid = new_jobid;
        if (obj_oldconns->conns[i]->next_jobid == old_jobid)
            obj->conns[i]->next_jobid = new_jobid;
    }

    return 0;
}
