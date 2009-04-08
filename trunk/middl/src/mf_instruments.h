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

#ifdef MIDLL_C
#undef MIDDL_H
#endif

#ifdef MIDDL_H

#define in_acousticgrand          0
#define in_brightacoustic         1
#define in_electricgrand          2
#define in_honkytonk              3
#define in_epiano1                4
#define in_epiano2                5
#define in_harpsichord            6
#define in_clavinet               7
#define in_celesta                8
#define in_glockenspiel           9
#define in_musicbox              10
#define in_vibraphone            11
#define in_marimba               12
#define in_xylophone             13
#define in_tubularbells          14
#define in_dulcimer              15
#define in_drawbarorgan          16
#define in_percussiveorgan       17
#define in_rockorgan             18
#define in_churchorgan           19
#define in_reedorgan             20
#define in_accordion             21
#define in_harmonica             22
#define in_tangoaccordion        23
#define in_nylonstringguitar     24
#define in_steelstringguitar     25
#define in_ejazzguitar           26
#define in_ecleanguitar          27
#define in_emutedguitar          28
#define in_overdrivenguitar      29
#define in_distortionguitar      30
#define in_guitarharmonics       31
#define in_acousticbass          32
#define in_ebassfinger           33
#define in_ebasspick             34
#define in_fretlessbass          35
#define in_slapbass1             36
#define in_slapbass2             37
#define in_synthbass1            38
#define in_synthbass2            39
#define in_violin                40
#define in_viola                 41
#define in_cello                 42
#define in_contrabass            43
#define in_tremolostrings        44
#define in_pizzicatostrings      45
#define in_orchestralstrings     46
#define in_timpani               47
#define in_stringensemble1       48
#define in_stringensemble2       49
#define in_synthstrings1         50
#define in_synthstrings2         51
#define in_choiraahs             52
#define in_voiceoohs             53
#define in_synthvoice            54
#define in_orchestrahit          55
#define in_trumpet               56
#define in_trombone              57
#define in_tuba                  58
#define in_mutedtrumpet          59
#define in_frenchhorn            60
#define in_brasssection          61
#define in_synthbrass1           62
#define in_synthbrass2           63
#define in_sopranosax            64
#define in_altosax               65
#define in_tenorsax              66
#define in_baritonesax           67
#define in_oboe                  68
#define in_englishhorn           69
#define in_bassoon               70
#define in_clarinet              71
#define in_piccolo               72
#define in_flute                 73
#define in_recorder              74
#define in_panflute              75
#define in_blownbottle           76
#define in_skakuhachi            77
#define in_whistle               78
#define in_ocarina               79
#define in_leadsquare            80
#define in_leadsawtooth          81
#define in_leadcalliope          82
#define in_leadchiff             83
#define in_leadcharang           84
#define in_leadvoice             85
#define in_leadfifths            86
#define in_leadbasslead          87
#define in_padnewage             88
#define in_padwarm               89
#define in_padpolysynth          90
#define in_padchoir              91
#define in_padbowed              92
#define in_padmetallic           93
#define in_padhalo               94
#define in_padsweep              95
#define in_fxrain                96
#define in_fxsoundtrack          97
#define in_fxcrystal             98
#define in_fxatmosphere          99
#define in_fxbrightness         100
#define in_fxgoblins            101
#define in_fxechoes             102
#define in_fxscifi              103
#define in_sitar                104
#define in_banjo                105
#define in_shamisen             106
#define in_koto                 107
#define in_kalimba              108
#define in_bagpipe              109
#define in_fiddle               110
#define in_shanai               111
#define in_tinklebell           112
#define in_agogo                113
#define in_steeldrums           114
#define in_woodblock            115
#define in_taikodrum            116
#define in_melodictom           117
#define in_synthdrum            118
#define in_reversecymbal        119
#define in_guitarfretnoise      120
#define in_breathnoise          121
#define in_seashore             122
#define in_birdtweet            123
#define in_telephonering        124
#define in_helicopter           125
#define in_applause             126
#define in_gunshot              127


