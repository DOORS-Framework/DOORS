// 
//  Copyright 1999
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
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

$Log: ptask.C,v $
Revision 1.7  2004/02/10 15:27:52  bilhanan
introducing ptaskid and get/set functions.

Revision 1.6  2003/10/15 10:48:54  bilhanan
execute() now checks for message count.

Revision 1.5  2002/08/14 09:31:40  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.4  2002/07/25 10:08:39  ik
Use std::string.

Revision 1.3  2002/07/23 14:25:20  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:50  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:33  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:32  bilhanan
Full licence clause added into doors 0.1


*/


#include <string>
#include <doors/ocoremac.h>
#include <doors/ptask.h>
#include <doors/sm.h>
#include <doors/hsi.h>


//
//Constructor: PTask
//
//Parameters:
//   std::string n,
//   Scheduler *s,
//   StateMachine *m
//
//Description:
//   Creates a new instance of PTask class.
//   
//   First parameter n is a name of the task.
//   
//   Second parameter s is a pointer to a scheduler who will
//   schedule the execution turns of tasks in DOORS system.
//   
//   Third parameter m is a pointer to a state machine of
//   this task.
//   

PTask :: PTask (std::string n, Scheduler *s, StateMachine *m)
: EventTask (n, s), sm (m), state (0) {

    _MT_SYNCMONITOR;
    STATIC_SYMBOL (state, this);
    STATIC_SYMBOL (systemQueue, this);
    STATIC_SYMBOL (messageQueue, this);
}


//
//Destructor: PTask
//
//Description:
//   Deletes this instance of PTask. (The state machine of
//   the task is not deleted.)
//   

PTask :: ~PTask (void) {

    _MT_SYNCMONITOR;
}


//
//Function: execute
//
//Member-Of: PTask
//
//Parameters:
//   EventType t,
//   Message *msg
//
//Description:
//   This function is called in EventTask's run function when
//   a message is going to be executed. In PTask's case it
//   calls a transit function of its state machine. After
//   transition the message is deleted if the return value
//   of transit function is false.
//   
//   First parameter t is a type of the event (msg). It can
//   be either UserEvent or SystemEvent.
//   
//   Second parameter msg is a message to be processed in
//   this execution turn.
//   

bool PTask :: execute (EventType t, Message *msg) {

    _MT_SYNCMONITOR;
    TRACE_MSG (msg);
    bool rc = false;
    msg->unlock ();

    if (sm) {
	rc = sm->transit (t, msg, this);
    }

    if (!rc && msg) {

      if (msg->isForMany())
	msg->decrement_rcpt_count();

      if ((msg->get_rcpt_count() == 0) && (!msg->isLocked ())) {
	DELETE_SYMBOL (msg);
	delete msg;
      }
    }



    return true;
}


//
//Function: allDefault
//
//Member-Of: PTask
//
//Parameters:
//   Message *msg
//
//Description:
//   This is a default function will be called if there is no
//   special handling function defined for message in current
//   state of task.
//   
//   Parameter msg is a message to be processed in this
//   execution turn.
//   

bool PTask :: allDefault (Message *msg) {

    _MT_SYNCMONITOR;
    return false;
}


//
//Function: getState
//
//Member-Of: PTask
//
//Return:
//   Uint16
//
//Description:
//   This functions returns a state of the task. It is
//   normally used in state machine while selecting the
//   appropriate task's handling function for the current
//   message.
//   
//   Returns an integer value that represents a current state
//   of the task.
//   

Uint16 PTask :: getState (void) {

    _MT_SYNCMONITOR;
    return state;
}


//
//Function: getStateMachine
//
//Member-Of: PTask
//
//Return:
//   StateMachine *
//
//Description:
//   By calling this function a state machine of this task
//   can be get.
//   
//   Returns a pointer to the base class of state machine
//   initialized in this class.
//   

StateMachine *PTask :: getStateMachine (void) {

    _MT_SYNCMONITOR;
    return sm;
}

Uint32 PTask :: getptaskid (void) {

  return ptaskid;
}

void PTask :: setptaskid (Uint32 newid) {

  ptaskid = newid;
  return;
}






