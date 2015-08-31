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

#include <sys/time.h>
#include <libssh2.h>

#include "common.h"
#include "jhkconf.h"
#include "jhklog.h"
#include "models.h"
#include "jhkssh.h"

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
static int jhkssh_waitsocket(jhkssh_t * obj)
{
    int rc;
    struct timeval tv;
    fd_set fd;
    fd_set *writefd = NULL;
    fd_set *readfd = NULL;
    int dir;

    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    tv.tv_sec = 5;
    tv.tv_usec = 0;
    FD_ZERO(&fd);
    FD_SET(obj->socket, &fd);

    dir = libssh2_session_block_directions(obj->session);
    if (dir & LIBSSH2_SESSION_BLOCK_INBOUND)
        readfd = &fd;
    if (dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
        writefd = &fd;

    rc = select(obj->socket + 1, readfd, writefd, NULL, &tv);

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
jhkssh_t *jhkssh_new(void)
{
    jhkssh_t *obj = NULL;

    jhklog_trace("In %s()", __func__);

    if (libssh2_init(0) != 0)
        return NULL;

    obj = (jhkssh_t *) malloc(sizeof(jhkssh_t));

    apr_pool_create(&(obj->pool), jx_pool);
    obj->socket = -1;
    obj->session = NULL;
    obj->channel = NULL;

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
void jhkssh_destroy(jhkssh_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return;

    jhkssh_init(obj);
    apr_pool_destroy(obj->pool);

    jhk_free(obj);
    obj = NULL;

    libssh2_exit();
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
int jhkssh_init(jhkssh_t * obj)
{
    jhklog_trace("In %s()", __func__);
    if (obj == NULL)
        return -1;

    if (obj->channel != NULL) {
        libssh2_channel_free(obj->channel);
    }
    if (obj->session != NULL) {
        libssh2_session_disconnect(obj->session, "Normal Shutdown");
        libssh2_session_free(obj->session);
    }

    apr_pool_clear(obj->pool);
    obj->socket = -1;
    obj->channel = NULL;
    obj->session = NULL;

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
int jhkssh_session(jhkssh_t * obj, const char *host, const int port)
{
    apr_status_t rv;
    apr_sockaddr_t *sa;
    apr_socket_t *sock;
    char errmsg[100];
    int rc_ssh;

    if (obj == NULL || host == NULL)
        return -1;
    jhklog_debug("In %s() host: %s, port: %d", __func__, host, port);
    jhkssh_init(obj);

    // create sockaddr
    rv = apr_sockaddr_info_get(&sa, host, APR_INET, port, 0, obj->pool);
    if (rv != APR_SUCCESS) {
        apr_strerror(rv, errmsg, sizeof(errmsg));
        jhklog_warn
            ("In %s() apr_sockaddr_info_get error. host: %s, port: %d, message: %s",
             __func__, host, port, errmsg);
        return -1;
    }
    // create socket
    rv = apr_socket_create(&sock, sa->family, SOCK_STREAM, 0, obj->pool);
    if (rv != APR_SUCCESS) {
        apr_strerror(rv, errmsg, sizeof(errmsg));
        jhklog_warn
            ("In %s() apr_socket_create error. host: %s, port: %d, message: %s",
             __func__, host, port, errmsg);
        return -1;
    }
    // connect socket
    rv = apr_socket_connect(sock, sa);
    if (rv != APR_SUCCESS) {
        apr_strerror(rv, errmsg, sizeof(errmsg));
        jhklog_warn
            ("In %s() apr_socket_connect error. host: %s, port: %d, message: %s",
             __func__, host, port, errmsg);
        return -1;
    }
    // get socket number
    apr_os_sock_get(&(obj->socket), sock);
    jhklog_debug("In %s() host: %s, port: %d, sock: %d", __func__, host,
                 port, obj->socket);

    // create ssh session
    obj->session = libssh2_session_init();
    if (obj->session == NULL)
        return -1;
    libssh2_session_set_blocking(obj->session, 0);
    while (1) {
        rc_ssh = libssh2_session_handshake(obj->session, obj->socket);
        if (rc_ssh == 0) {
            break;
        } else if (rc_ssh == LIBSSH2_ERROR_EAGAIN) {
            continue;
        } else if (rc_ssh == LIBSSH2_ERROR_SOCKET_DISCONNECT) {
            jhklog_warn
                ("In %s() libssh2_session_handshake disconnect. rc_ssh: %d",
                 __func__, rc_ssh);
            return -1;
        } else {
            jhklog_warn
                ("In %s() libssh2_session_handshake failed. rc_ssh: %d",
                 __func__, rc_ssh);
            return -1;
        }
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
int jhkssh_userauth_password(jhkssh_t * obj, const char *username,
                             const char *password)
{
    int rc_ssh;

    if (obj == NULL || username == NULL || password == NULL)
        return -1;
    jhklog_debug("In %s() username: %s", __func__, username);
    if (obj->session == NULL) {
        jhklog_error("In %s() ssh session is not established", __func__);
        return -1;
    }
    // ssh userauth
    while (1) {
        rc_ssh =
            libssh2_userauth_password(obj->session, username, password);
        if (rc_ssh == 0) {
            break;
        } else if (rc_ssh == LIBSSH2_ERROR_EAGAIN) {
            continue;
        } else {
            jhklog_warn
                ("In %s() libssh2_userauth_password failed. rc_ssh: %d",
                 __func__, rc_ssh);
            return -1;
        }
    }

    return jhkssh_channel(obj);
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
int jhkssh_userauth_publickey(jhkssh_t * obj, const char *username,
                              const char *privatekey,
                              const char *passphrase)
{
    jhkconf_t *cfg;
    char errmsg[100];
    apr_status_t rv;
    apr_file_t *fp = NULL;
    const char *tmpdir = NULL;
    char *privatefile;
    int rc_ssh;

    // check parameters
    jhklog_trace("In %s()", __func__);
    if (obj == NULL || username == NULL || privatekey == NULL
        || passphrase == NULL)
        return -1;

    jhklog_debug("In %s() username: %s", __func__, username);
    if (obj->session == NULL) {
        jhklog_error("In %s() ssh session is not established", __func__);
        return -1;
    }
    // get conf parameter
    cfg = jhkconf_get();
    if (cfg == NULL) {
        jhklog_error("In %s() conf is NULL", __func__);
        return -1;
    }
    // get tmp dir
    rv = APR_SUCCESS;
    if (strlen(cfg->tmpdir) == 0) {
        rv = apr_temp_dir_get(&tmpdir, obj->pool);
    } else {
        tmpdir = cfg->tmpdir;
    }
    if (rv != APR_SUCCESS) {
        apr_strerror(rv, errmsg, sizeof(errmsg));
        jhklog_error("In %s() can not get temp dir. %s", __func__, errmsg);
        return -1;
    }
    // get tmp file
    privatefile =
        apr_pstrcat(obj->pool, tmpdir, "/privatekey.XXXXXX", NULL);
    rv = apr_file_mktemp(&fp, privatefile, 0, obj->pool);
    if (rv != APR_SUCCESS) {
        apr_strerror(rv, errmsg, sizeof(errmsg));
        jhklog_error("In %s() can not mktemp file [%s]. %s", __func__,
                     privatefile, errmsg);
        return -1;
    }
    // save private key
    rv = apr_file_puts(privatekey, fp);
    if (rv != APR_SUCCESS) {
        apr_strerror(rv, errmsg, sizeof(errmsg));
        jhklog_error
            ("In %s() can not save private key to the file [%s]. %s",
             __func__, privatefile, errmsg);
        return -1;
    }
    jhklog_debug("In %s() create private key file. %s", __func__,
                 privatefile);

    // ssh userauth publickey
    while (1) {
        rc_ssh =
            libssh2_userauth_publickey_fromfile(obj->session, username,
                                                NULL, privatefile,
                                                passphrase);
        if (rc_ssh == 0) {
            break;
        } else if (rc_ssh == LIBSSH2_ERROR_EAGAIN) {
            continue;
        } else {
            jhklog_warn
                ("In %s() libssh2_userauth_publickey_fromfile failed. rc_ssh: %d",
                 __func__, rc_ssh);
            return -1;
        }
    }

    return jhkssh_channel(obj);
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
int jhkssh_channel(jhkssh_t * obj)
{
    int rc_ssh;
    char *errmsg = NULL;

    jhklog_trace("In %s()", __func__);

    if (obj == NULL || obj->session == NULL)
        return -1;

    if (obj->channel != NULL)
        return 0;

    // open ssh channel
    while (1) {
        obj->channel = libssh2_channel_open_session(obj->session);
        if (obj->channel != NULL)
            break;
        rc_ssh =
            libssh2_session_last_error(obj->session, &errmsg, NULL, 0);
        if (rc_ssh == LIBSSH2_ERROR_EAGAIN) {
            jhkssh_waitsocket(obj);
            continue;
        } else {
            jhklog_warn
                ("In %s() libssh2_channel_open_session failed. errmsg: %s, rc_ssh: %d",
                 __func__, errmsg, rc_ssh);
            return -1;
        }
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
int jhkssh_exec(jhkssh_t * obj, jobresult_t * obj_jobres,
                const char *command)
{
    int rc_ssh;
    apr_size_t n;
    char buf[JOB_RESULT_SIZE];
    char *str_signal;

    // check paramter
    if (obj == NULL || obj_jobres == NULL || command == NULL)
        return -1;

    jhklog_debug("In %s() command: %s", __func__, command);
    if (obj->channel == NULL) {
        jhklog_error("In %s() ssh channel is not established", __func__);
        return -1;
    }

    str_signal = NULL;

    // execute command
    while (1) {
        rc_ssh = libssh2_channel_exec(obj->channel, command);
        if (rc_ssh == 0) {
            break;
        } else if (rc_ssh == LIBSSH2_ERROR_EAGAIN) {
            jhkssh_waitsocket(obj);
            continue;
        } else {
            jhklog_warn
                ("In %s() libssh2_channel_exec failed. commmand: %s, rc_ssh: %d",
                 __func__, command, rc_ssh);
            return -1;
        }
    }

    // read stdout
    while (1) {
        do {
            rc_ssh =
                libssh2_channel_read(obj->channel, buf,
                                     JOB_RESULT_SIZE - 1);
            if (rc_ssh > 0) {
                buf[rc_ssh] = '\0';
                jhk_strncat(obj_jobres->stdout, buf, JOB_RESULT_SIZE);
            }
        } while (rc_ssh > 0);

        if (rc_ssh >= 0) {
            break;
        } else if (rc_ssh == LIBSSH2_ERROR_EAGAIN) {
            jhkssh_waitsocket(obj);
            continue;
        } else {
            jhklog_warn("In %s() libssh2_channel_read failed. rc_ssh: %d",
                        __func__, rc_ssh);
            return -1;
        }
    }
    jhklog_debug("In %s() stdout: \n%s", __func__, obj_jobres->stdout);

    // read stderr
    while (1) {
        do {
            rc_ssh =
                libssh2_channel_read_stderr(obj->channel, buf,
                                            JOB_RESULT_SIZE - 1);
            if (rc_ssh > 0) {
                buf[rc_ssh] = '\0';
                jhk_strncat(obj_jobres->stderr, buf, JOB_RESULT_SIZE);
            }
        } while (rc_ssh > 0);

        if (rc_ssh >= 0) {
            break;
        } else if (rc_ssh == LIBSSH2_ERROR_EAGAIN) {
            jhkssh_waitsocket(obj);
            continue;
        } else {
            jhklog_warn
                ("In %s() libssh2_channel_read_stderr failed. rc_ssh: %d",
                 __func__, rc_ssh);
            return -1;
        }
    }
    jhklog_debug("In %s() stderr: \n%s", __func__, obj_jobres->stderr);

    // get exitcode or exitsignal
    while (1) {
        rc_ssh = libssh2_channel_close(obj->channel);
        if (rc_ssh == 0) {
            obj_jobres->exitcode =
                libssh2_channel_get_exit_status(obj->channel);
            libssh2_channel_get_exit_signal(obj->channel,
                                            &(str_signal),
                                            NULL, NULL, NULL, NULL, NULL);
            break;
        } else if (rc_ssh == LIBSSH2_ERROR_EAGAIN) {
            jhkssh_waitsocket(obj);
            continue;
        } else {
            jhklog_warn
                ("In %s() libssh2_channel_close failed. rc_ssh: %d",
                 __func__, rc_ssh);
            return -1;
        }
    }

    // reset exitcode and exitsignal again
    if (str_signal != NULL) {
        jhk_strncat(obj_jobres->exitsignal, str_signal, EXIT_SIGNAL_SIZE);
        obj_jobres->exitcode = -1;
    }

    jhklog_debug("In %s() exitcode: %d, exitsignal: %s", __func__,
                 obj_jobres->exitcode, obj_jobres->exitsignal);

    jhk_free(str_signal);
    return 0;
}
