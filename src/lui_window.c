/*
** lui_window.c for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Thu Oct  7 22:39:56 2010 Guillaume Papin
** Last update Tue Oct 12 23:43:19 2010 Guillaume Papin
*/

/*
  `OOP model' inspired by:
  http://lua-users.org/wiki/UserDataWithPointerExample
  http://lua-users.org/wiki/ObjectProperties
*/

#define WINDOW_CLASS	"Window"	  /* class table name */
#define WINDOW_INST	"Window-instance" /* instance table name */

#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <panel.h>
#include "lua_utils.h"
#include "ncurses_utils.h"
#include "lui_window.h"

/* not in the header because need curses.h */
PANEL	*check_window(lua_State *L, int n);

/* Constructor */

static const luaL_reg	lui_window_class_methods[]=
  {
    {"new",	ui_new_window},
    {NULL,	NULL}
  };


/*
  Create a new window
  Window.new()
*/
int		ui_new_window(lua_State *L)
{
  int width	= luaL_checkint(L, 1);
  int height	= luaL_checkint(L, 2);
  int begin_x	= luaL_checkint(L, 3);
  int begin_y	= luaL_checkint(L, 4);
  WINDOW	*win;
  PANEL		*pan;
  PANEL		**userdata_pan;


  win = newwin(height, width, begin_y, begin_x);
  wrefresh(win);
  pan = new_panel(win);
  update_panels();
  wnoutrefresh(win);
  doupdate();

  userdata_pan = lua_newuserdata(L, sizeof(pan));
  *userdata_pan = pan;

  /* set instance metatable to registered methods */
  luaL_getmetatable(L, WINDOW_INST);
  lua_setmetatable(L, -2);

  return 1;
}



/* Accessor utils */

/* 
   Handler to the Window instance members,
   find the members and execute the associated function.

   Stack:
   1: the instance table
   2: the accessed key
   3: the value to set (optional, just for setters)
*/
int		lui_window_accessors(lua_State *L, const t_index_wrap *p)
{
  const char	*key = luaL_checkstring(L, 2);
  const size_t	len = strlen(key);

  for (; p->name != NULL; p++)
    if (p->len == len &&
	! strcmp(p->name, key))
      return p->func(L);
  return 0;			/* nothing is returned, TODO: maybe send nil ? */
}

/* Getters */

/* this structure map members to getters() */
static const t_index_wrap	lui_window_get_methods[]=
  {
    {ui_window_get_hidden,	REG_MEMBER("hidden")},
    {ui_window_get_autoscroll,	REG_MEMBER("autoscroll")},
    {NULL, NULL, 0}
  };


/*
  This function handle the `__index' field.

  Stack:
  1: the instance table
  2: the accessed key
*/
int		lui_window_index(lua_State *L)
{
  const char	*key = luaL_checkstring(L, 2);

  lua_getmetatable(L, 1);
  lua_getfield(L, -1, key);

  /* Either key is name of a method in the metatable */
  if(!lua_isnil(L, -1))
    return 1;

  /* ... or its a field access, so recall as self.get(self, value). */
  lua_settop(L, 2);   /* restore the initial call state, arg1, arg2 */

  return lui_window_accessors(L, lui_window_get_methods);
}

/* 
   window.hidden -> true/false
   the window is visible?

   Stack:
   1: the instance table
   2: the accessed key
*/
int		ui_window_get_hidden(lua_State *L)
{
  PANEL		*p;

  p = check_window(L, 1);
  lua_pushboolean(L, (panel_hidden(p) == TRUE));
  return 1;
}

/* 
   window.autoscroll -> true/false
   the window can scroll?

   Stack:
   1: the instance table
   2: the accessed key
*/
int		ui_window_get_autoscroll(lua_State *L)
{
  PANEL		*p;

  p = check_window(L, 1);
  lua_pushboolean(L, (is_scrollok(panel_window(p)) == TRUE));
  return 1;
}



/* Setters */

/* this structure map members to setters() */
static const t_index_wrap	lui_window_set_methods[]=
  {
    {ui_window_set_hidden,	REG_MEMBER("hidden")},
    {ui_window_set_autoscroll,	REG_MEMBER("autoscroll")},
    {NULL, NULL, 0}
  };

/*
  window.key = true/false
  or
  window["key"] = true/false

  Stack:
  1: the instance table
  2: the accessed key
  3: the value to set
*/
int		lui_window_newindex(lua_State *L)
{
  return lui_window_accessors(L, lui_window_set_methods);
}

