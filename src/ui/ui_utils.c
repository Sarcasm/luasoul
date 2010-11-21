/*
** utils.c for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
** 
** Started on  Wed Oct  6 22:39:15 2010 Guillaume Papin
** Last update Sun Nov 21 16:50:53 2010 Guillaume Papin
*/

#include <stdlib.h>
#include <locale.h>
#include "ui/ui_utils.h"

int		init_ui(void)
{
  setlocale(LC_ALL, "");
  initscr();

  /*
    FIXME: don't call raw AND cbreak()
    man curs_kernel for C-z and C-c ?
  */
  raw();			/* disable all signal like ^S, ^Q, ^C, ^Z.. */
  /* cbreak();			/\* `re-enable' ^C, ^Z *\/ */

  noecho();
  start_color();
  nonl();	/* enable ^M */

#ifdef NCURSES_VERSION
  /* get the terminal default color(for transparency) */
  use_default_colors();
#endif /* NCURSES_VERSION */

  /* enable keypad once (with that each input have automatically keypad-mode on) */
  keypad(stdscr, TRUE);

  /* FIXME: activate eight bit characters ? */
  meta(stdscr, TRUE);

  /* FIXME: I'm not sure... */
  idlok(stdscr, TRUE);

  refresh();
  return 0;
}

void		ui_close(void)
{
  endwin();
}
