/*
** input.h for luasoul in /home/papin_g/projects/luasoul/src
**
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
**
** Started on  Mon Nov  1 19:40:43 2010 Guillaume Papin
** Last update Sun Nov  7 19:52:31 2010 Guillaume Papin
*/

#ifndef _UI_INPUT_H_
#define _UI_INPUT_H_

#include "lua/ooutils.h"

#define INPUT_CLASS	"Input"		 /* class table name */
#define INPUT_INST	"Input-instance" /* instance table name */

  /* Constructor */
int		lui_new_input(lua_State *L);

  /* Getters */
int		lui_input_index(lua_State *L);
int		lui_input_get_buff(lua_State *L);
int		lui_input_get_index(lua_State *L);

  /* Setters */
int		lui_input_newindex(lua_State *L);
int		lui_input_set_style(lua_State *L);
int		lui_input_set_index(lua_State *L);
int		lui_input_set_buff(lua_State *L);

  /* Utils */
int		lui_input_register(lua_State *L);

  /* Methods  */
int		lui_refresh_input(lua_State *L);
int		lui_erase_input(lua_State *L);
int		lui_print_colored_input(lua_State *L);
int		lui_addch_input(lua_State *L);
int		lui_addstr_input(lua_State *L);
int		lui_remove_input(lua_State *L);
int		lui_input_move_cursor(lua_State *L);
int		lui_resize_input(lua_State *L);
int		lui_move_input(lua_State *L);
int		lui_scroll_input(lua_State *L);
int		lui_input_tostring(lua_State *L);

  /* Destructor */
int		lui_destroy_input(lua_State *L);

#endif /* _UI_INPUT_H_ */
