/*
** input.c for luasoul in /home/papin_g/projects/luasoul/src
**
** Made by Guillaume Papin
** Login   <guillaume.papin@epitech.eu>
**
** Started on  Mon Nov  1 19:33:30 2010 Guillaume Papin
** Last update Mon Nov  8 20:24:05 2010 Guillaume Papin
*/

/*
  `Object model' inspired by:
  http://lua-users.org/wiki/UserDataWithPointerExample
  http://lua-users.org/wiki/ObjectProperties
*/

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <curses.h>
#include "lua/lua_utils.h"
#include "ui/ui_utils.h"
#include "ui/input.h"
#include "ui/style.h"

typedef struct
{
  WINDOW	*pad;
  char		*buff;
  int		height;
  int		width;
  int		begin_y;
  int		begin_x;
  int		off;	   /* number of offscreen col (if height is 1)
			      or lines (height > 1) */
  size_t	max;		/* the position of the last char */
  size_t	len;		/* the lenght of the buffer */
}		INPUT;

/* not in the header because need curses.h */
static INPUT	*check_input(lua_State *L, int n);
static int	resize_input_buff(INPUT *i);
static int	refresh_input(INPUT *i);
static int	get_current_position(const INPUT *i);
static void	set_current_position(const INPUT *i, int pos);
static int	input_addch_at_pos(INPUT *i, char ch);

/* Constructor */

static const luaL_reg	lui_input_class_methods[]=
  {
    {"new",		lui_new_input},
    {NULL,		NULL}
  };

static INPUT	default_input =
  {
    NULL,			/* pad		*/
    NULL,			/* buff		*/
    0,				/* height	*/
    0,				/* width 	*/
    0,				/* begin_y	*/
    0,				/* begin_x	*/
    0,				/* off	 	*/
    0,				/* max	 	*/
    0,				/* len	 	*/
  };

/*
  Create a new input
  Input.new
  (
  -- required
  int		width,
  int		height,
  int		begin_x,
  int		begin_y,

  -- not required
  int		off
  )

  3 cases for off:
  - if height = 1	off is the number of off-screen cols
  - if height > 1	off is the number of off-screen lines
  - if off is not set	the field is not growable

  FIXME: error handling, bad values
*/
int		lui_new_input(lua_State *L)
{
  INPUT		*i;

  i		= malloc(sizeof(*i));
  *i		= default_input;
  i->width	= luaL_checkint(L, 1);
  i->height	= luaL_checkint(L, 2);
  i->begin_x	= luaL_checkint(L, 3);
  i->begin_y	= luaL_checkint(L, 4);
  i->off	= luaL_optint(L, 5, 0);

  if (i->height == 1)
    i->pad = newpad(i->height, i->width + i->off);
  else
    i->pad = newpad(i->height + i->off, i->width);

  /* init buffer */
  resize_input_buff(i);

  /* don't refresh, if the style is set after for example */
  /* refresh_input(c); */

  *((INPUT **) lua_newuserdata(L, sizeof(i))) = i;

  /* set instance metatable to registered methods */
  luaL_getmetatable(L, INPUT_INST);
  lua_setmetatable(L, -2);

  return 1;
}


/* Getters */

/* this structure map members to getters() */
static const t_index_wrap	lui_input_get_methods[]=
  {
    {lui_input_get_index,	REG_MEMBER("index")},
    {lui_input_get_buff,	REG_MEMBER("buff")},
    {NULL,			NULL, 0}
  };


/*
  This function handle the `__index' field.

  Stack:
  1: the instance table
  2: the accessed key
*/
int		lui_input_index(lua_State *L)
{
  const char	*key = luaL_checkstring(L, 2);

  lua_getmetatable(L, 1);
  lua_getfield(L, -1, key);

  /* Either key is name of a method in the metatable */
  if (!lua_isnil(L, -1))
    return 1;

  /* ... or its a field access, so recall as self.get(self, value). */
  lua_settop(L, 2);   /* restore the initial call state, arg1, arg2 */

  return lua_oo_accessors(L, lui_input_get_methods);
}

