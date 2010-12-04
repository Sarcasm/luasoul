/*
 * create_session.c for luasoul
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

#include <stdlib.h>
#include <string.h>

#include "netsoul.priv.h"

/**
 * Convert human readable network address into structured binary
 * formats for the operating system's networking API. (thx wikipedia).
 *
 * @param       server
 *                      the server address (if NULL then try
 *                      'ns-server.epita.fr')
 * @param       port
 *                      the server port (if NULL then try '4242')
 * @param       err_msg
 *                      on error this param is filled with an
 *                      explanation
 *
 * @return a list of addrinfo structures or NULL on error.
 */
static struct addrinfo  *netsoul_resolve_hostname(const char     *server,
                                                  const char     *port,
                                                  const char    **err_msg)
{
  int                   status;
  struct addrinfo       hints;
  struct addrinfo       *result;

  memset(&hints, 0, sizeof(hints));

  hints.ai_flags    = AI_CANONNAME;
  hints.ai_family   = AF_UNSPEC; /* allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP; /* 0 for any protocol */
  status            = getaddrinfo((server != NULL) ? server : "ns-server.epita.fr",
                                  (port != NULL)   ? port   : "4242",
                                  &hints, &result);
  if (status != 0)
    {
      *err_msg = gai_strerror(status);
      return NULL;
    }

  return result;
}

/**
 * Create a new Netsoul session.
 *
 * @param       err_msg
 *                      on error this param is filled with an
 *                      explanation
 * @return on success a new 'netsoulSession' is returned,
 * on error NULL is returned.
 */
netsoulSession          *netsoul_create_session(netsoulConfig  *settings,
                                                const char    **err_msg)
{
  netsoulSession        *N = malloc(sizeof(*N));
  char                  *tmp;

  if (N == NULL)
    {
      *err_msg = "malloc() fail: out of memory.";
      return NULL;
    }

  /* get login if the login is not set then try $USER environnement
     variable */
  tmp = settings->login != NULL ? settings->login : getenv("USER");
  if (tmp == NULL)
    {
      *err_msg = "login or $USER environnement variable required.";
      free(N);
      return NULL;
    }
  strncpy(N->login, tmp, NETSOUL_LOGIN_SIZE -1);

  /* get socks password for md5 authentication */
  if (settings->socks_pass == NULL)
    {
      *N->socks_pass = '\0';
      /* *err_msg = "socks password required."; */
      /* free(N); */
      /* return NULL;       */
    }
  else
    strncpy(N->socks_pass, settings->socks_pass, NETSOUL_SOCKS_SIZE -1);

  /* get userdata */
  strncpy(N->userdata, (settings->userdata != NULL)
                        ? settings->userdata
                        : "LuaSoul v0.42", NETSOUL_DATA_SIZE);

  /* get location */
  strncpy(N->location, (settings->location != NULL)
                        ? settings->location
                        : "-", NETSOUL_DATA_SIZE -1);

  /* get host settings */
  N->addr = netsoul_resolve_hostname(settings->server,
                                     settings->port,
                                     err_msg);
  if (N->addr == NULL)
    {
      *err_msg = "can't resolve hostname";
      free(N);
      return NULL;
    }

  return N;
}
