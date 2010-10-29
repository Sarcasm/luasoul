/*
** utils.h for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
** 
** Started on  Wed Oct  6 20:52:18 2010 Guillaume Papin
** Last update Sun Oct 24 16:33:54 2010 Guillaume Papin
*/

#ifndef _UTILS_H_
#define _UTILS_H_

#define RC_NAME	"rc.lua"	/* configuration file name */

/* FIXME: suround this declaration by `#ifdef' */
char		*strndup(const char *s, size_t n);

void		error(const char *fmt, ...);
char		*get_rc(void);

/* doesn't allocated a string but s is modified */
char			*url_decode(char *s);
/* return an allocated string */
char			*url_encode(const char *s);

#endif /* _UTILS_H_ */
