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


/* msq.open("file","r",
            int mf_read( char          *fname      , mf_fn_error   fn_error   ,
             mf_fn_header   fn_header  , mf_fn_track   fn_track   ,
             mf_fn_midi_evt fn_midi_evt, mf_fn_sys_evt fn_sys_evt  );

            */

static int mf_dmp_header (short type, short ntracks, short division)
{
  printf("HEADER: %u, %u, %u\n", type, ntracks, division);
  return 0;
}

static int mf_dmp_track (short eot, short tracknum, unsigned long tracklen)
{
  printf("TRACK %s: %d (%lu %s)\n", eot?"END":"START", tracknum, tracklen,eot?"ticks":"bytes");
  return 0;
}

static int mf_dmp_midi_evt(unsigned long tick, short type, short chan,
                                                  short data1, short data2)
{
  printf("%8ld %02X %02X %02X", tick, type, chan, data1);
  if (data2 >= 0) printf(" %02X", data2);  /* data2 < 0 means there's no data2! */
  printf("\n");
  return 0;
}

static int mf_dmp_sys_evt(unsigned long tick, short type, short aux,
                                               long len, unsigned char *data)
{
  printf("%8ld %02X ", tick, type);
  if (aux >= 0) printf("%02X ", aux);
  printf("%04lX ", (unsigned long)len);
  type = (type == 0xFF && (0x01 <= aux && aux <= 0x09));
  if (type) {  while (len-- > 0) printf("%c", *data++);   }  /* ASCII */
  else      {  while (len-- > 0) printf("%02X", *data++); }  /* DATA */
  printf("\n");

  return 0;
}

static int mf_dmp_error(short err, char *msg)
{
  if (msg == NULL) msg = "";
  fprintf(stderr, "Error %03d - %s\n", err, msg);
  return err;
};


static int l_open(lua_State *L)
{
  char *fname;
  char *mode;

  return 0;
}

static int l_close(lua_State *L)
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

static struct luaL_Reg msq_funcs[] = {
  {"open"             , l_open},
  {"close"            , l_close},
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

