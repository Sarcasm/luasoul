/*
 * send_typing.c for luasoul
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
 * Send an a typing start notification.
 *
 * @return 0 on success, -1 on error.
 */
int             netsoul_send_typing_start(netsoulSession *N,
                                          const char     *login)
{
  return
    (netsoul_send(N, "user_cmd msg_user %s dotnetSoul_UserTyping null\n",
                  login) != 0) ? -1 : 0;
}

/**
 * Send an a typing end notification.
 *
 * @return 0 on success, -1 on error.
 */
int             netsoul_send_typing_end(netsoulSession *N,
                                        const char     *login)
{
  return
    (netsoul_send(N,
                  "user_cmd msg_user %s dotnetSoul_UserCancelledTyping null\n",
                  login) != 0) ? -1 : 0;
}
