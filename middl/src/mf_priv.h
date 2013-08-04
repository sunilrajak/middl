/*
**  (C) 2008 by Remo Dentato (rdentato@users.sourceforge.net)
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

#ifndef MIDDL_PRIV_H
#define MIDDL_PRIV_H

#include "ctype.h"
#include "mf.h"

#define MThd 0x4d546864
#define MTrk 0x4d54726b

/*  One that will be called every time an error occur */
typedef int (*t_error)(short, char *);

/*  One to be called to write out a track */
typedef int (*t_writetrack)(short);

/*  Four to be called every time an event is found while reading a midifile.
*/

typedef int (*t_header    )(short, short, short);
typedef int (*t_track     )(short, short, unsigned long );
typedef int (*t_midi_event)(unsigned long, short, short, short, short );
typedef int (*t_sys_event )(unsigned long, short, short, long,
                                                          unsigned char *);

extern t_error      h_error      ;
extern t_writetrack h_writetrack ;
extern t_header     h_header     ;
extern t_track      h_track      ;
extern t_midi_event h_midi_event ;
extern t_sys_event  h_sys_event  ;

extern char mf_note_base[];

#define lowercase(x)    tolower((int)(x))
#define charisdigit(x)  isdigit((int)(x))
#define charisspace(x)  isspace((int)(x))

/* Get the number of parameters needed by a channel message
** s is the status byte.
*/
#define mf_numparms(s) ("\2\2\2\2\1\1\2"[(s & 0x70)>>4])

/* Debugging macros */
#ifdef DEBUG
#define dbgmsg(...) ((fflush(stdout), fprintf(stderr,"# "),\
                      fprintf(stderr,__VA_ARGS__), fflush(stderr)))

#define dbgdmp(p,l) do { unsigned char *q=p; int k=l;\
                      fflush(stdout); fprintf(stderr,"# ");\
                      while (k-- > 0) fprintf(stderr,"%02X ",*q++);\
                      fprintf(stderr,"\n"); fflush(stderr);\
                    } while(0)

#define dbgif(x,y) if x y

#else

#define dbgmsg(...)
#define dbgdmp(p,l)
#define dbgif(x,y)

#endif /* DEBUG */

#define _dbgmsg(...)
#define _dbgdmp(p,l)
#define _dbgif(x,y)

#endif
