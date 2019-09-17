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

$Log: sockconn.h,v $
Revision 1.4  2004/05/28 12:39:37  bilhanan
initial support for TCP connections over IPv6.

Revision 1.3  2002/08/09 06:38:13  hartmanj
#include <sockhelper.h> to #include <doors/sockhelper.h>

Revision 1.2  2002/07/23 14:25:16  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:00  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:23  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef SOCKCONN_H
#define SOCKCONN_H

//#include "macroif.h"
#include <doors/hsi.h>
#include <doors/soconnac.h>
#include <doors/buffer.h>
#include <doors/sockhelper.h>


/** SockConn can be seen  as a client that tries to
  connect into SockEnt. When connection is established 
  another SockConn instance is also created into the
  peer entity and the actual data transfer is done between
  these two instances.  The device informs different event to the
  controlling task by sending message with proper parameters. 
  SockConn is read-write device. */

class SockConn : public SocketConnectionAC, public SockHelper {
  public:
  /** Initializes the base class and own variables. This constructor is
    used when instance is created without actually connecting to 
    the peer entity. It can be done later with connect function call.
    @param io I/O handler of the system
    @param t the task that holds the device*/
  SockConn (IoHandler *io, EventTask *t);

  /** Initializes the base class and own variables. Constructor is
    used  when instance is created with given file decriptor. This
    usually happens in server side where descriptor is created by
    SocketEntity.
    @param ioh the i/o handler of the system.
    @param t the task that holds the device 
    @param sd is descriptor for connection to be 
    communicate with peer entity. */    
  SockConn (IoHandler *io, EventTask *t, Uint32 sd);

  /** Initializes the base class and own variables. Constructor is
    used when instance is created with given peer entity address. 
    This usually happens in client side where connection is established
    by the instance itself.
    @param ioh the i/o handler of the system,
    @param t the task that holds the device
    @param a an address of the peer entity. */
  SockConn (IoHandler *io, EventTask *t, Address *a);

  /**Delete connector and stream instances. */
  ~SockConn (void);

  /** Function is used for connecting to peer entity. The type of 
    connection is checked from the type of address and proper 
    connector and stream is created.
    The controlling task is informed both the success of failure
    of the connection establishement through the DevIf.
    @param a The address of peer entity.*/
  bool connect (Address *a);

  /** Function is used for connecting to peer entity. The type of 
    connection is checked from the type of file descriptor and 
    proper stream is created.
    @param sd The filedescriptor of peer entity */
  bool connect (Uint32 sd);


  /**Close device and send proper message to the 
    controlling task. */
  virtual bool close (void);

  /** @return File descriptor of this connection */
  virtual int getFd (void) const;
  
  /** Function is runned when something is to be writen into the
    socket. <p>
    If it is possible to write all data into the socket it is done without 
    any breaks. If it is not possible then writing is done only partially
    and it is continued when data can be sended.<p>
    If for some reason error happens then controlling task is informed
    with DevIf :: Close message. */
  void callbackWrite (void);

  /** Function is runned when something is to be read from the
    socket. <p>  
    If there is all data availeble then it is read without any breaks
    and SocketData message is send to the controlling task. If there 
    is not enough data availeble then partial reading is used  and reading 
    is continued when there is new data availeble.
    The data is sent in DevIF :: Read message to the task when either 
    packetlenght is zero or wsize is set for -1 (StreamDevice :: immediate).
    wsize is set to zero after data is sent and it must be set with 
    devices readBytes function before every reading for the 
    sake of synchronising. <p>  
    If for some reason error happens then controlling task is informed
    with DevIf :: Close message. */
  void callbackRead (void);

  /** Function wraps the system call getpeername.
    @return the address of the peer entity. */
  Address *getRemoteAddress (void);
  
  protected:
    int bindPort(int sock, int addrType);
};


#endif
