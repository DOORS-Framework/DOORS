
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

$Log: udpappsm.C,v $
Revision 1.3  2002/07/25 16:03:36  ik
Use std::string.

Revision 1.2  2002/07/23 14:28:53  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.2  2001/06/08 16:46:53  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:40:04  bilhanan
Full licence clause added into doors 0.1


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


#include <doors/udpapptask.h>
#include <doors/udpappsm.h>


//
//Constructor: UdpAppTaskSM
//
//Description:
//  Constructs a state machine for transiting messages 
//  to a proper handling function of the UdpAppTask task.
//

UdpAppTaskSM :: UdpAppTaskSM (void)
: states (1), events (5) {

  execute [0][0] = &UdpAppTask :: idle_default;
  execute [1][0] = &UdpAppTask :: idle_Dtind;
  execute [2][0] = &UdpAppTask :: idle_Errind;
  execute [3][0] = &UdpAppTask :: idle_Start;
  execute [4][0] = &UdpAppTask :: idle_timeout;

}


//
//Destructor: UdpAppTaskSM
//
//Description:
//  Deletes this object.
//

UdpAppTaskSM :: ~UdpAppTaskSM (void) {


}


//
//Function: transit
//
//Member-Of: UdpAppTaskSM
//
//Parameters:
//   EventType t, Message *msg, PTask *h
//
//Description:
//   This function is called from UdpAppTask's execute function.
//   It resolves a port from where the message given as 
//   a second parameter was received, the type of the 
//   message and a state of the task given as a third
//   parameter.
//
//   According to these three variable a proper handling
//   function of the UdpAppTask object is called to execute
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

bool UdpAppTaskSM :: transit (EventType t, Message *msg, PTask *h) {

  int msgId = 0;

  if (t == SystemEvent) {

    msgId = 4;

  } else

  if (msg->getTarget () == &((UdpAppTask *) h)->down) {
    switch (msg->getType ()) {
      case (UdpSAP :: dtind) :
        msgId = 1;
        break;

      case (UdpSAP :: errind) :
        msgId = 2;
        break;

    }

  } else

  if (msg->getTarget () == &((UdpAppTask *) h)->up) {
    switch (msg->getType ()) {
      case (UdpAppSAP :: start) :
        msgId = 3;
        break;

    }

  }
  return (((UdpAppTask *) h)->*execute [msgId][h->getState ()]) (msg);
}


//
//Function: stateName
//
//Member-Of: UdpAppTaskSM
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

std::string UdpAppTaskSM :: stateName (PTask *h) {

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
