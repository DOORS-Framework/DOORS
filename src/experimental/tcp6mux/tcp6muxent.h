//Editor-Info: -*- C++ -*-

//  Copyright 2002 
//  	Telecoms Lab, Tampere University of Technology.  All rights reserved.

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


//  $Log: tcp6muxent.h,v $
//  Revision 1.1  2004/08/18 06:22:16  bilhanan
//  TCP IPv6 multiplexer
//


#ifndef TCP6MUXENT_H
#define TCP6MUXENT_H

#include <map>
#include <string>
#include <doors/ptb.h>
#include "tcp6mux.h"
#include <doors/sockmsg.h>
#include <doors/sockent.h>
class Tcp6muxEntitySM;
class Tcp6muxConnSM;
class Tcp6muxConnection;

//
//Class: Tcp6muxEntity
//
//Inherited: PTask
//
//Description:
// Tcp6muxEntity is inherited from PTask. It performs IPv6 transport layer connection multiplexing 
// fpr upper layer protocols by reusing existing tran connections to remote hosts, .i.e
// if a socket connection to an ip address/port already exists, the cep is returned to the 
// upper layer in a Cconf message. If the connection doesn't exist, it is created.

// This is useful when creating an explicit tran connection for each upper layer request 
// message is wasteful. Unused and idle connections will be disconnected after a period of
// time which can be defined in the costructor of TranMuxEntity. The timeout time is in seconds.
// The default time is 5 minutes.
 
// It is the responsibility of the upper layer to properly handle connection demultiplexing 
// or tearing down Tcp6mux's connections which don't timeout. 

// Tcp6mux has a Tcp6mux SAP provider interface and one SocketEntity that acts as a server 
// listening to a certain socket port.

// It has also a table for unique Tcp6muxConnections that is updated whenever connections 
// are established or closed.
//
// At this point the number of socket connections is limited to 256. No range checking is done.
//

class Tcp6muxEntity : public PTask {
 public:

  Tcp6muxEntity (std::string n, Scheduler *s, IoHandler *io, Address *a,
				 Tcp6muxEntitySM *entSM, Uint32 idletimeout=300);

  virtual ~Tcp6muxEntity (void);
  
  bool ready_Creq (Message *msg);  
  bool ready_Cresp (Message *msg);
  bool ready_Dtreq (Message *msg);
  bool ready_Dreq (Message *msg);
  bool ready_Data (Message *msg);
  bool ready_Conn (Message *msg);
  bool ready_Disconn (Message *msg);
  bool ready_default (Message *msg);

  Tcp6muxConnection *getConnection (int i);
    
  Tcp6muxSAP :: Provider up;
  DevIf :: User entity;
  std::map <InetAddr6, Uint32> conntable;

 private:
  Tcp6muxConnSM *connSM;
  SockEnt device;
  Tcp6muxConnection *connections [256];
  Uint32 itimeout;
};


#endif  // TCP6MUXENT_H

