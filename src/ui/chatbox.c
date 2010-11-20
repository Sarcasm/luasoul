/*
** chatbox.c for luasoul in /home/papin_g
**
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
**
** Started on  Mon Nov  1 09:48:42 2010 Guillaume Papin
** Last update Sat Nov 20 18:08:30 2010 Guillaume Papin
*/

/*
  `Object model' inspired by:
  http://lua-users.org/wiki/UserDataWithPointerExample
  http://lua-users.org/wiki/ObjectProperties
*/

#include <stdlib.h>
#include <string.h>
#include "lua/lua_utils.h"
#include "ui/ui_utils.h"
#include "ui/chatbox.h"
#include "ui/style.h"

typedef struct
{
  WINDOW	*pad;
  int		height;
  int		width;
  int		begin_y;
  int		begin_x;
  int		histsize;	/* number of off screen lines */
  int		scroll;
}		CHATBOX;

/* not in the header because need curses.h */
static CHATBOX	*check_chatbox(lua_State *L, int n);
static int	refresh_chatbox(CHATBOX *c);

/* Constructor */

static const luaL_reg	lui_chatbox_class_methods[]=
  {
    {"new",		lui_new_chatbox},
    {NULL,		NULL}
  };

static CHATBOX	default_chatbox =
  {
    NULL,			/* pad        */
    0,				/* height     */
    0,				/* width      */
    0,				/* begin_y    */
    0,				/* begin_x    */
    0,				/* histsize   */
    0,				/* scroll     */
  };

/*
  Create a new chatbox
  Chatbox.new
  (
  -- required
  int		width,
  int		height,
  int		begin_x,
  int		begin_y,

  -- not required
  int		histsize
  )

  FIXME: error handling, negative values
*/
int		lui_new_chatbox(lua_State *L)
{
  CHATBOX	*c;

  c		= malloc(sizeof(*c)); /* FIXME: if NULL */
  *c		= default_chatbox;
  c->width	= luaL_checkint(L, 1);
  c->height	= luaL_checkint(L, 2);
  c->begin_x	= luaL_checkint(L, 3);
  c->begin_y	= luaL_checkint(L, 4);
  c->histsize	= luaL_optint(L, 5, 0); /* default histsize is 0 */

  c->pad = newpad(c->height + c->histsize, c->width);
  scrollok(c->pad, TRUE);

  /* skip beginning of history */
  wmove(c->pad, c->histsize, 0);

  /* don't refresh, in case set_style is call for example */
  /* refresh_chatbox(c); */

  *((CHATBOX **) lua_newuserdata(L, sizeof(c))) = c;

  /* set instance metatable to registered methods */
  luaL_getmetatable(L, CHATBOX_INST);
  lua_setmetatable(L, -2);


  return 1;
}


/* Getters */

/* this structure map members to getters() */
static const t_index_wrap	lui_chatbox_get_methods[]=
  {
    {NULL,			NULL, 0}
  };


/*
  This function handle the `__index' field.

  Stack:
  1: the instance table
  2: the accessed key
*/
int		lui_chatbox_index(lua_State *L)
{
  const char	*key = luaL_checkstring(L, 2);

  lua_getmetatable(L, 1);
  lua_getfield(L, -1, key);

  /* Either key is name of a method in the metatable */
  if (!lua_isnil(L, -1))
    return 1;

  /* ... or its a field access, so recall as self.get(self, value). */
  lua_settop(L, 2);   /* restore the initial call state, arg1, arg2 */

  return lua_oo_accessors(L, lui_chatbox_get_methods);
}

/* Setters */

/* this structure map members to setters() */
static const t_index_wrap	lui_chatbox_set_methods[]=
  {
    {lui_chatbox_set_style,	REG_MEMBER("style")},
    {NULL,			NULL, 0}
  };

/*
  chatbox.key = true/false
  or
  chatbox["key"] = true/false

  Stack:
  1: the instance table
  2: the accessed key
  3: the value to set
*/
int		lui_chatbox_newindex(lua_State *L)
{
  return lua_oo_accessors(L, lui_chatbox_set_methods);
}

/*
  chatbox:set_win_style{
  bold = true,
  foreground = 3,
  ...
  }

  set the global style of the chatbox, take effect immediatly for each row
  of the chatbox

  Stack:
  2nd argument is a accessed member (style)
  3rd argument is a table who can contain all the attributes above
*/
int		lui_chatbox_set_style(lua_State *L)
{
  CHATBOX	*c = check_chatbox(L, 1);
  t_style	s;

  get_style(L, 3, s);
  wbkgd(c->pad, s.on & ~s.off);
  return 0;
}


/* Utils */

static const luaL_reg lui_chatbox_instance_methods[]=
  {
    {"refresh",		lui_refresh_chatbox},
    {"addch",		lui_addch_chatbox},
    {"addstr",		lui_addstr_chatbox},
    {"print_colored",	lui_print_colored_chatbox},
    {"resize",		lui_resize_chatbox},
    {"move",		lui_move_chatbox},
    {"scroll",		lui_scroll_chatbox},
    {"__index",		lui_chatbox_index},
    {"__newindex",	lui_chatbox_newindex},
    {"__gc",		lui_destroy_chatbox},
    {"__tostring",	lui_chatbox_tostring},
    {NULL, NULL}
  };

