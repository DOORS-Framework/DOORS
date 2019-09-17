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
# Revision 1.3  2004/07/13 08:32:41  bilhanan
# initial version
#
# Revision 1.2  2004/05/26 06:28:23  hartmanj
# Fixed a type in rule uaproxysm.C:
#
# Revision 1.1  2004/05/25 09:39:21  hartmanj
# Initial versions
#
#

DOORS_DIR=/home/bilhanan/doors/
DOORS_INCLUDES_DIR=-I${DOORS_DIR}/build/include/
DOORS_LIB_DIRS=-L${DOORS_DIR}/build/base/.libs/ -L${DOORS_DIR}/build/protocols/.libs/ -L${DOORS_DIR}/build/tools-online/.libs/
DOORS_LIBS=-ldoors_tools -ldoors_protocols -ldoors_base
CXXFLAGS=-g -W ${DOORS_INCLUDES_DIR}
AAOBJS=aa.o aaent.o aaesm.o uaproxy.o uaproxysm.o aaconfigurator.o


all: aa

aa:  uaproxysm.h aaesm.h ${AAOBJS}
	g++  ${CXXFLAGS} ${DOORS_LIB_DIRS} -o aa ${AAOBJS} ${DOORS_LIBS}

aa.o: aa.C aaesm.h uaproxy.h aaconfigurator.h

aaent.o: aaent.C

uaproxy.o: uaproxy.C

aaconfigurator.o: aaconfigurator.C

aaesm.o: aaesm.C

uaproxysm.o: uaproxysm.C

aaesm.C: aaesm.h

aaesm.h: aaesm.sm
	${DOORS_DIR}build/tools-offline/xcg/dsmg/dsmg --cc-file aaesm.C \
	--hh-file aaesm.h aaesm.sm

uaproxysm.C: uaproxysm.h

uaproxysm.h: uaproxysm.sm
	${DOORS_DIR}build/tools-offline/xcg/dsmg/dsmg --cc-file uaproxysm.C \
	--hh-file uaproxysm.h uaproxysm.sm