#define p_abassdrum            (0x80 |  35)
#define p_bassdrum             (0x80 |  36)
#define p_sidestick            (0x80 |  37)
#define p_asnare               (0x80 |  38)
#define p_handclap             (0x80 |  39)
#define p_esnare               (0x80 |  40)
#define p_lfloortom            (0x80 |  41)
#define p_chihat               (0x80 |  42)
#define p_hfloortom            (0x80 |  43)
#define p_pedhihat             (0x80 |  44)
#define p_ltom                 (0x80 |  45)
#define p_ohihat               (0x80 |  46)
#define p_lmidtom              (0x80 |  47)
#define p_hmidtom              (0x80 |  48)
#define p_crashcy              (0x80 |  49)
#define p_htom                 (0x80 |  50)
#define p_ridecy               (0x80 |  51)
#define p_chinesecy            (0x80 |  52)
#define p_ridebell             (0x80 |  53)
#define p_tambourine           (0x80 |  54)
#define p_splashcy             (0x80 |  55)
#define p_cowbell              (0x80 |  56)
#define p_crashcy2             (0x80 |  57)
#define p_vibraslap            (0x80 |  58)
#define p_ridecy2              (0x80 |  59)
#define p_hbongo               (0x80 |  60)
#define p_lbongo               (0x80 |  61)
#define p_mhconga              (0x80 |  62)
#define p_ohconga              (0x80 |  63)
#define p_lconga               (0x80 |  64)
#define p_htimbale             (0x80 |  65)
#define p_ltimbale             (0x80 |  66)
#define p_hagogo               (0x80 |  67)
#define p_lagogo               (0x80 |  68)
#define p_cabasa               (0x80 |  69)
#define p_maracas              (0x80 |  70)
#define p_swhistle             (0x80 |  71)
#define p_lwhistle             (0x80 |  72)
#define p_sguiro               (0x80 |  73)
#define p_lguiro               (0x80 |  74)
#define p_claves               (0x80 |  75)
#define p_hwoodblock           (0x80 |  76)
#define p_lwoodblock           (0x80 |  77)
#define p_mcuica               (0x80 |  78)
#define p_ocuica               (0x80 |  79)
#define p_mtriangle            (0x80 |  80)
#define p_otriangle            (0x80 |  81)

int mf_percbyname(char *name);
int mf_instrbyname(char *name);
unsigned char mf_pitchbyname(char *note);
long mf_durationbyname(char *name);


#define n_Cb(o)          ((12 * (o) -  1) & 0x7F)
#define n_C(o)           ((12 * (o)     ) & 0x7F)
#define n_Db(o)          ((12 * (o) +  1) & 0x7F)
#define n_D(o)           ((12 * (o) +  2) & 0x7F)
#define n_Eb(o)          ((12 * (o) +  3) & 0x7F)
#define n_E(o)           ((12 * (o) +  4) & 0x7F)
#define n_F(o)           ((12 * (o) +  5) & 0x7F)
#define n_Gb(o)          ((12 * (o) +  6) & 0x7F)
#define n_G(o)           ((12 * (o) +  7) & 0x7F)
#define n_Ab(o)          ((12 * (o) +  8) & 0x7F)
#define n_A(o)           ((12 * (o) +  9) & 0x7F)
#define n_Bb(o)          ((12 * (o) + 10) & 0x7F)
#define n_B(o)           ((12 * (o) + 11) & 0x7F)
#define n_Bs(o)          ((12 * (o) + 12) & 0x7F)

#define n_(x) mf_pitchbyname(x)

char mf_notebystring(char *note);


