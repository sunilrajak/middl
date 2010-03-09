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

static char *control_names[] = {
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

static char *event_names[] = {
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

int mf_i_cmp(const void *k, const void *b);

int mf_eventbyname(char *name)
{
  unsigned char **q;

  q = bsearch(&name, event_names, sizeof(event_names)/sizeof(char *),
                                                       sizeof(char *), mf_i_cmp);
  return  (q == NULL)? -1 : (int)(**q);
}

int mf_ctrlbyname(char *name)
{
  unsigned char **q;

  q = bsearch(&name, control_names, sizeof(control_names)/sizeof(char *),
                                                       sizeof(char *), mf_i_cmp);
  return  (q == NULL)? -1 : (int)(**q);
}

