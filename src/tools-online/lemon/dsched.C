//Editor-Info: -*- C++ -*-
//
// 

#include <doors/dsched.h>
#include <doors/ttask.h>
#include <doors/hsi.h>

//
//Constructor: DebugScheduler
//
//Parameters: 
//      IoHandler *, TimerTask *, Scheduler *
//
//Description: 
//      The constructor initializes variables
//      all debug schedulers have the name "Debugging Scheduler"
//
DebugScheduler :: DebugScheduler (IoHandler *io, TimerTask *tt, Scheduler *s) 
: Scheduler (io, "Debugging Scheduler", tt, s), traceState (beforeTrace), 
  stopped (false), count (0)

{
  STATIC_SYMBOL (stopped, this);
  STATIC_SYMBOL (count, this);
  
  DebugScheduler &sched = *this;
  STATIC_SYMBOL (sched, 0);
}


DebugScheduler :: ~DebugScheduler (void) {

  DELETE_SYMBOL (&stopped);
  DELETE_SYMBOL (&count);
  DELETE_SYMBOL (this);
  
}


//
//Function: run
//
//Member-Of: DebugScheduler
//
//Description: 
//      This run function is copied from Scheduler, and modified
//      to handle stopping and continuing of execution.
//      traceBefore and traceAfter functions are called before and
//      and after execution to display tracing if wanted.
//
void DebugScheduler :: run (void)
{
  if (!stopped) {
    // the lower number the higher priority
    while (priority < DOORS_NO_PRIORITIES) {
      current = otask[priority].remove ();
      if (current) {
	traceState = beforeTrace;
	TRACE_TASK (current);
	current->run ();
	traceState = afterTrace;
	TRACE_TASK (current);
	
	// if the load of the task is not zero, it has 
	// more work to do. So, it must be kept 
	// in the runnable tasks' list. 
	// Note that the task may remove itself and no further
	// execution is needed.
	if (current && current->getLoad ()) {
	  otask[priority].append (current, NotDestroyed);
	} else {
	  // we can forget this task, and we have
	  // one job less 
	  decrementLoad ();
	}
      } else {
	++priority;
      }
    }
  }
}

//
//Function: request
//
//Member-Of: DebugScheduler
//
//Parameters: 
//      Otask *, OtaskPriority
// 
//Description: 
//      request is copied from Scheduler, with some modifications
//
bool DebugScheduler :: request (Otask *task, OtaskPriority prt)
{
    bool rc = true;

    if (task == current) {
	return false;
    }

    if (stopped) {
        count++;

	if (prt > DOORS_PRIORITY_LOWEST) {
	    prt = DOORS_PRIORITY_NORMAL;
	}

	if (prt < priority) {
	    priority = prt;
	    finishedList = false; 
	}
	otask[priority].append (task, NotDestroyed);

    } else {
        rc = Scheduler::request (task, prt);
    }
    return rc;
}


//
//Function: start
//
//Member-Of: DebugScheduler
//
//Description: 
//      Resets stop-flag and requests time from upper level scheduler, 
//      if needed.
//
void DebugScheduler :: start (void)
{
    stopped = false;
    setLoad (count);
    if (scheduler && count) {
	scheduler->request (this, DOORS_PRIORITY_NORMAL);
    }
    if (getTimerTask ()) {
	getTimerTask ()->start();
    }
    return;
}


//
//Function: stop
//
//Member-Of: DebugScheduler
//
//Description: 
//      Sets stop flag and clears load
//
void DebugScheduler :: stop (void)
{
    stopped = true;
    count = getLoad ();
    setLoad (0);
    if (getTimerTask ()) {
	getTimerTask ()->stop();
    }
    return;
}


//
//Function: step
//
//Member-Of: DebugScheduler
//
//Description: 
//      executes one task by calling Scheduler run -function
//
void DebugScheduler :: step (void)
{
  // the lower number the higher priority
  while (priority < DOORS_NO_PRIORITIES) {
    current = otask[priority].remove ();
    if (current) {
      if (getTimerTask ()) {
	getTimerTask ()->start();
	traceState = beforeTrace;
	TRACE_TASK (current);
	current->run ();
	traceState = afterTrace;
	TRACE_TASK (current);
	getTimerTask ()->stop();
      } else {
	current->run ();
      }
      
      // if the load of the task is not zero, it has 
      // more work to do. So, it must be kept 
      // in the runnable tasks' list. 
      // Note that the task may remove itself and no further
      // execution is needed.


      if (current && current->getLoad ()) {
	otask[priority].append (current, NotDestroyed);
      } else {
	// we can forget this task, and we have
	// one job less 
	decrementLoad ();
      }
    } else {
      ++priority;
    }
    
    --count;
    return;
  }
}

