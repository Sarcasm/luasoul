/*
** lua_ui.c for luasoul in /home/papin_g
** 
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
** 
** Started on  Thu Oct  7 19:12:49 2010 Guillaume Papin
** Last update Tue Nov  9 23:46:33 2010 Guillaume Papin
*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
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


#ifdef NCURSES_VERSION
/*_____________________
  BEGIN USER DEFINED KEYS
*/

/* the first value of a user defined key */
#define	KEY_DEF_MIN	(KEY_MAX + 100)

typedef struct
{
  char		*keyname;
  int		keycode;
}		t_bind;

/*
  three cases:
  (1) keycode is `0'
      register a new key with a key sequence and a new name, return NULL
  (2) key_sequence is NULL, new_name also
      get a key name, in this case keyname is NULL and the keyname is returned

  TODO: unbind, cleanup also know as free().
 */
char		*user_keys_hook(int keycode, const char *key_sequence,
				const char *new_name)
{
  static int	size = 0;	/* start with 0 keys */
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
	  binds[size -1].keyname = strdup(new_name);
	}
      return NULL;		/* nothing to return */
    }

  /* (2) get the key name */
  keycode -= KEY_DEF_MIN;
  if (keycode >= 0 && keycode < size)
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
  const char	*key_name = luaL_checkstring(L, 2);

  user_keys_hook(0, key_sequence, key_name);
  return 0;
}
/*_____________________
  END USER DEFINED KEYS
*/
#endif	/* !NCURSES_VERSION */


typedef struct
{
  const int	keycode;
  const char	*keyname;
}		keys_hook_t;

keys_hook_t	key_hook_names[]=
  {
    {KEY_UP,		"<up>"},
    {KEY_DOWN,		"<down>"},
    {KEY_LEFT,		"<left>"},
    {KEY_RIGHT,		"<right>"},
    {KEY_HOME,		"<home>"},
    {KEY_END,		"<end>"},
    {KEY_ENTER,		"RET"},
    {KEY_BACKSPACE,	"<backspace>"},
    {'\b',		"<backspace>"},
    {KEY_NPAGE,		"<PageDown>"},
    {KEY_PPAGE,		"<PageUp>"},
    {KEY_DL,		"<delete-line>"},
    {KEY_IL,		"<insert-line>"},
    {KEY_DC,		"<delete>"},
    {KEY_IC,		"<insert>"},
    {KEY_SDC,		"S-<delete>"},
    {KEY_SDL,		"S-<delete-line>"},
    {KEY_SHOME,		"S-<home>"},
    {KEY_SEND,		"S-<end>"},
    {KEY_SIC,		"S-<insert>"},
    {KEY_SLEFT,		"S-<left>"},
    {KEY_SRIGHT,	"S-<right>"},
    /* Just bind 16 functions keys */
    {KEY_F0,		"<f0>"},
    {KEY_F(1),		"<f1>"},
    {KEY_F(2),		"<f2>"},
    {KEY_F(3),		"<f3>"},
    {KEY_F(4),		"<f4>"},
    {KEY_F(5),		"<f5>"},
    {KEY_F(6),		"<f6>"},
    {KEY_F(7),		"<f7>"},
    {KEY_F(8),		"<f8>"},
    {KEY_F(9),		"<f9>"},
    {KEY_F(10),		"<f10>"},
    {KEY_F(11),		"<f11>"},
    {KEY_F(12),		"<f12>"},
    {KEY_F(13),		"<f13>"},
    {KEY_F(14),		"<f14>"},
    {KEY_F(15),		"<f15>"},
    {KEY_F(16),		"<f16>"},
    {0,			NULL}
  };

/*
  This function is a wrapper to the ncurses keyname()
  Try to convert some KEY_* more readable (or more 'emacs-like' ?).
 */
static const char	*keyname_hook(int ch)
{
  int			i;

  for (i = 0; key_hook_names[i].keyname != NULL; i++)
    if (key_hook_names[i].keycode == ch)
      return key_hook_names[i].keyname;

  /*
    FIXME: this should be avoided in the future
    keyname() can return NULL
   */
  return keyname(ch);
}


/*
  This function is the callback on keyboard input

  Lua config should define the following function:
  - key_received(key_value, key_name)

  FIXME:
  check if the called function exist
 */
int		lui_handle_input(lua_State *L)
{
  int		ch;
  const char	*name;
  #define	KEY_BUFSIZE 42
  char		name_buf[KEY_BUFSIZE];

  /* get the character */
  ch = getch();

  /* normal character */
  if (isprint(ch))
    {
      name_buf[0] = ch;
      name_buf[1] = '\0';
      name = name_buf;
    }
  else
  /* Control keys */
  if (ch >= 0 && ch < 0x20)
    {
      if (ch == 0x1b)
	name = "ESC";
      else if (ch == '\r')
	name = "RET";
      else if (ch == '\t')
	name = "TAB";
      else
	{
	  snprintf(name_buf, KEY_BUFSIZE -1, "C-%c", ch - CTRL('A') + 'a');
	  name = name_buf;
	}
    }
  else
#ifdef NCURSES_VERSION
    /* handle user key binding names */
    name = user_keys_hook(ch, NULL, NULL);
  if (name == NULL)
#endif	/* !NCURSES_VERSION */
    name = keyname_hook(ch);

  /*
    Call a lua function with on the following form:
    key_received(key_value, key_name)
  */
  call_lua_function(L, "key_received", "is", ch, name);

  return 0;
}
