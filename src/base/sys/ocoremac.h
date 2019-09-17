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

$Log: ocoremac.h,v $
Revision 1.4  2002/08/14 09:31:41  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 14:25:22  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:54  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:48  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:19  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef OCOREMAC_H
#define OCOREMAC_H

#include <doors/doorsconf.h>

#if defined(__SVR4)
#include <poll.h>
#endif

#if _MT_ENABLED

#include <doors/othread.h>
#include <doors/mlock.h>
#include <doors/monitor.h>

//
//Macro: _MT_CONDLOCK(name) 
//
//Description:
//   Condition synchronization variable. It can
//   be used for example in tasks to synchronize
//   execution.
//
//   This macro instantiates a condition lock
//   named by user.
// 
#define _MT_CONDLOCK(name) \
\
   CondLock name; \
\

//
//Macro: _MT_WAIT_CONDLOCK(name) 
//
//Description:
//   Wait for condition to be true for synchronization 
//   purposes.
//
//   This macro blocks user defined condition lock until 
//   the condition is true
//
#define _MT_WAIT_CONDLOCK(name) \
\
   (name).wait(); \
\

//
//Macro: _MT_SIGNAL_CONDLOCK(name) 
//
//Description:
//   Set condition true and signal for one of blocking threads
//   to proceed.
//
//   This macro releases user defined condition lock and
//   allows execution to proceed in some waiting thread.
//
#define _MT_SIGNAL_CONDLOCK(name) \
\
   (name).signal(); \
\

//
//Macro: _MT_RUNLOCK 
//
//Description:
//   Condition lock used by system to control execution of
//   active objects. In every task and device there is one 
//   condition lock that is used for controlling event based
//   execution.
//   
//   This macro is placed in Otask and Device classes.
// 
#define _MT_RUNLOCK \
\
   CondLock _DOORS_runLock; \
\

//
//Macro: _MT_WAIT_RUNLOCK 
//
//Description:
//   Condition lock used by system to block execution active 
//   objects when there are no available events.
// 
//   This macro is placed in EventTask run function and is
//   used for blocking task exection. The signal that unblocks
//   lock is placed in save function of the task. This means
//   that when message arrive, active tasks start to exectute
//   it.
//
#define _MT_WAIT_RUNLOCK \
\
   _DOORS_runLock.wait(); \
\

//
//Macro: _MT_SIGNAL_RUNLOCK 
//
//Description:
//   Condition lock used by system to activate blocking
//   objects.
//
//   This macro is placed in task save function and
//   it is used for unblocking task execution when new
//   message is to be handled.
//
#define _MT_SIGNAL_RUNLOCK \
\
   _DOORS_runLock.signal(); \
\

//
//Macro: _MT_ASYNCMONITOR 
//
//Description:
//   Monitor for protecting member functions of the
//   tasks. This monitor is required when function
//   uses the task variables needed for asynchronous
//   message passing.
//
//   This macro must be placed in every function that
//   uses variables guarded by tasks asynclock. These 
//   functions are for example task save, ports putmessage
//   and so on.
//
#define _MT_ASYNCMONITOR \
\
   FuncMonitor _DOORS_asyncMonitor (this->getAsyncLock()); \
\

//
//Macro: _MT_SYNCMONITOR  
//
//Description:
//   Monitor for protecting member functions of the active
//   objects. This monitor is required when function
//   uses any of task variables except those needed for 
//   asynchronous message passing.
// 
//   In practise this macro is placed almost in every 
//   member function of objects that needs protection.
//   Only functions that need not this macro are those
//   system functions needed for message passing.
// 
#define _MT_SYNCMONITOR \
\
   FuncMonitor _DOORS_syncMonitor (this->getSyncLock()); \
\

//
//Macro: _MT_STATIC_SMONITOR(name) 
//
//Description:
//   Monitor for protecting static member functions of the active
//   objects. It acquires the same lock as macro _MT_SYNCMONITOR 
//
#define _MT_STATIC_SMONITOR(name) \
\
   static FuncMonitor _DOORS_syncMonitor (&(name)); \
