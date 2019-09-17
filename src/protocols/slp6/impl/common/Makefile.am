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
# Revision 1.5  2004/04/07 13:41:32  hartmanj
# slpheader.h introduced
#
# Revision 1.4  2004/04/05 07:04:05  hartmanj
# Removed unnecessary files
#
# Revision 1.3  2004/02/26 09:45:02  hartmanj
# Changed serviceurl.C to include serviceurlparser.h instead of serviceurlparser.C
#
# Revision 1.2  2003/08/25 07:22:59  bilhanan
# removed slpmcasthelper & servercommon files
#
# Revision 1.1  2003/05/27 09:52:09  ik
# Files moved.
#
#

include $(top_srcdir)/doors.mk

PEERGFLAGS = --nomacros

noinst_LTLIBRARIES = libslp6_r.la

libslp6_r_la_SOURCES = configreader.C \
                       slpheader.C dafinder.C \
                       serviceurl.C slpcommon.C slppdu.C \
                       serviceurlparser.C

noinst_HEADERS = configreader.h  \
                 slptypes.h slpcommon.h \
                 dafinder.h serviceurl.h \
                 slppdu.h slpheader.h \
                 serviceurlparser.h

CLEANFILES = slppdu.h slppdu.i \
             $(top_builddir)/include/doors/slppdu.h \
             $(top_builddir)/include/doors/slppdu.i

pkgdata_DATA = slppdu.pdu

dafinder.C: slppdu.h
slppdu.C: slppdu.h slppdu.i
slppdu.h: slppdu.pdu


