/*
 * ui_utils.c for luasoul
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

#include <stdlib.h>
#include "ui/ui_utils.h"

int             init_ui(void)
{
  initscr();

  /*
    FIXME: don't call raw AND cbreak()
    man curs_kernel for C-z and C-c ?
  */
  raw();                        /* disable all signal like ^S, ^Q, ^C, ^Z.. */
  /* cbreak();                  /\* `re-enable' ^C, ^Z *\/ */

  noecho();
  start_color();
  nonl();       /* enable ^M */

#ifdef NCURSES_VERSION
  /* get the terminal default color(for transparency) */
  use_default_colors();
#endif /* NCURSES_VERSION */

  /* enable keypad once (with that each input have automatically keypad-mode on) */
  keypad(stdscr, TRUE);

  /* FIXME: activate eight bit characters ? */
  meta(stdscr, TRUE);

  /* select() should already block */
  nodelay(stdscr, TRUE);

  /* FIXME: I'm not sure... */
  idlok(stdscr, TRUE);

  refresh();
  return 0;
}

void            ui_close(void)
{
  endwin();
}
