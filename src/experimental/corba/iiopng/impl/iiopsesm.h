

//
//File: iiopsesm.h
//
//Date: Mon Feb 11 18:17:22 2002
//
//Commentary:
//  This is a machine generated file produced by 
// compiler. Editing is not recommended.
//
//Description:
//  This file declares a state machine class for
//  IIOPServerTask task. 
//


#ifndef _iiopsesm_h_
#define _iiopsesm_h_


#include "ptb.h"


//
//Class: IIOPServerTaskSM
//
//Inherited: StateMachine
//
//Description:
//   This class implements a state machine for
//   IIOPServerTask task.
//

class IIOPServerTask;

class IIOPServerTaskSM : public StateMachine {
  public:
    enum State { closed = 0, wfCopen, active, wfOpReply, wfLocReply, other };

    IIOPServerTaskSM (void);
    virtual ~IIOPServerTaskSM (void);

    const char *stateName (PTask *h);
    bool transit (EventType t, Message *msg, PTask *host);

  protected:
  private:
    int states;
    int events;
    bool (IIOPServerTask :: *execute [15][5]) (Message *msg);
};


#endif
