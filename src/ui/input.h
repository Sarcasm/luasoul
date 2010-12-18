/*
 * input.h for luasoul
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

#ifndef _INPUT_H_
#define _INPUT_H_

#include "lua/ooutils.h"

#define INPUT_CLASS     "Input"         /* Object 'class' name */


/* Constructor */
int             lui_new_input(lua_State *L);

/* Getters */
int             lui_input_index(lua_State *L);
int             lui_input_get_index(lua_State *L);
int             lui_input_get_buff(lua_State *L);

/* Setters */
int             lui_input_newindex(lua_State *L);
int             lui_input_set_index(lua_State *L);
int             lui_input_set_style(lua_State *L);

/* Utils */
int             lui_input_register(lua_State *L);

/* Methods  */
int             lui_refresh_input(lua_State *L);
int             lui_resize_input(lua_State *L);
int             lui_move_input(lua_State *L);
int             lui_erase_input(lua_State *L);
int             lui_delch_input(lua_State *L);
int             lui_addch_input(lua_State *L);
int             lui_input_tostring(lua_State *L);

/* Destructor */
int             lui_destroy_input(lua_State *L);


#endif /* _INPUT_H_ */
