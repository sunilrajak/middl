
#include "mf_seq.h"


/*

   =macro-name{jsjakdjks}
   $macro-name

*/



static int is_idchar(unsigned char c)
{
  return (c && (('A' <= c && c <= 'Z') || 
                ('a' <= c && c <= 'z') ||
                (c == '-')) );
}


static int is_macro_def(unsigned char *q)
{
  unsigned char *p=q;
   while (is_idchar(*q))  q++;             
   _dbgmsg("ismacro: [%s][%s]\n",p,q);
   return (*q == '{');
}


typedef struct {
  unsigned char **macro_vec;
  unsigned long   macro_cnt;
  unsigned long   macro_max;
} macro_defs;

#define MACRO_INCR 512

static int chkmacro(macro_defs *m, unsigned long n)
{
  unsigned long newmax;
  unsigned char **vec;
  
  if (!m) return 0;
  
  newmax = m->macro_max;
  while (n >= (newmax - m->macro_cnt))
    newmax += MACRO_INCR;
  
  if (newmax > m->macro_max) {
    vec = realloc(m->macro_vec, newmax * sizeof(unsigned char *));
    if (!vec) return 0;
    m->macro_vec = vec;
    m->macro_max = newmax;    
  }  
  return (m->macro_vec != NULL);
}

static unsigned char *addmacro(macro_defs *m,unsigned char *p)
{
  if (chkmacro(m,1)) {
    m->macro_vec[m->macro_cnt++] = p;
    while (*p && *p != '}') p++;
  }
  return p;
}

static unsigned char *skip_identifier(unsigned char *q)
{
   while (is_idchar(*q)) q++;             
   return q-1;
}

static int id_cmp(unsigned char *a,unsigned char *b)
{
  while (is_idchar(*a) && is_idchar(*b) && (*a == *b)) {a++; b++;}
  
  if (is_idchar(*a) || is_idchar(*b)) return (*a - *b);
  return 0;
}

static unsigned char *getmacro(macro_defs *m,unsigned char *p)
{
   int k;
   _dbgmsg("getmacro: %s\n",p);
   for (k=0; k< m->macro_cnt; k++) {
     if (id_cmp(m->macro_vec[k] , p) == 0) {
       p = m->macro_vec[k];
       while (*p && *p != '{') p++;
       return p;       
     }
   }
   return NULL;
}


typedef struct {
  unsigned char *buf;
  unsigned long  buf_cnt;
  unsigned long  buf_max;
} charbuf;

#define BUF_INCR (128*1024)

static int chkbuf(charbuf *b, unsigned long n)
{
  unsigned long newmax;
  unsigned char *buf;
  
  if (!b) return 0;
  
  newmax = b->buf_max;
  while (n >= (newmax - b->buf_cnt))
    newmax += BUF_INCR;
  
  if (newmax > b->buf_max) {
    buf = realloc(b->buf, newmax);
    if (!buf) return 0;
    b->buf = buf;
    b->buf_max = newmax;    
  }  
  return (b->buf != NULL);
}


static unsigned long addchar(charbuf *b, unsigned char c)
{
  if (chkbuf(b,2)) {
    b->buf[b->buf_cnt++] = c;
    b->buf[b->buf_cnt] = '\0';
    return b->buf_cnt;
  }
  return 0;
}

#define MAX_STK_TOP 128

#define RETURN_ERR(x) do {if (buf.buf) free(buf.buf); *err = (x); return p;} while(0)

static unsigned char *demacro(unsigned char *inbuf, int *err)
{
  macro_defs macros;
  charbuf buf;  
  
  unsigned char *p = inbuf;
  unsigned char *q;

  unsigned char *stk[MAX_STK_TOP];
  int stk_top = 0;
  
  macros.macro_vec = NULL;
  macros.macro_cnt = 0;
  macros.macro_max = 0;
  *err = 0;
 
  buf.buf = NULL;
  buf.buf_cnt = 0;
  buf.buf_max = 0;
 
  while (*p) {

    if (*p == '=') {
      if (is_macro_def(p+1)) {
        p = addmacro(&macros, p+1);
      }
      else addchar(&buf,*p);
    } else if (*p == '}') {
      if (stk_top == 0) RETURN_ERR(901); /* stray close brace */
      p = stk[--stk_top];
    } else if (*p == '{') {
      RETURN_ERR(903);                   /* stray open brace */
    } else if (*p == '$') {
      if (p[1] == '$') {
         addchar(&buf,'$'); p++;
      }
      else {
        q = getmacro(&macros, p+1);
        if (q) {
          _dbgmsg("foundmacro: %s\n",q);
          if (stk_top == MAX_STK_TOP) RETURN_ERR(902); /* too many levels (infinite loop?) */
          stk[stk_top++] = skip_identifier(p+1);
          p = q;
        }
        else RETURN_ERR(904); /* undefined macro */
      }
    }
    else  addchar(&buf,*p);

    if (*p) p++;    
  }  
  addchar(&buf,'\n');
  
  #if 0
  dbgmsg("macros: %d\n", macros.macro_cnt);
  {
    int k;
    for (k=0;k<macros.macro_cnt;k++) {
      dbgmsg("%s\n",macros.macro_vec[k]);
    }
  }
  #endif
  return buf.buf; 
}

#define MAX_TRACKS  20
#define MAX_REPT    32
#define DUR_INFINITE 0xFFFFFFFF

typedef struct {
  mf_seq *ms;
  unsigned char *buf;
  unsigned char *ptr;
  short ppqn;
  short track;
  short err;
  
  short rpt;
  unsigned long  rpt_pos[MAX_REPT];
           short rpt_cnt[MAX_REPT];
           
  unsigned long  tick[MAX_TRACKS];
  unsigned long   dur[MAX_TRACKS];
  unsigned char  chan[MAX_TRACKS];
  unsigned char  note[MAX_TRACKS];
  unsigned char  inst[MAX_TRACKS];
  unsigned char   vol[MAX_TRACKS];
} trk_data;


