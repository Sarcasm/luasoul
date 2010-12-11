/*
 * lua_protocol.h for luasoul
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

#ifndef _LUA_PROTOCOL_H_
#define _LUA_PROTOCOL_H_

#include "lua/lua_utils.h"

/* C-Lua stuff */
void            init_lua_protocol(lua_State *L);
void            create_protocol_ref(lua_State *L);
int             get_protocol_ref(lua_State *L);

/* Thx $(uuidgen) */
#define PROTOCOL_REF_NAME "66c0dc8e-db62-489d-90c3-8e3d614afb8e"

#endif /* _LUA_PROTOCOL_H_ */
