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

$Log: mp3sm.C,v $
Revision 1.2  2002/07/23 14:28:52  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.2  2001/06/08 16:46:51  bilhanan
Full Licence clause into DOORS 0.2


*/


//
//File: mp3sm.C
//
//Date: Wed Aug 16 19:31:28 2000
//
//Commentary:
//  This is a machine generated file produced by 
// compiler. Editing is not recommended.
//
//Description:
//  This file implements functions of MP3TaskSM
//  state machine.
//


#include <doors/mp3task.h>
#include <doors/mp3sm.h>


//
//Constructor: MP3TaskSM
//
//Description:
//  Constructs a state machine for transiting messages 
//  to a proper handling function of the MP3Task task.
//

MP3TaskSM :: MP3TaskSM (void)
: states (1), events (8) {

  execute [0][0] = &MP3Task :: allDefault;
  execute [1][0] = &MP3Task :: idle_Dti;
  execute [2][0] = &MP3Task :: idle_Sr;
  execute [3][0] = &MP3Task :: idle_Rr;
  execute [4][0] = &MP3Task :: idle_Sdes;
  execute [5][0] = &MP3Task :: idle_Bye;
  execute [6][0] = &MP3Task :: idle_App;
  execute [7][0] = &MP3Task :: idle_Timeout;

}


//
//Destructor: MP3TaskSM
//
//Description:
//  Deletes this object.
//

MP3TaskSM :: ~MP3TaskSM (void) {


}


//
//Function: transit
//
//Member-Of: MP3TaskSM
//
//Parameters:
//   EventType t, Message *msg, PTask *h
//
//Description:
//   This function is called from MP3Task's execute function.
//   It resolves a port from where the message given as 
//   a second parameter was received, the type of the 
//   message and a state of the task given as a third
//   parameter.
//
//   According to these three variable a proper handling
//   function of the MP3Task object is called to execute
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

bool MP3TaskSM :: transit (EventType t, Message *msg, PTask *h) {

  int msgId = 0;

  if (t == SystemEvent) {

    msgId = 7;

  } else

  if (msg->getTarget () == &((MP3Task *) h)->rtpdown) {
    switch (msg->getType ()) {
      case (RtpSAP :: dti) :
        msgId = 1;
        break;

    }

  } else

  if (msg->getTarget () == &((MP3Task *) h)->rtcpdown) {
    switch (msg->getType ()) {
      case (RtcpSAP :: srdti) :
        msgId = 2;
        break;

      case (RtcpSAP :: rrdti) :
        msgId = 3;
        break;

      case (RtcpSAP :: sdesdti) :
        msgId = 4;
        break;

      case (RtcpSAP :: byedti) :
        msgId = 5;
        break;

      case (RtcpSAP :: appdti) :
        msgId = 6;
        break;

    }

  }
  return (((MP3Task *) h)->*execute [msgId][h->getState ()]) (msg);
}


//
//Function: stateName
//
//Member-Of: MP3TaskSM
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

const char * MP3TaskSM :: stateName (PTask *h) {

  const char *rc = 0;

  switch (h->getState ()) {
    case 0 :
      rc = "Idle";
      break;
    default:
      rc = 0;
  }

  return rc;
}