\

//
//Macro: _MT_DATAMONITOR 
//
//Description:
//   Monitor for protecting particular data of shared
//   resources. This monitor does not use scoped locking
//   so user must implicitily release lock.
//
//   This macro is not currently used anywhere in DOORS code,
//   but it can be used by user to protect some shared
//   variables.
//
#define _MT_DATAMONITOR \
\
   DataMonitor _DOORS_dataMonitor; \
\

//
//Macro: _MT_ACQUIRE_ASYNCMONITOR
//
//Description:
//   Implicitely acquire async-monitor.
// 
#define _MT_ACQUIRE_ASYNCMONITOR \
\
   _DOORS_asyncMonitor.acquire (); \
\

//
//Macro: _MT_RELEASE_ASYNCMONITOR 
//
//Description:
//   Implicitely release async-monitor.
// 
#define _MT_RELEASE_ASYNCMONITOR \
\
   _DOORS_asyncMonitor.release (); \
\

//
//Macro: _MT_ACQUIRE_SYNCMONITOR 
//
//Description:
//   Implicitely acquire sync-monitor. 
// 
#define _MT_ACQUIRE_SYNCMONITOR \
\
   _DOORS_syncMonitor.acquire (); \
\

//
//Macro: _MT_RELEASE_SYNCMONITOR 
//
//Description:
//   Implicitely release sync-monitor. 
// 
#define _MT_RELEASE_SYNCMONITOR \
\
   _DOORS_syncMonitor.release (); \
\

//
//Macro: _MT_ACQUIRE_DATAMONITOR 
//
//Description:
//   Implicitely acquire data monitor. 
// 
#define _MT_ACQUIRE_DATAMONITOR \
\
   _DOORS_dataMonitor.acquire (); \
\

//
//Macro: _MT_RELEASE_DATAMONITOR 
//
//Description:
//   Implicitely release data monitor.  
// 
#define _MT_RELEASE_DATAMONITOR \
\
   _DOORS_dataMonitor.release (); \
\

//
//Macro: _MT_MONITOR(name) 
//
//Description:
//   Macro for creating user defined macro. Generally
//   this requires also a user defined lock that is
//   used with monitor.
//
#define _MT_MONITOR(name) \
\
   FuncMonitor _DOORS_##name ((name)); \
\

//
//Macro: _MT_FORWARDLOCK_ASMONITOR(name,task) 
//
//Description:
//   Macro for acquiring async-lock of some other
//   task.
//
//   This macro is used for example in member functions
//   of Port. This is because Tthe async-lock of tasks are also 
//   used for protecting ports of that task.
//
#define _MT_FORWARDLOCK_ASMONITOR(name,task) \
\
   FuncMonitor (name) (task ? (task)->getAsyncLock() : 0); \
\

//
//Macro: _MT_FORWARDLOCK_SMONITOR(name,task) 
//
//Description:
//   Macro for acquiring sync-lock of some other
//   task or some other shared resource.
// 
//   This macro is not currently used anywhere in DOORS.
//   It can be used by user to synchronize access of shared 
//   resources.
//
#define _MT_FORWARDLOCK_SMONITOR(name,task) \
\
   FuncMonitor (name) (task ? (task)->getSyncLock() : 0); \
\

//
//Macro: _MT_ACQUIRE_FL_MONITOR(name) 
//
//Description:
//   Implicitely acquire user defined monitor.  
// 
#define _MT_ACQUIRE_FL_MONITOR(name) \
\
   (name).acquire (); \
\

//
//Macro: _MT_RELEASE_FL_MONITOR(name) 
//
//Description:
//   Implicitely release user defined monitor.   
// 
//
#define _MT_RELEASE_FL_MONITOR(name) \
\
   (name).release (); \
\

