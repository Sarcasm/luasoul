/*
 * window.c for luasoul
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

/* curses.h and _XOPEN_SOURCE_EXTENDED before panel.h */
#include "ui/ui_utils.h"
#include <panel.h>

#include <stdlib.h>
#include <string.h>
#include "lua/lua_utils.h"
#include "ui/window.h"
#include "ui/style.h"

/* not in the header because need curses.h */
PANEL   *check_window(lua_State *L, int n);

/* Constructor */

/*
  Create a new window
  Window(width, height, begin_x, begin_y)
*/
int             lui_new_window(lua_State *L)
{
  int width     = luaL_checkint(L, 1);
  int height    = luaL_checkint(L, 2);
  int begin_x   = luaL_checkint(L, 3);
  int begin_y   = luaL_checkint(L, 4);
  WINDOW        *win;
  PANEL         *pan;

  win = newwin(height, width, begin_y, begin_x);
  wrefresh(win);
  pan = new_panel(win);

  update_panels();
  wnoutrefresh(win);
  doupdate();

  *((PANEL **) lua_newuserdata(L, sizeof(pan))) = pan;

  /* set instance metatable to registered methods */
  luaL_getmetatable(L, WINDOW_CLASS);
  lua_setmetatable(L, -2);

  return 1;
}


/* Getters */

/* this structure map members to getters() */
static const t_index_wrap       lui_window_get_methods[]=
  {
    {lui_window_get_hidden,     REG_MEMBER("hidden")},
    {lui_window_get_autoscroll, REG_MEMBER("autoscroll")},
    {lui_window_get_cursor_x,   REG_MEMBER("cursor_x")}, /* FIXME: find another name */
    {lui_window_get_cursor_y,   REG_MEMBER("cursor_y")}, /* FIXME: find another name */
    {NULL,                      NULL, 0}
  };


/*
  This function handle the `__index' field.

  Stack:
  1: the instance table
  2: the accessed key
*/
int             lui_window_index(lua_State *L)
{
  ooHandleIndex(lui_window_get_methods);
}

