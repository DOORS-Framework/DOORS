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


//  $Log: tcpmuxconn.h,v $
//  Revision 1.2  2005/02/21 13:59:20  bilhanan
//  development
//
//  Revision 1.1  2004/10/12 12:01:22  bilhanan
//  The protocol formerly known as tranmux
//
//  Revision 1.7  2003/05/15 22:17:05  ik
//  Wrong config header included. Fixed.
//
//  Revision 1.6  2002/08/16 09:55:57  hartmanj
//  Added idletimeout to constructor
//
//  Revision 1.5  2002/08/15 08:34:14  hartmanj
//  ONS taken away
//
//  Revision 1.4  2002/08/08 13:42:04  hartmanj
//  development
//
//  Revision 1.3  2002/07/23 16:38:34  ik
//  Od{Boolean,True,False} -> {bool,true,false}
//
//  Revision 1.2  2002/07/23 07:30:44  bilhanan
//  licence added, include headers getting fixed..
//


#ifndef TCPMUXCONN_H
#define TCPMUXCONN_H

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <doors/ptb.h>
#include <doors/tcpmux.h>
#include <doors/sockmsg.h>
#include <doors/sockconn.h>
#include <doors/tcpmuxent.h>

class TcpmuxEntity;

class TcpmuxConnSM;

//
//Class: TcpmuxConnection
//
//Inherited: PTask
//
//Description:
// TcpmuxConnection is inherited from PTask. It has a reference of Tcpmux 
// SAP provider interface from TranEntity which allows connection to 
// send indications directly to service user. It also has one 
// SocketConnection instance which is used for actual data transfer.

class TcpmuxConnection : public PTask {
  public:
    TcpmuxConnection (IoHandler *io, int id, TcpmuxEntity *e, 
		      TcpmuxConnSM *sm);

    virtual ~TcpmuxConnection (void);

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

    TcpmuxSAP :: Provider &up;


    DevIf :: User connection;

    void increment_muxcount (void) {
    muxcount++;
    }

    int get_cep (void) {
    return cep;
    }

    InetAddr myaddr;

  private:

    TcpmuxEntity *entity;
    Timer idleTimer;
    Timer respTimer;
    int cep;
    SockConn device;
    Uint16 reqici;
    int muxcount;
};

#endif  


