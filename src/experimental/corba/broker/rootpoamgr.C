
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

/*

$Log: rootpoamgr.C,v $
Revision 1.3  2002/08/14 11:51:41  bilhanan
development

Revision 1.2  2002/07/23 14:32:47  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:46:40  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:45  bilhanan
Full licence clause added into doors 0.1


*/

//
//File: rootpoamgr.C
//
//Date: Thu Dec 30 15:cd 40:10 1999
//
//Commentary:
//  This is a machine generated file produced by 
// compiler. Editing is not recommended.
//
// I edited it. Shoot me. -Bill
//
// Save into a separate file first if you're silly enough to regenerate
// rootpoamgr.aut
//
//Description:
//  This file implements functions of RootPOAManager
//  state machine.
//


#include <doors/RootPOA.h>

namespace PortableServer {
//
//Constructor: RootPOAManager
//
//Description:
//  Constructs a state machine for transiting messages 
//  to a proper handling function of the RootPOA task.
//

RootPOAManager :: RootPOAManager (void)
: states (4), events (4) {

  execute [0][0] = RootPOA :: allDefault;
  execute [1][0] = RootPOA :: holding_default;
  execute [2][0] = RootPOA :: holding_default;
  execute [3][0] = RootPOA :: allDefault;

  execute [0][1] = RootPOA :: allDefault;
  execute [1][1] = RootPOA :: active_default;
  execute [2][1] = RootPOA :: active_default;
  execute [3][1] = RootPOA :: allDefault;

  execute [0][2] = RootPOA :: allDefault;
  execute [1][2] = RootPOA :: discarding_default;
  execute [2][2] = RootPOA :: discarding_default;
  execute [3][2] = RootPOA :: discarding_default;

  execute [0][3] = RootPOA :: allDefault;
  execute [1][3] = RootPOA :: inactive_default;
  execute [2][3] = RootPOA :: inactive_default;
  execute [3][3] = RootPOA :: allDefault;

}


//
//Destructor: RootPOAManager
//
//Description:
//  Deletes this object.
//

RootPOAManager :: ~RootPOAManager (void) {

  _instance = 0; 
}


//
//Function: transit
//
//Member-Of: RootPOAManager
//
//Parameters:
//   EventType t, Message *msg, PTask *h
//
//Description:
//   This function is called from RootPOA's execute function.
//   It resolves a port from where the message given as 
//   a second parameter was received, the type of the 
//   message and a state of the task given as a third
//   parameter.
//
//   According to these three variable a proper handling
//   function of the RootPOA object is called to execute
//   the message.
//
//   First parameter t is a type of the event (msg). It can
//   be either UserEvent or SystemEvent (used for timers).
//
//   Second parameter msg is a message to be processed in
//   this execution turn.
//   
//   Third parameter h is a task calling this function.
//

bool RootPOAManager :: transit (EventType t, Message *msg, PTask *h) {

  int msgId = 0;

  if (t == SystemEvent) {

    msgId = 3;

  } else

  if (msg->getTarget () == &((RootPOA *) h)->up) {
    switch (msg->getType ()) {
      case (ServantSAP :: data) :
        msgId = 1;
        break;

    }

  } else

  if (msg->getTarget () == &((RootPOA *) h)->down) {
    switch (msg->getType ()) {
      case (OrbSAP :: data) :
        msgId = 2;
        break;

    }

  }
  return (((RootPOA *) h)->*execute [msgId][h->getState ()]) (msg);
}


//
//Function: stateName
//
//Member-Of: RootPOAManager
//
//Parameters:
//   PTask *h
//
//Return:
//   const char *
//
//Description:
//   By this function user can get a human understandable
//   string for state the task given as a parameter holds.
//   
//   Parameter h is a task whose state name will be returned.
//   
//   Returns a state of the calling task in string format.
//

const char * RootPOAManager :: stateName (PTask *h) {

  const char *rc = 0;

  switch (h->getState ()) {
    case 0 :
      rc = "HOLDING";
      break;
    case 1 :
      rc = "ACTIVE";
      break;
    case 2 :
      rc = "DISCARDING";
      break;
    case 3 :
      rc = "INACTIVE";
      break;
    default:
      rc = 0;
  }

  return rc;
}

RootPOAManager *RootPOAManager :: Instance (PTask *h){
 
  if (_instance == 0){
    DOORS_DEBUG(0, "Launching RootPOAManager");
    _instance = new RootPOAManager();
    rootpoa = h;
  }
 
  return _instance;
 
}

void RootPOAManager :: activate(void){
  rootpoa->state = ROOTPOAManager::ACTIVE;
}

void RootPOAManager :: hold_requests (CORBA::Boolean wait_for_completion){}

void RootPOAManager :: discard_requests (CORBA::Boolean wait_for_completion){}

void RootPOAManager :: deactivate (CORBA::Boolean etherealize_objects, CORBA::Boolean wait_for_completion){}

RootPOAManager::State RootPOAManager :: get_state(void){

  (RootPOAManager::State) return rootpoa->getState;
}

RootPOAManager *RootPOAManager :: _instance = 0;

}
