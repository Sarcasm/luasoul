/*
** lua_utils.c for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Wed Oct  6 21:02:00 2010 Guillaume Papin
** Last update Fri Oct 15 00:15:59 2010 Guillaume Papin
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
