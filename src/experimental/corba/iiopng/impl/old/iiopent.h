//Editor-Info: -*- C++ -*-
//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
//
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

$Log: iiopent.h,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:23  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.8  2001/06/08 08:40:09  bilhanan
Full licence clause added into doors 0.1


*/
//
//  =========================================================================
//  File:     iiopent.h
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================



#ifndef IIOPENT_H
#define IIOPENT_H


#define THIS "[" << getName() << "] "


#include <doors/sockmsg.h>
#include <doors/sockeos.h>

#include <doors/iiopsap.h>
#include <doors/iiopsm1_0.h>
#include <doors/iiopsm1_1.h>
#include <doors/iiopsm1_2.h>

class IiopConnection;


class IiopEntity : public EventTask {
public:
  
  IiopEntity (const char *n, Address *listenaddr);
  virtual ~IiopEntity (void);
  
  //OdBoolean execute (EventType type, Message *msg);
  
  //  IiopConnection *getConnection (Uint32 i);

  void createConnection (Uint16 reqid);

  Uint8 ByteOrder(void); 
  
  IiopPeer peer;
  //  IiopSAP :: Provider up;
  //  DevIf :: User entity;
  TranSAP :: User down;
  
  IiopSm_1_0 sm_1_0;
  IiopSm_1_1 sm_1_1;
  IiopSm_1_2 sm_1_2;
  
private:
  
  SocketEntityOS edevice;
  
  //  connections is port.
  Port connections;
  // connMux is the multiplexer.
  PortMux connMux;
  //  IiopConnection *connections [256];
};


#endif  // IIOPENT_H

