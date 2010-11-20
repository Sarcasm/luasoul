/*
** lua_ui.c for luasoul in /home/papin_g
**
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
**
** Started on  Thu Oct  7 19:12:49 2010 Guillaume Papin
** Last update Sat Nov 20 23:54:50 2010 Guillaume Papin
*/

#include <stdlib.h>
#include <string.h>
#include <wctype.h>
#include "luasoul.h"
#include "utils.h"
#include "lua_ui.h"
#include "ui/ui_utils.h"
#include "ui/style.h"
#include "ui/chatbox.h"
#include "ui/window.h"
#include "ui/input.h"


/* list of lua functions for the user interface */
t_lua_function				lua_ui_functions[]=
  {
#ifdef NCURSES_VERSION
    {"define_key",	lui_define_key},
#endif	/* !NCURSES_VERSION */
    {"get_screen_size",	lui_get_screen_size},
    {"clear",		lui_clear},
    {"suspend",		lui_suspend},
    {NULL,		NULL}
  };

void		init_lua_ui(lua_State *L)
{
  register_function(L, (t_lua_function *) (lua_ui_functions));

  /* Export LuaSoul `classes' */
  lui_style_register(L);
  lui_window_register(L);
  lui_chatbox_register(L);
  lui_input_register(L);
}

/**
 * Get the dimension of the terminal.
 * @return width the number of columns
 * @return height the number of lines
 */
int		lui_get_screen_size(lua_State *L)
{
  int		width;
  int		height;

  getmaxyx(stdscr, height, width);

  lua_pushnumber(L, width);
  lua_pushnumber(L, height);

  return 2;
}

/**
 * Clear the screen
 */
int		lui_clear(lua_State *L UNUSED_ATTRIBUTE)
{
  wclear(stdscr);
  wnoutrefresh(stdscr);
  return 0;
}

/**
 * Suspend Luasoul
 */
int			lui_suspend(lua_State *L)
{
  /* SIGSTOP... */
  luasoul_suspend();

  /* ...SIGCONT */
  luasoul_resume();

  /* Handle window resize */
  luasoul_resize(L);

  return 0;
}

#ifdef NCURSES_VERSION
/*_____________________
  BEGIN USER DEFINED KEYS
*/

/* the first value of a user defined key */
#define	KEY_DEF_MIN	(KEY_MAX + 100)

typedef struct
{
  wchar_t	*keyname;
  int		 keycode;
}		 t_bind;

/*
  three cases:
  (1) keycode is `0'
      register a new key with a key sequence and a new name, return NULL
  (2) key_sequence is NULL, new_name too
      get a key name, in this case keyname is NULL and the keyname is returned

  TODO: unbind, cleanup also know as free().
 */
static wchar_t		*user_keys_hook(wint_t		 keycode,
					const char	*key_sequence,
					const wchar_t	*new_name)
{
  static size_t	 size  = 0;
  static t_bind	*binds = NULL;

  /* (1) define a new key */
  if (key_sequence != NULL && new_name != NULL)
    {
      /* create the new key */
      define_key(key_sequence, KEY_DEF_MIN + size++);

      /* update the key list */
      binds = realloc(binds, size * sizeof(*binds));
      if (binds != NULL)
	{
	  binds[size -1].keycode = size;
	  binds[size -1].keyname = luasoul_wcsdup(new_name);
	}
      return NULL;		/* nothing to return */
    }

  /* (2) get the key name */
  keycode -= KEY_DEF_MIN;
  if (keycode < size)
    return binds[keycode].keyname; /* key found */
  return NULL;			   /* not found */
}

/*
  Ncurses specific, allow to define a human readable key sequences.

  Stack:
  1: key sequence
  2: name

  define_key(key_sequence, name)
  for exemple:
  define_key("Oa", "C-<up>")
*/
int		lui_define_key(lua_State *L)
{
  const char	*key_sequence = luaL_checkstring(L, 1);
  const wchar_t	*name	      = check_wcstr(L, 2);

  user_keys_hook(0, key_sequence, name);
  return 0;
}
/*_____________________
  END USER DEFINED KEYS
*/
#endif	/* !NCURSES_VERSION */


typedef struct
{
  const wint_t	keycode;
  const wchar_t	*keyname;
}		keys_hook_t;

keys_hook_t	key_hook_names[]=
  {
    {KEY_UP,		L"<up>"},
    {KEY_DOWN,		L"<down>"},
    {KEY_LEFT,		L"<left>"},
    {KEY_RIGHT,		L"<right>"},
    {KEY_HOME,		L"<home>"},
    {KEY_END,		L"<end>"},
    {KEY_ENTER,		L"RET"},
    {KEY_BACKSPACE,	L"<backspace>"},
    {KEY_NPAGE,		L"<PageDown>"},
    {KEY_PPAGE,		L"<PageUp>"},
    {KEY_DL,		L"<delete-line>"},
    {KEY_IL,		L"<insert-line>"},
    {KEY_DC,		L"<delete>"},
    {KEY_IC,		L"<insert>"},
    {KEY_EOL,		L"<clearline>"},
    {KEY_SDC,		L"S-<delete>"},
    {KEY_SDL,		L"S-<delete-line>"},
    {KEY_SHOME,		L"S-<home>"},
    {KEY_SEND,		L"S-<end>"},
    {KEY_SIC,		L"S-<insert>"},
    {KEY_SLEFT,		L"S-<left>"},
    {KEY_SRIGHT,	L"S-<right>"},
    /* Bind 'only' 32 functions keys */
    {0,			NULL}
  };


