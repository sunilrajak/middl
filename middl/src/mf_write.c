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

#include "mf_priv.h"

#include <math.h>

short mf_qnote_ticks ;
short mf_num_tracks  ;
short mf_format      ;
char  mf_meter[4]    ;

/* Get the number of parameters needed by a channel message
** s is the status byte.
*/
static char *nparms = "\2\2\2\2\1\1\2";
#define numparms(s) (nparms[(s & 0x70)>>4])

/*
****t*  wrt_types/event
* NAME
*  event
*
* DESCRIPTION
*   The event structure is just the header for the event. The
*   data field is to be considered the beginning of an array of
*   at least 4 bytes
*
********
*/
typedef struct event_s {
   unsigned long tick;
   unsigned long size;
   unsigned char data[2];
} event;

/*
****v*  wrt_variables/evt_buf, evt_buf_sz, evt_buf_wm, evt_lst, evt_lst_sz, evt_lst_cnt
*
* NAME
*  evt_buf, evt_buf_sz, evt_buf_wm,
*  evt_lst, evt_lst_sz, evt_lst_cnt
*
* DESCRIPTION
*  evt_buf     - the buffer for the events
*  evt_buf_wm  - the "watermark" is the offset to the next free byte.
*  evt_buf_sz  - the size (in bytes) of the buffer;
*  evt_lst     - the sorted array of events (as offeset to the event)
*  evt_lst_sz  - the size of the array of events
*  evt_lst_cnt - the number of events in the arrat;
*
********
*/
static unsigned char *evt_buf     = NULL;
static unsigned long  evt_buf_wm  = 0;
static unsigned long  evt_buf_sz  = 0;

static unsigned long *evt_lst     = NULL;
static unsigned long  evt_lst_sz  = 0;
static unsigned long  evt_lst_cnt = 0;

/*
****f*  wrt_functions/align
* NAME
*   align
*
* FUNCTION
*   Ensure that the variable is aligned to the size of unsigned long
*
* SYNOPSIS
*   align(var)
*
* INPUTS
*   var - the name of the variable to align
*
* BUGS
*   Assumes that sizeof(unsigned long) is a power of 2!
********
*/
#define align(x) do { while ((x) & (sizeof(unsigned long)-1)) x++;} while(0)


/*
****f*  wrt_functions/chk_evt_buf
* NAME
*  chk_evt_buf
*
* FUNCTION
*   Check if the events buffer has enough space and reallocates
*   it if needed.
*
* SYNOPSIS
*   chk_evt_buf(n)
*
* INPUTS
*   n - the bytes needed
*
* RESULT
*   0 - if there is enough space
*  -1 - if out of memory
********
*/
static int chk_evt_buf(unsigned long n)
{
  if ((evt_buf_sz - evt_buf_wm) <= n) {
    evt_buf_sz += ((n/1024) + 1 ) * 1024;
    evt_buf = realloc(evt_buf, evt_buf_sz);
    if (evt_buf == NULL) return -1;
  }
  return 0;
}

static int chk_evt_lst(unsigned long n)
{
 if ((evt_lst_sz - evt_lst_cnt) <= n * sizeof(unsigned long *)) {
   evt_lst_sz += (((n/128) + 1 ) * 128) * sizeof(unsigned long *);
   evt_lst = realloc(evt_lst, evt_lst_sz);
   if (evt_lst == NULL) return -1;
  }
  return 0;
}

static void b_write8(unsigned char n)
{
  evt_buf[evt_buf_wm++] = (n);
}

static void b_write7(unsigned char n)
{
  evt_buf[evt_buf_wm++] = (n & 0x7F);
}

#if 0
static void b_write16(unsigned short n)
{
  evt_buf[evt_buf_wm++] = (n >> 8);
  evt_buf[evt_buf_wm++] = (n & 0xFF);
}

static void b_write14(unsigned short n)
{
  evt_buf[evt_buf_wm++] = (n >> 7) & 0x7F;
  evt_buf[evt_buf_wm++] = (n & 0x7F);
}

