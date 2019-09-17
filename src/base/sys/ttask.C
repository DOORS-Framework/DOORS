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

$Log: ttask.C,v $
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

Revision 1.2  2001/06/08 16:46:29  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:27  bilhanan
Full licence clause added into doors 0.1


*/

#include <doors/ttask.h>
#include <doors/osched.h>
#include <doors/handler.h>


//
//Function: findPlace
//
//View: private
//
//Parameters:
//      Timer *a, Timer *b
//
//Return:
//      bool
//
//Description:
//      The function is used to find the right place in the list where
//      all timers are kept. This is used by the timer task, only.
//
//      The paramter a is a timer list element to be compared and the
//      b is an added timer.
//
static bool findPlace (const Timer *a, const Timer *b) 
{
    return (a && b) ? ((a->getTimeout () > b->getTimeout ()) ? true : false) : true;
}

//
//Constructor: TimerDevice
//
//Parameters:
//      IoHandler *io
//      TimerTask *t 
//
//Description:
//      Initializes the device.


TimerDevice :: TimerDevice (IoHandler *io, TimerTask *t)
: Device (io), task (t), stopTime (0), timeout (0), delta (0)
{
  _MT_SYNCMONITOR;
}

//
//Destructor: TimerDevice
//
//Description:
//      Removes the device from IoHandler list.

TimerDevice :: ~TimerDevice (void) 
{
  _MT_SYNCMONITOR;

  if (handler) {
    handler->forget (this);
    handler = 0;
  }
}

//
//Function: getTimeout
//
//Member-Of: TimerDevice
//
//Return: OTime
//
//Description:
//      Returns the next value of next timeout.
//      

OTime TimerDevice :: getTimeout (void) const 
{
  _MT_SYNCMONITOR;

  return timeout;
}

//
//Function: setTimeout
//
//Member-Of: TimerDevice
//
//Parameters: OTime
//
//Description:
//      Set the value of the next timeout.
//      

void TimerDevice :: setTimeout (OTime time) 
{
  _MT_SYNCMONITOR;

  if (time == 0) {
    status &= ~TIMEOUT;
    timeout = 0;
  } else {
    timeout = time + delta;
    status |= TIMEOUT;
  }
  _MT_SIGNAL_RUNLOCK;
}

//
//Function: getStopTime
//
//Member-Of: TimerDevice
//
//Return: OTime
//
//Description:
//      Return the time when system were stopped
//      

OTime TimerDevice :: getStopTime (void) const 
{
  _MT_SYNCMONITOR;

  return stopTime;
}

//
//Function: getLocalTime
//
//Member-Of: TimerDevice
//
//Return: OTime
//
//Description:
//      Return the system time.
//      

OTime TimerDevice :: getLocalTime (void) const 
{
  _MT_SYNCMONITOR;

  return getTime () - delta;
}

//
//Function: callbackTimeout
//
//Member-Of: TimerDevice
//
//Description:
//      Function for handling timeouts, and it is called 
//      by IoHandler. If timeout has occurred, the run
//      function of TimerTask is called (synchronously).
//

void TimerDevice :: callbackTimeout (void) 
{
  _MT_SYNCMONITOR;

  OTime local = getTime ();
  if (getTimeout() <= local) {
    status &= ~TIMEOUT;
    _MT_S_OTHER_RUNLOCK(task);
    _MT_RUN(task);
    timeout = 0;
    task->activate ();
  }
}

//
//Function: getFd
//
//Member-Of: TimerDevice
//
//Return: int
//
//Description:
//      Return the file descriptor of the device.
//      In timer device there is no file descriptors,
//      so -1 is returned. 
//

int TimerDevice :: getFd (void) const 
{  
  _MT_SYNCMONITOR;

  return -1;
}

//
//Function: start
//
//Member-Of: TimerDevice
//
//Description:
//      Start the system and calculate correct time value for delta, 
//      which is used for handling starting and stopping of the
//      system.
//

void TimerDevice :: start (void) 
{
  _MT_SYNCMONITOR;

  if (stopTime == 0) {
    return;
  }

  status |= TIMEOUT;
  delta = getTime () - stopTime;
  stopTime = 0;
}

//
//Function: stop
//
//Member-Of: TimerDevice
//
//Description:
//      Stop the system and calculate correct time value for stoptime, 
//      which is used for handling starting and stopping of the
//      system.
//

void TimerDevice :: stop (void) 
{
  _MT_SYNCMONITOR;

  if (stopTime == 0) {
    stopTime = getTime () - delta;
    status &= ~TIMEOUT;
  }
}

//
//Function: getTime
//
//Member-Of: TimerDevice
//
//Return: const OTime
//
//Description:
//      Return the system time either from IoHandler 
//      or from operating system.
//

const OTime TimerDevice :: getTime (void) const 
{
  _MT_SYNCMONITOR;
  
  if (handler) {
    return handler->getTime ();
  }
  else {
    struct timezone tz;
    OTime local;
    gettimeofday ((timeval *) &local, &tz);
    return local;
  }
}


//
//Constructor: TimerTask
//
//Parameters:
//      Scheduler *sched, 
//      IoHandler *io
//
//Description:
//      Initialize object and inform scheduler. New TimerDevice 
//      is also created.
//

TimerTask :: TimerTask (Scheduler *sched, IoHandler *io) 
: Otask("Timer Task", sched), tDevice(0), stopped(false)
{
  _MT_SYNCMONITOR;

  if (sched) {
    sched->setTimerTask (this);
  }

  tDevice = new TimerDevice (io, this);
  _MT_SCHEDULE(tDevice);
}

//
//Destructor: TimerTask
//
//Description:
//      Delete instance and TimerDevice.
//

