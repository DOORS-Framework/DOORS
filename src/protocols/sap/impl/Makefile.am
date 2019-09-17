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
# Revision 1.7  2003/05/23 20:48:36  ik
# SM, PDU and SAP rules moved to doors.mk and changes here accordingly.
#
# Revision 1.6  2003/05/17 20:18:17  ik
# Dependency fixes.
#
# Revision 1.5  2002/07/02 08:03:32  hartmanj
# Cleaned dependencies
#
# Revision 1.4  2002/07/01 12:09:17  hartmanj
# sapentity.h away from libsap_r_la_SOURCES
#
# Revision 1.3  2002/07/01 12:07:32  hartmanj
# sapsession.h away from libsap_r_la_SOURCES
#
# Revision 1.2  2002/06/18 14:16:15  ik
# Dependencies to code generators.
#
# Revision 1.1  2002/06/17 06:46:11  hartmanj
# Initial revision
#
#

include $(top_srcdir)/doors.mk

noinst_LTLIBRARIES = libsap_r.la

libsap_r_la_SOURCES = sapsession.C sapentity.C sapentitysm.C sapsessionsm.C
noinst_HEADERS = sapsession.h sapentity.h sapentitysm.h sapsessionsm.h

pkgdata_DATA = sapentitysm.sm sapsessionsm.sm

CLEANFILES = sapentitysm.C sapsessionsm.C sapentitysm.h sapsessionsm.h \
             $(top_builddir)/include/doors/sapentitysm.h \
             $(top_builddir)/include/doors/sapsessionsm.h

sapentity.C: sapentity.h sapentitysm.h
sapentitysm.C: sapentitysm.h
sapentitysm.h: sapentitysm.sm

sapsession.C: sapsession.h sapsessionsm.h
sapsessionsm.C: sapsessionsm.h sapentitysm.h
sapsessionsm.h: sapsessionsm.sm
