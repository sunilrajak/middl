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

#ifdef MIDDL_C
#undef MIDDL_H
#endif

#ifdef MIDDL_H

#define cc_bank_select                     0x00
#define cc_modulation_wheel                0x01
#define cc_breath_controller               0x02
#define cc_foot_controller                 0x04
#define cc_portamento_time                 0x05
#define cc_data_entry                      0x06
#define cc_channel_volume                  0x07
#define cc_balance                         0x08
#define cc_pan                             0x0a
#define cc_expression_controller           0x0b
#define cc_effect_control1                 0x0c
#define cc_effect_control2                 0x0d
#define cc_bank_select_lsb                 0x20
#define cc_modulation_wheel_lsb            0x21
#define cc_breath_controller_lsb           0x22
#define cc_foot_controller_lsb             0x24
#define cc_portamento_lsb                  0x25
#define cc_data_entry_lsb                  0x26
#define cc_channel_volume_lsb              0x27
#define cc_balance_lsb                     0x28
#define cc_pan_lsb                         0x2a
#define cc_expression_controller_lsb       0x2b
#define cc_effect_control1_lsb             0x2c
#define cc_effect_control2_lsb             0x2d
#define cc_damper_pedal                    0x40
#define cc_portamento                      0x41
#define cc_sostenuto                       0x42
#define cc_soft_pedal                      0x43
#define cc_legato                          0x44
#define cc_hold_2                          0x45
#define cc_sound_variation                 0x46
#define cc_timbre_intensity                0x47
#define cc_harmonic_intensity              0x47
#define cc_release_time                    0x48
#define cc_attack_time                     0x49
#define cc_brightness                      0x4a
#define cc_decay_time                      0x4b
#define cc_vibrato_rate                    0x4c
#define cc_vibrato_depth                   0x4d
#define cc_vibrato_delay                   0x4e
#define cc_portamento_control              0x54
#define cc_reverb_send                     0x5b
#define cc_tremolo_depth                   0x5c
#define cc_chorus_send                     0x5d
#define cc_detune                          0x5e
#define cc_phaser_depth                    0x5f
#define cc_data_increment                  0x60
#define cc_data_decrement                  0x61
#define cc_non_registered_number_lsb       0x62
#define cc_non_registered_number           0x63
#define cc_registered_number_lsb           0x64
#define cc_registered_number               0x65
#define cc_all_sound_off                   0x78
#define cc_reset_all_controllers           0x79
#define cc_local_control                   0x7a
#define cc_all_notes_off                   0x7b
#define cc_omni_mode_off                   0x7c
#define cc_omni_mode_on                    0x7d
#define cc_mono_mode_on                    0x7e
#define cc_poly_mode_off                   0x7e
#define cc_poly_mode_on                    0x7f
#define cc_mono_mode_off                   0x7f

/* Registered parameter number */
#define cc_pitch_bend_sensitivity          0x80
#define cc_channel_fine_tuning             0x81
#define cc_channel_coarse_tuning           0x82
#define cc_tuning_program                  0x83
#define cc_tuning_bank                     0x84
#define cc_modulation_depth_range          0x85
#define cc_rpn_reset                       0xFF

/* RPN or N-RPN (http://www.philrees.co.uk/nrpnq.htm)*/

#define cc_nrpn(h,l) ((((h) &0x7F)<<8) | ((l)&0x7F) | 0x8000)
#define cc_rpn(h,l)  ((((h) &0x7F)<<8) | ((l)&0x7F) | 0x0080)

#define st_note_off                0x80
#define st_note_on                 0x90
#define st_key_pressure            0xA0
#define st_control_change          0xB0
#define st_program_change          0xC0
#define st_channel_pressure        0xD0
#define st_pitch_bend              0xE0
#define st_system_exclusive        0xF0
#define st_system_continue         0xF7
#define st_meta_event              0xFF

#define me_sequence_number         0x00
#define me_text_event              0x01
#define me_copyright_notice        0x02
#define me_sequence_name           0x03
#define me_track_name              0x03
#define me_instrument_name         0x04
#define me_lyric                   0x05
#define me_marker                  0x06
#define me_cue_point               0x07
#define me_device_name             0x08
#define me_program_name            0x09
#define me_end_of_track            0x2f
#define me_set_tempo               0x51
#define me_smpte_offset            0x54
#define me_time_signature          0x58
#define me_key_signature           0x59
#define me_sequencer_specific      0x7F
#define me_channel_prefix          0x20
#define me_port_prefix             0x21

#define mf_text(t,s)             mf_text_event(t, me_text_event,s)
#define mf_copyright_notice(t,s) mf_text_event(t, me_copyright_notice,s)
#define mf_sequence_name(t,s)    mf_text_event(t, me_sequence_name,s)
#define mf_track_name(t,s)       mf_text_event(t, me_track_name,s)
#define mf_instrument_name(t,s)  mf_text_event(t, me_instrument_name,s)
#define mf_marker(t,s)           mf_text_event(t, me_marker,s)
#define mf_cue_point(t,s)        mf_text_event(t, me_cue_point,s)
#define mf_device_name(t,s)      mf_text_event(t, me_device_name,s)
#define mf_program_name(t,s)     mf_text_event(t, me_program_name,s)
#define mf_lyric(t,d,s)         (mf_text_event(t, me_lyric,s), mf_tick(mf_curtickt+d))


