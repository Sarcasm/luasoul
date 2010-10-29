/*
** utils.h for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Wed Oct  6 22:39:43 2010 Guillaume Papin
** Last update Mon Oct 25 21:50:55 2010 Guillaume Papin
*/

#ifndef _UI_UTILS_H_
#define _UI_UTILS_H_

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

#endif /* _UI_UTILS_H_ */
