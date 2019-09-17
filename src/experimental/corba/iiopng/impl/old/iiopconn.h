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

$Log: iiopconn.h,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:22  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.12  2001/06/08 08:40:08  bilhanan
Full licence clause added into doors 0.1


*/
//
//  =========================================================================
//  File:     iiopconn.h
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================



#ifndef IIOPCONN_H
#define IIOPCONN_H


#define THIS "[" << getName() << "] "


#include <doors/ptb.h>
#include <doors/config.h>
#include <doors/sockmsg.h>
#include <doors/sockcos.h>

#include <doors/iiopdefs.h>
#include <doors/iiopsap.h>

#include <doors/giop1_0.h>
#include <doors/giop1_1.h>
#include <doors/giop1_2.h>

#include <doors/fragbuf.h>

class IiopEntity;
class IiopSm_1_0;
class IiopSm_1_1;
class IiopSm_1_2;



class IiopMsgBuffer 
{
public:
  IiopMsgBuffer(void);
  
  void Add(Frame &frame);
  Frame* ExtractMsg(void);
  Frame* Operate(Frame &frame);
  
protected:
  
  Frame buffer;
  Uint32 buflen;
  Uint8 swap;

};


class IiopConnection : public EventTask 
{
public:
  enum State { closed = 0, wfSopen, wfCresp, open };
  
  IiopConnection (Uint32 id, IiopEntity *e);
  IiopConnection (IiopEntity *e);
  
  virtual ~IiopConnection (void);
  OdBoolean execute (EventType type, Message *msg);
  
  OdBoolean default_handler (Message *msg); 
  
  OdBoolean closed_Creq (Message *msg);
  OdBoolean closed_OPEN (Message *msg);
  OdBoolean closed_default (Message *msg);
  
  OdBoolean wfSopen_OPEN (Message *msg);
  OdBoolean wfSopen_CLOSE (Message *msg);
  OdBoolean wfSopen_timeout (Message *msg);
  OdBoolean wfSopen_default (Message *msg);
  
  OdBoolean wfCresp_CLOSE (Message *msg);
  OdBoolean wfCresp_Cresp (Message *msg);
  OdBoolean wfCresp_Dreq (Message *msg);
  OdBoolean wfCresp_timeout (Message *msg);
  OdBoolean wfCresp_default (Message *msg);
  
  OdBoolean open_OPEN(Message *msg);
  OdBoolean open_CLOSE (Message *msg);
  OdBoolean open_Dreq (Message *msg);
  OdBoolean open_default (Message *msg);
  
  OdBoolean peerPutMessage(Frame *msg);

 
  Sint32 getCep (void);
  
  IiopSAP :: Provider &up;
  DevIf :: User connection;


  enum modes { MODE_UNDEFINED, MODE_CLIENT, MODE_SERVER, 
	       MODE_BIDIRCLIENT, MODE_BIDIRSERVER};
  Uint16 mode;


  class FragmentBuffer_1_1{
  public:
    Frame *msg;
  };

  FragmentBuffer_1_1 fragbuf_1_1;
  FragmentBuffer_1_2 fragbuf_1_2;
  
  const Uint8 byteorder;
  
   
private:
  
  IiopEntity *entity;          // Entity controlling multiple conn's

  SocketConnectionOS cdevice;  // Socket connection device
  Sint32 cep;                  // Connection End Point (conn id #)
  
  Timer respTimer;             
  
  IiopMsgBuffer buffer;
  
  int state;
  
  Sint16 state_1_0;
  Sint16 state_1_1;
  Sint16 state_1_2;
  
  int version;
  int iiopstate;
  
  IiopSm_1_0 *sm_1_0;
  IiopSm_1_1 *sm_1_1;
  IiopSm_1_2 *sm_1_2;

};


#endif  