/*
  This function return the content of the buffer.

  Stack:
  1: the instance table
  2: the accessed key
 */
int		lui_input_get_buff(lua_State *L)
{
  const INPUT	*i = check_input(L, 1);

  lua_pushlstring(L, i->buff, i->max);
  return 1;
}

/*
  This function return the index of current character.

  Stack:
  1: the instance table
  2: the accessed key
 */
int		lui_input_get_index(lua_State *L)
{
  const INPUT	*i = check_input(L, 1);

  lua_pushnumber(L, get_current_position(i) + 1); /* lua 'array' start at 1 */
  return 1;
}


/* Setters */

/* this structure map members to setters() */
static const t_index_wrap	lui_input_set_methods[]=
  {
    {lui_input_set_style,	REG_MEMBER("style")},
    {lui_input_set_buff,	REG_MEMBER("buff")},
    {lui_input_set_index,	REG_MEMBER("index")},
    {NULL,			NULL, 0}
  };

/*
  input.key = true/false
  or
  input["key"] = true/false

  Stack:
  1: the instance table
  2: the accessed key
  3: the value to set
*/
int		lui_input_newindex(lua_State *L)
{
  return lua_oo_accessors(L, lui_input_set_methods);
}

/*
  input:set_win_style{
  bold = true,
  foreground = 3,
  ...
  }

  set the global style of the input, take effect immediatly for each row
  of the input

  Stack:
  2nd argument is a accessed member (style)
  3rd argument is a table who can contain all the attributes above

  TODO: Optimisation ?
*/

int		lui_input_set_style(lua_State *L)
{
  INPUT		*i = check_input(L, 1);
  t_style	s;

  get_style(L, 3, s);
  wbkgd(i->pad, s.on & ~s.off);
  return 0;
}

/*
  This function set the content of the buffer.

  Stack:
  1: the instance table
  2: the accessed key
  3: the new buff
 */
int		lui_input_set_buff(lua_State *L)
{
  INPUT		*i = check_input(L, 1);
  const char	*s = luaL_checkstring(L, 3);

  i->max = lua_objlen(L, 3);

  /* update buff */
  if (i->max > i->len)
    i->max = i->len;
  strncpy(i->buff, s, i->max);

  /* update screen */
  werase(i->pad);
  mvwaddnstr(i->pad, 0, 0, i->buff, i->max);

  return 0;
}

/*
  This function set the cursor position.

  Stack:
  1: the instance table
  2: the accessed key
  3: the new index
 */
int		lui_input_set_index(lua_State *L)
{
  const INPUT	*i = check_input(L, 1);
  int		n = luaL_checkint(L, 3);

  /* lua 'array' start a 1 */
  set_current_position(i, n - 1);
  return 0;
}

/* Utils */

static const luaL_reg lui_input_instance_methods[]=
  {
    {"refresh",		lui_refresh_input},
    {"erase",		lui_erase_input},
    {"addch",		lui_addch_input},
    {"addstr",		lui_addstr_input},
    {"remove",		lui_remove_input},
    {"move_cursor",	lui_input_move_cursor},
    {"resize",		lui_resize_input},
    {"move",		lui_move_input},
    {"__index",		lui_input_index},
    {"__newindex",	lui_input_newindex},
    {"__gc",		lui_destroy_input},
    {"__tostring",	lui_input_tostring},
    {NULL, NULL}
  };

