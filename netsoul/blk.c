/*
 * blk.c for luasoul
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

#include <unistd.h>
#include <fcntl.h>

#include "netsoul.priv.h"

/**
 * Set the netsoul socket in blocking mode.
 *
 * @param       N
 *                      the Netsoul session
 *
 * @return      0 on success
 */
int             netsoul_set_blocking(netsoulSession *N)
{
  int           flags = fcntl(N->sockfd, F_GETFL);

  return fcntl(N->sockfd, F_SETFL, flags & ~O_NONBLOCK) != -1;
}

/**
 * Set the netsoul socket in non blocking mode.
 *
 * @param       N
 *                      the Netsoul session
 *
 * @return      0 on success
 */
int             netsoul_set_nonblocking(netsoulSession *N)
{
  int           flags = fcntl(N->sockfd, F_GETFL);

  return fcntl(N->sockfd, F_SETFL, flags | O_NONBLOCK) != -1;
}
