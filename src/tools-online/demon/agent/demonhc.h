//Editor-Info: -*- C++ -*-
//  Copyright 2000 
//  	Telecoms Lab, Tampere University of Technology.  All rights reserved.

//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
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
//  Subject: DEMON/AgentTask header
//  Author:  Ilkka Karvinen, TTKK/DMI

/*
$Log: demonhc.h,v $
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

Revision 1.2  2002/05/03 13:42:49  ik
Memory handlers made conditional according to configuration.

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.6  2002/01/14 17:37:50  ik
Changes to make two-direction CORBA calls to work.

Revision 1.5  2001/12/12 13:49:57  ik
Demon modified to use Scheduler::Instance().

Revision 1.4  2001/12/12 13:19:41  ik
Log tag added.

*/

#ifndef DEMONHC_H
#define DEMONHC_H

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <doors/symhc.h>
#include <doors/agenttask.h>

class DemonHandlerCreator : public HandlerCreator {
public:
  DemonHandlerCreator();
  ~DemonHandlerCreator();

  void createHandler (Otask *, Symbol *s);
  void createHandler (EventTask *, Symbol *s);
  void createHandler (PTask *, Symbol *s);
  void createHandler (LinkList *, Symbol *s);
  void createHandler (EnvTask *, Symbol *s);
  void createHandler (Port *, Symbol *s);
  void createHandler (Timer *, Symbol *s);
  void createHandler (Iface *, Symbol *s);
  void createHandler (Peer *, Symbol *s);
  void createHandler (Message *, Symbol *s);
  void createHandler (UnixAddr *, Symbol *s);
  void createHandler (InetAddr *, Symbol *s);
  void createHandler (InetAddr6 *, Symbol *s);
  void createHandler (bool *, Symbol *s);
  void createHandler (Sint8 *, Symbol *s);
  void createHandler (Sint16 *, Symbol *s);
  void createHandler (Sint32 *, Symbol *s);
  void createHandler (Uint8 *, Symbol *s);
  void createHandler (Uint16 *, Symbol *s);
  void createHandler (Uint32 *, Symbol *s);
  void createHandler (Load *, Symbol *s);
  void createHandler (char **, Symbol *s);
  void createHandler (Frame *, Symbol *s);
  void createHandler (DebugScheduler *, Symbol *s);

#if _CF_MEM
  void createHandler (MemoryManager *, Symbol *s);
  void createHandler (StatPlainMemManager *, Symbol *s);
  void createHandler (StatBlockManager *, Symbol *s);
  void createHandler (StatBlockMemManager *, Symbol *s);
#endif

};


#endif  // DEMONHC_H