/*
  Map all the fields of the class/object.
*/
int		lui_chatbox_register(lua_State *L)
{
  luaL_register(L, CHATBOX_CLASS, lui_chatbox_class_methods);
  luaL_newmetatable(L, CHATBOX_INST);
  luaL_register(L, NULL, lui_chatbox_instance_methods);

  /* __METATABLE */
  /*
    __metatable is for protecting metatables. If you do not want a program
    to change the contents of a metatable, you set its __metatable field.
    With that, the program cannot access the metatable (and therefore cannot
    change it).
  */
  lua_pushliteral(L, "__metatable");
  lua_pushvalue(L, -2);		/* dup methods table*/
  lua_rawset(L, -3);		/* metatable.__metatable = metatable */


  /* __INDEX */
  /*
    When it is a function, Lua calls it with the table and the absent
    key as its arguments.
    When it is a table, Lua redoes the access in that table.
  */

  lua_pop(L, 1);		/* drop metatable */
  return 0;
}

/*
  Check if value at index `n' in the stack is a Chatbox instance,
  return it or raised an error.
*/
static CHATBOX	*check_chatbox(lua_State *L, int n)
{
  CHATBOX	**c;

  luaL_checktype(L, n, LUA_TUSERDATA);
  c = (CHATBOX **) luaL_checkudata(L, n, CHATBOX_INST);
  if (c == NULL || *c == NULL)
    luaL_typerror(L, n, CHATBOX_INST);
  return *c;
}

static int	refresh_chatbox(CHATBOX *c)
{
  return prefresh(c->pad,		     /* pad		*/
  		  c->histsize - c->scroll,   /* pad line	*/
  		  0,			     /* pad column	*/
  		  c->begin_y,		     /* screen line	*/
  		  c->begin_x,		     /* screen column	*/
  		  c->begin_y + c->height -1, /* screen max line	*/
  		  c->begin_x + c->width -1); /* screen max column */
}



/* Methods  */

/*
  chatbox:refresh()
  refresh the chatbox (update physical chatbox to match virtual chatbox).
*/
int		lui_refresh_chatbox(lua_State *L)
{
  CHATBOX	*c = check_chatbox(L, 1);

  refresh_chatbox(c);
  return 0;
}


/*
  chatbox:print_colored(s, {
  bold = true,
  foreground = 3,
  ...
  })

  Stack:
  2nd the string to display
  3rd argument is a table who can contain all the attributes above

  TODO: Optimisation ?
*/
int		lui_print_colored_chatbox(lua_State *L)
{
  CHATBOX	*c = check_chatbox(L, 1);
  const char	*s = luaL_checkstring(L, 2); /* string to display */
  attr_t	attr;
  short		pair;
  t_style	style;

  /* save current state */
  wattr_get(c->pad, &attr, &pair, NULL);

  get_style(L, 3, style);
  wattron(c->pad, style.on);
  wattroff(c->pad, style.off);

  /* put the string */
  c->scroll = 0;
  waddstr(c->pad, s);

  /* restore old attr */
  wattr_set(c->pad, attr, pair, NULL);

  return 0;
}

/*
  chatbox:addch(s)
  put a char in the virtual chatbox
*/
int		lui_addch_chatbox(lua_State *L)
{
  CHATBOX	*c = check_chatbox(L, 1);
  const char	*str = luaL_checkstring(L, 2);

  if (*str)
    {
      c->scroll = 0;
      waddch(c->pad, *str);
    }
  return 0;
}

/*
  chatbox:addstr(s)
  put a string in the virtual chatbox
*/
int		lui_addstr_chatbox(lua_State *L)
{
  CHATBOX	*c = check_chatbox(L, 1);
  const char	*str = luaL_checkstring(L, 2);

  c->scroll = 0;
  waddstr(c->pad, str);
  return 0;
}

/*
  chatbox:resize(width, height)
  resize the chatbox
*/
int		lui_resize_chatbox(lua_State *L)
{
  CHATBOX	*c = check_chatbox(L, 1);
  const int	width = luaL_checkint(L, 2);
  const int	height = luaL_checkint(L, 3);

  if (width <= 0 && height <= 0)
    {
      luaL_error(L, "can't resize chatbox: invalid dimension");
      return 0;
    }
  if (wresize(c->pad, height + c->histsize, width) == OK)
    {
      c->height = height;
      c->width = width;
    }
  return 0;
}

/*
  chatbox:move(x, y)
  move the chatbox
*/
int		lui_move_chatbox(lua_State *L)
{
  CHATBOX	*c = check_chatbox(L, 1);
  const int	x = luaL_checkint(L, 2);
  const int	y = luaL_checkint(L, 3);

  if (x < 0 && y < 0)
    {
      luaL_error(L, "can't move chatbox: invalid position");
      return 0;
    }
  else
    {
      c->begin_x = x;
      c->begin_y = y;
    }
  return 0;
}

/*
  chatbox:scroll(nlines)
  if nlines > 0 scroll up nlines
  otherwise scroll down nlines
*/
int		lui_scroll_chatbox(lua_State *L)
{
  CHATBOX	*c = check_chatbox(L, 1);
  const int	nlines = luaL_checkinteger(L, 2);

  c->scroll += nlines;
  c->scroll = c->scroll < 0 ? 0 : c->scroll > c->histsize ? c->histsize : c->scroll;
  refresh_chatbox(c);
  return 0;
}

/*
  tostring(chatbox), chatbox.__tostring
  just print the type and pointer address of the Chatbox
*/
int		lui_chatbox_tostring(lua_State *L)
{
  lua_pushfstring(L, "Chatbox: %p", lua_touserdata(L, 1));
  return 1;
}

/* Destructor */

/*
  chatbox.__gc
  destroy the chatbox

  Stack:
  1: the instance table
  2: the accessed key
*/
int		lui_destroy_chatbox(lua_State *L)
{
  CHATBOX		*c = check_chatbox(L, 1);

  delwin(c->pad);
  free(c);
  return 0;
}
