/*
** luasoul.c for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Wed Oct  6 00:43:31 2010 Guillaume Papin
** Last update Tue Oct 12 22:46:40 2010 Guillaume Papin
*/

#include <stdlib.h>
#include <stdio.h>
#include "lua_utils.h"
#include "ui.h"

int		main(void)
{
  lua_State	*L;
  int		ch;

  /* init lua */
  L = load_lua();
  if (L == NULL)
    return 1;

  /* initialize ncurses */
  init_ui();

  /* load rc.lua */
  if (load_config(L) == -1)
    return 1;

  #include <curses.h>		/* getch() */
  while ((ch = getch()) != 'q')
    {
      call_lua_function(L, "getch", "is", ch, keyname(ch));
    }

  lua_close(L);
  ui_close();
  return (0);
}
