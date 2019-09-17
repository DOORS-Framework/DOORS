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
# Revision 1.1  2004/10/12 11:57:18  bilhanan
# AA code merged into main tree from src/experimental
#

include $(top_srcdir)/doors.mk

noinst_LTLIBRARIES = libslp6_aa.la

libslp6_aa_la_SOURCES = aaent.C uaproxy.C aaesm.C uaproxysm.C aaconfigurator.C

noinst_HEADERS = uaproxy.h aaent.h \
                 uaproxysm.h aaesm.h aaconfigurator.h

CLEANFILES = uaproxysm.C aaesm.C \
             uaproxysm.h aaesm.h \
             $(top_builddir)/include/doors/uaproxysm.h \
             $(top_builddir)/include/doors/aaesm.h

pkgdata_DATA = uaproxysm.sm aaesm.sm

aaconfigurator.C: aaconfigurator.h
aaent.C: aaent.h aaesm.h uaproxysm.h
aaesm.C: aaesm.h
aaesm.h: aaesm.sm

uaproxy.C: uaproxy.h uaproxysm.h
uaproxysm.C: uaproxysm.h
uaproxysm.h: uaproxysm.sm

