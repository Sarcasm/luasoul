/*
 * chatbox.h for luasoul
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

#ifndef _UI_CHATBOX_H_
#define _UI_CHATBOX_H_

#include "lua/ooutils.h"

#define CHATBOX_CLASS	"Chatbox"	   /* Object 'class' name */

  /* Constructor */
int		lui_new_chatbox(lua_State *L);

  /* Getters */
int		lui_chatbox_index(lua_State *L);

  /* Setters */
int		lui_chatbox_newindex(lua_State *L);
int		lui_chatbox_set_style(lua_State *L);

  /* Utils */
int		lui_chatbox_register(lua_State *L);

  /* Methods  */
int		lui_refresh_chatbox(lua_State *L);
int		lui_print_colored_chatbox(lua_State *L);
int		lui_addch_chatbox(lua_State *L);
int		lui_addstr_chatbox(lua_State *L);
int		lui_resize_chatbox(lua_State *L);
int		lui_move_chatbox(lua_State *L);
int		lui_scroll_chatbox(lua_State *L);
int		lui_chatbox_tostring(lua_State *L);

  /* Destructor */
int		lui_destroy_chatbox(lua_State *L);

#endif /* _UI_CHATBOX_H_ */
