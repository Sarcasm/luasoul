/*
** utils.c for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
** 
** Started on  Sun Sep 19 13:17:56 2010 Guillaume Papin
** Last update Sun Nov  7 18:50:58 2010 Guillaume Papin
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
  FIXME: add ccompilation time flag here for include or not
*/
char		*strndup(const char *s, size_t n)
{
  char		*res;
  size_t	len = strlen(s);

  if (n > len)
    n = len;
  res = malloc((n + 1) * sizeof(*res));
  if (res != NULL)
    {
      res[n] = '\0';
      while (n--)
	res[n] = s[n];
    }
  return res;
}

char		*get_rc(void)
{
  char		rc_path[FILENAME_MAX];
  char		*dir;

  /* file in current directory ? */
  if (! access(RC_NAME, F_OK))
    return strdup(RC_NAME);

  /* file in XDG_CONFIG_HOME ? */
  dir = getenv("XDG_CONFIG_HOME");
  if (dir != NULL) {
    strcpy(rc_path, dir);
    strcat(rc_path, "/luasoul/");
    strcat(rc_path, RC_NAME);
    if (access(rc_path, F_OK) == 0)
      return strdup(rc_path);
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
