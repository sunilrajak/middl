/*
**  (C) by Remo Dentato (rdentato@gmail.com)
**
** Permission to use, copy, modify and distribute this code and
** its documentation for any purpose is hereby granted without
** fee, provided that the above copyright notice, or equivalent
** attribution acknowledgement, appears in all copies and
** supporting documentation.
**
** Copyright holder makes no representations about the suitability
** of this software for any purpose. It is provided "as is" without
** express or implied warranty.
*/

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "mf_seq.h"


static int l_read(lua_State *L)
{
    return 0;
}

static int l_write(lua_State *L)
{
    return 0;
}

static int l_wrt_midi_event(lua_State *L)
{
    return 0;
}

static int l_wrt_sys_event(lua_State *L)
{
    return 0;
}

static struct luaL_Reg msq_funcs[5] = {
  {"read"             , l_read},
  {"write"            , l_write},
  {"midi_event"       , l_wrt_midi_event},
  {"sys_event"        , l_wrt_sys_event},

  {NULL, NULL}
};

int luaopen_msq(lua_State *L)
{
  lua_newtable(L);
  luaL_setfuncs (L,msq_funcs,0);
  lua_pushvalue(L,-1);
  return 1;  /* Leave table on the top */
}

