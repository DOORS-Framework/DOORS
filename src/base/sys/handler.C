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

$Log: handler.C,v $
Revision 1.3  2002/08/14 09:31:40  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/07/23 14:25:21  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:20  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:14  bilhanan
Full licence clause added into doors 0.1


*/

#include <doors/handler.h>
#include <doors/osched.h>
#include <doors/ocoremac.h>
#include <doors/debug.h>
#include <signal.h>

const int syncTime = 10;

//
//Constructor: IoHandler
//
//Parameters:
//      Scheduler *sched
//
//Description:
//      The constructor of the i/o handler initializes the local
//      time and variables and gives itself to the scheduler 
//      given as parameter. The i/o handler can belong to only one
//      scheduler at time. 
//
//      The parameter sched is a pointer to the scheduler that will 
//      give time to this task.
//
IoHandler :: IoHandler (Scheduler *sched, const OTime &d) 
: Otask ("I/O-Handler", sched), nextTimeout (0), counter (0), delta (0),
  running (true)
{
  _MT_SYNCMONITOR;

  FD_ZERO (&readfds);
  FD_ZERO (&writefds);
  FD_ZERO (&excefds);
  
  adjustTime ();
  setLoad (1);
  signal(SIGPIPE, SIG_IGN); // hack

}


//
//Destructor: IoHandler
//
//Description:
//      The destructor clears the scheduler's reference of the 
//      i/o handler because it does not exist any longer. 
//
IoHandler :: ~IoHandler (void)
{
  _MT_SYNCMONITOR;

  running = false;

  if (delta) {
    delete delta;
    delta = 0;
  }

  if (scheduler) {
    scheduler->setIoHandler (0);
  }
}

void IoHandler::run(void)
{
    _MT_SYNCMONITOR; 

    int nfds;
    OTime poll;
    OTime *pollTime = &poll;
    update ();
  
    counter++; 
    // increase localtime validity counter
    // localTime will be adjusted, after counter reaches syncTime

    // if load of the scheduler > 1, then there are also some
    // other tasks to be runned and select should not block at all ?
    if (scheduler->getLoad() <= 1) {
	if(nextTimeout > 0) {
	    poll = nextTimeout - getTime();          // set waiting time to next timeout
	    if (!(poll > 0)) {                       // negative timeout
		poll = 0;
	    } else if (delta && !(poll > *delta)) {  // user has set delta
		pollTime = delta;                    // and it is smaller than next timeout
	    } 
	} else {
	    pollTime = delta;                        // wait for delta if no timeouts
	}
	// we have to adjust the clock after select if not polling
	counter = syncTime; // invalidate localTime
    }

    // _DOORS_DEBUG(0,"IoHandler makes *nuuh*");
    _MT_RELEASE_SYNCMONITOR;
    if ((nfds = SELECT (NFDBITS, &readfds, &writefds, &excefds, pollTime)) < 0) {
	_DOORS_FAULT(0,"unmanaged error condition on select");
	// sophisticated error-handling needed here
    }    
    _MT_ACQUIRE_SYNCMONITOR;
    
    (void) getTime (); // syncronizes time, if necessary
  
    // _DOORS_DEBUG(0,"Iohandlers nextTimeout: " << nextTimeout << " vs localTime: " << localTime);

    // check whether devices are ready for reading, writing, 
    // exception, and time-out, or not, and call 
    // callback routines to handle the i/o requests
    if ((nfds > 0) || ((nfds == 0) && (nextTimeout > 0))) {
	DeviceIterator iterator(&device);
	register Device *d;
	while (d = iterator.next ()) {
	    _lastRemoved = 0;
	    if(d->getFd() >= 0) {
		if (FD_ISSET (d->getFd(), &readfds)) {
		    _MT_RELEASE_SYNCMONITOR; 
		    _MT_STABLE_MONITOR; 
		    _MT_ACQUIRE_SYNCMONITOR;
		    d->callbackRead ();
		}
	    }
	    if(d == _lastRemoved) continue;
	    if(d->getFd() >= 0) {
		if (FD_ISSET (d->getFd(), &writefds)) {
		    _MT_RELEASE_SYNCMONITOR; 
		    _MT_STABLE_MONITOR; 
		    _MT_ACQUIRE_SYNCMONITOR;
		    d->callbackWrite ();
		}
	    }
	    if(d == _lastRemoved) continue;
	    if(d->getFd() >= 0) {
		if (FD_ISSET (d->getFd(), &excefds)) {
		    _MT_RELEASE_SYNCMONITOR; 
		    _MT_STABLE_MONITOR; 
		    _MT_ACQUIRE_SYNCMONITOR;
		    d->callbackException ();
		}
	    }
	    if(d == _lastRemoved) continue;
	    if ((d->getStatus() & Device::TIMEOUT) && (d->getTimeout() <= localTime)) {
		_MT_RELEASE_SYNCMONITOR; 
		_MT_STABLE_MONITOR; 
		_MT_ACQUIRE_SYNCMONITOR;
		d->callbackTimeout ();
	    }
	}
    }
}


