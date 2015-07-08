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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <wsman-api.h>

#include "common.h"
#include "charset.h"
#include "winjob.h"
#include "jobresult.h"
#include "jhklog.h"
#include "jhkwsman.h"

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
static void jobxwsman_log_handler(const char *str, debug_level_e level,
                                  void *user_data)
{
    jhklog_debug("%s", str);
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
jhkwsman_t *jhkwsman_new(void)
{
    jhkwsman_t *obj = NULL;

    jhklog_trace("In %s()", __func__);
    obj = (jhkwsman_t *) malloc(sizeof(jhkwsman_t));
    obj->client = NULL;

    debug_add_handler(jobxwsman_log_handler, DEBUG_LEVEL_ALWAYS, NULL);

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
void jhkwsman_destroy(jhkwsman_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return;
    jhkwsman_init(obj);
    obj = NULL;
    debug_destroy_handlers();
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
int jhkwsman_init(jhkwsman_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    if (obj->client != NULL) {
        wsmc_release(obj->client);
    }
    memset(obj->shell_id, 0, JHK_WSMAN_SIZE);
    memset(obj->command_id, 0, JHK_WSMAN_SIZE);
    memset(obj->errmsg, 0, JHK_WSMAN_SIZE);

    obj->client = NULL;
    obj->command_done = 0;

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
int jhkwsman_set(jhkwsman_t * obj, const char *user, const char *password,
                 const char *scheme, const char *host, const int port,
                 const char *path, const char *auth)
{
    int transport_timeout = 0;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    jhklog_trace("In %s() host: %s", __func__, host);

    jhkwsman_init(obj);
    obj->client = wsmc_create(host, port, path, scheme, user, password);
    if (obj->client == NULL) {
        jhklog_warn
            ("In %s() can not create wsman. scheme: %s, host: %s, port: %d, path: %s",
             __func__, scheme, host, port, path);
        return -1;
    }
    wsman_transport_set_auth_method(obj->client, auth);
    wsman_transport_set_timeout(obj->client, transport_timeout);
    wsman_transport_set_verify_host(obj->client, 2);

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
WsXmlDocH jhkwsman_send(jhkwsman_t * obj, WsXmlDocH request)
{
    char *buf;
    int size;
    WS_LASTERR_Code err;
    WsXmlDocH response;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL || request == NULL)
        return NULL;

    // init
    response = NULL;

    // trace request 
    buf = NULL;
    size = 0;
    err = WS_LASTERR_OK;
    ws_xml_dump_memory_node_tree(ws_xml_get_doc_root(request), &buf,
                                 &size);
    jhklog_debug("In %s() size: %d, request: \n%s", __func__, size, buf);
    jhk_free(buf);

    // send request
    if (wsman_send_request(obj->client, request) != 0) {
        err = wsmc_get_last_error(obj->client);
        snprintf(obj->errmsg, JHK_WSMAN_SIZE, "%s",
                 wsman_transport_get_last_error_string(err));
        goto error;
    }
    // check last error code
    err = wsmc_get_last_error(obj->client);
    if (err != WS_LASTERR_OK) {
        snprintf(obj->errmsg, JHK_WSMAN_SIZE, "%s",
                 wsman_transport_get_last_error_string(err));
        goto error;
    }

    response = wsmc_build_envelope_from_response(obj->client);
    if (response == NULL) {
        snprintf(obj->errmsg, JHK_WSMAN_SIZE, "response is NULL.");
        goto error;
    }
    // trace response 
    buf = NULL;
    size = 0;
    ws_xml_dump_memory_node_tree(ws_xml_get_doc_root(response), &buf,
                                 &size);
    jhklog_debug("In %s() size: %d, response: \n%s", __func__, size, buf);
    jhk_free(buf);

    return response;
  error:
    jhklog_warn("In %s() send request failed. endpoint: %s, error: %s",
                __func__, wsmc_get_endpoint(obj->client), obj->errmsg);
    return NULL;
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
int jhkwsman_soap_header(jhkwsman_t * obj, WsXmlDocH doc,
                         const char *action, const char *resource_uri,
                         const char *shell_id, const char *message_id)
{
    WsSerializerContextH serctx;
    WsXmlNodeH envelope;
    WsXmlNodeH header;
    WsXmlNodeH selectset;
    WsXmlNodeH node;
    char uuidBuf[100];

    // check soap xml
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || doc == NULL)
        return -1;
    envelope = ws_xml_get_soap_envelope(doc);
    header = ws_xml_get_soap_header(doc);
    if (envelope == NULL || header == NULL) {
        jhklog_warn("In %s() soap xml is incorrect", __func__);
        return -1;
    }
    // get serctx 
    serctx = wsmc_get_serialization_context(obj->client);
    generate_uuid(uuidBuf, sizeof(uuidBuf), 0);

    // add namespace
    ws_xml_ns_add(envelope, XML_NS_RSP, XML_NSDATA_RSP);
    ws_xml_ns_add(envelope, XML_NS_P, XML_NSDATA_P);
    ws_xml_ns_add(envelope, XML_NS_CFG, XML_NSDATA_CFG);

    // add header data
    // wsman
    ws_serialize_uint32(serctx, header, 153600, XML_NS_WS_MAN,
                        WSM_MAX_ENVELOPE_SIZE, 1);
    ws_serialize_str(serctx, header, "PT60S", XML_NS_WS_MAN,
                     WSM_OPERATION_TIMEOUT, 0);
    ws_serialize_str(serctx, header, resource_uri, XML_NS_WS_MAN,
                     WSM_RESOURCE_URI, 1);
    node = ws_xml_add_child(header, XML_NS_WS_MAN, MS_DATA_LOCALE, "");
    ws_xml_add_node_attr(node, NULL, SOAP_MUST_UNDERSTAND, "false");
    ws_xml_add_node_attr(node, NULL, XML_LANG, "en-US");

    // addressing
    ws_serialize_str(serctx, header, wsmc_get_endpoint(obj->client),
                     XML_NS_ADDRESSING, WSA_TO, 0);
    ws_serialize_str(serctx, header, action, XML_NS_ADDRESSING, WSA_ACTION,
                     1);
    if (message_id == NULL) {
        ws_serialize_str(serctx, header, uuidBuf, XML_NS_ADDRESSING,
                         WSA_MESSAGE_ID, 0);
    } else {
        ws_serialize_str(serctx, header, message_id, XML_NS_ADDRESSING,
                         WSA_MESSAGE_ID, 0);
    }
    node = ws_xml_add_child(header, XML_NS_ADDRESSING, WSA_REPLY_TO, NULL);
    ws_serialize_str(serctx, node, WSA_TO_ANONYMOUS, XML_NS_ADDRESSING,
                     WSA_ADDRESS, 1);

    // microsoft wsman
    node = ws_xml_add_child(header, XML_NS_P, MS_DATA_LOCALE, "");
    ws_xml_add_node_attr(node, NULL, SOAP_MUST_UNDERSTAND, "false");
    ws_xml_add_node_attr(node, NULL, XML_LANG, "en-US");

    // set shell_id
    if (shell_id == NULL)
        return 0;

    selectset =
        ws_xml_add_child(header, XML_NS_WS_MAN, WSM_SELECTOR_SET, NULL);
    node =
        ws_xml_add_child(selectset, XML_NS_WS_MAN, WSM_SELECTOR, shell_id);
    ws_xml_add_node_attr(node, NULL, WSM_NAME, "ShellId");

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
int jhkwsman_open_shell(jhkwsman_t * obj, const char *i_stream,
                        const char *o_stream,
                        const char *working_directory,
                        const char *env_vars, const char *noprofile,
                        const int codepage, const char *idle_timeout)
{
    int rc;
    WsXmlDocH rqstDoc;
    WsXmlDocH respDoc;
    WsSerializerContextH serctx;
    WsXmlNodeH header;
    WsXmlNodeH body;
    WsXmlNodeH optset;
    WsXmlNodeH rsp;
    WsXmlNodeH node;
    char *attr_value;
    char cp[8];

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    // init
    rc = -1;
    rqstDoc = ws_xml_create_envelope();
    respDoc = NULL;
    serctx = wsmc_get_serialization_context(obj->client);

    // create header data
    jhkwsman_soap_header(obj, rqstDoc, TRANSFER_ACTION_CREATE,
                         RESOURCE_URI_CMD, NULL, NULL);
    header = ws_xml_get_soap_header(rqstDoc);
    body = ws_xml_get_soap_body(rqstDoc);
    snprintf(cp, sizeof(cp), "%d", codepage);

    // add header option
    optset = ws_xml_add_child(header, XML_NS_WS_MAN, WSM_OPTION_SET, NULL);
    node = ws_xml_add_child(optset, XML_NS_WS_MAN, WSM_OPTION, noprofile);
    ws_xml_add_node_attr(node, NULL, WSM_NAME, "WINRS_NOPROFILE");
    node = ws_xml_add_child(optset, XML_NS_WS_MAN, WSM_OPTION, cp);
    ws_xml_add_node_attr(node, NULL, WSM_NAME, "WINRS_CODEPAGE");

    // add header data
    if (working_directory != NULL) {
        ws_serialize_str(serctx, header, working_directory, XML_NS_RSP,
                         MS_WORKING_DIRECTORY, 0);
    }
    if (idle_timeout != NULL) {
        ws_serialize_str(serctx, header, idle_timeout, XML_NS_RSP,
                         MS_IDLE_TIME_OUT, 0);
    }
    if (env_vars != NULL) {
    }
    // add body data
    rsp = ws_xml_add_child(body, XML_NS_RSP, MS_SHELL, "");
    node = ws_xml_add_child(rsp, XML_NS_RSP, MS_INPUT_STREAMS, "stdin");
    node =
        ws_xml_add_child(rsp, XML_NS_RSP, MS_OUTPUT_STREAMS,
                         "stdout stderr");
    // send message
    respDoc = jhkwsman_send(obj, rqstDoc);
    if (respDoc == NULL) {
        jhklog_warn("In %s() can not get response", __func__);
        goto error;
    }
    // get reponse data
    body = ws_xml_get_soap_body(respDoc);
    node = ws_xml_find_in_tree(body, XML_NS_WS_MAN, WSM_SELECTOR, 1);
    attr_value = ws_xml_find_attr_value(node, NULL, WSM_NAME);

    if (attr_value == NULL || strcmp(attr_value, "ShellId") != 0) {
        jhklog_warn
            ("In %s() can not find 'ShellId' from response document. %s",
             __func__, attr_value);
        goto error;
    }
    snprintf(obj->shell_id, JHK_WSMAN_SIZE, "%s",
             ws_xml_get_node_text(node));
    jhklog_debug("In %s() shell_id: %s", __func__, obj->shell_id);

    rc = 0;
  error:
    if (rqstDoc) {
        ws_xml_destroy_doc(rqstDoc);
    }
    if (respDoc) {
        ws_xml_destroy_doc(respDoc);
    }

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
int jhkwsman_run_command(jhkwsman_t * obj, const char *command,
                         const char *console_mode_stdin,
                         const char *skip_cmd_shell)
{
    int rc;
    WsXmlDocH rqstDoc;
    WsXmlDocH respDoc;
    WsSerializerContextH serctx;
    WsXmlNodeH header;
    WsXmlNodeH body;
    WsXmlNodeH optset;
    WsXmlNodeH cmdline;
    WsXmlNodeH node;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;
    if (obj->shell_id == NULL)
        return -1;

    // init
    rc = -1;
    rqstDoc = ws_xml_create_envelope();
    respDoc = NULL;
    serctx = wsmc_get_serialization_context(obj->client);

    // create header data
    jhkwsman_soap_header(obj, rqstDoc, ACTION_COMMAND, RESOURCE_URI_CMD,
                         obj->shell_id, NULL);
    header = ws_xml_get_soap_header(rqstDoc);
    body = ws_xml_get_soap_body(rqstDoc);

    // add header optionset
    optset = ws_xml_add_child(header, XML_NS_WS_MAN, WSM_OPTION_SET, NULL);
    node =
        ws_xml_add_child(optset, XML_NS_WS_MAN, WSM_OPTION,
                         console_mode_stdin);
    ws_xml_add_node_attr(node, NULL, WSM_NAME, "WINRS_CONSOLEMODE_STDIN");
    node =
        ws_xml_add_child(optset, XML_NS_WS_MAN, WSM_OPTION,
                         skip_cmd_shell);
    ws_xml_add_node_attr(node, NULL, WSM_NAME, "WINRS_SKIP_CMD_SHELL");

    // add body data
    cmdline = ws_xml_add_child(body, XML_NS_RSP, MS_COMMAND_LINE, NULL);
    node = ws_xml_add_child(cmdline, XML_NS_RSP, MS_COMMAND, command);

    // send message
    respDoc = jhkwsman_send(obj, rqstDoc);
    if (respDoc == NULL) {
        jhklog_warn("In %s() can not get response", __func__);
        goto error;
    }
    // get reponse data
    body = ws_xml_get_soap_body(respDoc);
    node = ws_xml_find_in_tree(body, XML_NS_RSP, "CommandId", 1);
    if (node == NULL)
        goto error;

    snprintf(obj->command_id, JHK_WSMAN_SIZE, "%s",
             ws_xml_get_node_text(node));
    jhklog_debug("In %s() command_id: %s", __func__, obj->command_id);

    rc = 0;
  error:
    if (rqstDoc) {
        ws_xml_destroy_doc(rqstDoc);
    }
    if (respDoc) {
        ws_xml_destroy_doc(respDoc);
    }

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
int jhkwsman_raw_get_command_output(jhkwsman_t * obj,
                                    jobresult_t * obj_jobres,
                                    const int codepage)
{
    int rc;
    int i, cnt;
    WsXmlDocH rqstDoc;
    WsXmlDocH respDoc;
    WsSerializerContextH serctx;
    WsXmlNodeH header;
    WsXmlNodeH body;
    WsXmlNodeH receive;
    WsXmlNodeH recres;
    WsXmlNodeH node;
    char *attr_value;
    char *text;
    char ascii[JOB_RESULT_SIZE];
    char *buf;

    jhklog_trace("In %s()", __func__);

    // check paramter
    if (obj == NULL || obj_jobres == NULL)
        return -1;
    if (obj->shell_id == NULL || obj->command_id == NULL)
        return -1;

    // init
    rc = -1;
    buf = NULL;
    rqstDoc = ws_xml_create_envelope();
    respDoc = NULL;
    serctx = wsmc_get_serialization_context(obj->client);

    // create header data
    jhkwsman_soap_header(obj, rqstDoc, ACTION_RECEIVE, RESOURCE_URI_CMD,
                         obj->shell_id, NULL);
    header = ws_xml_get_soap_header(rqstDoc);
    body = ws_xml_get_soap_body(rqstDoc);

    // add body data
    receive = ws_xml_add_child(body, XML_NS_RSP, MS_RECEIVE, NULL);
    node =
        ws_xml_add_child(receive, XML_NS_RSP, MS_DESIRED_STREAM,
                         "stdout stderr");
    ws_xml_add_node_attr(node, NULL, "CommandId", obj->command_id);

    // send message
    respDoc = jhkwsman_send(obj, rqstDoc);
    if (respDoc == NULL) {
        jhklog_warn("In %s() can not get response", __func__);
        goto error;
    }
    // get reponse data
    body = ws_xml_get_soap_body(respDoc);

    // get stdout, stderr
    recres = ws_xml_find_in_tree(body, XML_NS_RSP, "ReceiveResponse", 1);
    cnt = ws_xml_get_child_count(recres);
    for (i == 0; i < cnt; i++) {
        node = ws_xml_get_child(recres, i, XML_NS_RSP, "Stream");
        if (node == NULL)
            continue;
        attr_value = ws_xml_find_attr_value(node, NULL, "Name");
        text = ws_xml_get_node_text(node);
        if (text == NULL)
            continue;
        memset(ascii, 0, JOB_RESULT_SIZE);
        ws_base64_decode(text, strlen(text), ascii, JOB_RESULT_SIZE - 1);
        buf = charset_to_utf8(ascii, codepage);
        if (strcmp(attr_value, "stdout") == 0) {
            jhk_strncat(obj_jobres->stdout, buf, JOB_RESULT_SIZE);
        } else if (strcmp(attr_value, "stderr") == 0) {
            jhk_strncat(obj_jobres->stderr, buf, JOB_RESULT_SIZE);
        }
        jhk_free(buf);
    }

    // check command_done
    node = ws_xml_find_in_tree(body, XML_NS_RSP, "CommandState", 1);
    attr_value = ws_xml_find_attr_value(node, NULL, "State");
    if (attr_value != NULL) {
        if (strstr(attr_value, "CommandState/Done") != NULL) {
            obj->command_done = 1;
        }
    }
    // get exitcode
    if (obj->command_done == 1) {
        node = ws_xml_find_in_tree(body, XML_NS_RSP, "ExitCode", 1);
        obj_jobres->exitcode = ws_xml_get_node_ulong(node);
    }

    rc = 0;
  error:
    if (rqstDoc) {
        ws_xml_destroy_doc(rqstDoc);
    }
    if (respDoc) {
        ws_xml_destroy_doc(respDoc);
    }

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
int jhkwsman_get_command_output(jhkwsman_t * obj, jobresult_t * obj_jobres,
                                const int codepage)
{
    jhklog_trace("In %s()", __func__);

    while (obj->command_done == 0) {
        if (jhkwsman_raw_get_command_output(obj, obj_jobres, codepage) !=
            0)
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
int jhkwsman_cleanup_command(jhkwsman_t * obj)
{
    int rc;
    WsXmlDocH rqstDoc;
    WsXmlDocH respDoc;
    WsSerializerContextH serctx;
    WsXmlNodeH header;
    WsXmlNodeH body;
    WsXmlNodeH node;
    char uuidBuf[100];

    jhklog_trace("In %s()", __func__);
    // check paramter
    if (obj == NULL)
        return -1;
    if (obj->shell_id == NULL || obj->command_id == NULL)
        return -1;

    // init
    rc = -1;
    rqstDoc = ws_xml_create_envelope();
    respDoc = NULL;
    serctx = wsmc_get_serialization_context(obj->client);

    // create header data
    generate_uuid(uuidBuf, sizeof(uuidBuf), 0);
    jhkwsman_soap_header(obj, rqstDoc, ACTION_SIGNAL, RESOURCE_URI_CMD,
                         obj->shell_id, uuidBuf);
    body = ws_xml_get_soap_body(rqstDoc);

    // add body data
    node = ws_xml_add_child(body, XML_NS_RSP, MS_SIGNAL, NULL);
    ws_xml_add_node_attr(node, NULL, "CommandId", obj->command_id);
    ws_xml_add_child(node, XML_NS_RSP, MS_CODE, RSP_CODE);

    // send message
    respDoc = jhkwsman_send(obj, rqstDoc);
    if (respDoc == NULL) {
        jhklog_warn("In %s() can not get response", __func__);
        goto error;
    }
    // get reponse data
    header = ws_xml_get_soap_header(respDoc);

    // get uuid
    node = ws_xml_find_in_tree(header, XML_NS_ADDRESSING, "RelatesTo", 1);
    if (node == NULL) {
        goto error;
    }
    // compare unid and message_id
    if (strcmp(ws_xml_get_node_text(node), uuidBuf) != 0) {
        goto error;
    }

    rc = 0;
  error:
    if (rqstDoc) {
        ws_xml_destroy_doc(rqstDoc);
    }
    if (respDoc) {
        ws_xml_destroy_doc(respDoc);
    }

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
int jhkwsman_close_shell(jhkwsman_t * obj)
{
    int rc;
    WsXmlDocH rqstDoc;
    WsXmlDocH respDoc;
    WsSerializerContextH serctx;
    WsXmlNodeH header;
    WsXmlNodeH body;
    WsXmlNodeH node;
    char uuidBuf[100];

    jhklog_trace("In %s()", __func__);
    // check paramter
    if (obj == NULL)
        return -1;
    if (obj->shell_id == NULL)
        return -1;

    // init
    rc = -1;
    rqstDoc = ws_xml_create_envelope();
    respDoc = NULL;
    serctx = wsmc_get_serialization_context(obj->client);

    // create header data
    generate_uuid(uuidBuf, sizeof(uuidBuf), 0);
    jhkwsman_soap_header(obj, rqstDoc, ACTION_DELETE, RESOURCE_URI_CMD,
                         obj->shell_id, uuidBuf);
    body = ws_xml_get_soap_body(rqstDoc);

    // send message
    respDoc = jhkwsman_send(obj, rqstDoc);
    if (respDoc == NULL) {
        jhklog_warn("In %s() can not get response", __func__);
        goto error;
    }
    // get reponse data
    header = ws_xml_get_soap_header(respDoc);

    // get uuid
    node = ws_xml_find_in_tree(header, XML_NS_ADDRESSING, "RelatesTo", 1);
    if (node == NULL) {
        goto error;
    }
    if (strcmp(ws_xml_get_node_text(node), uuidBuf) != 0) {
        goto error;
    }

    rc = 0;
  error:
    if (rqstDoc) {
        ws_xml_destroy_doc(rqstDoc);
    }
    if (respDoc) {
        ws_xml_destroy_doc(respDoc);
    }

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
int jhkwsman_exec(jhkwsman_t * obj, jobresult_t * obj_jobres,
                  const char *command, const int codepage)
{
    int rc;
    char buf[MAX_STRING_LEN];

    // check paramter
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || obj_jobres == NULL || command == NULL)
        return -1;


    rc = -1;
    jhklog_debug("In %s() command: %s", __func__, command);
    if (obj->client == NULL) {
        jhklog_error("In %s() wsman is NULL", __func__);
        return -1;
    }
    // init jobresult
    jobresult_init(obj_jobres);

    // open shell
    if (jhkwsman_open_shell
        (obj, "stdin", "stdout stderr", NULL, NULL, "FALSE", codepage,
         NULL) != 0) {
        jhklog_warn("In %s() can not open shell", __func__);
        return -1;
    }
    // run command
    if (jhkwsman_run_command(obj, command, "TRUE", "FALSE") != 0)
        return -1;

    // get command output
    if (jhkwsman_get_command_output(obj, obj_jobres, codepage) != 0)
        return -1;

    // cleanup command
    if (jhkwsman_cleanup_command(obj) != 0) {
        jhklog_warn("In %s() can not cleanup command", __func__);
    } else {
        if (jhkwsman_close_shell(obj) != 0) {
            jhklog_warn("In %s() can not close shell", __func__);
        }
    }

    // reset exitcode and exitsignal again
    jhklog_debug("In %s() exitcode: %d, exitsignal: %s", __func__,
                 obj_jobres->exitcode, obj_jobres->exitsignal);

    rc = 0;
  error:
    return rc;
}
