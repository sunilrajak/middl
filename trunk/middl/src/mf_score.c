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
#include "mf_chords.h"
#include <setjmp.h>

#if 1 /*  **** MACRO EXPANSION */

/*
   $macro-name{jsjakdjks}
   $macro-name
*/

static int is_idchar(unsigned char c)
{
  return (c && (('A' <= c && c <= 'Z') || 
                ('a' <= c && c <= 'z') ||
                (c == '_')) );
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
   for (k=0; k < m->macro_cnt; k++) {
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

static unsigned long addlong(charbuf *b, unsigned long n)
{
  char data[16];
  char *p;
  sprintf(data,"%u",n);
  p=data;
  while (*p) addchar(b,*p++);
  addchar(b,' ');
  return 0;
}

#define MAX_STK_TOP 128

#define RETURN_ERR(x) do {if (buf.buf) free(buf.buf); *err = line * 1000 + (x); return p;} while(0)

#define addline(b,x) do { addchar(b,'\t'); addlong(b, x); } while (0)

static unsigned char *demacro(unsigned char *inbuf, int *err)
{
  macro_defs macros;
  charbuf buf;  
  
  unsigned long line = 0;
  
  unsigned char *p = inbuf;
  unsigned char *q;

  unsigned char *stk[MAX_STK_TOP];
  unsigned long  stk_ln[MAX_STK_TOP];
  int stk_top = 0;
  
  macros.macro_vec = NULL;
  macros.macro_cnt = 0;
  macros.macro_max = 0;
  *err = 0;
 
  buf.buf = NULL;
  buf.buf_cnt = 0;
  buf.buf_max = 0;
  
  addchar(&buf,'\n');
  addline(&buf,++line); 
 
  while (*p) {
    if (*p == '}') {
      if (stk_top == 0) RETURN_ERR(901); /* stray close brace */
      stk_top--;
      p = stk[stk_top];
      line = stk_ln[stk_top];
      addchar(&buf,'\n'); addline(&buf, line); 
    } else if (*p == '\t') {
      addchar(&buf,' ');
    } else if (*p == '\r') {
      if (p[1] != '\n') {
        addchar(&buf,'\n'); 
        if (stk_top == 0) {addline(&buf, ++line); }
      }
    } else if (*p == '\n') {
      addchar(&buf,'\n'); 
      if (stk_top == 0) { addline(&buf, ++line);  }
    } else if (*p == '{') {
      RETURN_ERR(903);                   /* stray open brace */
    } else if (*p == ')' && p[1] == '&') { /*  transform ")&" in "&)"  */
      addchar(&buf,'&');addchar(&buf,')'); p++;
    } else if (*p == '$') {
      if (p[1] == '$') {
         addchar(&buf,'$'); p++;
      }
      else if (isdigit(p[1])) {
         if (p[1] == '0') RETURN_ERR(904);
         addchar(&buf,'$'); addchar(&buf,p[1]); 
         p++;
      }
      else if (is_macro_def(p+1)) {
        p++;
        if (chkmacro(&macros,1)) {
          macros.macro_vec[macros.macro_cnt++] = p;
          while (*p && *p != '}') {
            if (*p == '\n') line++;
            if (*p == '\r' &&  p[1] != '\n') line++;
            p++;
          }
        }
      }
      else {
        q = getmacro(&macros, p+1);
        if (q) {
          _dbgmsg("foundmacro: %s\n",q);
          if (stk_top == MAX_STK_TOP) RETURN_ERR(902); /* too many levels (infinite loop?) */
          stk[stk_top] = skip_identifier(p+1);
          stk_ln[stk_top] = line;
          p = q;
          stk_top++;
        }
        else RETURN_ERR(904); /* undefined macro */
      }
    }
    else if (*p == '%') { /* strip Comment */
      while (*p && *p != '\r' && *p != '\n') p++;
      if (*p == '\r' && p[1] == '\n') p++;
      addchar(&buf,'\n'); addline(&buf, ++line); 
    }
    else  addchar(&buf,*p);

    if (*p) p++;    
  }  
  addchar(&buf,'\n');
  addline(&buf, ++line); 
  
  #if 0
  fprintf(stderr,"%s",buf.buf);
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

#endif /* */ 

#if 1 /*  **** PARSING */

#define MAX_TRACKS  20
#define MAX_REPT    32
#define MAX_TRGT    10
#define MAX_SCALE   16
#define MAX_CHORDN  16
#define DUR_INFINITE 0xFFFFFFFF

typedef struct { /* trk_data */
  mf_seq *ms;
  unsigned char *buf;
  unsigned char *ptr;
  unsigned long line;
  jmp_buf errjmp;
  short ppqn;
  
  short track;
  short err;
  
  short num;
  short den;
  
  short rpt_top;
  unsigned char *rpt_pos[MAX_REPT];
           short rpt_cnt[MAX_REPT];

  unsigned long  trgt[MAX_TRGT];
           
  unsigned long  trgt_r[MAX_TRGT];
           short trgt_n;

  unsigned char  scale[MAX_SCALE];
  unsigned short scale_n;
  
  unsigned long    tick[MAX_TRACKS];
  unsigned long     dur[MAX_TRACKS];
  unsigned char   notes[MAX_TRACKS][MAX_CHORDN];
  unsigned char chord_n[MAX_TRACKS];
  unsigned char    chan[MAX_TRACKS];
  unsigned char    inst[MAX_TRACKS];
  unsigned char     vol[MAX_TRACKS];
} trk_data;

static unsigned char *mnotes = "\x09\x0B\x00\x02\x04\x05\x07";

static short in_127(short n)
{
  while (n < 0)    { n += 12; } 
  while (n > 127)  { n -= 12; } 
  return n;
}

static unsigned char ch_current(trk_data *trks)
{ return *(trks->ptr); }

static unsigned char ch_next(trk_data *trks)
{ trks->ptr++; return *(trks->ptr); }

static unsigned char *ch_curptr(trk_data *trks)
{ return trks->ptr; }

static unsigned char *ch_skip(trk_data *trks)
{ return trks->ptr++; }

static unsigned char ch_get(trk_data *trks)
{ return *(trks->ptr++); }

static unsigned char ch_unget(trk_data *trks)
{ if (trks->ptr > trks->buf) trks->ptr--; return *(trks->ptr); }

static unsigned char ch_cur(trk_data *trks)
{ return *(trks->ptr); }

static unsigned char ch_ahead(trk_data *trks)
{ return *(trks->ptr + 1); }

#define ispitchU(c) ('A' <= (c) && (c) <= 'G')
#define ispitchL(c) ('a' <= (c) && (c) <= 'g')
#define isdig(c)    ('0' <= (c) && (c) <= '9')

#define ispitch(c) (ispitchU(c) || ispitchL(c))

static unsigned long notelen(trk_data *trks)
{
  unsigned char c;
  unsigned long  tmp = 0;
  unsigned long  dur = 0;
  unsigned short mul = 0;
  unsigned short div = 0;

  c = ch_get(trks);
  dur = trks->dur[trks->track];
  if ( c == '/') {
    
    c = ch_get(trks);
    
    if (c == '+') {
      dur = DUR_INFINITE;
      c = ch_get(trks);
    }
    else if (c == '-') {
      dur = 0;
      c = ch_get(trks);
    }
    else {
      dur = (trks->ppqn * 4);
      
      switch (c) {
        case 'w' : dur = (trks->ppqn * 4) ; c = ch_get(trks); break;
        case 'h' : dur = (trks->ppqn * 2) ; c = ch_get(trks); break;
        case 'q' : dur = (trks->ppqn    ) ; c = ch_get(trks); break;
        case 'e' : dur = (trks->ppqn / 2) ; c = ch_get(trks); break;
        case 's' : dur = (trks->ppqn / 4) ; c = ch_get(trks); break;
        case 't' : dur = (trks->ppqn / 8) ; c = ch_get(trks); break;
      }
      
      while ('0' <= c && c <= '9') {
        div = div * 10 + (c -'0');  
        c = ch_get(trks);
      }
      if (div == 0) div = 1;
      
      if (c == '*') {
        c = ch_get(trks);
        while ('0' <= c && c <= '9') {
          mul = mul * 10 + (c -'0');  
          c = ch_get(trks);
        }
      }
      if (mul == 0) mul = 1;
      
      dur = dur * mul;
      dur = dur / div;
      
      trks->dur[trks->track] = dur;
    }
  }
  tmp = dur;
  while ( c == ' ' || c == '\t' ||  c == '=' ) {
    if (c == '=') dur += tmp;
    c = ch_get(trks) ;
  }
  
  ch_unget(trks);
  
  return dur;
}

static int getnum(trk_data *trks)
{
  int n = 0;
  unsigned char c;
  short d = 1;

  c = ch_cur(trks);
  
  if ((c == '+' || c == '-')  && !isdigit(ch_ahead(trks))) return 0;
   
  if (c == '+')      { c = ch_get(trks);         }
  else if (c == '-') { c = ch_get(trks); d = -1; }
  
  while ((c = ch_cur(trks)) && '0' <= c && c <= '9') {
    n = n * 10 + (c - '0');
    c = ch_get(trks);
  }
   
  return n * d; 
}

static void getchord(trk_data *trks, short root, int play)
{
  unsigned long dur;
  unsigned char c;
  char *q;
  unsigned char inv=0;
  short k;
  short n;
  
  c = ch_get(trks); /* skip '[' */
  _dbgmsg("GCH1: %s\n",ch_curptr(trks));
  
  q = mf_chordbyname(ch_curptr(trks));
  
  _dbgmsg("GCH2: %p\n",q);
  
  /* look for inversion */
  while ( (c = ch_cur(trks)) && c != ']' && c != ':') c = ch_get(trks);
  if (c == ':') {
    if (isdigit(ch_ahead(trks))) {
      c = ch_get(trks);
      inv = c - '0';
    }
  }
  while ( (c = ch_cur(trks)) && c != ']')
    c = ch_get(trks);
  if (c) c = ch_get(trks);;
  
  if (q) {        /* chord found */
    if (root < 0) root = trks->notes[trks->track][0];
   
    trks->notes[trks->track][1] = root;
    for(k=2; *q && k < MAX_CHORDN; q++, k++) {
      n = trks->notes[trks->track][k-1] + *q;
      n = in_127(n);
      trks->notes[trks->track][k] = n;
    }
    trks->chord_n[trks->track] = k-1;
    dur = notelen(trks);
    if (play) {
      n = trks->chord_n[trks->track];
      if (dur != 0) {
        for (k=1; k<=n; k++) {
          trks->err = mf_seq_evt(trks->ms, trks->tick[trks->track], st_note_on, trks->chan[trks->track],
                                                 trks->notes[trks->track][k], trks->vol[trks->track]);
        }
      }
      if (dur != DUR_INFINITE) {
        trks->tick[trks->track] += dur;
        for (k=1; k<=n; k++) {
          trks->err = mf_seq_evt(trks->ms, trks->tick[trks->track], st_note_off, trks->chan[trks->track],
                                                 trks->notes[trks->track][k], 0);
        }  
      }
    }
  }
  
}

static void getnote(trk_data *trks,int play)
{
  unsigned char c;
  unsigned char istmp = 0;
  short n;
  short cur_oct = 0;
  short tmp_oct = 0;
  unsigned long dur;
  
  c = ch_get(trks);
  
  if (isdigit(c)) {
    ch_unget(trks);
    n = getnum(trks) + trks->notes[trks->track][0];
    istmp = 1;
  }
  else if (c == '[') {
    n = trks->notes[trks->track][0];
    ch_unget(trks);
  }
  else if (c == 'n' || c == 'N') {
    istmp = (c == 'n');
    _dbgmsg("n: %p [%c]\n",ch_curptr(trks),c);
    c = ch_cur(trks) ;
    n = ( c == '+' || c == '-') ? trks->notes[trks->track][0] : 0;
    n += getnum(trks);
  }
  else {
    if ('A' <= c && c <= 'G')       n = mnotes[c-'A'];
    else if ('a' <= c && c <= 'g')  n = mnotes[c-'a'];
    else if (c == 'X')              n = trks->notes[trks->track][0] % 12;
    else if (c == 'x')            { n = trks->notes[trks->track][0] % 12; istmp = 1; }
    else if (c == '$' && isdigit(ch_cur(trks)))
                                    n = trks->scale[(ch_get(trks) - '1') % trks->scale_n];
    else {ch_unget(trks); return ;}
    
    c = ch_get(trks);
    if (c == 'b')      { n--; while ((c = ch_cur(trks)) == 'b')  {n--; c = ch_get(trks);} }
    else if (c == '#') { n++; while ((c = ch_cur(trks)) == '#')  {n++; c = ch_get(trks);} }
    else ch_unget(trks);
    
    cur_oct = trks->notes[trks->track][0] / 12;
    c = ch_get(trks);
    if ('0' <= c && c <= '9')  { cur_oct = c-'0'+1; }
    else if ( c == 'N')        { cur_oct = 0; }
    else if ( c == '\'')       { tmp_oct++; while ((c = ch_cur(trks)) == '\'')  {tmp_oct++; c = ch_get(trks);} }
    else if ( c == ',')        { tmp_oct--; while ((c = ch_cur(trks)) == ',')   {tmp_oct--; c = ch_get(trks);} }
    else ch_unget(trks);
    
    n += cur_oct * 12;   
  }

  n = in_127(n);

  if (!istmp) trks->notes[trks->track][0] = n;
  
  n += tmp_oct * 12;

  n = in_127(n);

  if (ch_cur(trks) == '[') {
    getchord(trks, n, play);
  }
  else {
    dur = notelen(trks);
    
    if (play) {
      if (dur != 0) {
        trks->err = mf_seq_evt(trks->ms, trks->tick[trks->track], st_note_on, trks->chan[trks->track],
                                               n, trks->vol[trks->track]);
      }
      _dbgmsg("TICK: %d DUR: %d\n",  trks->tick[trks->track], dur);
      if (dur != DUR_INFINITE) {
        trks->tick[trks->track] += dur;
        trks->err = mf_seq_evt(trks->ms, trks->tick[trks->track], st_note_off, trks->chan[trks->track],
                                               n, 0);
      }  
    }
  }
}

static void rest(trk_data *trks)
{
  unsigned char c;
  unsigned long dur;
  unsigned short mul = 0;
  
  if ((c = ch_cur(trks)) == '-') {
    for(;;) {
     c = ch_get(trks);
           if (c == '-' || c == '=')  { mul++; }
      else if (c != '\t' && c != ' ') { ch_unget(trks); break; }
    }
    dur = trks->dur[trks->track] * mul;
  }
  else {
    c = ch_get(trks);
    dur = notelen(trks);
   
    if (dur == 0 || c == 'r') {
      trks->err = mf_seq_evt(trks->ms, trks->tick[trks->track], st_note_off, trks->chan[trks->track],
                                            trks->notes[trks->track][0], 0);
    }
  }
  
  if (dur != DUR_INFINITE) {  trks->tick[trks->track] += dur; }  
}

static void chgtrack(trk_data *trks)
{
  unsigned char  c;
  unsigned short trk;
  
  c = ch_get(trks); /* skip '|' */
  
  if (isdigit(ch_cur(trks)))  trk = getnum(trks);
  else trk = trks->track+1; 
  
  if (trk < 0) trk = 0;
  else if (MAX_TRACKS <= trk) trk = MAX_TRACKS-1;
  
  trks->track = trk;
  mf_seq_set_track(trks->ms, trk);
  
}

#if 1/* *** REPEAT */

static void back(trk_data *trks)
{
  unsigned char  c;
  
  c = ch_get(trks);
  
  c = ch_cur(trks);
  if ( isdigit(c)) {
    trks->tick[trks->track] = trks->trgt[c-'0'];
    c = ch_get(trks);
  }
  else if (trks->dur[trks->track] != DUR_INFINITE) {
    if (trks->tick[trks->track] < trks->dur[trks->track])
      trks->tick[trks->track] = 0;
    else 
      trks->tick[trks->track] -= trks->dur[trks->track];
  }
  _dbgmsg("BACK TO: %d\n", trks->tick[trks->track]);
}

static void rtarget(trk_data *trks)
{
  unsigned char  c;
  
  c = ch_get(trks);
  c = ch_get(trks);

  if (isdigit(c)) {
    trks->trgt[c-'0'] = trks->tick[trks->track];
  }
  else if (c == '(') {
    if (trks->trgt_n < MAX_TRGT) {
      trks->trgt_r[trks->trgt_n++] = trks->tick[trks->track];
      _dbgmsg("RTRGT: %d\n",trks->trgt_r[trks->trgt_n-1]);
    }
  }
  else if (c == ')') {
    if (trks->trgt_n > 0) trks->trgt_n--;
  }
  else if (c == '<') {
    ch_unget(trks) ; back(trks);
  }
  else if (c == '&') {
    if (trks->trgt_n > 0) {
      trks->tick[trks->track] = trks->trgt_r[trks->trgt_n-1];
      _dbgmsg("RGOTO: %d\n",trks->tick[trks->track]);
    }
    else {ch_unget(trks) ; back(trks);}
  }
  else {trks->ptr -=2 ; back(trks);}
  
}
#endif /**/

static void skiparg(trk_data *trks)
{
  unsigned char  c;
   c = ch_get(trks);
   if (c == '"') {
      c = ch_get(trks);
      while (c && c != '"') {
        if (c == '\\' && ch_cur(trks)) c = ch_get(trks);
        c = ch_get(trks);
      }
      if (c) { c = ch_get(trks); }
   }
   else {
     while (c && !isspace(c)) c = ch_get(trks); 
   }
   
}

static void skipctrl(trk_data *trks)
{
  unsigned char  c;
  c = ch_get(trks);
  while (c && c != ':') c = ch_get(trks);
}

static setvol(trk_data *trks)
{
}

static setvel(trk_data *trks)
{
  short n = 0;
  skipctrl(trks);
  n = getnum(trks);
  trks->vol[trks->track] = n;  
}

static void setmeter(trk_data *trks)
{
  short n = 0;
  short d = 2;
  unsigned char c;  
  unsigned char data[4];
  
  skipctrl(trks);
  n = getnum(trks);  
  if ( n == 0)  n=4;

  if (ch_cur(trks) == '/') {
    c = ch_get(trks);
    d = getnum(trks);  
    switch (d) {
      case   2: d = 1; break;
      case   4: d = 2; break;
      case   8: d = 3; break;
      case  16: d = 4; break;
      case  32: d = 5; break;
      default : d = 2; 
    }
  }

  trks->num = n;
  trks->den = d;
  
  data[0] = n; data[1]= d; data[2] = 24; data[3] = 8;
  
  mf_seq_set_track(trks->ms, 0);
  trks->err = mf_seq_sys(trks->ms,  trks->tick[trks->track], st_meta_event, me_time_signature, 4, data);
  mf_seq_set_track(trks->ms, trks->track);
 
}

static void setbpm(trk_data *trks)
{
  unsigned char data[4];
  unsigned long tempo;
  
  skipctrl(trks);
  tempo = getnum(trks);  
  
  if ( tempo <= 0)  tempo=90;

  _dbgmsg("TEMPO: %d ",tempo);
  tempo = 60000000  / tempo;
  _dbgmsg("  %d (us)\n",tempo);

  data[0] = (tempo >> 16) & 0xFF;
  data[1] = (tempo >>  8) & 0xFF;
  data[2] = (tempo      ) & 0xFF;

  mf_seq_set_track(trks->ms, 0);
  trks->err = mf_seq_sys(trks->ms,  trks->tick[trks->track], st_meta_event, me_set_tempo, 3, data);
  mf_seq_set_track(trks->ms, trks->track);
}

#if 1 /* *** SET KEY AND SCALE */

static unsigned char *scales[] = {
/*  name minor? scale
     /   /  ____/\________________
    / \ / \/                      \   */
   "aeo\001\002\001\002\002\001\002", 
   "dor\000\002\001\002\002\002\001",
   "har\000\002\001\002\002\001\003",
   "ion\000\002\002\001\002\002\002",
   "loc\000\001\002\002\001\002\002",
   "lyd\000\002\002\002\001\002\002",
   "maj\000\002\002\001\002\002\002",
   "min\001\002\001\002\002\001\002",
   "mix\000\002\002\001\002\002\001",
   "nmj\000\001\002\002\002\002\002",
   "nmn\000\001\002\002\002\001\003",
   "phr\000\001\002\002\002\001\002",
   "pmj\000\002\002\003\002"        ,
   "pmn\000\003\002\002\003"        
};


/* see  http://www.musictheory.net/lessons/25 */
                               /* A   B   C   D   E   F   G */
static unsigned char *keyacc = "\004\006\001\003\005\000\002";
/*          maj        #         10   12  7   9   11   6   8
                                 -2   0      -3   -1      -4
                                 
            min        #          7   9   4   6    8   3   5        
                                     -3           -4                

            maj        b         -4  -2  -7  -5   -3  -8  -6
                                                       4
                                                      
            min        b         -7  -5 -10  -8   -6 -11  -9
                                          2   4        1   3
                                     
*/

static int sc_cmp(const void *a, const void *b)
{
  _dbgmsg("SC_CMP: %.3s %.3s\n",*(char**)a, *(char **)b);
  return strncmp(*(char**)a, *(char **)b,3) ;
}

static char *getscale(trk_data *trks)
{
  unsigned char **q;
  unsigned char *s = ch_curptr(trks);

  if (!s || !s[0] || !s[1] || !s[2]) return NULL;
  
  q = bsearch(&s, scales, sizeof(scales)/sizeof(scales[0]),
                          sizeof(scales[0]), sc_cmp);
  if (q) {
    ch_skip(trks);ch_skip(trks);ch_skip(trks);
  }
  else {
    s = "maj";
    q = bsearch(&s, scales, sizeof(scales)/sizeof(scales[0]),
                            sizeof(scales[0]), sc_cmp);
  }
  return *q;
}

unsigned char *keyroot =
    /* Cb  Gb  Db  Ab  Eb  Bb  F   C   G   D   A   E   B  F#   C# */
     "\x0B\x06\x01\x08\x03\x0A\x06\x00\x07\x02\x09\x04\x0B\x06\x01"
     "\x08\x03\x0A\x05\x00\x07\x02\x09\x04\x0B\x06\x01\x08\x03\x0A";
    /* Ab  Eb  Bb  F   C   G   D   A   E   B   F#  C#  G#  D#  A# */
    /*-7  -6  -5  -4  -3  -2  -1   0   1   2   3   4   5   6   7  */
    
#define NO_KEY 100

static setkey(trk_data *trks)
{ 
  /*  @key:+3:min  @key:Db:min */
  short n = NO_KEY;
  short m = 0;
  short r = -1;
  short a = 0;
  short k = 0;
  unsigned char data[4];
  unsigned char *p = NULL;
  unsigned char *q = NULL;
  unsigned char c;
  
  skipctrl(trks);
  
  c = ch_cur(trks);
  
  if (c == '+' || c == '-' || isdigit(c)) {
    n = getnum(trks);  
  }
  else if (ispitchU(c)) {
    r = ch_get(trks) - 'A';
    switch(ch_get(trks)) {
      case 'b' : a = -1; break;
      case '#' : a =  1; break;
      default  : ch_unget(trks); 
    }
  }

  c = ch_cur(trks);
  if (c == ':') {
    ch_skip(trks);
    q = getscale(trks);
  }
  
  m = q[3];
  
  if (n == NO_KEY) {
    n = keyacc[r]-1;
    n += a * 7;
    n -= m * 3;
  } 
  
  while (n < -7) n += 12;
  while (n >  7) n -= 12;

  _dbgmsg("KEY %.3s %d %d\n",q,m,n);

  trks->scale[0] = keyroot[(7+n)+m*15];
  
  for (k=1, q += 4; *q && k < MAX_SCALE; k++,q++) {
    trks->scale[k] = trks->scale[k-1] + *q;
  }
  trks->scale_n = k+1;
  
  _dbgmsg("Key: r=%d a=%d m=%d n=%d p=[%s]\n",r,a,m,n,p);

  data[0] = n; data[1] = m;
  
  mf_seq_set_track(trks->ms, 0);
  trks->err = mf_seq_sys(trks->ms,  trks->tick[trks->track], st_meta_event, me_key_signature, 2, data);
  mf_seq_set_track(trks->ms, trks->track);
}

#endif /**/ 

static void setinstr(trk_data *trks)
{
  unsigned char  c;
  short n = -1;
  char iname[32];
  short k = 0;
  
  skipctrl(trks);
  
  c = ch_cur(trks);
  if (isdigit(c)) {
    n = getnum(trks);
  } 
  else {
    c = ch_get(trks);
    while (c && isalnum(c) && k<30) {
      iname[k++] = c;
      c = ch_get(trks);
    }
    iname[k] = '\0';
    while (c && isalnum(c)) {
      c = ch_get(trks);
    }
    ch_unget(trks);

    n = mf_instrbyname(iname);
  }
  if (n >= 0) {
    trks->err = mf_seq_evt(trks->ms, trks->tick[trks->track], st_program_change, trks->chan[trks->track], n, 0);
    if (!trks->err && k>0) 
      trks->err = mf_seq_sys(trks->ms,  trks->tick[trks->track], st_meta_event, me_instrument_name, k, iname);
  }
}

static void ctrl(trk_data *trks)
{
  unsigned char  c;
  unsigned char *p;
  
  ch_skip(trks);
  p = ch_curptr(trks);
  c = ch_cur(trks);

  _dbgmsg("CTRL: [%c]\n",c);
  
  if ( '1' <= c && c <= '9') {
    trks->chan[trks->track] = (c-'1');  
    c = ch_get(trks);
  }
  else if (strncmp(p,"meter:",6) == 0)    { setmeter(trks); }
  else if (strncmp(p,"key:"  ,4) == 0)    { setkey(trks);   } 
  else if (strncmp(p,"bpm:"  ,4) == 0)    { setbpm(trks);   } 
  else if (strncmp(p,"pan:"  ,4) == 0)    {  } 
  else if (strncmp(p,"vol:"  ,4) == 0)    { setvol(trks);   }
  else if (strncmp(p,"vel:"  ,4) == 0)    { setvel(trks);   }
  else if (strncmp(p,"instr:",6) == 0)    { setinstr(trks); }
  else { skipctrl(trks); skiparg(trks); }
}

static void defvalue(trk_data *trks)
{
  unsigned char c;
   
  ch_skip(trks);        /* skip ':' */
  c = ch_cur(trks);
  _dbgmsg("DEFV: [%c]\n",c);
  if (!c || isspace(c)) return;
  getnote(trks,0);
}  

static void gettxt(trk_data *trks)
{
   short n = 0;
   unsigned char c;
   unsigned char q;
   unsigned char *data;
   short e;
   unsigned long dur;
   
   q = ch_get(trks);
   data = ch_curptr(trks);
   c = ch_get(trks);
   
   while (c && c != q) {
     _dbgmsg("C: %d [%c]\n",n,c);
     if (c == '\\' && ch_cur(trks)) { n++; c = ch_get(trks); }
     c = ch_get(trks);
     n++;
   }
   if (!c) ch_unget(trks);

   dur = notelen(trks);

   e = me_lyric;
   if (q == '"') e = me_text;
   mf_seq_sys(trks->ms,  trks->tick[trks->track], st_meta_event, e, n, data);
   if (dur != DUR_INFINITE) {
     trks->tick[trks->track] += dur;
   }  
}

#if 1 /* REPEAT */

static void rptstart(trk_data *trks)
{
   unsigned char c;
   
    c = ch_get(trks);
    c = ch_cur(trks);
    
    if (c && trks->rpt_top < MAX_REPT) {
      trks->rpt_pos[trks->rpt_top] = ch_curptr(trks);
      trks->rpt_cnt[trks->rpt_top] = -1;
      trks->rpt_top++;
    }
}

static void rptskipcnt(trk_data *trks)
{
   unsigned char c;
   c = ch_cur(trks);
   
   if (c == '*') {
     c = ch_get(trks);
     while (isdigit((c = ch_cur(trks)))) {c = ch_get(trks);}
   }
}

static void rptend(trk_data *trks)
{
  unsigned char c;
  int k;
  
  c = ch_get(trks);     /* skip ) */
  c = ch_cur(trks);
   
  if (trks->rpt_top == 0) {
    rptskipcnt(trks);
    return;
  }
        
  k =  trks->rpt_top - 1;
    
  if (trks->rpt_cnt[k] > 0) { /* we are repeating */
    trks->ptr = trks->rpt_pos[k];
    trks->rpt_cnt[k]--;
  }
  else if (trks->rpt_cnt[k] == 0) { /* that was last repetition */
    rptskipcnt(trks);
    trks->rpt_top--;
  }
  else {  /* that was the first repetition (set cnt) */
    if (c == '*') {
      if (trks->rpt_top == MAX_REPT) {
        rptskipcnt(trks);     
      }
      else {
        trks->rpt_cnt[k] = 0;
        c = ch_get(trks);
        trks->rpt_cnt[k] = getnum(trks)-2;
        if (trks->rpt_cnt[k] >= 0)  { trks->ptr = trks->rpt_pos[k]; }
        else trks->rpt_top--;
      }
    }
  }
}
#endif /**/

static void getlinenum(trk_data *trks)
{
  unsigned char c;
 
  ch_skip(trks);       /* skip \n */
  c = ch_get(trks);
  if (c == '\t') {
    trks->line = getnum(trks);
    ch_skip(trks);    /* skip ' ' */
  }
  else ch_unget(trks);
  _dbgmsg("LINE: %d\n",trks->line);
}

static void parse(trk_data *trks)
{
  unsigned char c;
  
  while ((c = ch_cur(trks))) {
    _dbgmsg("C: %c\n",c);
    if (ispitch(c) || isdigit(c) || 
        (c == '$') || (c == 'N') || (c == 'n') ||
        (c == 'x') || (c == 'X' ) )               { getnote(trks,1);   }
    else if ( c == 'r' || c == 'R' || c == '-')   { rest(trks);        }
    else if ( c == '|' )                          { chgtrack(trks);    }
    else if ( c == '&' )                          { rtarget(trks);     }
    else if ( c == '@' )                          { ctrl(trks);        }
    else if ( c == ':' )                          { defvalue(trks);    }
    else if ( c == '"' )                          { gettxt(trks);      }
    else if ( c == '(' )                          { rptstart(trks);    }
    else if ( c == ')' )                          { rptend(trks);      }
    else if ( c == '\'' )                         { gettxt(trks);      }
    else if ( c == '\n' )                         { getlinenum(trks);  }
    else if ( isspace(c) )                        { ch_skip(trks);     }
    else ch_skip(trks);
  }
}

static int tomidi(char *fname, short division, unsigned char *s)
{
  int err = 0;
  trk_data tracks;
  int k;
  
  if (!fname)    return 911;
  if (!s || !*s) return 912;
  
  for (k = 0; k < 10 ; k++) {
    tracks.trgt[k] = 0;
  }
  
  tracks.scale[0] =  0;  /* C */
  tracks.scale[1] =  2;  /* D */
  tracks.scale[2] =  4;  /* E */
  tracks.scale[3] =  5;  /* F */
  tracks.scale[4] =  7;  /* G */
  tracks.scale[5] =  9;  /* A */
  tracks.scale[6] = 11;  /* B */

  tracks.scale_n = 7;
  
  for (k = 0; k < MAX_TRACKS; k++) {
       tracks.dur [k]     = division;  /* a quarter note */
       tracks.tick[k]     = 0;
       tracks.chan[k]     = 0;
       tracks.inst[k]     = 0;
      tracks.notes[k][0]  = 60;
    tracks.chord_n[k]     = 0;
        tracks.vol[k]     = 90;
  }

  tracks.line = 0;
  
  tracks.track   = 0;
  tracks.ppqn    = division;
  tracks.buf     = s;
  tracks.ptr     = tracks.buf;
  tracks.err     = 0;
  tracks.rpt_top = 0;
  tracks.trgt_n  = 0;

  tracks.num = 4;
  tracks.den = 2;
  
  tracks.ms = mf_seq_new(fname, division);

  if (tracks.ms) {  
    if (setjmp(tracks.errjmp) == 0) {
      parse(&tracks);
    }
    err = mf_seq_close(tracks.ms);
  } 
  else err = 910; /* Unable to open midifile */

  if (tracks.err) err = tracks.err;

  if (err) err = tracks.line * 1000 + err;
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

#endif /**/ 

#ifdef MF_SCORE_TEST

/* 100Kb max */
#define SCORE_BUF_SIZE (102400-1)
unsigned char score_buf[SCORE_BUF_SIZE+1];

int main(int argc, char *argv[])
{
  unsigned char *s;
  unsigned long k;
  int err = 0;
  int c;
  FILE *f;
  
  if (argc > 1) {
    f = fopen(argv[1],"r");
    if (f) {
      s = score_buf;
      k = 0;
      while (((c = fgetc(f)) != EOF) && (k < SCORE_BUF_SIZE)) {
        _dbgmsg("R: %c\n",c);
        *s++ = c;
        k++;
      }
      *s = '\0';
       
      fclose(f); 
      err = mf_score("sc.mid",384, score_buf);    
    }
    else { err = 185; }
  }  
  else { err = 186; }
  
  if (err) printf("ERR: %d\n",err);
  
  return(err);
}

#endif 

