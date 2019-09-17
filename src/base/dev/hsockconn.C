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

$Log: hsockconn.C,v $
Revision 1.3  2002/08/14 09:31:36  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/07/23 14:25:15  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:21  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:15  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/hsockconn.h>
#include <doors/sockmsg.h>

//
//Constructor: HSockConn
//
//Parameters:
//      IoHandler *ioh,
//      EventTask *t,
//      NetworkHeader *hdr 
//
//Description:
//  Initializes the base class and own variables. Constructor is
//  used when instance is created without actually connecting to 
//  the peer entity. It can be done later with connect function call. 
//
//  The parameter ioh is the i/o handler of the system,
//  t is the task that holds the device and hdr is the pointer
//  to class that handles the headers in the front of the streams.
//
HSockConn :: HSockConn (IoHandler *io, EventTask *t, NetworkHeader *h): 
SockConn (io, t),
header(h),
internal(false)
{
  _MT_SYNCMONITOR;

  if (!header){
    header = new SockHeader;
    internal = true;
  }
}


//
//Constructor: HSockConn
//
//Parameters:
//      IoHandler *ioh,
//      EventTask *t,
//      int sd,
//      NetworkHeader *hdr 
//
//Description:
//  Initializes the base class and own variables. Constructor is
//  used when instance is created with given file decriptor. This
//  usually happens in server side where descriptor is created by
//  SocketEntity.
//
//  The parameter ioh is the i/o handler of the system,
//  t is the task that holds the device and hdr is the pointer
//  to class that handles the headers in the front of the streams.
//  sd is descriptor for connection to be communicate with peer
//  entity.
//
HSockConn :: HSockConn (IoHandler *io, EventTask *t, Uint32 sd, NetworkHeader *h):
SockConn (io,t,sd),
header (h),
internal(false)
{
  _MT_SYNCMONITOR;

  if (!header){
    header = new SockHeader;
    internal = true;
  }
  SockConn :: readBytes (DEFAULT_CELLSIZE);
}

//
//Constructor: SockConn
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
//  t is the task that holds the device and hdr is the pointer
//  to class that handles the headers in the front of the streams.
//  a is a address of the peer entity.
//
HSockConn :: HSockConn (IoHandler *io, EventTask *t, Address *a, NetworkHeader *h):
SockConn (io,t,a),
header (h),
internal(false)
{
  _MT_SYNCMONITOR;

  if (!header){
    header = new SockHeader;
    internal = true;
  }
  SockConn :: readBytes (DEFAULT_CELLSIZE);
}


//
//Destructor: HSockConn
//
//Description:
//  Delete connector and stream instances.
//
HSockConn :: ~HSockConn (void) {

  _MT_SYNCMONITOR;

  status &= ~Device::READ;
  status &= ~Device::WRITE;
  status = Device::IDLE;    

  if (header && internal) {
    delete header;
    header = 0;
  } 
}

//Function: connect
//
//Member-Of: HSockConn
//
//Returns: bool
//
//Parameters:
//        Address *a
//
//Description:
//  Function is used for connecting to peer entity given as
//  a parameter a. The type of connection is checked from the
//  type of address and proper connector and stream is created.
//  The controlling task is informed both the success of failure
//  of the connection establishement through the DevIf.
//

bool HSockConn :: connect (Address *a) {
 
  bool retval = SockConn :: connect (a);
  SockConn :: readBytes (DEFAULT_CELLSIZE);
  return retval;
}

//Function: connect
//
//Member-Of: HSockConn
//
//Returns: bool
//
//Parameters:
//        Uint32 sd
//
//Description:
//  Function is used for connecting to peer entity given as
//  a file descriptor sd. The type of connection is checked from the
//  type of file descriptor and proper stream is created.
 
bool HSockConn :: connect (Uint32 sd) {

  bool retval = SockConn :: connect (sd);
  SockConn :: readBytes (DEFAULT_CELLSIZE);
  return retval;
}

 
//Function: write
//
//Member-Of: HSockConn
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

bool HSockConn :: write (Frame &frm) {

  _MT_SYNCMONITOR;

  if (_connected) {
    header->putHeader (frm);
    out.frame->combine (frm);
    status |= Device::WRITE;
    
    // write will be performed later in callback function
    return false;
  }
  
  // Not connected to socket
  return true;
}


//
//Function: setCellSize
//
//Member-Of: HSockConn
//
//Parameters:
//      Sint32 i
//
//Description:
//  This function is called when the cell size is wanted to be changed. 
//  To do this function calls SocketConnectionAC function readBytes, where
//  the in buffer size is set to the correct size.
//


void HSockConn :: setCellSize (Sint32 i)
{
  _MT_SYNCMONITOR;
  SocketConnectionAC :: readBytes (i);
}

//
//Function: getPacketSize
//
//Member-Of: HSockConn
//
//Returns:
//      Sint32 i
//
//Description:
//  This function removes the header and determines the size of incoming 
//  and returns it.


Sint32 HSockConn :: getPacketSize(void){

  _MT_SYNCMONITOR;

  int nbytes;
  Message *bmsg = 0;
  
  if (wsize == 0) {
    wsize = header->removeHeader (fd);
  }

  return wsize;
}

//
//Function: readBytes
//
//Member-Of: HSockConn
//
//Parameters:
//      Sint32 i
//
//Description:
//  Inform wanted data amount to be read from device. 
//  

void HSockConn :: readBytes (Sint32 i)
{
  _MT_SYNCMONITOR;
  in.setSize(i);
  setRead (true);
}








