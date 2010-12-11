/*
 * netsoul.h for luasoul
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

#ifndef _PROTOCOL_NETSOUL_H_
#define _PROTOCOL_NETSOUL_H_

#include "lua/ooutils.h"

#define NETSOUL_CLASS   "Netsoul"               /* Object 'class' name */

/* Callback handlers */
int           ns_new_msg(void *L, const char *login,
                         const char *msg);
int           ns_typing_start(void *L, const char *login);
int           ns_typing_end(void *L, const char *login);
int           ns_login(void *L, const char *login);
int           ns_logout(void *L, const char *login);
int           ns_status_changed(void *L,const char *login,
                                const char *status);
int           ns_unknow_event(void *L, const char *msg);

/* Constructor */
int             new_ns(lua_State *L);

/* Getters */
int             ns_index(lua_State *L);

/* Setters */
int             ns_newindex(lua_State *L);
int             ns_set_ns_func(lua_State *L);

/* Utils */
int             ns_register(lua_State *L);

/* Methods  */
int             ns_connect(lua_State *L);
int             ns_deconnect(lua_State *L);
int             ns_spy_user(lua_State *L);
int             ns_send_msg(lua_State *L);
int             ns_send_typing_start(lua_State *L);
int             ns_send_typing_end(lua_State *L);
int             ns_tostring(lua_State *L);

/* Destructor */
int             destroy_ns(lua_State *L);

#endif /* _PROTOCOL_NETSOUL_H_ */
