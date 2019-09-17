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
# Revision 1.2  2002/05/17 14:37:39  ik
# Fixes to make it compile after directory structure changes.
#
# Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
# Development
#
# Revision 1.2  2001/06/08 16:46:15  bilhanan
# Full Licence clause into DOORS 0.2
#
#

include $(top_srcdir)/doors.mk

bin_PROGRAMS = broker client_in client_out

# broker
broker_SOURCES = broker.C cmb_impl.C CMB.C
broker_LDADD = -L$(top_builddir)/corba/broker -ldoors_broker \
	       -L$(top_builddir)/corba/services/namingservice -ldoors_naming \
               -L$(top_builddir)/base -ldoors_base \
               @MICOLIBS@
broker_DEPENDENCIES = $(top_builddir)/corba/broker/libdoors_broker.la \
                      $(top_builddir)/corba/services/namingservice/libdoors_naming.la \
                      $(top_builddir)/base/libdoors_base.la

# client_in
client_in_SOURCES = client_in.C cmb_impl.C CMB.C
client_in_LDADD = -L$(top_builddir)/corba/broker -ldoors_broker \
    	          -L$(top_builddir)/corba/services/namingservice -ldoors_naming \
                  -L$(top_builddir)/base -ldoors_base \
                  @MICOLIBS@
client_in_DEPENDENCIES = $(top_builddir)/corba/broker/libdoors_broker.la \
                         $(top_builddir)/corba/services/namingservice/libdoors_naming.la \
                         $(top_builddir)/base/libdoors_base.la

# client_out
client_out_SOURCES = client_out.C CMB.C
client_out_LDADD = -L$(top_builddir)/corba/broker -ldoors_broker \
	           -L$(top_builddir)/corba/services/namingservice -ldoors_naming \
                   -L$(top_builddir)/base -ldoors_base \
                   @MICOLIBS@
client_out_DEPENDENCIES = $(top_builddir)/corba/broker/libdoors_broker.la \
                          $(top_builddir)/corba/services/namingservice/libdoors_naming.la \
                          $(top_builddir)/base/libdoors_base.la


CMB.h: CMB.idl
	@IDL@ --any --poa --c++-suffix C $<

CMB.C: CMB.h

cmb_impl.h: CMB.h

cmb_impl.C: cmb_impl.h

broker.C: cmb_impl.h

client_in.C: cmb_impl.h

client_out.C: CMB.h
