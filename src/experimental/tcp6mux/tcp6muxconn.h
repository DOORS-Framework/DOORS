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


//  $Log: tcp6muxconn.h,v $
//  Revision 1.1  2004/08/18 06:22:16  bilhanan
//  TCP IPv6 multiplexer
//


#ifndef TCP6MUXCONN_H
#define TCP6MUXCONN_H

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <doors/ptb.h>
#include "tcp6mux.h"
#include <doors/sockmsg.h>
#include <doors/sockconn.h>
#include "tcp6muxent.h"
#include <doors/inetaddr6.h>

class Tcp6muxEntity;
typedef Tcp6muxEntity Tcp6muxEnt;

class Tcp6muxConnSM;

//
//Class: Tcp6muxConnection
//
//Inherited: PTask
//
//Description:
// Tcp6muxConnection is inherited from PTask. It has a reference of tcp6mux 
// SAP provider interface from Tcp6muxEntity which allows connection to 
// send indications directly to service user. It also has one 
// SocketConnection instance which is used for actual data transfer.

class Tcp6muxConnection : public PTask {
  public:
    Tcp6muxConnection (IoHandler *io, int id, Tcp6muxEnt *e, 
					   Tcp6muxConnSM *sm, Uint32 idletimeout=300);
  Tcp6muxConnection (IoHandler *io, Tcp6muxEnt *e, Tcp6muxConnSM *sm, Uint32 idletimeout=300);

    virtual ~Tcp6muxConnection (void);

    bool closed_Creq (Message *msg);
    bool closed_Conn (Message *msg);
    bool closed_default (Message *msg);

    bool wfSopen_Conn (Message *msg);
    bool wfSopen_Disconn (Message *msg);
    bool wfSopen_timeout (Message *msg);
    bool wfSopen_default (Message *msg);

    bool wfCresp_Cresp (Message *msg);
    bool wfCresp_Dreq (Message *msg);
    bool wfCresp_Disconn (Message *msg);
    bool wfCresp_timeout (Message *msg);
    bool wfCresp_default (Message *msg);

    bool open_Dtreq (Message *msg);
    bool open_Dreq (Message *msg);
    bool open_Data (Message *msg);
    bool open_Disconn (Message *msg);
    bool open_timeout (Message *msg);
    bool open_default (Message *msg);

    int getCep (void);

    Tcp6muxSAP :: Provider &up;

    DevIf :: User connection;

  private:

    Tcp6muxEnt *entity;
    Timer respTimer;
    Timer idleTimer;
    int cep;
    SockConn device;
    InetAddr6 myaddr;
    Uint16 reqici;
};

#endif  


