/*
** lua_ui.c for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Thu Oct  7 19:12:49 2010 Guillaume Papin
** Last update Mon Oct 11 22:56:13 2010 Guillaume Papin
*/

#include <stdlib.h>
#include <string.h>
#include "lua_ui.h"
#include "ui.h"


/* list of lua functions for the user interface */
t_lua_function				lua_ui_functions[]=
  {
    {"get_screen_size",	lui_get_screen_size},
    {NULL,				NULL}
  };

void		init_lua_ui(lua_State *L)
{
  register_function(L, (t_lua_function *) (lua_ui_functions));
}

/* return width and height of the screen */
int		lui_get_screen_size(lua_State *L)
{
  int		width, height;

  ui_get_screen_size(&width, &height);
  lua_pushnumber(L, width);
  lua_pushnumber(L, height);
  return 2;
}
