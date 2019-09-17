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

$Log: monitor.C,v $
Revision 1.3  2002/07/23 14:25:21  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:53  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:22  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:17  bilhanan
Full licence clause added into doors 0.1


*/

#include <doors/monitor.h>
#include <doors/util.h>
#include <doors/core.h>

//
//Constructor: Monitor
//
//Description:
//    Initialize object.

Monitor :: Monitor (const MutexLock *l) : locked(false), mutex ((MutexLock *) l) {

}

//
//Destructor: Monitor
//
//Description:
//    Just to be.

Monitor :: ~Monitor (void) {

}

//
//Constructor: FuncMonitor
//
//Description:
//    Initialize monitor and lock it. Recursive deadlock
//    is prevented by lockin only when needed.
//
FuncMonitor :: FuncMonitor (const MutexLock *l): Monitor (l) {

  if (mutex && (mutex->getOwner() != OThread :: self())) {
    mutex->lock();
    locked = true;
  } 
}

//
//Destructor: FuncMonitor
//
//Description:
//    Release lock if it has been acquired by this instance.
//    This functionality enables the usage hiararchical function
//    calls.
//
FuncMonitor :: ~FuncMonitor (void) {

  if (mutex && (mutex->getOwner() == OThread :: self())) {
    if (locked == true) 
      if (mutex->isLocked ()) {
	locked = false;
	mutex->unlock();
      }
  }
}

//
//Function: release
//
//Member-Of: FuncMonitor
// 
//Description:
//    Implicitely release lock. 
// 

void FuncMonitor :: release (void) {

  if (mutex)
    mutex->unlock();
}

//
//Function: acquire
//
//Member-Of: FuncMonitor
// 
//Description:
//    Implicitely acquire lock if needed. 
// 
void FuncMonitor :: acquire (void) {

  if (mutex && !(mutex->getOwner() == OThread :: self()))
    mutex->lock();
}

//
//Constructor: DataMonitor
//
//Description:
//    Initialize monitor and lock it. Recursive deadlock
//    is prevented by lockin only when needed.
//
DataMonitor :: DataMonitor (void): Monitor (&lock) {

}

//
//Destructor: DataMonitor
//
//Description:
//    Destroy instance, but does not release lock.
//
DataMonitor :: ~DataMonitor (void) {

  mutex = 0;
}

//
//Function: release
//
//Member-Of: DataMonitor
// 
//Description:
//    Implicitely release lock. 
// 

void DataMonitor :: release (void) {
  if (mutex)
    mutex->unlock();
}

//
//Function: acquire
//
//Member-Of: DataMonitor
// 
//Description:
//    Implicitely acquire lock is needed. 
// 

void DataMonitor :: acquire (void) {
  if (mutex && (mutex->getOwner() != OThread :: self())) {
    mutex->lock();
  }
}

//
//Function: getLock
//
//Member-Of: DataMonitor
// 
//Description:
//    Return lock
// 

const MutexLock *DataMonitor :: getLock (void) const {

  return &lock;
}

//
//Constructor: Barrier
//
//Description:
//    Initialize instance
//
Barrier :: Barrier (void): counter (0) {

  _cond = true;
}

//
//Destructor: Barrier
//
//Description:
//    Just to be.
//
Barrier :: ~Barrier (void) {

}

//
//Function: increment
//
//Member-Of: Barrier
// 
//Description:
//    Increment barrier counter. This means that wait
//    function call will block until condition is set
//    true by decrementing the counter to zero.

void Barrier :: increment (void) {

  lock();
  counter ++;
  _cond = false;
  unlock();
}

//
//Function: decrement
//
//Member-Of: Barrier
// 
//Description:
//    decrement barrier counter. This means that threads
//    that are blocking will receive signal and they must
//    inspect condition. Condition is true when counter
//    of the barrier is set zero. At that point blocking
//    threads can proceed. 

void Barrier :: decrement (void) {

  lock();
  counter --;
  unlock();
  signal();
}

//
//Function: reset
//
//Member-Of: Barrier
// 
//Description:
//    Reset condition.

void Barrier :: reset (void) {

  counter = 0;
  _cond = true;
}

//
//Function: cond
//
//Member-Of: Barrier
// 
//Description:
//    Condition implementation of barrier
//

bool Barrier :: cond (void) {

  if (counter < 1)
    _cond = true;
  return _cond;
}



