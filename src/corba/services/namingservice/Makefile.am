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
# Revision 1.9  2002/09/25 13:41:05  ik
# idl-compiler called in a wrong place.
#
# Revision 1.8  2002/08/09 09:37:15  ik
# ldapattributelist.C added to the list of files to compile.
#
# Revision 1.7  2002/07/31 12:22:20  ik
# Typos fixed.
#
# Revision 1.6  2002/07/04 08:21:37  hartmanj
# Development
#
# Revision 1.5  2002/07/01 11:19:22  hartmanj
# Development
#
# Revision 1.4  2002/07/01 11:17:11  hartmanj
# Development
#
# Revision 1.3  2002/05/17 14:37:46  ik
# Fixes to make it compile after directory structure changes.
#
# Revision 1.2  2002/05/16 15:45:15  bilhanan
# General development
#
# Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
# Development
#
# Revision 1.2  2001/06/08 16:46:15  bilhanan
# Full Licence clause into DOORS 0.2
#
#

include $(top_srcdir)/doors.mk

lib_LTLIBRARIES = libdoors_naming.la

libdoors_naming_la_SOURCES = cosnaming.C namingcontext_impl.C ldapcon.C slpwrap.C ldapattributelist.C

libdoors_naming_la_LIBADD = $(top_builddir)/corba/broker/libdoors_broker.la @MICOLIBS@

cosnaming.h: cosnaming.idl
	idl --any --poa --c++-suffix C $<

cosnaming.C: cosnaming.h

namingcontext_impl.h: cosnaming.h

namingcontext_impl.C: namingcontext_impl.h
