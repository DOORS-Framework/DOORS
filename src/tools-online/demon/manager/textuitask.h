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
//  Subject: DEMON/TextUITask header
//  Author:  Ilkka Karvinen, TTKK/DMI

/*
$Log: textuitask.h,v $
Revision 1.4  2002/07/23 14:46:57  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.3  2002/05/24 09:45:38  ik
SLP fixes.

Revision 1.2  2002/05/24 08:04:43  ik
Development

Revision 1.1.1.1  2002/05/13 14:54:56  bilhanan
Restructured

Revision 1.1  2002/05/10 10:45:35  ik
Text UI first commit.

*/

#ifndef TEXTUITASK_H
#define TEXTUITASK_H

#include <string>
#include <vector>
#include <map>
#include <doors/etask.h>
#include <doors/timer.h>
#include "demon.h"

/** Text User Interface task
 */
class TextUITask : public EventTask,
		   public PortableServer::DynamicImplementation
{
 public:
  TextUITask (const char* n, short verbose);
  virtual ~TextUITask(void);

  // CORBA standards
  virtual void invoke (CORBA::ServerRequest_ptr svreq);
  virtual char* _primary_interface(const PortableServer::ObjectId&, 
                                   PortableServer::POA_ptr); 

  // outgoing
  CORBA::Boolean dui_start(CORBA::Short verbose);
  CORBA::Boolean dui_stop();

  // incoming
  CORBA::Boolean dui_setAgent(CORBA::Long handle, const char* agent_url);
  CORBA::Boolean dui_unsetAgent(CORBA::Long handle);
  CORBA::Boolean dui_setManager(const char* manager_ior);
  CORBA::Boolean dui_sendSymbol (CORBA::Long agent_handle,
				 Demon::TimeStamp time, 
				 Demon::Symbol symbol);

  // EventTask executor
  virtual bool execute (EventType type, Message *msg);

  bool isTerminated() { return terminated_; }

private:
  std::string typeName(const Demon::Symbol& symbol) const;
  void printSymbol(const Demon::Symbol& symbol, int level = 0);

  bool terminated_;
  ::PortableServer::POA_ptr poa_;
  ::CORBA::ORB_ptr orb_;
  CORBA::Object_ptr manager_obj_;
  Timer timer_;
  bool initial_;
  std::string ior_;
  bool manager_set_;                        // Manager has sent its IOR
  short verbose_;
  int id_;
};

#endif // TEXTUITASK_H
