// 
//  Copyright 1999
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
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

$Log: ptask.h,v $
Revision 1.5  2004/02/10 15:27:52  bilhanan
introducing ptaskid and get/set functions.

Revision 1.4  2002/07/25 10:08:39  ik
Use std::string.

Revision 1.3  2002/07/23 14:25:20  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:51  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:52  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:32  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef PTASK_H
#define PTASK_H

#include <string>
#include <doors/core.h>

class StateMachine;

//
//Class: PTask
//
//Description:
//   PTask is a base class for all protocol entities. A
//   concrete protocol entity should be inherited from this
//   class. In derived task there should be all properties
//   (e.g. task's variables, timers, buffers ...) and
//   functions the entity task needs to accomplish its duty.
//   PTask is derived from EventTask defined in DOORS core
//   module. It has same functionality as EventTask.
//   Furthermore it gives a concept of state relating to
//   state machine.
// 

class PTask : public EventTask {
  public:
    PTask (std::string n, Scheduler *s, StateMachine *m = 0);
    virtual ~PTask (void);

    virtual bool execute (EventType t, Message *msg);

    virtual bool allDefault (Message *msg);

    virtual Uint16 getState (void);
    StateMachine *getStateMachine (void);

    Uint32 getptaskid (void);
    void setptaskid (Uint32 newid);

  protected:
    StateMachine *sm;
    int state;
    Uint32 ptaskid;
  private:
};



#endif  // PTASK_H