/*
  window.hidden -> true/false
  the window is visible?

  Stack:
  1: the instance table
  2: the accessed key
*/
int             lui_window_get_hidden(lua_State *L)
{
  PANEL         *p = check_window(L, 1);

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
int             lui_window_get_autoscroll(lua_State *L)
{
  PANEL         *p = check_window(L, 1);

  lua_pushboolean(L, (is_scrollok(panel_window(p)) == TRUE));
  return 1;
}

/*
  window.cursor.x
  return the column number of the cursor

  Stack:
  1: the instance table
  2: the accessed key
*/
int             lui_window_get_cursor_x(lua_State *L)
{
  PANEL         *p = check_window(L, 1);

  lua_pushinteger(L, getcurx(panel_window(p)));
  return 1;
}

/*
  window.cursor.y
  return the row number of the cursor

  Stack:
  1: the instance table
  2: the accessed key
*/
int             lui_window_get_cursor_y(lua_State *L)
{
  PANEL         *p = check_window(L, 1);

  lua_pushinteger(L, getcury(panel_window(p)));
  return 1;
}



/* Setters */

/* this structure map members to setters() */
static const t_index_wrap       lui_window_set_methods[]=
  {
    {lui_window_set_style,      REG_MEMBER("style")},
    {lui_window_set_hidden,     REG_MEMBER("hidden")},
    {lui_window_set_autoscroll, REG_MEMBER("autoscroll")},
    {NULL,                      NULL, 0}
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
int             lui_window_newindex(lua_State *L)
{
  return lua_oo_accessors(L, lui_window_set_methods);
}

/*
  window:set_win_style{
  bold = true,
  foreground = 3,
  ...
  }

  set the global style of the window, take effect immediatly for each row
  of the window

  Stack:
  2nd argument is a table who can contain all the attributes above

  TODO: Optimisation ?
*/

int             lui_window_set_style(lua_State *L)
{
  WINDOW        *w = panel_window(check_window(L, 1));
  t_style       s;

  get_style(L, 3, s);
  wbkgd(w, s.on & ~s.off);
  return 0;
}

/*
  window.["hidden"] = true/false
  hide or unhide the window.

  Stack:
  1: the instance table
  2: the accessed key
  3: the value to set
*/
int             lui_window_set_hidden(lua_State *L)
{
  PANEL         *p = check_window(L, 1);

  if (lua_toboolean(L, 3))      /* hidden = true -> hide the window */
    hide_panel(p);
  else                         /* hidden = false -> show the window */
    show_panel(p);
  update_panels();
  return 0;                     /* TODO: send something ? */
}

/*
  window["autoscroll"] = true/false
  enable/disable autoscrolling

  Stack:
  1: the instance table
  2: the accessed key
  3: the value to set
*/
int             lui_window_set_autoscroll(lua_State *L)
{
  WINDOW        *w = panel_window(check_window(L, 1));

  scrollok(w, lua_toboolean(L, 3) == 1 ? TRUE : FALSE);
  wrefresh(w);
  return 0;                     /* TODO: send something ? */
}



/* Utils */

static const luaL_reg lui_window_instance_methods[]=
  {
    {"refresh",         lui_refresh_window},
    {"clear",           lui_clear_window},
    {"resize",          lui_resize_window},
    {"move",            lui_move_window},
    {"addch",           lui_addch_window},
    {"addstr",          lui_addstr_window},
    {"print_colored",   lui_print_colored_window},
    {"scroll",          lui_scroll_window},
    {"__index",         lui_window_index},
    {"__newindex",      lui_window_newindex},
    {"__gc",            lui_destroy_window},
    {"__tostring",      lui_window_tostring},
    {NULL,              NULL}
  };

/*
  Map all the fields of the class/object.
*/
int             lui_window_register(lua_State *L)
{
  ooHandleFuncMapping(WINDOW_CLASS, lui_window_instance_methods);
}

/*
  Check if value at index `n' in the stack is a Window instance,
  return it or raised an error.
*/
PANEL           *check_window(lua_State *L, int n)
{
  PANEL         **win;

  luaL_checktype(L, n, LUA_TUSERDATA);
  win = (PANEL **) luaL_checkudata(L, n, WINDOW_CLASS);
  if (win == NULL)
    luaL_typerror(L, n, WINDOW_CLASS);
  if (*win == NULL)
    luaL_error(L, "null window");
  return *win;
}


/* Methods  */

/*
  window:refresh()
  refresh the window (update physical window to match virtual window).
*/
int             lui_refresh_window(lua_State *L)
{
  WINDOW        *w = panel_window(check_window(L, 1));

  /*
    FIXME:
    set the bkgd each time we refresh is absolutely not necessary,
    but when we clear the screen it's useful...
  */
  wbkgd(w, getbkgd(w));

  update_panels();
  doupdate();
  return 0;
}

/*
  window:clear()
  clearing the window
*/
int             lui_clear_window(lua_State *L)
{
  PANEL         *p;

  p = check_window(L, 1);

  wclear(panel_window(p));
  update_panels();
  doupdate();
  return 0;
}

/*
  window:print_colored(s, {
  bold = true,
  foreground = 3,
  ...
  })

  Stack:
  2nd the string to display
  3rd argument is a table who can contain all the attributes above

  TODO: Optimisation ?
*/
int             lui_print_colored_window(lua_State *L)
{
  WINDOW        *w = panel_window(check_window(L, 1));
  const char    *s = luaL_checkstring(L, 2); /* string to display */
  attr_t        attr;
  short         pair;
  t_style       style;

  /* save current state */
  wattr_get(w, &attr, &pair, NULL);

  get_style(L, 3, style);
  wattron(w, style.on);
  wattroff(w, style.off);

  /* put the string */
  waddstr(w, s);

  /* restore old attr */
  wattr_set(w, attr, pair, NULL);

  return 0;
}

/*
  window:addch(c)
  put a char in the virtual window
*/
int             lui_addch_window(lua_State *L)
{
  WINDOW        *w = panel_window(check_window(L, 1));
  const char    *str = luaL_checkstring(L, 2);

  if (*str)
    waddch(w, *str);
  return 0;
}

/*
  window:addstr(s)
  put a string in the virtual window
*/
int             lui_addstr_window(lua_State *L)
{
  PANEL         *p;
  char          *str;
  WINDOW        *w;

  p = check_window(L, 1);
  str = luasoul_strdup(luaL_checkstring(L, 2));
  w = panel_window(p);

  waddstr(w, str);
  free(str);
  return 0;
}

/*
  window:resize(width, height)
  resize the window
*/
int             lui_resize_window(lua_State *L)
{
  PANEL         *p;
  WINDOW        *w;
  const int     width = luaL_checkint(L, 2);
  const int     height = luaL_checkint(L, 3);

  p = check_window(L, 1);
  w = panel_window(p);

  if (width <= 0 && height <= 0)
    {
      luaL_error(L, "can't resize window: invalid dimension");
      return 0;
    }
  wresize(w, height, width);
  return 0;
}

/*
  window:move(x, y)
  move the window
*/
int             lui_move_window(lua_State *L)
{
  PANEL         *p = check_window(L, 1);
  const int     x = luaL_checkint(L, 2);
  const int     y = luaL_checkint(L, 3);

  move_panel(p, y, x);
  return 0;
}

/*
  window:scroll(nlines)
  if nlines > 0 scroll up nlines
  otherwise scroll down nlines
*/
int             lui_scroll_window(lua_State *L)
{
  PANEL         *p;
  WINDOW        *w;
  int           nlines;

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
int             lui_window_tostring(lua_State *L)
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
int             lui_destroy_window(lua_State *L)
{
  PANEL         *p;
  WINDOW        *w;

  p = check_window(L, 1);
  w = panel_window(p);
  del_panel(p);
  delwin(w);
  return 0;
}
