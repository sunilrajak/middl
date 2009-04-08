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

#include "mf.h"

static char *instr_names[] = {
   "\025accordion",                  /*  21*/
   "\040acousticbass",               /*  32*/
   "\000acousticgrand",              /*   0*/
   "\161agogo",                      /* 113*/
   "\101altosax",                    /*  65*/
   "\176applause",                   /* 126*/
   "\155bagpipe",                    /* 109*/
   "\151banjo",                      /* 105*/
   "\103baritonesax",                /*  67*/
   "\106bassoon",                    /*  70*/
   "\173birdtweet",                  /* 123*/
   "\114blownbottle",                /*  76*/
   "\075brasssection",               /*  61*/
   "\171breathnoise",                /* 121*/
   "\001brightacoustic",             /*   1*/
   "\010celesta",                    /*   8*/
   "\052cello",                      /*  42*/
   "\064choiraahs",                  /*  52*/
   "\023churchorgan",                /*  19*/
   "\107clarinet",                   /*  71*/
   "\007clavinet",                   /*   7*/
   "\053contrabass",                 /*  43*/
   "\036distortionguitar",           /*  30*/
   "\020drawbarorgan",               /*  16*/
   "\017dulcimer",                   /*  15*/
   "\041ebassfinger",                /*  33*/
   "\042ebasspick",                  /*  34*/
   "\033ecleanguitar",               /*  27*/
   "\032ejazzguitar",                /*  26*/
   "\002electricgrand",              /*   2*/
   "\034emutedguitar",               /*  28*/
   "\105englishhorn",                /*  69*/
   "\004epiano1",                    /*   4*/
   "\005epiano2",                    /*   5*/
   "\156fiddle",                     /* 110*/
   "\111flute",                      /*  73*/
   "\074frenchhorn",                 /*  60*/
   "\043fretlessbass",               /*  35*/
   "\143fxatmosphere",               /*  99*/
   "\144fxbrightness",               /* 100*/
   "\142fxcrystal",                  /*  98*/
   "\146fxechoes",                   /* 102*/
   "\145fxgoblins",                  /* 101*/
   "\140fxrain",                     /*  96*/
   "\147fxscifi",                    /* 103*/
   "\141fxsoundtrack",               /*  97*/
   "\011glockenspiel",               /*   9*/
   "\170guitarfretnoise",            /* 120*/
   "\037guitarharmonics",            /*  31*/
   "\177gunshot",                    /* 127*/
   "\026harmonica",                  /*  22*/
   "\006harpsichord",                /*   6*/
   "\175helicopter",                 /* 125*/
   "\003honkytonk",                  /*   3*/
   "\154kalimba",                    /* 108*/
   "\153koto",                       /* 107*/
   "\127leadbasslead",               /*  87*/
   "\122leadcalliope",               /*  82*/
   "\124leadcharang",                /*  84*/
   "\123leadchiff",                  /*  83*/
   "\126leadfifths",                 /*  86*/
   "\121leadsawtooth",               /*  81*/
   "\120leadsquare",                 /*  80*/
   "\125leadvoice",                  /*  85*/
   "\014marimba",                    /*  12*/
   "\165melodictom",                 /* 117*/
   "\012musicbox",                   /*  10*/
   "\073mutedtrumpet",               /*  59*/
   "\030nylonstringguitar",          /*  24*/
   "\104oboe",                       /*  68*/
   "\117ocarina",                    /*  79*/
   "\067orchestrahit",               /*  55*/
   "\056orchestralstrings",          /*  46*/
   "\035overdrivenguitar",           /*  29*/
   "\134padbowed",                   /*  92*/
   "\133padchoir",                   /*  91*/
   "\136padhalo",                    /*  94*/
   "\135padmetallic",                /*  93*/
   "\130padnewage",                  /*  88*/
   "\132padpolysynth",               /*  90*/
   "\137padsweep",                   /*  95*/
   "\131padwarm",                    /*  89*/
   "\113panflute",                   /*  75*/
   "\021percussiveorgan",            /*  17*/
   "\110piccolo",                    /*  72*/
   "\055pizzicatostrings",           /*  45*/
   "\112recorder",                   /*  74*/
   "\024reedorgan",                  /*  20*/
   "\167reversecymbal",              /* 119*/
   "\022rockorgan",                  /*  18*/
   "\172seashore",                   /* 122*/
   "\152shamisen",                   /* 106*/
   "\157shanai",                     /* 111*/
   "\150sitar",                      /* 104*/
   "\115skakuhachi",                 /*  77*/
   "\044slapbass1",                  /*  36*/
   "\045slapbass2",                  /*  37*/
   "\100sopranosax",                 /*  64*/
   "\162steeldrums",                 /* 114*/
   "\031steelstringguitar",          /*  25*/
   "\060stringensemble1",            /*  48*/
   "\061stringensemble2",            /*  49*/
   "\046synthbass1",                 /*  38*/
   "\047synthbass2",                 /*  39*/
   "\076synthbrass1",                /*  62*/
   "\077synthbrass2",                /*  63*/
   "\166synthdrum",                  /* 118*/
   "\062synthstrings1",              /*  50*/
   "\063synthstrings2",              /*  51*/
   "\066synthvoice",                 /*  54*/
   "\164taikodrum",                  /* 116*/
   "\027tangoaccordion",             /*  23*/
   "\174telephonering",              /* 124*/
   "\102tenorsax",                   /*  66*/
   "\057timpani",                    /*  47*/
   "\160tinklebell",                 /* 112*/
   "\054tremolostrings",             /*  44*/
   "\071trombone",                   /*  57*/
   "\070trumpet",                    /*  56*/
   "\072tuba",                       /*  58*/
   "\016tubularbells",               /*  14*/
   "\013vibraphone",                 /*  11*/
   "\051viola",                      /*  41*/
   "\050violin",                     /*  40*/
   "\065voiceoohs",                  /*  53*/
   "\116whistle",                    /*  78*/
   "\163woodblock",                  /* 115*/
   "\015xylophone",                  /*  13*/
};

