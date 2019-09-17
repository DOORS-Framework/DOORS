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

$Log: soconnac.C,v $
Revision 1.4  2002/08/14 09:31:36  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 14:25:16  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:48  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:28  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:25  bilhanan
Full licence clause added into doors 0.1


*/

#include <doors/soconnac.h>
#include <doors/sockmsg.h>

//
//Constructor: SocketConnectionAC
//
//Parameters:
//      IoHandler *ioh,
//      EventTask *t,
//
//Description:
//  Initializes the base class and own variables. Constructor is
//  used when instance is created without actually connecting to 
//  the peer entity. It can be done later with connect function call. 
//
//  The parameter ioh is the i/o handler of the system,
//  t is the task that holds the device.
//
SocketConnectionAC :: SocketConnectionAC (IoHandler *io, EventTask *t): 
StreamDevice (io,t),
packetLength (0),
in (256),
out (256),
wsize (0),
_connected (false) {

  _MT_SYNCMONITOR;
  _MT_RESET_BLOCKTIME(0);
}


//
//Constructor: SocketConnectionAC
//
//Parameters:
//      IoHandler *ioh,
//      EventTask *t,
//      int sd,
//
//Description:
//  Initializes the SocketConnectionAbstractClass and variables.
//
//  The parameter ioh is the i/o handler of the system,
//  t is the task that holds the device and sd is descriptor 
//  for connection to be communicate with peer entity.
//
SocketConnectionAC :: SocketConnectionAC (IoHandler *io, EventTask *t, Uint32 sd):
StreamDevice (io,t),
packetLength (0),
in (256), 
out (256),
wsize (0),
_connected (true) {

  _MT_SYNCMONITOR;
}

//
//Constructor: SocketConnectionAC
//
//Parameters:
//      IoHandler *ioh,
//      EventTask *t,
//      Address *a,
//      NetworkHeader *hdr 
//
//Description:
//  Initializes the base class and own variables. Constructor is
//  used when instance is created with given peer entity address. 
//  This usually happens in client side where connection is established
//  by the instance itself.
//
//  The parameter ioh is the i/o handler of the system,
//  t is the task that holds the device and a is a address of 
//  the peer entity.
//
SocketConnectionAC :: SocketConnectionAC (IoHandler *io, EventTask *t, Address *a):
StreamDevice (io,t),
packetLength (0),
in (256), 
out (256),
wsize (0),
_connected (false) {

  _MT_SYNCMONITOR;
}

//
//Destructor: SocketConnectionAC
//
//Description:
//

SocketConnectionAC :: ~SocketConnectionAC (void) {

  _MT_SYNCMONITOR;

}

//
//Returns: bool
//
//Parameters:
//
//Description:
//  Returns wheather instance is connected or not.
//
bool SocketConnectionAC :: connected (void) {
  
  _MT_SYNCMONITOR;
  return _connected;
}

//
//Function: write
//
//Member-Of: SocketConnectionAC
//
//Parameters:
//      Frame &frm
//Return:
//      bool
//Description:
//  Writes the given frame to the socket. Writing is asyncronous,
//  that is the function returns immediately. The actual writing
//  is done later by interoperation of callback routines
//  of the device and the i/o handler.
//
//  Due to the protocol used the size of a packet is limited to 0xffff.
//

bool SocketConnectionAC :: write (Frame &frm)
{
  _MT_SYNCMONITOR;

  if (_connected) {

    out.frame->combine (frm);
    status |= Device::WRITE;
    
    // write will be performed later in callback function
    return false;
  }
  
  // Not connected to socket
  return true;
}

//
//Function: readBytes
//
//Member-Of: SocketConnectionAC
//
//Description:
//      Sets the size of buffer and makes device ready for reading.
//      If i is -1 (StreamDevice :: immediate), the device sends data
//      immediately to the task.


void SocketConnectionAC :: readBytes (Sint32 i)
{
  _MT_SYNCMONITOR;

  in.setSize(i);
  if (i != 0) {
    setRead(true);
  }
  else setRead(false);

}

//
//Function: getPacketSize
//
//Member-Of: SocketConnectionAC
//
//Description:
//      Determines the size of cell based to the size of in buffer.


Sint32 SocketConnectionAC :: getPacketSize(void){

  _MT_SYNCMONITOR;

  wsize = in.getSize();
  setRead(false);  
  if (wsize > 0)
    return wsize;
  else
    return DEFAULT_CELLSIZE;

}


//
//Function: setPacketSize
//
//Member-Of: SocketConnectionAC
//
//Description:
//      Set the size of cell in buffers.

void SocketConnectionAC :: setPacketSize (Sint32 i){

  _MT_SYNCMONITOR;

  in.setSize (i);
  out.setSize (i);
}


//Function: readBuffer
//
//Member-Of: SocketConnectionAC
//
//Parameters: 
//
//Description:
//  Function returns the host task 

Sint32 SocketConnectionAC :: readBuffer (void) 
{
  _MT_SYNCMONITOR;

  return in.frame->length();
}


//Function: readBuffer
//
//Member-Of: SocketConnectionAC
//
//Parameters: 
//
//Description:
//  Function returns the host task 

Sint32 SocketConnectionAC :: writeBuffer (void) 
{
  _MT_SYNCMONITOR;

  return out.frame->length();
}
