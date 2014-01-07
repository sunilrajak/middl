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

#include "mf.h"




static struct luaL_reg msq_funcs[] = {
  {"read"             , l_read},
  {"write"            , l_write},
  {"score"            , l_score},
  {"midi_event"       , l_wrt_midi_event},
  {"sys_event"        , l_wrt_sys_event},
  
  {NULL, NULL}
};

int mf_luaopen(lua_State *L)
{
  gL = L;
  luaL_register(gL, "msq", msq_funcs);
  return 0; 
}