//
//Function: inform
//
//Member-Of: IoHandler
//
//Parameters:
//      Device *dev
//
//Description:
//      Takes information about a new device into the i/o handler's 
//      internal structures. Typically called by the constructors of
//      devices. 
//
//      The paramter dev is a new device to be monitored.
//
void IoHandler :: inform (Device *dev)
{
  _MT_SYNCMONITOR;

  device.append (dev, false);
  return;
}


//
//Function: forget
//
//Member-Of: IoHandler
//
//Parameters:
//      Device *dev
//
//Description:
//      Removes information about a device from the i/o handler's 
//      internal structures. Typically called by the destructors of
//      devices.
//
//      The parameter dev is a device not to be monitored any more.
void IoHandler :: forget (Device *dev)
{
  _MT_SYNCMONITOR;
  device.remove (dev);
  _lastRemoved = dev;
  return;
}


//
//Function: update
//
//Member-Of: IoHandler
//
//View: protected
//
//Description:
//      Builds the bit fields for the select system call and
//      calculates the next time-out. 
//
void IoHandler :: update (void)
{
  _MT_SYNCMONITOR;
  
  DeviceIterator iterator (&device);
  register Device *d;

  FD_ZERO (&readfds);
  FD_ZERO (&writefds);
  FD_ZERO (&excefds);
  nextTimeout = 0.0L;
  
  while (d = iterator.next ()) {
    if(d->getFd() >= 0) {
      // negative file descriptor means error
      if (d->getStatus() & Device::READ) {
	FD_SET (d->getFd(), &readfds);
      } 
      if (d->getStatus() & Device::WRITE) {
	FD_SET (d->getFd(), &writefds);
      } 
      if (d->getStatus() & Device::EXCEPTION) {
	FD_SET (d->getFd(), &excefds);
      } 
    }
    if ((d->getStatus() & Device::TIMEOUT) &&
	((nextTimeout == 0) || (d->getTimeout() <= nextTimeout))) {
      nextTimeout = d->getTimeout();
    }
  }
}


//
//Function: ajustTime
//
//Member-Of: IoHandler
//
//View: protected
//
//Description:
//      Syncronizes the local time to the system time.
//
void IoHandler :: adjustTime (void)
{
  _MT_SYNCMONITOR;

  struct timezone tz;
  gettimeofday (&localTime, &tz);
}


//
//Function: getTime
//
//Member-Of: IoHandler
//
//Return:
//      OTime
//
//Description:
//      Returns the local time used in the handler.
//      Adjust localTime if validity counter has reached syncTime.
//
OTime &IoHandler :: getTime (void) 
{
  _MT_SYNCMONITOR;

  // if the handler is executed syncTime times
  // local time is adjusted
  if (counter >= syncTime) {
    adjustTime ();
    counter = 0;
  }
  
  return localTime;
}

void IoHandler :: setDelta (const OTime *d) 
{
    _MT_SYNCMONITOR;
    if (delta) {
	delete delta;
	delta = 0;
    }
    if(d) {
	delta = new OTime (*d);
    }
}


IoHandler *IoHandler :: Instance (Scheduler *s){
 
  Scheduler *sched = s;
  if (_instance == 0){
    
    //IOHandler created for the first time
    _DOORS_DEBUG(0, "Launching Iohandler");
    _instance = new IoHandler(s);

    if (sched == 0){
      sched = Scheduler::Instance(_instance);

    }
    _instance->scheduler = sched;
    sched->request (_instance, DOORS_PRIORITY_NORMAL);
   
  }
 
  return _instance;
 
}
 
IoHandler *IoHandler :: _instance = 0;