/* Shortcuts:
**
**   muted    m
**   short    s
**   long     l
**   high     h
**   low      l
**   cymbal   cy
**   acoustic a
**   electric e
**   closed   c
**   open     o
**
**  EXAMPLES:
**   
**   closed-hihat  = chihat
**   muted cuica   = mcuica
**   low tom       = ltom
**   long whistle  = lwhistle
**   short whistle = swhistle
**    
*/
static char *perc_names[] = {
  "\243abassdrum",                  /*  (0x80 |  35) */
  "\246asnare",                     /*  (0x80 |  38) */
  "\243b2",                         /*  (0x80 |  35) */
  "\244bassdrum",                   /*  (0x80 |  36) */
  "\244bd",                         /*  (0x80 |  36) */
  "\271c2",                         /*  (0x80 |  57) */
  "\305cabasa",                     /*  (0x80 |  69) */
  "\270cb",                         /*  (0x80 |  56) */
  "\261cc",                         /*  (0x80 |  49) */
  "\252ch",                         /*  (0x80 |  42) */
  "\252chihat",                     /*  (0x80 |  42) */
  "\264chinesecy",                  /*  (0x80 |  52) */
  "\313claves",                     /*  (0x80 |  75) */
  "\270cowbell",                    /*  (0x80 |  56) */
  "\261crashcy",                    /*  (0x80 |  49) */
  "\271crashcy2",                   /*  (0x80 |  57) */
  "\250esnare",                     /*  (0x80 |  40) */
  "\303hagogo",                     /*  (0x80 |  67) */
  "\247handclap",                   /*  (0x80 |  39) */
  "\274hbongo",                     /*  (0x80 |  60) */
  "\247hc",                         /*  (0x80 |  39) */
  "\253hfloortom",                  /*  (0x80 |  43) */
  "\254hh",                         /*  (0x80 |  44) */
  "\260hmidtom",                    /*  (0x80 |  48) */
  "\301htimbale",                   /*  (0x80 |  65) */
  "\262htom",                       /*  (0x80 |  50) */
  "\314hwoodblock",                 /*  (0x80 |  76) */
  "\304lagogo",                     /*  (0x80 |  68) */
  "\275lbongo",                     /*  (0x80 |  61) */
  "\300lconga",                     /*  (0x80 |  64) */
  "\251lfloortom",                  /*  (0x80 |  41) */
  "\312lguiro",                     /*  (0x80 |  74) */
  "\257lmidtom",                    /*  (0x80 |  47) */
  "\302ltimbale",                   /*  (0x80 |  66) */
  "\255ltom",                       /*  (0x80 |  45) */
  "\310lwhistle",                   /*  (0x80 |  72) */
  "\315lwoodblock",                 /*  (0x80 |  77) */
  "\306maracas",                    /*  (0x80 |  70) */
  "\316mcuica",                     /*  (0x80 |  78) */
  "\276mhconga",                    /*  (0x80 |  62) */
  "\320mtriangle",                  /*  (0x80 |  80) */
  "\317ocuica",                     /*  (0x80 |  79) */
  "\256oh",                         /*  (0x80 |  46) */
  "\277ohconga",                    /*  (0x80 |  63) */
  "\256ohihat",                     /*  (0x80 |  46) */
  "\321otriangle",                  /*  (0x80 |  81) */
  "\254pedhihat",                   /*  (0x80 |  44) */
  "\273r2",                         /*  (0x80 |  59) */
  "\265rb",                         /*  (0x80 |  53) */
  "\263rc",                         /*  (0x80 |  51) */
  "\265ridebell",                   /*  (0x80 |  53) */
  "\263ridecy",                     /*  (0x80 |  51) */
  "\273ridecy2",                    /*  (0x80 |  59) */
  "\245rs",                         /*  (0x80 |  37) */
  "\250s2",                         /*  (0x80 |  40) */
  "\267sc",                         /*  (0x80 |  55) */
  "\246sd",                         /*  (0x80 |  38) */
  "\311sguiro",                     /*  (0x80 |  73) */
  "\245sidestick",                  /*  (0x80 |  37) */
  "\267splashcy",                   /*  (0x80 |  55) */
  "\307swhistle",                   /*  (0x80 |  71) */
  "\251t1",                         /*  (0x80 |  41) */
  "\253t2",                         /*  (0x80 |  43) */
  "\255t3",                         /*  (0x80 |  45) */
  "\257t4",                         /*  (0x80 |  47) */
  "\260t5",                         /*  (0x80 |  48) */
  "\262t6",                         /*  (0x80 |  50) */
  "\266ta",                         /*  (0x80 |  54) */
  "\266tambourine",                 /*  (0x80 |  54) */
  "\264tc",                         /*  (0x80 |  52) */
  "\251tom1",                       /*  (0x80 |  41) */
  "\253tom2",                       /*  (0x80 |  43) */
  "\255tom3",                       /*  (0x80 |  45) */
  "\257tom4",                       /*  (0x80 |  47) */
  "\260tom5",                       /*  (0x80 |  48) */
  "\262tom6",                       /*  (0x80 |  50) */
  "\272vibraslap",                  /*  (0x80 |  58) */
};

