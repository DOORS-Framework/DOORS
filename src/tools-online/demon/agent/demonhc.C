//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
//
//  THIS SOFTWARE IS PROVIDED BY TAMPERE UNIVERSITY OF TECHNOLOGY AND 
//  CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
//  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//  FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY 
//  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Subject: DEMON handler creator
//  Author:  Ilkka Karvinen, TTKK/DMI

/*
$Log: demonhc.C,v $
Revision 1.4  2002/08/14 09:33:38  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/08/09 11:46:33  ik
Handler for InetAddr6.

Revision 1.2  2002/07/23 14:46:56  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/05/13 14:54:40  bilhanan
Restructured

Revision 1.3  2002/05/10 10:49:17  ik
Text UI.

Revision 1.2  2002/05/03 13:42:48  ik
Memory handlers made conditional according to configuration.

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.6  2002/01/14 17:37:51  ik
Changes to make two-direction CORBA calls to work.

Revision 1.5  2001/12/13 10:20:42  ik
Manager moved to root of demon directory due to autoconf structure
problem.

Revision 1.4  2001/12/12 13:49:58  ik
Demon modified to use Scheduler::Instance().

Revision 1.3  2001/12/11 15:56:46  ik
Added Makefile.am for demon. Structure changes.

Revision 1.2  2001/12/11 15:21:00  ik
Added licence for initial demon files. Directory structure changed
due to problems in autoconf.

*/

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <iostream>
#include <doors/demonhc.h>
#include <doors/demonhand.h>
#include <doors/osched.h>

#include <doors/agenttask.h>

DemonHandlerCreator::DemonHandlerCreator()
{
  AgentTask::Create();
}

DemonHandlerCreator::~DemonHandlerCreator()
{
}

void DemonHandlerCreator::createHandler (Otask *, Symbol *s) 
{
}


void DemonHandlerCreator::createHandler (EventTask *t, Symbol *s) 
{

  s->setHandler (new DemonEventTaskHandler (s, "EventTask"), DEMON_HANDLER);
}


void DemonHandlerCreator::createHandler (PTask *t, Symbol *s) 
{
  
  s->setHandler (new DemonPTaskHandler (s, "Protocol task"), DEMON_HANDLER);
}


void DemonHandlerCreator::createHandler (EnvTask *t, Symbol *s) 
{

  s->setHandler (new DemonEventTaskHandler (s, "EnvTask"), DEMON_HANDLER);
}


void DemonHandlerCreator::createHandler (LinkList *t, Symbol *s) 
{

  s->setHandler (new DemonLinkListHandler (s, "Message queue object"), DEMON_HANDLER);
}

void DemonHandlerCreator::createHandler (Port *t, Symbol *s) 
{

  s->setHandler (new DemonPortHandler (s, "Port"), DEMON_HANDLER);
}


void DemonHandlerCreator::createHandler (Timer *t, Symbol *s) 
{

  s->setHandler (new DemonTimerHandler (s, "Timer object"), DEMON_HANDLER);
}


void DemonHandlerCreator::createHandler (Iface *t, Symbol *s) 
{

  s->setHandler (new DemonIfaceHandler (s, "Interface object"), DEMON_HANDLER);
}


void DemonHandlerCreator::createHandler (Peer *t, Symbol *s) 
{

  s->setHandler (new DemonIfaceHandler (s, "Peer interface object"), DEMON_HANDLER);
}


void DemonHandlerCreator::createHandler (Message *t, Symbol *s) 
{

  s->setHandler (new DemonMessageHandler (s, "Message"), DEMON_HANDLER);
}


void DemonHandlerCreator::createHandler (UnixAddr *t, Symbol *s) 
{

  s->setHandler (new DemonUnixAddrTypeHandler (s), DEMON_HANDLER);
}


void DemonHandlerCreator::createHandler (InetAddr *t, Symbol *s) 
{

  s->setHandler (new DemonInetAddrTypeHandler (s), DEMON_HANDLER);
}


void DemonHandlerCreator::createHandler (InetAddr6 *t, Symbol *s) 
{

  s->setHandler (new DemonInetAddr6TypeHandler (s), DEMON_HANDLER);
}


void DemonHandlerCreator::createHandler (bool *t, Symbol *s) 
{

  s->setHandler (new DemonboolTypeHandler (s), DEMON_HANDLER);
}

void DemonHandlerCreator::createHandler (Sint8 *t, Symbol *s) 
{

  s->setHandler (new DemonSint8TypeHandler (s), DEMON_HANDLER);
}

void DemonHandlerCreator::createHandler (Sint16 *t, Symbol *s) 
{

  s->setHandler (new DemonSint16TypeHandler (s), DEMON_HANDLER);
}

void DemonHandlerCreator::createHandler (Sint32 *t, Symbol *s) 
{

  s->setHandler (new DemonSint32TypeHandler (s), DEMON_HANDLER);
}

void DemonHandlerCreator::createHandler (Uint8 *t, Symbol *s) 
{

  s->setHandler (new DemonUint8TypeHandler (s), DEMON_HANDLER);
}

void DemonHandlerCreator::createHandler (Uint16 *t, Symbol *s) 
{

  s->setHandler (new DemonUint16TypeHandler (s), DEMON_HANDLER);
}

void DemonHandlerCreator::createHandler (Uint32 *t, Symbol *s) 
{

  s->setHandler (new DemonUint32TypeHandler (s), DEMON_HANDLER);
}

void DemonHandlerCreator::createHandler (int *t, Symbol *s) 
{

  s->setHandler (new DemonIntTypeHandler (s), DEMON_HANDLER);
}

void DemonHandlerCreator::createHandler (char **t, Symbol *s) 
{

  s->setHandler (new DemonStringTypeHandler (s), DEMON_HANDLER);
}

void DemonHandlerCreator::createHandler (Frame *t, Symbol *s) 
{

  s->setHandler (new DemonFrameTypeHandler (s), DEMON_HANDLER);
}

void DemonHandlerCreator::createHandler (DebugScheduler *, Symbol *s) 
{

  s->setHandler (new DemonDebugSchHandler (s, "DebugScheduler"), 
		 DEMON_HANDLER);
}

#if _CF_MEM
void DemonHandlerCreator::createHandler (MemoryManager *t, Symbol *s) 
{
}

void DemonHandlerCreator::createHandler (StatPlainMemManager *t, Symbol *s) 
{

  s->setHandler (new DemonStatPlainMMHandler (s, "Access to statistics of memory manager."), DEMON_HANDLER);
}

void DemonHandlerCreator::createHandler (StatBlockManager *t, Symbol *s) 
{

  s->setHandler (new DemonStatBlockMHandler (s, "Access to statistics of memory manager."), LEMON_HANDLER);
}

void DemonHandlerCreator::createHandler (StatBlockMemManager *t, Symbol *s) 
{

  s->setHandler (new DemonStatBlockMMHandler (s, "Access to statistics of memory manager."), LEMON_HANDLER);
}
#endif






