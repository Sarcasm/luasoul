/*
 * spy_user.c for luasoul
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
 * Watch the activity of the given user.
 * activity can be: login, logout, status change, ...
 *
 * @param       login
 *                      the login of the user to watch
 *
 * @return 0 on success, -1 on error.
 */
int             netsoul_spy_user(netsoulSession *N, const char *login)
{
  const char    *buddy[2];

  buddy[0] = login;
  buddy[1] = NULL;

  return netsoul_spy_users(N, buddy);
}

/**
 * Watch the activity of the given list of users.
 * activity can be: login, logout, status change, ...
 *
 * @param       logins
 *                      the list of user to watch for events
 *
 * @return 0 on success, -1 on error.
 */
int             netsoul_spy_users(netsoulSession *N, const char *logins[])
{
  int           i;
  size_t        len = 0;

  for (i = 0; logins[i] != NULL; i++)
    {
      size_t    login_len = strlen(logins[i]);

      len += login_len;
      /* check invalid login */
      if (login_len != strspn(logins[i], LOGIN_CHARACTERS))
        return -1;
    }

  /* 0 login -> bye */
  if (!i) return 0;

  /* here 'i - 1' is the number of ',' to add */
  len += i - 1;

  if (N->watch_list != NULL)
    {
      const size_t list_len = strlen(N->watch_list);

      /*
        +1 for ','
        +1 for '\0'
      */
      N->watch_list = realloc(N->watch_list, (len + list_len + 2)
                              * sizeof(*N->watch_list));
      if (N->watch_list == NULL)
        return -1;

      N->watch_list[list_len] = '\0';
      for (i = 0; logins[i] != NULL; i++)
        {
          strcat(N->watch_list, ",");
          strcat(N->watch_list, logins[i]);
        }
    }
  else
    {
      N->watch_list = malloc((len + 1) * sizeof(*N->watch_list));
      if (N->watch_list == NULL)
        return -1;

      strcpy(N->watch_list, logins[0]);
      for (i = 1; logins[i] != NULL; i++)
        {
          strcat(N->watch_list, ",");
          strcat(N->watch_list, logins[i]);
        }
    }

  return (netsoul_send(N, "user_cmd watch_log_user {%s}\n",
                       N->watch_list) != 0) ? -1 : 0;
}
