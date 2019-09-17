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
# Revision 1.13  2004/10/12 12:18:06  bilhanan
# updated for tcpmux, tcp6mux and slp6/aa, removed tranmux build.
#
# Revision 1.12  2004/04/07 13:41:07  hartmanj
# slpheader.h introduced
#
# Revision 1.11  2004/04/05 07:32:50  hartmanj
# Changes in protocols/slp6 filenames
#
# Revision 1.10  2004/03/27 13:04:47  bilhanan
# removing uaapptask and uaapptasksm
#
# Revision 1.9  2004/03/08 10:25:08  bilhanan
# added uaconfigurator, saconfigurator
#
# Revision 1.8  2004/03/01 16:51:10  bilhanan
# added daconfigurator
#
# Revision 1.7  2003/08/25 12:55:44  bilhanan
# removal of slpmcasthelper.h & servercommon.h
#
# Revision 1.6  2003/07/09 15:34:06  bilhanan
# Makefiles modified, sbuf.h and sbuf.C moved from protocols/link to
# base/util
#
# Revision 1.5  2003/07/02 14:32:50  ik
# Altbit added.
#
# Revision 1.4  2003/05/27 11:47:16  ik
# slp6 headers relocated. CLEANFILES tag added.
#
# Revision 1.3  2003/05/15 20:46:25  ik
# config.h removed.
#
# Revision 1.2  2003/05/14 15:16:01  ik
# Install rule fixes.
#
# Revision 1.1  2003/05/13 14:24:09  ik
# Initial revision.
#
# Revision 1.1.1.1  2002/04/30 12:32:48  bilhanan
# Development
#
# Revision 1.2  2001/06/08 16:46:09  bilhanan
# Full Licence clause into DOORS 0.2
#
#

# Add the static header files here
# You must add a symbolic link rule and a line to noinst_HEADERS for
# every single header file. No generated files here.


cmb_impl.h:
	@$(LN_S) $(top_srcdir)/apps/chatservice/cmb_impl.h cmb_impl.h

filempegrtp.h:
	@$(LN_S) $(top_srcdir)/apps/rtp/filempegrtp.h filempegrtp.h

framebuf.h:
	@$(LN_S) $(top_srcdir)/apps/rtp/framebuf.h framebuf.h

mp3sm.h:
	@$(LN_S) $(top_srcdir)/apps/rtp/mp3sm.h mp3sm.h

mp3task.h:
	@$(LN_S) $(top_srcdir)/apps/rtp/mp3task.h mp3task.h

mpegrtp.h:
	@$(LN_S) $(top_srcdir)/apps/rtp/mpegrtp.h mpegrtp.h

sounddev.h:
	@$(LN_S) $(top_srcdir)/apps/rtp/sounddev.h sounddev.h

udpapp.h:
	@$(LN_S) $(top_srcdir)/apps/udp/udpapp.h udpapp.h

udpappsm.h:
	@$(LN_S) $(top_srcdir)/apps/udp/udpappsm.h udpappsm.h

udpapptask.h:
	@$(LN_S) $(top_srcdir)/apps/udp/udpapptask.h udpapptask.h

udp6app.h:
	@$(LN_S) $(top_srcdir)/apps/udp6/udp6app.h udp6app.h

udp6appsm.h:
	@$(LN_S) $(top_srcdir)/apps/udp6/udp6appsm.h udp6appsm.h

udp6apptask.h:
	@$(LN_S) $(top_srcdir)/apps/udp6/udp6apptask.h udp6apptask.h

device.h:
	@$(LN_S) $(top_srcdir)/base/dev/device.h device.h

devif.h:
	@$(LN_S) $(top_srcdir)/base/dev/devif.h devif.h

filedev.h:
	@$(LN_S) $(top_srcdir)/base/dev/filedev.h filedev.h

hsockconn.h:
	@$(LN_S) $(top_srcdir)/base/dev/hsockconn.h hsockconn.h

pipedev.h:
	@$(LN_S) $(top_srcdir)/base/dev/pipedev.h pipedev.h

socentac.h:
	@$(LN_S) $(top_srcdir)/base/dev/socentac.h socentac.h

sockconn.h:
	@$(LN_S) $(top_srcdir)/base/dev/sockconn.h sockconn.h

