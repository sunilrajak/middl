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

    if (*p == '@') {
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
    else if (*p == '%') { /* strip Comment */
      while (*p && *p != '\r' && *p != '\n') p++;
      if (*p == '\r' && p[1] == '\n') p++;
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
  
  short num;
  short den;
  
  short rpt_top;
  unsigned char *rpt_pos[MAX_REPT];
           short rpt_cnt[MAX_REPT];

  unsigned long  trgt[10];
           
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
  unsigned long  tmp = 0;
  unsigned long  dur = 0;
  unsigned short mul = 0;
  unsigned short div = 0;

  
  c = *(trks->ptr++);
  dur = trks->dur[trks->track];
  if ( c == '/') {
    
    c = *(trks->ptr++);
    
    if (c == '+') {
      dur = DUR_INFINITE;
      c = *(trks->ptr++);
    }
    else if (c == '-') {
      dur = 0;
      c = *(trks->ptr++);
    }
    else {
      dur = (trks->ppqn * 4);
      
      switch (c) {
        case 'w' : dur = (trks->ppqn * 4) ; c = *(trks->ptr++); break;
        case 'h' : dur = (trks->ppqn * 2) ; c = *(trks->ptr++); break;
        case 'q' : dur = (trks->ppqn    ) ; c = *(trks->ptr++); break;
        case 'e' : dur = (trks->ppqn / 2) ; c = *(trks->ptr++); break;
        case 's' : dur = (trks->ppqn / 4) ; c = *(trks->ptr++); break;
        case 't' : dur = (trks->ppqn / 8) ; c = *(trks->ptr++); break;
      }
      
      while ('0' <= c && c <= '9') {
        div = div * 10 + (c -'0');  
        c = *(trks->ptr++);
      }
      if (div == 0) div = 1;
      
      if (c == '*') {
        c = *(trks->ptr++);
        while ('0' <= c && c <= '9') {
          mul = mul * 10 + (c -'0');  
          c = *(trks->ptr++);
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
    c = *(trks->ptr++) ;
  }
  
  trks->ptr--;
  
  return dur;
}

static int getnum(trk_data *trks)
{
  int n = 0;
  unsigned char c;
  short d = 1;

  c = *(trks->ptr);
  
  if ((c == '+' || c == '-')  && !isdigit(*(trks->ptr + 1))) return 0;
   
  if (c == '+')      { trks->ptr++;         }
  else if (c == '-') { trks->ptr++; d = -1; }
  
  while ((c = *(trks->ptr)) && '0' <= c && c <= '9') {
    n = n * 10 + (c - '0');
    trks->ptr++;
  }
   
  return n * d; 
}

static void getnote(trk_data *trks,int play)
{
  unsigned char c;
  unsigned char istmp = 0;
  short n;
  short cur_oct = 0;
  short tmp_oct = 0;
  unsigned long dur;
  
  c = *(trks->ptr++);
  
  if ('0' <= c && c <= '9') {
     trks->ptr--;
     n = getnum(trks) + trks->note[trks->track];
     istmp = 1;
  }
  else if (c == 'x' || c == 'X') {
    dbgmsg("X: %p [%c]\n",trks->ptr,c);
    n = getnum(trks) + trks->note[trks->track];
    istmp = (c == 'x');
  }
  else {
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
    else if ( c == '\'')       { tmp_oct++; while ((c =*(trks->ptr)) == '\'')  {tmp_oct++; trks->ptr++;} }
    else if ( c == ',')        { tmp_oct--; while ((c =*(trks->ptr)) == ',')   {tmp_oct--; trks->ptr++;} }
    else trks->ptr--;
    
    n += cur_oct * 12;   
  }
  while (n < 0)    { n += 12; } 
  while (n > 127)  { n -= 12; } 
  
  if (!istmp) trks->note[trks->track] = n;
  
  n += tmp_oct * 12;
  while (n < 0)    { n += 12; } 
  while (n > 127)  { n -= 12; } 
  
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

#define note(t) getnote(t,1)

static void rest(trk_data *trks)
{
  unsigned char c;
  unsigned long dur;
  unsigned short mul = 0;
  
  if ((c = *(trks->ptr)) == '-') {
    for(;;) {
     c = *(trks->ptr++);
           if (c == '-' || c == '=')  { mul++; }
      else if (c != '\t' && c != ' ') { trks->ptr--; break; }
    }
    dur = trks->dur[trks->track] * mul;
  }
  else {
    trks->ptr++;
    dur = notelen(trks);
   
    if (dur == 0 || c == 'r') {
      trks->err = mf_seq_evt(trks->ms, trks->tick[trks->track], st_note_off, trks->chan[trks->track],
                                            trks->note[trks->track], 0);
    }
  }
  
  if (dur != DUR_INFINITE) {  trks->tick[trks->track] += dur; }  
}

static void chgtrack(trk_data *trks)
{
  unsigned char  c;
  unsigned short trk;
  
  c = *(trks->ptr++); /* skip '|' */
  
  if (isdigit(*(trks->ptr)))  trk = getnum(trks);
  else trk = trks->track+1; 
  
  if (trk < 0) trk = 0;
  else if (MAX_TRACKS <= trk) trk = MAX_TRACKS-1;
  
  trks->track = trk;
  mf_seq_set_track(trks->ms, trk);
  
}

static void back(trk_data *trks)
{
  unsigned char  c;
  
  c = *(trks->ptr++);
  
  c = *(trks->ptr);
  if ( '0' <= c && c <= '9') {
    trks->tick[trks->track] = trks->trgt[c-'0'];
    trks->ptr++;
  }
  else if (trks->dur[trks->track] != DUR_INFINITE) {
    if (trks->tick[trks->track] < trks->dur[trks->track])
      trks->tick[trks->track] = 0;
    else 
      trks->tick[trks->track] -= trks->dur[trks->track];
  }
  _dbgmsg("BACK TO: %d\n", trks->tick[trks->track]);
}

static void target(trk_data *trks)
{
  unsigned char  c;
  
  c = *(trks->ptr++);
  c = *(trks->ptr++);
  
  if ( '0' <= c && c <= '9') {
    trks->trgt[c-'0'] = trks->tick[trks->track];
    _dbgmsg("TARGET: %d\n", trks->tick[trks->track]);
  }
  else trks->ptr--;
}

static void skiparg(trk_data *trks)
{
  unsigned char  c;
   c = *(trks->ptr++);
   if (c == '"') {
      c = *(trks->ptr++);
      while (c && c != '"') {
        if (c == '\\' && *(trks->ptr)) trks->ptr++;
        c = *(trks->ptr++);
      }
      if (c) trks->ptr++;
   }
   else {
     while (c && !isspace(c)) c = *(trks->ptr++); 
   }
   
}

static void skipctrl(trk_data *trks)
{
  unsigned char  c;
  c = *(trks->ptr++);
  while (c && c != ':') c = *(trks->ptr++);
}

static setvol(trk_data *trks)
{
  short n = 0;
  skipctrl(trks);
  n = getnum(trks);
  trks->vol[trks->track] = n;  
}

static setchan(trk_data *trks)
{
  short n = 0;
  skipctrl(trks);
  n = getnum(trks);
  if (n>0) n--;
  trks->chan[trks->track] = n & 0x0F;  
  dbgmsg("CHAN: %d\n",n);
}

static void setmeter(trk_data *trks)
{ /*  =meter:3/4  */
  short n = 0;
  short d = 2;
  
  unsigned char data[4];
  
  skipctrl(trks);
  n = getnum(trks);  
  if ( n == 0)  n=4;

  if (*(trks->ptr) == '/') {
    trks->ptr++;
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
static setkey(trk_data *trks)
{ /*  =key:+3:min  @key:Db:min */
  short n = 0;
  short m = 0;
  short r = -1;
  short a = 0;
  unsigned char data[4];
  unsigned char *p;
  
  skipctrl(trks);
  
  p = trks->ptr;
  
  if (*p == '+' || *p == '-' || isdigit(*p)) {
    n = getnum(trks);  
  }
  else if ('A' <= *p && *p <= 'G') {
    r = *(trks->ptr++) - 'A';
    a = *(trks->ptr++);
    if (a == 'b') a = -1;
    else if (a == '#') a = 1;
    else { a = 0;  trks->ptr--;  }
  }

  p = trks->ptr;
  if (strncmp(p,":min",4) == 0) {
    m = 1;
    trks->ptr += 4;
  }
  else if (strncmp(p,":maj",4) == 0) {
    trks->ptr += 4;
  }  

  if (r >= 0) {
    n = keyacc[r]-1;
    n += a * 7;
    n -= m * 3;
  }
  
  while (n < -7) n += 12;
  while (n >  7) n -= 12;
  
  dbgmsg("Key: r=%d a=%d m=%d n=%d p=[%s]\n",r,a,m,n,p);

  data[0] = n; data[1] = m;
  
  mf_seq_set_track(trks->ms, 0);
  trks->err = mf_seq_sys(trks->ms,  trks->tick[trks->track], st_meta_event, me_key_signature, 2, data);
  mf_seq_set_track(trks->ms, trks->track);
}

static void setinstr(trk_data *trks)
{
  unsigned char  c;
  short n = -1;
  char iname[32];
  short k = 0;
  
  skipctrl(trks);
  
  c = *(trks->ptr);
  if ('0' <= c && c <= '9') {
    n = getnum(trks);
  } 
  else {
    c = *(trks->ptr++);
    while (c && isalnum(c) && k<30) {
      iname[k++] = c;
      c = *(trks->ptr++);
    }
    iname[k] = '\0';
    while (c && isalnum(c)) {
      c = *(trks->ptr++);
    }
    trks->ptr--;

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
  
  c = *(trks->ptr++);
  p = trks->ptr;
  c = *p;

  _dbgmsg("CTRL: [%c]\n",c);
  
  if ( '0' <= c && c <= '9') {
    trks->trgt[c-'0'] = trks->tick[trks->track];
    _dbgmsg("TARGET: %d\n", trks->tick[trks->track]);
    trks->ptr++;
  }
  else if (strncmp(p,"meter:",6) == 0)    { setmeter(trks); }
  else if (strncmp(p,"key:"  ,4) == 0)    { setkey(trks); } 
  else if (strncmp(p,"bpm:"  ,4) == 0)    { setbpm(trks); } 
  else if (strncmp(p,"pan:"  ,4) == 0)    {  } 
  else if (strncmp(p,"vol:"  ,4) == 0)    { setvol(trks); }
  else if (strncmp(p,"ch:"   ,3) == 0)    { setchan(trks); }
  else if (strncmp(p,"instr:",6) == 0)    { setinstr(trks); }
  else { skipctrl(trks); skiparg(trks); }
  
}

static void defvalue(trk_data *trks)
{
  unsigned char c;
   
  c = *(trks->ptr++); /* skip ':' */
  c = *(trks->ptr);
  _dbgmsg("DEFV: [%c]\n",c);
  if (!c || isspace(c)) return;
  if ( ('A' <= c && c <= 'G') || ('a' <= c && c <= 'g') )
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
   
   q = *(trks->ptr++);
   data = trks->ptr;
   c = *(trks->ptr++);
   while (c && c != q) {
     _dbgmsg("C: %d [%c]\n",n,c);
     if (c == '\\' && *(trks->ptr)) { n++; trks->ptr++; }
     c = *(trks->ptr++);
     n++;
   }
   if (!c) trks->ptr--;

   dur = notelen(trks);

   e = me_lyric;
   if (q == '"') e = me_text;
   mf_seq_sys(trks->ms,  trks->tick[trks->track], st_meta_event, e, n, data);
   if (dur != DUR_INFINITE) {
     trks->tick[trks->track] += dur;
   }  
}

static void rptstart(trk_data *trks)
{
   unsigned char c;
   
    trks->ptr++;
    c = *(trks->ptr);
    
    if (c && trks->rpt_top < MAX_REPT) {
      trks->rpt_pos[trks->rpt_top] = trks->ptr;
      trks->rpt_cnt[trks->rpt_top] = -1;
      trks->rpt_top++;
    }
}

static void rptskipcnt(trk_data *trks)
{
   unsigned char c;
   c = *(trks->ptr);
   
   if (c == '*') {
     trks->ptr++;
     while (isdigit((c = *(trks->ptr)))) {trks->ptr++;}
   }
}

static void rptend(trk_data *trks)
{
  unsigned char c;
  int k;
  
  trks->ptr++;       /* skip ) */
  c = *(trks->ptr);
   
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
        trks->ptr++;
        trks->rpt_cnt[k] = getnum(trks)-2;
        if (trks->rpt_cnt[k] >= 0)  { trks->ptr = trks->rpt_pos[k]; }
        else trks->rpt_top--;
      }
    }
  }
}

static void parse(trk_data *trks)
{
   unsigned char c;
  
   while ((c = *(trks->ptr))) {
     _dbgmsg("C: %c\n",c);
     if (('A' <= c && c <= 'G') || ('a' <= c && c <= 'g') ||
         ('0' <= c && c <= '9') || ( c == 'x') || (c == 'X' ))
                                       { note(trks);     }
     else if ( c == 'r' || c == 'R' ||  c == '-')
                                       { rest(trks);     }
     else if ( c == '|' )              { chgtrack(trks); }
     else if ( c == '<' )              { back(trks); }
     else if ( c == '@' )              { ctrl(trks); }
     else if ( c == ':' )              { defvalue(trks); }
     else if ( c == '"' )              { gettxt(trks); }
     else if ( c == '(' )              { rptstart(trks); }
     else if ( c == ')' )              { rptend(trks); }
     else if ( c == '\'' )             { gettxt(trks); }
     else if ( isspace(c) )            { while (isspace(*(trks->ptr))) trks->ptr++; }
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
  
  for (k = 0; k < 10 ; k++) {
    tracks.trgt[k] = 0;
  }
  
  for (k = 0; k < MAX_TRACKS; k++) {
    tracks.dur [k] = division;
    tracks.tick[k] = 0;
    tracks.chan[k] = 0;
    tracks.inst[k] = 0;
    tracks.note[k] = 60;
    tracks.vol [k] = 90;
  }

  tracks.track   = 0;
  tracks.ppqn    = division;
  tracks.buf     = s;
  tracks.ptr     = tracks.buf;
  tracks.err     = 0;
  tracks.rpt_top = 0;

  tracks.ms = mf_seq_new(fname, division);

  tracks.num = 4;
  tracks.den = 2;
  
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

#define SCORE_BUF_SIZE 10239
unsigned char score_buf[SCORE_BUF_SIZE+1];

int main(int argc, char *argv[])
{
  unsigned char *s;
  unsigned long k;
  int err;
  int c;
  FILE *f;
  
  if (argc < 2) {
    fprintf(stderr, "ut_score file-in\n");
    exit (1);
  }  
  
  f = fopen(argv[1],"r");
  if (!f) {
    fprintf(stderr, "Unable to open file\n");
    exit (1);
  }
  
  s = score_buf;
  k = 0;
  while ( ((c = fgetc(f)) != EOF) && (k < SCORE_BUF_SIZE)) {
    _dbgmsg("R: %c\n",c);
    *s++ = c;
    k++;
  }
  *s = '\0';
   
  fclose(f); 
   
  err = mf_score("sc.mid",384, score_buf);
  
  if (err) printf("ERR: %d\n",err);
  
  return(err);
}

#endif 

