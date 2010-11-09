/*
** luasoul.c for luasoul in /home/papin_g
**
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
**
** Started on  Wed Oct  6 00:43:31 2010 Guillaume Papin
** Last update Tue Nov  9 23:48:19 2010 Guillaume Papin
*/

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "lua/lua_utils.h"
#include "ui/ui_utils.h"

/* loop stuff, move that one day */
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/select.h>
#include <unistd.h>
#include "lua_ui.h"


/* ugly...for signal handling */
int			globalFlag = 0;

/*
  this function just toggle a the global int flag
*/
void			sigwinch_handler(int sig)
{
  globalFlag = sig;
}

/*
  FIXME:
  The Loop is ugly for the moment, it's just a draft
*/
void			lOOoop(lua_State *L)
{
  fd_set		activefds;
  fd_set		readfds;
  int			nfds;
  int			nb_fds;
  /* SIGWINCH handler */
  struct sigaction	sa;
  struct winsize	size;

  FD_ZERO(&activefds);
  FD_SET(STDIN_FILENO, &activefds);

  nfds = STDIN_FILENO + 1;

  /*
    set the SIGWINCH handler
    FIXME: SA_SIGINFO flag is correct ?
  */
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = sigwinch_handler;
  sa.sa_flags = SA_SIGINFO;
  if (sigaction(SIGWINCH, &sa, NULL) == -1)
    {
      /* FIXME: find a good handling of error */
      perror("sigaction");
      return ;
    }

  while (1)
    {
      /* FIXME: is it really correct to do that ? */
      readfds = activefds;

      /*
	 select returns 0 if timeout, 1 if input available, -1 if error.
	 Set timeout to NULL for blocking operation
      */
      nb_fds = select(nfds, &readfds, NULL, NULL, NULL);
	/* handle lost connection and SIGWINCH */
      if (nb_fds <= 0) {

	if (globalFlag == SIGWINCH)
	  {
#include <curses.h>
	    /*
	      some tips found in the ncurses distribution:
	      ncurses-5.7/test/view.c#adjust(int sig)
	    */

	    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &size) == 0)
	      {
	    	resize_term(size.ws_row, size.ws_col);
	    	wrefresh(curscr);	/* Linux needs this */
	      }

	    /* Alternative */
	    /* endwin(); */
	    /* refresh(); */

	    /* clearok(curscr, TRUE);	/\* screen contents are unknown *\/ */
	    call_lua_function(L, "window_resize", "");
	    globalFlag = 0;
	  }
      	/*
      	  temporary just for supporting Signals (SIGWINCH, SIGSUSPEND, SIGCONT...)
      	  here check for lost connection for example...
      	*/
      	continue ;
      }

      /*
      	Event from keyboard ?
      	call lua function on root input handler
      */
      if (FD_ISSET(STDIN_FILENO, &readfds))
      	lui_handle_input(L);
    }
}

int		main(void)
{
  lua_State	*L;

  /* init lua */
  L = load_lua();
  if (L == NULL)
    return 1;

  /* initialize ncurses */
  init_ui();

  /* load rc.lua */
  if (load_config(L) == -1)
    return 1;

  /* self explanatory */
  lOOoop(L);

  /* cleanup */
  lua_close(L);
  ui_close();

  /* bye :) */
  return 0;
}