static void b_write32(unsigned long n)
{
  evt_buf[evt_buf_wm++] = (n >> 24);
  evt_buf[evt_buf_wm++] = (n >> 16) & 0xFF;
  evt_buf[evt_buf_wm++] = (n >>  8) & 0xFF;
  evt_buf[evt_buf_wm++] = (n      ) & 0xFF;
}
#endif 

static void b_writemsg(unsigned long len, unsigned char *data)
{
  if (chk_evt_buf(len) != 0) return;
  while(len-- > 0) evt_buf[evt_buf_wm++] = *data++;
}

static void b_writevar(unsigned long n)
{
  unsigned long buf;

  n &= 0x0FFFFFFF;
  _dbgmsg("vardata: %08lX -> ", n);

  buf = n & 0x7F;
  while ((n >>= 7) != 0) {
    buf = (buf << 8) | (n & 0x7F) | 0x80;
  }
  _dbgmsg("%08lX\n", buf);
  while (1) {
    evt_buf[evt_buf_wm++] = buf & 0xFF;
    if ((buf & 0x80) == 0) break;
    buf >>= 8;
  }
}

/******************************/

static FILE * midi_file;
static unsigned long bytes_cnt;

#define eputc(c) (bytes_cnt++, fputc(c,midi_file))

static void f_write8(unsigned char n)
{
  eputc(n);
}

#if 0  /* Not needed */
static void f_write7(unsigned char n)
{
  eputc(n & 0x7F);
}
#endif 

static void f_write16(unsigned short n)
{
  eputc(n >> 8  );
  eputc(n & 0xFF);
}

#if 0  /* Not needed */
static void f_write14(unsigned short n)
{
  eputc(n >> 7  );
  eputc(n & 0x7F);
}
#endif

static void f_write32(unsigned long n)
{
  eputc((n >> 24)       );
  eputc((n >> 16) & 0xFF);
  eputc((n >>  8) & 0xFF);
  eputc((n      ) & 0xFF);
}

static void f_writemsg(unsigned long len, unsigned char *data)
{
  unsigned long k = len;
  if (chk_evt_buf(len) != 0) return;
  while(k-- > 0) eputc(*data++);
}

static void f_writevar(unsigned long n)
{
  unsigned long buf;

  n &= 0x0FFFFFFF;
  _dbgmsg("vardata: %08lX -> ", n);

  buf = n & 0x7F;
  while ((n >>= 7) != 0) {
    buf = (buf << 8) | (n & 0x7F) | 0x80;
  }
  _dbgmsg("%08lX\n", buf);
  while (1) {
    eputc(buf & 0xFF);
    if ((buf & 0x80) == 0) break;
    buf >>= 8;
  }
}


/*****************************/

/*
****v*  wrt_variables/evt_start
*
* NAME
*   evt_start
*
* DESCRIPTION
*   This is used to communicate between the start_event and
*   the end_event functions.
*****
*/
static unsigned long  evt_start;


/*
****f*  wrt_functions/evt_at
* NAME
*  evt_at
*
* RETURN VALUE
*   The pointer to the event at the specified offset.
*
* SYNOPSIS
*   evt_at(offset)
*
* INPUTS
*   offset - offset in byte
*
********
*/
#define evt_at(offset) ((event *)(evt_buf+(offset)))

