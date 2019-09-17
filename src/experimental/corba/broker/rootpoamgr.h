

//  Copyright 2000 
//  	Telecoms Lab, Tampere University of Technology.  All rights reserved.

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

$Log: rootpoamgr.h,v $
Revision 1.2  2002/07/23 14:32:47  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:54  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:39:45  bilhanan
Full licence clause added into doors 0.1


*/

//
//File: rootpoamgr.h
//
//Date: Thu Dec 30 15:39:51 1999
//
//Commentary:
//  This is a machine generated file produced by 
// compiler. Editing is not recommended.
//
// Drat.
// I needed to edit it. -Bill
// Among other things, Singleton'ed, CORBA'd and namespaced.
// Save into a separate file first if you're silly enough to regenerate
// rootpoamgr.aut

//Description:
//  This file declares a state machine class for
//  RootPOA task. 
//


#ifndef _rootpoamgr_h_
#define _rootpoamgr_h_


#include "ptb.h"

namespace PortableServer {
//
//Class: RootPOAManager
//
//Inherited: StateMachine
//
//Description:
//   This class implements a state machine for
//   RootPOA task.
//

  class RootPOA;

  class RootPOAManager : public StateMachine {
  public:
    enum State { HOLDING = 0, ACTIVE, DISCARDING, INACTIVE, other };
    
    
    virtual ~RootPOAManager (void);
    
    const char *stateName (PTask *h);
    bool transit (EventType t, Message *msg, PTask *host);
    static RootPOAManager *Instance (Ptask *h);
    
    // the following are all in the idl defs
    void activate(void);
    void hold_requests (CORBA::Boolean wait_for_completion);
    void discard_requests (CORBA::Boolean wait_for_completion);
    void deactivate (CORBA::Boolean etherealize_objects, CORBA::Boolean wait_for_completion);
    RootPOAManager::State get_state(void);
    
  protected:
  private:
    int states;
    int events;
    PTask* rootpoa;
    bool (RootPOA :: *execute [4][4]) (Message *msg);
    static RootPOAManager* _instance;
    RootPOAManager (void);
  };
  
}
#endif