/*
  Map all the fields of the class/object.
*/
int		lui_input_register(lua_State *L)
{
  luaL_register(L, INPUT_CLASS, lui_input_class_methods);
  luaL_newmetatable(L, INPUT_INST);
  luaL_register(L, NULL, lui_input_instance_methods);

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
  Check if value at index `n' in the stack is a Input instance,
  return it or raised an error.
*/
static INPUT	*check_input(lua_State *L, int n)
{
  INPUT		**i;

  luaL_checktype(L, n, LUA_TUSERDATA);
  i = (INPUT **) luaL_checkudata(L, n, INPUT_INST);
  if (i == NULL || *i == NULL)
    luaL_typerror(L, n, INPUT_INST);
  return *i;
}

/*
  Return the index of the current char.
*/
static int	get_current_position(const INPUT *i)
{
  int		x, y;

  getyx(i->pad, y, x);
  return x + y * i->width;
}

/*
  Set a new cursor position.
*/
static void	set_current_position(const INPUT *i, int pos)
{
  if (pos > (int) i->max)
    pos = i->max;
  else if (pos < 0)
    pos = 0;

  wmove(i->pad,
	pos / i->width,		/* y */
	pos % i->width);	/* x */
}

/*
  Resize (or create if was NULL) the buffer
  set the following fields of the INPUT:
  i->len
  i->buff
  i->max (only if max is out of range for the new buffer lenght)
  @return ERR on error, OK on success
*/
static int	resize_input_buff(INPUT *i)
{
  /* FIXME: +1 for each \n ? */
  i->len = (i->height == 1) ? i->width + i->off : i->width * (i->height
							      + i->off);

  /* max can't be upper to the lenght of the buffer */
  if (i->max > i->len)
    i->max = i->len;

  /* allocate a new buffer */
  i->buff = realloc(i->buff, sizeof(i->buff) * i->len + 1);
  if (i->buff == NULL)
    return ERR;

  return OK;
}

/* TODO: Handle multi-column input */
static int	refresh_input(INPUT *i)
{
  if (i->height == 1)
    {
      int	pad = getcurx(i->pad) - i->width + 1;

      if (pad > i->off)
      	pad = i->off;
      /*
	 pad < 0 is not tested because `man prefresh' say:
	 negative vale for pad column are treated as if they were zero.
      */
      return prefresh(i->pad,			 /* pad		*/
		      0,			 /* pad line	*/
		      pad,			 /* pad column	*/
		      i->begin_y,		 /* screen line	*/
		      i->begin_x,		 /* screen column	*/
		      i->begin_y + i->height -1, /* screen max line	*/
		      i->begin_x + i->width -1); /* screen max column */
    }
  return prefresh(i->pad,		     /* pad		*/
  		  0,			     /* pad line	*/
  		  0,			     /* pad column	*/
  		  i->begin_y,		     /* screen line	*/
  		  i->begin_x,		     /* screen column	*/
  		  i->begin_y + i->height -1, /* screen max line	*/
  		  i->begin_x + i->width -1); /* screen max column */
}

/*
  Put the char under the cursor position, and move the cursor one line
  FIXME: handle tabulation, and other multi line char
  En fait il faut re faire la fonction pour qu'elle gere les insertions toussa
  et checker si le s[pos] ne depasse pas la taille du buffer
*/
static int	input_addch_at_pos(INPUT *i, char ch)
{
  int		x, y;

  getyx(i->pad, y, x);
  if (isprint(ch) && winsch(i->pad, ' ') != ERR && waddch(i->pad, ch) != ERR)
    {
      size_t	pos = x + y * i->width;

      if (pos > i->len)		/* shouldn't happen */
	return ERR;

      /* need to move next characters */
      if (pos < i->max)
	{
	  size_t	next;

	  for (next = i->max; next > pos; next--)
	    i->buff[next] = i->buff[next -1];
	}
      i->buff[pos] = ch;
      /* if (! insert_mode) */
      i->max++;

      return OK;
    }
  return ERR;
}



/* Methods  */

/*
  input:refresh()
  refresh the input (update physical input to match virtual input).
*/
int		lui_refresh_input(lua_State *L)
{
  INPUT		*i = check_input(L, 1);

  refresh_input(i);
  return 0;
}

/*
  input:erase()
  erase the input buffer.
*/
int		lui_erase_input(lua_State *L)
{
  INPUT		*i = check_input(L, 1);

  if (werase(i->pad) == OK)
    i->max = 0;
  return 0;
}

