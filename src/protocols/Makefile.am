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
# Revision 1.8  2004/10/12 12:18:06  bilhanan
# updated for tcpmux, tcp6mux and slp6/aa, removed tranmux build.
#
# Revision 1.7  2003/07/02 20:35:14  ik
# Altbit libraries added.
#
# Revision 1.6  2003/05/27 12:16:19  ik
# Development.
#
# Revision 1.5  2002/08/19 08:09:52  hartmanj
# Added protocols/tranmux to Makefiles
#
# Revision 1.4  2002/06/18 14:37:45  ik
# SAP library added.
#
# Revision 1.3  2002/06/06 21:53:13  ik
# Fixes to IPv6 UDP.
#
# Revision 1.2  2002/05/16 15:45:16  bilhanan
# General development
#
# Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
# Development
#
# Revision 1.2  2001/06/08 16:46:09  bilhanan
# Full Licence clause into DOORS 0.2
#
#

include $(top_srcdir)/doors.mk

lib_LTLIBRARIES = libdoors_protocols.la

if WITH_FINAL

libdoors_protocols_la_LIBADD = \
      $(top_builddir)/base/libdoors_base.la

else 

SUBDIRS = @PROTOSUBS@

libdoors_protocols_la_LIBADD = \
      $(top_builddir)/protocols/rtp/interface/librtp.la \
      $(top_builddir)/protocols/rtp/impl/librtp_r.la \
      $(top_builddir)/protocols/rtcp/interface/librtcp.la \
      $(top_builddir)/protocols/rtcp/impl/librtcp_r.la \
      $(top_builddir)/protocols/sap/interface/libsap.la \
      $(top_builddir)/protocols/sap/impl/libsap_r.la \
      $(top_builddir)/protocols/altbit/interface/libaltbit.la \
      $(top_builddir)/protocols/altbit/impl/libaltbit_r.la \
      $(top_builddir)/protocols/udp/libudp.la \
      @UDP6LIB@ @SLP6LIBS@ \
      $(top_builddir)/protocols/phys_virtual/interface/libphys.la \
      $(top_builddir)/protocols/phys_virtual/impl/libphys_v.la \
      $(top_builddir)/protocols/link/interface/liblink.la \
      $(top_builddir)/protocols/link/impl/liblink_r.la \
      $(top_builddir)/protocols/tcpmux/interface/libtcpmux.la \
      $(top_builddir)/protocols/tcpmux/impl/libtcpmux_r.la \
      $(top_builddir)/protocols/tcp6mux/interface/libtcp6mux.la \
      $(top_builddir)/protocols/tcp6mux/impl/libtcp6mux_r.la \
      $(top_builddir)/base/libdoors_base.la

endif

libdoors_protocols_la_SOURCES = protocols.C
libdoors_protocols_la_LDFLAGS = $(DOORSLDFLAGS)

libdoors_protocols_la_DEPENDENCIES = $(top_builddir)/base/libdoors_base.la


