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

$Log: device.C,v $
Revision 1.4  2002/08/14 09:31:35  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 14:25:15  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:47  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:19  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:12  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/device.h>
#include <doors/ocoremac.h>
#include <doors/debug.h>

const OTime Device :: undefined (LONG_MAX, LONG_MAX);

//
//Constructor: Device
//
//Parameters:
//      IoHandler *ioHandler
//
//Description:
//      The constructor initializes all device's variables and tells
//      the i/o handler about the existence of this device. Without 
//      the i/o handler the device cannot operate. If it is not given
//      to the constructor the user must herself call the inform 
//      function of the i/o handler. 
//
//      The ioHandler parameter is a pointer to the i/o handler of 
//      the system. 
//
Device :: Device (IoHandler *ioHandler)
{
  _MT_SYNCMONITOR;
  _DOORS_DEBUG(0,"Device, fd:");

  handler = ioHandler;
  status = Device::IDLE;
  
  if (handler) {
    handler->inform (this);
  }

  _MT_RESET_DEVICE_VARIABLES;
  _MT_RESET_ALIVE;
}


//
//Destructor: Device
//
//Description:
//      Tells the i/o handler that this device is no longer in use.
//
Device :: ~Device (void)
{
  _MT_SYNCMONITOR;
  _DOORS_DEBUG(0,"~Device, fd:");

  if (handler) {
    handler->forget (this);
  }
  _MT_RESET_ALIVE;
  _MT_RELEASE_SYNCMONITOR;

  _MT_SIGNAL_RUNLOCK;
  _MT_WAIT_EXITBARRIER(this);
}

//
//Function: setRead
//
//Member-Of: Device
//
//Parameters:
//      bool
// 
//Description:
//  Set read bit of the device.
//

void Device :: setRead (bool r) 
{
  _MT_SYNCMONITOR;
  r ? (status |= Device::READ) : (status &= ~Device::READ);
}

//
//Function: setWrite
//
//Member-Of: Device
//
//Parameters:
//      bool
// 
//Description:
//  Set write bit of the device.
//

void Device :: setWrite (bool w) 
{
  _MT_SYNCMONITOR;
  w ? (status |= Device::WRITE) : (status &= ~Device::WRITE);
}

//
//Function: getTimeout
//
//Member-Of: Device
//
//Return: OTime      
// 
//Description:
//  Function returns the timeout time of the device.
//  In this base class undefined value is returned.

OTime Device :: getTimeout (void) const 
{
  _MT_SYNCMONITOR;
  return undefined;
}
