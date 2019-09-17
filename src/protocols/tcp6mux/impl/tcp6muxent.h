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
//  Revision 1.3  2005/03/02 14:32:41  bilhanan
//  development updates
//
//  Revision 1.2  2005/02/21 16:30:41  bilhanan
//  clean port of tcpmux to ipv6
//
//  Revision 1.2  2005/02/21 13:59:20  bilhanan
//  development
//
//  Revision 1.1  2004/10/12 12:01:22  bilhanan
//  The protocol formerly known as tranmux
//
//  Revision 1.6  2002/09/13 12:56:50  kalliosa
//  fixed problem with default value in constructor
//
//  Revision 1.5  2002/08/16 09:55:58  hartmanj
//  Added idletimeout to constructor
//
//  Revision 1.4  2002/08/08 13:42:04  hartmanj
//  development
//
//  Revision 1.3  2002/07/23 16:38:35  ik
//  Od{Boolean,True,False} -> {bool,true,false}
//
//  Revision 1.2  2002/07/23 07:30:45  bilhanan
//  licence added, include headers getting fixed..

#ifndef TCP6MUXENT_H
#define TCP6MUXENT_H

#include <map>
#include <string>
#include <doors/ptb.h>
#include <doors/tcp6mux.h>
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
// Tcp6muxEntity is inherited from PTask. It performs transport layer connection multiplexing 
// fpr upper layer protocols by reusing existing tcp6mux connections to remote hosts, .i.e
// if a socket connection to an ip address/port already exists, the cep is returned to the 
// upper layer in a Cconf message. If the connection doesn't exist, it is created.
// In this version, only outgoing TCP6 connections are multiplexed.

// This is useful when creating an explicit tcp6mux connection for each upper layer request 
// message is wasteful. Unused and idle connections will be disconnected after a period of
// time which can be defined in the constructor of Tcp6muxConnection.
// The default time is 1000 seconds.
 
// It is the responsibility of the upper layer to properly handle connection demultiplexing 
// or tearing down Tcp6mux's connections which don't timeout. 

// Tcp6mux has a Tcp6mux SAP provider interface up and one SocketEntity that acts as a server 
// listening certain socket port. If TCPMux is to be used as client only, set address to 0.

// It has also an array and a map for unique Tcp6muxConnections that is updated whenever connections 
// are established or closed.
//
// At this point the number of socket connections is limited to 256. Notice
// also that no range checking is done.
//

class Tcp6muxEntity : public PTask {
 public:

  Tcp6muxEntity (std::string n, Scheduler *s, IoHandler *io, Address *a,
				 Tcp6muxEntitySM *entSM);

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

 private:
  Tcp6muxConnSM *connSM;
  SockEnt device;
  Tcp6muxConnection *connections [256];
  std::map <InetAddr6, int> activemap;
  int unknown;
};


#endif  // TCP6MUXENT_H

