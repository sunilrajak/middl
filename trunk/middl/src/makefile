# 
#  (C) by Remo Dentato (rdentato@gmail.com)
# 
# This software is distributed under the terms of the BSD license:
#   http://creativecommons.org/licenses/BSD/
#   http://opensource.org/licenses/bsd-license.php
#

# This makefile is for GNU tool chain it has been tested under 
# Windows (Mingw gcc 3.4.2) and Ubuntu Linux 8.04 (gcc 4.1.2)

TOP=../

include $(TOP)config.mk

HDRS= mf_start.h mf_instruments.h mf_chords.h mf_control.h
LIBOBJ = mf_strings$(_OBJ) mf_handlers$(_OBJ) mf_write$(_OBJ) mf_read$(_OBJ)

$(LIBNAME) : $(DIST)$(S)mf.h $(LIBOBJ) 
	$(AR) $@ $(LIBOBJ)
	$(CP) $@ $(DIST)

$(DIST)$(S)mf.h: mf.h
	$(CP) mf.h $(DIST)

mf.h: hdrs$(_EXE) $(HDRS)
	$(TOP)src/hdrs $(HDRS) > mf.h

hdrs$(_EXE): hdrs.c
	gcc -O2 -s -o $@ $<

clean:
	$(RM) $(DIST)$(S)mf.h
	$(RM) mf.h
	$(RM) $(LIBNAME)
	$(RM) $(DIST)$(S)$(LIBNAME)
	$(RM) $(LIBOBJ)
	$(RM) gmon.out
	$(RM) hdrs$(_EXE)

include $(TOP)targets.mk

	
