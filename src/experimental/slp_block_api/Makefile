#  Copyright 2004
#      Telecoms Lab, Tampere University of Technology.  All rights reserved.

#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
#  THIS SOFTWARE IS PROVIDED BY TAMPERE UNIVERSITY OF TECHNOLOGY AND
#  CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
#  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#  FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY
#  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# $Log: Makefile,v $
# Revision 1.1  2004/08/03 05:31:07  hartmanj
# Initial revision
#
#

DOORS_DIR=/home/hartmanj/doors/
top_builddir=${DOORS_DIR}build/
DOORS_INCLUDES_DIR=-I${DOORS_DIR}/build/include/
DOORS_LIB_DIRS=-L${DOORS_DIR}/build/base/.libs/ \
	-L${DOORS_DIR}/build/protocols/.libs/
DOORS_LIBS=-ldoors_protocols -ldoors_base

CXXFLAGS=-g -W ${DOORS_INCLUDES_DIR}
OBJS=main.o doors_slp_api.o doorsmain.o uaplugin.o uapluginsm.o

all: slpfind

relink:
	g++ ${CXXFLAGS} ${DOORS_LIB_DIRS} -o slpfind ${OBJS} ${DOORS_LIBS} -lpthread

slpfind: ${OBJS}
	g++ ${CXXFLAGS} ${DOORS_LIB_DIRS} -o slpfind ${OBJS} ${DOORS_LIBS} -lpthread

main.o: main.C minislp.h

doors_slp_api.o: doors_slp_api.C minislp.h

doorsmain.o: doorsmain.C minislp.h

uaplugin.o: uaplugin.C uaplugin.h

uapluginsm.o: uapluginsm.C

uapluginsm.C: uaplugin.sm
	${DOORS_DIR}build/tools-offline/xcg/dsmg/dsmg --cc-file uapluginsm.C \
	--hh-file uapluginsm.h uaplugin.sm