sockent.h:
	@$(LN_S) $(top_srcdir)/base/dev/sockent.h sockent.h

sockheader.h:
	@$(LN_S) $(top_srcdir)/base/dev/sockheader.h sockheader.h

sockhelper.h:
	@$(LN_S) $(top_srcdir)/base/dev/sockhelper.h sockhelper.h

sockmsg.h:
	@$(LN_S) $(top_srcdir)/base/dev/sockmsg.h sockmsg.h

soconnac.h:
	@$(LN_S) $(top_srcdir)/base/dev/soconnac.h soconnac.h

strdev.h:
	@$(LN_S) $(top_srcdir)/base/dev/strdev.h strdev.h

ttydev.h:
	@$(LN_S) $(top_srcdir)/base/dev/ttydev.h ttydev.h

udpsockconn.h:
	@$(LN_S) $(top_srcdir)/base/dev/udpsockconn.h udpsockconn.h

udpsockconn6.h:
	@$(LN_S) $(top_srcdir)/base/dev/udpsockconn6.h udpsockconn6.h

hsi.h:
	@$(LN_S) $(top_srcdir)/base/hsi/hsi.h hsi.h

hsimacro.h:
	@$(LN_S) $(top_srcdir)/base/hsi/hsimacro.h hsimacro.h

hsitif.h:
	@$(LN_S) $(top_srcdir)/base/hsi/hsitif.h hsitif.h

macroif.h:
	@$(LN_S) $(top_srcdir)/base/hsi/macroif.h macroif.h

symbol.h:
	@$(LN_S) $(top_srcdir)/base/hsi/symbol.h symbol.h

symhand.h:
	@$(LN_S) $(top_srcdir)/base/hsi/symhand.h symhand.h

symhc.h:
	@$(LN_S) $(top_srcdir)/base/hsi/symhc.h symhc.h

symiter.h:
	@$(LN_S) $(top_srcdir)/base/hsi/symiter.h symiter.h

symtable.h:
	@$(LN_S) $(top_srcdir)/base/hsi/symtable.h symtable.h

atexit.h:
	@$(LN_S) $(top_srcdir)/base/mem/atexit.h atexit.h

bheader.h:
	@$(LN_S) $(top_srcdir)/base/mem/bheader.h bheader.h

blockm.h:
	@$(LN_S) $(top_srcdir)/base/mem/blockm.h blockm.h

blockmm.h:
	@$(LN_S) $(top_srcdir)/base/mem/blockmm.h blockmm.h

bstat.h:
	@$(LN_S) $(top_srcdir)/base/mem/bstat.h bstat.h

dmm.h:
	@$(LN_S) $(top_srcdir)/base/mem/dmm.h dmm.h

memmgr.h:
	@$(LN_S) $(top_srcdir)/base/mem/memmgr.h memmgr.h

memopt.h:
	@$(LN_S) $(top_srcdir)/base/mem/memopt.h memopt.h

plainmm.h:
	@$(LN_S) $(top_srcdir)/base/mem/plainmm.h plainmm.h

sbheader.h:
	@$(LN_S) $(top_srcdir)/base/mem/sbheader.h sbheader.h

stat.h:
	@$(LN_S) $(top_srcdir)/base/mem/stat.h stat.h

statbm.h:
	@$(LN_S) $(top_srcdir)/base/mem/statbm.h statbm.h

statbmm.h:
	@$(LN_S) $(top_srcdir)/base/mem/statbmm.h statbmm.h

statpmm.h:
	@$(LN_S) $(top_srcdir)/base/mem/statpmm.h statpmm.h

pdu.h:
	@$(LN_S) $(top_srcdir)/base/ptb/pdu.h pdu.h

peer.h:
	@$(LN_S) $(top_srcdir)/base/ptb/peer.h peer.h

portmux.h:
	@$(LN_S) $(top_srcdir)/base/ptb/portmux.h portmux.h

portset.h:
	@$(LN_S) $(top_srcdir)/base/ptb/portset.h portset.h

ptask.h:
	@$(LN_S) $(top_srcdir)/base/ptb/ptask.h ptask.h

ptb.h:
	@$(LN_S) $(top_srcdir)/base/ptb/ptb.h ptb.h

