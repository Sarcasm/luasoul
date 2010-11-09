/*
** style.h for luasoul in /home/papin_g/projects/luasoul/src
** 
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
** 
** Started on  Mon Oct 25 21:50:43 2010 Guillaume Papin
** Last update Tue Nov  9 23:55:23 2010 Guillaume Papin
*/

#ifndef _UI_STYLE_H_
#define _UI_STYLE_H_

#include "lua/ooutils.h"

#define STYLE_CLASS	"Style"		 /* class table name */
#define STYLE_INST	"Style-instance" /* instance table name */

/*
  This is the content of the userdata
  With attributs to set (on) and attributes to unset (off).
*/
typedef struct
{
  int		on;
  int		off;
}		t_style;


  /* Constructor */
int		lui_new_style(lua_State *L);

  /* Accessors utils */
  /* Getters */
  /* Setters */
  /* Utils */
/*
  Set a new (definitive, used more than once) style
  L - The Lua VM
  n - Position on the stack of a Style, or a Lua table
  s - A structure (not a pointer) to fill
*/
#define	get_style(L, n, s)	  if (lua_isuserdata(L, n))		\
    s = *check_style(L, n);						\
  else									\
    table_to_style(L, n, &s)

t_style		*check_style(lua_State *L, int n);
int		lui_style_register(lua_State *L);
void		table_to_style(lua_State *L, int n, t_style *s);


  /* Destructor */



#endif /* _UI_STYLE_H_ */
