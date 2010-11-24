/*
** input.h for luasoul in /home/papin_g/projects/luasoul/src
** 
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
** 
** Started on  Sun Nov 21 17:45:35 2010 Guillaume Papin
** Last update Wed Nov 24 23:45:43 2010 Guillaume Papin
*/

#ifndef _INPUT_H_
#define _INPUT_H_

#include "lua/ooutils.h"

#define INPUT_CLASS	"Input"		/* Object 'class' name */


/* Constructor */
int		lui_new_input(lua_State *L);

/* Getters */
int		lui_input_index(lua_State *L);
int		lui_input_get_index(lua_State *L);
int		lui_input_get_buff(lua_State *L);

/* Setters */
int		lui_input_newindex(lua_State *L);
int		lui_input_set_index(lua_State *L);
int		lui_input_set_style(lua_State *L);

/* Utils */
int		lui_input_register(lua_State *L);

/* Methods  */
int		lui_refresh_input(lua_State *L);
int		lui_erase_input(lua_State *L);
int		lui_delch_input(lua_State *L);
int		lui_addch_input(lua_State *L);
int		lui_input_tostring(lua_State *L);

/* Destructor */
int		lui_destroy_input(lua_State *L);


#endif /* _INPUT_H_ */