//
//Macro: _MT_SYNCLOCK 
//
//Description:
//   Synchronization variable used by system to protect
//   task attributes (variables) used for synchronous
//   communication. In every shared resource (task, device, 
//   and so on) there is one lock like this. 
//
#define _MT_SYNCLOCK \
\
   MutexLock _DOORS_syncLock; \
\

//
//Macro: _MT_POINTER_TO_SLOCK 
//
//Description:
//   Pointer to Synchronization variable used by system to protect
//   task attributes (variables) used for synchronous communication.
//   
//   This macro can be used in shared resources which are 
//   protected by sync-lock of some other instance. In practice
//   this macro is used in hsi symbols, because they are protected
//   with the lock of the resources (task, device) they point to. 
//
#define _MT_POINTER_TO_SLOCK \
\
   MutexLock *_DOORS_syncLock; \
\

//
//Macro: _MT_ASYNCLOCK 
//
//Description:
//   Synchronization variable used by system to protect
//   task attributes (variables) used for asynchronous
//   message passing. In every task there is one lock
//   like this. 
//
//   This macro is placed in class Otask.
//
#define _MT_ASYNCLOCK \
\
   MutexLock _DOORS_asyncLock; \
\

//
//Macro: _MT_LOCK(name) 
//
//Description:
//   User defined lock for any purpose.
// 
#define _MT_LOCK(name) \
\
   MutexLock (name); \
\

//
//Macro: _MT_STATICLOCK(name) 
//
//Description:
//   Static lock for protecting static member functions.
// 
//   This macro is used for example in TimerTask to
//   protect instantiation of the class.
//
#define _MT_STATICLOCK(name) \
\
   static MutexLock (name); \
\

//
//Macro: _MT_RESET_POINTER 
//
//Description:
//   Macro for resetting sync-lock pointer to zero.
// 
//
#define _MT_RESET_POINTER \
\
   _DOORS_syncLock = 0; \
\

//
//Macro: _MT_GETDATALOCK 
//
//Description:
//   Macro for adding member function that returns data lock.
// 
#define _MT_GETDATALOCK virtual MutexLock *getDataLock (void) const {\
   return (MutexLock *) _DOORS_dataMonitor.getLock(); \
};\
\

//
//Macro: _MT_GETSYNCLOCK 
//
//Description:
//   Macro for adding member function that returns sync-lock. 
//
#define _MT_GETSYNCLOCK virtual MutexLock *getSyncLock (void) const {\
   return (MutexLock *) &_DOORS_syncLock; \
};\
\

//
//Macro: _MT_GETASYNCLOCK 
//
//Description:
//   Macro for adding member function that returns async-lock.  
//
#define _MT_GETASYNCLOCK virtual MutexLock *getAsyncLock (void) const {\
   return (MutexLock *) &_DOORS_asyncLock; \
};\
\

//
//Macro: _MT_GETRUNLOCK 
//
//Description:
//   Macro for adding member function that returns run lock.  
//
#define _MT_GETRUNLOCK virtual CondLock *getRunLock (void) const {\
   return (CondLock *) &_DOORS_runLock; \
};\
\

//
//Macro: _MT_GETSLOCK_POINTER 
//
//Description:
//   Macro for adding member function that returns run lock. 
// 
#define _MT_GETSLOCK_POINTER virtual MutexLock *getSyncLock (void) const{\
   return (MutexLock *) _DOORS_syncLock; \
};\
\

//
//Macro: _MT_SETSLOCK_POINTER 
//
//Description:
//   Macro for adding member function that is used for setting 
//   run lock pointer value.  
//
#define _MT_SETSLOCK_POINTER virtual void setSyncLock (MutexLock *l) {\
   _DOORS_syncLock = l; \
};\
\

//
//Macro: _MT_GETEXITBARRIER 
//
//Description:
//   Macro for adding member function that returns exit barrier. 
//
#define _MT_GETEXITBARRIER virtual Barrier *getExitBarrier (void) const {\
   return (Barrier *) &_DOORS_exitBarrier; \
};\
\

