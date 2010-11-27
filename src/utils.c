/*
 * utils.c for luasoul
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
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

void		error(const char *fmt, ...)
{
  va_list	argp;

  va_start(argp, fmt);
  fprintf(stderr, "error: ");
  vfprintf(stderr, fmt, argp);
  va_end(argp);
}

/*
  Duplicate a string.
*/
char		*luasoul_strdup(const char *s)
{
  char		*res;

  res = malloc((strlen(s) + 1) * sizeof(*res));
  return res != NULL ? strcpy(res, s) : NULL;
}

/*
  Duplicate at most n characters of the string s.
*/
char		*luasoul_strndup(const char *s, size_t n)
{
  char		*res;
  size_t	 len = strlen(s);

  if (n > len)
    n = len;
  res = malloc((n + 1) * sizeof(*res));
  return res != NULL ? strncpy(res, s, n) : NULL;
}

/*
  Duplicate a wide-character string.
*/
wchar_t		*luasoul_wcsdup(const wchar_t *s)
{
  wchar_t	*res;

  res = malloc((wcslen(s) + 1) * sizeof(*res));
  return res != NULL ? wcscpy(res, s) : NULL;
}

/*
  Duplicate at most n characters of the wide-character string s.
*/
wchar_t		*luasoul_wcsndup(const wchar_t *s, size_t n)
{
  wchar_t	*res;
  size_t	len = wcslen(s);

  if (n > len)
    n = len;
  res = malloc((n + 1) * sizeof(*res));
  return res != NULL ? wcsncpy(res, s, n) : NULL;
}

char		*get_rc(void)
{
  char		rc_path[FILENAME_MAX];
  char		*dir;

  /* file in current directory ? */
  if (! access(RC_NAME, F_OK))
    return luasoul_strdup(RC_NAME);

  /* file in XDG_CONFIG_HOME ? */
  dir = getenv("XDG_CONFIG_HOME");
  if (dir != NULL) {
    strcpy(rc_path, dir);
    strcat(rc_path, "/luasoul/");
    strcat(rc_path, RC_NAME);
    if (access(rc_path, F_OK) == 0)
      return luasoul_strdup(rc_path);
  }
  /* file not found */
  error("can't find configuration file: `%s'\n", RC_NAME);
  return NULL;
}

static char		hexa_value(char c)
{
  return (c >= '0' && c <= '9') ? c - '0' : (c & 0xDF) - 'A' + 10;
}

char			*url_decode(char *s)
{
  int			i = 0;
  char			*res = s;

  while (*s) {
    /* `%' take the 2 following numbers and convert them from hexa to dec */
    if (*s == '%')
      {
	/* case the string end with a `%' */
	if (! *(++s))
	  break ;
	/* first hexadecimal char here */
	res[i] = hexa_value(*(s++)) * 16;
	res[i] += hexa_value(*s);
      }
    else
      res[i] = *s;
    i++;
    s++;
  }
  res[i] = '\0';
  return res;
}

char			*url_encode(const char *s)
{
  char			*res;
  int			i = 0;

  res = malloc(3 * sizeof(*res) * strlen(s) + 1);
  while (*s) {
    /* reserved characters */
    if (strchr(" \n\r\v*'();:@&=+$,/?#[]<>~.\"{}|\\-`_^%", *s) != NULL) {
      res[i++] = '%';
      res[i++] = "0123456789ABCDEF"[*s/16];
      res[i] = "0123456789ABCDEF"[*s%16];
    }
    /* not reserved */
    else
      res[i] = *s;
    s++;
    i++;
  }
  res[i] = '\0';
  return res;
}
