/*
** lua_utils.h for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Wed Oct  6 20:54:25 2010 Guillaume Papin
** Last update Mon Oct 11 02:18:35 2010 Guillaume Papin
*/

#ifndef _LUA_UTILS_H_
#define _LUA_UTILS_H_

/* Common includes for lua */
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


/* Max size of a table name */
#define		TABLE_MAXSIZE	42

enum	LUA_QUERY
  {
    PUSH_TABLE,			/* push a table */
    GET_STRING,			/* get a string value */
    GET_INT,			/* get an integer value */
    GET_DOUBLE,			/* self explanatory... */
    GET_BOOLEAN,		/* self explanatory... */
    GET_NBR_STRING,		/* get the string representation of a number */
    IS_NIL			/* if the key is set */
  };


typedef struct		s_lua_function
{
  const char		*name;
  lua_CFunction		function;
}			t_lua_function;


/* Protos */
lua_State	*load_lua(void);
int		load_config(lua_State *L);
void		register_function(lua_State *L, t_lua_function functions[]);
int		global_manip(lua_State *L, enum LUA_QUERY request,
			     const char *key, void *arg);
char		**get_array(lua_State *L, const char *name);
void		call_lua_function(lua_State *L,const char *func,
				  const char *sig, ...);

#endif /* _LUA_UTILS_H_ */
