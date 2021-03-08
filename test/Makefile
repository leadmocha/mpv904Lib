#
# File:
#    Makefile
#
# Description:
#    Makefile for building the Pentland Systems Limited MPV 904 (DAC) library
#
#
BASENAME=mpv904
#
# Uncomment DEBUG line, to include some debugging info ( -g and -Wall)
DEBUG	?= 1
QUIET	?= 1
#
ifeq ($(QUIET),1)
        Q = @
else
        Q =
endif

ARCH	?= $(shell uname -m)

# Check for CODA 3 environment
ifdef CODA_VME

INC_CODA	= -I${CODA_VME}/include
LIB_CODA	= -L${CODA_VME_LIB}

endif

# Defs and build for PPC using VxWorks
ifeq (${ARCH}, PPC)
OS			= VXWORKS
VXWORKS_ROOT		?= /site/vxworks/5.5/ppc/target

ifdef LINUXVME_INC
VME_INCLUDE             ?= -I$(LINUXVME_INC)
endif

CC			= ccppc
LD			= ldppc
DEFS			= -mcpu=604 -DCPU=PPC604 -DVXWORKS -D_GNU_TOOL -mlongcall \
				-fno-for-scope -fno-builtin -fvolatile -DVXWORKSPPC
INCS			= -I. -I../ -I$(VXWORKS_ROOT)/h  \
				$(VME_INCLUDE) ${INC_CODA}
CFLAGS			= $(INCS) $(DEFS)
else
OS			= LINUX
endif

# Defs and build for i686, x86_64 Linux
ifeq ($(OS),LINUX)

# Safe defaults
LINUXVME_LIB		?= ../lib
LINUXVME_INC		?= ../include

CC			= gcc
ifeq ($(ARCH),i686)
CC			+= -m32
endif
AR                      = ar
RANLIB                  = ranlib
CFLAGS			= -L. -L${LINUXVME_LIB} ${LIB_CODA} -ljvme -L.. -l${BASENAME}
INCS			= -I. -I${LINUXVME_INC} ${INC_CODA} -I../

PROGS			= ${BASENAME}LibTest
endif #OS=LINUX#

ifeq ($(DEBUG),1)
CFLAGS			+= -Wall -Wno-unused -g
else
CFLAGS			+= -O2
endif
SRC			= ${BASENAME}LibTest.c
HDRS			= $(SRC:.c=.h)
OBJ			= $(SRC:.c=.o)
DEPS			= $(SRC:.c=.d)

all: $(PROGS)

%: %.c
	@echo " CC     $@"
	${Q}$(CC) $(CFLAGS) $(INCS) -o $@ $<

ifeq ($(OS),LINUX)
%.d: %.c
	@echo " DEP    $@"
	@set -e; rm -f $@; \
	$(CC) -MM -shared $(INCS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(DEPS)

endif

clean:
	@rm -vf ${OBJ} ${LIBS} ${DEPS} ${PROGS}

echoarch:
	@echo "Make for $(OS)-$(ARCH)"

.PHONY: clean echoarch