//
//Macro: _MT_EXITBARRIER 
//
//Description:
//   Barrier used by system to control deleting order and
//   timing instances. Barrier is used for preventing conflicts
//   in hierarchical data structures when they are destroyed.
//
//   For example barrier is used for making sure that device
//   connected to certain task is destroyed before the task.
//   Barrier blocks the deleting of task until device has
//   been destroyed.
//
#define _MT_EXITBARRIER \
\
   Barrier _DOORS_exitBarrier; \
\

//
//Macro: _MT_INC_EXITBARRIER 
//
//Description:
//   Increment exit barrier. This would mean that owner
//   of this barrier can not be deleted before counter
//   of the barrier is decremented. 
//
//   This macro is placed in beginning of active objects 
//   run function.
// 
#define _MT_INC_EXITBARRIER \
\
   _DOORS_exitBarrier.increment(); \
\

//
//Macro: _MT_DEC_EXITBARRIER 
//
//Description:
//   Decrement exit barrier.
// 
//   This macro is placed at end of active objects run function.
// 
#define _MT_DEC_EXITBARRIER \
\
   _DOORS_exitBarrier.decrement(); \
\

//
//Macro: _MT_WAIT_EXITBARRIER(name) 
//
//Description:
//   Block the execution if counter of barrier
//   is non-zero. This would mean that some
//   other instance must be destroyed before destroyin
//   the owner of this barrier.
//
#define _MT_WAIT_EXITBARRIER(name) \
\
   (name)->getExitBarrier()->wait();\
\

//
//Macro: _MT_BARRIER(name) 
//
//Description:
//   User defined barrier for any purpose.
// 
//
#define _MT_BARRIER(name) \
\
   Barrier (name); \
\

//
//Macro: _MT_INC_BARRIER(name) 
//
//Description:
//   Increment user defined barrier.
//
#define _MT_INC_BARRIER(name) \
\
   (name).increment(); \
\

//
//Macro: _MT_DEC_BARRIER(name) 
//
//Description:
//   Decrement user defined barrier.
//
#define _MT_DEC_BARRIER(name) \
\
   (name).decrement(); \
\

//
//Macro: _MT_WAIT_BARRIER(name) 
//
//Description:
//   Block user defined barrier if needed.
// 
#define _MT_WAIT_BARRIER(name) \
\
   (name).wait(); \
\

//
//Macro: _MT_EXTERN_BARRIER(name) 
//
//Description:
//   Macro for adding the visibility of user
//   defined barrier.
//
#define _MT_EXTERN_BARRIER(name) \
\
   extern Barrier (name);\
\

//
//Macro: _MT_S_OTHER_RUNLOCK(name) 
//
//Description:
//   Signal the run lock of some other active
//   object.
//
//   This macro is used in TimerDevice to
//   inform TimerTask for timout.
//
#define _MT_S_OTHER_RUNLOCK(name) \
\
   (name)->getRunLock()->signal(); \
\

//
//Macro: _MT_KILLTHREAD(name) 
//
//Description:
//   Macro for calling a active objects kill
//   member function. This will recursively
//   destroy the running threads.
//
//   The order of destroying is important and
//   it must be done top-down. In practice this
//   would mean that threads of devices must be
//   destroyed before the thread of task is destroyed. 
//
#define _MT_KILLTHREAD(name) \
\
   (name)->kill(); \
\

//
//Macro: _MT_UPDATELOCKS_DEC 
//
//Description:
//   Declaration of member function of Symbol.
// 
#define _MT_UPDATELOCKS_DEC \
\
void updateLocks (void); \
\

//
//Macro: _MT_UPDATELOCKS 
//
//Description:
//   Implemantation of member function of Symbol.
//   It is used for properly handling locking of
//   hierarchical symbol structures.
//
#define _MT_UPDATELOCKS \
\
void Symbol :: updateLocks (void) { \
\
    _MT_SYNCMONITOR; \
    SymbolChildIterator iterator (this); \
\
    Symbol *tmp = 0; \
    while ((tmp = iterator.next()) && (tmp->getSyncLock() == 0)) { \
      tmp->setSyncLock (getSyncLock()); \
      tmp->updateLocks(); \
    } \
} \

