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
# Revision 1.4  2002/06/18 15:16:28  ik
# directory tools->tools-online.
#
# Revision 1.3  2002/06/17 12:24:59  hartmanj
# Typo
#
# Revision 1.2  2002/06/17 10:53:38  hartmanj
# Added doors_tools dependency
#
# Revision 1.1  2002/06/14 16:02:25  hartmanj
# Initial
#

include $(top_srcdir)/doors.mk

bin_PROGRAMS = sapdemo
sapdemo_SOURCES = sapdemo.C

sapdemo_LDADD = -L$(top_builddir)/protocols -ldoors_protocols \
                -L$(top_builddir)/base -ldoors_base \
                -L$(top_builddir)/tools-online -ldoors_tools

sapdemo_DEPENDENCIES = $(top_builddir)/protocols/libdoors_protocols.la \
                       $(top_builddir)/base/libdoors_base.la \
                       $(top_builddir)/tools-online/libdoors_tools.la

