/*
** ncurses_utils.c for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Wed Oct  6 22:39:15 2010 Guillaume Papin
** Last update Sun Oct 10 19:40:23 2010 Guillaume Papin
*/

#include <stdlib.h>
#include <locale.h>
#include <curses.h>
#include "ncurses_utils.h"

void		ui_get_screen_size(int *width, int *height)
{
  *width = getmaxx(stdscr);
  *height = getmaxy(stdscr);
}

int		init_ui(void)
{
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  start_color();
  use_default_colors();		/* get the terminal default color (for transparency) */
  keypad(stdscr, TRUE);
  refresh();
  return 0;
}

void		ui_close(void)
{
  endwin();
}
