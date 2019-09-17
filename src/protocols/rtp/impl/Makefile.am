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
# Revision 1.16  2003/05/23 20:48:35  ik
# SM, PDU and SAP rules moved to doors.mk and changes here accordingly.
#
# Revision 1.15  2003/05/23 16:52:48  ik
# PDU, SM and SAP rules moved to doors.mk and changes accordingly.
#
# Revision 1.14  2003/05/17 20:18:16  ik
# Dependency fixes.
#
# Revision 1.13  2003/05/17 15:13:47  ik
# Distribution tags.
#
# Revision 1.12  2003/05/13 15:08:34  ik
# Buggy tag.
#
# Revision 1.11  2003/04/29 12:47:48  ik
# Dependency changes. Added symlink for rtppdu.pdu.
#
# Revision 1.10  2002/07/23 17:07:48  bilhanan
# *** empty log message ***
#
# Revision 1.9  2002/07/23 16:34:28  hartmanj
#
# Development
#
# Revision 1.8  2002/07/23 14:28:31  bilhanan
# xcg fixes
#
# Revision 1.7  2002/07/23 13:22:25  hartmanj
# Fixes
#
# Revision 1.6  2002/07/23 13:17:46  hartmanj
# Development
#
# Revision 1.5  2002/07/23 11:49:41  bilhanan
# xcg mods
#
# Revision 1.4  2002/07/23 11:38:01  bilhanan
# fix fix
#
# Revision 1.3  2002/07/23 11:33:58  bilhanan
# xcg fix
#
# Revision 1.2  2002/06/14 16:24:26  ik
# Changes for renaming of mcudp.{h,C}.
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

noinst_LTLIBRARIES = librtp_r.la

librtp_r_la_SOURCES = rtpudptask.C rtppdu.C rtpsource.C rtptask.C rtpsm.C
noinst_HEADERS = defs.h payload.h rtp.h rtpsource.h rtptask.h \
                       rtpudptask.h rtpsm.h rtppdu.h rtppdu.i

pkgdata_DATA = rtppdu.pdu rtpsm.sm

CLEANFILES = rtpsm.C rtpsm.h rtppdu.h rtppdu.i \
             $(top_builddir)/include/doors/rtpsm.h \
             $(top_builddir)/include/doors/rtppdu.h \
             $(top_builddir)/include/doors/rtppdu.i

rtptask.C: rtpsm.h
rtpsm.C: rtpsm.sm
rtpsm.h: rtpsm.sm 

rtppdu.C: rtppdu.h rtppdu.i
rtppdu.h: rtppdu.pdu

