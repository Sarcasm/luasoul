/*
** input.c for luasoul in /home/papin_g
**
** Made by Guillaume Papin
** Login   <papin_g@epitech.net>
**
** Started on  Mon Oct 18 23:56:48 2010 Guillaume Papin
** Last update Thu Oct 28 23:49:29 2010 Guillaume Papin
*/

/*
  `OOP model' inspired by:
  http://lua-users.org/wiki/UserDataWithPointerExample
  http://lua-users.org/wiki/ObjectProperties
*/

#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <form.h>
#include "utils.h"
#include "lua/lua_utils.h"
#include "ui/ui_utils.h"
#include "ui/input.h"

/* not in the header because need curses.h */
FORM		*check_input(lua_State *L, int n);
static FIELD	*create_input_field(lua_State *L, int width, int height, int n);

/* Constructor */

static const luaL_reg	lui_input_class_methods[]=
  {
    {"new",		lui_new_input},
    {NULL,		NULL}
  };


/*
  Input.new(widht, height, x, y[, limit])

  Create a new input
  if set `limit' is (2 cases):
  - if input field is only one line (height = 1)
  limit is the numbers of chars in the field.
  - else (height > 1)
  limit is the number of line of the field.

  limit set to 0 means `no limit' field have an infinite size
*/
int		lui_new_input(lua_State *L)
{
  WINDOW	*w;
  FIELD		*field[2];
  FORM		*f;
  int		rows, cols;
  int		width	= luaL_checkint(L, 1);
  int		height	= luaL_checkint(L, 2);
  int		begin_x	= luaL_checkint(L, 3);
  int		begin_y	= luaL_checkint(L, 4);

  /* create a new input field */
  field[0] = create_input_field(L, width, height, 5);
  field[1] = NULL;

  /* Create the form */
  f = new_form(field);

  /* Calculate the area required for the form */
  scale_form(f, &rows, &cols);

  /* Create the window to be associated with the form */
  w = newwin(rows, cols, begin_y, begin_x);

  /* Set main window and sub window */
  set_form_win(f, w);
  set_form_sub(f, w);		/* useless ? */

  /* Post form */
  post_form(f);
  wrefresh(w);

  /* create and fill a new userdata */
  *(FORM **) (lua_newuserdata(L, sizeof(f))) =  f;

  /* set instance metatable to registered methods */
  luaL_getmetatable(L, INPUT_INST);
  lua_setmetatable(L, -2);

  return 1;
}


/* Accessor utils */


/* Getters */

/* this structure map members to getters() */
static const t_index_wrap	lui_window_get_methods[]=
  {
    {lui_input_get_buff,	REG_MEMBER("buff")},
    {NULL,			NULL,	0}
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

  return lua_oo_accessors(L, lui_window_get_methods);
}

/*
   input.buf
   the field content

   Stack:
   1: the instance table
   2: the accessed key
*/
int		lui_input_get_buff(lua_State *L)
{
  FORM		*f = check_input(L, 1);
  char		*buff;
  size_t	len;

  form_driver(f, REQ_VALIDATION);

  /* return the entire field buffer, with trailing spaces */
  buff = field_buffer(current_field(f), 0);

  len = strlen(buff);
  if (!len--)	       /* probably impossible for a field buffer... */
    luaL_error(L, "(internal error) empty input field buffer");
  while (len && buff[len] == ' ')
    len--;

  /*
    if len = 0 and the character is a space, len is really 0,
    otherwise len is 1, the character[0]
  */
  if (*buff != ' ' || len > 0)
    len++;

  /* push the buffer content on the top of the Lua stack */
  lua_pushlstring(L, buff, len);
  return 1;
}


/* Setters */

/* this structure map members to setters() */
static const t_index_wrap	lui_input_set_methods[]=
  {
    {lui_input_set_buff,	REG_MEMBER("buff")},
    {NULL,			NULL, 0}
  };

/*
  input.key = "..."
  or
  input["key"] = "..."

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
  input.buff = "..."
  fill the input field buffer

  Stack:
  1: the instance table
  2: the accessed key
  3: the value to set
*/
int		lui_input_set_buff(lua_State *L)
{
  FORM		*f = check_input(L, 1);
  const char	*buff = luaL_checkstring(L, 3);

  set_field_buffer(current_field(f), 0, buff);
  form_driver(f, REQ_END_FIELD);
  return 0;			/* TODO: send something ? */
}


/* Utils */

static const luaL_reg lui_input_instance_methods[]=
  {
    {"driver",		lui_input_driver},
    {"resize",		lui_resize_input},
    {"move",		lui_move_input},
    {"refresh",		lui_refresh_input},
    {"__index",		lui_input_index},
    {"__newindex",	lui_input_newindex},
    {"__gc",		lui_destroy_input},
    /* {"__tostring",	lui_input_tostring}, /\* TODO: code me :) *\/ */
    {NULL,		NULL}
  };


