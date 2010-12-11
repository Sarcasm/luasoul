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

#define LOGIN_CHARACTERS        "abcdefghijklmnopqrstuvwxyz0123456789_"
#define STATUS_CHARACTERS       LOGIN_CHARACTERS

typedef struct netsoulSession  netsoulSession;

typedef struct
{
  int           (*new_msg)(void *data, const char *login, const char *msg);
  int           (*typing_start)(void *data, const char *login);
  int           (*typing_end)(void *data, const char *login);
  int           (*login)(void *data, const char *login);
  int           (*logout)(void *data, const char *login);
  int           (*status_changed)(void *data, const char *login,
                                  const char *status);
  int           (*unknow_event)(void *data, const char *msg);
}                netsoulCallbacks;

typedef struct
{
  const char       *login;      /* default $USER                           */
  const char       *socks_pass; /* socks password (for md5 authentication) */
  const char       *userdata;   /* default 'LuaSoul v0.42'                 */
  const char       *location;   /* default '-'                             */
  const char       *server;     /* default 'ns-server.epita.fr'            */
  const char       *port;       /* default 4242                            */
  netsoulCallbacks  callbacks;
}               netsoulConfig;

netsoulSession *netsoul_create_session(netsoulConfig  *settings,
                                       const char    **err_msg);
void            netsoul_destroy_session(netsoulSession *N);

int             netsoul_connect(netsoulSession *N, const char **err_msg);
void            netsoul_deconnect(netsoulSession *N);

int             netsoul_get_fd(netsoulSession *N);

int             netsoul_set_blocking(netsoulSession *N);
int             netsoul_set_nonblocking(netsoulSession *N);

int             netsoul_event_handler(netsoulSession *N, void *data);

int             netsoul_send_msg(netsoulSession *N, const char *login,
                                 const char *msg);
int             netsoul_send_typing_start(netsoulSession *N,
                                          const char *login);
int             netsoul_send_typing_end(netsoulSession *N,
                                        const char *login);
int             netsoul_set_status(netsoulSession *N, const char *status);
int             netsoul_spy_user(netsoulSession *N, const char *login);
int             netsoul_spy_users(netsoulSession *N, const char *logins[]);

#endif /* _LIB_NETSOUL_H_ */
