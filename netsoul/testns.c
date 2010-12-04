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

#define _BSD_SOURCE             /* for getpass(), this file is
                                   just for testing the library
                                   so even is the function is
                                   obselete is use it. */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "netsoul.h"

/*
  gcc -I. -g -o netsoul testns.c -L. -lnetsoul
*/

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

  settings.login      = NULL;   /* $USER should be fine */
  settings.socks_pass = pass;
  settings.userdata   = NULL;
  settings.location   = NULL;
  settings.server     = NULL;
  settings.port       = NULL;

  N = netsoul_create_session(&settings, &err); /* callbacks */

  if (N == NULL)
    fprintf(stderr, "error ! netsoul_create_session(): %s\n", err);
  else
    {
      printf("success !\n");
      if (netsoul_connect(N, &err) == 0)
        {
          printf("connected !\n");
          sleep(3);
        }
      else
        printf("couldn't connect !\n");
      netsoul_destroy_session(N);
      printf("destroyed !\n");
    }
  return 0;
}
