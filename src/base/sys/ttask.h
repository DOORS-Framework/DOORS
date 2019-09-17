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

$Log: ttask.h,v $
Revision 1.5  2003/08/14 12:21:54  bilhanan
_ONS_syncLock is now _DOORS_syncLock

Revision 1.4  2002/08/14 09:31:41  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 14:25:22  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:54  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:06  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:27  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef TTASK_H
#define TTASK_H


#include <doors/library.h>
#include <doors/util.h>
#include <doors/otask.h>
#include <doors/timer.h>
#include <doors/device.h>

typedef TQueue<Timer> TimerList;
typedef TQueueListIterator<Timer> TimerListIterator;

class TimerDevice;

/** The purpose of a timer task is to hold the information of 
  running timers.<p>
  
  The usual user needs only to make the instance of the timer
  task if timers are used. Time-outs are seen as time-out 
  messages in the user's task. All other functions are not
  seen by the users.<p>
 
  The timer task holds the running timers in a linked list. This
  may cause performance problems if there are lots of timers. 
  The timer task must scan the list through when removing (stopping)
  timers.
  <p>
  When the timer task is executed it checks if a time-out has
  occurred. If there is a time-out it calls the timeout
  function of the timer to create a time-out message.
  <p>
  Commentary<p>
  A timer task is designed to handle timers of tasks. All timers
  are in one linked list. This may cause performance problems
  if there are lots of timers in the system. In this case you 
  should consider of using multiple timer tasks. But then you
  must have multiple schedulers, also, that may be a bigger
  problem. Performance decrease is due to scanning the linked
  list when removing the information of a stopped timer. */

class TimerTask : public Otask {
 public:
  /** Delete instance and TimerDevice. */
  ~TimerTask (void);

  /**Initialize object and inform scheduler. New TimerDevice 
    is also created. 
    @param sched is the scheduler of system
    @param io is the i/o handler of the system*/
  static TimerTask *instance (Scheduler *scheduler = 0, IoHandler *io = 0);

  /**The run function is the main function of the timer task. It
    monitors timers that are running. If the function notices that
    a timer (or timers) is triggered it calls the timeout function
    of the timer to create a time-out message and to save it to the 
    message queue of the task. This function is called by TimerDevice
    from its callbackTimeout function. */
  virtual void run (void);

  /**Takes a new timer to the timer task's internal data
    structures to be monitored for possible time-out. This function 
    is usually called when a timer is started and used only by timers. 
    <p>
    The timer itself is responsible to removing itself from the
    timer task when it is deleted (or started twice). Typically
    called by timers.
    @param tmp the new timer.*/
  void inform (Timer *tmr);

  /** With this function timers tell the timer task to remove the 
    given timer from its data structures. The timer needs no more
    attention i.e. the timer is no longer in use. This function
    is used by timers, only.
    @param timer the timer to be forgotten.*/
  void forget (Timer *tmr);

  /** @return the local time that the i/o handler uses. */
  OTime getLocalTime (void);

  /** This function is called from TimerDevice::callbackTimeout to  
    make timerTask to request running fron scheduler. */
  void activate (void);

  /** @return true if the timer task is stopped. */
  bool isStopped (void);

  /** @return a time when the timer task has been stopped. */
  OTime stoppedTime (void);
  
  /** This function starts the timer task. The debug scheduler will call 
    it when the scheduling is restarted. If the timer task is not 
    in stopped state nothing will be done. */
  void start (void);

  /** This function stops the timer task. The debug scheduler will call 
    it when the scheduling is stopped. If the timer task is already
    in stopped state nothing will be done. */
  void stop (void);

  _MT_GETSYNCLOCK
  _MT_KILLALL_DEC
  
 protected:
  TimerTask (Scheduler *scheduler = 0, IoHandler *io = 0);

  /** This is an internal function of the timer task. It is used for
    updating device dependent information, for example, the next
    time when a time-out occurs and the status of the device, which
    indicates if there is a timer running. */
  void updateTimeout (void);
  
  TimerDevice *tDevice;
  TimerList timers;
  bool stopped;
  static TimerTask *_instance;  
  _MT_STATICLOCK(_DOORS_syncLock)

};



/** The purpose of a timer device is to hold the information of 
  the next timeout and co-operate with IoHandler.
  <p> 
  When IoHandler is runned, timeouts are also checked.
  The callback function of timer device is called, and
  if needed, the timer task run function is called. */
class TimerDevice : public Device {
public:
  /** Initializes the device. 
   @param ioh is the i/o handler of the system.
   @param t is the Timer task of system.*/
  TimerDevice (IoHandler *io, TimerTask *t);

  /** Removes the device from IoHandler list. */
  ~TimerDevice (void);

  /** return the filedescriptor of device. In timer device there
    is no file descriptors, so -1 is returned.  */
  int getFd (void) const;
  
  /**return the next value of next timeout. */
  OTime getTimeout (void) const;

  /** Set the value of the next timeout. */
  void setTimeout (OTime);

  /** @return the time when system were stopped */
  OTime getStopTime (void) const;
  
  /** @return the system time. */
  OTime getLocalTime (void) const;

  /** Start the system and calculate correct time value for delta, 
    which is used for handling starting and stopping of the
    system. */
  void start (void);
  /** Stop the system and calculate correct time value for stoptime, 
    which is used for handling starting and stopping of the
    system. */
  void stop (void);

  _MT_DEVICERUN
  _MT_POLLTIMERDEVICE

 protected:

  OTime timeout;
  OTime stopTime;
  OTime delta;

  TimerTask *task;
  
 private:
  void callbackTimeout (void);
  /** @return the system time either from IoHandler  or from operating system.
   */
  const OTime getTime (void) const;
};


#endif  // TTASK_H



