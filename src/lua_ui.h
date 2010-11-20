/*
** lua_ui.h for luasoul	 in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
** 
** Started on  Thu Oct  7 19:14:04 2010 Guillaume Papin
** Last update Sat Nov 20 13:17:36 2010 Guillaume Papin
*/

#ifndef _LUA_UI_H_
#define _LUA_UI_H_

#include "lua/lua_utils.h"

/* C-Lua stuff */
void	init_lua_ui(lua_State *L);
int	lui_handle_input(lua_State *L);

/* Lua functions */
int	lui_get_screen_size(lua_State *L);
int	lui_clear(lua_State *L);
int	lui_suspend(lua_State *L);
int	lui_define_key(lua_State *L);

#endif /* _LUA_UI_H_ */
