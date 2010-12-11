/*
 * lua_protocol.c for luasoul
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

#include <sys/select.h>         /* for FD_* stuff */

#include "lua_protocol.h"
#include "protocol/ns.h"

/* list of lua functions for the user interface */
t_lua_function          lua_protocol_functions[]=
  {
    {"Netsoul",         new_ns},
    {NULL,              NULL}
  };

void            init_lua_protocol(lua_State *L)
{
  register_function(L, (t_lua_function *) (lua_protocol_functions));

  /* Export LuaSoul `classes methods' */
  ns_register(L);
  create_protocol_ref(L);
}

/*
  Create a reference to the table that contains the list of
  Netsoul sessions.
 */
void            create_protocol_ref(lua_State *L)
{
  int           ref;

  lua_createtable(L, 0, 1); /* Probably only 1 netsoulSession will be here */
  ref = luaL_ref(L, LUA_REGISTRYINDEX);

  /* save the reference in the global environnement */
  lua_pushinteger(L, ref);
  lua_setglobal(L, PROTOCOL_REF_NAME);
}

/*
  Get the protocol table reference.
*/
int             get_protocol_ref(lua_State *L)
{
  lua_getglobal(L, PROTOCOL_REF_NAME);
  return lua_tointeger(L, -1);
}

/* Retrieve Netsoul fd */
#include "netsoul.h"
int             get_fd_list(lua_State *L, fd_set *readfds, const int ref)
{
  size_t        len;
  int           sockfd;
  netsoulSession *N;
  int           nb_fds = 0;

  lua_rawgeti(L, LUA_REGISTRYINDEX, ref);

  for (len = lua_objlen(L, -1); len != 0; len--)
    {
      lua_rawgeti(L, -1, len);

      luasoul_checkclass(L, -1, NETSOUL_CLASS, N);

      sockfd = netsoul_get_fd(N);

      if (sockfd != -1)
        {
          if (sockfd + 1 > nb_fds)
            nb_fds = sockfd + 1;
          FD_SET(sockfd, readfds);
        }

      lua_pop(L, 1);
    }
  return nb_fds;
}

int             handle_protocol(lua_State *L, fd_set *readfds, const int ref)
{
  size_t          len;
  int             sockfd;
  netsoulSession *N;

  lua_rawgeti(L, LUA_REGISTRYINDEX, ref);

  for (len = lua_objlen(L, -1); len != 0; len--)
    {
      lua_rawgeti(L, -1, len);

      luasoul_checkclass(L, -1, NETSOUL_CLASS, N);

      sockfd = netsoul_get_fd(N);

      if (sockfd != -1 && FD_ISSET(sockfd, readfds))
        {
          /* the key on the stack here is the userdatum, for the ns functions */
          if (netsoul_event_handler(N, L) != 0)
            luasoul_error(L, "Event error from Netsoul");
        }

      lua_pop(L, 1);
    }
  return 0;
}
