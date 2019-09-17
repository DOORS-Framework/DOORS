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

$Log: sm.C,v $
Revision 1.3  2002/07/23 14:25:20  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:52  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:34  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:34  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/sm.h>


//
//Function: transit
//
//Member-Of: StateMachine
//
//Parameters:
//   EventType t, Message *msg, PTask *h
//
//Return: bool
//
//Description:
//   This function is called from PTask's execute function.
//   It is meant to be replaced in a class inherited from
//   this base class where it should
//   resolve the type of the message given as a second
//   parameter and then execute the message.
//   
//   First parameter t is a type of the event (msg). It can
//   be either UserEvent or SystemEvent (used for timers).
//   
//   Second parameter msg is a message to be processed in
//   this execution turn.
//   
//   Third parameter h is a task calling this function.
//
//   true return value indicates that message can
//   no longer be manipulated. false indicates
//   that message can be deleted.   

bool StateMachine :: transit (EventType t, Message *msg, PTask *h) {

    if (h) {
	return h->allDefault (msg);
    }
}