/*
****f*  wrt_functions/start_event
* NAME
*  start_event
*
* FUNCTION
*   Starts a new event in the event buffer. As side effect it also
*   increases the number of events in the buffer.
*
* SYNOPSIS
*   error = start_event(curtick)
*
* INPUTS
*   curtick - the tick at which the event occurs
*
* TODO
*   Assumes that events will be inserted already almost sorted by
*   time. If this assumption is true, the "one pass of bubble sort"
*   strategy will be quicker than a more sophisticatded sorting
*   algorithm. If, for any reason, events are often posted in reverse
*   order, we'll have to switch to quicksort.
*
* RETURN VALUE
*   zero on success, -1 on failure (out of memory)
*
* SEE ALSO
*  <end_event>.
********
*/
static int  start_event(unsigned long tick)
{
  event *evt;
  unsigned long k;

  if (chk_evt_buf(sizeof(event) + 4) < 0) return -1;
  if (chk_evt_lst(1) < 0) return -1;

  align(evt_buf_wm);

  evt_start = evt_buf_wm;
  evt = evt_at(evt_start);

  evt->tick = tick;
  evt->size = 0;

  evt_lst[evt_lst_cnt++] = evt_start;

  /* One pass of BUBBLE SORT!!! */
  for (k = evt_lst_cnt-1 ;
       k > 0 && evt_at(evt_lst[k-1])->tick > tick;
       k--) {
    evt_lst[k] = evt_lst[k-1];
    evt_lst[k-1] = evt_start;
  }

  evt_buf_wm += offsetof(event, data);

  return 0;
}

/*
****f*  wrt_functions/end_event
* NAME
*  end_event
*
* FUNCTION
*   Set the size of tehe event an.
*
* SYNOPSIS
*   end_event()
*
* SEE ALSO
*  start_event
********
*/
static void end_event()
{
  event *evt;

  evt = evt_at(evt_start);
  evt->size = evt_buf_wm - evt_start - offsetof(event, data) ;
  evt_start = 0;
}


/*
****f*  wrt_functions/mf_midi_event
* NAME
*   mf_midi_event
*
* SYNOPSIS
*   mf_midi_event(tick, type, chan, data1, data2)
*
* FUNCTION
*   Writes out a midi event, i.e. events that are associated to
*   a channel.
*
* INPUTS
*     tick - the clock tick at which the event occurs.
*     type - the status byte. It can be one of:
*              st_note_off           0x80
*              st_note_on            0x90
*              st_key_pressure       0xA0
*              st_control_change     0xB0
*              st_program_change     0xC0
*              st_channel_pressure   0xD0
*              st_pitch_bend         0xE0
*
*      chan  - the channel on which the event occurs. Channels are
*              numbered from 1 to 16.
*      data1 - the first (or only) data byte
*      data2 - the second data byte. If it's <0 has to be ignored.
*
* RETURN VALUE
*   zero on success, -1 on failure
*
* SEE ALSO
*   mf_sys_event
********
*/
int mf_midi_event(unsigned long tick, short type, short chan,
                                                   short data1, short data2)
{
  unsigned char status;

  type &= 0xF0;

  if ((type <= 0x7F) || (0xF0 <= type) || (start_event(tick) != 0))
    return -1 ;

  status = type | (chan > 0 ? (chan - 1) & 0x0F : 0);
  b_write8(status);
  b_write7(data1);
  if (numparms(status) == 2) b_write7(data2);
  end_event();

  return 0;
}

/*
****f*  wrt_functions/mf_sys_event
* NAME
*  mf_sys_event
*
* SYNOPSIS
*   mf_sys_event(tick, type, aux, len, data)
*
* FUNCTION
*     Writes out a non-midi event. This includes sysex,
*     system continuation and meta events.
*
* INPUTS
*  tick - the clock tick at which the event occurs.
*  type - may have the following values:
*          st_meta_event       for meta events
*          st_system_exclusive for sysex
*          st_system_continue  for continuations or arbitrary data.
*  aux  - only meaningful for meta events, contains the type
*         of the metaevent.
*  len  - the number of bytes in the event data.
*  data - the event data.
*
* SEE ALSO
*   mf_midi_event
********
*/
int mf_sys_event(unsigned long tick, short type, short aux,
                                      long len, unsigned char *data)
{
  if (start_event(tick) != 0) return -1;

  b_write8(type);
  if (type == 0xFF) b_write8(aux);
  b_writevar(len);
  b_writemsg(len, data );
  end_event();

  return 0;
}

/************************/

