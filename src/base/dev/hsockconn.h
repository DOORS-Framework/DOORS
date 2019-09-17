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

$Log: hsockconn.h,v $
Revision 1.2  2002/07/23 14:25:15  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:35  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:15  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef HSOCKCONN_H
#define HSOCKCONN_H

#include <doors/util.h>
#include <doors/core.h>
#include <doors/hsi.h>
#include <doors/buffer.h>
#include <doors/sockheader.h>
#include <doors/sockconn.h>

/** HSockConn can be seen as a client that tries to 
  connect into SockEnt. When connection is established another 
  HSockConn instance is also created into the peer 
  entity and the actual data transfer is done between  these two instances.
  The device informs different event to the controlling task by sending 
  message with proper parameters. HSockConn is 
  read-write device. This class is used for headered communication. */
class HSockConn : public SockConn {
public:
  
  /** Initializes the base class and own variables. Constructor is
    used when instance is created without actually connecting to 
    the peer entity. It can be done later with connect function call. 
    
    @param ioh the i/o handler of the system.
    @param t the task that holds the device  
    @param hdr the pointer to class that handles the headers in the 
    front of the streams. */
  HSockConn (IoHandler *io, EventTask *t, NetworkHeader *h = 0);
  /**  Initializes the base class and own variables. Constructor is
    used when instance is created with given file decriptor. This
    usually happens in server side where descriptor is created by
    SockEnt.
    @param ioh the i/o handler of the system.
    @param t the task that holds the device
    @param sd descriptor for connection to be communicate with peer entity
    @param hdr the pointer to class that handles the headers in the 
    front of the streams */
  HSockConn (IoHandler *io, EventTask *t, Uint32 sd, NetworkHeader *h = 0);

  /** Initializes the base class and own variables. Constructor is
    used when instance is created with given peer entity address. 
    This usually happens in client side where connection is established
    by the instance itself.
    @param ioh the i/o handler of the system.
    @param t the task that holds the device.
    @param a a address of the peer entity.
    @param hdr the pointer to class that handles the headers in the 
    front of the streams */
  HSockConn (IoHandler *io, EventTask *t, Address *a, NetworkHeader *h = 0);
  /** In destructor connector and stream instance are deleted */
  ~HSockConn (void);

  /** Function is used for connecting to peer entity given as
    a parameter a. The type of connection is checked from the
    type of address and proper connector and stream is created.
    The controlling task is informed both the success of failure
    of the connection establishement through the Device Interface DevIf
    @param a the adress of peer entity */
  bool connect (Address *a);

  /** Function is used for connecting to peer entity given as
    a file descriptor sd. The type of connection is checked from the
    type of file descriptor and proper stream is created.
    @param sd the filedescriptor of peer entity.*/
  bool connect (Uint32 sd);

  /** This function is called when the cell size is wanted to be changed. 
    To do this function calls SocketConnectionAC function readBytes, where
    the in buffer size is set to the correct size.
    @param i the wanted size of cell.*/
  void setCellSize (Sint32 i);

  /**Writes the given frame to the socket. Writing is asyncronous,
    that is the function returns immediately. The actual writing
    is done later by interoperation of callback routines
    of the device and the i/o handler.<p>
    Due to the protocol used the size of a packet is limited to 0xffff
    @param frm is the frame containing data to be written.*/
  bool write (Frame &frm);
    
  protected:
  /** the amount of data wanted to be read from device.
    @param i the amount of bytes wanted to be read.
    StreamDevice :: immediate uses default value.
    */
    virtual void readBytes (Sint32 i);

    /** Function removes the header and determines the size of incoming 
      packet and returns it.
      @return size of packet */
    virtual Sint32 getPacketSize(void);
    NetworkHeader *header;

  private:
    bool internal;

};

#endif



