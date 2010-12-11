/*
 * lua_ui.h for luasoul
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

#ifndef _LUA_UI_H_
#define _LUA_UI_H_

#include "lua/lua_utils.h"

/* C-Lua stuff */
void    init_lua_ui(lua_State *L);
int     lui_handle_input(lua_State *L);

/* Lua functions */
int     lui_get_screen_size(lua_State *L);
int     lui_clear(lua_State *L);
int     lui_quit(lua_State *L);
int     lui_suspend(lua_State *L);
int     lui_define_key(lua_State *L);

#endif /* _LUA_UI_H_ */
