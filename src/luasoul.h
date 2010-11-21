/*
** luasoul.h for luasoul in /home/papin_g/Downloads/ncurses-5.7/ncurses
** 
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
** 
** Started on  Sun Nov 14 21:29:05 2010 Guillaume Papin
** Last update Fri Nov 19 22:37:13 2010 Guillaume Papin
*/

#ifndef _LUASOUL_H_
#define _LUASOUL_H_

#include <lua.h>

void		luasoul_suspend(void);
void		luasoul_resume(void);
void		luasoul_resize(lua_State *L);

#endif /* _LUASOUL_H_ */
