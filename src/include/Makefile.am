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
# Revision 1.14  2004/10/12 12:18:05  bilhanan
# updated for tcpmux, tcp6mux and slp6/aa, removed tranmux build.
#
# Revision 1.13  2004/04/07 13:41:06  hartmanj
# slpheader.h introduced
#
# Revision 1.12  2004/04/05 07:12:06  hartmanj
# Changes in filenames in protocols/slp6/ua,sa,da/
#
# Revision 1.11  2004/03/27 13:04:46  bilhanan
# removing uaapptask and uaapptasksm
#
# Revision 1.10  2004/03/08 10:25:07  bilhanan
# added uaconfigurator, saconfigurator
#
# Revision 1.9  2004/03/01 16:51:09  bilhanan
# added daconfigurator
#
# Revision 1.8  2004/01/19 18:03:35  bilhanan
# make install support for doorsconf.h added
#
# Revision 1.7  2003/08/25 12:55:43  bilhanan
# removal of slpmcasthelper.h & servercommon.h
#
# Revision 1.6  2003/07/02 20:36:17  ik
# Altbit files added.
#
# Revision 1.5  2003/07/02 14:32:50  ik
# Altbit added.
#
# Revision 1.4  2003/05/27 11:47:43  ik
# Development.
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


# Add headers that are to be installed to this file


SUBDIRS = doors

# Add static headers that are not to be installed here
#noinst_HEADERS = \
#doors/agentevent.h \
#doors/agentlist.h \
#doors/agentlist.ui.h \
#doors/agentlistitem.h \
#doors/agentsymbol.h \
#doors/agentwin.h \
#doors/agentwin.ui.h

# Add a dummy rule here for each of the generated files to make sure that
# 'make' works before these files are actually generated
doors/linkesm.h:
doors/linkcsm.h:
doors/linkpdu.h:
doors/linkpdu.i:
doors/link.h:
doors/physsm.h:
doors/phys.h:
doors/rtcpsm.h:
doors/rtcppdu.h:
doors/rtcppdu.i:
doors/rtcpsap.h:
doors/rtpsm.h:
doors/rtppdu.h:
doors/rtppdu.i:
doors/rtpsap.h:
doors/sapentitysm.h:
doors/sapsessionsm.h:
doors/sapentitysap.h:
doors/tcpmuxesm.h:
doors/tcpmuxcsm.h:
doors/tcpmux.h:
doors/tcp6muxesm.h:
doors/tcp6muxcsm.h:
doors/tcp6mux.h:
doors/altbitpdu.h:
doors/altbitpdu.i:
doors/altbitsm.h:
doors/altbitsap.h:
doors/daentsm.h:
doors/doorsconf.h:

# Add all the generated header files here
nobase_include_HEADERS = \
doors/linkesm.h \
doors/linkcsm.h \
doors/linkpdu.h \
doors/linkpdu.i \
doors/link.h \
doors/physsm.h \
doors/phys.h \
doors/rtcpsm.h \
doors/rtcppdu.h \
doors/rtcppdu.i \
doors/rtcpsap.h \
doors/rtpsm.h \
doors/rtppdu.h \
doors/rtppdu.i \
doors/rtpsap.h \
doors/sapentitysm.h \
doors/sapsessionsm.h \
doors/sapentitysap.h \
doors/tcpmuxesm.h \
doors/tcpmuxcsm.h \
doors/tcpmux.h \
doors/tcp6muxesm.h \
doors/tcp6muxcsm.h \
doors/tcp6mux.h \
doors/altbitpdu.h \
doors/altbitpdu.i \
doors/altbitsm.h \
doors/altbitsap.h \
doors/daentsm.h \
doors/doorsconf.h

