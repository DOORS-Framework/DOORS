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

$Log: rttask.h,v $
Revision 1.4  2002/07/25 10:08:39  ik
Use std::string.

Revision 1.3  2002/07/23 14:25:20  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:51  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:57  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:33  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef RTTASK_H
#define RTTASK_H

#include <string>
#include <sys/time.h>
#include <doors/ptb.h>

/** Real-time version of PTask
  */
class RTTask : public PTask {
public:
  /** Constructor for a real-time task. Similar to PTask constructor
      except for the usecs-parameter that sets an initial value for
      tick interval (= time between calls to timeout function).
      @param n Task name
      @param s Scheduler
      @param sm State machine
      @param usecs Tick interval in microseconds
  */
  RTTask(std::string n, Scheduler *s, StateMachine *sm, int usecs);

  /** Stops the timer
    */
  void stopTimer();

  /** Starts the timer
    */
  void startTimer();

  /** Virtual function that is called on every tick. The function should
      return the time to the next call of this function in milliseconds.
      If it returns zero, the timer is stopped and it can be started again
      with startTimer(). The default implementation just returns zero.
    */
  virtual int RTCallback() { return 0; }

  /** Timer callback function
      @param msg DOORS message
      @return If false, message is to be deleted by DOORS
  */
  bool TimerCallback (Message *msg);

private:
  Timer timer_;                    // Tick timer object
  struct timeval lasttime_;        // Timestamp of the last RTP packet
  int usecs_;			   // Delay between packets

  /** Wait for the time when to call timeout function. That time
      is calculated from the timestamp of the last tick plus the
      time given in usecs parameter. If the calculated time is in the
      future, this method sleeps until that moment using select system
      call. If the time has already gone, the method returns immediately
      and false is returned to indicate that we are not in sync with
      the system clock.
      The timestamp is saved every time this method is called. If
      this method is called for the first time, the current system time is
      saved and subsequent calls are based on that base time and the
      current system time.
      @param usecs Delay between the last and the next timeout function call
                   (microseconds)
      @return true if we are in sync with system clock
  */
  bool wait_for_calltime(unsigned int usecs);

};

#endif
