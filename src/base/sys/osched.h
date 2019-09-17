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

$Log: osched.h,v $
Revision 1.4  2002/12/17 13:36:31  ik
Development.

Revision 1.3  2002/07/25 10:08:40  ik
Use std::string.

Revision 1.2  2002/07/23 14:25:22  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:48  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.7  2001/06/08 08:39:19  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef _doors_Sched_
#define _doors_Sched_

#include <string>
#include <doors/library.h>
#include <doors/otask.h>
#include <doors/util.h>

/** High task priority */
#define DOORS_PRIORITY_HIGH    0

/** Normal task priority */
#define DOORS_PRIORITY_NORMAL  1

/** Low task priority */
#define DOORS_PRIORITY_LOW     2

/** Background task priority */
#define DOORS_PRIORITY_BACK    3

#define DOORS_PRIORITY_TIMER   DOORS_PRIORITY_HIGH
#define DOORS_PRIORITY_HIGHEST DOORS_PRIORITY_HIGH
#define DOORS_PRIORITY_LOWEST  DOORS_PRIORITY_BACK
#define DOORS_NO_PRIORITIES    4


/** Seqeuence of OTask pointers */
typedef TQueue<Otask> OtaskList;

/** Iterator for OTask* sequence */
typedef TQueueListIterator<Otask> OtaskIterator;


class Scheduler : public Otask {

protected:
  Scheduler (IoHandler *ioh = 0, std::string n = "Round robin scheduler", TimerTask *tt = 0, Scheduler *s = 0);
  static Scheduler* _instance;

public:

  static Scheduler* Instance (IoHandler *ioh = 0, std::string n = "Round robin scheduler", TimerTask *tt = 0, Scheduler *s = 0);
  virtual ~Scheduler (void);
    /** Hmm, what is this ? */
  virtual void finished (Otask *task, Load load) {};
  virtual void run (void);
  virtual void inform (Otask *task);
  virtual void forget (Otask *task);
  virtual bool request (Otask *task, OtaskPriority priority = DOORS_PRIORITY_NORMAL);
  
  void setIoHandler (IoHandler *ioh);
  
  void setTimerTask (TimerTask *tt);
  
  // impl Otask
  IoHandler *getIoHandler (void) const;
  TimerTask *getTimerTask (void) const;
  
  // whether there is domething for scheduler to do or not

  bool isBusy (void);

  enum State { beforeTrace, afterTrace };
  bool beforeRun (void) { return traceState == beforeTrace ? true : false; }

protected:
  State traceState;
  IoHandler *ioHandler;
  TimerTask *timerTask;
  
  OtaskList otask[DOORS_NO_PRIORITIES];
  
  Otask *current;
  bool finishedList;
  OtaskPriority priority;
  OtaskPriority highestOtaskRequested;

};



#endif