/* Key signatures encoding:
**    min  maj
**    sxxx tyyy
**    \\_/ \\_/
**     \ \  \ \__ # of accidentals
**      \ \  \___ 0: sharp 1: flat
**       \ \_____ # of accidentals
**        \______ 0: sharp 1: flat
*/
#define ks_min 1
#define ks_maj 0
#define ks_A        0x03
#define ks_Asharp   0x70
#define ks_Aflat    0xFC
#define ks_B        0x25
#define ks_Bflat    0xDA
#define ks_C        0xB0
#define ks_Csharp   0x47
#define ks_Cflat    0x0F
#define ks_D        0x92
#define ks_Dsharp   0x60
#define ks_Dflat    0xDD
#define ks_E        0x14
#define ks_Eflat    0xEB
#define ks_F        0xC9
#define ks_Fsharp   0x36
#define ks_G        0xA1
#define ks_Gsharp   0x50
#define ks_Gflat    0x0E

#define tm_msec  0
#define tm_bpm   1

int mf_eventbyname(char *name);
int mf_ctrlbyname(char *name);


#endif /* MIDDL_H */


#ifdef MIDDL_C

#ifdef MIDLL_STRINGS

static unsigned char *control_names[] = {
  "\173all_notes_off"              ,
  "\170all_sound_off"              ,
  "\111attack_time"                ,
  "\010balance"                    ,
  "\050balance_lsb"                ,
  "\000bank_select"                ,
  "\040bank_select_lsb"            ,
  "\002breath_controller"          ,
  "\042breath_controller_lsb"      ,
  "\112brightness"                 ,
  "\202channel_coarse_tuning"      ,
  "\201channel_fine_tuning"        ,
  "\007channel_volume"             ,
  "\047channel_volume_lsb"         ,
  "\135chorus_send"                ,
  "\100damper_pedal"               ,
  "\141data_decrement"             ,
  "\006data_entry"                 ,
  "\046data_entry_lsb"             ,
  "\140data_increment"             ,
  "\113decay_time"                 ,
  "\136detune"                     ,
  "\014effect_control1"            ,
  "\054effect_control1_lsb"        ,
  "\015effect_control2"            ,
  "\055effect_control2_lsb"        ,
  "\013expression_controller"      ,
  "\053expression_controller_lsb"  ,
  "\004foot_controller"            ,
  "\044foot_controller_lsb"        ,
  "\107harmonic_intensity"         ,
  "\105hold_2"                     ,
  "\104legato"                     ,
  "\172local_control"              ,
  "\205modulation_depth_range"     ,
  "\001modulation_wheel"           ,
  "\041modulation_wheel_lsb"       ,
  "\177mono_mode_off"              ,
  "\176mono_mode_on"               ,
  "\143non_registered_number"      ,
  "\142non_registered_number_lsb"  ,
  "\174omni_mode_off"              ,
  "\175omni_mode_on"               ,
  "\012pan"                        ,
  "\052pan_lsb"                    ,
  "\137phaser_depth"               ,
  "\200pitch_bend_sensitivity"     ,
  "\176poly_mode_off"              ,
  "\177poly_mode_on"               ,
  "\101portamento"                 ,
  "\124portamento_control"         ,
  "\045portamento_lsb"             ,
  "\005portamento_time"            ,
  "\145registered_number"          ,
  "\144registered_number_lsb"      ,
  "\110release_time"               ,
  "\171reset_all_controllers"      ,
  "\133reverb_send"                ,
  "\377rpn_reset"                  ,
  "\103soft_pedal"                 ,
  "\102sostenuto"                  ,
  "\106sound_variation"            ,
  "\107timbre_intensity"           ,
  "\134tremolo_depth"              ,
  "\204tuning_bank"                ,
  "\203tuning_program"             ,
  "\116vibrato_delay"              ,
  "\115vibrato_depth"              ,
  "\114vibrato_rate"               ,
};

static unsigned char *event_names[] = {
  "\040channel_prefix"     ,
  "\320channel_pressure"   ,
  "\260control_change"     ,
  "\002copyright_notice"   ,
  "\007cue_point"          ,
  "\010device_name"        ,
  "\057end_of_track"       ,
  "\004instrument_name"    ,
  "\240key_pressure"       ,
  "\131key_signature"      ,
  "\005lyric"              ,
  "\006marker"             ,
  "\377meta_event"         ,
  "\200note_off"           ,
  "\220note_on"            ,
  "\340pitch_bend"         ,
  "\041port_prefix"        ,
  "\300program_change"     ,
  "\011program_name"       ,
  "\003sequence_name"      ,
  "\000sequence_number"    ,
  "\177sequencer_specific" ,
  "\121set_tempo"          ,
  "\124smpte_offset"       ,
  "\367system_continue"    ,
  "\360system_exclusive"   ,
  "\001text_event"         ,
  "\130time_signature"     ,
  "\003track_name"         ,
};

int mf_eventbyname(char *name)
{
  unsigned char **q;

  q = bsearch(&name, event_names, sizeof(event_names)/sizeof(char *),
                                                       sizeof(char *), i_cmp);
  return  (q == NULL)? -1 : (int)(**q);
}

int mf_ctrlbyname(char *name)
{
  unsigned char **q;

  q = bsearch(&name, control_names, sizeof(control_names)/sizeof(char *),
                                                       sizeof(char *), i_cmp);
  return  (q == NULL)? -1 : (int)(**q);
}

#else
int mf_eventbyname(char *name) {return -1;}
int mf_ctrlbyname(char *name)  {return -1;}

#endif /* MIDLL_STRINGS */
#endif /* MIDLL_C */