static void write_buf()
{
  unsigned long time = 0;
  unsigned char status;
  unsigned char running = 0;
  int skip;
  int k;
  event *evt;

  for (k = 0; k < evt_lst_cnt; k++) {
    evt = evt_at(evt_lst[k]);
    f_writevar(evt->tick - time);
    status = *(evt->data);
    /* handle running events */
    skip = 0;
    if ((status & 0xF0) == 0xF0) {
      running = 0;
    }
    else if (status & 0x80) {
      if (status == running) skip = 1;
      running = status;
    }

    f_writemsg(evt->size - skip, evt->data + skip);
    time = evt->tick;
  }
}

static int write_track(long trknum)
{
  unsigned long pos_len;
  unsigned long pos_cur;
  long eot;

  if (midi_file == NULL) {
    h_error(309, "Output file not defined");
    return 309;
  }

  f_write32(MTrk);

  if ((pos_len = ftell(midi_file)) <  0) {
    h_error(301, "Unable to store file position");
    return 301;
  }

  f_write32(0);  /* just a placeholder for now */
  bytes_cnt = 0; /* start counting from here */

  evt_buf_wm = 0;
  evt_lst_cnt = 0;

  mf_curtick = 0;
  mf_endtick = 0;

  eot = h_writetrack(trknum);

  if (eot < 0) {
    eot = -eot;
    h_error(eot, "Error while writing track");
    return (eot);
  }

  write_buf();

  f_writevar(eot);    /* write delta to end-of-track as a varnum */
  f_write8(0xFF);  f_write8(0x2F);  f_write8(0x00);

  if ((pos_cur = ftell(midi_file)) <  0) {
    h_error(302, "Unable to store file position");
    return 302;
  }

  if (fseek(midi_file, pos_len, SEEK_SET) < 0) {
    h_error(303, "Unable to reset file position");
    return 303;
  }

  f_write32(bytes_cnt);

  if (fseek(midi_file, pos_cur, SEEK_SET) < 0) {
    h_error(304, "Unable to reset file position");
    return 304;
  }

  return 0;
}


#include <time.h>

/*
****f*  wrt_functions/mf_write
* NAME
*  mf_write
*
* SYNOPSIS
*   error = mf_write(fname, format, ntracks, division)
*
* FUNCTION
*   Creates a new midifile. The h_writetrack(} handler is
*   called for each track to be written.
*
* INPUTS
*   fname    - the output filename
*   format   - 0,1 or 2
*   ntracks  - number of tracks
*   division - ppqn if >0 or smtpe if < 0
*
* TODO
*   handle smpte
*
* RESULTS
*   Returns zero on success or an error code.
*   If an error occurs, the h_error() handler is called.
*
********
*/
int mf_write(char *fname, short format, short ntracks, short division)
{
  short i;
  int error = 0;

  if ((midi_file = fopen(fname, "wb")) == NULL) {
    h_error(308, "Unable to open output file");
    return 308;
  }

  mf_qnote_ticks = division;
  mf_num_tracks  = ntracks;
  mf_format      = format;

  _dbgmsg("WRITE: %d %d %d\n", division, ntracks,format );

  srand(time(NULL));

  if (format == 0) ntracks = 1;

  /* Write Header chunk */
  f_write32(MThd);
  f_write32(6);
  f_write16(format);
  f_write16(ntracks);
  f_write16(division);

  /* The rest of the file is a series of tracks */
  for ( i = 0; i < ntracks && !error; i++) {
    error = write_track(i);
  }

  fclose(midi_file);
  midi_file = NULL;

  free(evt_buf);   evt_buf = NULL;
  evt_buf_sz  = 0; evt_buf_wm  = 0;

  free(evt_lst);   evt_lst = NULL;
  evt_lst_sz  = 0; evt_lst_cnt = 0;

  return error;
}

/*************************************************************/


/**** CHANNEL EVENTS ****/

/*
****v*  wrt_variables/mf_curtick, mf_endtick
*
* NAME
*   mf_curtick
*   mf_endtick
*   
* DESCRIPTION
*   This is the current timer tick. i.e. the last tick an evant was 
*   set for. 
*   mf_endtick records the highest tick seen.
*   mf_lasttick records the previous tick.
********
*/
unsigned long mf_curtick;
unsigned long mf_endtick;
unsigned long mf_lasttick;

