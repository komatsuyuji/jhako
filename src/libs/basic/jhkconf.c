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

#include <confuse.h>
#include "common.h"
#include "jhkconf.h"

/////////////////////////////////////////////////////////////////////////////////
static jhkconf_t *jx_cfg = NULL;

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
int jhkconf_create(const char *conffile)
{
    int rc = -1;
    cfg_t *cfg;
    int rc_cfg;

    if (jx_cfg != NULL)
        return -1;
    if (conffile == NULL)
        return -1;

    jx_cfg = (jhkconf_t *) malloc(sizeof(jhkconf_t));
    jx_cfg->pidfile = NULL;
    jx_cfg->logfile = NULL;
    jx_cfg->loglevel = -1;
    jx_cfg->logsize = -1;
    jx_cfg->tmpdir = NULL;
    jx_cfg->dbadapter = NULL;
    jx_cfg->dbhost = NULL;
    jx_cfg->dbname = NULL;
    jx_cfg->dbuser = NULL;
    jx_cfg->dbpassword = NULL;
    jx_cfg->dbencoding = NULL;
    jx_cfg->joblogfile = NULL;

    cfg_opt_t opts[] = {
        CFG_SIMPLE_STR("PidFile", &(jx_cfg->pidfile)),
        CFG_SIMPLE_STR("LogFile", &(jx_cfg->logfile)),
        CFG_SIMPLE_INT("LogLevel", &(jx_cfg->loglevel)),
        CFG_SIMPLE_INT("LogSize", &(jx_cfg->logsize)),
        CFG_SIMPLE_STR("TmpDir", &(jx_cfg->tmpdir)),
        CFG_SIMPLE_STR("DBAdapter", &(jx_cfg->dbadapter)),
        CFG_SIMPLE_STR("DBHost", &(jx_cfg->dbhost)),
        CFG_SIMPLE_STR("DBName", &(jx_cfg->dbname)),
        CFG_SIMPLE_STR("DBUser", &(jx_cfg->dbuser)),
        CFG_SIMPLE_STR("DBPassword", &(jx_cfg->dbpassword)),
        CFG_SIMPLE_STR("DBEncoding", &(jx_cfg->dbencoding)),
        CFG_SIMPLE_STR("JobLogFile", &(jx_cfg->joblogfile)),
        CFG_END()
    };

    cfg = cfg_init(opts, 0);
    rc_cfg = cfg_parse(cfg, conffile);
    if (rc_cfg != CFG_SUCCESS) {
        goto error;
    }

    if (jx_cfg->pidfile == NULL)
        jx_cfg->pidfile = strdup("/tmp/jhako_server.pid");
    if (jx_cfg->logfile == NULL)
        jx_cfg->logfile = strdup("/tmp/jhako_server.log");
    if (jx_cfg->loglevel == -1)
        jx_cfg->loglevel = 3;
    if (jx_cfg->logsize == -1)
        jx_cfg->logsize = 1;
    if (jx_cfg->tmpdir == NULL)
        jx_cfg->tmpdir = strdup("");
    if (jx_cfg->dbadapter == NULL)
        jx_cfg->dbadapter = strdup("mysql");
    if (jx_cfg->dbhost == NULL)
        jx_cfg->dbhost = strdup("localhost");
    if (jx_cfg->dbname == NULL)
        jx_cfg->dbname = strdup("jhako");
    if (jx_cfg->dbuser == NULL)
        jx_cfg->dbuser = strdup("jhako");
    if (jx_cfg->dbpassword == NULL)
        jx_cfg->dbpassword = strdup("jhako");
    if (jx_cfg->dbencoding == NULL)
        jx_cfg->dbencoding = strdup("UTF-8");
    if (jx_cfg->joblogfile == NULL)
        jx_cfg->joblogfile = strdup("");

    rc = 0;
  error:
    if (rc != 0) {
        jhkconf_destroy();
        if (rc_cfg == CFG_FILE_ERROR) {
            fprintf(stderr, "Can not read the conf file. %s\n", conffile);
        }
    }
    cfg_free(cfg);
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
void jhkconf_destroy(void)
{
    if (jx_cfg == NULL)
        return;

    jhk_free(jx_cfg->pidfile);
    jhk_free(jx_cfg->logfile);
    jhk_free(jx_cfg->tmpdir);
    jhk_free(jx_cfg->dbadapter);
    jhk_free(jx_cfg->dbhost);
    jhk_free(jx_cfg->dbname);
    jhk_free(jx_cfg->dbuser);
    jhk_free(jx_cfg->dbpassword);
    jhk_free(jx_cfg->dbencoding);
    jhk_free(jx_cfg->joblogfile);

    jhk_free(jx_cfg);
    jx_cfg = NULL;
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
jhkconf_t *jhkconf_get(void)
{
    return jx_cfg;
}
