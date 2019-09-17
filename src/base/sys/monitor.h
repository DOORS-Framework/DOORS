//
// $Id: monitor.h,v 1.3 2002/07/23 14:25:22 ik Exp $
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

$Log: monitor.h,v $
Revision 1.3  2002/07/23 14:25:22  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:53  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:42  bilhanan
Full Licence clause into DOORS 0.2


*/

#ifndef MONITOR_H
#define MONITOR_H

#include <doors/mlock.h>
class Otask;

/** Monitor is abstract base class for different types      
  of more advanced synchronization variables.
  */

class Monitor {
 public:
  virtual ~Monitor (void);

  virtual void release (void) = 0;
  virtual void acquire (void) = 0;

 protected:
  /** Initialize object.*/
  Monitor (const MutexLock *lock);

  MutexLock *mutex;
  bool locked;
};

/** FuncMonitor is the base of DOORS resourse synchronization. 
  It is used in every protectable object in its every
  public memberfunction.
  */
class FuncMonitor : public Monitor {
 public:
  /** Initialize monitor and lock it. Recursive deadlock
    is prevented by lockin only when needed. */
  FuncMonitor (const MutexLock *lock);

  /** Release lock if it has been acquired by this instance.
    This functionality enables the usage hiararchical function
    calls. */
  ~FuncMonitor (void);

  /** Implicitely release lock */
  void release (void);
  /** Implicitely acquire lock if needed. */
  void acquire (void);
};


/** DataMonitor is a synchronization variable for 
  protecting shared data for longer periods of
  time. It does not automatically unlock itself
  when leaving its scope. It is not used at this
  time. */

class DataMonitor : public Monitor {
 public:
  /** Initialize monitor and lock it. Recursive deadlock
   is prevented by lockin only when needed. */
  DataMonitor (void);
  /** Destroy instance, but does not release lock. */
  ~DataMonitor (void);

  /**Implicitely release lock. */
  void release (void);

  /**Implicitely acquire lock is needed. */
  void acquire (void);

  /**@return lock */
  const MutexLock *getLock (void) const;
 protected:
  MutexLock lock;
};

/** Barrier is more advanced method for blocking execution.
  It is generally used for stopping a thread until all
  needed tasks for continuing execution are done. 
  */
class Barrier: public CondLock {
 public:
  /** Initialize instance */
  Barrier (void);
  
  /** destructor */
  virtual ~Barrier (void);
  
  /**Increment barrier counter. This means that wait
    function call will block until condition is set
    true by decrementing the counter to zero.
   */ 
  virtual void increment (void);

  /** decrement barrier counter. This means that threads
    that are blocking will receive signal and they must
    inspect condition. Condition is true when counter
    of the barrier is set zero. At that point blocking
    threads can proceed. */
  virtual void decrement (void);

 protected:
  /** Reset condition. */
  virtual void reset (void);

  /** Condition implementation of barrier */
  virtual bool cond (void);

  int counter;
};

#endif


