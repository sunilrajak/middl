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

#ifdef DEBUG
#define DMP_HANDLERS
#endif

#include "mf_priv.h"

int mf_null_handler() {return 0;}

t_error      h_error      = (t_error     )mf_null_handler;
t_writetrack h_writetrack = (t_writetrack)mf_null_handler;
t_header     h_header     = (t_header    )mf_null_handler;
t_track      h_track      = (t_track     )mf_null_handler;
t_midi_event h_midi_event = (t_midi_event)mf_null_handler;
t_sys_event  h_sys_event  = (t_sys_event )mf_null_handler;

#ifdef DMP_HANDLERS

/* == Demo handlers
**
**   These default handlers are for demonstration and documentation
** purpose only. They dump the events in hex format.
**
****f*  handlers/dmp_header
* NAME
*  dmp_header
*
* SYNOPSIS
*   error = dmp_header (type, ntracks, division)
*
* FUNCTION
*   It's an example of a |header| handler. It is called when
*   the header of the midifile is recognized.
*
* INPUTS
*   type     - the type of the midifile (0, 1, or 2)
*   ntracks  - the number of tracks in the midifile
*   division - the resolution of events. If >0 it's
*              expressed as /ppqn/, if negative is /smpte/.
*              See the <midi> documentation for more details.
*
* RESULTS
*   error - non-zero on failure
********
**
*/
static int dmp_header (short type, short ntracks, short division)
{
  printf("HEADER: %u, %u, %u\n", type, ntracks, division);
  return 0;
}

/****f* handlers/dmp_track

* NAME
*  dmp_track

* SYNOPSIS
*  error = dmp_track (eot, tracknum, tracklen)
*
* FUNCTION
*       This handler is called twice per track. Once at the
*       beginning of the track and once at the end.
*
* INPUTS
* eot      - false (0) if the call refers to the beginning of the
*            track. true (non-0) if the call is for the end of the track
* tracknum - the number of track the call refers to. Tracks are
*            numbered from 0.
* tracklen - if it's the end of the track, it contains the
*            clock tick of the {me_end_of_track} metaevent.
*            If it's the beginning of the track is the length of the
*            track in bytes.
* RESULT
**   error - non-zero on failure
*********
*/

static int dmp_track (short eot, short tracknum, unsigned long tracklen)
{
  if (eot)
    h_sys_event(tracklen, st_meta_event, me_end_of_track, 0, NULL);
  else
    printf("TRACK: %d (%lu)\n", tracknum, tracklen);

  return 0;
}

/*
****f*  handlers/dmp_midi_event
*  NAME
*    dmp_midi_event
*
*  SYNOPSIS
*    error = int dmp_midi_event(tick, type, chan, data1, data2)
*
*  FUNCTION
*    This handler is called when a midi event is encoutered. Events
*    this type ara also referred to as "channel events".
*
*  INPUTS
*     tick - the clock tick at which the event occurs.
*     type - the /status/ byte. It can be one of:
*              {st_note_off}           (|0x80|)
*              {st_note_on}            (|0x90|)
*              {st_key_pressure}       (|0xA0|)
*              {st_control_change}     (|0xB0|)
*              {st_program_change}     (|0xC0|)
*              {st_channel_pressure}   (|0xD0|)
*              {st_pitch_bend}         (|0xE0|)
*
*      chan  - the channel on which the event occurs. Channels are
*              numbered from 1 to 16.
*      data1 - the first (or only) data byte
*      data2 - the second data byte. If it's <0 has to be ignored.
*
*  RESULT
*    error - non-zero on failure
*
********
*/

static int dmp_midi_event(unsigned long tick, short type, short chan,
                                                  short data1, short data2)
{
  printf("%8ld %02X %02X %02X", tick, type, chan, data1);
  if (data2 >= 0) printf(" %02X", data2);
  printf("\n");
  return 0;
}

