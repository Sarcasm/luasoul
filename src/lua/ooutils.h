/*
** ooutils.h for luasoul	 in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Fri Oct 22 23:42:55 2010 Guillaume Papin
** Last update Sun Oct 24 12:11:15 2010 Guillaume Papin
*/

#ifndef _LUA_OOUTILS_H_
#define _LUA_OOUTILS_H_

/* Common includes for lua */
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define	REG_MEMBER(name) name, (sizeof(name) -1)
typedef struct
{
  lua_CFunction	func;		/* TODO: personal type here ? */
  const char	*name;
  const size_t	len;
}		t_index_wrap;

  /* Accessors utils */
int		lua_oo_accessors(lua_State *L, const t_index_wrap *p);

#endif /* _LUA_OOUTILS_H_ */
