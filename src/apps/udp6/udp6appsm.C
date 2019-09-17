
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

$Log: udp6appsm.C,v $
Revision 1.3  2002/07/25 16:03:37  ik
Use std::string.

Revision 1.2  2002/07/23 14:28:54  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.1  2002/06/14 22:04:05  ik
Development. Initial commit for udp6 application.


*/



//
//Commentary:
//  This is a machine generated file produced by 
// compiler. Editing is not recommended.
//
//Description:
//  This file implements functions of AppTaskSM
//  state machine.
//


#include <doors/udp6apptask.h>
#include <doors/udp6appsm.h>


//
//Constructor: Udp6AppTaskSM
//
//Description:
//  Constructs a state machine for transiting messages 
//  to a proper handling function of the Udp6AppTask task.
//

Udp6AppTaskSM :: Udp6AppTaskSM (void)
: states (1), events (5) {

  execute [0][0] = &Udp6AppTask :: idle_default;
  execute [1][0] = &Udp6AppTask :: idle_Dtind;
  execute [2][0] = &Udp6AppTask :: idle_Errind;
  execute [3][0] = &Udp6AppTask :: idle_Start;
  execute [4][0] = &Udp6AppTask :: idle_timeout;

}


//
//Destructor: Udp6AppTaskSM
//
//Description:
//  Deletes this object.
//

Udp6AppTaskSM :: ~Udp6AppTaskSM (void) {


}


//
//Function: transit
//
//Member-Of: Udp6AppTaskSM
//
//Parameters:
//   EventType t, Message *msg, PTask *h
//
//Description:
//   This function is called from Udp6AppTask's execute function.
//   It resolves a port from where the message given as 
//   a second parameter was received, the type of the 
//   message and a state of the task given as a third
//   parameter.
//
//   According to these three variable a proper handling
//   function of the Udp6AppTask object is called to execute
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

bool Udp6AppTaskSM :: transit (EventType t, Message *msg, PTask *h) {

  int msgId = 0;

  if (t == SystemEvent) {

    msgId = 4;

  } else

  if (msg->getTarget () == &((Udp6AppTask *) h)->down) {
    switch (msg->getType ()) {
      case (Udp6SAP :: dtind) :
        msgId = 1;
        break;

      case (Udp6SAP :: errind) :
        msgId = 2;
        break;

    }

  } else

  if (msg->getTarget () == &((Udp6AppTask *) h)->up) {
    switch (msg->getType ()) {
      case (Udp6AppSAP :: start) :
        msgId = 3;
        break;

    }

  }
  return (((Udp6AppTask *) h)->*execute [msgId][h->getState ()]) (msg);
}


//
//Function: stateName
//
//Member-Of: Udp6AppTaskSM
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

std::string Udp6AppTaskSM :: stateName (PTask *h) {

  const char *rc = 0;

  switch (h->getState ()) {
    case 0 :
      rc = "idle";
      break;
    default:
      rc = 0;
  }

  return rc;
}
