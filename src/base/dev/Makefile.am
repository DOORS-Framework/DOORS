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
# Revision 1.4  2003/06/03 09:47:09  ik
# Headers were installed from here. Not anymore.
#
# Revision 1.3  2003/03/10 11:08:22  ik
# IPv6 conditional to Makefiles.
#
# Revision 1.2  2002/06/06 12:19:12  ik
# IPv6 UdpSockConn.
#
# Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
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

noinst_LTLIBRARIES = libdev.la

if HAVE_IPV6

libdev_la_SOURCES = \
device.C \
devif.C \
filedev.C \
hsockconn.C \
pipedev.C \
socentac.C \
sockconn.C \
sockent.C \
sockheader.C \
sockhelper.C \
sockmsg.C \
soconnac.C \
strdev.C \
ttydev.C \
udpsockconn.C \
udpsockconn6.C

noinst_HEADERS = \
device.h \
devif.h \
filedev.h \
hsockconn.h \
pipedev.h \
socentac.h \
sockconn.h \
sockent.h \
sockheader.h \
sockhelper.h \
sockmsg.h \
soconnac.h \
strdev.h \
ttydev.h \
udpsockconn.h \
udpsockconn6.h

else

libdev_la_SOURCES = \
device.C \
devif.C \
filedev.C \
hsockconn.C \
pipedev.C \
socentac.C \
sockconn.C \
sockent.C \
sockheader.C \
sockhelper.C \
sockmsg.C \
soconnac.C \
strdev.C \
ttydev.C \
udpsockconn.C 

noinst_HEADERS = \
device.h \
devif.h \
filedev.h \
hsockconn.h \
pipedev.h \
socentac.h \
sockconn.h \
sockent.h \
sockheader.h \
sockhelper.h \
sockmsg.h \
soconnac.h \
strdev.h \
ttydev.h \
udpsockconn.h

endif