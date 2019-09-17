#  Copyright 2000 
#  	Telecoms Lab, Tampere University of Technology.  All rights reserved.
#
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
#
# $Log: Makefile.am,v $
# Revision 1.1  2004/10/12 12:04:40  bilhanan
# IPv6 version of the protocol formerly known as tranmux
#
# Revision 1.3  2003/05/23 20:48:37  ik
# SM, PDU and SAP rules moved to doors.mk and changes here accordingly.
#
# Revision 1.2  2003/05/17 20:18:18  ik
# Dependency fixes.
#
# Revision 1.1  2002/08/19 08:10:49  hartmanj
# Initial revision
#
#
#

include $(top_srcdir)/doors.mk

noinst_LTLIBRARIES = libtcp6mux_r.la

libtcp6mux_r_la_SOURCES = tcp6muxent.C tcp6muxconn.C tcp6muxesm.C tcp6muxcsm.C
noinst_HEADERS = tcp6muxent.h tcp6muxconn.h tcp6muxesm.h tcp6muxcsm.h

pkgdata_DATA = tcp6muxesm.sm tcp6muxcsm.sm

CLEANFILES = tcp6muxesm.C tcp6muxcsm.C tcp6muxesm.h tcp6muxcsm.h \
             $(top_builddir)/include/doors/tcp6muxesm.h \
             $(top_builddir)/include/doors/tcp6muxcsm.h

tcp6muxent.C: tcp6muxent.h tcp6muxconn.h tcp6muxesm.h tcp6muxcsm.h
tcp6muxconn.C: tcp6muxconn.h tcp6muxcsm.h

tcp6muxesm.C: tcp6muxesm.h
tcp6muxesm.h: tcp6muxesm.sm

tcp6muxcsm.C: tcp6muxcsm.h
tcp6muxcsm.h: tcp6muxcsm.sm
