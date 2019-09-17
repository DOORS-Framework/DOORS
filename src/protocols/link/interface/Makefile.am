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
# Revision 1.5  2003/05/23 20:48:33  ik
# SM, PDU and SAP rules moved to doors.mk and changes here accordingly.
#
# Revision 1.4  2003/05/17 20:18:12  ik
# Dependency fixes.
#
# Revision 1.3  2003/05/17 15:13:46  ik
# Distribution tags.
#
# Revision 1.2  2002/07/23 11:41:06  bilhanan
# xcg fix
#
# Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
# Development
#
#

include $(top_srcdir)/doors.mk

noinst_LTLIBRARIES = liblink.la

liblink_la_SOURCES = link.C
noinst_HEADERS = link.h linksapbase.h

pkgdata_DATA = link.sap

CLEANFILES = link.C link.h $(top_builddir)/include/doors/link.h

link.C: link.h
link.h: link.sap
