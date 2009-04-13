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

#define c_p4                       0 
#define c_p4_1                     1 
#define c_p4_2                     2 
#define c_p4_3                     3 
#define c_p9                       4 
#define c_p9_1                     5 
#define c_p9_2                     6 
#define c_p9_3                     7 
#define c_11                       8 
#define c_11_1                     9 
#define c_11_2                    10 
#define c_11_3                    11 
#define c_11_4                    12 
#define c_11_5                    13 
#define c_5                       14 
#define c_5_1                     15 
#define c_5_2                     16 
#define c_6                       17 
#define c_6_1                     18 
#define c_6_2                     19 
#define c_6_3                     20 
#define c_7                       21 
#define c_7_5b                    22 
#define c_7_5b_1                  23 
#define c_7_5b_2                  24 
#define c_7_5b_3                  25 
#define c_7_1                     26 
#define c_7_2                     27 
#define c_7_3                     28 
#define c_7sus                    29 
#define c_7sus9                   30 
#define c_7sus9_1                 31 
#define c_7sus9_2                 32 
#define c_7sus9_3                 33 
#define c_7sus_1                  34 
#define c_7sus_2                  35 
#define c_7sus_3                  36 
#define c_9                       37 
#define c_9_1                     38 
#define c_9_2                     39 
#define c_9_3                     40 
#define c_9_4                     41 
#define c_aug                     42 
#define c_aug7                    43 
#define c_aug7_1                  44 
#define c_aug7_2                  45 
#define c_aug7_3                  46 
#define c_aug_1                   47 
#define c_aug_2                   48 
#define c_dim                     49 
#define c_dim7                    50 
#define c_dim7_1                  51 
#define c_dim7_2                  52 
#define c_dim7_3                  53 
#define c_dim9                    54 
#define c_dim9_1                  55 
#define c_dim9_2                  56 
#define c_dim9_3                  57 
#define c_dim9_4                  58 
#define c_dim_1                   59 
#define c_dim_2                   60 
#define c_maj                     61 
#define c_maj7                    62 
#define c_maj7_1                  63 
#define c_maj7_2                  64 
#define c_maj7_3                  65 
#define c_maj9                    66 
#define c_maj9_1                  67 
#define c_maj9_2                  68 
#define c_maj9_3                  69 
#define c_maj9_4                  70 
#define c_maj_1                   71 
#define c_maj_2                   72 
#define c_min                     73 
#define c_min6                    74 
#define c_min6_1                  75 
#define c_min6_2                  76 
#define c_min6_3                  77 
#define c_min7                    78 
#define c_min7_5b                 79 
#define c_min7_5b_1               80 
#define c_min7_5b_2               81 
#define c_min7_5b_3               82 
#define c_min7_1                  83 
#define c_min7_2                  84 
#define c_min7_3                  85 
#define c_min9                    86 
#define c_min9_1                  87 
#define c_min9_2                  88 
#define c_min9_3                  89 
#define c_min9_4                  90 
#define c_min_1                   91 
#define c_min_2                   92 
#define c_sus                     93 
#define c_sus9                    94 
#define c_sus9_1                  95 
#define c_sus9_2                  96 
#define c_sus_1                   97 
#define c_sus_2                   98 

#define chord_max                 99

#define mf_ischord(x)      ((x)[0] & 0x80)
#define mf_isformula(x)    ((x)[0] & 0x40)
#define mf_isgchord(x)     ((x)[0] & 0x20)
#define mf_chordlen(x)     ((x)[0] & 0x0F)
#define mf_chordnote(x,n)  ((x)[(n)+1])   
 
char *mf_chordbyname(char *name);
char *mf_gchordbyname(char *name);

#define mf_chordend -128
