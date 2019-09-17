//Editor-Info: -*- C++ -*-
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

/*

$Log: ctask.h,v $
Revision 1.3  2002/07/24 14:57:00  ik
Use std::string.

Revision 1.2  2002/07/23 14:25:21  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:26  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:12  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef _CTASK_H_
#define _CTASK_H_

#include <string>
#include <doors/etask.h>
#include <doors/message.h>


// The ClientTask is a powerful new way to launch DOORS Clients non-interactively.
// This class forms the base class of a user's concrete implementation.
// Instead of describing the actions to be performed by a client in the main() function
// of an application, the user describes the main functionality of the client application
// within the ClientRoutine function call in the ClientTask-derived class. The ClientTask
// immediately requests the scheduler for an execution turn. When the scheduler's run() function
// starts, the ClientRoutine() is automagically executed. 
//
// This therefore solves the issue of how to execute client-side code after starting the scheduler 
// in an infinite loop without resorting to an I/O Handler or user-interface.
//
// Optionally, a user-defined message can also be derived from ClientMessage, giving some 
// flexible benefits such as the client application being able to pass command line arguments 
// as parameters of the user-defined ClientMessage to the ClientTask for processing within the
// ClientRoutine(). 
//
// ClientTask can also be used as a top level stub on a protocol stack, and can be chained with other
// tasks.
//
// There are alot of other ways to use this class, and they will be documented.

class ClientTask : public EventTask {

public:

  enum ClientMessageType { clientmsg = 0x7fffffff };
  
  class ClientMessage : public Message {

  public:
    ClientMessage (void) {};
    virtual ~ClientMessage (void){};
    
    Message *clone (void) const;
    MessageType getType (void) const {
      return clientmsg;
    };
    
  };

  ClientTask (std::string n = "ClientTask", ClientMessage *msg = 0);
  virtual ~ClientTask (void);
  
  bool execute (EventType type, Message *msg);

  // Pure virtual function which the user defines in his class.
  virtual bool ClientRoutine (ClientMessage *msg) = 0;

};

#endif
