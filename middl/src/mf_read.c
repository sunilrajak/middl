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

#include "mf_read.h"
#include "mf_control.h"
#include "mf_priv.h"

typedef struct {
  FILE          *midi_file;
  unsigned char *chrbuf;
  unsigned long  chrbuf_sz;
} mf_read_t;

/********************************************************************/

/* == Reading Values
**
**  readnum(n)  reads n bytes and assembles them to create an integer
**              if n is 0, reads a variable length representation
*/

static long readvar(mf_read_t *mfile)
{
  long v=0; int c;

  if ((c = fgetc(mfile->midi_file)) == EOF) return -1;

  while (c & 0x80 ) {
    v = (v << 7) | (c & 0x7f);
    if ((c = fgetc(mfile->midi_file)) == EOF) return -1;
  }
  v = (v << 7) | c;
  return (v);
}

static long readnum(mf_read_t *mfile,int k)
{
  long x=0, v = 0;

  if (k == 0) return(readvar(mfile));

  while (k-- > 0) {
    if ((x = fgetc(mfile->midi_file)) == EOF) return -1;
    v = (v << 8) | x;
  }
  return v;
}

/* === Read messages
**   readmsg(n)  reads n bytes, stores them in a buffer end returns
**               a pointer to the buffer;
*/

static unsigned char *chrbuf_set(mf_read_t *mfile, long sz)
{
  unsigned char *t = NULL;
  if (sz > mfile->chrbuf_sz) {
    t = realloc(mfile->chrbuf, sz);
    if (t) {
      mfile->chrbuf    = t;
      mfile->chrbuf_sz = sz;
    }
  }
  return mfile->chrbuf;
}

static unsigned char *readmsg(mf_read_t *mfile, long n)
{
  int   c;
  unsigned char *s;

  if (n == 0) return (unsigned char *)"";

  chrbuf_set(mfile, n);
  if (mfile->chrbuf_sz < n) return NULL;
  
  s = mfile->chrbuf;
  while (n-- > 0) {   /*** Read the message ***/
    if ((c = fgetc(mfile->midi_file)) == EOF) return NULL;
    *s++ = c;
  }
  
  return mfile->chrbuf;
}


/* == Finite State Machines
**
**   These macros provide a simple mechanism for defining
** finite state machines (FSM).
**
**   Each state containse a block of instructions:
**
** | STATE(state_name) {
** |   ... C instructions ...
** | }
**
**   To move from a state to another you use the GOTO macro:
**
** | if (c == '*') GOTO(stars);
**
** or, in case of an error) the FAIL(x) macro:
**
** | if (c == '?') FAIL(404);  ... 404 is an error code
**
**   There must be two special states states: ON_FAIL and ON_END
**
*/

#define STATE(x)     x##_: if (0) goto x##_;
#define GOTO(x)      goto x##_
#define FAIL(e)      do {ERROR = e; goto fail_; } while(0)
#define ON_FAIL      fail_:
#define ON_END       FINAL_:
#define FINAL        FINAL_
#define GOTOEND      GOTO(FINAL)
#define FALLTHROUGH  do {ERROR=ERROR;} while (0) 

