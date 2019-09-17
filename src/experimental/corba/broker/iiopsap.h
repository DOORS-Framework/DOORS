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

$Log: iiopsap.h,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:37  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:40:07  bilhanan
Full licence clause added into doors 0.1


*/
//
//  =========================================================================
//  File:     iiopsap.h
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     This file declares classes of IiopSAP
//            service access point
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================


#ifndef _iiopsap_h_
#define _iiopsap_h_


#include <doors/ptb.h>
#include <doors/giop.h>


class IiopSAP {
 public:
  
  class User : public ReqIface {
  public:
    User (EventTask *t, Peer *p = 0);
    ~User (void);
  };
  
  class Provider : public IndIface {
  public:
    Provider (EventTask *t);
    ~Provider (void);
  };
  
  enum ReqType { upRequest = 1, upReply, upCancelRequest, 
		 upLocateRequest, upLocateReply, upCloseConnection, 
		 upFragment, creq, cresp, dreq };
  
   
  class Req : public Message {
  public:
    Req (void);
    ~Req (void);
    
    Uint16 cid;
  };
  

  class UpRequest : public Req {
  public:
    UpRequest (void);
    UpRequest (const UpRequest &msg);
    ~UpRequest (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    CORBA::Octet version;
    Giop::RequestHeader *reqhdr;
    CORBA::ULong firstParamSize;
    Frame body;
  };
  

  class UpReply : public Req {
  public:
    UpReply (void);
    UpReply (const UpReply &msg);
    ~UpReply (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    CORBA::Octet version;
    Giop::ReplyHeader *rephdr;
    CORBA::ULong firstParamSize;
    Frame body;
  };


  class UpCancelRequest : public Req {
  public:
    UpCancelRequest (void);
    UpCancelRequest (const UpCancelRequest &msg);
    ~UpCancelRequest (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    CORBA::Octet version;
    Giop::CancelRequestHeader *hdr;
  };
  

  class UpLocateRequest : public Req {
  public:
    UpLocateRequest (void);
    UpLocateRequest (const UpLocateRequest &msg);
    ~UpLocateRequest (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    CORBA::Octet version;
    Giop::LocateRequestHeader *reqhdr;
  };
  

  class UpLocateReply : public Req {
  public:
    UpLocateReply (void);
    UpLocateReply (const UpLocateReply &msg);
    ~UpLocateReply (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    CORBA::Octet version;
    Giop::LocateReplyHeader *rephdr;
    CORBA::ULong firstParamSize;
    Frame body;
  };
  
  
  class UpCloseConnection : public Req {
  public:
    UpCloseConnection (void);
    UpCloseConnection (const UpCloseConnection &msg);
    ~UpCloseConnection (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    CORBA::Octet version;
  };
  
  
  class UpFragment : public Req {
  public:
    UpFragment (void);
    UpFragment (const UpFragment &msg);
    ~UpFragment (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    CORBA::Octet version;
    CORBA::Octet moreflag;
    CORBA::ULong request_id;
    CORBA::ULong firstParamSize;
    Frame body;
  };
  
  
  class Creq : public Req {
  public:
    Creq (void);
    Creq (const Creq &msg);
    ~Creq (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    InetAddr destination;
  };
  
  
  class Cresp : public Req {
  public:
    Cresp (void);
    Cresp (const Cresp &msg);
    ~Cresp (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    CORBA::UShort accept;
  };
  
   
  class Dreq : public Req {
  public:
    Dreq (void);
    Dreq (const Dreq &msg);
    ~Dreq (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    Uint32 reason;
  };
  
  enum IndType { request = 1, reply, cancelRequest, locateRequest, 
		 locateReply, closeConnection, messageError, 
		 cind, cconf, dind };

  
  class Ind : public Message {
  public:
    Ind (void);
    ~Ind (void);
    