/*
  Map all the fields of the class/object.
*/
int		lui_input_register(lua_State *L)
{
  luaL_register(L, INPUT_CLASS, lui_input_class_methods);
  luaL_newmetatable(L, INPUT_INST);
  luaL_register(L, NULL, lui_input_instance_methods);

  lua_pop(L, 1);		/* drop metatable */
  return 0;
}

/*
  Check if value at index `n' in the stack is a Input instance,
  return it or raised an error.
*/
FORM		*check_input(lua_State *L, int n)
{
  FORM		**input;

  luaL_checktype(L, n, LUA_TUSERDATA);
  input = (FORM **) luaL_checkudata(L, n, INPUT_INST);
  if (input == NULL)
    luaL_typerror(L, n, INPUT_INST);
  if (*input == NULL)
    luaL_error(L, "null input");
  return *input;
}

/*
  Create a new input field of the given size, set some basics attributes

  n is the position on the stack of the optionnal field limit
*/
static FIELD	*create_input_field(lua_State *L, int width, int height, int n)
{
  int		limit	= lua_isnoneornil(L, n) ? width : luaL_checkint(L, n);
  FIELD		*field;

  field = new_field(height, width, 0, 0, 0, 0);

  /* set field options */
  field_opts_off(field, O_AUTOSKIP | O_STATIC);

  /*
    by default the maximum number of characters
    is limited to the width of the field
  */
  set_max_field(field, limit);

  return field;
}

/* Methods  */

/*
  input:driver(var)
  var can be used to add character in the form or request a command:
  - an integer, a char to handle
  - a string to add (or if the string is a command like REQ_XXX... a command)
  FIXME: how to send REQ_XXX string ?
*/
int		lui_input_driver(lua_State *L)
{
  FORM		*f = check_input(L, 1);
  int		ch;
  const char	*s;

  if (! lua_isnumber(L, 2) &&	/* not a number and not a command ?
  				   add each character of the string */
      (ch = form_request_by_name((s = luaL_checkstring(L, 2)))) == E_NO_MATCH)
    {
      while (*s)
  	form_driver(f, *(s++));
    }
  else
    {
      /* FIXME: already called in the first if statement */
      if (lua_isnumber(L, 2))
	ch = lua_tointeger(L, 2);
      form_driver(f, ch);
    }
  wrefresh(form_win(f));
  return 0;
}

/*
  input:refresh()
  refresh the form window
*/
int		lui_refresh_input(lua_State *L)
{
  wrefresh(form_win(check_input(L, 1)));
  return 0;
}

/*
  input_field:resize(width, height[, new_limit])
  resize the input_field, old limit is not copied
*/
int		lui_resize_input(lua_State *L)
{
  FORM		*f = check_input(L, 1);
  WINDOW	*w;
  FIELD		**ofield;	/* old field */
  FIELD		*nfield;	/* new field */
  const int	width = luaL_checkint(L, 2);
  const int	height = luaL_checkint(L, 3);
  char		*buffer;
  int		rows, cols;

  /* update win first */
  w = form_win(f);
  if (width <= 0 && height <= 0)
    {
      luaL_error(L, "can't resize window: invalid dimension");
      return 0;
    }


  /* save old field and old buffer */
  form_driver(f, REQ_VALIDATION);
  ofield = form_fields(f);
  buffer = field_buffer(ofield[0], 0);

  /* create the new field */
  nfield = create_input_field(L, height, width, 4);

  /*
    Copy the old buffer in the new field

    TODO: restore cursor position
   */
  set_field_buffer(nfield, 0, buffer);
  free_field(ofield[0]);

  /* fill the new field with old datas */
  free_field(ofield[0]);
  ofield[0] = nfield;
  /*
    I don't know if it's useless or not but I suppose set_form_fields() can
    update few things...
  */
  set_form_fields(f, ofield);



  /* Calculate the area required for the form */
  scale_form(f, &rows, &cols);

  /* resize the window */
  wresize(w, rows, cols);

  return 0;
}

/*
  input_field:move(x, y)
  move the input field
*/
int		lui_move_input(lua_State *L)
{
  WINDOW	*w = form_win(check_input(L, 1));
  const int	x = luaL_checkint(L, 2);
  const int	y = luaL_checkint(L, 3);

  wmove(w, y, x);
  return 0;
}



/* Destructor */

/*
  input.__gc
  destroy the input field

  Stack:
  1: the instance table
  2: the accessed key
*/
int		lui_destroy_input(lua_State *L)
{
  FORM		*f;

  f = check_input(L, 1);
  unpost_form(f);

  /* free_field(form_fields(f)[0]); */

  /*
    FIXME: segfault
    free_form(f);
  */
  return 0;
}
