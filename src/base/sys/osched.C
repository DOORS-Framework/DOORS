// -*- C++ -*-
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

$Log: osched.C,v $
Revision 1.6  2005/01/20 10:30:04  bilhanan
bugfix: for() loop in forget()

Revision 1.5  2002/12/17 13:36:31  ik
Development.

Revision 1.4  2002/08/14 09:31:41  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/25 10:08:40  ik
Use std::string.

Revision 1.2  2002/07/23 14:25:22  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.3  2002/02/25 17:06:05  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.

Revision 1.2  2001/06/08 16:46:24  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.7  2001/06/08 08:39:19  bilhanan
Full licence clause added into doors 0.1


*/

#include <string>
#include <doors/osched.h>
#include <doors/handler.h>
#include <doors/debug.h>

#include <doors/hsi.h>

Scheduler :: Scheduler (IoHandler *ioh, std::string n, TimerTask *tt, Scheduler *s)
  : Otask (n, s),
    ioHandler (ioh),
    timerTask (tt),
    current (0), traceState(beforeTrace),
    finishedList (false),
    priority (DOORS_PRIORITY_HIGH), 
    highestOtaskRequested (DOORS_PRIORITY_LOWEST + 1)
{
  if (ioHandler == 0){
    ioHandler = IoHandler :: Instance (this);
  }
}

Scheduler :: ~Scheduler (void) 
{
  _instance = 0;
}

void Scheduler :: run (void)
{
  // the lower number the higher priority
  while (priority < DOORS_NO_PRIORITIES) {
    current = otask[priority].remove ();
    if (current) {

      traceState = beforeTrace;
      TRACE_TASK(current);
      current->run ();
      traceState = afterTrace;
      TRACE_TASK(current);

      // if the load of the task is not zero, it has 
      // more work to do. So, it must be kept 
      // in the runnable tasks' list. 
      // Note that the task may remove itself and no further
      // execution is needed.
      if (current && current->getLoad ()) {
	otask[priority].append (current, NotDestroyed);
      } else {
	// we can forget this task, and we have
	// one job less 
	decrementLoad ();
      }
      break;
    } else {
      ++priority;
    }
  }

  current = 0;
}

void Scheduler :: inform (Otask *task)
{
  return;
}

void Scheduler :: forget (Otask *task)
{
    for (OtaskPriority i = DOORS_PRIORITY_LOWEST; i<=DOORS_PRIORITY_HIGH; --i) {
        Otask *removed = otask[i].remove (task);
        if (removed) {
            // The task to be forgotten was found.

            // check if removed task is the task currently being processed 
            if (removed == current) {
                 current = 0;
            }
	    // but, it might also be on other queues, so we must
	    // check them also
        }
    }

    // If the task is currently run, it might not be in any list
    if (task == current) {
	current = 0;
    }
    return;
}

bool Scheduler :: request (Otask *task, OtaskPriority prt)
{
    if (task == current) {
	// ignore because the scheduler checks after the current
	// has run if it needs more executions
	return false;
    }

    if (prt > DOORS_PRIORITY_LOWEST) {
        prt = DOORS_PRIORITY_NORMAL;
    }

    // A higher priority task has got a message and it 
    // should be run before the lower priority tasks 
    // that are next being executed
    if (prt < priority) {
        priority = prt;
        finishedList = false; 
    }

    otask[priority].append (task, NotDestroyed);

    // the upper scheduler must be informed to run this scheduler
    // because this scheduler got something to be executed.
    if (scheduler) {
	scheduler->request (this, priority);
    }

    incrementLoad ();

    return true;
}

void Scheduler :: setIoHandler (IoHandler *ioh) 
{
  ioHandler = ioh;
  return;
}

void Scheduler :: setTimerTask (TimerTask *tt) 
{ 
  timerTask = tt;
  return;
};

IoHandler *Scheduler :: getIoHandler (void) const 
{
  if (!ioHandler && scheduler) {
    return scheduler->getIoHandler ();
  }
  return ioHandler; 
}
    
TimerTask *Scheduler :: getTimerTask (void) const
{
  if (!timerTask && scheduler) {
    return scheduler->getTimerTask ();
  }
  return timerTask;
}

//
//Function: isBusy
//
//Member-Of: Scheduler
//
//Return:
//  bool
//
//Description:
//  Function returns non zero if there is some 
//  unexecuted messages in the system. If there is
//  no job to do then zero is returned.
//
//  This function can be used for controlling
//  the execution of application for example
//  when optimizing memory usage without embedded
//  user interface.

bool Scheduler :: isBusy (void)
{ 
  OtaskIterator iterator (&otask[priority]);

  return (iterator.head()) ? true : false; 
}

Scheduler* Scheduler :: Instance (IoHandler *ioh, std::string n,
				  TimerTask *tt, Scheduler *s){

  if (_instance == 0){
    _DOORS_DEBUG(0, "Launching Scheduler");
    _instance = new Scheduler (ioh, n, tt, s);
  }
  return _instance;

}

Scheduler* Scheduler :: _instance = 0;