TimerTask :: ~TimerTask (void) 
{ 
  _MT_SYNCMONITOR;

  _MT_KILLTHREAD(this);
  if (tDevice)
    delete tDevice;
}

//
//Function: inform
//
//Member-Of: TimerTask
//
//Parameters:
//      Timer *tmr
//
//Description:
//      Takes a new timer to the timer task's internal data
//      structures to be monitored for possible time-out. This function 
//      is usually called when a timer is started and used only by timers. 
//
//      The timer itself is responsible to removing itself from the
//      timer task when it is deleted (or started twice). Typically
//      called by timers. 
//

void TimerTask :: inform (Timer *tmr) 
{
  _MT_SYNCMONITOR;

  OTime to = tmr->getInitial () + getLocalTime ();
  tmr->setTimeout (to); 
  timers.add (tmr, findPlace, NotDestroyed);
  updateTimeout ();
  return;
}


//
//Function: forget
//
//Member-Of: TimerTask
//
//Parameters:
//      Timer *tmr
//
//Description:
//      With this function timers tell the timer task to remove the 
//      given timer from its data structures. The timer needs no more
//      attention i.e. the timer is no longer in use. This function
//      is used by timers, only. 
//

void TimerTask::forget (Timer *tmr) 
{
  _MT_SYNCMONITOR;

  timers.remove (tmr);
  updateTimeout ();
}

//
//Function: isStopped
//
//Member-Of: TimerTask
//
//Return: bool
//
//Description:
//      Returns true if the timer task is stopped.
//

bool TimerTask :: isStopped (void) 
{
  _MT_SYNCMONITOR;

  return stopped;
}    

//
//Function: stoppedTime 
//
//Member-Of: TimerTask
//
//Return: OTime
//
//Description:
//      Returns a time when the timer task has been stopped.
//

OTime TimerTask :: stoppedTime (void)
{
  _MT_SYNCMONITOR;

  return tDevice->getStopTime ();
}    

//
//Function: start
//
//Member-Of: TimerTask
//
//Description:
//      This function starts the timer task. The debug scheduler will call 
//      it when the scheduling is restarted. If the timer task is not 
//      in stopped state nothing will be done.
//

void TimerTask :: start (void)
{
  _MT_SYNCMONITOR;

  if (!stopped) {
    return;
  }
  
  stopped = false;
  tDevice->start ();
  updateTimeout ();
}

//
//Function: stop
//
//Member-Of: TimerTask
//
//Description:
//      This function stops the timer task. The debug scheduler will call 
//      it when the scheduling is stopped. If the timer task is already
//      in stopped state nothing will be done.
//

void TimerTask :: stop (void)
{
  _MT_SYNCMONITOR;

  if (!stopped) {
    tDevice->stop ();
    stopped = true;
  }
}    

//
//Function: updateTimeout
//
//Member-Of: TimerTask
//
//Description:
//      This is an internal function of the timer task. It is used for
//      updating device dependent information, for example, the next
//      time when a time-out occurs and the status of the device, which
//      indicates if there is a timer running. 
//

void TimerTask::updateTimeout (void)
{
  _MT_SYNCMONITOR;

  TimerListIterator iterator (&timers);
  Timer *next = iterator.head();
  
  if (next) {
    tDevice->setTimeout (next->getTimeout ());
  } else {
    tDevice->setTimeout (0);
  }
}

//Function: activate
//
//Member-Of: TimerTask
//
//Description:
//      This function is called from TimerDevice::callbackTimeout to  
//      make timerTask to request running fron scheduler
//
void TimerTask::activate (void)
{
    // Inform the scheduler that this task need to be executed in near future
    if (scheduler && getLoad () == 0) {
	scheduler->request (this, priority);
	setLoad (1);
    }
}


//
//Function: run
//
//Member-Of: TimerTask
//
//Description:
//      The run function is the main function of the timer task. It
//      monitors timers that are running. If the function notices that
//      a timer (or timers) is triggered it calls the timeout function
//      of the timer to create a time-out message and to save it to the 
//      message queue of the task. This function is called by TimerDevice
//      from its callbackTimeout function.
//

void TimerTask :: run (void) 
{
  _MT_SYNCMONITOR;

  TimerListIterator iterator (&timers);
  Timer *tmr;
  
  while ((tmr = iterator.head ()) && ((OTime &) tmr->getTimeout ()) <= getLocalTime ()) {
    timers.remove (tmr);
    tmr->timeout ();
  }
  updateTimeout ();
  setLoad (0);
}


//
//Function: getLocalTime
//
//Member-Of: TimerTask
//
//Return:
//      OTime
//
//Description:
//      Returns the local time that the i/o handler uses. 
//

OTime TimerTask :: getLocalTime (void)
{
  _MT_SYNCMONITOR;

  if (stopped) {
    return tDevice->getStopTime ();
  } else {
    return tDevice->getLocalTime ();
  }
}

//
//Function: instance
//
//Member-Of: TimerTask
//
//Parameters:
//      Scheduler *s
//      IoHandler *io
//
//Return: TimerTask *
//
//Description:
//      Function is used for creating a instance of TimerTask.
//      This insures that there is only one TimerTask in the system.
//

TimerTask *TimerTask :: instance (Scheduler *s, IoHandler *io)
{
  if (_instance == 0) {
    _MT_STATIC_SMONITOR(_DOORS_syncLock);
    if (_instance == 0) {
      _instance = new TimerTask (s, io);  
    } 
  }
  
  return _instance;
}


TimerTask *TimerTask :: _instance = 0;

_MT_LOCK (TimerTask :: _DOORS_syncLock);
_MT_KILLALL_TTIMPL(tDevice);