static unsigned char *mnotes = "\x09\x0B\x00\x02\x04\x05\x07";


static unsigned long notelen(trk_data *trks)
{
  unsigned char c;
  unsigned long  dur = 0;
  unsigned short mul = 1;
  unsigned short div = 1;

  if (*(trks->ptr) != ':') return trks->dur[trks->track];
  
  trks->ptr++;
  
  c = *(trks->ptr++);
  if ('0' <= c && c <= '9')  { mul = c-'0'; }
  else trks->ptr--;
  
  c = *(trks->ptr++);
  switch ( c ) {
    case 'w' : dur = trks->ppqn * 4; break;
    case 'h' : dur = trks->ppqn * 2; break;
    case 'q' : dur = trks->ppqn    ; break;
    case 'e' : dur = trks->ppqn / 2; break;
    case 's' : dur = trks->ppqn / 4; break;
    case 't' : dur = trks->ppqn / 8; break;
    default  : trks->ptr--;
  }
  
  c = *(trks->ptr++);
  if ('0' <= c && c <= '9')  { div = c-'0'; }
  else trks->ptr--;

  dur = trks->ppqn * mul;
  if (div > 0) dur = dur / div;
  else dur = DUR_INFINITE;
  
  trks->dur[trks->track] = dur;
  return dur;
}

static void note(trk_data *trks)
{
  unsigned char c;
  short n;
  short cur_oct = 0;
  short tmp_oct = 0;
  unsigned long dur;
  
  c = *(trks->ptr++);
  if ('A' <= c && c <= 'G')       n = mnotes[c-'A'];
  else if ('a' <= c && c <= 'g')  n = mnotes[c-'a']-1;
  else {trks->ptr--; return ;}
  
  c = *(trks->ptr++);
  if (c == 'b')      { n--; }
  else if (c == '#') { n++; }
  else trks->ptr--;

  cur_oct = trks->note[trks->track] / 12;
  
  c = *(trks->ptr++);
  if ('0' <= c && c <= '9')  { cur_oct = c-'0'+1; }
  else if ( c == 'N')        { cur_oct = 0; }
  else if ( c == '\'')       { tmp_oct = 1; }
  else if ( c == ',')        { tmp_oct = -1; }
  else trks->ptr--;

  n += (cur_oct + tmp_oct) * 12;   
  while (n < 0)    { n += 12; } 
  while (n > 127)  { n -= 12; } 
  
  trks->note[trks->track] = n;
  
  dur = notelen(trks);
  
  if (dur > 0) {
    mf_seq_evt(trks->ms, trks->tick[trks->track], st_note_on, trks->chan[trks->track],
                                          trks->note[trks->track], trks->vol[trks->track]);
  }
  _dbgmsg("TICK: %d DUR: %d\n",  trks->tick[trks->track], dur);
  if (dur != DUR_INFINITE) {
    trks->tick[trks->track] += dur;
    mf_seq_evt(trks->ms, trks->tick[trks->track], st_note_off, trks->chan[trks->track],
                                          trks->note[trks->track], 0);
  }  
}


static void rest(trk_data *trks)
{
  unsigned char c;
  short n;
  short cur_oct = 0;
  short tmp_oct = 0;
  unsigned long dur;
  
  c = *(trks->ptr++);
 
  dur = notelen(trks); 
   
  if (dur == 0 || c == 'r') {
    mf_seq_evt(trks->ms, trks->tick[trks->track], st_note_off, trks->chan[trks->track],
                                          trks->note[trks->track], 0);
  }
  
  if (dur != DUR_INFINITE) {
    trks->tick[trks->track] += dur;
  }  
}


static void parse(trk_data *trks)
{
   unsigned char c;
  
   while ((c = *(trks->ptr))) {
     if (('A' <= c && c <= 'G') || ('a' <= c && c <= 'g') ) {
       note(trks);
     }
     else if ( c == 'r' || c == 'R' ) {
       rest(trks);
     }
     else trks->ptr++;
   }
}

static int tomidi(char *fname, short division, unsigned char *s)
{
  int err = 0;
  trk_data tracks;
  int k;
  
  if (!fname)    return 911;
  if (!s || !*s) return 912;
  
  for (k = 0; k < MAX_TRACKS; k++) {
    tracks.dur [k] = division;
    tracks.tick[k] = 0;
    tracks.chan[k] = 0;
    tracks.inst[k] = 0;
    tracks.note[k] = 60;
    tracks.vol [k] = 90;
  }

  tracks.track = 0;
  tracks.ppqn = division;
  tracks.buf  = s;
  tracks.ptr  = tracks.buf;
  tracks.err  = 0;
  tracks.rpt  = 0;

  tracks.ms = mf_seq_new(fname, division);

  if (tracks.ms) {  
    parse(&tracks);
    err = mf_seq_close(tracks.ms);
  } 
  else err = 910; /* Unable to open midifile */

  err += tracks.err * 1000;

  return err;
}

int mf_score(char *fname, short division, unsigned char *score)
{
  unsigned char *p;
  int err;

  p = demacro(score, &err);

  if (!err) {
    err = tomidi(fname, division, p);
    free(p);
  }

  return err;
}

#ifdef MF_SCORE_TEST

int main(int argc, char *argv[])
{
  unsigned char *s;
  int err;
  
  s = "C D:q3 E F";
  
  err = mf_score("sc.mid",384, s);
  
  if (err) printf("ERR: %d\n",err);
  
  return(err);
}

#endif 

