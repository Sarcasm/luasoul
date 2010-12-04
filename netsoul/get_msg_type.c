/*
 * get_msg_type.c for luasoul
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

#include <stddef.h>
#include <string.h>
#include "netsoul.priv.h"

typedef struct
{
  const char            *str;   /* string to find */
  enum NETSOUL_MSG_TYPE  type;
}               Msg_types;

static Msg_types msgs[]=
  {
    {"rep 002 -- cmd end\n",    REP_OK},
    {NULL,                      UNKNOW_TYPE},
  };

/**
 * return the type of the given message.
 *
 * @param       msg
 *                      message to be analyzed to get its type
 */
enum NETSOUL_MSG_TYPE   netsoul_get_msg_type(const char *msg)
{
  int           i;

  for (i = 0; msgs[i].str != NULL; i++)
    if (strstr(msg, msgs[i].str) != NULL)
      return msgs[i].type;
  return UNKNOW_TYPE;
}
