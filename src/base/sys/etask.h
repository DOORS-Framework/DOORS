// -*- C++ -*-
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

$Log: etask.h,v $
Revision 1.3  2002/07/24 14:57:01  ik
Use std::string.

Revision 1.2  2002/07/23 14:25:21  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:28  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:13  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef ETASK_H
#define ETASK_H

#include <string>
#include <doors/otask.h>
#include <doors/osched.h>
#include <doors/hsimacro.h>

/** Basic type of message event */
enum EventType { NoEvent = 0, UserEvent = 1, SystemEvent = 2};

/**
  The user of the DOORS library should inherit a base class
  for her executable tasks from the event task. The event task
  has message queues for storing messages it gets from other
  tasks through ports it has. The event task and all kind of 
  ports are designed to work together when passing message. 
  <p>
  Another feature of the event task is priority, which can be
  deployed by the scheduler. 
  <p>
  The logic of the task should be implemented to the execute 
  function. 
  */
class EventTask : public Otask {
public:
    /** The consturctor of the event task just initializes the
      base class and its variables by the name of the task
      and the scheduler of the task.
      @param name The parameter name is the name of the task. It is used only
      for debugging purposes. 
      @param sched The sched is the scheduler for this task. 
      */
    EventTask (std::string n, Scheduler *sched = 0);

    virtual ~EventTask (void);
    
    /** This is just dummy execute function. This function is 
      assumed to be user's defined code of a finite state machine.
      The message must be deleted if the default run function is used. 
      @param type The first parameters is the type of message.
      @param msg The msg is the handled message.
      */
    virtual bool execute (EventType type, Message *msg);

    /** Saves the given msg to the proper message queue according
      to the type of the msg. This function is usually called
      by the getMessage function of a port.
      @param msg  Message to push back into queue
      @param type Type of the message
      @return Return true if save manages the msg, false if
      save is not interested about message and user have futher
      manage the msg.
      */
    virtual bool save (Message *msg, EventType type = UserEvent);

    /** Gets a message from the queue, system or user queue.
      @return Returns the type of the message stored as a pointer to the
      parameter. May return SystemEvent, UserEvent, or NoEvent,
      if a message is not returned.
      */
    virtual EventType event (Message *&msg);

    /** This function can decide
      if it accept or not the connection and can be used to monitor
      all the connections to a task. 
      @param at The parameter is the port to which is to be connected. 
      @return true return value means that the connection is accepted, and 
      false means refusing. 
      */
    virtual bool connect (Port *at);

    /**
      This is a default function that is called when a communication
      channel is disconnected, that is one of the task's ports is 
      disconnected by the task itself or other task that has connection
      to this task. This function makes the task able to react somehow
      to the disconnection. 
      @param at Port that is disconnected.
      @return Should always return zero, because the function cannot refuse 
      the disconnection.
      */
    virtual bool disconnect (Port *at);

    // impl
    virtual void run (void);
    
protected:
    LinkList messageQueue;
    LinkList systemQueue;
};


#endif  // TASK_H


