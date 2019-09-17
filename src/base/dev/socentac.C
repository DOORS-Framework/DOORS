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

$Log: socentac.C,v $
Revision 1.3  2002/08/14 09:31:36  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/06/03 09:19:47  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:26  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:22  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/socentac.h>
#include <doors/sockmsg.h>
#include <doors/ocoremac.h>

//
//Constructor: SocketEntityAC
//
//Parameters:
//      IoHandler *ioh,
//      EventTask *t,
//
//Description:
//  Initializes the SocketEntityAbstractClass. 
//
//  The parameter ioh is the i/o handler of the system
//  and t is the task that holds the device. 

SocketEntityAC :: SocketEntityAC (IoHandler *io, EventTask *t)
: StreamDevice (io, t)
{
  _MT_SYNCMONITOR;
  _MT_RESET_BLOCKTIME(1);
}

//
//Destructor: SocketEntityAC
//
//Description:
//

SocketEntityAC :: ~SocketEntityAC (void) {
  
  _MT_SYNCMONITOR;

  status &= ~Device::READ;
  status = Device::IDLE;    

  if (handler) {
    handler->forget (this);
    handler = 0;
  }

}

//
//Function: readBytes
//
//Member-Of: SocketEntityAC
//
//Description:
// There is nothing to read from SocketEntity so implementation is empty.

void SocketEntityAC :: readBytes (Sint32 i)
{
  _MT_SYNCMONITOR;
  return;
}