/*
****f*  handlers/dmp_sys_event
* NAME
*  dmp_sys_event
*
* SYNOPSIS
*  error = dmp_sys_event(tick, type, aux, len, data)
*
* FUNCTION
*   This handler is called when a non-midi event is encoutered.
*   This includes sysex, system continuation and meta events.
*
* INPUTS
*   tick - the clock tick at which the event occurs.
*   type - may have the following values:
*          st_meta_event         for meta events
*          st_system_exclusive   for sysex
*          st_system_continue    for continuations or arbitrary data.
*   aux  - only meaningful for meta events, contains the type
*          of the metaevent.
*   len  - the number of bytes in the event data.
*   data - the event data.
*
* RESULTS
*  error - the errocode (0 if no errors).
********
*/
static int dmp_sys_event(unsigned long tick, short type, short aux,
                                               long len, unsigned char *data)
{
  printf("%8ld %02X ", tick, type);
  if (aux >= 0) printf("%02X ", aux);
  printf("%04lX ", (unsigned long)len);
  while (len-- > 0)
    printf("%02X", *data++);
  printf("\n");

  return 0;
}

/*
****f*  handlers/dmp_error
* NAME
*   dmp_error
*
* SYNOPSIS
*   error = dmp_error(err, msg)
*
* FUNCTION
*   This handler is called when an error occurs. It is used both
*   when reading or writing a midifile.
*
* INPUTS
*   err  - the error code
*   msg  - the error message. It may be NULL.
*
* RESULTS
*  error - the errocode (0 if no errors).
*
********
*/
static int dmp_error(short err, char *msg)
{
  if (msg == NULL) msg = "";
  fprintf(stderr, "Error %d - %s\n", err, msg);
  return err;
};

/*
****f*  handlers/dmp_writetrack
* NAME
*   dmp_writetrack
*
*  SYNOPSIS
*   ret = dmp_writetrack(trknum)
*
* FUNCTION
*   This handler is called by {mf_write()} once for each track that
*   has to be written in the midifile.
*
* INPUTS
*   trknum - the number of the track. Tracks are numbered startin
*            from 0
*
* RESULTS
*   ret - A negative number on error or the non negative delta
*         to the {end-of-track} metaevent on success.
*
********
*/
static int dmp_writetrack(short trknum)
{
  return 0;
};

mf_handlers dmp_handlers = {
  (mf_hndl) dmp_error,      (mf_hndl) dmp_writetrack,
  (mf_hndl) dmp_header,     (mf_hndl) dmp_track,
  (mf_hndl) dmp_midi_event, (mf_hndl) dmp_sys_event,
};
#else

/* If the symbol {DMP_HANDLERS} is not defined when compiling this
** file, {dmp_handlers} is set to contain empty handlers. This means
** that is always safe to execute the following:
**       |mf_sethandlers(dmp_handlers);|
*/
mf_handlers dmp_handlers = {
  mf_null_handler, mf_null_handler, mf_null_handler,
  mf_null_handler, mf_null_handler, mf_null_handler
};
#endif

/* == Managing handlers <#hndl>
*
* :warning: No check is perfomed on the pointers that are passed
*           as arguments. Use great care when setting or getting handlers
*           especially with the order of pointers. See <mf.h#hndl> for
*           further details.
*
***f*  handlers/mf_sethandlers
* NAME
*   mf_sethandlers
*
* FUNCTION
*   Set the handlers that will be used for reading/writing midifiles.
*
* INPUTS
*   new_handlers - an array of pointers to functions.
*
********
*/

void mf_sethandlers(mf_handlers new_handlers)
{
  h_error      = (t_error      )new_handlers[hndl_error     ];
  h_writetrack = (t_writetrack )new_handlers[hndl_writetrack];
  h_header     = (t_header     )new_handlers[hndl_header    ];
  h_track      = (t_track      )new_handlers[hndl_track     ];
  h_midi_event = (t_midi_event )new_handlers[hndl_midi_event];
  h_sys_event  = (t_sys_event  )new_handlers[hndl_sys_event ];
}

/*
****f*  handlers/mf_gethandlers
* NAME
*   mf_gethandlers
*
* FUNCTION
*   Copies the current handlers into an array.
*
* INPUTS
*   cur_handlers - the array into which copy the pointers
*                  to the current handlers.
*
********
*/
void mf_gethandlers(mf_handlers cur_handlers)
{
  cur_handlers[hndl_error     ] = (mf_hndl)h_error      ;
  cur_handlers[hndl_writetrack] = (mf_hndl)h_writetrack ;
  cur_handlers[hndl_header    ] = (mf_hndl)h_header     ;
  cur_handlers[hndl_track     ] = (mf_hndl)h_track      ;
  cur_handlers[hndl_midi_event] = (mf_hndl)h_midi_event ;
  cur_handlers[hndl_sys_event ] = (mf_hndl)h_sys_event  ;
}

