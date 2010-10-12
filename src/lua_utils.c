/*
** lua_utils.c for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Wed Oct  6 21:02:00 2010 Guillaume Papin
** Last update Mon Oct 11 22:56:21 2010 Guillaume Papin
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "utils.h"
#include "lua_utils.h"
#include "lua_ui.h"
#include "lui_window.h"

/**
 * Create the lua environnement.
 * @return L
 */
lua_State	*load_lua(void)
{
  lua_State	*L;

  L = lua_open();
  if (L == NULL)
    {
      error("Can't execute lua_open\n");
      return NULL;
    }
  /* open standard libs */
  luaL_openlibs(L);
  init_lua_ui(L);
  lui_window_register(L);
  return L;
}

/**
 * Load configuration file.
 * @return L
 */
int		load_config(lua_State *L)
{
  char		*rc_file;
  int		success;

  rc_file = get_rc();
  if (rc_file != NULL)
    {
      success = ! luaL_loadfile(L, rc_file) && ! lua_pcall(L, 0, 0, 0);
      free(rc_file);
      if (success)
	return 0;
      error("%s.\n", lua_tostring(L, -1)); /* error raised on stack */
    }
  lua_close(L);
  return -1;
}

/**
 * Push a list of new lua functions
 * @param A pointer to a null terminated list
 */
void		register_function(lua_State *L, t_lua_function functions[])
{
  int		i = 0;

  while (functions[i].name != NULL)
    {
      lua_pushcfunction(L, functions[i].function);
      lua_setglobal(L, functions[i].name);
      i++;
    }
}

int		global_manip(lua_State *L, enum LUA_QUERY request,
			    const char *key, void *arg)
{
  switch (request) {
  case GET_STRING:
    lua_getglobal(L, key);
    if (lua_isstring(L, -1))
      *((char **)arg) = strdup(lua_tostring(L, -1));
    break ;
  default:
    break ;
  }
  return 0;
}

char		**get_array(lua_State *L, const char *name)
{
  char		**array = NULL;
  int		n;  

  lua_getglobal(L, name);
  if  (!lua_istable(L, -1))
    {
      printf("%s\n", "error table");
      return NULL;
    }

  /* Get the number of entry in contact list */
  n = lua_objlen(L, -1);
  if (n <= 0)
    return NULL;
  array = malloc(sizeof(*array) * (n + 1));
  if (array == NULL)
    return NULL;
  array[n] = NULL;

  /* first key */
  lua_pushnil(L);
  /* table is in the stack at index '-2' after the push_nil */
  while (lua_next(L, -2) != 0) {
    /* uses 'key' (at index -2) and 'value' (at index -1) */
    /* removes 'value'; keeps 'key' for next iteration */
    if (lua_isstring(L, -1))
      array[--n] = strdup(lua_tostring(L, -1));
    else
      {
	error("%s contain invalid argument (string required).\n", name);
	while (array[n++] != NULL)
	  free(array[n]);
	free(array);
	return NULL;
      }
    lua_pop(L, 1);
  }
  return array;
}

int		table_manip(lua_State *L, enum LUA_QUERY request,
			    const char *key, void *arg)
{
  char		*error_type = NULL;
  static char	table[TABLE_MAXSIZE] = {0};
  int		len;
  int		nil;

  /* put the table on the stack */
  if (request == PUSH_TABLE)
    {
      /* save the table name for error printing */
      len = strlen(key);
      strncpy(table, key,
	      len >= TABLE_MAXSIZE ? TABLE_MAXSIZE -1 : len);
      table[len] = '\0';
      lua_getglobal(L, key);
      if (! lua_istable(L, -1))
	{
	  error("`%s' is not a valid configuration table\n", table);
	  return 1;
	}
      return 0;
    }

  /* get the value associated to the key according
     to the required type*/
  lua_getfield(L, -1, key);

  /* value is nil ? */
  if (request == IS_NIL)
    {
      nil = lua_isnil(L, -1);
      lua_pop(L, 1);  /* remove the value */
      return nil;
    }

  switch (request) {
  case GET_STRING:
    if (lua_isstring(L, -1))
      *((char **)arg) = strdup(lua_tostring(L, -1));
    else
      error_type = (char *) "string";
    break;
  case GET_NBR_STRING:
    if (lua_isnumber(L, -1))
      *((char **)arg) = strdup(lua_tostring(L, -1));
    else
      error_type = (char *) "number";
    break;
  case GET_INT:
    if (lua_isnumber(L, -1))
      *((int *)arg) = lua_tointeger(L, -1);
    else
      error_type = (char *) "number";
    break;
  case GET_BOOLEAN:
    if (lua_isboolean(L, -1))
      *((int *)arg) = lua_toboolean(L, -1);
    else
      error_type = (char *) "boolean";
    break;
  case GET_DOUBLE:
    if (lua_isnumber(L, -1))
      *((double *)arg) = lua_tonumber(L, -1);
    else
      error_type = (char *) "number";
    break;
  default:
    fprintf(stderr, "Internal error: request %d at %s, line %d.\n",
	    request, __FILE__, __LINE__);
    break;
  }
  lua_pop(L, 1);  /* remove the value */
  if (error_type != NULL)
    {
      error("`%s.%s' is not a valid %s\n", table, key, error_type);
      return 1;
    }
  return 0;
}

void		get_if_not_nil(lua_State *L, enum LUA_QUERY request,
			    const char *key, void *arg)
{
  if (!table_manip(L, IS_NIL, key, arg))
    table_manip(L, request, key, arg);
}

/*
  source: Programming in Lua
  url: http://www.lua.org/pil/25.3.html
*/
void		call_lua_function(lua_State *L,const char *func,
				  const char *sig, ...)
{
  va_list	vl;
  int		narg;		/* number of arguments  */
  int		nres;		/* results */
    
  va_start(vl, sig);
  lua_getglobal(L, func);	/* get function */
    
  /* push arguments */
  narg = 0;
  while (*sig && *sig != '>') {	/* push arguments */
    switch (*sig++) {
    
    case 'd':			/* double argument */
      lua_pushnumber(L, va_arg(vl, double));
      break;
    
    case 'i':			/* int argument */
      lua_pushnumber(L, va_arg(vl, int));
      break;
    
    case 's':			/* string argument */
      lua_pushstring(L, va_arg(vl, char *));
      break;
    
    default:
      error("invalid option (%c)", *(sig - 1));
    }
    narg++;
    luaL_checkstack(L, 1, "too many arguments");
  }


  if (*sig == '>')
    sig++;
    
  /* do the call */
  nres = strlen(sig);		/* number of expected results */
  if (lua_pcall(L, narg, nres, 0) != 0)	/* do the call */
    error("error running function `%s': %s",
	  func, lua_tostring(L, -1));
    
  /* retrieve results */
  nres = -nres;			/* stack index of first result */
  while (*sig) {		/* get results */
    switch (*sig++) {
    
    case 'd':			/* double result */
      if (!lua_isnumber(L, nres))
	error("wrong result type");
      *va_arg(vl, double *) = lua_tonumber(L, nres);
      break;
    
    case 'i':			/* int result */
      if (!lua_isnumber(L, nres))
	error("wrong result type");
      *va_arg(vl, int *) = (int)lua_tonumber(L, nres);
      break;
    
    case 's':			/* string result */
      if (!lua_isstring(L, nres))
	error("wrong result type");
      *va_arg(vl, const char **) = lua_tostring(L, nres);
      break;
    
    default:
      error("invalid option (%c)", *(sig - 1));
    }
    nres++;
  }
  va_end(vl);
}