//
//Macro: _MT_UPDATE(name) 
//
//Description:
//   Macro for calling member function of Symbol. 
// 
#define _MT_UPDATE(name) \
\
  (name)->updateLocks();
\

//
//Macro: _MT_SET_LOCKPOINTER(name1, name2)
//
//Description:
//   Macro for setting sync-lock pointer of resource
//   from used defined resource.
//
#define _MT_SET_LOCKPOINTER(name1, name2) \
\
  (name1)->setSyncLock((name2)->getSyncLock()); \
\

//
//Macro: _MT_ACTIVE_TASKRUN 
//
//Description:
//   Member function of active task that is given as a
//   parameter on thread creation function.
//
#define _MT_ACTIVE_TASKRUN inline static void *activeRun (void *t) {\
\
  Otask *task = (Otask *) t; \
  if (!task->getLoad()) \
    task->_DOORS_runLock.wait(); \
  task->taskRun(); \
  return 0; \
};\
\

//
//Macro: _MT_ACTIVE_DEVICERUN 
//
//Description:
//   Member function of active device taht is given as a
//   parameter on thread creation function.
//
#define _MT_ACTIVE_DEVICERUN inline static void *activeRun (void *d) {\
\
  Device *dev = (Device *) d; \
\
  dev->deviceRun (); \
  return 0; \
} \
\

//
//Macro: _MT_TASKRUN 
// 
//Description:
//   Active tasks run function that basically replaces
//   the functionality of Scheduler. 
//
//   _DOORS_exitBarrier is used for controlling destruction
//   of task.
//
//   _DOORS_runLock is used for controlling execution of
//   incoming messages.
//  
//   alive variable is used for stating the status of
//   task. The thread of active task can be destroyed
//   by setting alive variable false.
//
#define _MT_TASKRUN inline virtual void taskRun (void) {\
\
  _MT_SYNCMONITOR; \
\
  _DOORS_exitBarrier.increment(); \
  while (isAlive()) { \
    run(); \
    _MT_RELEASE_SYNCMONITOR; \
    if (!getLoad()) { \
      _DOORS_runLock.wait(); \
    } else {\
      OThread :: yield ();\
    } \
    _MT_ACQUIRE_SYNCMONITOR; \
  } \
  _DOORS_exitBarrier.decrement(); \
}\
\

//
//Macro: _MT_DEVICERUN_NULL 
//
//Description:
//   Macro needed for creating abstract base class.
//
#define _MT_DEVICERUN_NULL inline virtual void deviceRun (void) = 0;

//
//Macro: _MT_DEVICERUN 
// 
//Description:
//   Run function of active device that can be compared to
//   tasks activeRun function. It has the same basic 
//   functionality.
//
#define _MT_DEVICERUN inline virtual void deviceRun (void) {\
\
  _MT_SYNCMONITOR; \
\
  if (task) \
    task->getExitBarrier()->increment(); \
\
  _DOORS_exitBarrier.increment(); \
  while (isAlive()) { \
    pollDevice (); \
    _MT_RELEASE_SYNCMONITOR; \
    OThread :: yield ();\
    _MT_ACQUIRE_SYNCMONITOR; \
  } \
  _DOORS_exitBarrier.decrement(); \
\
  if (task) \
    task->getExitBarrier()->decrement(); \
}\
\

//
//Macro: _MT_ISALIVE 
//
//Description:
//   Macro for adding member function to active objects.
//
#define _MT_ISALIVE inline virtual bool isAlive (void) {\
\
  _MT_SYNCMONITOR; \
  return alive; \
}\
\

