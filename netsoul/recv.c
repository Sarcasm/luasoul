/*
 * recv.c for libnetsoul
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

#include <stddef.h>
#include <unistd.h>

#include "netsoul.priv.h"

/**
 * Receive a message from the netsoul server or wait for incoming
 * data.
 *
 * @param       N
 *                      the Netsoul session.
 *
 * @return the received message or NULL on error.
 * @note the return value can be modified but the function free()
 * shouldn't be called because it's a static buffer.
 */
char            *netsoul_recv(netsoulSession *N)
{
  static char   msg[NETSOUL_MSG_SIZE] = {0};
  const ssize_t nb_read = recv(N->sockfd, msg, NETSOUL_MSG_SIZE -1, 0);

  if (nb_read == -1)
    return NULL;

  /* server call shutdown, reconnect */
  if (nb_read == 0)
    {
      const char *err_msg;

      close(N->sockfd);
      netsoul_connect(N, &err_msg);
    }

  msg[nb_read] = '\0';
  return msg;
}
