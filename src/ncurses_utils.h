/*
** ncurses_utils.h for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Wed Oct  6 22:39:43 2010 Guillaume Papin
** Last update Thu Oct  7 22:41:23 2010 Guillaume Papin
*/

#ifndef _NCURSES_UTILS_H_
#define _NCURSES_UTILS_H_

/*
   source:
   http://www.mail-archive.com/debian-user@lists.debian.org/msg191079.html
*/
#ifndef CTRL
#define CTRL(x)         ((x) & 0x1f)
#endif

/* DELETE */
void	ui_addstr(const char *s);


int	init_ui(void);
void	ui_close(void);
void	ui_get_screen_size(int *width, int *height);

#endif /* _NCURSES_UTILS_H_ */
