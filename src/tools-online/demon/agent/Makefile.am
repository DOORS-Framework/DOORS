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
# Revision 1.6  2003/04/16 14:32:40  ik
# Extra LDFLAGS line removed.
#
# Revision 1.5  2002/07/23 09:13:29  ik
# Path to dependencies.
#
# Revision 1.4  2002/06/18 12:28:29  ik
# Fixes due to directory structure changes.
#
# Revision 1.3  2002/05/17 14:37:54  ik
# Fixes to make it compile after directory structure changes.
#
# Revision 1.2  2002/05/17 11:57:11  ik
# Changes to Makefiles due to structure changes. agent.idl removed
# as it is not needed.
#
# Revision 1.1.1.1  2002/05/13 14:54:40  bilhanan
# Restructured
#
# Revision 1.4  2002/05/10 10:45:54  ik
# Changes for Text UI.
#
# Revision 1.3  2002/05/03 13:44:55  ik
# bootstrap
#
# Revision 1.2  2002/05/03 08:21:49  ik
# Paths fixed.
#
# Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
# Development
#
# Revision 1.2  2001/06/08 16:46:09  bilhanan
# Full Licence clause into DOORS 0.2
#
#

include $(top_srcdir)/doors.mk

lib_LTLIBRARIES = libdoors_demon.la

libdoors_demon_la_LDFLAGS = $(DOORSLDFLAGS)

libdoors_demon_la_SOURCES = agenttask.C demonhand.C demonhc.C \
                            demon.C

libdoors_demon_la_LIBADD = $(top_builddir)/base/libdoors_base.la \
                           @MICOLIBS@

libdoors_demon_la_DEPENDENCIES = $(top_builddir)/base/libdoors_base.la

demon.h: demon.idl
	@echo "Hack: Mico IDL-compiler translates included files in .idl-file"
	@echo "       to .h-files with absolute path which doesn't work if compiling"
	@echo "       takes place somewhere else than source directory. This is"
	@echo "       fixed by creating a symbolic link from build directory to"
	@echo "       source directory."
	@test -f demon.idl || \
	ln -s $(top_srcdir)/tools-online/demon/agent/demon.idl demon.idl
	@IDL@ --any --poa --c++-suffix C demon.idl

demon.C: demon.h

# These dependencies are written manually here because makedep is
# disabled and that is because automake fails to create dependencies
# for Sun compiler.

$(top_srcdir)/tools-online/demon/agent/agenttask.C: $(top_srcdir)/tools-online/demon/agent/agenttask.h
$(top_srcdir)/tools-online/demon/agent/agenttask.h: demon.h

$(top_srcdir)/tools-online/demon/manager/textuitask.C: $(top_srcdir)/tools-online/demon/manager/textuitask.h
$(top_srcdir)/tools-online/demon/manager/textuitask.h: demon.h

$(top_srcdir)/tools-online/demon/agent/demonhand.C: $(top_srcdir)/tools-online/demon/agent/demonhand.h
$(top_srcdir)/tools-online/demon/agent/demonhand.h: demon.h

