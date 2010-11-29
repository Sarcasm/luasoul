/*
 * netsoul_connect.c for luasoul
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

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "netsoul.h"
#include "netsoul.priv.h"

/**
 * Convert human readable network address into structured binary
 * formats for the operating system's networking API. (thx wikipedia).
 *
 * @param       server
 *                      the server address (probably
 *                      'ns-server.epita.fr')
 * @param       port
 *                      the server port (probably '4242')
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
  hints.ai_flags = AI_CANONNAME;
  hints.ai_family = AF_UNSPEC;  /* allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP; /* 0 for any protocol */
  status = getaddrinfo(server, port, &hints, &result);
  if (status != 0)
    {
      *err_msg = gai_strerror(status);
      return NULL;
    }

  return result;
}

/**
 * Create a new socket for the given server and port.
 *
 * @param       server
 *                      the server address (probably
 *                      'ns-server.epita.fr')
 * @param       port
 *                      the server port (probably '4242')
 * @param       err_msg
 *                      on error this param is filled with an
 *                      explanation
 *
 * @return the socket file descriptor, on error -1 is returned.
 */
static int      netsoul_connect_socket(const char        *server,
                                       const char        *port,
                                       const char       **err_msg)
{
  int                    sockfd;
  struct addrinfo       *result, *iter;

  /* resolve the real adress of the server */
  result = netsoul_resolve_hostname(server, port, err_msg);
  if (result == NULL)
    return -1;

  /* real server name is `result->ai_canonname' */
  for (iter = result; iter != NULL; iter = iter->ai_next)
    {
      sockfd = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol);

      /* bad adress try the next */
      if (sockfd == -1)
        continue;

      if (connect(sockfd, iter->ai_addr, sizeof(*iter->ai_addr)) == 0)
        break ;
      netsoul_close(sockfd);
    }
  freeaddrinfo(result);         /* no longer needed */

  if (iter == NULL || sockfd == -1)
    {
      *err_msg = "couldn't connect to the given server/port";
      return -1;
    }

  return sockfd;
}


/**
 * Server authentication.
 *
 * @param       sockfd
 *                      the netsoul socket file descriptor
 * @param       login
 *                      user login
 * @param       socks_pass
 *                      socks password
 * @param       userdata
 *                      generally name + version of the client
 *                      (maximum 64 characters)
 * @param       location
 *                      location of the user (maximum 64 characters)
 * @param       err_msg
 *                      on error this param is filled with an
 *                      explanation
 *
 * @return      0 on success, (!= 0 on error)
 */
static int      netsoul_md5_auth(int          sockfd,
                                 const char  *login,
                                 const char  *socks_pass,
                                 const char  *userdata,
                                 const char  *location,
                                 const char **err_msg)
{
  /*
    format of the first message:
    salut <socket number> <random md5 hash> <client ip><client port> \
    <server timestamp>
  */
  char        *tmp = netsoul_recv(sockfd);
  char        msg[SOCK_MSG_SIZE];
  char        *md5server;
  char        *cip;
  char        *cport;

  if (tmp == NULL)
    {
      *err_msg = "netsoul_recv() error (return NULL)";
      return -1;
    }

  /* save the first message, because the next call to netsoul_recv()
     will erase its content */
  strcpy(msg, tmp);

  /* pre-authentication message */
  netsoul_send(sockfd, "%s\n", "auth_ag ext_user none -");

  /* check server answer */
  if (netsoul_get_msg_type(netsoul_recv(sockfd)) != REP_OK)
    {
      *err_msg = "server send bad response";
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
      *err_msg = "unknow message from netsoul server";
      return -1;
    }

  netsoul_send(sockfd, "ext_user_log %s %s %.64s %.64s\n",
               login,
               netsoul_md5sum("%s-%s/%s%s", md5server, cip, cport, socks_pass),
               userdata,
               location);

  if (netsoul_get_msg_type(netsoul_recv(sockfd)) != REP_OK)
    {
      *err_msg = "server send bad response to ext_user_log";
      return -1;
    }

  return 0;
}

/**
 * Create a new connection to the netsoul server.
 *
 * @param       server
 *                      the server address (probably
 *                      'ns-server.epita.fr')
 * @param       port
 *                      the server port (probably '4242')
 * @param       login
 *                      user login
 * @param       socks_pass
 *                      socks password
 * @param       userdata
 *                      generally name + version of the client
 *                      (maximum 64 characters)
 * @param       location
 *                      location of the user (maximum 64 characters)
 * @param       err_msg
 *                      on error this param is filled with an
 *                      explanation
 *
 * @return the socket file descriptor (-1 on error and fill err_msg).
 */
int             netsoul_connect(const char       *server,
                                const char       *port,
                                const char       *login,
                                const char       *socks_pass,
                                const char       *userdata,
                                const char       *location,
                                const char      **err_msg)
{
  int            sockfd;

  /* connect to server */
  sockfd = netsoul_connect_socket(server, port, err_msg);

  /* authentification */
  if (sockfd != -1)
    {
      if (netsoul_md5_auth(sockfd, login, socks_pass, userdata,
                           location, err_msg) == -1)
        {
          netsoul_close(sockfd);
          return -1;
        }
      netsoul_set_status(sockfd, "actif");
    }
  return sockfd;
}