rttask.h:
	@$(LN_S) $(top_srcdir)/base/ptb/rttask.h rttask.h

sap.h:
	@$(LN_S) $(top_srcdir)/base/ptb/sap.h sap.h

sm.h:
	@$(LN_S) $(top_srcdir)/base/ptb/sm.h sm.h

addr.h:
	@$(LN_S) $(top_srcdir)/base/sys/addr.h addr.h

buffer.h:
	@$(LN_S) $(top_srcdir)/base/sys/buffer.h buffer.h

classes.h:
	@$(LN_S) $(top_srcdir)/base/sys/classes.h classes.h

core.h:
	@$(LN_S) $(top_srcdir)/base/sys/core.h core.h

ctask.h:
	@$(LN_S) $(top_srcdir)/base/sys/ctask.h ctask.h

etask.h:
	@$(LN_S) $(top_srcdir)/base/sys/etask.h etask.h

handler.h:
	@$(LN_S) $(top_srcdir)/base/sys/handler.h handler.h

iface.h:
	@$(LN_S) $(top_srcdir)/base/sys/iface.h iface.h

inetaddr.h:
	@$(LN_S) $(top_srcdir)/base/sys/inetaddr.h inetaddr.h

inetaddr6.h:
	@$(LN_S) $(top_srcdir)/base/sys/inetaddr6.h inetaddr6.h

library.h:
	@$(LN_S) $(top_srcdir)/base/sys/library.h library.h

logger.h:
	@$(LN_S) $(top_srcdir)/base/sys/logger.h logger.h

message.h:
	@$(LN_S) $(top_srcdir)/base/sys/message.h message.h

monitor.h:
	@$(LN_S) $(top_srcdir)/base/sys/monitor.h monitor.h

nethdr.h:
	@$(LN_S) $(top_srcdir)/base/sys/nethdr.h nethdr.h

ocoremac.h:
	@$(LN_S) $(top_srcdir)/base/sys/ocoremac.h ocoremac.h

osched.h:
	@$(LN_S) $(top_srcdir)/base/sys/osched.h osched.h

otask.h:
	@$(LN_S) $(top_srcdir)/base/sys/otask.h otask.h

othread.h:
	@$(LN_S) $(top_srcdir)/base/sys/othread.h othread.h

pipeport.h:
	@$(LN_S) $(top_srcdir)/base/sys/pipeport.h pipeport.h

port.h:
	@$(LN_S) $(top_srcdir)/base/sys/port.h port.h

timer.h:
	@$(LN_S) $(top_srcdir)/base/sys/timer.h timer.h

ttask.h:
	@$(LN_S) $(top_srcdir)/base/sys/ttask.h ttask.h

unixaddr.h:
	@$(LN_S) $(top_srcdir)/base/sys/unixaddr.h unixaddr.h

vstream.h:
	@$(LN_S) $(top_srcdir)/base/sys/vstream.h vstream.h

cell.h:
	@$(LN_S) $(top_srcdir)/base/util/cell.h cell.h

datatypes.h:
	@$(LN_S) $(top_srcdir)/base/util/datatypes.h datatypes.h

debug.h:
	@$(LN_S) $(top_srcdir)/base/util/debug.h debug.h

errmgmt.h:
	@$(LN_S) $(top_srcdir)/base/util/errmgmt.h errmgmt.h

frame.h:
	@$(LN_S) $(top_srcdir)/base/util/frame.h frame.h

genmac.h:
	@$(LN_S) $(top_srcdir)/base/util/genmac.h genmac.h

gnu_tmpls.h:
	@$(LN_S) $(top_srcdir)/base/util/gnu_tmpls.h gnu_tmpls.h

llist.h:
	@$(LN_S) $(top_srcdir)/base/util/llist.h llist.h

mlock.h:
	@$(LN_S) $(top_srcdir)/base/util/mlock.h mlock.h

name.h:
	@$(LN_S) $(top_srcdir)/base/util/name.h name.h

obl_dcl.h:
	@$(LN_S) $(top_srcdir)/base/util/obl_dcl.h obl_dcl.h

obl_process.h:
	@$(LN_S) $(top_srcdir)/base/util/obl_process.h obl_process.h

obl_string.h:
	@$(LN_S) $(top_srcdir)/base/util/obl_string.h obl_string.h

