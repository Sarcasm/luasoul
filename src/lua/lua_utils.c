/*
 * lua_utils.c for luasoul
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
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "lua_ui.h"
#include "lua/lua_utils.h"

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

  TODO: Return the number of results sent to the Lua VM
*/
void		call_lua_function(lua_State	*L,
				  const char	*func,
				  const char	*sig,
				  ...)
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

    case 'b':			/* boolean argument */
      lua_pushboolean(L, va_arg(vl, int));
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

    case 'b':			/* boolean result */
      if (!lua_isboolean(L, nres))
	error("wrong result type");
      *va_arg(vl, int *) = (int)lua_tonumber(L, nres);
      break;
    
    
    default:
      error("invalid option (%c)", *(sig - 1));
    }
    nres++;
  }
  va_end(vl);
}

/**
 * Get a wide character string and store the length.
 * @param n	the index of the lua string to convert
 * @param len
 * @return	the wide character string
 */
wchar_t		*luasoul_tolwcstr(lua_State *L, int n, size_t *len)
{
  const char	*str;
  size_t	 lstr;
  wchar_t	*wstr;

  luaL_checktype(L, n, LUA_TSTRING);
  str = lua_tolstring(L, n, &lstr);

  wstr = malloc((lstr + 1) * sizeof(wstr));
  if (wstr != NULL)
    {
      *len = mbstowcs(wstr, str, lstr);
      if (*len == (size_t) -1)
	{
	  free(wstr);
	  return NULL;
	}
      if (*len == lstr)
	wstr[lstr] = L'\0';
    }
  return wstr;
}

/**
 * Get a wide character string.
 * @param n	the index of the lua string to convert
 * @return	the wide character string
 */
wchar_t		*luasoul_towcstr(lua_State *L, int n)
{
  size_t	len;

  return luasoul_tolwcstr(L, n, &len);
}

/**
 * Call a Lua function when an error occur.
 * This function is protected against infinite recursion.
 */
int		luasoul_error(lua_State *L, const char *msg)
{
  static int	in = 0;

  /* already in the function */
  if (in)
    return 0;
  in = 1;
  /* 'protected block' */
  {  
    /* TODO: file, line number and function
       http://stackoverflow.com/questions/2780500
       http://stackoverflow.com/questions/2555856/current-line-number-in-lua
       http://www.lua.org/manual/5.1/manual.html#lua_getinfo */
    call_lua_function(L, "luasoul_error", "s", msg);
  }
  in = 0;
  return 0;
}

/* thx `luaL_checkudata()' in `lua-5.1.4/src/lauxlib.c:124' */
void		*luasoul_toclass(lua_State *L, int n, const char *tname)
{
  void		**udata = lua_touserdata(L, n);

  if (udata != NULL && lua_getmetatable(L, n))
    {
      lua_getfield(L, LUA_REGISTRYINDEX, tname);
      if (lua_rawequal(L, -1, -2))
	{
	  lua_pop(L, 2);	/* remove both metatables */
	  return *udata;
	}
    }
  return NULL;
}

/**
 * Push a wide character string on the stack.
 */
int		luasoul_pushwstring(lua_State *L, wchar_t *wstr)
{
  int		len = MB_LEN_MAX * wcslen(wstr);
  char		*buff = malloc((len + 1) * sizeof(*buff));

  if (buff == NULL || wcstombs(buff, wstr, len) == (size_t) -1)
    {
      lua_pushnil(L);
      luasoul_error(L, "can't convert wide character string");
    }
  else
    {
      lua_pushstring(L, buff);
      free(buff);
    }
  return 1;
}