/* 
   window.["hidden"] = true/false
   hide or unhide the window.

   Stack:
   1: the instance table
   2: the accessed key
   3: the value to set
*/
int		ui_window_set_hidden(lua_State *L)
{
  PANEL		*p = check_window(L, 1);

  if (lua_toboolean(L, 3))	/* hidden = true -> hide the window */
    hide_panel(p);
  else			       /* hidden = false -> show the window */
    show_panel(p);
  update_panels();
  return 0;			/* TODO: send something ? */
}

/* 
   window.["autoscroll"] = true/false
   enable/disable autoscrolling

   Stack:
   1: the instance table
   2: the accessed key
   3: the value to set
*/
int		ui_window_set_autoscroll(lua_State *L)
{
  WINDOW	*w = panel_window(check_window(L, 1));

  scrollok(w, lua_toboolean(L, 3) == 1 ? TRUE : FALSE);
  wrefresh(w);
  return 0;			/* TODO: send something ? */
}



/* Utils */

static const luaL_reg lui_window_instance_methods[]=
  {
    {"addstr",		ui_addstr_window},
    {"refresh",		ui_refresh_window},
    {"scroll",		ui_scroll_window},
    {"__index",		lui_window_index},
    {"__newindex",	lui_window_newindex},
    {"__gc",		lui_destroy_window},
    {"__tostring",	lui_window_tostring},
    {NULL, NULL}
  };

/*
  Map all the fields of the class/object.
*/
int		lui_window_register(lua_State *L)
{
  luaL_register(L, WINDOW_CLASS, lui_window_class_methods);
  luaL_newmetatable(L, WINDOW_INST);
  luaL_register(L, NULL, lui_window_instance_methods);

  /* __METATABLE */
  /*
    __metatable is for protecting metatables. If you do not want a program
    to change the contents of a metatable, you set its __metatable field.
    With that, the program cannot access the metatable (and therefore cannot
    change it).
  */
  lua_pushliteral(L, "__metatable");
  lua_pushvalue(L, -2);		/* dup methods table*/
  lua_rawset(L, -3);		/* metatable.__metatable = metatable */


  /* __INDEX */
  /*
    When it is a function, Lua calls it with the table and the absent
    key as its arguments.
    When it is a table, Lua redoes the access in that table.
  */


  lua_pop(L, 1);		/* drop metatable */
  return 0;
}

/*
  Check if value at index `n' in the stack is a Window instance,
  return it or raised an error.
*/
PANEL		*check_window(lua_State *L, int n)
{
  PANEL		**win;

  luaL_checktype(L, n, LUA_TUSERDATA);
  win = (PANEL **) luaL_checkudata(L, n, WINDOW_INST);
  if (win == NULL)
    luaL_typerror(L, n, WINDOW_INST);
  if (*win == NULL)
    luaL_error(L, "null window");
  return *win;
}



/* Methods  */

/*
  window:refresh()
  refresh the window (update physical window to match virtual window).
*/
int		ui_refresh_window(lua_State *L)
{
  PANEL		*p;

  p = check_window(L, 1);
  update_panels();
  doupdate();
  return 0;
}

/*
  window:addstr(s)
  put a string in the virtual window
*/
int		ui_addstr_window(lua_State *L)
{
  PANEL		*p;
  char		*str;
  WINDOW	*w;

  p = check_window(L, 1);
  str = strdup(luaL_checkstring(L, 2));
  w = panel_window(p);

  waddstr(w, str);
  return 0;
}

/*
  window:scroll(nlines)
  if nlines > 0 scroll up nlines
  otherwise scroll down nlines
*/
int		ui_scroll_window(lua_State *L)
{
  PANEL		*p;
  WINDOW	*w;
  int		nlines;

  p = check_window(L, 1);
  nlines = luaL_checkinteger(L, 2);
  w = panel_window(p);

  wscrl(w, nlines);
  wrefresh(w);
  return 0;
}

/* 
   tostring(window), window.__tostring
   just print the type and pointer address of the Window
*/
int		lui_window_tostring(lua_State *L)
{
  lua_pushfstring(L, "window: %p", lua_touserdata(L, 1));
  return 1;
}

/* Destructor */

/* 
   window.__gc
   destroy the window

   Stack:
   1: the instance table
   2: the accessed key
*/
int		lui_destroy_window(lua_State *L)
{
  PANEL		*p;
  WINDOW	*w;

  p = check_window(L, 1);
  if (p != NULL)
    {
      w = panel_window(p);
      del_panel(p);
      delwin(w);
    }
  return 0;
}
