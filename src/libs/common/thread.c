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
pid_t jhk_fork(void)
{
    pid_t pid = -1;

    jhklog_trace("In %s()", __func__);
    fflush(stdout);
    fflush(stderr);
    pid = fork();

    return pid;
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
static void jhk_signal_handler(int sig, siginfo_t * siginfo, void *context)
{
    int sipid = -1;
    int siuid = -1;
    int sicode = -1;

    jhklog_debug("In %s() signal: %d, name: %s", __func__, sig,
                 apr_signal_description_get(sig));
    if (siginfo == NULL) {
        jhklog_debug("In %s() siginfo: NULL", __func__);
    } else {
        sipid = siginfo->si_pid;
        siuid = siginfo->si_uid;
        sicode = siginfo->si_code;
    }
    if (context == NULL) {
        jhklog_debug("In %s() context: NULL", __func__);
    }

    switch (sig) {
    case SIGILL:
    case SIGFPE:
    case SIGSEGV:
    case SIGBUS:
        jhklog_crit("In %s() got signal[sig: %d, name: %s]", __func__, sig,
                    apr_signal_description_get(sig));
        exit(-1);
        break;
    case SIGINT:
    case SIGQUIT:
    case SIGTERM:
        jhklog_warn
            ("In %s() got signal[sig: %d, name: %s], sender[pid: %d, uid: %d, code: %d]",
             __func__, sig, apr_signal_description_get(sig), sipid, siuid,
             sicode);
        exit(-1);
        break;
    default:
        break;
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
void jhk_set_signal_handler(void)
{
    struct sigaction phan;

    jhklog_debug("In %s()", __func__);
    phan.sa_sigaction = (void *) jhk_signal_handler;
    sigemptyset(&phan.sa_mask);
    phan.sa_flags = SA_SIGINFO;

    sigaction(SIGILL, &phan, NULL);
    sigaction(SIGFPE, &phan, NULL);
    sigaction(SIGSEGV, &phan, NULL);
    sigaction(SIGBUS, &phan, NULL);

    sigaction(SIGINT, &phan, NULL);
    sigaction(SIGQUIT, &phan, NULL);
    sigaction(SIGTERM, &phan, NULL);
}
