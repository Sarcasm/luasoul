/*
 * lua_utils.h for luasoul
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

#ifndef _LUA_UTILS_H_
#define _LUA_UTILS_H_

/* Common includes for lua */
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "utils.h"

typedef struct          s_lua_function
{
  const char            *name;
  lua_CFunction         function;
}                       t_lua_function;


/* Protos */
lua_State       *load_lua(void);
int             load_config(lua_State *L);
void            register_function(lua_State *L, t_lua_function functions[]);
void            call_lua_function(lua_State *L,const char *func,
                                  const char *sig, ...);
wchar_t         *luasoul_tolwcstr(lua_State *L, int n, size_t *len);
wchar_t         *luasoul_towcstr(lua_State *L, int n);
int             luasoul_error(lua_State *L, const char *msg);
void            *luasoul_toclass(lua_State *L, int n, const char *tname);
int             luasoul_pushwstring(lua_State *L, wchar_t *wstr);

/* Get the integer at postion `narg' or return a luasoul_error */
#define luasoul_checkint(L, narg, dest) {               \
    if (lua_isnumber(L, narg))                          \
      dest = (int) lua_tointeger(L, narg);              \
    else                                                \
      return luasoul_error(L, "integer expected");      \
  }                                                     \

#define luasoul_optint(L, narg, dest, d) {                      \
    if (lua_isnumber(L, narg))                                  \
      dest = (int) lua_tointeger(L, narg);                      \
    else if (lua_type(L, narg) == LUA_TNONE)                    \
      dest = d;                                                 \
    else                                                        \
      return luasoul_error(L, "integer or none expected");      \
  }                                                             \

/* Get the string at postion `narg' or return a luasoul_error */
#define luasoul_checkstring(L, narg, dest) {            \
    if (lua_isstring(L, narg))                          \
      dest = lua_tostring(L, narg);                     \
    else                                                \
      return luasoul_error(L, "string expected");       \
  }                                                     \

#define luasoul_optstring(L, narg, dest, d) {                   \
    if (lua_isstring(L, narg))                                  \
      dest = lua_tostring(L, narg);                             \
    else if (lua_type(L, narg) == LUA_TNONE)                    \
      dest = d;                                                 \
    else                                                        \
      return luasoul_error(L, "string or none expected");       \
  }                                                             \

#define luasoul_checkstring_field(L, ntable, key, dest) {       \
    lua_getfield(L, ntable, key);                               \
    if (lua_isstring(L, -1))                                    \
      dest = lua_tostring(L, -1);                               \
    else                                                        \
      return luasoul_error(L, key ": string required");         \
    lua_pop(L, 1);                                              \
  }                                                             \

#define luasoul_optstring_field(L, ntable, key, dest, val) {            \
    lua_getfield(L, ntable, key);                                       \
    if (lua_isstring(L, -1))                                            \
      dest = lua_tostring(L, -1);                                       \
    else if (lua_type(L, -1) == LUA_TNIL)                               \
      dest = val;                                                       \
    else                                                                \
      return luasoul_error(L, key ": string or nothing expected");      \
  }                                                                     \

#define luasoul_optfunc_field(L, ntable, key, dest, val) {              \
    lua_getfield(L, ntable, key);                                       \
    if (lua_isfunction(L, -1))                                          \
      dest = lua_tostring(L, -1);                                       \
    else if (lua_type(L, -1) == LUA_TNIL)                               \
      dest = val;                                                       \
    else                                                                \
      return luasoul_error(L, key ": function or nothing expected");    \
  }                                                                     \

/* TODO: change 'tname expected' */
#define luasoul_checkclass(L, n, tname, dest)           \
  dest = luasoul_toclass(L, n, tname);                  \
  if (dest == NULL)                                     \
    return luasoul_error(L, "tname expected");          \

/* Get a wstring at postion `narg' or return a luasoul_error */
#define luasoul_checkwcstr(L, narg, dest) {             \
    if (lua_isstring(L, narg))                          \
      dest = luasoul_towcstr(L, narg);                  \
    else                                                \
      return luasoul_error(L, "string expected");       \
  }                                                     \

#define luasoul_checklwcstr(L, narg, dest, len) {       \
    if (lua_isstring(L, narg))                          \
      dest = luasoul_tolwcstr(L, narg, &(len));         \
    else                                                \
      return luasoul_error(L, "string expected");       \
  }                                                     \

#endif /* _LUA_UTILS_H_ */