#define AddToBuff(s)	wcsncat(buff, s, len)
/*
  Control keys
*/
static void	append_cntrl(wint_t ch, wchar_t *buff, size_t len)
{
  if (ch == 0x1b)
    AddToBuff(L"ESC");
  else if (ch == '\r')
    AddToBuff(L"RET");
  else if (ch == '\t')
    AddToBuff(L"TAB");
  else if (ch == '\b')
    AddToBuff(L"<backspace>");
  else
    {
      wchar_t tmp[2];

      tmp[0] = ch - CTRL('A') + 'a';
      tmp[1] = 0;
      AddToBuff(L"C-");
      AddToBuff(tmp);
    }
}

/*
  Convert key value to key name
 */
static void	append_keyname(wint_t ch, wchar_t *buff, size_t len)
{
  size_t	i;

  /* Function keys, like arrow keys, delete, ... */
  for (i = 0; key_hook_names[i].keyname != NULL; i++)
    if (key_hook_names[i].keycode == ch)
      {
	AddToBuff(key_hook_names[i].keyname);
	return ;
      }

#ifdef NCURSES_VERSION
  /* handle user defined keys names */
  {
    const wchar_t	*wname;

    wname = user_keys_hook(ch, NULL, NULL);
    if (wname != NULL)
      {
	AddToBuff(wname);
	return ;
      }
  }
#endif	/* !NCURSES_VERSION */

  /*
     Function keys (f1, f2, ...)
     man getch()
     ...
     KEY_F(n)        For 0 ≤ n ≤ 63
     ...
  */
  if(ch >= KEY_F(0) && ch <= KEY_F(63))
    {
      /* max width is 2 characters */
      wchar_t	num[3];

      AddToBuff(L"<f");

      /* fkey numbers to *wchar_t */
      num[0] = '0' + (ch - KEY_F(0)) / 10;
      i = num[0] != '0';
      num[i] = '0' + (ch - KEY_F(0)) % 10;
      num[++i] = L'\0';
      AddToBuff(num);

      AddToBuff(L">");
      return ;
    }

  /* keep ncurses handle unknow characters */
  {
    const char	*sname = key_name(ch);

    if (sname != NULL)
      {
	i = wcslen(buff);
	while (*sname != '\0' && i < len)
	  buff[i++] = *(sname++);
	buff[i] = L'\0';
      }
    /* TODO: call lua error function here, send keycode */
  }
}
#undef AddToBuff

/*
  This function is the callback on keyboard input

  Lua config should define the following function:
  - key_received(key_value, key_name, printable)

  bool printable:
  - true if the character is printable
  - false if the character is not printable (arrow keys, control keys, ...)

  FIXME:
  check if the called function exist
 */
int		lui_handle_input(lua_State *L)
{
  #define	KEY_BUFSIZE	42 /* maximum lenght for one keyname */
  wchar_t	name_buf[KEY_BUFSIZE] = {0};
  wint_t	ch;
  int		ret;
  int		func_key = 0;

  /* get the character */
  ret = get_wch(&ch);

  /* handle Alt key */
  func_key = (ret == OK && ch == 0x1b);
  if (func_key)
    {
      nodelay(stdscr, TRUE);
      ret = get_wch(&ch);
      nodelay(stdscr, FALSE);
      if (ret != ERR)
	wcsncpy(name_buf, L"M-", KEY_BUFSIZE);
      else			/* just was send ESC */
	{
	  ch = 0x1b;
	  ret = OK;
	}
    }
  func_key += (ret == KEY_CODE_YES || ch < 20);

  if (ret == KEY_CODE_YES)
    append_keyname(ch, name_buf, KEY_BUFSIZE -1);
  else if (ret == OK)
    {
      if (ch < 0x20)		/* control characters */
	append_cntrl(ch, name_buf, KEY_BUFSIZE -1);
      else			/* a character */
	{
	  wchar_t	wc2wstr[2];

	  wc2wstr[0] = ch;
	  wc2wstr[1] = L'\0';
	  wcsncat(name_buf, wc2wstr, KEY_BUFSIZE -1);
	}
    }
  /* TODO: ret == ERR, call error function */

  /* convert wstring to string and call lua function */
  {
    char	lua_name[MB_LEN_MAX * KEY_BUFSIZE];

    if (wcstombs(lua_name, name_buf, MB_LEN_MAX * KEY_BUFSIZE) != (size_t) -1)
      call_lua_function(L, "key_received", "isb", ch, lua_name, !func_key);
    /* else
       TODO: ret == ERR, call error function */
  }

  return 0;
}
