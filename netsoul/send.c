/*
 * send.c for libnetsoul
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

#undef  _XOPEN_SOURCE      /* vsnprintf() need _XOPEN_SOURCE >= 500 */
#define _XOPEN_SOURCE 500  /* for vsnprintf() */
#include <stdio.h>
#include <stdarg.h>

#include <stddef.h>
#include <sys/socket.h>

#include "netsoul.h"
#include "netsoul.priv.h"

/**
 * Send a formated message to the netsoul server.
 *
 * @param       fmt, ...
 *                      formatted string like printf()
 *
 * @return 0 on success (!= 0 on error).
 */
int             netsoul_send(int sockfd, const char *fmt, ...)
{
  static char   msg[SOCK_MSG_SIZE];
  int           nb_write;
  va_list       argp;

  va_start(argp, fmt);
  nb_write = vsnprintf(msg, SOCK_MSG_SIZE -1, fmt, argp);
  va_end(argp);
  if (nb_write >= SOCK_MSG_SIZE || nb_write <= 0)
    return -1;
  printf("{%s}\n", msg);
  return send(sockfd, msg, nb_write, 0) != -1; /* 0 on success */
}
