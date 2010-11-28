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

void            error(const char *fmt, ...)
{
  va_list       argp;

  va_start(argp, fmt);
  fprintf(stderr, "error: ");
  vfprintf(stderr, fmt, argp);
  va_end(argp);
}

/*
  Duplicate a string.
*/
char            *luasoul_strdup(const char *s)
{
  char          *res;

  res = malloc((strlen(s) + 1) * sizeof(*res));
  return res != NULL ? strcpy(res, s) : NULL;
}

/*
  Duplicate at most n characters of the string s.
*/
char            *luasoul_strndup(const char *s, size_t n)
{
  char          *res;
  size_t         len = strlen(s);

  if (n > len)
    n = len;
  res = malloc((n + 1) * sizeof(*res));
  return res != NULL ? strncpy(res, s, n) : NULL;
}

/*
  Duplicate a wide-character string.
*/
wchar_t         *luasoul_wcsdup(const wchar_t *s)
{
  wchar_t       *res;

  res = malloc((wcslen(s) + 1) * sizeof(*res));
  return res != NULL ? wcscpy(res, s) : NULL;
}

/*
  Duplicate at most n characters of the wide-character string s.
*/
wchar_t         *luasoul_wcsndup(const wchar_t *s, size_t n)
{
  wchar_t       *res;
  size_t        len = wcslen(s);

  if (n > len)
    n = len;
  res = malloc((n + 1) * sizeof(*res));
  return res != NULL ? wcsncpy(res, s, n) : NULL;
}

char            *get_rc(void)
{
  char          rc_path[FILENAME_MAX];
  char          *dir;

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
