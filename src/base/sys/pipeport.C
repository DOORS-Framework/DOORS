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

$Log: pipeport.C,v $
Revision 1.3  2002/08/14 09:31:41  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/07/23 14:25:22  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:25  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:21  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/pipeport.h>
#include <doors/devif.h>
#include <doors/debug.h>

//
//Constructor: PipePort
//
//Parameters:
//      Port *,
//      PipeDevice *
//
//Description:
//      Initializes the instance.
//

PipePort :: PipePort (Port *pdevport, PipeDevice *pdev) :
Port (0), pPort (pdevport), device(pdev)
{

}


//
//Destructor: PipePort
//
//Description:
//      Disconnects the port, so the other port knows that
//      the channel is no longer in use. 
//

PipePort :: ~PipePort (void)
{

  disconnect ();
}


//
//Function: getMessage
//
//Member-Of: PipePort
//
//Parameters:
//      Message * message
//
//Return:
//      bool
//
//Description:
//      Receives the given message from other port. Called by 
//      the putMessage function of other end's port when sending
//      a message. The received message is put into PipeDevices
//      output port when necessary.
//
//      If a message is received successfully true is returned. 
//      Otherwise false value is returned. 
//

bool PipePort::getMessage (Message *message) 
{

  _DOORS_FAULT(message,"need message");

  bool rc = false;
  switch (message->getType()) {
  case (DevIf :: open) : {
    delete message;
    rc = true;
    break;
  }
  case (DevIf :: read): {
    pPort->putMessage(message);
    rc = true;
    break;
  }
  case (DevIf :: close): {
    pPort->putMessage(message);
    device->portClosed(this);
    rc = true;
    disconnect();
  }
  rc = true;
  }
  return rc;
}

