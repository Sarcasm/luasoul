/*
 * netsoul.h for luasoul
 * 
 * Copyright © 2010 Guillaume Papin <guillaume.papin@epitech.eu>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LIB_NETSOUL_H_
#define _LIB_NETSOUL_H_

/**
 * List of types for the netsoul server message.
 */
enum            NETSOUL_MSG_TYPE
  {
    REP_OK,
    UNKNOW_TYPE
  };

int             netsoul_connect(const char       *server,
                                const char       *port,
                                const char       *login,
                                const char       *pass_socks,
                                const char       *userdata,
                                const char       *location,
                                const char      **err_msg);
void            netsoul_close(int sockfd);

/* common status :
   actif, away, connection, idle, lock, server, none */
int             netsoul_set_status(int sockfd, const char *status);

enum NETSOUL_MSG_TYPE   netsoul_get_msg_type(const char *msg);

#endif /* _LIB_NETSOUL_H_ */
