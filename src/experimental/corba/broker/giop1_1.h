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

$Log: giop1_1.h,v $
Revision 1.1.1.1  2002/05/13 14:38:35  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:32  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.7  2001/06/08 08:40:14  bilhanan
Full licence clause added into doors 0.1


*/

//===========================================================================
//File:     giop1_1.h
//Date:     27.09.1999
//Subject:  IIOP ver 1.2
//Desc:   
//
//Author:   Janne Makihonka, TTKK/DMI
//===========================================================================

#ifndef GIOP1_1_H
#define GIOP1_1_H

#include <doors/giop1_0.h>

 
class Giop_1_1 : public Giop_1_0 {
public:
  
  //======================================================================
  //Giop_1_1::MessageHeader
  //======================================================================

  enum MsgType {
    request, 
    reply, 
    cancelRequest, 
    locateRequest, 
    locateReply, 
    closeConnection, 
    messageError, 
    fragment     
  }; 

  struct MessageHeader { 
    char magic [4]; 
    Version GIOP_version; 
    CORBA::Octet flags; 
    CORBA::Octet message_type; 
    CORBA::ULong message_size; 
  };

  inline void 
  EncodeMessageHeader( const MessageHeader &from,
			Frame &tofrm, Uint32 &len) {
    DEBUGINFO("Giop_1_1::EncodeMessageHeader");
    IiopCodec :: Encode( (Byte *)&from, tofrm, 12, len );
  };
  
  inline void DecodeMessageHeader (MessageHeader &to, Frame &fromfrm, Uint32 &len){ 
    DEBUGINFO("Giop_1_1::DecodeMessageHeader");
    Sint32 frlen = fromfrm.length();
    if( frlen >= 12 ){ 
      IiopCodec :: Decode( (Byte *)&to, fromfrm, 0, 12, len );
      return;
    }
    ERROR( "Giop_1_1::DecodeMessageHeader, Frame length " << frlen << ", " 
	   << "while it should be at least " << 12 << "." );
  }; 

  
  //======================================================================
  //Giop_1_1::RequestHeader
  //======================================================================
  
  struct RequestHeader {     
    IOP::ServiceContextList service_context; 
    CORBA::ULong request_id; 
    CORBA::Boolean response_expected; 
    CORBA::Octet reserved[3]; 
    CORBA::Sequence <CORBA::Octet> object_key; 
    CORBA::iiopString operation; 
    CORBA_Principal requesting_principal; 
  };

  inline void 
  EncodeRequestHeader( const RequestHeader &from, Frame &tofrm, Uint32 &len) {
    DEBUGINFO("Giop_1_1::EncodeRequestHeader");
    IOP :: EncodeSequenceServiceContext( from.service_context, tofrm, len);
    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: Encode( (Byte) from.response_expected, tofrm, len );
    IiopCodec :: EncodeSequence( from.object_key, tofrm, len );
    IiopCodec :: EncodeString( from.operation, tofrm, len );
    IiopCodec :: EncodePrincipal( from.requesting_principal, tofrm, len );
  };
  
  inline void 
  DecodeRequestHeader( RequestHeader &to, Frame &fromfrm, bool swap, Uint32 &len) {
    DEBUGINFO("Giop_1_1::DecodeRequestHeader");
    Sint32 frlen = fromfrm.length() + len;
    IOP :: DecodeSequenceServiceContext( to.service_context, fromfrm, swap, len);
    if((frlen - len) >= 20){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
      Byte tmp = 0;
      IiopCodec :: Decode( tmp, fromfrm, swap, len);
      to.response_expected = tmp; 
      IiopCodec :: DecodeSequence( to.object_key, fromfrm,  swap, len);
      IiopCodec :: DecodeString( to.operation, fromfrm, swap, len );
      IiopCodec :: DecodePrincipal( to.requesting_principal, fromfrm, swap, len ); 
      return;
    }
    ERROR( "Giop_1_1::DecodeRequestHeader, Frame length " << frlen << ", " 
	   << "while it should be at least " << 20 << "." );
  };


  //======================================================================
  //======================================================================

  class IiopMsg {
  public:
    IiopMsg(void){};
    IiopMsg(CORBA::Octet mestype, CORBA::Octet byteord = 0,
	    CORBA::Octet moreflag = 0  ){
      memcpy(&(msghdr.magic[0]), "GIOP",4);
      msghdr.GIOP_version.major = 1;
      msghdr.GIOP_version.minor = 1;
      msghdr.flags = (Uint8)1 & byteord;
      msghdr.flags += (Uint8)2 & moreflag;
      msghdr.message_type = mestype;
      msghdr.message_size = 0;
    };

    MessageHeader msghdr;
    Uint32 firstParamSize;
    Uint32 body_offset;

    Uint8 morebit(){
      return (Uint8)2 & msghdr.flags;
    }
  };
  
  class Request : public IiopMsg {
  public:
    Request(void){};
    Request(CORBA::Octet byteord, CORBA::Octet moreflag = 0) : 
      IiopMsg((CORBA::Octet)request, byteord, moreflag){
    };
    
    RequestHeader reqhdr;
    Frame body;
  };

 
  class Reply : public IiopMsg {
  public:
    Reply(void){};
    Reply(CORBA::Octet byteord, CORBA::Octet moreflag = 0) : 
      IiopMsg((CORBA::Octet)reply, byteord, moreflag){
    };

    ReplyHeader rephdr;
    Frame body;
  };
  
  class CancelRequest : public IiopMsg {
  public:
    CancelRequest(void){};
    CancelRequest(CORBA::Octet byteord) : IiopMsg((CORBA::Octet)cancelRequest, 
						  byteord){
    };

    CancelRequestHeader reqhdr;
  };
  
  class LocateRequest : public IiopMsg {
  public:
    LocateRequest(void){};
    LocateRequest(CORBA::Octet byteord) : IiopMsg((CORBA::Octet)locateRequest,
						  byteord){
    };
    LocateRequestHeader reqhdr;
  };
  
  class LocateReply : public IiopMsg {
  public: 
    LocateReply(void){};
    LocateReply(CORBA::Octet byteord) : IiopMsg((CORBA::Octet)locateReply, 
						byteord){
    };

    LocateReplyHeader rephdr;
    Frame body;
  };
  
  class CloseConnection : public IiopMsg {
  public:
    CloseConnection(void){};
    CloseConnection(CORBA::Octet byteord) : 
      IiopMsg((CORBA::Octet)closeConnection, byteord){
    };
  };
  
  class MessageError : public IiopMsg {
  public:
    MessageError(void){};
    MessageError(CORBA::Octet byteord) : 
      IiopMsg((CORBA::Octet)messageError, byteord){
    };
  };

  class Fragment : public IiopMsg {
  public:
    Fragment(void){};
    Fragment(CORBA::Octet byteord, CORBA::Octet moreflag) : 
      IiopMsg((CORBA::Octet)fragment, byteord, moreflag){
    };
    
    Frame body;
  };
};

#endif;
