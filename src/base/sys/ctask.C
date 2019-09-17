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

$Log: ctask.C,v $
Revision 1.3  2002/07/24 14:57:00  ik
Use std::string.

Revision 1.2  2002/07/23 14:25:21  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:18  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:11  bilhanan
Full licence clause added into doors 0.1


*/


#include <string>
#include <doors/ctask.h>

ClientTask :: ClientTask (std::string n, ClientMessage *msg)
  : EventTask (n, Scheduler::Instance()) {

  if (msg == 0){
    msg = new ClientMessage;
    msg->lock();
    this->save(msg);
  } else {
    if (msg->getType() == clientmsg){
      msg->lock();
      this->save(msg);
    } else {
      delete (msg);
      delete this;
    }
  }
}

ClientTask :: ~ClientTask (void){

}

bool ClientTask :: execute (EventType type, Message *msg) {

  return (this->ClientRoutine ((ClientMessage *)msg));
}

Message * ClientTask :: ClientMessage :: clone (void) const {
  
  return new ClientMessage (*this);

}

