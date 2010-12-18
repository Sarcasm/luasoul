/*
 * style.h for luasoul
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

#ifndef _UI_STYLE_H_
#define _UI_STYLE_H_

#include "lua/ooutils.h"

#define STYLE_CLASS     "Style"          /* Object 'class' name */

/*
  This is the content of the userdata
  With attributs to set (on) and attributes to unset (off).
*/
typedef struct
{
  int           on;
  int           off;
}               t_style;


  /* Constructor */
int             lui_new_style(lua_State *L);

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
#define get_style(L, n, s)        if (lua_isuserdata(L, n))             \
    s = *check_style(L, n);                                             \
  else                                                                  \
    table_to_style(L, n, &s)

t_style         *check_style(lua_State *L, int n);
int             lui_style_register(lua_State *L);
void            table_to_style(lua_State *L, int n, t_style *s);


  /* Destructor */



#endif /* _UI_STYLE_H_ */
