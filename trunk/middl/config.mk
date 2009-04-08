# 
#  (C) by Remo Dentato (rdentato@gmail.com)
# 
# This softWare is distributed under the terms of the BSD license:
#   http://creativecommons.org/licenses/BSD/
#   http://opensource.org/licenses/bsd-license.php
#
###############

SYS=LINUX

ifneq "$(COMSPEC)" ""
 SYS=MINGW
endif

#SYS=MINGW

###############

AR=ar -ru
CAT=cat
RM=rm -f
CP=cp -f
LN=gcc $(LNFLAGS) -o

_LIB=.a
_OBJ=.o
_EXE=
MLIB=-lm

ifeq ($(SYS),MINGW)
_EXE=.exe
MLIB=
endif

##################

S=/

DIST=$(TOP)dist
LIBNAME=libmsq$(_LIB)
CHKLIB=$(DIST)$(S)$(LIBNAME)

CFLAGS=-I. -I$(DIST) -Wall $(CCOPTS)
LNFLAGS=-L. -L$(DIST)  $(LNOPTS)

.SUFFIXES: .c .h .o 

.c.o:
	$(CC) $(CFLAGS) -c -o $*.o $*.c
	
###########
