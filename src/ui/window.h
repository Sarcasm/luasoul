/*
** window.h for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Thu Oct  7 22:41:34 2010 Guillaume Papin
** Last update Mon Oct 25 22:45:23 2010 Guillaume Papin
*/

#ifndef _UI_WINDOW_H_
#define _UI_WINDOW_H_

#include "lua/ooutils.h"

#define WINDOW_CLASS	"Window"	  /* class table name */
#define WINDOW_INST	"Window-instance" /* instance table name */

  /* Constructor */
int		lui_new_window(lua_State *L);

  /* Getters */
int		lui_window_index(lua_State *L);
int		lui_window_get_hidden(lua_State *L);
int		lui_window_get_autoscroll(lua_State *L);
int		lui_window_get_cursor_x(lua_State *L);
int		lui_window_get_cursor_y(lua_State *L);

  /* Setters */
int		lui_window_newindex(lua_State *L);
int		lui_window_set_hidden(lua_State *L);
int		lui_window_set_autoscroll(lua_State *L);

  /* Utils */
int		lui_window_register(lua_State *L);
/*
  PANEL		*check_window(lua_State *L, int n);
  this is not necessary for the other file and I don't want to
  include curses.h in this file.
*/

  /* Methods  */
int		lui_refresh_window(lua_State *L);
int		lui_resize_window(lua_State *L);
int		lui_move_window(lua_State *L);
int		lui_addstr_window(lua_State *L);
int		lui_scroll_window(lua_State *L);
int		lui_window_tostring(lua_State *L);
int		lui_clear_window(lua_State *L);
int		lui_set_window_style(lua_State *L);
int		lui_set_window_attr(lua_State *L);
int		lui_print_colored_window(lua_State *L);

  /* Destructor */
int		lui_destroy_window(lua_State *L);

#endif /* _UI_WINDOW_H_ */
