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
# Revision 1.8  2003/07/09 15:34:07  bilhanan
# Makefiles modified, sbuf.h and sbuf.C moved from protocols/link to
# base/util
#
# Revision 1.7  2003/05/23 20:48:32  ik
# SM, PDU and SAP rules moved to doors.mk and changes here accordingly.
#
# Revision 1.6  2003/05/17 20:18:12  ik
# Dependency fixes.
#
# Revision 1.5  2002/07/23 17:08:37  bilhanan
# *** empty log message ***
#
# Revision 1.4  2002/07/23 16:34:10  hartmanj
#
# Development
#
# Revision 1.3  2002/07/23 14:28:31  bilhanan
# xcg fixes
#
# Revision 1.2  2002/07/23 11:49:41  bilhanan
# xcg mods
#
# Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
# Development
#
#

include $(top_srcdir)/doors.mk

noinst_LTLIBRARIES = liblink_r.la

liblink_r_la_SOURCES = linkent.C linkconn.C linkpdu.C \
                       linkcsm.C linkesm.C
noinst_HEADERS = linkent.h linkconn.h linkesm.h linkcsm.h linkpdu.h \
                 linkpdu.i

pkgdata_DATA = linkesm.sm linkcsm.sm linkpdu.pdu

CLEANFILES = linkesm.C linkcsm.C linkesm.h linkcsm.h linkpdu.h linkpdu.i \
             $(top_builddir)/include/doors/linkesm.h \
             $(top_builddir)/include/doors/linkcsm.h \
             $(top_builddir)/include/doors/linkpdu.i \
             $(top_builddir)/include/doors/linkpdu.h

linkent.C: linkent.h linkconn.h linkesm.h linkcsm.h
linkconn.C: linkconn.h linkesm.h linkcsm.h

linkesm.C: linkesm.h
linkesm.h: linkesm.sm

linkcsm.C: linkcsm.h
linkcsm.h: linkcsm.sm

linkconn.h: linkpdu.h
linkent.h: linkpdu.h
linkpdu.C: linkpdu.h linkpdu.i
linkpdu.h: linkpdu.pdu

