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

#ifndef MIDDL_H
#define MIDDL_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>

long mf_durationbyname(char *name);

extern short mf_qnote_ticks ;
extern short mf_num_tracks;
extern short mf_format;
extern char  mf_meter[4];

extern unsigned long mf_curtick;
extern unsigned long mf_endtick;


int mf_midi_event(unsigned long tick, short type, short chan,
                                                   short data1, short data2);
int mf_sys_event(unsigned long tick, short type, short aux,
                                      long len, unsigned char *data);
int mf_write(char *fname, short format, short ntracks, short division);
void mf_program_change(unsigned long tick, unsigned char chan, short instr);
void mf_humanize(short lw, short lq, short vw, short vq);
void mf_note_off(unsigned long tick, unsigned char chan, unsigned char pitch);
void mf_note_on(unsigned long tick, unsigned char chan,
                                      unsigned char pitch, char volume);
void mf_note(unsigned long tick, unsigned char chan, unsigned char pitch,
                                      long duration, char duty, char velocity);
void mf_chord(unsigned long tick, unsigned char chan, char *pitches,
                                      long duration, char duty, char velocity);
void mf_key_pressure(unsigned long tick, unsigned char chan, char pitch,
                                                                  char press);
void mf_channel_pressure(unsigned long tick,  unsigned char chan, char press);
void mf_control_change(unsigned long tick, unsigned char chan,
                                            unsigned short ctrl, short value);
void mf_pitch_bend(unsigned long tick, unsigned char chan, float bend);
void mf_sequence_number(unsigned long tick, short seqnum);
void mf_text_event(unsigned long tick, short type, char *str);
void mf_set_tempo(unsigned long tick, long tempo, short type);
void mf_key_signature(unsigned long tick, char key, char minmaj);
void mf_time_signature(unsigned long tick, char num, char den,
                                                    char clks, char q32nd);
void mf_sysex(unsigned long tick, short type, long len, char *data);
void mf_sequencer_specific(unsigned long tick, long len, char *data);

unsigned long mf_tick(unsigned long tick);
unsigned char mf_channel(unsigned char chan);

unsigned char mf_midinote(char note, char acc, char octave);

#define MF_GET_TICK 0xFFFFFFFF
#define MF_GET_CHANNEL 0xFF


