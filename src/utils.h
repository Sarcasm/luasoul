/*
** utils.h for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
** 
** Started on  Wed Oct  6 20:52:18 2010 Guillaume Papin
** Last update Sat Nov 20 13:35:07 2010 Guillaume Papin
*/

#ifndef _UTILS_H_
#define _UTILS_H_

#include <wchar.h>		/* wide characters stuff */

#define RC_NAME	"rc.lua"	/* configuration file name */

#define	UNUSED_ATTRIBUTE	__attribute__ ((__unused__))

/* FIXME: suround this declaration by `#ifdef' */
char		*strndup(const char *s, size_t n);
wchar_t		*luasoul_wcsdup(const wchar_t *s);
wchar_t		*luasoul_wcsndup(const wchar_t *s, size_t n);

void		error(const char *fmt, ...);
char		*get_rc(void);

/* doesn't allocated a string but s is modified */
char			*url_decode(char *s);
/* return an allocated string */
char			*url_encode(const char *s);

#endif /* _UTILS_H_ */