#define n_Cflat        n_Cb
#define n_Csharp       n_Db
#define n_Cs           n_Db
#define n_Dflat        n_Db
#define n_Dsharp       n_Eb
#define n_Ds           n_Eb
#define n_Eflat        n_Eb
#define n_Esharp       n_F
#define n_Es           n_F
#define n_Fflat        n_E
#define n_Fb           n_E
#define n_Fsharp       n_Gb
#define n_Fs           n_Gb
#define n_Gflat        n_Gb
#define n_Gsharp       n_Ab
#define n_Gs           n_Ab
#define n_Aflat        n_Ab
#define n_Asharp       n_Bb
#define n_As           n_Bb
#define n_Bflat        n_Bb
#define n_Bsharp       n_Bs

#define n_none         ((unsigned char)(-1))
#define n_rest         ((unsigned char)(-2))
#define mf_rest(t,d)    mf_note(t,1,n_rest,d,100,0)

/*  = Note durations
**   These values are used to denote a the duration of musical
** event (notes and rests).
*/

#define d_double     (mf_qnote_ticks << 3)
#define d_whole      (mf_qnote_ticks << 2)
#define d_half       (mf_qnote_ticks << 1)
#define d_quarter     mf_qnote_ticks
#define d_8th        (mf_qnote_ticks >> 1)
#define d_16th       (mf_qnote_ticks >> 2)
#define d_64th       (mf_qnote_ticks >> 3)
#define d_128th      (mf_qnote_ticks >> 4)

#define d_beat       (d_whole / msq_meter[1])
#define d_measure    (d_beat * msq_meter[0])

/* Dotted notes */

#define d_dwhole     ((d_whole   * 3) / 2)
#define d_dhalf      ((d_half    * 3) / 2)
#define d_dquarter   ((d_quarter * 3) / 2)
#define d_d8th       ((d_8th     * 3) / 2)
#define d_d16th      ((d_16th    * 3) / 2)
#define d_d64th      ((d_64th    * 3) / 2)
#define d_d128th     ((d_128th   * 3) / 2)

#define d_(s)        mf_durationbyname(s)

#endif /* MIDDL_H */

#ifdef MIDDL_C

#ifdef MIDDL_STRINGS

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

#include "utl.h"

static int i_cmp(const void *k, const void *b)
{
  unsigned char *p = *(unsigned char **)k;
  unsigned char *q = (*(unsigned char **)b)+1;
  int c;

  _dbgmsg(":: '%s' '%2x'\n",*((unsigned char **)k),(*((unsigned char **)b))[0]);
  while (1) {
    c = tolower(*p) - tolower(*q);
    _dbgmsg("%c (%p) %c (%p) %d\n",tolower(*p), p, tolower(*q), q, c);
    if ( c != 0 || *p == '\0' || *q == '\0') break;
    p++; q++;
  }

  return c;
}

int mf_instrbyname(char *name)
{
  unsigned char **q;

  q = bsearch(&name, instr_names, sizeof(instr_names)/sizeof(char *),
                                                       sizeof(char *), i_cmp);
  _dbgmsg("INSTR: %s %p\n",name,q);
  return  (q == NULL)? -1 : (int)(**q) + 1;
}

int mf_percbyname(char *name)
{
  unsigned char **q;

  q = bsearch(&name, perc_names, sizeof(perc_names)/sizeof(char *),
                                                       sizeof(char *), i_cmp);
  return  (q == NULL)? -1 : (int)(**q);
}



#else

int mf_instrbyname(char *name) { return -1; }
int mf_percbyname (char *name) { return -1; }

#endif /* MIDDL_STRINGS */


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

  _dbgmsg("NOTEBYNAME: %s\n",note);
  if ((pitch = mf_percbyname(note)) >= 0) return pitch;

  _dbgmsg("           [%s]\n",note);

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

  _dbgmsg("Note by string: %s %d %d\n",note,pitch, octave);

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
  _dbgmsg("dur: %s =",name);
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
  _dbgmsg(" (num: %s %d) ",name,c);
  if (c > 0) duration *= c;

  for (k=0; name[k] && (name[k] != '/'); k++) ;
  if (name[k])  {
    c = atoi(name+1);
    if (c > 0) duration /= c;
  }
  _dbgmsg("%ld\n",duration);

  return duration;
}

#endif /* MIDLL_C */
