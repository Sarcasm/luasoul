/*
** chatbox.h for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
** 
** Started on  Mon Nov  1 09:48:19 2010 Guillaume Papin
** Last update Sun Nov 21 17:07:43 2010 Guillaume Papin
*/

#ifndef _UI_CHATBOX_H_
#define _UI_CHATBOX_H_

#include "lua/ooutils.h"

#define CHATBOX_CLASS	"Chatbox"	   /* Object 'class' name */

  /* Constructor */
int		lui_new_chatbox(lua_State *L);

  /* Getters */
int		lui_chatbox_index(lua_State *L);

  /* Setters */
int		lui_chatbox_newindex(lua_State *L);
int		lui_chatbox_set_style(lua_State *L);

  /* Utils */
int		lui_chatbox_register(lua_State *L);

  /* Methods  */
int		lui_refresh_chatbox(lua_State *L);
int		lui_print_colored_chatbox(lua_State *L);
int		lui_addch_chatbox(lua_State *L);
int		lui_addstr_chatbox(lua_State *L);
int		lui_resize_chatbox(lua_State *L);
int		lui_move_chatbox(lua_State *L);
int		lui_scroll_chatbox(lua_State *L);
int		lui_chatbox_tostring(lua_State *L);

  /* Destructor */
int		lui_destroy_chatbox(lua_State *L);

#endif /* _UI_CHATBOX_H_ */
