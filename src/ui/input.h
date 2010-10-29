/*
** input.h for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Mon Oct 18 23:53:35 2010 Guillaume Papin
** Last update Thu Oct 28 23:43:55 2010 Guillaume Papin
*/

#ifndef _UI_INPUT_H_
#define _UI_INPUT_H_

#include "lua/ooutils.h"

#define INPUT_CLASS	"Input"		 /* class table name */
#define INPUT_INST	"Input-instance" /* instance table name */

  /* Constructor */
int		lui_new_input(lua_State *L);

  /* Accessors utils */

  /* Getters */
int		lui_input_index(lua_State *L);
int		lui_input_get_buff(lua_State *L);

  /* Setters */
int		lui_input_newindex(lua_State *L);
int		lui_input_set_buff(lua_State *L);

  /* Utils */
int		lui_input_register(lua_State *L);

  /* Methods  */
int		lui_input_driver(lua_State *L);
int		lui_refresh_input(lua_State *L);
int		lui_resize_input(lua_State *L);
int		lui_move_input(lua_State *L);

  /* Destructor */
int		lui_destroy_input(lua_State *L);


#endif /* _UI_INPUT_H_ */
