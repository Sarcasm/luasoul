/*
** utils.c for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
** 
** Started on  Wed Oct  6 22:39:15 2010 Guillaume Papin
** Last update Sat Nov 13 14:34:17 2010 Guillaume Papin
*/

#include <stdlib.h>
#include <locale.h>
#include <curses.h>
#include "ui/ui_utils.h"

/**
 * This function create a new color pair
 * @param tmp if true pair number can be re-used later,
 * otherwise pair number is definitive.
 * @return the number of the new color pair
 */
int		new_pair(int fg, int bg, int tmp)
{
  static int	last_pair = 1;	/* tmp pair number is 1 */

  if (tmp == TRUE)
    {
      init_pair(1, fg, bg);
      return COLOR_PAIR(1);
    }
  init_pair(++last_pair, fg, bg);	/* skip last pair and create a new pair */
  return COLOR_PAIR(last_pair);
}

int		init_ui(void)
{
  setlocale(LC_ALL, "");
  initscr();

  /*
    FIXME: don't call raw AND cbreak()
    man curs_kernel for C-z and C-c ?
  */
  raw();			/* disable all signal like ^S, ^Q, ^C, ^Z.. */
  cbreak();			/* `re-enable' ^C, ^Z */

  noecho();
  start_color();
  nonl();	/* enable ^M */

  /* get the terminal default color(for transparency) */
  use_default_colors();

  /* enable keypad once (with that each input have automatically keypad-mode on) */
  keypad(stdscr, TRUE);

  /* activate eight bit characters */
  meta(stdscr, TRUE);

  /* I'm not sure... */
  idlok(stdscr, TRUE);

  refresh();
  return 0;
}

void		ui_close(void)
{
  endwin();
}
