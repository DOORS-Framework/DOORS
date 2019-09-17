//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
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

$Log: rttask.C,v $
Revision 1.4  2002/07/25 10:08:39  ik
Use std::string.

Revision 1.3  2002/07/23 14:25:20  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:51  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:33  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:33  bilhanan
Full licence clause added into doors 0.1


*/

#include <string>
#include <sys/time.h>
#include <doors/ptb.h>
#include <doors/rttask.h>

#ifndef timersub
#define timersub(a, b, result)                                                \
  do {                                                                        \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;                             \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;                          \
    if ((result)->tv_usec < 0) {                                              \
      --(result)->tv_sec;                                                     \
      (result)->tv_usec += 1000000;                                           \
    }                                                                         \
  } while (0)
#endif
#ifndef timeradd
#define	timeradd(a, b, result)						      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;			      \
    (result)->tv_usec = (a)->tv_usec + (b)->tv_usec;			      \
    if ((result)->tv_usec >= 1000000)					      \
      {									      \
	++(result)->tv_sec;						      \
	(result)->tv_usec -= 1000000;					      \
      }									      \
  } while (0)
#endif

RTTask::RTTask(std::string n, Scheduler *s, StateMachine *_sm, int usecs)
  : PTask(n, s, _sm), timer_ (this, OTime (0, 1)), usecs_(usecs)
{
  timerclear(&lasttime_);
}

void RTTask::startTimer()
{
  timerclear(&lasttime_);
  timer_.start();
}

void RTTask::stopTimer()
{
  timer_.stop();
}

bool RTTask :: wait_for_calltime(unsigned int usecs)
{
  struct timeval now, subtime, futuretime;
  struct timeval zerotime = {0, 0};
  bool in_sync = false;

  if (lasttime_.tv_sec == 0)
    ::gettimeofday(&lasttime_, 0);

  ::gettimeofday(&now, 0);
  // Convert useconds to struct timeval
  subtime.tv_sec  = usecs / 1000000;
  subtime.tv_usec = usecs % 1000000;
  // Resolve the timestamp in the future adding wanted timeout
  // to the last timestamp.
  timeradd(&lasttime_, &subtime, &futuretime);
  // Resolve time that we need to wait
  timersub(&futuretime, &now, &subtime);
  // If sending time is in the future, have a nap
  if (timercmp(&subtime,&zerotime,>)) {
    select(0, NULL, NULL, NULL, &subtime);
    in_sync = true;
  }
  lasttime_.tv_sec = futuretime.tv_sec;
  lasttime_.tv_usec = futuretime.tv_usec;
  return in_sync;
}

bool RTTask :: TimerCallback (Message *msg)
{
  // Start timer again
  timer_.start();

  // If interval is not yet set, call the real-time callback
  // function for the first time and return. The callback function 
  // should set interval
  if (usecs_ == 0) {
    usecs_ = RTCallback();
    if (usecs_ == 0) // If zero is returned, stop the timer
      timer_.stop();
    return false;
  }

  bool in_sync = true;
  do {
    // Wait for the next tick
    in_sync = wait_for_calltime(usecs_);
    usecs_ = RTCallback();
    if (usecs_ == 0) { // If zero is returned, stop the timer and break the loop
      timer_.stop();
      break;
    }
  } while (!in_sync);

  return false;
}





