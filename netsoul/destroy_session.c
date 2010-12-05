/*
 * destroy_session.c for netsoul
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

#include "netsoul.priv.h"

/**
 * Destroy the given Netsoul session.
 *
 * @param       N
 *                      the Netsoul session
 */
void             netsoul_destroy_session(netsoulSession *N)
{
  if (N->addr != NULL)
    freeaddrinfo(N->addr);      /* no longer needed */

  if (N->sockfd != -1)
    netsoul_deconnect(N);

  if (N->watch_list != NULL)
    free(N->watch_list);

  free(N);
}
