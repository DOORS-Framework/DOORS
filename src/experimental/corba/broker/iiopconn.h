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
Revision 1.2  2002/07/23 14:32:47  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/05/13 14:38:35  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:36  bilhanan
Full Licence clause into DOORS 0.2

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
  bool execute (EventType type, Message *msg);
  
  bool default_handler (Message *msg); 
  
  bool closed_Creq (Message *msg);
  bool closed_OPEN (Message *msg);
  bool closed_default (Message *msg);
  
  bool wfSopen_OPEN (Message *msg);
  bool wfSopen_CLOSE (Message *msg);
  bool wfSopen_timeout (Message *msg);
  bool wfSopen_default (Message *msg);
  
  bool wfCresp_CLOSE (Message *msg);
  bool wfCresp_Cresp (Message *msg);
  bool wfCresp_Dreq (Message *msg);
  bool wfCresp_timeout (Message *msg);
  bool wfCresp_default (Message *msg);
  
  bool open_OPEN(Message *msg);
  bool open_CLOSE (Message *msg);
  bool open_Dreq (Message *msg);
  bool open_default (Message *msg);
  
  bool peerPutMessage(Frame *msg);

 
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