static int scan_midi(mf_read_t *mfile)
{
  long tmp;
  long v1, v2;
  int ERROR = 0;
  long track_time;
  long tracklen;
  long ntracks;
  long curtrack = 0;
  long status = 0;
  unsigned char *msg;
  long chan;

  STATE(mthd) {
    if (readnum(mfile, 4) != MThd) FAIL(110);
    tmp = readnum(mfile, 4); /* chunk length */
    if (tmp < 6) FAIL(111);
    v1 = readnum(mfile,2);
    ntracks = readnum(mfile,2);
    v2 = readnum(mfile,2);
    ERROR = h_header(v1, ntracks, v2);
    if (ERROR) FAIL(ERROR);
    if (tmp > 6) readnum(mfile,tmp-6);
    GOTO(mtrk);
  }

  STATE(mtrk) {
    if (curtrack++ == ntracks) GOTOEND;
    if (readnum(mfile,4) != MTrk) FAIL(120);
    tracklen = readnum(mfile,4);
    if (tracklen < 0) FAIL(121);
    track_time = 0;
    status = 0;
    ERROR = h_track(0, curtrack, tracklen);
    if (ERROR) FAIL(ERROR);
    GOTO(event);
  }

  STATE(event) {
    tmp = readnum(mfile,0); if (tmp < 0) FAIL(2111);
    track_time += tmp;

    tmp = readnum(mfile,1); if (tmp < 0) FAIL(2112);

    if ((tmp & 0x80) == 0) {
      if (status == 0) FAIL(223); /* running status not allowed! */
      GOTO(midi_event);
    }

    status = tmp;
    v1 = -1;
    if (status == 0xFF) GOTO(meta_event);
    if (status == 0xF0) GOTO(sys_event);
    if (status == 0xF7) GOTO(sys_event);
    if (status >  0xF0) FAIL(543);
    tmp = readnum(mfile,1);
    GOTO(midi_event);
  }

  STATE(midi_event) {
    chan = 1+(status & 0x0F);
    v1 = tmp;
    v2 = -1;
    if (mf_numparms(status) == 2) {
      v2 = readnum(mfile,1);
      if (v2 < 0) FAIL(212);
    }
    ERROR = h_midi_event(track_time, status & 0xF0, chan, v1, v2);
    if (ERROR) FAIL(ERROR);

    GOTO(event);
  }

  STATE(meta_event) {
    v1 = readnum(mfile,1);
    if (v1 < 0) FAIL(2114);
    GOTO(sys_event);
  }

  STATE(sys_event) {
    v2 = readnum(mfile,0);
    if (v2 < 0) FAIL(2115);

    msg = readmsg(mfile,v2);
    if (msg == NULL) FAIL(2116);
    
    if (v1 == me_end_of_track) {
      ERROR = h_track(1, curtrack, track_time);
      if (ERROR) FAIL(ERROR);
      GOTO(mtrk);
    }
    ERROR = h_sys_event(track_time, status, v1, v2, msg);
    if (ERROR) FAIL(ERROR);
    status = 0;
    GOTO(event);
  }

  ON_FAIL {
    if (ERROR < 0) ERROR = -ERROR;
    h_error(ERROR, NULL);
    GOTOEND;
  }

  ON_END {
    return ERROR;
  }
}


/* == Read Midi files
*/

/*
****f*  readmidifile/mf_read
* NAME
*  mf_read
*
* SYNOPSIS
*   error = mf_read(fname)
*
* FUNCTION
*   Reads a midifile and calls the appropriate handlers for
*   each event encountered.
*
* INPUTS
*   fname - the midifile file name
*
* RESULTS
*   error - 0 if everything went fine or an error code.
*
********
*/

int mf_read(char *fname)
{
  int ret;
  mf_read_t mfile;
  
  mfile.midi_file  = NULL;
  mfile.chrbuf     = NULL;
  mfile.chrbuf_sz  = 0;
  
  mfile.midi_file = fopen(fname, "rb");
  if ( mfile.midi_file == NULL) {
    h_error(79,"File not found");
    return -1;
  }
  
  ret = scan_midi(&mfile);
  
  if (mfile.midi_file) fclose(mfile.midi_file); 
  if (mfile.chrbuf)    free(mfile.chrbuf);

  return ret;
}

/*************************************************************/

#define hndl_error        0
#define hndl_writetrack   1
#define hndl_header       2
#define hndl_track        3
#define hndl_midi_event   4
#define hndl_sys_event    5

int mf_null_handler() {return 0;}

