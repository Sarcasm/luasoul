/*
** luasoul.c for luasoul in /home/papin_g
**
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
**
** Started on  Wed Oct  6 00:43:31 2010 Guillaume Papin
** Last update Wed Nov 24 21:55:27 2010 Guillaume Papin
*/

#define	_POSIX_SOURCE		/* for kill() */
#include <sys/types.h>
#include <signal.h>

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "ui/ui_utils.h"
#include "lua/lua_utils.h"

#include <sys/select.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "lua_ui.h"

/* not really beautiful...for signal handling */
int			globalFlag = 0;

/*
  this function just toggle a the global int flag
*/
void			luasoul_sighandler(int sig)
{
  globalFlag = sig;
}

void			luasoul_suspend(void)
{
  def_prog_mode();		/* Save the tty modes		  */
  endwin();			/* End curses mode temporarily	  */
  kill(getpid(), SIGTSTP);	/* Stop Luasoul */
}


/*
  SIGCONT handling taken from ncurses distribution:
  ncurses-5.7/ncurses/tty/lib_tstp.c
*/
void			luasoul_resume(void)
{
  flushinp();
  def_shell_mode();
  doupdate();
}

/*
  Some tips found in the ncurses distribution:
  ncurses-5.7/test/view.c#adjust(int sig)
*/
void			luasoul_resize(lua_State *L)
{
  int			row;
  int			col;
  struct winsize	size;

  getmaxyx(stdscr, row, col);

  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &size) == 0
      && (size.ws_row != row || size.ws_col != col))
    {
      resize_term(size.ws_row, size.ws_col);
      wrefresh(curscr);		/* Linux needs this */
      call_lua_function(L, "window_resize", "");
      doupdate();
    }
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

  FD_ZERO(&activefds);
  FD_SET(STDIN_FILENO, &activefds);

  nfds = STDIN_FILENO + 1;

  /*
    set the SIGWINCH handler
    FIXME: SA_SIGINFO flag is correct ?
  */
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO;
  sa.sa_handler = luasoul_sighandler;
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
      if (nb_fds <= 0)
	{
	  if (globalFlag == SIGWINCH)
	    {
	      luasoul_resize(L);
	      globalFlag = 0;
	    }
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

  setlocale(LC_ALL, "");

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
