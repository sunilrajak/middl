/*
**  (C) Remo Dentato (rdentato@gmail.com)
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

#include "mf_rw.h"

#define MF_SEQ_TRACK_MAX 16
#define MF_SEQ_BUF_STEP  (1024*128)
#define MF_SEQ_EVT_STEP  (1024)

typedef struct {

  unsigned char *buf;
  unsigned long  buf_cnt;
  unsigned long  buf_max;
  
  unsigned long *evt;
  unsigned long  evt_cnt;
  unsigned long  evt_max;
  
  char  *fname;
  short  format;
  short  division;
  short  ntracks;
  short  curtrack;
  
} mf_seq;  

mf_seq *mf_seq_new (char *fname, short division);
int mf_seq_close(mf_seq *ms);
int mf_seq_set_track(mf_seq *ms, int track);
int mf_seq_get_track(mf_seq *ms, int track);
int mf_seq_evt (mf_seq *ms, unsigned long tick, short type, short chan, short data1, short data2);
int mf_seq_sys(mf_seq *ms, unsigned long tick, short type, short aux, long len, unsigned char *data);
