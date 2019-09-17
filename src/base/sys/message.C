// 
//  Copyright 1999 
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
 
 
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

$Log: message.C,v $
Revision 1.6  2003/11/14 10:04:48  bilhanan
reintroduced unsignedness, multiple receipient count decrementing
modified.

Revision 1.5  2003/11/12 17:46:38  bilhanan
added signedness.

Revision 1.4  2003/10/15 10:48:28  bilhanan
added broadcast message functionality.

Revision 1.3  2002/07/23 14:25:21  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:53  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:22  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:17  bilhanan
Full licence clause added into doors 0.1


*/

#include <doors/message.h>
#include <doors/port.h>

Message :: Message (void) 
  : Link (0), freezed (false), target (0), sender (0), MultipleTargets (false), rcpt_count (0)
{
}

Message :: Message (const Message &msg) 
  : Link (0), freezed (msg.isLocked ()), target (msg.getTarget ()), sender (msg.getSender ()), MultipleTargets (msg.isForMany()), rcpt_count (msg.get_rcpt_count())
{
}

// create vtbl here
Message :: ~Message (void)
{
}

void Message :: mark (Port *s, Port *t) {

    if (!freezed) {
	sender = s;
	target = t;
    }
}

void Message :: lock (void) {

    freezed = true;
}

void Message :: unlock (void) {

    freezed = false;
}

bool Message :: isLocked (void) const {

    return freezed;
}

Message * Message :: clone (void) const {

    return new Message (*this);
}

void Message :: setMultipleTargets (bool Multiple_) {

  MultipleTargets = Multiple_;
  // freezed = Multiple_;  // automatic lock/unlock if multiple, single
  
}

void Message :: increment_rcpt_count (void) {

  if (MultipleTargets)
    ++rcpt_count;
}

Uint16 Message :: decrement_rcpt_count (void) {

  if (MultipleTargets) {
    if (rcpt_count)
      --rcpt_count;
  }
  return rcpt_count;
}

