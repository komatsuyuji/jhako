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

#ifndef JHAKO_JHKWSMAN_H
#define JHAKO_JHKWSMAN_H

#include <wsman-api.h>
#include "jobresult.h"

#define XML_NS_RSP "http://schemas.microsoft.com/wbem/wsman/1/windows/shell"
#define XML_NSDATA_RSP "rsp"
#define XML_NS_P "http://schemas.microsoft.com/wbem/wsman/1/wsman.xsd"
#define XML_NSDATA_P "p"
#define XML_NS_CFG "http://schemas.microsoft.com/wbem/wsman/1/config"
#define XML_NSDATA_CFG "cfg"

#define RESOURCE_URI_CMD "http://schemas.microsoft.com/wbem/wsman/1/windows/shell/cmd"
#define ACTION_COMMAND "http://schemas.microsoft.com/wbem/wsman/1/windows/shell/Command"
#define ACTION_RECEIVE "http://schemas.microsoft.com/wbem/wsman/1/windows/shell/Receive"
#define ACTION_SIGNAL "http://schemas.microsoft.com/wbem/wsman/1/windows/shell/Signal"
#define ACTION_DELETE "http://schemas.xmlsoap.org/ws/2004/09/transfer/Delete"
#define RSP_CODE "http://schemas.microsoft.com/wbem/wsman/1/windows/shell/signal/terminate"

#define XML_LANG "xml:lang"
#define MS_WORKING_DIRECTORY "WorkingDirectory"
#define MS_IDLE_TIME_OUT "IdleTimeOut"
#define MS_SHELL "Shell"
#define MS_INPUT_STREAMS "InputStreams"
#define MS_OUTPUT_STREAMS "OutputStreams"
#define MS_DATA_LOCALE "DataLocale"
#define MS_COMMAND_LINE "CommandLine"
#define MS_COMMAND "Command"
#define MS_RECEIVE "Receive"
#define MS_DESIRED_STREAM "DesiredStream"
#define MS_SIGNAL "Signal"
#define MS_CODE "Code"

#define JHK_WSMAN_SIZE 256

typedef struct {
    WsManClient *client;
    char shell_id[JHK_WSMAN_SIZE];
    char command_id[JHK_WSMAN_SIZE];
    int command_done;
    char errmsg[JHK_WSMAN_SIZE];
} jhkwsman_t;

jhkwsman_t *jhkwsman_new(void);
void jhkwsman_destroy(jhkwsman_t * obj);
int jhkwsman_init(jhkwsman_t * obj);
int jhkwsman_set(jhkwsman_t * obj, const char *user, const char *password,
                 const char *scheme, const char *host, const int port,
                 const char *path, const char *auth);

WsXmlDocH jhkwsman_send(jhkwsman_t * obj, WsXmlDocH request);
int jhkwsman_soap_header(jhkwsman_t * obj, WsXmlDocH doc,
                         const char *action, const char *resource_uri,
                         const char *shell_id, const char *message_id);
int jhkwsman_open_shell(jhkwsman_t * obj, const char *i_stream,
                        const char *o_stream,
                        const char *working_directory,
                        const char *env_vars, const char *noprofile,
                        const int codepage, const char *idle_timeout);
int jhkwsman_run_command(jhkwsman_t * obj, const char *command,
                         const char *console_mode_stdin,
                         const char *skip_cmd_shell);
int jhkwsman_raw_get_command_output(jhkwsman_t * obj,
                                    jobresult_t * obj_jobres,
                                    const int codepage);
int jhkwsman_get_command_output(jhkwsman_t * obj, jobresult_t * obj_jobres,
                                const int codepage);
int jhkwsman_cleanup_command(jhkwsman_t * obj);
int jhkwsman_close_shell(jhkwsman_t * obj);

int jhkwsman_exec(jhkwsman_t * obj, jobresult_t * obj_jobres,
                  const char *command, const int codepage);

#endif
