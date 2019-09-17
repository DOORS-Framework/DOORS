//Editor-Info: -*- C++ -*-
//


#ifndef DSCHED_H
#define DSCHED_H

#include <doors/osched.h>


//
//Class: DebugScheduler
//
//Inherited: Scheduler
//
//Description: 
//      Class is derived from normal scheduler. As an addition, a possibility
//      to stop and continue execution is added. When the scheduler is 
//      stopped, count of requests is storen into a local variable. 
//      In continuation of execution, upper level scheduler is informed
//      if there are requests in queue. 
//      Typically a development version of user system consists of a main
//      scheduler, which handles all system like TimerTask and such,
//      and under it, there is a DebugScheduler, which handles user's 
//      tasks.
//
class DebugScheduler : public Scheduler {
  public:
    DebugScheduler (IoHandler *io = 0, TimerTask *tt = 0, Scheduler * = 0);
    virtual ~DebugScheduler (void);

    enum State { beforeTrace, afterTrace };

    virtual void run (void);
    virtual bool request (Otask *task, OtaskPriority priority);
    
    void start (void);
    void stop (void);
    void step (void);

    bool beforeRun (void) { return traceState == beforeTrace ? true : false; }
    
  protected:
    State traceState;
    bool stopped;
    Load count;
};

#endif  // DSCHED_H

