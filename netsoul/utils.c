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


#undef  _XOPEN_SOURCE      /* vsnprintf() need _XOPEN_SOURCE >= 500 */
#define _XOPEN_SOURCE 500  /* for vsnprintf() */
#include <stdio.h>
#include <stdarg.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "md5.h"

/*
  return the hexadecimal md5 sum of the formatted string.
*/
#define MD5SUM_BUFF_LENGHT 200
char            *netsoul_md5sum(const char *fmt, ...)
{
  char          buff[MD5SUM_BUFF_LENGHT];
  va_list       argp;
  static char   md5sum[16 * 2 + 1];
  md5_state_t   state;
  md5_byte_t    digest[16];
  int           i;
  size_t        len;

  va_start(argp, fmt);
  len = vsnprintf(buff, MD5SUM_BUFF_LENGHT -1, fmt, argp);
  va_end(argp);

  md5_init(&state);
  md5_append(&state, (const md5_byte_t *)buff, len);
  md5_finish(&state, digest);
  for (i = 0; i < 16; ++i) {
    sprintf(md5sum + i * 2, "%02x", digest[i]);
  }
  md5sum[16*2] = '\0';

  return md5sum;
}
#undef  MD5SUM_BUFF_LENGHT



static char     netsoul_hexa_value(char c)
{
  return (c >= '0' && c <= '9') ? c - '0' : (c & 0xDF) - 'A' + 10;
}

char            *netsoul_url_decode(char *s)
{
  int            i   = 0;
  char          *res = s;

  while (*s)
    {
      /* `%' take the 2 following numbers and convert them from hexa to dec */
      if (*s == '%')
        {
          /* if the string end with a `%' */
          if (! *(++s))
            break ;
          /* first hexadecimal character here */
          res[i] = netsoul_hexa_value(*(s++)) * 16;
          res[i] += netsoul_hexa_value(*s);
        }
      else
        res[i] = *s;
      i++;
      s++;
    }
  res[i] = '\0';

  return res;
}

/**
 * URL encode a string.
 *
 * @param       dest
 *                      the destination string
 * @param       src
 *                      the string to encode
 * @param       len
 *                      the lenght of the destination string
 *
 * @return dest
 */
char            *netsoul_url_encode(char *dest, const char *src, size_t len)
{
  size_t        i = 0;

  while (*src && i < len)
    {
      /* reserved characters */
      /* a-zA-Z0-9_.\-\\ */
      /* if (strchr(" \n\r\v*'();:@&=+$,/?#[]<>~.\"{}|\\-`_^%", *src) != NULL) */
      if ((*src < 'a' || *src > 'z') &&
          (*src < 'A' || *src > 'Z') &&
          (*src < '0' || *src > '9') &&
          *src != '_' &&
          *src != '-' &&
          *src != '\\')
        {
          if (i + 2 >= len)
            break;
          dest[i++] = '%';
          dest[i++] = "0123456789ABCDEF"[*src / 16];
          dest[i] = "0123456789ABCDEF"[*src % 16];
        }
      /* not reserved */
      else
        dest[i] = *src;
      src++;
      i++;
    }
  dest[i] = '\0';

  return dest;
}
