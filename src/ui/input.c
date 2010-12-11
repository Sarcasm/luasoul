/*
 * input.c for luasoul
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

#include <stdlib.h>
#include "utils.h"
#include "ui/ui_utils.h"
#include "lua/lua_utils.h"
#include "ui/input.h"
#include "ui/style.h"

typedef struct
{
  WINDOW        *pad;
  int            height;
  int            width;
  int            begin_y;
  int            begin_x;
  int            off;      /* number of offscreen col (if height is 1)
                              or lines (height > 1) */
  wchar_t       *wbuff; /* contain spaces for the content of the window */
  size_t        len;    /* wbuff lenght */
}               INPUT;

static int      get_current_position(const INPUT *i);
static void     set_current_position(const INPUT *i, int pos);
static int      alloc_cchar_buffer(INPUT *i);

static INPUT    default_input =
  {
    NULL,                       /* pad          */
    0,                          /* height       */
    0,                          /* width        */
    0,                          /* begin_y      */
    0,                          /* begin_x      */
    0,                          /* off          */
    NULL,                       /* wccstr       */
    0,                          /* len          */
  };

/* Constructor */
/*
  Create a new input
  Input
  (
  -- required
  int           width,
  int           height,
  int           begin_x,
  int           begin_y,

  -- not required
  int           off
  )

  3 cases for off:
  - if height = 1       off is the number of off-screen cols
  - if height > 1       off is the number of off-screen lines
  - if off is not set   the field is not growable
  FIXME: check NULL pad
*/
int             lui_new_input(lua_State *L)
{
  INPUT         *i;

  i             = malloc(sizeof(*i));
  *i            = default_input;
  luasoul_checkint(L, 1, i->width);
  luasoul_checkint(L, 2, i->height);
  luasoul_checkint(L, 3, i->begin_x);
  luasoul_checkint(L, 4, i->begin_y);
  luasoul_optint(L, 5, i->off, 0);

  if (i->height == 1)
    i->pad = newpad(i->height, i->width + i->off);
  else
    i->pad = newpad(i->height + i->off, i->width);

  if (i->pad == NULL || alloc_cchar_buffer(i))
    {
      luasoul_error(L, "can't create input");
      return 0;
    }

  *((INPUT **) lua_newuserdata(L, sizeof(i))) = i;

  /* set instance metatable to registered methods */
  luaL_getmetatable(L, INPUT_CLASS);
  lua_setmetatable(L, -2);

  return 1;
}

/* Getters */

/* this structure map members to getters() */
static const t_index_wrap       lui_input_get_methods[]=
  {
    {lui_input_get_index,       REG_MEMBER("index")},
    {lui_input_get_buff,        REG_MEMBER("buff")},
    {NULL,                      NULL, 0}
  };


/*
  This function handle the `__index' field.

  Stack:
  1: the instance table
  2: the accessed key
*/
int             lui_input_index(lua_State *L)
{
  ooHandleIndex(lui_input_get_methods);
}

/*
  This function return the index of current character.

  Stack:
  1: the instance table
  2: the accessed key
 */
int             lui_input_get_index(lua_State *L)
{
  INPUT         *i;

  luasoul_checkclass(L, 1, INPUT_CLASS, i); /* get input */
  lua_pushnumber(L, get_current_position(i) + 1); /* lua 'array' start at 1 */
  return 1;
}

/*
  This function return the content of the buffer.

  Stack:
  1: the instance table
  2: the accessed key
  FIXME: use win_wchnstr, handle multiple lines ?
*/
int             lui_input_get_buff(lua_State *L)
{
  INPUT         *i;
  int            pos;
  wchar_t       *buff;
  int            len;


  luasoul_checkclass(L, 1, INPUT_CLASS, i); /* get input */
  pos = get_current_position(i);            /* save cursor position */

  /* assume i->height == 1 */
  len = i->width * i->off;
  buff = malloc((len + 1) * sizeof(*buff));
  len = mvwinnwstr(i->pad, 0, 0, buff, len) - 1;
  while (len > 0 && iswspace(buff[len])) /* remove leading space characters */
    --len;
  buff[len + 1] = L'\0';
  luasoul_pushwstring(L, buff);
  free(buff);

  set_current_position(i, pos); /* restore cursor position */
  return 1;
}



/* Setters */