# Add all the static header files here
nobase_include_HEADERS += \
doors/cmb_impl.h \
doors/filempegrtp.h \
doors/framebuf.h \
doors/mp3sm.h \
doors/mp3task.h \
doors/mpegrtp.h \
doors/sounddev.h \
doors/udpapp.h \
doors/udpappsm.h \
doors/udpapptask.h \
doors/udp6app.h \
doors/udp6appsm.h \
doors/udp6apptask.h \
doors/device.h \
doors/devif.h \
doors/filedev.h \
doors/hsockconn.h \
doors/pipedev.h \
doors/socentac.h \
doors/sockconn.h \
doors/sockent.h \
doors/sockheader.h \
doors/sockhelper.h \
doors/sockmsg.h \
doors/soconnac.h \
doors/strdev.h \
doors/ttydev.h \
doors/udpsockconn.h \
doors/udpsockconn6.h \
doors/hsi.h \
doors/hsimacro.h \
doors/hsitif.h \
doors/macroif.h \
doors/symbol.h \
doors/symhand.h \
doors/symhc.h \
doors/symiter.h \
doors/symtable.h \
doors/atexit.h \
doors/bheader.h \
doors/blockm.h \
doors/blockmm.h \
doors/bstat.h \
doors/dmm.h \
doors/memmgr.h \
doors/memopt.h \
doors/plainmm.h \
doors/sbheader.h \
doors/stat.h \
doors/statbm.h \
doors/statbmm.h \
doors/statpmm.h \
doors/pdu.h \
doors/peer.h \
doors/portmux.h \
doors/portset.h \
doors/ptask.h \
doors/ptb.h \
doors/rttask.h \
doors/sap.h \
doors/sm.h \
doors/addr.h \
doors/buffer.h \
doors/classes.h \
doors/core.h \
doors/ctask.h \
doors/etask.h \
doors/handler.h \
doors/iface.h \
doors/inetaddr.h \
doors/inetaddr6.h \
doors/library.h \
doors/logger.h \
doors/message.h \
doors/monitor.h \
doors/nethdr.h \
doors/ocoremac.h \
doors/osched.h \
doors/otask.h \
doors/othread.h \
doors/pipeport.h \
doors/port.h \
doors/timer.h \
doors/ttask.h \
doors/unixaddr.h \
doors/vstream.h \
doors/cell.h \
doors/datatypes.h \
doors/debug.h \
doors/errmgmt.h \
doors/frame.h \
doors/genmac.h \
doors/gnu_tmpls.h \
doors/llist.h \
doors/mlock.h \
doors/name.h \
doors/obl_dcl.h \
doors/obl_process.h \
doors/obl_string.h \
doors/obl_time.h \
doors/otime.h \
doors/synonym.h \
doors/tqueue.h \
doors/util.h \
doors/vdebug.h \
doors/ldapattributelist.h \
doors/ldapcon.h \
doors/namingcontext_impl.h \
doors/slpwrap.h \
doors/PropertyService_impl.h \
doors/linkconn.h \
doors/linkent.h \
doors/sbuf.h \
doors/linksapbase.h \
doors/rtcp.h \
doors/rtcptask.h \
doors/rtcpbase.h \
doors/physent.h \
doors/defs.h \
doors/payload.h \
doors/rtp.h \
doors/rtpsource.h \
doors/rtptask.h \
doors/rtpudptask.h \
doors/sapentity.h \
doors/sapsession.h \
doors/daent.h \
doors/daconfigurator.h \
doors/configreader.h \
doors/dafinder.h \
doors/serviceurl.h \
doors/slpcommon.h \
doors/slptypes.h \
doors/slpheader.h \
doors/saent.h \
doors/saconfigurator.h \
doors/sareg.h \
doors/uaconn.h \
doors/uaent.h \
doors/uaconfigurator.h \
doors/uaproxy.h \
doors/aaent.h \
doors/aaconfigurator.h \
doors/tcpmuxconn.h \
doors/tcpmuxent.h \
doors/tcpmuxbase.h \
doors/tcp6muxconn.h \
doors/tcp6muxent.h \
doors/tcp6muxbase.h \
doors/udp.h \
doors/udptask.h \
doors/udp6.h \
doors/udp6task.h \
doors/SAX.h \
doors/c14n.h \
doors/catalog.h \
doors/debugXML.h \
doors/encoding.h \
doors/entities.h \
doors/globals.h \
doors/hash.h \
doors/libxml.h \
doors/list.h \
doors/parser.h \
doors/parserInternals.h \
doors/threads.h \
doors/tree.h \
doors/uri.h \
doors/valid.h \
doors/xinclude.h \
doors/xlink.h \
doors/xmlIO.h \
doors/xmlerror.h \
doors/xmlmemory.h \
doors/xmlversion.h \
doors/xmlwin32version.h \
doors/xpath.h \
doors/xpathInternals.h \
doors/xpointer.h \
doors/interface.h \
doors/structure.h \
doors/xcgmessage.h \
doors/xmldocument.h \
doors/peergenerator.h \
doors/peerinterface.h \
doors/peermessage.h \
doors/xcgpeer.h \
doors/sapgenerator.h \
doors/sapinterface.h \
doors/sapmessage.h \
doors/altbittask.h \
doors/xcgsap.h \
doors/dsminterface.h \
doors/event.h \
doors/smgenerator.h \
doors/state.h \
doors/xcgsm.h \
doors/agenttask.h \
doors/demoncore.h \
doors/demonhand.h \
doors/demonhc.h \
doors/corbaevent.h \
doors/linewidget.h \
doors/managerevent.h \
doors/managericons.h \
doors/managermain.h \
doors/managermain.ui.h \
doors/messagelabel.h \
doors/mgrcorbaclnt.h \
doors/mgrcorbasrvr.h \
doors/mgrslpclnt.h \
doors/slpevent.h \
doors/symboltools.h \
doors/systemview.h \
doors/systemview.ui.h \
doors/traceview.h \
doors/tracewin.h \
doors/tracewin.ui.h \
doors/managertask.h \
doors/textuitask.h \
doors/alias.h \
doors/command.h \
doors/dsched.h \
doors/editask.h \
doors/envtask.h \
doors/inputask.h \
doors/lemon.h \
doors/lemonhand.h \
doors/lemonhc.h \
doors/otl_base.h \
doors/otl_dcl.h \
doors/otl_opt.h \
doors/otl_record.h \
doors/uitask.h

# Make clean rule for just doorsconf.h since there is nowhere else to put this

CLEANFILES = doors/doorsconf.h
