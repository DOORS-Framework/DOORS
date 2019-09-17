
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

$Log: udp6appsm.h,v $
Revision 1.4  2003/05/10 18:22:54  bilhanan
Obsolete references to OVOPS removed.

Revision 1.3  2002/07/25 16:03:37  ik
Use std::string.

Revision 1.2  2002/07/23 14:28:55  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.1  2002/06/14 22:04:06  ik
Development. Initial commit for udp6 application.


*/


//Description:
//  This file declares a state machine class for
//  AppTask task. 
//


#ifndef _udp6appsm_h_
#define _udp6appsm_h_


#include <doors/ptb.h>


//
//Class: Udp6AppTaskSM
//
//Inherited: StateMachine
//
//Description:
//   This class implements a state machine for
//   Udp6AppTask task.
//

class Udp6AppTask;

class Udp6AppTaskSM : public StateMachine {
  public:
    enum State { idle = 0, other };

    Udp6AppTaskSM (void);
    virtual ~Udp6AppTaskSM (void);

    std::string stateName (PTask *h);
    bool transit (EventType t, Message *msg, PTask *host);

  protected:
  private:
    int states;
    int events;
    bool (Udp6AppTask :: *execute [5][1]) (Message *msg);
};


#endif
