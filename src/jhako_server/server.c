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
#include "jhkarg.h"
#include "jhkconf.h"
#include "jhklog.h"
#include "jhkjoblog.h"
#include "jhkpid.h"
#include "jhkdb.h"

#include "expander/expander.h"
#include "loader/loader.h"
#include "bootstrap/bootstrap.h"
#include "executive/executive.h"
#include "monitor/monitor.h"
#include "housekeeper/housekeeper.h"

/////////////////////////////////////////////////////////////////////////////////
apr_pool_t *jx_pool = NULL;
pid_t jx_pid = -1;

int jx_expander_forks = 1;
int jx_loader_forks = 1;
int jx_bootstrap_forks = 1;
int jx_executive_forks = 1;
int jx_monitor_forks = 1;
int jx_housekeeper_forks = 1;

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
static void jhako_on_exit()
{
    if (jx_pid != getpid())
        return;

    jhklog_info("Stopping jHako Server");
    jhkpid_destroy();
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
static void jhako_children_start(void)
{
    int i, status;
    int children_num;
    int forks = 0;

    jhklog_debug("In %s()", __func__);
    children_num =
        jx_loader_forks + jx_expander_forks + jx_bootstrap_forks +
        jx_executive_forks + jx_monitor_forks + jx_housekeeper_forks;

    for (i = 0; i < children_num; i++) {
        if (jhk_fork() == 0)
            break;
    }

    jhklog_debug("In %s() child process #%d started. pid: %d",
                 __func__, i, getpid());
    if (i >= 0 && i < (forks += jx_loader_forks)) {
        loader_mainloop();
    } else if (i < (forks += jx_expander_forks)) {
        expander_mainloop();
    } else if (i < (forks += jx_bootstrap_forks)) {
        bootstrap_mainloop();
    } else if (i < (forks += jx_executive_forks)) {
        executive_mainloop();
    } else if (i < (forks += jx_monitor_forks)) {
        monitor_mainloop();
    } else if (i < (forks += jx_housekeeper_forks)) {
        housekeeper_mainloop();
    } else if (i == children_num) {
        jhklog_debug("In %s() parent process waiting ...", __func__);
        wait(&status);
    }
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
int main(int argc, char **argv)
{
    int rc;
    jhkarg_t *argument;
    jhkconf_t *cfg;

    rc = -1;
    apr_initialize();
    apr_pool_create(&jx_pool, NULL);
    apr_signal_init(jx_pool);

    // load command line
    if (jhkarg_create(argc, argv) != 0)
        goto error;
    argument = jhkarg_get();

    // load config file
    if (jhkconf_create(argument->config) != 0)
        goto error;
    cfg = jhkconf_get();

    // log
    if (jhklog_create(cfg) != 0)
        goto error;
    if (jhklog_open() != 0)
        goto error;

    // logging the basic information
    jhklog_info("Starting jHako Server. %s v%s",
                APPLICATION_NAME, JHAKO_VERSION);
    jhklog_info("Using configuration file. %s", argument->config);

    // joblog
    if (jhkjoblog_create(cfg->joblogfile) != 0)
        goto error;
    if (jhkjoblog_open() != 0)
        goto error;

    // database
    if (jhkdb_create(cfg) != 0)
        goto error;

    // start daemon
    apr_proc_detach(APR_PROC_DETACH_DAEMONIZE);

    // parent process
    jx_pid = getpid();
    if (jhkpid_create(cfg->pidfile) != 0) {
        jhklog_crit("In %s() can not create pidfile. %s", __func__,
                    cfg->pidfile);
        goto error;
    }

    atexit(jhako_on_exit);
    jhk_set_signal_handler();
    jhako_children_start();

    rc = 0;
  error:
    jhkdb_destroy();
    jhkjoblog_destroy();
    jhklog_destroy();
    jhkconf_destroy();
    jhkarg_destroy();

    apr_terminate();
    exit(rc);
}