    Uint16 cid;
  };
  
   
  class Request : public Ind {
  public:
    Request (void);
    Request (const Request &msg);
    ~Request (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    CORBA::Octet version;
    Giop::RequestHeader *reqhdr;
    CORBA::ULong body_offset;
    Frame body;
  };
  
  
  class Reply : public Ind {
  public:
    Reply (void);
    Reply (const Reply &msg);
    ~Reply (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    CORBA::Octet version;
    Giop::ReplyHeader *rephdr;
    CORBA::ULong body_offset;
    Frame body;
  };
  
  
  class CancelRequest : public Ind {
  public:
    CancelRequest (void);
    CancelRequest (const CancelRequest &msg);
    ~CancelRequest (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    CORBA::Octet version;
    Giop::CancelRequestHeader *hdr;
  };
  

  class LocateRequest : public Ind {
  public:
    LocateRequest (void);
    LocateRequest (const LocateRequest &msg);
    ~LocateRequest (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    CORBA::Octet version;
    Giop::LocateRequestHeader *reqhdr;
  };

  
  class LocateReply : public Ind {
  public:
    LocateReply (void);
    LocateReply (const LocateReply &msg);
    ~LocateReply (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    CORBA::Octet version;
        Giop::LocateReplyHeader *rephdr;
        CORBA::ULong body_offset;
        Frame body;
  };
  
   
  class CloseConnection : public Ind {
  public:
    CloseConnection (void);
    CloseConnection (const CloseConnection &msg);
    ~CloseConnection (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    CORBA::Octet version;
  };
  

  class MessageError : public Ind {
  public:
    MessageError (void);
    MessageError (const MessageError &msg);
    ~MessageError (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;

    CORBA::Octet version;
    Frame info;
    };

    
  class Cind : public Ind {
  public:
    Cind (void);
    Cind (const Cind &msg);
    ~Cind (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    InetAddr source;
  };
  
  
  class Cconf : public Ind {
  public:
    Cconf (void);
    Cconf (const Cconf &msg);
    ~Cconf (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;
    
    InetAddr source;
  };
  
  
  class Dind : public Ind {
  public:
    Dind (void);
    Dind (const Dind &msg);
    ~Dind (void);
    
    Message *clone (void) const;
    
    MessageType getType (void) const;

    Uint16 serviceId;
    Frame data;
  };
  
 protected:
 private:
};


inline MessageType IiopSAP :: UpRequest :: getType (void) const 
{
  return upRequest;
}


inline MessageType IiopSAP :: UpReply :: getType (void) const 
{
  return upReply;
}


inline MessageType IiopSAP :: UpCancelRequest :: getType (void) const 
{
  return upCancelRequest;
}


inline MessageType IiopSAP :: UpLocateRequest :: getType (void) const 
{
  return upLocateRequest;
}


inline MessageType IiopSAP :: UpLocateReply :: getType (void) const 
{
  return upLocateReply;
}


inline MessageType IiopSAP :: UpCloseConnection :: getType (void) const 
{
  return upCloseConnection;
}


inline MessageType IiopSAP :: UpFragment :: getType (void) const 
{
  return upFragment;
}


inline MessageType IiopSAP :: Creq :: getType (void) const 
{
  return creq;
}


inline MessageType IiopSAP :: Cresp :: getType (void) const 
{
  return cresp;
}


inline MessageType IiopSAP :: Dreq :: getType (void) const 
{
  return dreq;
}


inline MessageType IiopSAP :: Request :: getType (void) const 
{
  return request;
}


inline MessageType IiopSAP :: Reply :: getType (void) const 
{
  return reply;
}


inline MessageType IiopSAP :: CancelRequest :: getType (void) const 
{
  return cancelRequest;
}


inline MessageType IiopSAP :: LocateRequest :: getType (void) const 
{
  return locateRequest;
}


inline MessageType IiopSAP :: LocateReply :: getType (void) const 
{
  return locateReply;
}


inline MessageType IiopSAP :: CloseConnection :: getType (void) const 
{
  return closeConnection;
}


inline MessageType IiopSAP :: MessageError :: getType (void) const 
{
  return messageError;
}


inline MessageType IiopSAP :: Cind :: getType (void) const 
{
  return cind;
}


inline MessageType IiopSAP :: Cconf :: getType (void) const 
{
  return cconf;
}


inline MessageType IiopSAP :: Dind :: getType (void) const 
{
  return dind;
}

#endif
