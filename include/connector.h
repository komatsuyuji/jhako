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

#ifndef JHAKO_CONNECTOR_H
#define JHAKO_CONNECTOR_H

#include <apr_portable.h>
#include <dbi/dbi.h>

typedef struct {
    apr_uint64_t id;
    apr_uint64_t jobunit_id;
    apr_uint64_t proc_jobunit_id;
    apr_uint64_t hist_jobunit_id;
    int kind;
    apr_uint64_t prev_jobid;
    apr_uint64_t next_jobid;
} connector_t;

typedef struct {
    int num;
    connector_t **conns;
} connectors_t;

connector_t *connector_new(void);
void connector_destroy(connector_t * obj);
int connector_init(connector_t * obj);
int connector_load(connector_t * obj, dbi_result res);

apr_uint64_t proc_connector_insert(connector_t * obj);
apr_uint64_t hist_connector_insert(connector_t * obj);
int proc_connector_delete(const apr_uint64_t proc_jobunit_id);
int hist_connector_delete(const apr_uint64_t hist_jobunit_id);

connectors_t *connectors_new(void);
void connectors_destroy(connectors_t * obj);
int connectors_init(connectors_t * obj);
int connectors_create(connectors_t * obj, const int num);
int connectors_load(connectors_t * obj, dbi_result res);

int connectors_select(connectors_t * obj, const apr_uint64_t jobunit_id);
int proc_connectors_select(connectors_t * obj,
                           const apr_uint64_t proc_jobunit_id);

int proc_connectors_insert(connectors_t * obj);
int hist_connectors_insert(connectors_t * obj);

int connectors_calc_boot(connectors_t * obj,
                         const apr_uint64_t next_jobid);
int connectors_renew_jobid(connectors_t * obj, connectors_t * obj_oldconns,
                           const apr_uint64_t jobnet_id,
                           const apr_uint64_t old_jobid,
                           const apr_uint64_t new_jobid);


#endif
