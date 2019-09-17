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

$Log: strdev.C,v $
Revision 1.4  2002/08/14 09:31:37  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 14:25:16  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:48  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:29  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:26  bilhanan
Full licence clause added into doors 0.1


*/

#include <fcntl.h>
#include <unistd.h>


#include <doors/strdev.h>
#include <doors/util.h>

//
//Constructor: StreamDevice
//
//Parameters:
//      IoHandler *ioh
//      EventTask *t
//
//Description:
//  Class that gives general interface for the stream based devices.  
//


StreamDevice :: StreamDevice (IoHandler *ioh, EventTask *t ) 
    : Device(ioh), iFace (t), task (t)
{
  _MT_SYNCMONITOR;
}
//
//Destructor: StreamDevice
//

StreamDevice :: ~StreamDevice (void)
{
  _MT_SYNCMONITOR;
}


//Function: setRead
//
//Member-Of: StreamDevice
//
//Parameters: 
//      bool r
//
//Description:
//  Function sets the stream based device ready for reading.

void StreamDevice :: setRead (bool r) 
{
  _MT_SYNCMONITOR;

  r ? (status = Device::READ) : (status &= ~Device::READ);
}


//Function: getTask
//
//Member-Of: StreamDevice
//
//Parameters: 
//
//Description:
//  Function returns the host task 

EventTask *StreamDevice :: getTask (void) 
{
  _MT_SYNCMONITOR;

  return task;
}


//Function: readBuffer
//
//Member-Of: StreamDevice
//
//Return: Sint32 
//
//Description:
//  Empty implementations. 

Sint32 StreamDevice :: readBuffer (void) 
{
  _MT_SYNCMONITOR;

  return 0;
}


//Function: writeBuffer
//
//Member-Of: StreamDevice
//
//Return: Sint32 
//
//Description:
//  Empty implementations 

Sint32 StreamDevice :: writeBuffer (void) 
{
  _MT_SYNCMONITOR;

  return 0;
}
