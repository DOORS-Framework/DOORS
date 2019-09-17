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

$Log: handler.h,v $
Revision 1.4  2002/12/17 13:35:56  ik
IoHandler constructor moved from private: to protected:

Revision 1.3  2002/08/14 09:31:41  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/07/23 14:25:21  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:33  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:39:14  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef _doors_handler_
#define _doors_handler_


#include <doors/library.h>
#include <doors/otask.h>
#include <doors/util.h>
#include <doors/device.h>
#include <doors/obl_time.h>

typedef TQueue<Device> DeviceList;
typedef TQueueListIterator<Device> DeviceIterator;


/**
      The i/o handler is the task that handles all devices in the 
      system. It has pointers to devices. Devices inform the handler
      when they are created and deleted. In this way, the handler
      knows all the time proper devices. Each time the handler is
      called it constructs the parameters for the select system
      call and calls it to check the fds. Depending on the return
      values of the select it serves devices by calling their 
      callback routines. 

      The handler is used by schedulers, usually. And it should not
      be seen by programmers. Of course, they must create an instance
      of the handler. 

      This handler is dependent on UNIX operating system because it
      uses the select system call to monitor the fds associated with
      devices. The use of the select system call is the main purpose
      of this task. So, it is, perhaps, better to write another
      handler for different operating system, if needed. 

      Timers are implemented as devices because the handler must
      know what is the maximum time it can block. So, it is natural
      that devices have time-out property. 

      The handler maintains its own local time. Local time is
      adjusted by the help of the select system call. And it is also
      syncronized with the sytem time now and then. Local time is
      to provide time with micro seconds. This is needed when we
      want to have timers that have time-out value less than one
      second. 
      */

class IoHandler : public Otask {

  static IoHandler *_instance;
  
 public:
  virtual ~IoHandler (void);
  
  void run (void);
  void inform (Device *dev);
  void forget (Device *dev);
  OTime &getTime (void);
  void setDelta (const OTime *d);
  void getSelectParams (fd_set *, fd_set *, fd_set *, timeval *);
  
  static IoHandler *Instance (Scheduler *s = 0);
  
 protected:
  IoHandler (Scheduler *sched = 0, const OTime &d = 0);

      void update (void);
      void adjustTime (void);

    // vars
    DeviceList device;
    fd_set readfds;
    fd_set writefds;
    fd_set excefds;
    OTime localTime;
    OTime nextTimeout;
    OTime *delta;
    int counter;
    bool running;
    Device *_lastRemoved; // set in inform & check in run if device id deleted
};


//
//Inline-Functions:
//


//
//Function: getSelectParams
//
//Member-Of: IoHandler
//
//Parameters:
//      struct fd_set *r, 
//      struct fd_set *w,
//      struct fd_set *e, 
//      struct timeval *t
//
//Description:
//      Returns in parameters the fd_sets that includes information (fds)
//      about the devices on the system. Also, retuns the time of the 
//      next time-out. This function is to provide help for using
//      many systems, which use select system call, in the same prosess. 
//
//      The parameter r is a fd_set used for reading, and w is a fd_set 
//      used for writing, and e is a fd_set used for exceptions, and
//      finally t is the next time-out time.
//
inline void IoHandler 
    :: getSelectParams (fd_set *r, fd_set *w, 
                        fd_set *e, timeval *t)
{
  _MT_SYNCMONITOR;

  update ();
  if (r) {
    *r = readfds;
  }
  if (w) {
    *w = writefds;
  }
  if (e) {
    *e = excefds;
  }
  if (t) {
    *t = nextTimeout;
  }
  return;
}


#endif  

