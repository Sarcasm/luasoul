/*
** ooutils.h for luasoul	 in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
** 
** Started on  Fri Oct 22 23:42:55 2010 Guillaume Papin
** Last update Sun Nov 21 17:02:24 2010 Guillaume Papin
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

/*
  This define handle the `__index' field.

  Stack:
  1: the instance table
  2: the accessed key
*/
#define		ooHandleIndex(get_methods) const char	*key = luaL_checkstring(L, 2); \
									\
  lua_getmetatable(L, 1);						\
  lua_getfield(L, -1, key);						\
									\
  /* Either key is name of a method in the metatable */			\
  if (!lua_isnil(L, -1))						\
    return 1;								\
									\
  /* ... or its a field access, so recall as self.get(self, value). */	\
  lua_settop(L, 2);   /* restore the initial call state, arg1, arg2 */	\
									\
  return lua_oo_accessors(L, get_methods);

/*
  Map all the fields of the class/object.

  __METATABLE
									
  __metatable is for protecting metatables. If you do not want a program 
  to change the contents of a metatable, you set its __metatable field.
  With that, the program cannot access the metatable (and therefore cannot
  change it).


  __INDEX 
  
  When it is a function, Lua calls it with the table and the absent
  key as its arguments.
  When it is a table, Lua redoes the access in that table.
*/
#define		ooHandleFuncMapping(instName, instMethods)		\
  luaL_newmetatable(L, instName);					\
  luaL_register(L, NULL, instMethods);					\
									\
  lua_pushliteral(L, "__metatable");					\
  lua_pushvalue(L, -2);		/* dup methods table*/			\
  lua_rawset(L, -3);		/* metatable.__metatable = metatable */	\
									\
  lua_pop(L, 1);		/* drop metatable */			\
  return 0;								\



#endif /* _LUA_OOUTILS_H_ */
