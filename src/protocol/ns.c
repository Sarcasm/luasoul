/*
 * ns.c for luasoul
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
#include <string.h>
#include "lua/lua_utils.h"
#include "netsoul.h"
#include "protocol/ns.h"
#include "lua_protocol.h"

#define luasoul_optfunction_field(L, narg, fname) {                     \
    lua_getfield(L, narg, fname); /* function to be called */           \
    if (lua_isnil(L, narg))                                             \
      return 0;                                                         \
    else                                                                \
      if (!lua_isfunction(L, narg))                                     \
        {                                                               \
          luasoul_error(L, fname " should be either a function or `none'"); \
          return 1;                       /* error */                   \
        }                                                               \
  }                                                                     \
    
/* Callback handlers */
int           ns_new_msg(void *L, const char *login,
                         const char *msg)
{
  luasoul_optfunction_field(L, -1, "new_msg");
  lua_pushvalue(L, -2);           /* dup `this', first argument */
  lua_pushstring(L, login);
  lua_pushstring(L, msg);

  /* 3 args, no return, no error function */
  lua_pcall(L, 3, 0, 0);

  return 0;
}

int           ns_typing_start(void *L, const char *login)
{
  luasoul_optfunction_field(L, -1, "typing_start");
  lua_pushvalue(L, -2);           /* dup `this', first argument */
  lua_pushstring(L, login);

  /* 2 args, no return, no error function */
  lua_pcall(L, 2, 0, 0);
  return 0;
}

int           ns_typing_end(void *L, const char *login)
{
  luasoul_optfunction_field(L, -1, "typing_end");
  lua_pushvalue(L, -2);           /* dup `this', first argument */
  lua_pushstring(L, login);

  /* 2 args, no return, no error function */
  lua_pcall(L, 2, 0, 0);
  return 0;
}

int           ns_login(void *L, const char *login)
{
  luasoul_optfunction_field(L, -1, "login");
  lua_pushvalue(L, -2);           /* dup `this', first argument */
  lua_pushstring(L, login);

  /* 2 args, no return, no error function */
  lua_pcall(L, 2, 0, 0);
  return 0;
}

int           ns_logout(void *L, const char *login)
{
  luasoul_optfunction_field(L, -1, "logout");
  lua_pushvalue(L, -2);           /* dup `this', first argument */
  lua_pushstring(L, login);

  /* 2 args, no return, no error function */
  lua_pcall(L, 2, 0, 0);
  return 0;
}

int           ns_status_changed(void *L, const char *login, const char *status)
{
  luasoul_optfunction_field(L, -1, "status_changed");
  lua_pushvalue(L, -2);           /* dup `this', first argument */
  lua_pushstring(L, login);
  lua_pushstring(L, status);

  /* 3 args, no return, no error function */
  lua_pcall(L, 3, 0, 0);

  return 0;
}

int           ns_unknow_event(void *L, const char *msg)
{
  luasoul_optfunction_field(L, -1, "unknow_event");
  lua_pushvalue(L, -2);           /* dup `this', first argument */
  lua_pushstring(L, msg);

  /* 2 args, no return, no error function */
  lua_pcall(L, 2, 0, 0);
  return 0;
}


/* Constructor */
/*
  Create a new netsoul session

  Netsoul{
  -- Variables
    login               : Default $USER
    socks_pass          : for md5 auth, REQUIRED
    userdata            : Client name, default 'LuaSoul v0.42'
    location            : User location, default '-'
    server              : Default 'ns-server.epita.fr'
    port                : Default '4242'

  -- Callbacks (optionals)
    new_msg(login, msg)
    typing_start(login)
    typing_end(login)
    login(login)
    logout(login)
    status_changed(login, new_status)
    unknow_event(raw_msg)
  }

  on success return a Netsoul session otherwise return `nil' and an
  error message.
*/
int             new_ns(lua_State *L)
{
  netsoulConfig   settings;
  netsoulSession *N;
  const char     *err;
  int             protocol_ref;

  /* set callbacks */
  memset(&settings, 0, sizeof(settings));

  luasoul_optstring_field(L,   1, "login",      settings.login,      NULL);
  luasoul_checkstring_field(L, 1, "socks_pass", settings.socks_pass);
  luasoul_optstring_field(L,   1, "userdata",   settings.userdata,   NULL);
  luasoul_optstring_field(L,   1, "location",   settings.location,   NULL);
  luasoul_optstring_field(L,   1, "server",     settings.server,     NULL);
  luasoul_optstring_field(L,   1, "port",       settings.port,       NULL);

  /* callbacks */
  settings.callbacks.unknow_event   = ns_unknow_event;
  settings.callbacks.new_msg        = ns_new_msg;
  settings.callbacks.typing_start   = ns_typing_start;
  settings.callbacks.typing_end     = ns_typing_end;
  settings.callbacks.login          = ns_login;
  settings.callbacks.logout         = ns_logout;
  settings.callbacks.status_changed = ns_status_changed;

  N = netsoul_create_session(&settings, &err);
  if (N == NULL)
    {
      lua_pushnil(L);
      lua_pushfstring(L, "Netsoul{} failed: %s\n", err);
      return 2;
    }

  /* push the protocol table onto the stack */
  protocol_ref = get_protocol_ref(L);
  lua_rawgeti(L, LUA_REGISTRYINDEX, protocol_ref);

  /* fprintf(stderr, "ist--> %s\n", lua_typename(L, lua_type(L, -1))); */

  /* Create the userdatum */
  *((netsoulSession **) lua_newuserdata(L, sizeof(N))) = N;
  /* set instance metatable to registered methods */
  luaL_getmetatable(L, NETSOUL_CLASS);
  lua_setmetatable(L, -2);

  /* dup userdatum (rawseti() pop the value) */
  lua_pushvalue(L, -1);

  /*
    Stack:
    -3: the protocol table
    -2: the userdatum
    -1: the userdatum
  */
  lua_rawseti(L, -3, lua_objlen(L, -3) + 1);

  return 1;
}