obl_time.h:
	@$(LN_S) $(top_srcdir)/base/util/obl_time.h obl_time.h

otime.h:
	@$(LN_S) $(top_srcdir)/base/util/otime.h otime.h

synonym.h:
	@$(LN_S) $(top_srcdir)/base/util/synonym.h synonym.h

tqueue.h:
	@$(LN_S) $(top_srcdir)/base/util/tqueue.h tqueue.h

util.h:
	@$(LN_S) $(top_srcdir)/base/util/util.h util.h

vdebug.h:
	@$(LN_S) $(top_srcdir)/base/util/vdebug.h vdebug.h

ldapattributelist.h:
	@$(LN_S) $(top_srcdir)/corba/services/namingservice/ldapattributelist.h ldapattributelist.h

ldapcon.h:
	@$(LN_S) $(top_srcdir)/corba/services/namingservice/ldapcon.h ldapcon.h

namingcontext_impl.h:
	@$(LN_S) $(top_srcdir)/corba/services/namingservice/namingcontext_impl.h namingcontext_impl.h

slpwrap.h:
	@$(LN_S) $(top_srcdir)/corba/services/namingservice/slpwrap.h slpwrap.h

PropertyService_impl.h:
	@$(LN_S) $(top_srcdir)/corba/services/propertyservice/PropertyService_impl.h PropertyService_impl.h

linkconn.h:
	@$(LN_S) $(top_srcdir)/protocols/link/impl/linkconn.h linkconn.h

linkent.h:
	@$(LN_S) $(top_srcdir)/protocols/link/impl/linkent.h linkent.h

sbuf.h:
	@$(LN_S) $(top_srcdir)/base/util/sbuf.h sbuf.h

linksapbase.h:
	@$(LN_S) $(top_srcdir)/protocols/link/interface/linksapbase.h linksapbase.h

rtcp.h:
	@$(LN_S) $(top_srcdir)/protocols/rtcp/impl/rtcp.h rtcp.h

rtcptask.h:
	@$(LN_S) $(top_srcdir)/protocols/rtcp/impl/rtcptask.h rtcptask.h

rtcpbase.h:
	@$(LN_S) $(top_srcdir)/protocols/rtcp/interface/rtcpbase.h rtcpbase.h

physent.h:
	@$(LN_S) $(top_srcdir)/protocols/phys_virtual/impl/physent.h physent.h

defs.h:
	@$(LN_S) $(top_srcdir)/protocols/rtp/impl/defs.h defs.h

payload.h:
	@$(LN_S) $(top_srcdir)/protocols/rtp/impl/payload.h payload.h

rtp.h:
	@$(LN_S) $(top_srcdir)/protocols/rtp/impl/rtp.h rtp.h

rtpsource.h:
	@$(LN_S) $(top_srcdir)/protocols/rtp/impl/rtpsource.h rtpsource.h

rtptask.h:
	@$(LN_S) $(top_srcdir)/protocols/rtp/impl/rtptask.h rtptask.h

rtpudptask.h:
	@$(LN_S) $(top_srcdir)/protocols/rtp/impl/rtpudptask.h rtpudptask.h

altbittask.h:
	@$(LN_S) $(top_srcdir)/protocols/altbit/impl/altbittask.h altbittask.h

sapentity.h:
	@$(LN_S) $(top_srcdir)/protocols/sap/impl/sapentity.h sapentity.h

sapsession.h:
	@$(LN_S) $(top_srcdir)/protocols/sap/impl/sapsession.h sapsession.h

daent.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/da/daent.h daent.h

daconfigurator.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/da/daconfigurator.h daconfigurator.h

configreader.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/common/configreader.h configreader.h

constants.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/common/constants.h constants.h

dafinder.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/common/dafinder.h dafinder.h

serviceurl.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/common/serviceurl.h serviceurl.h

slpcommon.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/common/slpcommon.h slpcommon.h

slptypes.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/common/slptypes.h slptypes.h

slpheader.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/common/slpheader.h slpheader.h

saent.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/sa/saent.h saent.h

saconfigurator.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/sa/saconfigurator.h saconfigurator.h

sareg.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/sa/sareg.h sareg.h

