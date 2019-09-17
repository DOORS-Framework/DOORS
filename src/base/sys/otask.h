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

$Log: otask.h,v $
Revision 1.3  2004/05/25 09:13:51  bilhanan
setName() introduced.

Revision 1.2  2002/07/24 13:41:25  ik
Use std::string.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:49  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:20  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef OTASK_H
#define OTASK_H

#include <string>

#include <doors/library.h>
#include <doors/util.h>
#include <doors/ocoremac.h>
#include <doors/name.h>

/** Priority of Task scheduling */
typedef Uint8 OtaskPriority;

/**
  Otask is a runnable DOORS task class. This is the base 
  class of all tasks. The main purpose of this class is to
  hold essential information needed by the task and scheduler. 
  It contains information about its scheduler, which is needed 
  for requesting execution time, and the load of the task. The 
  name of the task is for debugging purposes. 
  <p>
  The run function is defined by the system. It is the function
  that is called when the scheduler wants to execute the task. 
  Any other method to execute a task is not permitted because
  it affects to the execution of the other tasks. The run
  function is assumed to be as a finite state machine code.
  It must not block and it should do its work gradually because
  all the other tasks are waiting for executing.
  <p>
  The load can be changed by the task itself, only. That is 
  because you cannot assume that anybody else but the task knows
  the behavior of the task. 
  */
class Otask {
public:
    /** Tells the scheduler, if given, about a new task to be
      scheduled. The given name is duplicated. 
      @param name The parameter name is the name of this task
      @param s is the scheduler for this task.
      */
    Otask (std::string name, Scheduler *s = 0);

    /** Tells the scheduler, if known, that this task does not need
      time for execution any more.
      */
    virtual ~Otask (void);
    
    /** doc++ fix */

    /** The fundamental function of the DOORS <b>time sharing</b>
      system. Implementations os the function run() implements a atomic
      time slice every task can run. In implmentation of run() is
      good to measure how long it consumes time. Relatively too
      long execution makes whole system <b>reactivity</b> time smaller.
      <p>
      run() is called by implementations of Scheduler and by the
      user in the program main loop.
      @see Scheduler
      @see prof(1)
      */
    virtual void run() = 0;

    /** Returns the i/o handler of the system, or zero if it is 
      not known by this task's scheduler.
      */
    virtual IoHandler *getIoHandler (void) const;

    /** Returns the timer task, or zero if the task does not know
      the scheduler or the scheduler does not know about the 
      timer task.
      */
    virtual TimerTask *getTimerTask (void) const;

    /** The load is an indication of how much work this task has. 
      It is usually the same number as the number of the messages
      in the queue waiting to be handled. However, an otask does
      not have a message queue. It is a property of an event task.
      However schedulers need information about the load for
      scheduling.
      @return Returns the load of this task.
      */
    Load getLoad (void) const;

    /** Return name of the task. If the name is not known an
	empty string is returned. */
    std::string getName (void) const;

    /** Sets the name of the task. Recommended only when changing 
        the name, otherwise set in constructor. */
    void setName (std::string newName);

    /** Sets the internal variable of the task holding the priority. */
    void setPriority (OtaskPriority pri);

    /** Returns the priority of the task. */
    OtaskPriority getPriority (void) const;

    /** Returns the scheduler of the task. */
    Scheduler *getScheduler (void) const;

    /** doc++ fix */
protected:

    /** Decrements load by one if it is not zero already. Load cannot
      be negative. This function is usually called when a message
      is handled. 
      */
    void decrementLoad (void);

    /** Increments load by one. This function is usually called when
      a message is stored in the event task's message queue. 
      */
    void incrementLoad (void);

    /** Sets the load of this task. Load is a positive number. 
      If negative number is given load is set to zero. 
      Note that the setLoad and decrementLoad member functions
      makes sure that the load is positive. 
      @param l The parameter l is the new load value.
      */
    void setLoad (Load l);
    
    Scheduler *scheduler;
    OtaskPriority priority;
    Name name; // name is a great help in debugging

  private:
    Load requestCounter;
}; 

inline Load Otask :: getLoad (void) const
{ 

  return requestCounter; 
}

inline void Otask :: setLoad (Load l)
{ 

  requestCounter = (l < 0) ? 0 : l;
  return;
}

inline void Otask :: decrementLoad (void)
{ 

  if (requestCounter) {
    --requestCounter;
  }
}

inline void Otask :: incrementLoad (void)
{ 

  ++requestCounter;
  return;
}

inline void Otask :: setPriority (OtaskPriority pri)
{

  priority = pri;
  return;
}

inline OtaskPriority Otask :: getPriority (void) const
{

  return priority;
}

inline Scheduler* Otask :: getScheduler (void) const 
{

  return scheduler;
}

#endif  // OTASK_H

