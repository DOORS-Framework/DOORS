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

$Log: mlock.h,v $
Revision 1.3  2002/07/23 14:25:23  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:55  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:42  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:17  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef MLOCK_H
#define MLOCK_H

#include <doors/util.h>
#include <thread.h>

/** MutexLock class is used for wrapping C thread library
      mutex_t into it. Now locking can be done in more C++ manner.
      Underlying Solaris implementation can also be changed
      into Posix implementation without changing the interface 
      of the class. 
      */
class MutexLock {
 public:
  /** Initialize lock. */
  MutexLock (void);

  /** Destructor */
  ~MutexLock (void);

  /**    Unconditionally acquire lock */
  void lock (void);

  /** Release lock. */
  void unlock (void);

  /** Acquire lock if it is available. */
  int tryLock (void);

  /** destroy lock.*/
  void destroy (void);
  /** @return the status of lock. */
  bool isLocked (void);

  /** @return owner of lock */
  int getOwner (void) {return owner;};

 protected:
  thread_t owner;
  bool locked;
  mutex_t mutex;
};


/** CondLock class is used for wrapping C thread library
  cond_t into it. Now locking can be done in more C++ manner.
  Underlying Solaris implementation can also be changed
  into Posix implementation without changing the interface 
  of the class. 
  */
class CondLock : public MutexLock {
 public:
  /** Initialize lock*/
  CondLock (void);

  /** Destructor */
  virtual ~CondLock (void);
  /**Block until condition is true. */
  void wait (void);
  /**  Block until condition is true or user defined time
    is elapsed 
    @param to the time for waiting*/
  void timedWait (OTime *to);

  /** Set condition true and signal for one blocking
    condition lock. */
  void signal (void);

  /** Set condition true and signal for all blocking
    condition locks. */
  void broadcast (void);

  /**   Destroy condition lock. */
  void destroy (void);

 protected:
  /**  Reset condition lock. */
  virtual void reset (void);

  /** Set condition true. */
  virtual bool cond (void);

  cond_t condition;
  bool _cond;
};

/**      SemaLock class is used for wrapping C thread library
  sema_t into it. Now locking can be done in more C++ manner.
  Underlying Solaris implementation can also be changed
  into Posix implementation without changing the interface 
  of the class. */ 
  
class SemaLock {
 public:
  /**    Initialize lock. */
  SemaLock (unsigned int count);

  /** destructor */
  ~SemaLock (void);

  /**  Unconditionally acquire lock. */
  int wait (void);
  /**  Acquire lock if it is available. */
  int tryWait (void);
  /**   Signal for availability of lock.*/
  int post (void);
  /**  Destroy lock. */
  int destroy (void);

 protected:
  sema_t sema;
};

#endif