uaconn.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/ua/uaconn.h uaconn.h

uaent.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/ua/uaent.h uaent.h

uaconfigurator.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/ua/uaconfigurator.h uaconfigurator.h

uaproxy.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/aa/uaproxy.h uaproxy.h

aaent.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/aa/aaent.h aaent.h

aaconfigurator.h:
	@$(LN_S) $(top_srcdir)/protocols/slp6/impl/aa/aaconfigurator.h aaconfigurator.h

tcpmuxconn.h:
	@$(LN_S) $(top_srcdir)/protocols/tcpmux/impl/tcpmuxconn.h tcpmuxconn.h

tcpmuxent.h:
	@$(LN_S) $(top_srcdir)/protocols/tcpmux/impl/tcpmuxent.h tcpmuxent.h

tcpmuxbase.h:
	@$(LN_S) $(top_srcdir)/protocols/tcpmux/interface/tcpmuxbase.h tcpmuxbase.h

tcp6muxconn.h:
	@$(LN_S) $(top_srcdir)/protocols/tcp6mux/impl/tcp6muxconn.h tcp6muxconn.h

tcp6muxent.h:
	@$(LN_S) $(top_srcdir)/protocols/tcp6mux/impl/tcp6muxent.h tcp6muxent.h

tcp6muxbase.h:
	@$(LN_S) $(top_srcdir)/protocols/tcp6mux/interface/tcp6muxbase.h tcp6muxbase.h

udp.h:
	@$(LN_S) $(top_srcdir)/protocols/udp/udp.h udp.h

udptask.h:
	@$(LN_S) $(top_srcdir)/protocols/udp/udptask.h udptask.h

udp6.h:
	@$(LN_S) $(top_srcdir)/protocols/udp6/udp6.h udp6.h

udp6task.h:
	@$(LN_S) $(top_srcdir)/protocols/udp6/udp6task.h udp6task.h

SAX.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/SAX.h SAX.h

c14n.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/c14n.h c14n.h

catalog.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/catalog.h catalog.h

	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/debugXML.h debugXML.h

encoding.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/encoding.h encoding.h

entities.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/entities.h entities.h

globals.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/globals.h globals.h

hash.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/hash.h hash.h

libxml.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/libxml.h libxml.h

list.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/list.h list.h

parser.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/parser.h parser.h

parserInternals.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/parserInternals.h parserInternals.h

threads.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/threads.h threads.h

tree.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/tree.h tree.h

uri.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/uri.h uri.h

valid.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/valid.h valid.h

xinclude.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/xinclude.h xinclude.h

xlink.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/xlink.h xlink.h

xmlIO.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/xmlIO.h xmlIO.h

xmlerror.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/xmlerror.h xmlerror.h

xmlmemory.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/xmlmemory.h xmlmemory.h

xmlversion.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/xmlversion.h xmlversion.h

xmlwin32version.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/xmlwin32version.h xmlwin32version.h

xpath.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/xpath.h xpath.h

xpathInternals.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/xpathInternals.h xpathInternals.h

xpointer.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/parser/xpointer.h xpointer.h

interface.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/interface.h interface.h

structure.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/structure.h structure.h

xcgmessage.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/xcgmessage.h xcgmessage.h

xmldocument.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/common/xmldocument.h xmldocument.h

peergenerator.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/dpeerg/peergenerator.h peergenerator.h

peerinterface.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/dpeerg/peerinterface.h peerinterface.h

peermessage.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/dpeerg/peermessage.h peermessage.h

xcgpeer.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/dpeerg/xcgpeer.h xcgpeer.h

sapgenerator.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/dsapg/sapgenerator.h sapgenerator.h

sapinterface.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/dsapg/sapinterface.h sapinterface.h

sapmessage.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/dsapg/sapmessage.h sapmessage.h

xcgsap.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/dsapg/xcgsap.h xcgsap.h

dsminterface.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/dsmg/dsminterface.h dsminterface.h

event.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/dsmg/event.h event.h

smgenerator.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/dsmg/smgenerator.h smgenerator.h

state.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/dsmg/state.h state.h

xcgsm.h:
	@$(LN_S) $(top_srcdir)/tools-offline/xcg/dsmg/xcgsm.h xcgsm.h

agenttask.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/agent/agenttask.h agenttask.h

