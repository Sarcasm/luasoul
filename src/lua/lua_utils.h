/*
** utils.h for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
** 
** Started on  Wed Oct  6 20:54:25 2010 Guillaume Papin
** Last update Sun Nov 21 01:08:00 2010 Guillaume Papin
*/

#ifndef _LUA_UTILS_H_
#define _LUA_UTILS_H_

/* Common includes for lua */
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "utils.h"

typedef struct		s_lua_function
{
  const char		*name;
  lua_CFunction		function;
}			t_lua_function;


/* Protos */
lua_State	*load_lua(void);
int		load_config(lua_State *L);
void		register_function(lua_State *L, t_lua_function functions[]);
void		call_lua_function(lua_State *L,const char *func,
				  const char *sig, ...);
wchar_t		*check_wcstr(lua_State *L, int n);
int		luasoul_error(lua_State *L, const char *msg);

#endif /* _LUA_UTILS_H_ */
