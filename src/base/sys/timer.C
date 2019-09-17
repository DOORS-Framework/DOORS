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

$Log: timer.C,v $
Revision 1.3  2002/08/14 09:31:41  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/07/23 14:25:22  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:29  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:26  bilhanan
Full licence clause added into doors 0.1


*/

#include <doors/timer.h>
#include <doors/ttask.h>   

#include <doors/debug.h>

//
//Constructor: Timer
//
//Description:
//      A default constructor that should be used only if
//      forced. The timer is unusable if the there is no information
//      about the task to which it belongs. Hence, it cannot send 
//      time-out messages.
//
Timer :: Timer (void) : Port (0), initialTime (0), ticking (false), timers (0)
{
}


//
//Constructor: Timer
//
//Parameters:
//      EventTask *host, 
//      OTime &count
//
//Description:
//      The constructor initializes the internal variables. The task
//      to which a timer belongs is needed because timer must create
//      and store a time-out message into the message queue of the task.
//
//      There must also be a timer task in the system that the
//      scheduler knows. Without information about the timer task 
//      a timer cannot be started.
//
//      The parameter host is the task to which this timer belongs,
//      and count is an initial time for time-out.
//

Timer :: Timer (TimerTask *t) : Port (0), 
	initialTime (0), ticking (false), timers (t)
{
}


Timer :: Timer (EventTask *host, const OTime &count) : Port (host), 
	initialTime (0), ticking (false), timers (0)
{
  init (host, (OTime&)count);
}


//
//Destructor: Timer
//
//Description:
//      The destructor just tells the timer task to forget this timer,
//      if the timer is on. 
//
Timer :: ~Timer (void)
{

  if (timers && ticking) {
    timers->forget (this);
  }
}


//
//Function: init
//
//Member-Of: Timer
//
//Parameters:
//      EventTask *host, 
//      OTime &count
//
//Description:
//      An initialization function for initialization after the 
//      timer is created. This is just for a case when you need
//      to create timers without parameters and then initialize
//      them afterwards.
//
//      The paramter host is the task to which this timer belongs, and
//      count is an initial time for time-out.
//
void Timer :: init (EventTask *host, const OTime &count)
{

  if (timers && ticking) {
    timers->forget (this);
  }
  
  initialTime = count;
  if (host) {
    if (!timers)
      timers = host->getTimerTask ();
    
    task = host;
    
    // Timer is useless if there is no timer task, i.e. there is no 
    // information about the timer task
    _DOORS_FAULT(timers,"no timer task");
  } else {
    // Timer is useless if there is no host, i.e. there is no 
    // information about the timer task. To allow timer
    // initialization without the host pointer is just because the
    // current compilers do not support table initialization by 
    // using a constructor with parameters.
  }
  return;
}


//
//Function: start
//
//Member-Of: Timer
//
//Description:
//      The start function starts the timer. A timer can be started
//      again without stopping it first. Information about the running
//      timers is kept in the timer task. This timer is just an
//      interface for the timer task.
//
void Timer :: start (void)
{
  // timer itself takes care of removing itself from the list of a
  // timer task, because it knows best if it is ticking, or not.
  // This is just because the performance reasons.
  if (ticking) {
    timers->forget (this);
  }
    
  // start the timer in a timer task
  timers->inform (this);

  ticking = true;
  return;
}


//
//Function: stop
//
//Member-Of: Timer
//
//Description:
//      This function stops the timer. It tells the timer task to 
//      remove its information the list of the timer task where all
//      running timers are kept. A timer can be stopped, even it has
//      not been started. 
//
void Timer :: stop (void)
{
  // check if stop is called twice. If it is, this method returns immediately
  if (ticking) {
    timers->forget (this);
    ticking = false;    
  }
  return;
}

//
//Function: timeout
//
//Member-Of: Timer
//
//Description:
//      The timeout function is called by the timer task when
//      time-out occurs. It creates a time-out message and asks the
//      task to store it its message queue. 
//
void Timer :: timeout (void)
{
  if (task) {
    Message *msg = new TimerMessage (getInitial ());
    msg->mark (this, this);
    task->save (msg, SystemEvent);
    }
  ticking = false;
  return;
}


//
//Function: delta
//
//Member-Of: Timer
//
//Return:
//      OTime
//
//Description:
//      The delta function tells the time left before the time-out.
//      
//      Returns the time left before the time-out if the timer has
//      been started previously. Otherwise returns zero time.
//
OTime Timer :: delta (void) const
{
    if (ticking) {
	return timeoutTime - getTimerTask ()->getLocalTime ();
    }
    return 0;
}











