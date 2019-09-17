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

$Log: sm.h,v $
Revision 1.4  2002/07/25 10:08:39  ik
Use std::string.

Revision 1.3  2002/07/23 14:25:20  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:52  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:59  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:34  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef SM_H 
#define SM_H

#include <string>
#include <doors/core.h>
#include <doors/ptask.h>


//
//Class: StateMachine
//
//Description:
//   StateMachine is a base class for state machines user can
//   implement for PTask. It defines a virtual transit
//   function for executing the messages. Furthermore it
//   defines function to return a user understandable name of
//   the task's state.
//   

class StateMachine {
  public:
    virtual bool transit (EventType t, Message *msg, PTask *h);

    virtual std::string stateName (PTask *h) = 0;

  protected:
  private:
};


//
//Function: stateName
//
//Member-Of: StateMachine
//
//Parameters:
//   PTask *h
//
//Return:
//   std::string
//
//Description:
//   By this function user can get a human understandable
//   string for state the task given as a parameter holds.
//   
//   Parameter h is a task whose state name will be returned.
//   
//   Returns a state of the calling task in string format.
//   

#endif  // SM_H

