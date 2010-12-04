/*
 * set_status.c for luasoul
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

#include <time.h>

#include "netsoul.priv.h"

/**
 * Set a new Netsoul status.
 * common values :
 * actif, away, connection, idle, lock, server, none
 *
 * @param       N
 *                      the Netsoul session
 * @param       status
 *                      new status
 *
 * @return 0 on success (!= 0 on error).
 */
int             netsoul_set_status(netsoulSession *N, const char *status)
{
  /* status command format
     state <new status>:<timestamp> */
  return netsoul_send(N, "user_cmd state %s:%lu\n",
                      status,
                      time(NULL));
}