//
//Macro: _MT_KILL 
//
//Description:
//   Macro for adding member function to active objects. 
//
//   This macro is used in active object to kill
//   the thread of control. It is virtual function
//   and can be implemented again in inherited classes.
//
#define _MT_KILL inline virtual void kill (void) {\
\
  _MT_SYNCMONITOR; \
  alive = false; \
  _DOORS_runLock.signal(); \
}\
\

//
//Macro: _MT_KILLALL(name) 
//
//Description:
//   Macro for adding member function to active objects. 
// 
//   This macro is used for defining new kill function
//   for tasks that hold one other active object (like device).
//   This function enables recursive kill.
//
#define _MT_KILLALL(name) inline virtual void kill (void) {\
\
  _MT_SYNCMONITOR; \
  (name)->kill(); \
  (name)->getExitBarrier()->wait(); \
  alive = false; \
  _DOORS_runLock.signal(); \
}\
\

//
//Macro: _MT_KILLALL2(name1,name2) 
//
//Description:
//   Macro for adding member function to active objects. 
// 
//   This macro is used for defining new kill function
//   for tasks that hold two other active objects (like device).
//   This function enables recursive kill.
//
#define _MT_KILLALL2(name1,name2) inline virtual void kill (void) {\
\
  _MT_SYNCMONITOR; \
  (name1)->kill(); \
  (name1)->getExitBarrier()->wait(); \
  (name2)->kill(); \
  (name2)->getExitBarrier()->wait(); \
  alive = false; \
  _DOORS_runLock.signal(); \
}\
\

//
//Macro:_MT_KILLALL3(name1,name2,name3) 
//
//Description:
//   Macro for adding member function to active objects. 
// 
//   This macro is used for defining new kill function
//   for tasks that hold tree other active objects (like device).
//   This function enables recursive kill.
//
#define _MT_KILLALL3(name1,name2,name3) inline virtual void kill (void) {\
\
  _MT_SYNCMONITOR; \
  (name1)->kill(); \
  (name1)->getExitBarrier()->wait(); \
  (name2)->kill(); \
  (name2)->getExitBarrier()->wait(); \
  (name3)->kill(); \
  (name3)->getExitBarrier()->wait(); \
  alive = false; \
  _DOORS_runLock.signal(); \
}\
\

//
//Macro: _MT_KILLALL_DEC 
//
//Description:
//   Declaration of member function.
//
#define _MT_KILLALL_DEC virtual void kill (void); \
\

//
//Macro: _MT_KILLALL_TTIMPL(name) 
//
//Description:
//   Implementation of kill function for TimerTask.
//
#define _MT_KILLALL_TTIMPL(name) void TimerTask :: kill (void) {\
\
  _MT_SYNCMONITOR; \
  (name)->kill(); \
  (name)->getExitBarrier()->wait(); \
  alive = false; \
  _DOORS_runLock.signal(); \
}\
\

//
//Macro: _MT_TASKSCHEDULE
//
//Description:
//   Macro for adding member function schedule for tasks
//
#define _MT_TASKSCHEDULE inline virtual void schedule (void) {\
\
  _MT_SYNCMONITOR; \
  int ret = OThread :: create (0, 0, Otask :: activeRun, this, THR_DETACHED, 0); \
  if (ret == 0) \
    alive = true; \
  else { \
    printf ("Thread not created: %s\n", strerror(ret)); \
  } \
} \
\

//
//Macro: _MT_DEVICESCHEDULE 
//
//Description:
//   Macro for adding member function schedule for device
//
#define _MT_DEVICESCHEDULE inline virtual void schedule (void) {\
\
  _MT_SYNCMONITOR; \
  alive = true; \
  OThread :: create (0, 0, Device :: activeRun, this, THR_DETACHED, 0); \
} \
\

//
//Macro: _MT_SCHEDULE(name) 
//
//Description:
//   Macro for calling a shedule function for active objects.
//   
//   This macro is used for creating a new thread of control
//   for active objects. It must be placed after a instantation
//   of object in order to make it run.
//
#define _MT_SCHEDULE(name) \
\
  (name)->schedule(); \
\

