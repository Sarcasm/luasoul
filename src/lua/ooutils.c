/*
 * ooutils.c for luasoul
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
#include "lua/ooutils.h"

/* Accessor utils */

/*
   Handler to the Window instance members,
   find the members and execute the associated function.

   Stack:
   1: the instance table
   2: the accessed key
   3: the value to set (optional, just for setters)
*/
int             lua_oo_accessors(lua_State *L, const t_index_wrap *p)
{
  const char   *key = luaL_checkstring(L, 2);
  const size_t  len = strlen(key);

  for (; p->name != NULL; p++)
    if (p->len == len &&
        ! strcmp(p->name, key))
      return p->func(L);
  return 0;                     /* nothing is returned, TODO: maybe send nil ? */
}