/*
  input:addch(s)
  put a char in the virtual input
*/
int		lui_addch_input(lua_State *L)
{
  INPUT		*i	= check_input(L, 1);
  const char	*str	= luaL_checkstring(L, 2);

  input_addch_at_pos(i, *str);
  return 0;
}

/*
  input:addstr(s)
  put a string in the virtual input
*/
int		lui_addstr_input(lua_State *L)
{
  INPUT		*i	= check_input(L, 1);
  const char	*str	= luaL_checkstring(L, 2);

  while (*str && input_addch_at_pos(i, *str) == OK)
    str++;
  return 0;
}

/*
  string input:remove(n)
  removes the characters from the cursor position to `n'
  return the removed characters
*/
int		lui_remove_input(lua_State *L)
{
  INPUT		*i	= check_input(L, 1);
  char		n	= luaL_checkint(L, 2);
  size_t	pos;
  size_t	prev;
  int		dec;


  pos = get_current_position(i);

  /*
    if n is negative move the cursor |n| backwards characters
    and set n to the opposite value, with that we can delete |n| characters
  */
  if (n < 0)
    {
      /* check border */
      dec = pos + n;
      if (dec < 0)
	n -= dec;

      /* move backward |n| characters */
      pos += n;
      set_current_position(i, pos);

      n = -n;
    }
  else				/* n > 0 */
    {
      /* check border */
      dec = n + pos - i->max;
      if (dec > 0)
	n -= dec;
    }

  /* count number of successful delete */
  for (dec = 0; n--; dec++)
    if (wdelch(i->pad) == ERR)
      break ;

  /* nothing removed */
  if (dec == 0)
    return 0;

  /* push the deleted characters */
  lua_pushlstring(L, i->buff + pos, dec);

  /* delete in buffer */
  for (prev = pos; (prev + dec) < i->max; prev++)
    i->buff[prev] = i->buff[prev + dec];

  /* update max */
  i->max -= dec;

  return 1;
}

/*
  input:move_cursor(n)
  move the cursor n characters
  if n is negative the cursor go backward
  FIXME: vocabulary
*/
int		lui_input_move_cursor(lua_State *L)
{
  INPUT		*i	= check_input(L, 1);
  const int	n	= luaL_checkint(L, 2);

  set_current_position(i, get_current_position(i) + n);
  return 0;
}

/*
  input:resize(width, height)
  resize the input

  FIXME: prendre en compte l'offset
*/
int		lui_resize_input(lua_State *L)
{
  INPUT		*i	= check_input(L, 1);
  const int	width	= luaL_checkint(L, 2);
  const int	height	= luaL_checkint(L, 3);

  if (width <= 0 && height <= 0)
    {
      luaL_error(L, "can't resize input: invalid dimension");
      return 0;
    }
  if (wresize(i->pad, height, width + i->off) == OK)
    {
      i->height = height;
      i->width = width;
      resize_input_buff(i);
    }
  return 0;
}

/*
  input:move(x, y)
  move the input
*/
int		lui_move_input(lua_State *L)
{
  INPUT		*i = check_input(L, 1);
  const int	x = luaL_checkint(L, 2);
  const int	y = luaL_checkint(L, 3);

  if (x < 0 && y < 0)
    {
      luaL_error(L, "can't move input: invalid position");
      return 0;
    }
  else
    {
      i->begin_x = x;
      i->begin_y = y;
    }
  return 0;
}

/*
  tostring(input), input.__tostring
  just print the type and pointer address of the Input
*/
int		lui_input_tostring(lua_State *L)
{
  lua_pushfstring(L, "Input: %p", lua_touserdata(L, 1));
  return 1;
}

/* Destructor */

/*
  input.__gc
  destroy the input

  Stack:
  1: the instance table
  2: the accessed key
*/
int		lui_destroy_input(lua_State *L)
{
  INPUT		*i = check_input(L, 1);

  delwin(i->pad);
  free(i);
  return 0;
}
