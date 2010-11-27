/*
 * ui_utils.h for luasoul
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

#ifndef _UI_UTILS_H_
#define _UI_UTILS_H_

/* #define _XOPEN_SOURCE_EXTENDED	/\* want wide character support *\/ */
#include <curses.h>

/*
   source:
   http://www.mail-archive.com/debian-user@lists.debian.org/msg191079.html
*/
#undef CTRL
#define CTRL(x)	((x) & 0x1f)


int		init_ui(void);
void		ui_close(void);
void		ui_get_screen_size(int *width, int *height);

#endif /* _UI_UTILS_H_ */