//
//Macro: _MT_POLLDEVICE
//
//Description:
//   Macro for adding member function pollDevice fo device.
// 
//   This function distributes the functionality of IoHandler
//   into the devices so that they no longer need controlling 
//   objects like IoHandler. Each active device is responsible
//   for its own file descriptor polling.
//
#define _MT_POLLDEVICE inline virtual void pollDevice (void) {\
\
  _MT_SYNCMONITOR; \
  int nfds = 0; \
\
  if(getFd() >= 0) { \
    if (getStatus() & Device::READ) { \
       pollfds[0].fd = getFd(); \
       pollfds[0].events = POLLIN; \
       pollfds[0].revents = 0; \
       nfds = 0; \
\
       _MT_RELEASE_SYNCMONITOR; \
       nfds = poll (pollfds, 1, 0); \
\
       if (nfds < 0) \
         printf ("poll failed, %s\n", strerror (errno)); \
\
       _MT_ACQUIRE_SYNCMONITOR; \
    } \
  } \
\
  if (nfds > 0) { \
    if (pollfds[0].revents == POLLIN) { \
      _MT_RELEASE_SYNCMONITOR; \
      _MT_STABLE_MONITOR; \
      _MT_ACQUIRE_SYNCMONITOR; \
      callbackRead (); \
    } \
  } \
\
  if (getStatus() & Device::WRITE) { \
    _MT_RELEASE_SYNCMONITOR; \
    _MT_STABLE_MONITOR; \
    _MT_ACQUIRE_SYNCMONITOR; \
    callbackWrite (); \
  } \
\
  if (getStatus() & Device::EXCEPTION) { \
    _MT_RELEASE_SYNCMONITOR; \
    _MT_STABLE_MONITOR; \
    _MT_ACQUIRE_SYNCMONITOR; \
    callbackException (); \
  } \
\
  if (getStatus() & Device::TIMEOUT) { \
    _MT_RELEASE_SYNCMONITOR; \
    _MT_STABLE_MONITOR; \
    _MT_ACQUIRE_SYNCMONITOR; \
    callbackTimeout (); \
  } \
} \
\

//
//Macro: _MT_POLLTIMERDEVICE 
//
//Description:
//   Macro for adding member function pollDevice fo TimerDevice. 
// 
//   TimerDevice does not use file descriptor based communication
//   so there is no need to do polling. The activation of TimerDevice
//   is handled with condition variables.
//
#define _MT_POLLTIMERDEVICE inline virtual void pollDevice (void) {\
\
  _MT_SYNCMONITOR; \
\
  if (timeout == 0) { \
    _MT_RELEASE_SYNCMONITOR; \
    _DOORS_runLock.wait (); \
    _MT_ACQUIRE_SYNCMONITOR; \
  } \
  else { \
      _MT_RELEASE_SYNCMONITOR; \
        _DOORS_runLock.timedWait (&timeout); \
      _MT_ACQUIRE_SYNCMONITOR; \
      callbackTimeout (); \
  } \
} \
\

//
//Macro: _MT_DEVICE_VARIABLES 
//
//Description:
//   Macro for adding few variables to device.
//
#define _MT_DEVICE_VARIABLES \
\
   OTime *blockTime; \
   struct pollfd pollfds[1]; \
\

//
//Macro: _MT_RESET_DEVICE_VARIABLES 
//
//Description:
//   Macro for reseting added variables of device. 
// 
//
#define _MT_RESET_DEVICE_VARIABLES \
\
  blockTime = 0; \
\

//
//Macro: _MT_RESET_BLOCKTIME(time) 
//
//Description:
//   Macro for resetting blockTime of active device.
//
#define _MT_RESET_BLOCKTIME(time) \
\
  blockTime = new OTime (time); \
\

//
//Macro: _MT_ALIVE
//
//Description:
//   Macro for adding alive variable to active objects.
//
#define _MT_ALIVE \
\
   bool alive; \
\

