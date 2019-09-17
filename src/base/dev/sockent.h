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

$Log: sockent.h,v $
Revision 1.2  2002/07/23 14:25:16  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:01  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:24  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef SOCKENT_H
#define SOCKENT_H

#include <doors/util.h>
#include <doors/core.h>
#include <doors/socentac.h>
#include <doors/sockhelper.h>


//
// SockEnt acts as a server listening certain socket port number. It 
// informs task about incoming connection request by sending 
// DevIf::Open message. The task that is attached 
// to SockEnt must handle the connection requests and create 
// actual data transferring objects by instantiating SockConn.
// SockEnt is read-only device.

class SockEnt : public SocketEntityAC,public SockHelper {
  public:
  /**  Initializes the base class and own variables. 
    @param ioh the i/o handler of the system.
    @param t the task that holds the device. */
  SockEnt (IoHandler *io, EventTask *t);

  /** Inform i/o handler to ignore this device */
  ~SockEnt (void);
  /**Function is runned when connection request is
    noticed. Created descriptor is returned for the
    controlling task by sending the DevIf :: Open 
    message to it. */
  void callbackRead (void);
  
  /* @return the filedescriptor. */
  virtual int getFd (void) const;

  /** Function is used for connecting to peer entity. The type of 
    connection is checked from the type of address and proper 
    connector and stream is created.
    The controlling task is informed both the success of failure
    of the connection establishement with DevIf message. 
    @param the address of peer entity*/
  void connect (Address *a);
  
  /** Close device and send proper message to the 
    controlling task. */
  virtual bool close (void);
};


#endif