/*
****f*  wrt_functions/mf_tick, set_tick
*
* NAME
*   mf_tick, set_tick
*
* SYNOPSIS
*   cur_tick = mf_tick(tick)
*
* FUNCTION
*   Sets or query the tick timer. set_tick()
*   is implemented as a macro and usend internally.
*   Also the highest tick is recorded.
*
* INPUTS
*   tick - If equal to MF_GET_TICK it is ignored, otherwise
*          the tick timer is set to tick.
*
* RESULTS
*   The current timer tick
*
********
*/
#define set_tick(t)  \
  ((mf_lasttick = mf_curtick, mf_curtick=(t)) > mf_endtick \
                                ? mf_endtick = mf_curtick \
                                : mf_curtick)

unsigned long mf_tick(unsigned long tick)
{
  if (tick != MF_GET_TICK) set_tick(tick);
  return mf_curtick;
}


/*
****v*  wrt_variables/cur_channel, tmp_channel
*
* NAME
*   cur_channel
*   tmp_channel
*   
* DESCRIPTION
*   This is the last channel an event has been
*   directed to. tmp_channel is used internally.
********
*/
static unsigned char cur_channel=1;
static unsigned char tmp_channel=1;

/*
****f*  wrt_functions/mf_channel, set_channel
*
* NAME
*   mf_channel, set_channel
*
* SYNOPSIS
*   cur_ch = mf_channel(new_ch)
*
* FUNCTION
*   Sets or query the current channel. set_channel()
*   is implemented as a macro and usend internally.
*
* INPUTS
*   new_ch - If in the range 1..16, sets the channel, otherwise
*            is ignored
*
* RESULTS
*   The current channel
*
********
*/

#define set_channel(c) \
              ((tmp_channel=c) > 0 && tmp_channel <=16 ? \
                             (cur_channel = tmp_channel) : cur_channel)
                                       
unsigned char mf_channel(unsigned char chan)
{
  return set_channel(chan);
}                                       

/*
****f*  wrt_functions/mf_program_change
* NAME
*  mf_program_change
*
* SYNOPSIS
*   mf_program_change(tick, chan, instr)
*
* FUNCTION
*   Sets the instrument for the current channel
*
* INPUTS
*  tick  - the clock tick at which the event occurs.
*  chan  - the channel the event is for
*  instr - the instrument number (from 1 to 128).
*
********
*/

void mf_program_change(unsigned long tick, unsigned char chan, short instr)
{
  _dbgmsg("MF_INSTR: %d\n",instr);
  instr--;
  if (instr <0) instr = 0;
  else if (instr > 127) instr = 127;
  
  mf_midi_event(set_tick(tick), st_program_change, set_channel(chan), instr, -1);
}

/* same random functions used by musicapad */
#define ranf() (((float)rand())/((float)RAND_MAX))

static float rndq(int q)
{
  static float y2;
  static int   phase = 0;
  float x1, x2, w, y1;

  if (q == -1) {
    /* See http://www.taygeta.com/random/gaussian.html for
    ** reference on generating Gaussian distribution.
    */
    phase = 1 - phase;
    if (phase == 0) return y2;
    do {
      x1 = 2.0 * ranf() - 1.0;
      x2 = 2.0 * ranf() - 1.0;
      w = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );

    w = sqrt((-2.0 * log(w)) / w );
    y1 = x1 * w;
    y2 = x2 * w;
    return y1;
  }

  x1 = ranf();
  x2 = 2.0 * fabs(x1 - 0.5);
  
  _dbgmsg("Random: %f %f\n",x1, x2);
  w = 1.0;

  while (q-- > 0) w *= x2;

  x2 = w * (x1 > 0.5? 1 : -1);

  return x2;
}

static short loosew  = 0;
static short looseq  = 0;
static short velvarw = 0;
static short velvarq = 0;