//
//Macro: _MT_RESET_ALIVE
//
//Description:
//   Macro for resetting alive variable.
//
#define _MT_RESET_ALIVE \
\
   alive = false; \
\

//
//Macro: _MT_RUN(name) 
//
//Description:
//   In multithreded version this implementation is null
//   It is used in sequential execution in TimerDevice
//   to execute run function of TimerTask.
//
#define _MT_RUN(name) 

#else

#define _MT_CONDLOCK(name)
#define _MT_WAIT_CONDLOCK(name)
#define _MT_SIGNAL_CONDLOCK(name)
#define _MT_RUNLOCK
#define _MT_WAIT_RUNLOCK
#define _MT_SIGNAL_RUNLOCK
#define _MT_ASYNCMONITOR
#define _MT_SYNCMONITOR
#define _MT_STATIC_SMONITOR(name)
#define _MT_DATAMONITOR
#define _MT_ACQUIRE_ASYNCMONITOR
#define _MT_RELEASE_ASYNCMONITOR
#define _MT_ACQUIRE_SYNCMONITOR
#define _MT_RELEASE_SYNCMONITOR
#define _MT_ACQUIRE_DATAMONITOR
#define _MT_RELEASE_DATAMONITOR
#define _MT_FORWARDLOCK_ASMONITOR(name,task)
#define _MT_FORWARDLOCK_SMONITOR(name,task)
#define _MT_ACQUIRE_FL_MONITOR(name)
#define _MT_RELEASE_FL_MONITOR(name)
#define _MT_ACTIVE_TASKRUN
#define _MT_ACTIVE_DEVICERUN
#define _MT_TASKRUN
#define _MT_DEVICERUN_NULL
#define _MT_DEVICERUN
#define _MT_ISALIVE
#define _MT_KILL
#define _MT_KILLALL(name)
#define _MT_KILLALL2(name1,name2)
#define _MT_KILLALL3(name1,name2,name3)
#define _MT_KILLALL_DEC
#define _MT_KILLALL_TTIMPL(name)
#define _MT_TASKSCHEDULE
#define _MT_DEVICESCHEDULE
#define _MT_SCHEDULE(name)
#define _MT_POLLDEVICE
#define _MT_POLLTIMERDEVICE
#define _MT_DEVICE_VARIABLES
#define _MT_RESET_DEVICE_VARIABLES
#define _MT_RESET_BLOCKTIME(time)
#define _MT_ALIVE
#define _MT_RESET_ALIVE
#define _MT_SYNCLOCK
#define _MT_ASYNCLOCK
#define _MT_LOCK(name)
#define _MT_STATICLOCK(name)
#define _MT_GETDATALOCK
#define _MT_GETSYNCLOCK
#define _MT_GETASYNCLOCK
#define _MT_GETRUNLOCK
#define _MT_GETSLOCK_POINTER
#define _MT_SETSLOCK_POINTER
#define _MT_POINTER_TO_SLOCK
#define _MT_RESET_POINTER
#define _MT_GETEXITBARRIER
#define _MT_EXITBARRIER
#define _MT_INC_EXITBARRIER
#define _MT_DEC_EXITBARRIER
#define _MT_WAIT_EXITBARRIER(name)
#define _MT_S_OTHER_RUNLOCK(name)
#define _MT_KILLTHREAD(name)
#define _MT_MONITOR(name)
#define _MT_ACQUIRE_FL_MONITOR(name)
#define _MT_RELEASE_FL_MONITOR(name)
#define _MT_UPDATELOCKS
#define _MT_UPDATELOCKS_DEC
#define _MT_UPDATE(name)
#define _MT_SET_LOCKPOINTER(name1, name2)
#define _MT_BARRIER(name)
#define _MT_INC_BARRIER(name)
#define _MT_DEC_BARRIER(name)
#define _MT_WAIT_BARRIER(name)
#define _MT_EXTERN_BARRIER(name)
#define _MT_RUN(name) \
\
  (name)->run(); \
\


#endif

#endif