int mf_i_cmp(const void *k, const void *b)
{
  unsigned char *p = *(unsigned char **)k;
  unsigned char *q = (*(unsigned char **)b)+1;
  int c;

  while (1) {
    c = tolower(*p) - tolower(*q);
    if ( c != 0 || *p == '\0' || *q == '\0') break;
    p++; q++;
  }

  return c;
}

int mf_instrbyname(char *name)
{
  unsigned char **q;

  q = bsearch(&name, instr_names, sizeof(instr_names)/sizeof(char *),
                                                       sizeof(char *), mf_i_cmp);
  return  (q == NULL)? -1 : (int)(**q) + 1;
}

int mf_percbyname(char *name)
{
  unsigned char **q;

  q = bsearch(&name, perc_names, sizeof(perc_names)/sizeof(char *),
                                                       sizeof(char *), mf_i_cmp);
  return  (q == NULL)? -1 : (int)(**q);
}


/* notes name are case insensitive
** flat   can be written as 'b' or '-'
** sharps can be written as '#' or '+'
** multiple accidentals are recognized
** so that "Ebb" is the same as "D"
** octaves ranges from 0 to 10;
**
** percussion names are also valid
** arguments for this function.
*/
extern char mf_note_base[];
unsigned char mf_pitchbyname(char *note)
{
  int pitch  = 0;
  int octave = 0;
  int k;

  if ((pitch = mf_percbyname(note)) >= 0) return pitch;

  if (tolower(note[0]) == 'z') return n_rest;

  pitch = tolower(note[0]) - 'a';

  if (pitch < 0) pitch = 0;
  else if (pitch > 6) pitch = 6;

  pitch = mf_note_base[pitch];

  k = 1;
  while (1) {
    if (note[k] == '-' || tolower(note[k] == 'b')) pitch--;
    else if (note[k] == '+' || note[k] == '#') pitch++;
    else break;
    k++;
  }

  if (isdigit(note[k])) {
    octave = octave * 10 + (note[k++]-'0');
    if (isdigit(note[k])) {
      octave = octave * 10 + (note[k++]-'0');
    }
  }
  else octave = 5;

  pitch += 12 * octave;
  if (pitch > 127) pitch = 127;

  return  (unsigned char)(pitch & 0xFF);
}

static char *dur ="dwhqestfo";
               /*  012345678 */

long mf_durationbyname(char *name)
{
  long duration = d_whole; /* whole note */
  int k;
  int c;

  c=tolower(*name);
  for (k=0; dur[k] && c != dur[k]; k++) ;
  if (dur[k]) {
    duration = (mf_qnote_ticks << 3) >> k;
    name++;
  }

  for (k=0; name[k] == '.'; k++) ;
  /*
  * Given k dots you multiply by $(2^(k+1) - 1) / (2^k)$
  */
  if (k > 0) {
    duration *= ((1 << (k+1)) - 1);
    duration /= (1 << k);
  }

  name += k;
  c = atoi(name);
  if (c > 0) duration *= c;

  for (k=0; name[k] && (name[k] != '/'); k++) ;
  if (name[k])  {
    c = atoi(name+1);
    if (c > 0) duration /= c;
  }

  return duration;
}

