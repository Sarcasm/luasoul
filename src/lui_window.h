/*
** lui_window.h for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Thu Oct  7 22:41:34 2010 Guillaume Papin
** Last update Tue Oct 12 00:57:46 2010 Guillaume Papin
*/

#ifndef _LUI_WINDOW_H_
#define _LUI_WINDOW_H_


#define	REG_MEMBER(name) name, (sizeof(name) -1)
typedef struct
{
  lua_CFunction	func;		/* TODO: personal type here ? */
  const char	*name;
  const size_t	len;
}		t_index_wrap;


  /* Constructor */
int		ui_new_window(lua_State *L);

  /* Accessors utils */
int		lui_window_accessors(lua_State *L, const t_index_wrap *p);

  /* Getters */
int		lui_window_index(lua_State *L);
int		ui_window_get_hidden(lua_State *L);
int		ui_window_get_autoscroll(lua_State *L);

  /* Setters */
int		lui_window_newindex(lua_State *L);
int		ui_window_set_hidden(lua_State *L);
int		ui_window_set_autoscroll(lua_State *L);

  /* Utils */
int		lui_window_register(lua_State *L);
/*
  PANEL		*check_window(lua_State *L, int n);
  this is not necessary for the other file and I don't want to
  include curses.h in this file.
*/

  /* Methods  */
int		ui_refresh_window(lua_State *L);
int		ui_addstr_window(lua_State *L);
int		ui_scroll_window(lua_State *L);
int		lui_window_tostring(lua_State *L);

  /* Destructor */
int		lui_destroy_window(lua_State *L);

#endif /* _LUI_WINDOW_H_ */
