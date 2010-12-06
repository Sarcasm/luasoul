/*
 * testns.c for luasoul
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

/*
  Compile command:
  gcc -I. -g -o netsoul testns.c -L. -lnetsoul
*/

#define _BSD_SOURCE             /* for getpass(), this file is
                                   just for testing the library
                                   so even is the function is
                                   obselete is use it.
                                   snprintf() also need this.
                                */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sys/select.h>

#include "netsoul.h"


/* Thx: http://linuxgazette.net/issue65/padala.html */
#define error(msg)   fprintf(stderr, "[0;31m%s[0m\n", msg)
#define info(msg)    printf("[0;33m%s[0m\n", msg)
#define msg_in(msg)  printf("[0;32m<< %s[0m\n", msg)
#define msg_out(msg) printf("[0;34m>> %s[0m\n", msg)

#define UNUSED(x)       (void) x

int             ns_new_msg(netsoulSession *N, void *data,
                           const char *login,
                           const char *msg)
{
  char          buff[1024];

  UNUSED(N);
  UNUSED(data);

  snprintf(buff, 1023, "%s say: '%s'", login, msg);
  msg_in(buff);

  return 0;
}

int             ns_typing_start(netsoulSession *N, void *data, const char *login)
{
  UNUSED(N);
  UNUSED(data);

  printf("[0;32m%s is typing...[0m\n", login);

  return 0;
}

int             ns_typing_end(netsoulSession *N, void *data, const char *login)
{
  UNUSED(N);
  UNUSED(data);

  printf("[0;32m%s typing finish...[0m\n", login);

  return 0;
}

int             ns_login(netsoulSession *N, void *data, const char *login)
{
  UNUSED(N);
  UNUSED(data);

  printf("[0;32m%s login...[0m\n", login);

  return 0;
}

int             ns_logout(netsoulSession *N, void *data, const char *login)
{
  UNUSED(N);
  UNUSED(data);

  printf("[0;32m%s logout...[0m\n", login);

  return 0;
}

int             ns_status_changed(netsoulSession *N, void *data,
                                  const char *login, const char *status)
{
  UNUSED(N);
  UNUSED(data);

  printf("[0;32m%s: status -> %s[0m\n", login, status);
  return 0;
}

int             ns_unknow(netsoulSession *N, void *data, const char *msg)
{
  UNUSED(N);
  UNUSED(data);

  msg_in(msg);

  return 0;
}

char                    *get_line(void)
{
  static char           buff[1024];
  ssize_t               nb_read;

  nb_read = read(STDIN_FILENO, buff, 1023);
  if (nb_read < 0)
    return NULL;
  else if (nb_read == 0)
    {
      *buff = 0;
      return buff;
    }

  if (buff[nb_read - 1] == '\n') /* remove trailing LF */
    nb_read--;
  buff[nb_read] = '\0';

  return buff;
}

void                    lOOoop(netsoulSession *N)
{
  fd_set                readfds;
  int                   nfds;
  int                   nb_fds;
  int                   sockfd;

  netsoul_set_nonblocking(N);
  info("non blocking !");


  while (1)
    {
      FD_ZERO(&readfds);
      FD_SET(STDIN_FILENO, &readfds);

      sockfd = netsoul_get_fd(N);
      if (sockfd != -1 && sockfd > STDIN_FILENO)
        {
          nfds =  sockfd + 1;          
          FD_SET(sockfd, &readfds);
        }
      else
        nfds =  STDIN_FILENO + 1;

      /*
        select returns 0 if timeout, 1 if input available, -1 if error.
        Set timeout to NULL for blocking operation
      */
      nb_fds = select(nfds, &readfds, NULL, NULL, NULL);

      /* handle lost connection here */
      if (nb_fds <= 0)
        {
          break ;
          error("Select return an error.");
        }

      /*
        Event from keyboard ?
      */
      if (FD_ISSET(STDIN_FILENO, &readfds))
        {
          const char *line = get_line();

          if (line == NULL)
            error("read() fail");
          else
            {
              msg_out(line);
              if (strcmp(line, "exit") == 0)
                break ;
              if (*line == '>')
                netsoul_send_msg(N, "papin_g", line + 1);
              if (*line == '|')
                netsoul_send_typing_start(N, "papin_g");
              if (*line == '\\')
                netsoul_send_typing_end(N, "papin_g");
            }
        }

      if (sockfd != -1 && FD_ISSET(sockfd, &readfds))
        {
          /* Send the netsoul state, and a void * pointer */
          if (netsoul_event_handler(N, NULL) != 0)
            error("Callback echec.");
        }
    }
}

int             main(void)
{
  char           *pass;
  /* char            login[12]; */
  netsoulConfig   settings;
  netsoulSession *N;
  const char     *err;

  /* puts("login plz: "); */
  /* if (fgets(login, 11, stdin) == NULL) */
  /*   return EXIT_FAILURE; */

  /* remove line feed */
  /* if ((pass = strchr(login, '\n')) != NULL) */
  /*   *pass = '\0'; */

  pass = getpass("password plz: ");
  if (pass == NULL)
    return EXIT_FAILURE;

  /* printf("login: %s\n",         login); */
  printf("password: %s\n",      pass);

  /* set callbacks */
  memset(&settings, 0, sizeof(settings));

  settings.login                    = NULL; /* $USER should be fine */
  settings.socks_pass               = pass;
  settings.userdata                 = (char *) "libnetsoul test";
  settings.location                 = (char *) "pas tres loin";
  /* callbacks */
  settings.callbacks.unknow_event   = ns_unknow;
  settings.callbacks.new_msg        = ns_new_msg;
  settings.callbacks.typing_start   = ns_typing_start;
  settings.callbacks.typing_end     = ns_typing_end;
  settings.callbacks.login          = ns_login;
  settings.callbacks.logout         = ns_logout;
  settings.callbacks.status_changed = ns_status_changed;


  /* Create the netsoul session */
  N = netsoul_create_session(&settings, &err);
  if (N == NULL)
    {
      fprintf(stderr, "error ! netsoul_create_session(): %s\n", err);
      return 1;
    }
  
  info("netsoul_create_session() success !");

  if (netsoul_connect(N, &err) == 0)
    {
      const char *buddies[] = {"papin_g", NULL};

      info("connected !");
      
      netsoul_spy_users(N, buddies);
      info("watch list added !");

      lOOoop(N);
    }
  else
    error(err);

  netsoul_destroy_session(N);
  info("destroyed !");

  return 0;
}
