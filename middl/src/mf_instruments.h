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