/* Getters */

/* this structure map members to getters() */
static const t_index_wrap       ns_get_methods[]=
  {
    {NULL,                      NULL, 0}
  };


/*
  This function handle the `__index' field.

  Stack:
  1: the instance table
  2: the accessed key
*/
int             ns_index(lua_State *L)
{
  ooHandleIndex(ns_get_methods);
}



/* Setters */

/* this structure map members to setters() */
static const t_index_wrap       ns_set_methods[]=
  {
    {ns_set_ns_func,            REG_MEMBER("new_msg")},
    {ns_set_ns_func,            REG_MEMBER("typing_start")},
    {ns_set_ns_func,            REG_MEMBER("typing_end")},
    {ns_set_ns_func,            REG_MEMBER("login")},
    {ns_set_ns_func,            REG_MEMBER("logout")},
    {ns_set_ns_func,            REG_MEMBER("status_changed")},
    {ns_set_ns_func,            REG_MEMBER("unknow_event")},
    {NULL,                      NULL, 0}
  };


/*
  netsoul.key = true/false
  or
  netsoul["key"] = true/false

  Stack:
  1: the instance table
  2: the accessed key
  3: the value to set
*/
int             ns_newindex(lua_State *L)
{
  return lua_oo_accessors(L, ns_set_methods);
}


/*
  This function set the cursor position.

  Stack:
  1: the instance table
  2: the accessed key
  3: the new index
 */
int             ns_set_ns_func(lua_State *L)
{
  lua_getmetatable(L, 1);

  /* move metatable in position 1 */
  lua_replace(L, 1);

  /* now we can set metatable.key = value */
  lua_rawset(L, 1);

  return 0;
}


/* Utils */

static const luaL_reg ns_instance_methods[]=
  {
    {"connect",         ns_connect},
    {"spy_user",        ns_spy_user},
    {"send",            ns_send_msg},
    {"typing_start",    ns_send_typing_start},
    {"typing_end",      ns_send_typing_end},
    {"deconnect",       ns_deconnect},
    {"__index",         ns_index},
    {"__newindex",      ns_newindex},
    {"__gc",            destroy_ns},
    {"__tostring",      ns_tostring},
    {NULL,              NULL}
  };


/*
  Map all the fields of the class/object.
*/
int             ns_register(lua_State *L)
{
  ooHandleFuncMapping(NETSOUL_CLASS, ns_instance_methods);
}



/* Methods  */

/*
  Netsoul:connect()
  connect to the netsoul server.

  if an error occurs return `false' and call luasoul_error() else
  return true.
*/
int             ns_connect(lua_State *L)
{
  netsoulSession *N;
  const char     *err;

  luasoul_checkclass(L, 1, NETSOUL_CLASS, N); /* get input */
  if (netsoul_connect(N, &err))               /* error */
    {
      luasoul_error(L, err);
      lua_pushboolean(L, 0);
      return 1;
    }

  /* TODO: store the Ns session */
  lua_pushboolean(L, 1);

  return 1;
}

/*
  Netsoul:spy_user(login)
  show event for the user
*/
int             ns_spy_user(lua_State *L)
{
  netsoulSession *N;
  const char     *login;

  luasoul_checkclass(L, 1, NETSOUL_CLASS, N); /* get input */
  luasoul_checkstring(L, 2, login);

  netsoul_spy_user(N, login);

  return 0;
}

/*
  Netsoul:send_msg(login, msg)
  send a message
*/
int             ns_send_msg(lua_State *L)
{
  netsoulSession *N;
  const char     *login;
  const char     *msg;

  luasoul_checkclass(L, 1, NETSOUL_CLASS, N); /* get input */
  luasoul_checkstring(L, 2, login);
  luasoul_checkstring(L, 3, msg);

  netsoul_send_msg(N, login, msg);

  return 0;
}

/*
  Netsoul:typing_start(login)
  send 'typing start...'
*/
int             ns_send_typing_start(lua_State *L)
{
  netsoulSession *N;
  const char     *login;

  luasoul_checkclass(L, 1, NETSOUL_CLASS, N); /* get input */
  luasoul_checkstring(L, 2, login);

  netsoul_send_typing_start(N, login);

  return 0;
}

/*
  Netsoul:typing_end(login)
  send 'typing end...'
*/
int             ns_send_typing_end(lua_State *L)
{
  netsoulSession *N;
  const char     *login;

  luasoul_checkclass(L, 1, NETSOUL_CLASS, N); /* get input */
  luasoul_checkstring(L, 2, login);

  netsoul_send_typing_end(N, login);

  return 0;
}

/*
  Netsoul:deconnect()
  deconnect to the netsoul server.
*/
int             ns_deconnect(lua_State *L)
{
  netsoulSession *N;

  luasoul_checkclass(L, 1, NETSOUL_CLASS, N); /* get input */
  netsoul_deconnect(N);
  return 0;
}

/*
  tostring(netsoul), netsoul.__tostring
  just print the type and pointer address of the Netsoul
*/
int             ns_tostring(lua_State *L)
{
  lua_pushfstring(L, "netsoul: %p", lua_touserdata(L, 1));
  return 1;
}



/* Destructor */

/*
  netsoul.__gc
  destroy the netsoul session

  Stack:
  1: the instance table
  2: the accessed key
*/
int             destroy_ns(lua_State *L)
{
  netsoulSession *N;

  luasoul_checkclass(L, 1, NETSOUL_CLASS, N); /* get input */
  netsoul_destroy_session(N);

  return 0;
}
