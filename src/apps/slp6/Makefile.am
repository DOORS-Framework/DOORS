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
# Revision 1.2  2004/10/12 12:23:12  bilhanan
# adding support for aa
#
# Revision 1.1  2003/07/10 11:34:40  bilhanan
# SLP6 apps added.
#
# Revision 1.2  2002/06/18 14:26:42  ik
# tools directory to tools-online.
#
# Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
# Development
#
#

include $(top_srcdir)/doors.mk

bin_PROGRAMS = da sa ua aa
da_SOURCES = da.C

da_LDADD = -L$(top_builddir)/tools-online -ldoors_tools \
                -L$(top_builddir)/protocols -ldoors_protocols \
                -L$(top_builddir)/base -ldoors_base

da_DEPENDENCIES = $(top_builddir)/tools-online/libdoors_tools.la \
                       $(top_builddir)/protocols/libdoors_protocols.la \
                       $(top_builddir)/base/libdoors_base.la


sa_SOURCES = sa.C

sa_LDADD = -L$(top_builddir)/tools-online -ldoors_tools \
                -L$(top_builddir)/protocols -ldoors_protocols \
                -L$(top_builddir)/base -ldoors_base

sa_DEPENDENCIES = $(top_builddir)/tools-online/libdoors_tools.la \
                       $(top_builddir)/protocols/libdoors_protocols.la \
                       $(top_builddir)/base/libdoors_base.la

ua_SOURCES = ua.C

ua_LDADD = -L$(top_builddir)/tools-online -ldoors_tools \
                -L$(top_builddir)/protocols -ldoors_protocols \
                -L$(top_builddir)/base -ldoors_base

ua_DEPENDENCIES = $(top_builddir)/tools-online/libdoors_tools.la \
                       $(top_builddir)/protocols/libdoors_protocols.la \
                       $(top_builddir)/base/libdoors_base.la

aa_SOURCES = aa.C

aa_LDADD = -L$(top_builddir)/tools-online -ldoors_tools \
                -L$(top_builddir)/protocols -ldoors_protocols \
                -L$(top_builddir)/base -ldoors_base

aa_DEPENDENCIES = $(top_builddir)/tools-online/libdoors_tools.la \
                       $(top_builddir)/protocols/libdoors_protocols.la \
                       $(top_builddir)/base/libdoors_base.la

