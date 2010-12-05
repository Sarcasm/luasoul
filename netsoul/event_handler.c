/*
 * event_handler.c for luasoul
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

/* "name"    ->    "name", strlen("name") */
#define REG_MEMBER(name) name, (sizeof(name) -1)


static int      netsoul_handle_ping(netsoulSession *N,
                                    void           *data,
                                    char           *msg)
{
  UNUSED(data);

  netsoul_send(N, "%s", msg);
  return 0;
}

static int      netsoul_handle_msg(netsoulSession *N,
                                   void           *data,
                                   char           *login,
                                   char           *msg)
{
  char          *tmp;

  if (N->callbacks.new_msg == NULL)
    return 0;

  /* find end of msg */
  tmp = strchr(msg, ' ');
  if (tmp != NULL)
    *tmp = 0; /* don't care dst={...} */

  return N->callbacks.new_msg(N, data, login, netsoul_url_decode(msg));
}

static int      netsoul_handle_typing_start(netsoulSession *N,
                                            void           *data,
                                            char           *login,
                                            char           *msg)
{
  UNUSED(msg);

  return (N->callbacks.typing_start == NULL) ? 0
    : N->callbacks.typing_start(N, data, login);
}

static int      netsoul_handle_typing_end(netsoulSession *N,
                                          void           *data,
                                          char           *login,
                                          char           *msg)
{
  UNUSED(msg);

  return (N->callbacks.typing_end == NULL) ? 0
    : N->callbacks.typing_end(N, data, login);
}

static int      netsoul_handle_login(netsoulSession *N,
                                     void           *data,
                                     char           *login,
                                     char           *msg)
{
  UNUSED(msg);

  return (N->callbacks.login == NULL) ? 0
    : N->callbacks.login(N, data, login);
}

static int      netsoul_handle_logout(netsoulSession *N,
                                      void           *data,
                                      char           *login,
                                      char           *msg)
{
  UNUSED(msg);

  return (N->callbacks.logout == NULL) ? 0
    : N->callbacks.logout(N, data, login);
}

static int      netsoul_handle_state(netsoulSession *N,
                                     void           *data,
                                     char           *login,
                                     char           *msg)
{
  char          status[NETSOUL_DATA_SIZE + 1];
  int           i = 0;

  while (i < NETSOUL_DATA_SIZE
         && strchr(STATUS_CHARACTERS, *msg) != NULL)
    status[i++] = *(msg++);
  status[i] = '\0';

  return (N->callbacks.status_changed == NULL) ? 0
    : N->callbacks.status_changed(N, data, login, status);
}

typedef struct
{
  int           (*func)(netsoulSession *N, void *data, char *login, char *msg);
  const char    *cmd;
  size_t        cmd_len;
}               UserCmdHandlers;

/* list of user_cmd, after the ' | ' */
static UserCmdHandlers            user_cmd_handlers[]=
  {
    {netsoul_handle_msg,          REG_MEMBER("msg ")},
    {netsoul_handle_typing_start, REG_MEMBER("dotnetSoul_UserTyping ")},
    {netsoul_handle_typing_start, REG_MEMBER("typing_start ")},
    {netsoul_handle_typing_end,   REG_MEMBER("dotnetSoul_UserCancelledTyping ")},
    {netsoul_handle_typing_end,   REG_MEMBER("typing_end ")},
    {netsoul_handle_login,        REG_MEMBER("login ")},
    {netsoul_handle_logout,       REG_MEMBER("logout ")},
    {netsoul_handle_state,        REG_MEMBER("state ")},
    {NULL,                        NULL, 0}
  };

/* user_cmd <socket>:user:<trust level>:<login>@<user host>: ... */
static char     *netsoul_get_login(char login[], const char *buff, size_t n)
{
  int           i = 0;

  while (*buff)
    if (*(buff++) == ':')
      if (++i == 3)
        {
          size_t  len = strspn(buff, LOGIN_CHARACTERS) + 1;

          if (len > n)
            len = n;
          strncpy(login, buff, len);

          if (len > 0)
            login[len - 1] = '\0';

          return login;
        }

  return NULL;
}

/* user_cmd ... | <command> <command extension> */
static char     *netsoul_get_usercmd(char usercmd[], const char *buff, size_t n)
{
  char          *begin = strstr(buff, "| ");

  if (begin != NULL)
    {
      /* +2 for '| ' */
      strncpy(usercmd, begin + 2, n);
      if (n > 0)
        usercmd[n - 1] = '\0';
      return usercmd;
    }

  return NULL;
}

/*
  user_cmd <socket>:user:<trust level>:<login>@<user host>:\
  <workstation type>:<location>:<groupe> | <command> <command extension>
  FIXME: style
*/
static int      netsoul_handle_user_cmd(netsoulSession *N,
                                        void           *data,
                                        char           *msg)
{
  char          login[NETSOUL_LOGIN_SIZE];
  char          usercmd[NETSOUL_MSG_SIZE];

  if (netsoul_get_login(login, msg, NETSOUL_LOGIN_SIZE) != NULL &&
      netsoul_get_usercmd(usercmd, msg, NETSOUL_MSG_SIZE) != NULL)
    {
      int           i;

      for (i = 0; user_cmd_handlers[i].cmd != NULL; i++)
        if (strncmp(usercmd, user_cmd_handlers[i].cmd,
                    user_cmd_handlers[i].cmd_len) == 0)
          return
            user_cmd_handlers[i].func(N, data, login,
                                      /* start with the argument after
                                         usercmd */
                                      usercmd + user_cmd_handlers[i].cmd_len);
    }

  /* shouldn't happen */
  return (N->callbacks.unknow_event == NULL) ? 0
    : N->callbacks.unknow_event(N, data, msg);
}

typedef struct
{
  int           (*func)(netsoulSession *N, void *data, char *msg);
  const char    *cmd;
  size_t        cmd_len;
}               Handlers;

static Handlers                 handlers[]=
  {
    {netsoul_handle_user_cmd,   REG_MEMBER("user_cmd ")},
    {netsoul_handle_ping,       REG_MEMBER("ping ")},
    {NULL,                      NULL, 0}
  };

/**
 * Read a message from the socket and execute the corresponding
 * callback.
 *
 * @param       N
 *                      the Netsoul session
 * @param       data
 *                      a user pointer
 *
 * @return the execution of the correct callback, 0 on succes, -1 on error.
 */
int             netsoul_event_handler(netsoulSession *N, void *data)
{
  char         *msg = netsoul_recv(N);
  int           i;

  for (i = 0; handlers[i].cmd != NULL; i++)
    if (strncmp(msg, handlers[i].cmd, handlers[i].cmd_len) == 0)
      return handlers[i].func(N, data, msg);

  /* unknow message */
  if (N->callbacks.unknow_event != NULL)
    return N->callbacks.unknow_event(N, data, msg);

  return 0;                     /* no callback -> success */
}
