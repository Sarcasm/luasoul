/*
 * connect.c for luasoul
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
 * Create a new socket and connect to the Netsoul server.
 *
 * @param       N
 *                      the Netsoul session
 * @param       err_msg
 *                      on error this param is filled with an
 *                      explanation
 *
 * @return the socket file descriptor, on error -1 is returned.
 */
static int              netsoul_connect_socket(netsoulSession *N, const char **err_msg)
{
  int                   sockfd;
  struct addrinfo       *iter;

  /* real server name is `result->ai_canonname' */
  for (iter = N->addr; iter != NULL; iter = iter->ai_next)
    {
      sockfd = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);

      /* bad adress try the next */
      if (sockfd == -1)
        continue;

      if (connect(sockfd, iter->ai_addr, sizeof(*iter->ai_addr)) == 0)
        return sockfd;          /* SUCCESS */

      netsoul_deconnect(N);
    }

  *err_msg = "couldn't connect";
  return -1;
}


/**
 * MD5 authentication with socks password.
 *
 * @param       N
 *                      the Netsoul session
 * @param       err_msg
 *                      on error this param is filled with an
 *                      explanation
 *
 * @return      0 on success, (!= 0 on error)
 */
static int      netsoul_md5_auth(netsoulSession *N, const char **err_msg)
{
  /*
    format of the first message:
    salut <socket number> <random md5 hash> <client ip><client port> \
    <server timestamp>
  */
  char        *tmp = netsoul_recv(N);
  char        msg[NETSOUL_MSG_SIZE];
  char        *md5server;
  char        *cip;
  char        *cport;

  if (tmp == NULL)
    {
      *err_msg = "netsoul_recv() error, NULL was returned";
      return -1;
    }

  /* save the first message, because the next call to netsoul_recv()
     will erase its content */
  strcpy(msg, tmp);

  /* pre-authentication message */
  netsoul_send(N, "%s\n", "auth_ag ext_user none -");

  /* check server answer */
  if (! MessageIsOK(netsoul_recv(N)))
    {
      *err_msg = "bad response from server";
      return -1;
    }

  /*
    final auth format:
    ext_user_log <login user> <md5 answer> <user data> <user location>
    maximum lenght for 'userdata' and 'location' is 64
    where md5 answer is:
    MD5("<md5 server>-<client ip>/<client port><pass socks>")
  */
  strtok(msg, " ");              /* salut */
  strtok(NULL, " ");             /* the socket number */
  md5server = strtok(NULL, " "); /* hash md5 */
  cip       = strtok(NULL, " "); /* host client */
  cport     = strtok(NULL, " "); /* port client */
  /* ... timestamp */

  if (md5server == NULL && cip == NULL && cport == NULL)
    {
      *err_msg = "unknow message from server";
      return -1;
    }

  {
    char        url_enc_userdata[NETSOUL_DATA_SIZE];
    char        url_enc_location[NETSOUL_DATA_SIZE];

    /* encode strings */
    netsoul_url_encode(url_enc_userdata, N->userdata, NETSOUL_DATA_SIZE -1);
    netsoul_url_encode(url_enc_location, N->location, NETSOUL_DATA_SIZE -1);

    netsoul_send(N, "ext_user_log %s %s %.64s %.64s\n",
                 N->login,
                 netsoul_md5sum("%s-%s/%s%s", md5server, cip, cport,
                                N->socks_pass),
                 url_enc_userdata,
                 url_enc_location);
  }

  if (! MessageIsOK(netsoul_recv(N)))
    {
      *err_msg = "bad response from server (in reply to 'ext_user_log')";
      return -1;
    }

  return 0;
}

/**
 * Create a new connection to the netsoul server.
 *
 * @param       N
 *                      the Netsoul session
 * @param       err_msg
 *                      on error this param is filled with an
 *                      explanation
 *
 * @return on success 0, on error -1 and err_msg is filled.
 */
int             netsoul_connect(netsoulSession *N, const char **err_msg)
{
  /* connect to server */
  N->sockfd = netsoul_connect_socket(N, err_msg);

  if (N->sockfd != -1)
    {
      /* authentification */
      if (netsoul_md5_auth(N, err_msg) != -1)
        {
          /* enable access to school resources */
          netsoul_send(N, "%s\n", "user_cmd attach");

          /* default state is 'connection' */
          netsoul_set_status(N, "actif");

          return 0;
        }
      else
        netsoul_deconnect(N);
    }
  return -1;
}
