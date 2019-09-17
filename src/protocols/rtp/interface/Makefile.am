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
# Revision 1.9  2003/05/23 17:04:12  ik
# SAP rule moved to doors.mk and changes accordingly.
#
# Revision 1.8  2003/05/17 20:18:16  ik
# Dependency fixes.
#
# Revision 1.7  2003/05/17 15:13:46  ik
# Distribution tags.
#
# Revision 1.6  2002/07/23 16:34:29  hartmanj
#
# Development
#
# Revision 1.5  2002/07/23 14:28:31  bilhanan
# xcg fixes
#
# Revision 1.4  2002/07/23 13:23:19  hartmanj
# Fixes
#
# Revision 1.3  2002/07/23 11:40:19  bilhanan
# fix fix
#
# Revision 1.2  2002/07/23 11:22:01  bilhanan
# xcg modifications
#
# Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
# Development
#
# Revision 1.3  2002/02/25 17:52:40  ik
# sockaddr.* modifications.
#
# Revision 1.2  2001/06/08 16:46:17  bilhanan
# Full Licence clause into DOORS 0.2
#
#

include $(top_srcdir)/doors.mk

noinst_LTLIBRARIES = librtp.la

librtp_la_SOURCES = rtpsap.C
noinst_HEADERS = rtpsap.h

CLEANFILES = rtpsap.C rtpsap.h $(top_builddir)/include/doors/rtpsap.h

# Definition files are included in the distribution
pkgdata_DATA = rtpsap.sap

rtpsap.C: rtpsap.h
rtpsap.h: rtpsap.sap