void mf_humanize(short lw, short lq, short vw, short vq)
{
  loosew  = lw;
  looseq  = lq;
  velvarw = vw;
  velvarq = vq;
  printf("LW: %d LQ: %d VW:%d VQ:%d\n",loosew,looseq,velvarw,velvarq);
}

static long loose(unsigned long x)
{
  long l;

  if (loosew > 0) {
    l = (long)floor(0.5 + (float)loosew * rndq(looseq));
    printf("Loose: %ld\n",l);
    l += x;
    if ( l < 0) x = 0; 
    else  x = l;
  }
 return x;
}

static char velvar(char velocity)
{
  float t;
  long vel;
  
  if (velvarw > 0) {
    t = 1.0 + (velvarw * rndq(velvarq)) / 100.0;
    vel = (long)(((float)velocity) * t);
    /*printf("v: %d t: %f = %d \n",velocity, t,vel);*/
    if (vel < 0) vel = 0;
    else if (vel > 127) vel = 127;
    velocity = (char)vel;
  }
  return velocity;
}

/*
****f*  wrt_functions/mf_note_off
* NAME
*  mf_note_off
*
* SYNOPSIS
*   mf_note_off(tick, chan, pitch)
*
* FUNCTION
*   Insert a "note off" midi event
*
* INPUTS
*   tick   - the clock tick at which the event occurs. If humanization
*            parameters have been set, they are applied.
*    chan  - the channel on which the event occurs. Channels are
*            numbered from 1 to 16.
*   pitch  - the note pitch.
*            If highest bit is set, then it's a percussion pitch
*            and channel 10 is used.
*
********
*/
void mf_note_off(unsigned long tick, unsigned char chan, unsigned char pitch)
{
  if (pitch == n_none || pitch == n_rest ) return;
  tick = set_tick(tick);
  mf_midi_event(loose(tick), st_note_off,
                      pitch & 0x80 ? 10 : set_channel(chan), pitch, 0);
}

/*
****f*  wrt_functions/mf_note_on
* NAME
*  mf_note_on
*
* SYNOPSIS
*   mf_note_on(tick, chan, pitch, velocity)
*
* FUNCTION
*   Insert a "note on" midi event
*
* INPUTS
*   tick   - the clock tick at which the event occurs. If humanization
*            parameters have been set, they are applied.
*    chan  - the channel on which the event occurs. Channels are
*            numbered from 1 to 16.
*   pitch  - the note pitch.
*            If highest bit is set, then it's a percussion pitch
*            and channel 10 is used.
*  velocity  - note velocity (0-127). If humanization parameters have been
*            set, they are applied.
*            If 0 it's equivalent to a call to mf_note_off.
*
********
*/
void mf_note_on(unsigned long tick, unsigned char chan,
                                      unsigned char pitch, char velocity)
{
  if (pitch == n_none || pitch == n_rest ) return;
  
  if (velocity == 0) {
    mf_note_off(tick, chan, pitch);
  }
  else {
    tick = set_tick(tick);
    mf_midi_event(loose(tick), st_note_on,
                    pitch & 0x80 ? 10 : set_channel(chan), pitch, velvar(velocity));
  }
}

/*
****f*  wrt_functions/mf_note
* NAME
*  mf_note
*
* SYNOPSIS
*   mf_note(tick, chan, duration, pitch, velocity)
*
* FUNCTION
*   Insert both a "note on" and a "note off".
*
* INPUTS
*      tick  - the clock tick at which the event occurs. If humanization
*              parameters have been set, they are applied.
*      chan  - the channel on which the event occurs. Channels are
*              numbered from 1 to 16.
*     pitch  - the note pitch.
*  duration  - the note duration
*      duty  - Percentage (0-100) of the duration the note is
*              actually played.
*  velocity  - note velocity (0-127).
*
********
*/
void mf_note(unsigned long tick, unsigned char chan, unsigned char pitch,
                                      long duration, char duty, char velocity)
{
  _dbgmsg("NOTE: %d %d %d %d %d\n",tick,chan,duration,pitch,velocity);
  if (pitch == n_rest) {
    set_channel(chan);
  }
  else { 
    if (duration > 0 && velocity > 0) {
      mf_note_on (tick, chan, pitch, velocity);
      mf_note_off(tick + ((duration * 100)/duty), chan,  pitch);
    }
  }
  tick = set_tick(tick+duration);
}