demoncore.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/agent/demoncore.h demoncore.h

demonhand.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/agent/demonhand.h demonhand.h

demonhc.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/agent/demonhc.h demonhc.h

agentevent.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/agentevent.h agentevent.h

agentlist.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/agentlist.h agentlist.h

agentlist.ui.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/agentlist.ui.h agentlist.ui.h

agentlistitem.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/agentlistitem.h agentlistitem.h

agentsymbol.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/agentsymbol.h agentsymbol.h

agentwin.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/agentwin.h agentwin.h

agentwin.ui.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/agentwin.ui.h agentwin.ui.h

corbaevent.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/corbaevent.h corbaevent.h

linewidget.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/linewidget.h linewidget.h

managerevent.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/managerevent.h managerevent.h

managericons.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/managericons.h managericons.h

managermain.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/managermain.h managermain.h

managermain.ui.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/managermain.ui.h managermain.ui.h

messagelabel.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/messagelabel.h messagelabel.h

mgrcorbaclnt.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/mgrcorbaclnt.h mgrcorbaclnt.h

mgrcorbasrvr.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/mgrcorbasrvr.h mgrcorbasrvr.h

mgrslpclnt.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/mgrslpclnt.h mgrslpclnt.h

slpevent.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/slpevent.h slpevent.h

symboltools.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/symboltools.h symboltools.h

systemview.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/systemview.h systemview.h

systemview.ui.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/systemview.ui.h systemview.ui.h

traceview.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/traceview.h traceview.h

tracewin.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/tracewin.h tracewin.h

tracewin.ui.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/qt/tracewin.ui.h tracewin.ui.h

managertask.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/managertask.h managertask.h

textuitask.h:
	@$(LN_S) $(top_srcdir)/tools-online/demon/manager/textuitask.h textuitask.h

alias.h:
	@$(LN_S) $(top_srcdir)/tools-online/lemon/alias.h alias.h

command.h:
	@$(LN_S) $(top_srcdir)/tools-online/lemon/command.h command.h

dsched.h:
	@$(LN_S) $(top_srcdir)/tools-online/lemon/dsched.h dsched.h

editask.h:
	@$(LN_S) $(top_srcdir)/tools-online/lemon/editask.h editask.h

envtask.h:
	@$(LN_S) $(top_srcdir)/tools-online/lemon/envtask.h envtask.h

inputask.h:
	@$(LN_S) $(top_srcdir)/tools-online/lemon/inputask.h inputask.h

lemon.h:
	@$(LN_S) $(top_srcdir)/tools-online/lemon/lemon.h lemon.h

lemonhand.h:
	@$(LN_S) $(top_srcdir)/tools-online/lemon/lemonhand.h lemonhand.h

lemonhc.h:
	@$(LN_S) $(top_srcdir)/tools-online/lemon/lemonhc.h lemonhc.h

otl_base.h:
	@$(LN_S) $(top_srcdir)/tools-online/lemon/otl_base.h otl_base.h

otl_dcl.h:
	@$(LN_S) $(top_srcdir)/tools-online/lemon/otl_dcl.h otl_dcl.h

otl_opt.h:
	@$(LN_S) $(top_srcdir)/tools-online/lemon/otl_opt.h otl_opt.h

otl_record.h:
	@$(LN_S) $(top_srcdir)/tools-online/lemon/otl_record.h otl_record.h

uitask.h:
	@$(LN_S) $(top_srcdir)/tools-online/lemon/uitask.h uitask.h

