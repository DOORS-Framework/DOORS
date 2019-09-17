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
# Revision 1.4  2003/07/02 12:59:44  ik
# Comment lines disturbed automake.
#
# Revision 1.3  2003/03/10 11:29:15  ik
# IPv6 conditionals.
#
# Revision 1.2  2002/06/04 18:20:59  ik
# InetAddr6 class.
#
# Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
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

noinst_LTLIBRARIES = libsys.la

if HAVE_IPV6

libsys_la_SOURCES = \
addr.C \
buffer.C \
ctask.C \
etask.C \
handler.C \
iface.C \
inetaddr.C \
inetaddr6.C \
logger.C \
message.C \
nethdr.C \
ocore-inst.C \
osched.C \
otask.C \
pipeport.C \
port.C \
timer.C \
ttask.C \
unixaddr.C \
vstream.C

else

libsys_la_SOURCES = \
addr.C \
buffer.C \
ctask.C \
etask.C \
handler.C \
iface.C \
inetaddr.C \
logger.C \
message.C \
nethdr.C \
ocore-inst.C \
osched.C \
otask.C \
pipeport.C \
port.C \
timer.C \
ttask.C \
unixaddr.C \
vstream.C

endif