void mf_chord(unsigned long tick, unsigned char chan, char *pitches,
                                      long duration, char duty, char velocity)
{
  if (duration > 0 && velocity > 0) {
    while (*pitches > mf_chordend) {    
      mf_note_on (tick, chan, *pitches, velocity);
      mf_note_off(tick + ((duration * 100)/duty), chan,  *pitches);
      pitches++;
    }
  }
  tick = set_tick(tick+duration);
}


/*
****f*  wrt_functions/mf_key_pressure
* NAME
*  mf_key_pressure
*
* SYNOPSIS
*   mf_key_pressure(tick, chan, pitch, press)
*
* FUNCTION
*
* INPUTS

*
********
*/
void mf_key_pressure(unsigned long tick,  unsigned char chan,
                                                       char pitch, char press)
{
  mf_midi_event(set_tick(tick), st_key_pressure, 
                                              set_channel(chan), pitch, press);
}

void mf_channel_pressure(unsigned long tick,  unsigned char chan, char press)
{
  mf_midi_event(set_tick(tick), st_channel_pressure,
                                                 set_channel(chan), press, -1);
}

void mf_control_change(unsigned long tick, unsigned char chan,
                                             unsigned short ctrl, short value)
{
  tick = set_tick(tick);
  if (ctrl & 0x8080) { /* NRPN or RPN */
    char c = (ctrl & 0x8000) ? cc_non_registered_number
                             : cc_registered_number;
    ctrl &= 0x7F7F;
    mf_midi_event(tick, st_control_change, chan, c  , ctrl >> 8);
    mf_midi_event(tick, st_control_change, chan, c-1, ctrl);

    mf_control_change(tick, chan, cc_data_entry, value);

    mf_midi_event(tick, st_control_change, chan,
                                    cc_registered_number, 0x7F);

    mf_midi_event(tick, st_control_change, chan,
                                    cc_registered_number_lsb  , 0x7F);
  }
  else if (ctrl == cc_non_registered_number || ctrl == cc_registered_number) {
    mf_midi_event(tick, st_control_change, chan, ctrl, value >> 8);
    ctrl --;
  }
  else if (ctrl < 32 && value > 127) { /* 14 bits controllers */
    mf_midi_event(tick, st_control_change, chan, ctrl + 32, value);
    value >>= 7;
  }
  mf_midi_event(tick, st_control_change, set_channel(chan), ctrl, value);
}

/*
****f*  wrt_functions/mf_pitch_bend
* NAME
*  mf_pitch_bend
*
* SYNOPSIS
*   mf_pitch_bend(tick, chan, bend)
*
* FUNCTION
*   Insert a "pitch bend" event
*
* INPUTS
*    tick  - the clock tick at which the event occurs.
*    chan  - the channel on which the event occurs. Channels are
*            numbered from 1 to 16.
*    bend  - the amount of bending. float value ranges from -100.0 to 100.0
*
********
*/
void mf_pitch_bend(unsigned long tick, unsigned char chan, float bend)
{
  int b;
  
  b = floor(8192 + (8192 * bend) /100.0);

  if (b < 0) b  = 0;
  else if (b >  16384) b  = 16383;

  mf_midi_event(set_tick(tick), st_pitch_bend, set_channel(chan), b >> 7, b);
}


/*******************/

void mf_sequence_number(unsigned long tick, short seqnum)
{
  char buf[2];
  buf[0] = seqnum >> 8;  buf[1] = seqnum & 0xFF;
  mf_sys_event(set_tick(tick), st_meta_event, me_sequence_number, 2, buf);
}