noinst_HEADERS = \
cmb_impl.h \
filempegrtp.h \
framebuf.h \
mp3sm.h \
mp3task.h \
mpegrtp.h \
sounddev.h \
udpapp.h \
udpappsm.h \
udpapptask.h \
udp6app.h \
udp6appsm.h \
udp6apptask.h \
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
udpsockconn6.h \
hsi.h \
hsimacro.h \
hsitif.h \
macroif.h \
symbol.h \
symhand.h \
symhc.h \
symiter.h \
symtable.h \
atexit.h \
bheader.h \
blockm.h \
blockmm.h \
bstat.h \
dmm.h \
memmgr.h \
memopt.h \
plainmm.h \
sbheader.h \
stat.h \
statbm.h \
statbmm.h \
statpmm.h \
pdu.h \
peer.h \
portmux.h \
portset.h \
ptask.h \
ptb.h \
rttask.h \
sap.h \
sm.h \
addr.h \
buffer.h \
classes.h \
core.h \
ctask.h \
etask.h \
handler.h \
iface.h \
inetaddr.h \
inetaddr6.h \
library.h \
logger.h \
message.h \
monitor.h \
nethdr.h \
ocoremac.h \
osched.h \
otask.h \
othread.h \
pipeport.h \
port.h \
timer.h \
ttask.h \
unixaddr.h \
vstream.h \
cell.h \
datatypes.h \
debug.h \
errmgmt.h \
frame.h \
genmac.h \
gnu_tmpls.h \
llist.h \
mlock.h \
name.h \
obl_dcl.h \
obl_process.h \
obl_string.h \
obl_time.h \
otime.h \
synonym.h \
tqueue.h \
util.h \
vdebug.h \
ldapattributelist.h \
ldapcon.h \
namingcontext_impl.h \
slpwrap.h \
PropertyService_impl.h \
linkconn.h \
linkent.h \
sbuf.h \
linksapbase.h \
rtcp.h \
rtcptask.h \
rtcpbase.h \
physent.h \
defs.h \
payload.h \
rtp.h \
rtpsource.h \
rtptask.h \
rtpudptask.h \
altbittask.h \
sapentity.h \
sapsession.h \
daent.h \
daconfigurator.h \
configreader.h \
constants.h \
dafinder.h \
serviceurl.h \
slpcommon.h \
slptypes.h \
slpheader.h \
saent.h \
saconfigurator.h \
sareg.h \
uaconn.h \
uaent.h \
uaconfigurator.h \
uaproxy.h \
aaent.h \
aaconfigurator.h \
tcpmuxconn.h \
tcpmuxent.h \
tcpmuxbase.h \
tcp6muxconn.h \
tcp6muxent.h \
tcp6muxbase.h \
udp.h \
udptask.h \
udp6.h \
udp6task.h \
SAX.h \
c14n.h \
catalog.h \
debugXML.h \
encoding.h \
entities.h \
globals.h \
hash.h \
libxml.h \
list.h \
parser.h \
parserInternals.h \
threads.h \
tree.h \
uri.h \
valid.h \
xinclude.h \
xlink.h \
xmlIO.h \
xmlerror.h \
xmlmemory.h \
xmlversion.h \
xmlwin32version.h \
xpath.h \
xpathInternals.h \
xpointer.h \
interface.h \
structure.h \
xcgmessage.h \
xmldocument.h \
peergenerator.h \
peerinterface.h \
peermessage.h \
xcgpeer.h \
sapgenerator.h \
sapinterface.h \
sapmessage.h \
xcgsap.h \
dsminterface.h \
event.h \
smgenerator.h \
state.h \
xcgsm.h \
agenttask.h \
demoncore.h \
demonhand.h \
demonhc.h \
agentevent.h \
agentlist.h \
agentlist.ui.h \
agentlistitem.h \
agentsymbol.h \
agentwin.h \
agentwin.ui.h \
corbaevent.h \
linewidget.h \
managerevent.h \
managericons.h \
managermain.h \
managermain.ui.h \
messagelabel.h \
mgrcorbaclnt.h \
mgrcorbasrvr.h \
mgrslpclnt.h \
slpevent.h \
symboltools.h \
systemview.h \
systemview.ui.h \
traceview.h \
tracewin.h \
tracewin.ui.h \
managertask.h \
textuitask.h \
alias.h \
command.h \
dsched.h \
editask.h \
envtask.h \
inputask.h \
lemon.h \
lemonhand.h \
lemonhc.h \
otl_base.h \
otl_dcl.h \
otl_opt.h \
otl_record.h \
uitask.h 

