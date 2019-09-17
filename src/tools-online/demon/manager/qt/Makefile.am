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
# Revision 1.14  2002/11/21 17:00:34  ik
# symboltools.C added.
#
# Revision 1.13  2002/11/20 14:31:18  ik
# systemview.C added.
#
# Revision 1.12  2002/11/19 16:31:26  ik
# Added agentevent.moc* and messagelabel*.
#
# Revision 1.11  2002/11/08 12:31:29  ik
# tracewin.C added.
#
# Revision 1.10  2002/10/29 17:24:45  ik
# Added lines for agentsymbol.C.
#
# Revision 1.9  2002/10/28 16:48:11  ik
# agentevent.C added.
#
# Revision 1.8  2002/10/16 15:14:00  ik
# agentlist and agentlistitem added.
#
# Revision 1.7  2002/10/16 08:18:54  ik
# agentview.*->agentwin.*. agentlist.* added.
#
# Revision 1.6  2002/10/15 16:25:26  ik
# mgrcorbaclnt.moc.h added.
#
# Revision 1.5  2002/10/15 15:44:12  ik
# mgrcorbasrvr.moc.h added.
#
# Revision 1.4  2002/10/14 16:12:53  ik
# Changes for the new mgrslpclnt moc-file.
#
# Revision 1.3  2002/10/07 16:12:49  ik
# Added mgrslpclnt.{h,C} to Makefile.
#
# Revision 1.2  2002/10/03 08:16:49  ik
# Dependencies. Path to moc changed.
#
# Revision 1.1  2002/09/30 15:20:40  ik
# Manager/Qt added to repository.
#
#

include $(top_srcdir)/doors.mk

bin_PROGRAMS = manager

manager_SOURCES = manager.C managermain.C agentwin.C agentlist.C \
                  agentlistitem.C mgrcorbasrvr.C mgrcorbaclnt.C mgrslpclnt.C \
                  agentevent.C agentsymbol.C traceview.C systemview.C \
                  symboltools.C
manager_LDADD = -L$(top_builddir)/base -ldoors_base \
                -L$(top_builddir)/tools-online/demon/agent -ldoors_demon \
                @MICOLIBS@ @QTLIBS@
manager_DEPENDENCIES = $(top_builddir)/base/libdoors_base.la \
                    $(top_builddir)/tools-online/demon/agent/libdoors_demon.la

managermain.C: managermain.h managermain.moc.h

messagelabel.C: messagelabel.h messagelabel.moc.h

agentwin.C: agentwin.h agentwin.moc.h

tracewin.C: tracewin.h tracewin.moc.h

systemview.C: systemview.h systemview.moc.h

agentlist.C: agentlist.h agentlist.moc.h

agentlistitem.C: agentlistitem.h agentlistitem.moc.h

agentevent.C: agentevent.h agentevent.moc.h

mgrslpclnt.C: mgrslpclnt.h mgrslpclnt.moc.h

mgrcorbasrvr.C: mgrcorbasrvr.h mgrcorbasrvr.moc.h

mgrcorbaclnt.C: mgrcorbaclnt.h mgrcorbaclnt.moc.h

managermain.moc.h:
	@QTMOC@ -o managermain.moc.h $(top_srcdir)/tools-online/demon/manager/qt/managermain.h

messagelabel.moc.h:
	@QTMOC@ -o messagelabel.moc.h $(top_srcdir)/tools-online/demon/manager/qt/messagelabel.h

agentwin.moc.h:
	@QTMOC@ -o agentwin.moc.h $(top_srcdir)/tools-online/demon/manager/qt/agentwin.h

tracewin.moc.h:
	@QTMOC@ -o tracewin.moc.h $(top_srcdir)/tools-online/demon/manager/qt/tracewin.h

systemview.moc.h:
	@QTMOC@ -o systemview.moc.h $(top_srcdir)/tools-online/demon/manager/qt/systemview.h

agentlist.moc.h:
	@QTMOC@ -o agentlist.moc.h $(top_srcdir)/tools-online/demon/manager/qt/agentlist.h

agentlistitem.moc.h:
	@QTMOC@ -o agentlistitem.moc.h $(top_srcdir)/tools-online/demon/manager/qt/agentlistitem.h

agentevent.moc.h:
	@QTMOC@ -o agentevent.moc.h $(top_srcdir)/tools-online/demon/manager/qt/agentevent.h

mgrslpclnt.moc.h:
	@QTMOC@ -o mgrslpclnt.moc.h $(top_srcdir)/tools-online/demon/manager/qt/mgrslpclnt.h

mgrcorbasrvr.moc.h:
	@QTMOC@ -o mgrcorbasrvr.moc.h $(top_srcdir)/tools-online/demon/manager/qt/mgrcorbasrvr.h

mgrcorbaclnt.moc.h:
	@QTMOC@ -o mgrcorbaclnt.moc.h $(top_srcdir)/tools-online/demon/manager/qt/mgrcorbaclnt.h
