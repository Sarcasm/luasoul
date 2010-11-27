/*
 * style.c for luasoul
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
#include "lua/lua_utils.h"
#include "ui/ui_utils.h"
#include "ui/style.h"

/* Constructor */
/*
  Create a new style

  Style{
  normal	= true/false,
  bold		= true/false,
  underline	= true/false,
  blink		= true/false,
  standout	= true/false,
  reverse	= true/false,
  foreground	= <number>,
  foreground	= <number>,
  }
  TODO: Destructor, free() the pointer
*/
int		lui_new_style(lua_State *L)
{
  t_style	*s;

  s = (t_style *) lua_newuserdata(L, sizeof(*s));

  /* transform received table into a new style */
  table_to_style(L, 1, s);

  /* set instance metatable to registered methods */
  luaL_getmetatable(L, STYLE_CLASS);
  lua_setmetatable(L, -2);

  return 1;
}



/* Utils */

/*
  Check if value at index `n' in the stack is a Style,
  return it or raised an error.
*/
t_style		*check_style(lua_State *L, int n)
{
  t_style	*s;

  luaL_checktype(L, n, LUA_TUSERDATA);
  s = (t_style *) luaL_checkudata(L, n, STYLE_CLASS);
  if (s == NULL)
    luaL_typerror(L, n, STYLE_CLASS);
  return s;
}

/*
  Map all the fields of the class/object.
*/
int		lui_style_register(lua_State *L)
{
  luaL_newmetatable(L, STYLE_CLASS);

  /* __METATABLE */
  /*
    __metatable is for protecting metatables. If you do not want a program
    to change the contents of a metatable, you set its __metatable field.
    With that, the program cannot access the metatable (and therefore cannot
    change it).
  */
  lua_pushliteral(L, "__metatable");
  lua_pushliteral(L, "you are a bad man"); /* TODO: ... */
  lua_rawset(L, -3);


  /* __INDEX */
  /*
    When it is a function, Lua calls it with the table and the absent
    key as its arguments.
    When it is a table, Lua redoes the access in that table.
  */
  lua_pushliteral(L, "__index");
  lua_pushliteral(L, "you are a very bad man"); /* TODO: ... */
  lua_rawset(L, -3);

  lua_pop(L, 1);		/* drop metatable */
  return 0;
}

/*
  This function create a new color pair.
*/
static int	new_pair(int fg, int bg)
{
  static int	last_pair = 0;

  init_pair(++last_pair, fg, bg);	/* skip last pair and create a new pair */
  return COLOR_PAIR(last_pair);
}

/* describe an attribute with name and value */
typedef struct
{
  const char	*name;
  const int	value;
}		t_style_attr;


t_style_attr	attr_list[]=
  {
    {"normal",		A_NORMAL},
    {"bold",		A_BOLD},
    {"underline",	A_UNDERLINE},
    {"blink",		A_BLINK},
    {"standout",	A_STANDOUT},
    {"reverse",		A_REVERSE},
    {NULL,		0}
  };

/**
 * transform a table to an understandable value for ncurses functions
 *
 * bool	normal
 * bool	bold
 * bool	underline
 * bool	blink
 * bool	standout
 * bool	reverse
 * int	foreground
 * int	background
 *
 * @param: L
 * @param: n	position of the table on the stack
 *
 * the following parameter is filled by the function
 * @param: s	attributes to turn on or off
 *
 * FIXME: vocabulary...?
 */
void	table_to_style(lua_State *L, int n, t_style *s)
{
  int		i;
  int		fg = -1;
  int		bg = -1;

  s->on = A_NORMAL;
  s->off = A_NORMAL;

  if (! lua_istable(L, n)) {
    luaL_error(L, "invalid argument, table expected (got %s)", lua_typename(L, lua_type(L, n)));
  }

  for (i = 0; attr_list[i].name != NULL; i++)
    {
      lua_getfield(L, n, attr_list[i].name); /* get key */

      if (! lua_isnoneornil(L, -1) && lua_isboolean(L, -1))
	{
	  if (lua_toboolean(L, -1)) /* t[key] = true */
	    s->on |= attr_list[i].value;
	  else			/* t[key] = false */
	    s->off |= attr_list[i].value;
	}
      lua_pop(L, 1);
    }

  /* foreground and background settings */
  if (has_colors())		/* terminal is capable to output colors ? */
    {
      lua_getfield(L, n, "foreground"); /* -2 */
      lua_getfield(L, n, "background"); /* -1 */

      /* at least one of background/foreground property is set ? */
      if (! lua_isnil(L, -1) || ! lua_isnil(L, -2))
  	{
  	  if (! lua_isnil(L, -2) && lua_isnumber(L, -2)) /* fg */
  	    fg = lua_tointeger(L, -2);

  	  if (! lua_isnil(L, -1) && lua_isnumber(L, -1)) /* bg */
  	    bg = lua_tointeger(L, -1);

  	  s->on |= new_pair(fg, bg);
  	}
      lua_pop(L, 2);
    }
}
