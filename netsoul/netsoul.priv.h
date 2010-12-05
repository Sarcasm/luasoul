/*
 * netsoul.priv.h for libnetsoul
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

#ifndef _LIBNETSOUL_PRIV_H_
#define _LIBNETSOUL_PRIV_H_

#include "netsoul.h"

/* Need to know 'struct addrinfo' */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define UNUSED(x)       (void) x


#define NETSOUL_HOST_SIZE  64   /* max lenght for hostname              */
#define NETSOUL_PORT_SIZE  32   /* max lenght for port                  */
#define NETSOUL_DATA_SIZE  64   /* max lenght for location and userdata */
#define NETSOUL_LOGIN_SIZE 20   /* max lenght for login                 */
#define NETSOUL_SOCKS_SIZE 20   /* max lenght for socks passwd          */
#define NETSOUL_MSG_SIZE  4096  /* max lenght for in/out socket message */

/* utils */
char            *netsoul_url_decode(char *s);
char            *netsoul_url_encode(char *dest, const char *src, size_t len);
char            *netsoul_md5sum(const char *fmt, ...);

int             netsoul_send(netsoulSession *N, const char *fmt, ...);
char            *netsoul_recv(netsoulSession *N);


struct             netsoulSession
{
  char             login[NETSOUL_LOGIN_SIZE];
  char             socks_pass[NETSOUL_SOCKS_SIZE];
  char             userdata[NETSOUL_DATA_SIZE]; /* default 'LuaSoul v0.42' */
  char             location[NETSOUL_DATA_SIZE];
  struct addrinfo *addr;
  int              sockfd;
  netsoulCallbacks callbacks;
};

#define MessageIsOK(msg) (strcmp(msg, "rep 002 -- cmd end\n") == 0)

#endif /* _LIBNETSOUL_PRIV_H_ */