t_error      h_error      = (t_error     )mf_null_handler;
t_writetrack h_writetrack = (t_writetrack)mf_null_handler;
t_header     h_header     = (t_header    )mf_null_handler;
t_track      h_track      = (t_track     )mf_null_handler;
t_midi_event h_midi_event = (t_midi_event)mf_null_handler;
t_sys_event  h_sys_event  = (t_sys_event )mf_null_handler;

#define DMP_HANDLERS
#ifdef DMP_HANDLERS

/* == Demo handlers
**
**   These default handlers are for demonstration and documentation
** purpose only. They dump the events in hex format.
**
****f*  handlers/mf_dmp_header
* NAME
*  mf_dmp_header
*
* SYNOPSIS
*   error = mf_dmp_header (type, ntracks, division)
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
static int mf_dmp_header (short type, short ntracks, short division)
{
  printf("HEADER: %u, %u, %u\n", type, ntracks, division);
  return 0;
}

/****f* handlers/mf_dmp_track

* NAME
*  mf_dmp_track

* SYNOPSIS
*  error = mf_dmp_track (eot, tracknum, tracklen)
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

static int mf_dmp_track (short eot, short tracknum, unsigned long tracklen)
{
  if (eot)
    h_sys_event(tracklen, st_meta_event, me_end_of_track, 0, NULL);
  else
    printf("TRACK: %d (%lu)\n", tracknum, tracklen);

  return 0;
}

/*
****f*  handlers/mf_dmp_midi_event
*  NAME
*    mf_dmp_midi_event
*
*  SYNOPSIS
*    error = int mf_dmp_midi_event(tick, type, chan, data1, data2)
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
*      data2 - the second data byte. If it's < 0  is to be ignored.
*
*  RESULT
*    error - non-zero on failure
*
********
*/

static int mf_dmp_midi_event(unsigned long tick, short type, short chan,
                                                  short data1, short data2)
{
  printf("%8ld %02X %02X %02X", tick, type, chan, data1);
  if (data2 >= 0) printf(" %02X", data2);
  printf("\n");
  return 0;
}

/*
****f*  handlers/mf_dmp_sys_event
* NAME
*  mf_dmp_sys_event
*
* SYNOPSIS
*  error = mf_dmp_sys_event(tick, type, aux, len, data)
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
static int mf_dmp_sys_event(unsigned long tick, short type, short aux,
                                               long len, unsigned char *data)
{
  printf("%8ld %02X ", tick, type);
  if (aux >= 0) printf("%02X ", aux);
  printf("%04lX ", (unsigned long)len);
  if (type == 0xFF && (0x01 <= aux && aux <= 0x07)) {
    while (len-- > 0) printf("%c", *data++);
  }
  else {
    while (len-- > 0) printf("%02X", *data++);
  }
  printf("\n");

  return 0;
}

/*
****f*  handlers/mf_dmp_error
* NAME
*   mf_dmp_error
*
* SYNOPSIS
*   error = mf_dmp_error(err, msg)
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
static int mf_dmp_error(short err, char *msg)
{
  if (msg == NULL) msg = "";
  fprintf(stderr, "Error %d - %s\n", err, msg);
  return err;
};

/*
****f*  handlers/mf_dmp_writetrack
* NAME
*   mf_dmp_writetrack
*
*  SYNOPSIS
*   ret = mf_dmp_writetrack(trknum)
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
static int mf_dmp_writetrack(short trknum)
{
  return 0;
};

mf_handlers mf_dmp_handlers = {
  (mf_hndl) mf_dmp_error,      (mf_hndl) mf_dmp_writetrack,
  (mf_hndl) mf_dmp_header,     (mf_hndl) mf_dmp_track,
  (mf_hndl) mf_dmp_midi_event, (mf_hndl) mf_dmp_sys_event,
};
#else

/* If the symbol {DMP_HANDLERS} is not defined when compiling this
** file, {mf_dmp_handlers} is set to contain empty handlers. This means
** that is always safe to execute the following:
**       |mf_sethandlers(mf_dmp_handlers);|
*/
mf_handlers mf_dmp_handlers = {
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

