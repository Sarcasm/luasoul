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

#include "netsoul.h"

/**
 * Set the netsoul socket in blocking mode.
 *
 * @param       sockfd
 *                      the netsoul socket file descriptor
 *
 * @return      0 on success
 */
int             netsoul_set_blocking(int sockfd)
{
  int           flags = fcntl(sockfd, F_GETFL);

  return fcntl(sockfd, F_SETFL, flags & ~O_NONBLOCK) != -1;
}

/**
 * Set the netsoul socket in non blocking mode.
 *
 * @param       sockfd
 *                      the netsoul socket file descriptor
 *
 * @return      0 on success
 */
int             netsoul_set_nonblocking(int sockfd)
{
  int           flags = fcntl(sockfd, F_GETFL);

  return fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) != -1;
}
