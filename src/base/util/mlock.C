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

$Log: mlock.C,v $
Revision 1.3  2002/07/23 14:25:23  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:55  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:22  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:17  bilhanan
Full licence clause added into doors 0.1


*/

#include <doors/mlock.h>
#include <doors/otask.h>


//
//Constructor: MutexLock
//
//Description:
//    Initialize lock.

MutexLock :: MutexLock (void) : owner (0), locked (false) {

  if (mutex_init (&mutex, USYNC_THREAD, 0) != 0)
    printf ("mutex error\n");
}

//
//Destructor: MutexLock
//
//Description:
//    Just to be.

MutexLock :: ~MutexLock (void) {

}

//
//Function: lock
//
//Member-Of: MutexLock
// 
//Description:
//    Unconditionally acquire lock. 
// 

void MutexLock :: lock (void) {

  mutex_lock (&mutex);
  locked = true;
  owner = OThread :: self();
}

//
//Function: trylock
//
//Member-Of: MutexLock
//
//Description:
//    Acquire lock if it is available. 
// 

int MutexLock :: tryLock (void) {

  int ret = 0;
  if (!(ret = mutex_trylock (&mutex))) {
    locked = true;
    owner = OThread :: self();
  }
  return ret;
}

//
//Function: unlock
//
//Member-Of: MutexLock
//
//Description:
//    Release lock.
// 

void MutexLock :: unlock (void) {

  locked = false;
  owner = 0;
  mutex_unlock (&mutex);
}

//
//Function: destroy
//
//Member-Of: MutexLock
//
//Description:
//    destroy lock.
// 

void MutexLock :: destroy (void) {

  mutex_lock (&mutex);
  locked = false;
  owner = -1;
  mutex_destroy (&mutex);
}

//
//Function: isLocked
//
//Member-Of: MutexLock
//
//Description:
//    return the status of lock.
// 

bool MutexLock :: isLocked (void) {

  return locked;
}

//
//Constructor: CondLock
//
//Description:
//    Initialize lock.

CondLock :: CondLock (void) : _cond (false) {
  cond_init (&condition, USYNC_THREAD, 0);
  reset();
}

//
//Destructor: CondLock
//
//Description:
//    Just to be.

CondLock :: ~CondLock (void) {

}

//
//Function: wait
//
//Member-Of: CondLock
//
//Description:
//    Block until condition is true.
// 

void CondLock :: wait (void) {

  mutex_lock (&mutex);

  while (!_cond) {
    cond_wait (&condition, &mutex);
  }
  reset();
  mutex_unlock (&mutex);
} 

//
//Function: timedWait
//
//Member-Of: CondLock
//
//Parameter:
//    OTime *
//
//Description:
//    Block until condition is true or user defined time.
//    is elapsed.

void CondLock :: timedWait (OTime *time) {

  if (time) {
    timestruc_t to;
    mutex_lock (&mutex);
    
    to.tv_sec = time->getSeconds ();
    to.tv_nsec = (time->getMicros () * 1000);
    
    while (!_cond) {
      int err =  cond_timedwait (&condition, &mutex, &to);
      if (err == ETIME)
	break;
    }
    reset();
    mutex_unlock (&mutex);
  }
} 

//
//Function: signal
//
//Member-Of: CondLock
//
//Description:
//    Set condition true and signal for one blocking
//    condition lock.

void CondLock :: signal (void) {

  mutex_lock(&mutex);
  cond ();
  cond_signal (&condition);
  mutex_unlock(&mutex);
}

//
//Function: broadcast
//
//Member-Of: CondLock
//
//Description:
//    Set condition true and signal for all blocking
//    condition locks.

void CondLock :: broadcast (void) {

  mutex_lock(&mutex);
  cond ();
  cond_broadcast (&condition);
  mutex_unlock(&mutex);
}

//
//Function: destroy
//
//Member-Of: CondLock
//
//Description:
//    Destroy condition lock.

void CondLock :: destroy (void) {

  mutex_lock(&mutex);
  reset();
  mutex_unlock(&mutex);
  cond_destroy (&condition);
}

//
//Function: reset
//
//Member-Of: CondLock
//
//Description:
//    Reset condition lock.
//

void CondLock :: reset (void) {

  _cond = false;
}

//
//Function: cond
//
//Member-Of: CondLock
//
//Description:
//    Set condition true.
//

bool CondLock :: cond (void) {
  
  _cond = true;
  return _cond;
}


//
//Constructor: SemaLock
//
//Description:
//    Initialize lock.

SemaLock :: SemaLock (unsigned int c) 
{
  sema_init (&sema, c, USYNC_THREAD, 0);
}

//
//Constructor: SemaLock
//
//Description:
//    Initialize lock.
//
SemaLock :: ~SemaLock (void) 
{

}

//
//Function: wait
//
//Member-Of: SemaLock
//
//Description:
//    Unconditionally acquire lock.
//

int SemaLock :: wait (void) 
{
  return sema_wait (&sema);
}

//
//Function: tryWait
//
//Member-Of: SemaLock
//
//Description:
//    Acquire lock if it is available.
//

int SemaLock :: tryWait (void) 
{
  return sema_trywait (&sema);
}

//
//Function: post
//
//Member-Of: SemaLock
//
//Description:
//    Signal for availability of lock.
//

int SemaLock :: post (void) 
{
  return sema_post (&sema);
}

//
//Function: destroy
//
//Member-Of: SemaLock
//
//Description:
//    Destroy lock.
//

int SemaLock :: destroy (void) 
{
  return sema_destroy (&sema);
}





