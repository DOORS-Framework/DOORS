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
# Revision 1.4  2004/04/05 07:01:46  hartmanj
# Renamed uamain* to uaent*
#
# Revision 1.3  2004/03/27 13:04:47  bilhanan
# removing uaapptask and uaapptasksm
#
# Revision 1.2  2004/03/08 10:26:11  bilhanan
# added uaconfigurator
#
# Revision 1.1  2003/05/27 09:49:03  ik
# Files moved.
#
#

include $(top_srcdir)/doors.mk

noinst_LTLIBRARIES = libslp6_ua.la

libslp6_ua_la_SOURCES = uaent.C uaconn.C uaentsm.C uaconnsm.C uaconfigurator.C

noinst_HEADERS = uaconn.h uaent.h \
                 uaconnsm.h uaentsm.h uaconfigurator.h

CLEANFILES = uaconnsm.C uaentsm.C \
             uaconnsm.h uaentsm.h \
             $(top_builddir)/include/doors/uaconnsm.h \
             $(top_builddir)/include/doors/aentsm.h

pkgdata_DATA = uaconnsm.sm uaentsm.sm

uaconfigurator.C: uaconfigurator.h
uaent.C: uaent.h uaentsm.h uaconnsm.h
uaentsm.C: uaentsm.h
uaentsm.h: uaentsm.sm

uaconn.C: uaconn.h uaconnsm.h
uaconnsm.C: uaconnsm.h
uaconnsm.h: uaconnsm.sm

