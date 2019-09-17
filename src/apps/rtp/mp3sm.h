//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
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

$Log: mp3sm.h,v $
Revision 1.2  2002/07/23 14:28:52  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.2  2001/06/08 16:45:42  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:40:01  bilhanan
Full licence clause added into doors 0.1


*/



//
//File: mp3sm.h
//
//Date: Wed Aug 16 19:31:22 2000
//
//Commentary:
//  This is a machine generated file produced by 
// compiler. Editing is not recommended.
//
//Description:
//  This file declares a state machine class for
//  MP3Task task. 
//


#ifndef _mp3sm_h_
#define _mp3sm_h_


#include <doors/ptb.h>


//
//Class: MP3TaskSM
//
//Inherited: StateMachine
//
//Description:
//   This class implements a state machine for
//   MP3Task task.
//

class MP3Task;

class MP3TaskSM : public StateMachine {
  public:
    enum State { Idle = 0, other };

    MP3TaskSM (void);
    virtual ~MP3TaskSM (void);

    const char *stateName (PTask *h);
    bool transit (EventType t, Message *msg, PTask *host);

  protected:
  private:
    int states;
    int events;
    bool (MP3Task :: *execute [8][1]) (Message *msg);
};


#endif
