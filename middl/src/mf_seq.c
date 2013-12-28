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

#include "mf_seq.h"

mf_seq *mf_seq_new (char *fname,short format, short ntracks, short division)
{
  int k;
  
  mf_seq *ms;
   
  ms = malloc(sizeof(mf_seq));
   
  if (!ms) { return NULL; }   
   
  ms->buf     = NULL;
  ms->buf_cnt = 0;
  ms->buf_max = 0;
  
  ms->evt     = NULL;
  ms->evt_cnt = 0;
  ms->evt_max = 0;
  
  ms->fname    = fname;
  ms->format   = format;
  ms->ntracks  = ntracks;
  ms->division = division;
  ms->curtrack = 0;
  
  return ms;
}

static void dmp_evts(mf_seq *ms)
{
  unsigned long k;
  unsigned char *p;
  if (!ms) return;
  
  
  for (k=0; k< ms->evt_cnt; k++) {
    p = ms->buf+ms->evt[k];
    
    printf("%02X %02X%02X%02X%02X\n", p[0], p[1],p[2],p[3],p[4]);
  
  }
  
  
}

int mf_seq_close(mf_seq *ms)
{
  if (!ms) return 799;
  
  /*** DO THE SERIOUS STUFF HERE ***/
  
  fprintf(stderr, "Events: %d\n", ms->evt_cnt);
  dmp_evts(ms);
  if (ms->buf) free(ms->buf);
  if (ms->evt) free(ms->evt);
  free(ms);
  return 0;
}

static int chkbuf(mf_seq *ms, unsigned long spc)
{
   unsigned long newsize;
   unsigned char *buf;

   if (!ms) return 739;
   
   newsize = ms->buf_max;
   
   while (spc >= (newsize - ms->buf_cnt)) 
      newsize += MF_SEQ_BUF_STEP;

   if (newsize > ms->buf_max) {
      buf = realloc(ms->buf, newsize);
      if (!buf) return 730; 
      ms->buf = buf;
      ms->buf_max = newsize;
   }
   dbgmsg("CHKBUF: buf:%p cnt:%d max:%d need:%d\n", ms->buf, ms->buf_cnt, ms->buf_max,spc);
   
   return 0;
}

static int chkevt(mf_seq *ms, unsigned long n)
{
   unsigned long newsize;
   unsigned long *evt;

   if (!ms) return 749;
   
   newsize = ms->evt_max;
   while (n >= (newsize - ms->evt_cnt)) 
      newsize += MF_SEQ_EVT_STEP;
      
   if (newsize > ms->evt_max) {
      evt = realloc(ms->evt, newsize * sizeof(unsigned long));
      if (!evt) return 740; 
      ms->evt = evt;
      ms->evt_max = newsize;
   }
   
   dbgmsg("CHKEVT: evt:%p cnt:%d max:%d need:%d\n", ms->evt, ms->evt_cnt, ms->evt_max,n);
   return 0;
}

int mf_seq_set_track(mf_seq *ms, int track)
{
  if (!ms) return 719;
  ms->curtrack = track & 0xFF;
  return 0;
}

int mf_seq_get_track(mf_seq *ms, int track)
{
  if (!ms) return 789;
  return ms->curtrack;
}

#define add_evt(ms)    (ms->evt[ms->evt_cnt++] = ms->buf_cnt)
#define add_byte(ms,b) (ms->buf[ms->buf_cnt++] = (unsigned char)(b))

static void add_data(mf_seq *ms, unsigned long l, unsigned char *d)
{  if (ms) while (l--) add_byte(ms,*d++); }

static void add_ulong(mf_seq *ms, unsigned long l)
{ 
  add_byte(ms,(l >>24) & 0xFF);
  add_byte(ms,(l >>16) & 0xFF);
  add_byte(ms,(l >> 8) & 0xFF);
  add_byte(ms,(l     ) & 0xFF);
}

static void add_str(mf_seq *ms, char *s)
{ 
  add_data(ms,strlen(s),(unsigned char *)s);
}

int mf_seq_evt (mf_seq *ms, unsigned long tick, short type, short chan, short data1, short data2)
{
  int ret = 0;
  
  type &= 0xF0;
  
  if (!ms)  ret = 759;
  if (!ret) ret = chkbuf(ms,32);
  if (!ret) ret = chkevt(ms,1);
  if (!ret) ret = type == 0xF0 ? 758 : 0;
  
  if (!ret) {
    add_evt(ms);
    
    add_byte(ms, ms->curtrack);
    add_ulong(ms,tick);
                             
    add_byte(ms, (type |  (chan & 0x0F)));
    add_byte(ms, data1 & 0xFF);
    add_byte(ms, data2 & 0xFF);
  }
  return ret;
}

int mf_seq_sys(mf_seq *ms, unsigned long tick, short type, short aux,
                                               long len, unsigned char *data)
{
  int ret;
  
  if (!ms)  ret = 779;
  if (!ret) ret = chkbuf(ms,32+len);
  if (!ret) ret = chkevt(ms,1);
  if (!ret) ret = type == 0xF0 ? 0 : 778;

  if (!ret) {  
    add_evt(ms);
    
    add_byte(ms, ms->curtrack);
    add_ulong(ms,tick);

    add_byte(ms,type);
    add_byte(ms,aux);
    
    add_ulong(ms,len);
    add_data(ms,len,data);
  }
  
  return ret;
}


#ifdef MF_SEQ_TEST

int main(int argc, char *argv[])
{
  mf_seq *m;

  m = mf_seq_new("ss.mid",1, 2, 384);
  
  if (m) {
    mf_seq_set_track(m, 2);
    mf_seq_evt (m, 0, st_note_on, 60, 90);
    mf_seq_evt (m, 192, st_note_off, 60, 0);
    mf_seq_set_track(m, 1);
    mf_seq_evt (m, 0, st_note_on, 64, 90);
    mf_seq_evt (m, 255, st_note_off, 64, 0);
    mf_seq_close(m);
  }
  
}

#endif
