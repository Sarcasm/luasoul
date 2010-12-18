/*
 * window.h for luasoul
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

#ifndef _UI_WINDOW_H_
#define _UI_WINDOW_H_

#include "lua/ooutils.h"

#define WINDOW_CLASS    "Window"          /* Object 'class' name */

  /* Constructor */
int             lui_new_window(lua_State *L);

  /* Getters */
int             lui_window_index(lua_State *L);
int             lui_window_get_hidden(lua_State *L);
int             lui_window_get_autoscroll(lua_State *L);
int             lui_window_get_cursor_x(lua_State *L);
int             lui_window_get_cursor_y(lua_State *L);

  /* Setters */
int             lui_window_newindex(lua_State *L);
int             lui_window_set_style(lua_State *L);
int             lui_window_set_hidden(lua_State *L);
int             lui_window_set_autoscroll(lua_State *L);

  /* Utils */
int             lui_window_register(lua_State *L);
/*
  PANEL         *check_window(lua_State *L, int n);
  this is not necessary for the other file and I don't want to
  include curses.h in this file.
*/

  /* Methods  */
int             lui_refresh_window(lua_State *L);
int             lui_resize_window(lua_State *L);
int             lui_move_window(lua_State *L);
int             lui_addch_window(lua_State *L);
int             lui_addstr_window(lua_State *L);
int             lui_scroll_window(lua_State *L);
int             lui_window_tostring(lua_State *L);
int             lui_clear_window(lua_State *L);
int             lui_print_colored_window(lua_State *L);

  /* Destructor */
int             lui_destroy_window(lua_State *L);

#endif /* _UI_WINDOW_H_ */