/* this structure map members to setters() */
static const t_index_wrap       lui_input_set_methods[]=
  {
    {lui_input_set_index,       REG_MEMBER("index")},
    {lui_input_set_style,       REG_MEMBER("style")},
    {NULL,                      NULL, 0}
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
int             lui_input_newindex(lua_State *L)
{
  return lua_oo_accessors(L, lui_input_set_methods);
}


/*
  input.style = {
  bold = true,
  foreground = 3,
  ...
  }
  or input.style = myStyle

  Set the global style of the input, take effect immediatly for each row
  of the input.

  Stack:
  2nd argument is a accessed member (style)
  3rd argument is a table who can contain all the attributes above
*/
int             lui_input_set_style(lua_State *L)
{
  INPUT         *i;
  t_style       s;

  luasoul_checkclass(L, 1, INPUT_CLASS, i); /* fill i */
  get_style(L, 3, s);

  wbkgd(i->pad, s.on & ~s.off);

  return 0;
}

/*
  This function set the cursor position.

  Stack:
  1: the instance table
  2: the accessed key
  3: the new index
 */
int             lui_input_set_index(lua_State *L)
{
  INPUT         *i;
  int            n;

  luasoul_checkclass(L, 1, INPUT_CLASS, i); /* fill i */
  luasoul_checkint(L, 3, n);
  /* lua 'array' start a 1 */
  set_current_position(i, n - 1);
  return 0;
}


/* Utils */

static const luaL_reg lui_input_instance_methods[]=
  {
    {"refresh",         lui_refresh_input},
    {"erase",           lui_erase_input},
    {"delch",           lui_delch_input},
    {"addch",           lui_addch_input},
    /* {"addstr",               lui_addstr_input}, */
    {"__index",         lui_input_index},
    {"__newindex",      lui_input_newindex},
    {"__gc",            lui_destroy_input},
    {"__tostring",      lui_input_tostring},
    {NULL,              NULL}
  };


/*
  Map all the fields of the class/object.
*/
int             lui_input_register(lua_State *L)
{
  ooHandleFuncMapping(INPUT_CLASS, lui_input_instance_methods);
}


/*
  Allocate a cchar_t* buffer, for input.buff and friends.
  Return 0 on success, 1 on error
*/
static int      alloc_cchar_buffer(INPUT *i)
{
  if (i->height == 1)
    i->len = (i->width + i->off) * i->height;
  else
    i->len = (i->height + i->off) * i->width;
  i->wbuff = malloc((i->len + 1) * sizeof(i->wbuff));
  return i->wbuff == NULL;
}

/* TODO: Handle multi-column input */
static int      refresh_input(INPUT *i)
{
  int   pad = getcurx(i->pad) - i->width + 1;

  /*
    pad < 0 is not tested because `man prefresh' say:
    negative value for pad column are treated as if they were zero.
  */
  if (pad > i->off)
    pad = i->off;

  return prefresh(i->pad,                    /* pad               */
                  0,                         /* pad line          */
                  pad,                       /* pad column        */
                  i->begin_y,                /* screen line       */
                  i->begin_x,                /* screen column     */
                  i->begin_y + i->height -1, /* screen max line   */
                  i->begin_x + i->width -1); /* screen max column */
}

/*
  Return the index of the current char.
*/
static int      get_current_position(const INPUT *i)
{
  int           x, y;

  getyx(i->pad, y, x);
  return x + y * i->width;
}

/*
  Set a new cursor position.
*/
static void     set_current_position(const INPUT *i, int pos)
{
  /* FIXME: check max limits */
  if (pos < 0)
    pos = 0;

  wmove(i->pad,
        pos / i->width,         /* y */
        pos % i->width);        /* x */
}


/* Methods  */

/*
  input:refresh()
  refresh/display/update input
*/
int             lui_refresh_input(lua_State *L)
{
  INPUT         *i;

  luasoul_checkclass(L, 1, INPUT_CLASS, i); /* get input */
  refresh_input(i);
  return 0;
}

/*
  input:erase()
  erase the input buffer.
*/
int             lui_erase_input(lua_State *L)
{
  INPUT         *i;

  luasoul_checkclass(L, 1, INPUT_CLASS, i); /* get input */
  werase(i->pad);
  return 0;
}

/*
  input:addch(ch)
  put a wide character
*/
int             lui_addch_input(lua_State *L)
{
  INPUT         *i;
  wchar_t       *wstr;
  size_t        len;
  cchar_t       ch;

  luasoul_checkclass(L, 1, INPUT_CLASS, i); /* get input */
  luasoul_checklwcstr(L, 2, wstr, len);     /* get wstring */

  /* FIXME: correct attributs to setcchar() ? */
  if (setcchar(&ch, wstr, WA_NORMAL, 0, NULL) == OK
      && wins_wch(i->pad, &ch) == OK)
    {
      /* man setcchar()
         ...contain at most one spacing character, which must be the first */
      len = (size_t) wcwidth(*wstr);
      if (len != (size_t) -1 && len) /* don't move for 0 width */
        set_current_position(i, get_current_position(i) + len);
    }

  free(wstr);
  return 0;
}

/*
  input:delch()
  delete a character
*/
int             lui_delch_input(lua_State *L)
{
  INPUT         *i;

  luasoul_checkclass(L, 1, INPUT_CLASS, i); /* get input */
  wdelch(i->pad);

  return 0;
}

/*
  input:addstr(s)
  put a wide character string
*/
/* int          lui_addstr_input(lua_State *L) */
/* { */
/* TODO */
/*   INPUT              *i; */
/*   wchar_t    *wstr; */
/*   /\* const char     *wstr = luaL_checkstring(L, 2); *\/ */
/*   size_t     len; */
/*   cchar_t    c; */

/*   luasoul_checkclass(L, 1, INPUT_CLASS, i); /\* get input *\/ */
/*   luasoul_checklwcstr(L, 2, wstr, len); */
/*   /\* luaL_checkstring(L, 2); *\/ */

/*   /\* wins_wstr(i->pad, wstr); *\/ */


/* /\* int setcchar(cchar_t *wcval, const wchar_t *wch, const attr_t attrs, *\/ */
/* /\*              short color_pair, const void *opts); *\/ */
/*  setcchar(&c, wstr, A_NORMAL, 1, NULL); */


/*   /\* while (*wstr != L'\0') *\/ */
/*  wadd_wch(i->pad, &c); */
/*   /\* wins_wstr(i->pad, L"Pourquoi ça ?"); *\/ */
/*   /\* wprintw(i->pad, "%s", wstr); *\/ */
/*   set_current_position(i, get_current_position(i) + len); */

/*   return 0; */
/* } */

/*
  tostring(input), input.__tostring
  just print the type and pointer address of the Input
*/
int             lui_input_tostring(lua_State *L)
{
  lua_pushfstring(L, "input: %p", lua_touserdata(L, 1));
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
int             lui_destroy_input(lua_State *L)
{
  INPUT         *i;

  luasoul_checkclass(L, 1, INPUT_CLASS, i); /* get input */

  delwin(i->pad);
  free(i->wbuff);
  free(i);

  return 0;
}
