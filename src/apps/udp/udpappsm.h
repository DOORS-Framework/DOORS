
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

$Log: udpappsm.h,v $
Revision 1.4  2003/05/10 18:22:53  bilhanan
Obsolete references to OVOPS removed.

Revision 1.3  2002/07/25 16:03:37  ik
Use std::string.

Revision 1.2  2002/07/23 14:28:53  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.2  2001/06/08 16:45:23  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:40:05  bilhanan
Full licence clause added into doors 0.1


*/


//Description:
//  This file declares a state machine class for
//  AppTask task. 
//


#ifndef _udpappsm_h_
#define _udpappsm_h_


#include <doors/ptb.h>


//
//Class: UdpAppTaskSM
//
//Inherited: StateMachine
//
//Description:
//   This class implements a state machine for
//   UdpAppTask task.
//

class UdpAppTask;

class UdpAppTaskSM : public StateMachine {
  public:
    enum State { idle = 0, other };

    UdpAppTaskSM (void);
    virtual ~UdpAppTaskSM (void);

    std::string stateName (PTask *h);
    bool transit (EventType t, Message *msg, PTask *host);

  protected:
  private:
    int states;
    int events;
    bool (UdpAppTask :: *execute [5][1]) (Message *msg);
};


#endif
