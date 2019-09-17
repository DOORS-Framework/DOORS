// 
//  Copyright 1999 
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
 
 
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

$Log: etask.C,v $
Revision 1.5  2002/12/17 13:36:31  ik
Development.

Revision 1.4  2002/08/14 09:31:40  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/24 14:57:01  ik
Use std::string.

Revision 1.2  2002/07/23 14:25:21  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:19  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:13  bilhanan
Full licence clause added into doors 0.1


*/

#include <string>
#include <doors/ocoremac.h>
#include <doors/etask.h>
#include <doors/message.h>
#include <doors/debug.h>
#include <doors/hsi.h>

EventTask :: EventTask (std::string n, Scheduler *sched)
    : Otask (n, sched)
{
  //  _MT_SYNCMONITOR;
}

EventTask :: ~EventTask (void)
{
  //  _MT_SYNCMONITOR;
}

void EventTask :: run (void) 
{ 
  //  _MT_SYNCMONITOR;
  //  _MT_RELEASE_SYNCMONITOR; 
  //  _MT_STABLE_MONITOR; 
  //  _MT_ACQUIRE_SYNCMONITOR;
  //  _MT_TRACE_TASK;
  Message *msg;
  EventType type = event (msg);
  if (NoEvent != type) {
    TRACE_MSG(msg);
    execute (type, msg);
    decrementLoad ();
  }
  //  _MT_TRACE_TASK;
  return;
}

bool EventTask :: save (Message *msg, EventType type)
{
  //  _MT_ASYNCMONITOR;

  switch (type) {
  case UserEvent:
    messageQueue.append (msg);
    break;
    
  case SystemEvent:
    systemQueue.append (msg);
    break;
    
  default:
      _DOORS_ERROR(0,"EventTask: bad type of Message");
    delete msg;
  }

  // Inform the scheduler that this task need to be executed in near future
  if (scheduler && getLoad () == 0) {
    scheduler->request (this, priority);
  }
  
  // we keep track of the number of messages we have
  incrementLoad ();
  
  //  _MT_SIGNAL_RUNLOCK;

  // when the message is saved or deleted nobody should no more 
  // have any references to it.

  return true;
}

bool EventTask :: execute (EventType, Message *msg)
{
  //  _MT_SYNCMONITOR;

  // a message that is not processed properly is just deleted
  if (msg) {
    delete msg;
  }
  return true;
}

bool EventTask :: connect (Port *)
{
  //  _MT_ASYNCMONITOR;
  
  return true;
}

bool EventTask :: disconnect (Port *)
{
  //  _MT_ASYNCMONITOR;

  return true;
}

EventType EventTask :: event (Message *&msg)
{
  //  _MT_ASYNCMONITOR;

  EventType type = NoEvent;
  if ((msg = (Message *)systemQueue.remove ())) {
    type = SystemEvent;
  } else {
    if ((msg = (Message *)messageQueue.remove ())) {
      type = UserEvent;
    }    
  }
  return type;
}