/*
****f*  wrt_functions/mf_text_event
* NAME
*   mf_text_event
*
* SYNOPSIS
*   mf_text_event(tick, type, str))
*
* FUNCTION
*   Insert a text event
*
* INPUTS
*      tick  - the clock tick at which the event occurs.
*      type  - 
*      str   - the string of text
*
********
*/
void mf_text_event(unsigned long tick, short type, char *str)
{
  mf_sys_event(set_tick(tick), st_meta_event, type & 0x0F, strlen(str), str);
}

/*
****f*  wrt_functions/mf_set_tempo
* NAME
*   mf_set_tempo
*
* SYNOPSIS
*   mf_set_tempo(tick, tempo, type))
*
* FUNCTION
*   Insert a tempo change. Tempo can be specified as microseconds 
*   per quarter notes (the midi way) or in bpm.
*
* INPUTS
*      tick  - the clock tick at which the event occurs.
*      tempo - the new tempo 
*      type  - tm_bmp or tm_msecs 
*
********
*/
void mf_set_tempo(unsigned long tick, long tempo, short type)
{
  char buf[4];
  if (tempo <= 0) tempo = 60;
  if (type == tm_bpm) {
    tempo = 60000000 / tempo;
  }
  buf[0] = (tempo >> 16) & 0xFF;
  buf[1] = (tempo >>  8) & 0xFF;
  buf[2] = (tempo      ) & 0xFF;

  mf_sys_event(set_tick(tick), st_meta_event, me_set_tempo, 3, buf);
}

/*
****f*  wrt_functions/mf_key_signature
* NAME
*   mf_key_signature
*
* SYNOPSIS
*   mf_key_signature(tick, key, minmaj)
*
* FUNCTION
*   Insert a tempo change. Tempo can be specified as microseconds 
*   per quarter notes (the midi way) or in bpm.
*
* INPUTS
*      tick   - the clock tick at which the event occurs.
*      key    - the new tempo 
*      minmaj - min (0) or maj (1)
*
********
*/
void mf_key_signature(unsigned long tick, char key, char minmaj)
{
  char buf[2];

  minmaj &= 0x01;
  key >>= minmaj;
  buf[0] = key & 0x7; if (key & 0x08) buf[0] = -buf[0];
  buf[1] = minmaj;
  mf_sys_event(set_tick(tick), st_meta_event, me_key_signature, 2, buf);
}

void mf_time_signature(unsigned long tick, char num, char den,
                                                    char clks, char q32nd)
{
/* Comments are from :
** http://commonmusic.sourceforge.net/doc/dict/midi-time-signature-cls.html
*/

  mf_meter[0] = num;  /* The beats-per-measure (upper number)
                      ** of the time signature. */
  mf_meter[1] = den;  /* The type of beat (lower number)
                      ** of the time signature. */
  mf_meter[2] = clks; /* The number of "MIDI clocks" between metronome clicks.
                      ** There are 24 MIDI clocks in one quarter note.*/
  mf_meter[3] = q32nd;/* he number of notated 32nds in 24 MIDI clocks.
                      ** The default value is 8. */

  mf_sys_event(set_tick(tick), st_meta_event, me_time_signature , 4, mf_meter);
}

void mf_sysex(unsigned long tick, short type, long len, char *data)
{
  mf_sys_event(set_tick(tick), type, -1, len, data);
}

void mf_sequencer_specific(unsigned long tick, long len, char *data)
{
  mf_sys_event(set_tick(tick), st_meta_event, me_sequencer_specific, len, data);
}

/***********/

char mf_note_base[] = {
/*  a   b  c  d  e  f  g  -*/
    9, 11, 0, 2, 4, 5, 7, 0,
};

unsigned char mf_midinote(char note, char acc, char octave)
{
  int np;   

  np = mf_note_base[tolower(note)-'a'];
  switch(acc) {
    case '-': case 'b': np--; break;
    case '+': case '#': np++; break;
  }
  np += 12 * octave;
  while (np < 0) np += 12; 
  while (np > 127) np -= 12; 

  return (unsigned char)np;
}

