/*
** lua_ui.h for luasoul	 in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Thu Oct  7 19:14:04 2010 Guillaume Papin
** Last update Sat Oct  9 00:17:36 2010 Guillaume Papin
*/

#ifndef _LUA_UI_H_
#define _LUA_UI_H_

#include "lua_utils.h"

/* lua functions */
void	init_lua_ui(lua_State *L);
int	lui_get_screen_size(lua_State *L);

#endif /* _LUA_UI_H_ */
