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

#ifndef JHAKO_JHKSSH_H
#define JHAKO_JHKSSH_H

#include <libssh2.h>
#include "common.h"
#include "jobresult.h"

typedef struct {
    apr_pool_t *pool;
    apr_os_sock_t socket;
    LIBSSH2_SESSION *session;
    LIBSSH2_CHANNEL *channel;
} jhkssh_t;

jhkssh_t *jhkssh_new(void);
void jhkssh_destroy(jhkssh_t * obj);
int jhkssh_init(jhkssh_t * obj);

int jhkssh_session(jhkssh_t * obj, const char *host, const int port);
int jhkssh_userauth_password(jhkssh_t * obj, const char *username,
                             const char *password);
int jhkssh_userauth_publickey(jhkssh_t * obj, const char *username,
                              const char *privatekey,
                              const char *passphrase);
int jhkssh_channel(jhkssh_t * obj);
int jhkssh_exec(jhkssh_t * obj, jobresult_t * obj_jobres,
                const char *command);

#endif
