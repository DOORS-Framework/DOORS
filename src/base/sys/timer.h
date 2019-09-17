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

$Log: timer.h,v $
Revision 1.2  2002/07/23 14:25:22  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:05  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:26  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef TIMER_H
#define TIMER_H

#include <doors/message.h>
#include <doors/etask.h>
#include <doors/port.h>
#include <doors/obl_dcl.h>
#include <doors/obl_time.h>

/** A message that is used when a time-out occurs. Created by
      timers. */
class TimerMessage : public SystemMessage {
    public:
        TimerMessage (OTime &t) : SystemMessage (M_TIMER), count (t) {};
        OTime count;
};


/**A timer class provides an interface to the timer task. Timers
  and a timer task together provide fully operating timers. 
  The timer requires that the scheduler of the task to which 
  the timer belongs knows the timer task. 
  
  Commentary:
  Running timers are kept in one list in the timer task. This is
  because the list of devices is wanted to be as short as
  possible. Now only the timer task is a device, not all timers,
  which was one possiblity. To have timers in a timer task
  timers can be distributed to many timer tasks for better
  performance. */
class Timer : public Port {
    public:
  /** A default constructor that should be used only if
    forced. The timer is unusable if the there is no information
    about the task to which it belongs. Hence, it cannot send 
    time-out messages. */
  Timer (void);

  /** The constructor initializes the internal variables. The task
    to which a timer belongs is needed because timer must create
    and store a time-out message into the message queue of the task.
    <p>
    There must also be a timer task in the system that the
    scheduler knows. Without information about the timer task 
    a timer cannot be started.
    
    @param host is the task to which this timer belongs.*/
  Timer (TimerTask *host);

  /** The constructor initializes the internal variables. The task
    to which a timer belongs is needed because timer must create
    and store a time-out message into the message queue of the task.
    <p>
    There must also be a timer task in the system that the
    scheduler knows. Without information about the timer task 
    a timer cannot be started.
    
    @param host is the task to which this timer belongs.
    @param count is an initial time for time-out. */
  Timer (EventTask *host, const OTime &count);

  /** The destructor just tells the timer task to forget this timer,
    if the timer is on */
  virtual ~Timer (void);

  /**An initialization function for initialization after the 
    timer is created. This is just for a case when you need
    to create timers without parameters and then initialize
    them afterwards.
    @param host the task to which this timer belongs.
    @param count is an initial time for time-out. */
  virtual void init (EventTask *host, const OTime &count);

  /** The start function starts the timer. A timer can be started
    again without stopping it first. Information about the running
    timers is kept in the timer task. This timer is just an
    interface for the timer task. */
  virtual void start (void);
  /** function stops the timer. It tells the timer task to 
      remove its information the list of the timer task where all
      running timers are kept. A timer can be stopped, even it has
      not been started. */
  virtual void stop (void);

  /**The timeout function is called by the timer task when
    time-out occurs. It creates a time-out message and asks the
    task to store it its message queue.*/
  virtual void timeout (void);

  /** The delta function tells the time left before the time-out.
      
    @return the time left before the time-out if the timer has
    been started previously. Otherwise returns zero time. */
  OTime delta (void) const;

  void setTimeout (OTime &time) { timeoutTime = time; }
  const OTime &getTimeout (void) const { return timeoutTime; }
  void setInitial (OTime &count) { initialTime = count; }
  OTime &getInitial (void) { return initialTime; }
  TimerTask *getTimerTask (void) const { return timers; }
  bool isTicking (void) { return ticking; }

protected:
    OTime timeoutTime;
    OTime initialTime;
    bool ticking;
    TimerTask *timers;

private:
    // use of port methods is denied
    bool connect (Port *) { return false; }
    bool disconnect (void) { return false; }
    bool disconnectRequest (Port *) { return false; }
    bool putMessage (Message *) { return false; }
    bool getMessage (Message *) { return false; }
};


#endif  // TIMER_H

