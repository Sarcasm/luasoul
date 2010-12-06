/*
 * send_msg.c for luasoul
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

#include "netsoul.priv.h"

/**
 * Send a message to the given user.
 *
 * @return 0 on success, -1 on error.
 */
int             netsoul_send_msg(netsoulSession *N,
                                 const char     *login,
                                 const char     *msg)
{
  char          buff[NETSOUL_MSG_SIZE];

  netsoul_url_encode(buff, msg, NETSOUL_MSG_SIZE -1);

  return (netsoul_send(N, "user_cmd msg_user %s msg %s\n", login,
                       buff) != 0) ? -1 : 0;
}
