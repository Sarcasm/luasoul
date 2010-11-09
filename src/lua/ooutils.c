/*
** ooutils.c for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
** 
** Started on  Fri Oct 22 23:42:35 2010 Guillaume Papin
** Last update Sun Nov  7 18:50:55 2010 Guillaume Papin
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
int		lua_oo_accessors(lua_State *L, const t_index_wrap *p)
{
  const char	*key = luaL_checkstring(L, 2);
  const size_t	len = strlen(key);

  for (; p->name != NULL; p++)
    if (p->len == len &&
	! strcmp(p->name, key))
      return p->func(L);
  return 0;			/* nothing is returned, TODO: maybe send nil ? */
}

