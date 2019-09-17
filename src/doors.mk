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
# $Log: doors.mk,v $
# Revision 1.10  2003/05/27 11:44:28  ik
# Added SMGFLAGS, SAPGFLAGS and PEERGFLAGS for setting extra flags.
#
# Revision 1.9  2003/05/26 11:16:17  ik
# Comment typo.
#
# Revision 1.8  2003/05/23 19:43:01  ik
# Symlinks to header files from include/doors.
#
# Revision 1.7  2003/05/23 17:00:11  ik
# General rules for SM, PDU and SAP files.
#
# Revision 1.6  2003/05/13 14:29:36  ik
# Changes due to new versions of autoconf, automake and libtool.
#
# Revision 1.5  2002/09/30 15:18:15  ik
# Qt checks.
#
# Revision 1.4  2002/07/23 07:09:00  ik
# Include path fixed.
#
# Revision 1.3  2002/05/17 14:37:38  ik
# Fixes to make it compile after directory structure changes.
#
# Revision 1.2  2002/05/03 11:24:31  ik
# Path fix.
#
# Revision 1.1.1.1  2002/04/30 12:32:48  bilhanan
# Development
#
#

# This file is for the global make rules

INCLUDES = @MICOINCS@ @LAMEINCS@ @QTINCS@ -I. -I$(top_srcdir) \
           -I$(top_builddir)/include \
           -I$(top_builddir)/corba/services/propertyservice \
           -I$(top_builddir)/corba/services/namingservice \
           -I$(top_builddir)/tools-online/demon/agent

MAINTAINERCLEANFILES = Makefile.in

DISTCLEANFILES = SunWS_cache

# If you do use only g++, please go ahead and comment this line
AUTOMAKE_OPTIONS = no-dependencies

DOORSLDFLAGS = -release @VERSION@ -no-undefined

SUFFIXES = .sm .pdu .sap

# General rule for creating foo.C from foo.sm
# Some makes may interpret $< without full path which would make this
# rule to fail. GNU make works.
.sm.C: $(top_builddir)/tools-offline/xcg/dsmg/dsmg
	$(top_builddir)/tools-offline/xcg/dsmg/dsmg --no-hhfile \
        $(SMGFLAGS) --hh-file $(@:.C=.h) --cc-file $@ $<

# General rule for creating foo.h from foo.sm
# Header file in this and other .h rules are symlinked to include/doors.
.sm.h: $(top_builddir)/tools-offline/xcg/dsmg/dsmg
	$(top_builddir)/tools-offline/xcg/dsmg/dsmg --hh-file $@ --no-ccfile \
        $(SMGFLAGS) --cc-file $(@:.h=.C) $<
	@test -f $(top_builddir)/include/doors/$@ || \
         $(LN_S) `pwd`/$@ $(top_builddir)/include/doors/$@

# General rule for creating foo.h from foo.pdu
.pdu.h: $(top_builddir)/tools-offline/xcg/dpeerg/dpeerg
	$(top_builddir)/tools-offline/xcg/dpeerg/dpeerg \
	$(PEERGFLAGS) --hh-file $@ --no-ifile --i-file $(@:.h=.i) $<
	@test -f $(top_builddir)/include/doors/$@ || \
         $(LN_S) `pwd`/$@ $(top_builddir)/include/doors/$@

# General rule for creating foo.i from foo.pdu
# Header .i file in this rule symlinked to include/doors.
.pdu.i: $(top_builddir)/tools-offline/xcg/dpeerg/dpeerg
	$(top_builddir)/tools-offline/xcg/dpeerg/dpeerg \
	$(PEERGFLAGS) --hh-file $(@:.i=.h) --no-hhfile --i-file $@ $<
	@test -f $(top_builddir)/include/doors/$@ || \
         $(LN_S) `pwd`/$@ $(top_builddir)/include/doors/$@

# General rule for creating foo.C from foo.sap
.sap.C: $(top_builddir)/tools-offline/xcg/dsapg/dsapg
	$(top_builddir)/tools-offline/xcg/dsapg/dsapg --no-hhfile \
	$(SAPGFLAGS) --hh-file $(@:.C=.h) --cc-file $@ $<

# General rule for creating foo.h from foo.sap
.sap.h: $(top_builddir)/tools-offline/xcg/dsapg/dsapg
	$(top_builddir)/tools-offline/xcg/dsapg/dsapg --no-ccfile \
	$(SAPGFLAGS) --hh-file $@ --cc-file $(@:.C=.h) $<
	@test -f $(top_builddir)/include/doors/$@ || \
         $(LN_S) `pwd`/$@ $(top_builddir)/include/doors/$@
