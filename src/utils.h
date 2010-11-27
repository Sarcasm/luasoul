/*
 * utils.h for luasoul
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

#ifndef _UTILS_H_
#define _UTILS_H_

#include <wchar.h>		/* wide characters stuff */

#define RC_NAME	"rc.lua"	/* configuration file name */

#define	UNUSED_ATTRIBUTE	__attribute__ ((__unused__))

/* FIXME: suround this declaration by `#ifdef' */
char		*luasoul_strndup(const char *s, size_t n);
char		*luasoul_strdup(const char *s);
wchar_t		*luasoul_wcsdup(const wchar_t *s);
wchar_t		*luasoul_wcsndup(const wchar_t *s, size_t n);

void		error(const char *fmt, ...);
char		*get_rc(void);

/* doesn't allocated a string but s is modified */
char			*url_decode(char *s);
/* return an allocated string */
char			*url_encode(const char *s);

#endif /* _UTILS_H_ */
