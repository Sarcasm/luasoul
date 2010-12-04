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

typedef struct netsoulSession  netsoulSession;

typedef struct
{
  char         *login;      /* default $USER                           */
  char         *socks_pass; /* socks password (for md5 authentication) */
  char         *userdata;   /* default 'LuaSoul v0.42'                 */
  char         *location;   /* default '-'                             */
  char         *server;     /* default 'ns-server.epita.fr'            */
  char         *port;       /* default 4242                            */
}               netsoulConfig;

typedef struct
{
  int           (*callbackName1)(void *, char, char);
  int           (*callbackName2)(void *, char, char, char);
}                netsoulCallbacks;

netsoulSession *netsoul_create_session(netsoulConfig  *settings,
                                       const char    **err_msg);
void            netsoul_destroy_session(netsoulSession *N);

int             netsoul_connect(netsoulSession *N, const char **err_msg);
void            netsoul_deconnect(netsoulSession *N);

int             netsoul_set_blocking(netsoulSession *N);
int             netsoul_set_nonblocking(netsoulSession *N);

int             netsoul_set_status(netsoulSession *N, const char *status);

#endif /* _LIB_NETSOUL_H_ */