# Copy the headers from noinst_HEADERS here
CLEANFILES = \
cmb_impl.h \
filempegrtp.h \
framebuf.h \
mp3sm.h \
mp3task.h \
mpegrtp.h \
sounddev.h \
udpapp.h \
udpappsm.h \
udpapptask.h \
udp6app.h \
udp6appsm.h \
udp6apptask.h \
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
udpsockconn6.h \
hsi.h \
hsimacro.h \
hsitif.h \
macroif.h \
symbol.h \
symhand.h \
symhc.h \
symiter.h \
symtable.h \
atexit.h \
bheader.h \
blockm.h \
blockmm.h \
bstat.h \
dmm.h \
memmgr.h \
memopt.h \
plainmm.h \
sbheader.h \
stat.h \
statbm.h \
statbmm.h \
statpmm.h \
pdu.h \
peer.h \
portmux.h \
portset.h \
ptask.h \
ptb.h \
rttask.h \
sap.h \
sm.h \
addr.h \
buffer.h \
classes.h \
core.h \
ctask.h \
etask.h \
handler.h \
iface.h \
inetaddr.h \
inetaddr6.h \
library.h \
logger.h \
message.h \
monitor.h \
nethdr.h \
ocoremac.h \
osched.h \
otask.h \
othread.h \
pipeport.h \
port.h \
timer.h \
ttask.h \
unixaddr.h \
vstream.h \
cell.h \
datatypes.h \
debug.h \
errmgmt.h \
frame.h \
genmac.h \
gnu_tmpls.h \
llist.h \
mlock.h \
name.h \
obl_dcl.h \
obl_process.h \
obl_string.h \
obl_time.h \
otime.h \
synonym.h \
tqueue.h \
util.h \
vdebug.h \
ldapattributelist.h \
ldapcon.h \
namingcontext_impl.h \
slpwrap.h \
PropertyService_impl.h \
linkconn.h \
linkent.h \
sbuf.h \
linksapbase.h \
rtcp.h \
rtcptask.h \
rtcpbase.h \
physent.h \
defs.h \
payload.h \
rtp.h \
rtpsource.h \
rtptask.h \
rtpudptask.h \
altbittask.h \
sapentity.h \
sapsession.h \
daent.h \
daconfigurator.h \
configreader.h \
constants.h \
dafinder.h \
serviceurl.h \
slpcommon.h \
slptypes.h \
slpheader.h \
saent.h \
saconfigurator.h \
sareg.h \
uaconn.h \
uaent.h \
uaconfigurator.h \
uaproxy.h \
aaent.h \
aaconfigurator.h \
tcpmuxconn.h \
tcpmuxent.h \
tcpmuxbase.h \
tcp6muxconn.h \
tcp6muxent.h \
tcp6muxbase.h \
udp.h \
udptask.h \
udp6.h \
udp6task.h \
SAX.h \
c14n.h \
catalog.h \
debugXML.h \
encoding.h \
entities.h \
globals.h \
hash.h \
libxml.h \
list.h \
parser.h \
parserInternals.h \
threads.h \
tree.h \
uri.h \
valid.h \
xinclude.h \
xlink.h \
xmlIO.h \
xmlerror.h \
xmlmemory.h \
xmlversion.h \
xmlwin32version.h \
xpath.h \
xpathInternals.h \
xpointer.h \
interface.h \
structure.h \
xcgmessage.h \
xmldocument.h \
peergenerator.h \
peerinterface.h \
peermessage.h \
xcgpeer.h \
sapgenerator.h \
sapinterface.h \
sapmessage.h \
xcgsap.h \
dsminterface.h \
event.h \
smgenerator.h \
state.h \
xcgsm.h \
agenttask.h \
demoncore.h \
demonhand.h \
demonhc.h \
agentevent.h \
agentlist.h \
agentlist.ui.h \
agentlistitem.h \
agentsymbol.h \
agentwin.h \
agentwin.ui.h \
corbaevent.h \
linewidget.h \
managerevent.h \
managericons.h \
managermain.h \
managermain.ui.h \
messagelabel.h \
mgrcorbaclnt.h \
mgrcorbasrvr.h \
mgrslpclnt.h \
slpevent.h \
symboltools.h \
systemview.h \
systemview.ui.h \
traceview.h \
tracewin.h \
tracewin.ui.h \
managertask.h \
textuitask.h \
alias.h \
command.h \
dsched.h \
editask.h \
envtask.h \
inputask.h \
lemon.h \
lemonhand.h \
lemonhc.h \
otl_base.h \
otl_dcl.h \
otl_opt.h \
otl_record.h \
uitask.h 
