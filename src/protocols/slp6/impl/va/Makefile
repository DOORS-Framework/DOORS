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
# Revision 1.5  2007/01/15 12:40:47  bilhanan
# devt
#
# Revision 1.4  2007/01/15 11:03:19  bilhanan
# devt
#
# Revision 1.3  2007/01/11 21:16:27  bilhanan
# devt
#
# Revision 1.2  2007/01/11 21:10:45  bilhanan
# devt
#
# Revision 1.1  2006/08/10 15:39:05  bilhanan
# initial commit
#
# Revision 1.7  2004/07/12 07:26:00  hartmanj
# General development
#
# Revision 1.6  2004/06/17 08:56:29  hartmanj
# Development.
#
# Revision 1.5  2004/05/20 12:35:47  hartmanj
# Added -lmobapi and stuff to rule va:
#
# Revision 1.4  2004/05/20 07:03:11  hartmanj
# Manual fix to last comment.
#
# Revision 1.3  2004/05/19 13:52:01  hartmanj
# Update
#
# Revision 1.2  2004/05/12 08:33:19  hartmanj
# Development
#

DOORS_DIR=/Users/bilhanan/projects/slpmovementdetection/
DOORS_INCLUDES_DIR=-I${DOORS_DIR}/build/include/
DOORS_LIB_DIRS=-L${DOORS_DIR}/build/base/.libs/ -L${DOORS_DIR}/build/protocols/.libs/ -L${DOORS_DIR}/build/tools-online/.libs/
DOORS_LIBS=-ldoors_tools -ldoors_protocols -ldoors_base
CXXFLAGS=-g -W ${DOORS_INCLUDES_DIR}
VAOBJS=va.o vaent.o vaesm.o vuaconn.o vuaconnsm.o servicereg.o serviceregsm.o agentconn.o \
	agentconnsm.o vasap.o vaconfigurator.o serviceregsap.o \
	movenotifysap.o test_logger.o

all: va


serviceregsap.o: serviceregsap.C

test_logger.o: test_logger.C test_logger.h

va: vasap.h ${VAOBJS}
	g++  ${CXXFLAGS} ${DOORS_LIB_DIRS}  -o va ${VAOBJS} ${DOORS_LIBS}

va.o: va.C vasap.h vaesm.h serviceregsap.C movenotifysap.C

vasap.o: vasap.C

vasap.h : vasap.C

vasap.C: vasap.sap
	${DOORS_DIR}/build/tools-offline/xcg/dsapg/dsapg --cc-file vasap.C \
	--hh-file vasap.h vasap.sap

movenotifysap.o: movenotifysap.C

movenotifysap.h: movenotifysap.C

movenotifysap.C: movenotifysap.sap
	${DOORS_DIR}/build/tools-offline/xcg/dsapg/dsapg --cc-file movenotifysap.C \
	--hh-file movenotifysap.h movenotifysap.sap

serviceregsap.C: serviceregsap.sap
	${DOORS_DIR}/build/tools-offline/xcg/dsapg/dsapg --cc-file serviceregsap.C \
	--hh-file serviceregsap.h serviceregsap.sap


vaent.o: vaent.C vuaconnsm.C agentconnsm.C serviceregsm.C movenotifysap.C

vuaconn.o: vuaconn.C vuaconn.h

servicereg.o: servicereg.C servicereg.h

agentconn.o: agentconn.C agentconn.h serviceregsap.h

vaesm.o: vaesm.C

vuaconnsm.o: vuaconnsm.C

serviceregsm.o: serviceregsm.C

vaesm.h : vaesm.C

vaesm.C: vaesm.sm
	${DOORS_DIR}/build/tools-offline/xcg/dsmg/dsmg --cc-file vaesm.C \
	--hh-file vaesm.h vaesm.sm

vuaconnsm.C: vuaconnsm.sm
	${DOORS_DIR}/build/tools-offline/xcg/dsmg/dsmg --cc-file vuaconnsm.C \
	--hh-file vuaconnsm.h vuaconnsm.sm

serviceregsm.C: serviceregsm.sm
	${DOORS_DIR}/build/tools-offline/xcg/dsmg/dsmg --cc-file serviceregsm.C \
	--hh-file serviceregsm.h serviceregsm.sm

agentconnsm.C: agentconnsm.sm
	${DOORS_DIR}/build/tools-offline/xcg/dsmg/dsmg --cc-file agentconnsm.C \
	--hh-file agentconnsm.h agentconnsm.sm

clean:
	rm va *.o agentconnsm.C agentconnsm.h serviceregsap.C serviceregsap.h serviceregsm.C serviceregsm.h vaesm.C vaesm.h vasap.C vasap.h vuaconnsm.C vuaconnsm.h va_test_log.out movenotifysap.h movenotifysap.C
