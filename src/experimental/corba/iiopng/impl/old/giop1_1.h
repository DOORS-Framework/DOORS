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
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:21  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.7  2001/06/08 08:40:14  bilhanan
Full licence clause added into doors 0.1


*/


// Original Author:   Janne Makihonka


#ifndef GIOP1_1_H
#define GIOP1_1_H

#include <doors/giop.h>

// Contains encoding and decoding functions 
// for the GIOP 1.1 data structs in giop.h
// Also contains the base class for GIOP 1.1 PDUs
 
namespace GIOP {

  
  //======================================================================
  //Giop_1_1::MessageHeader
  //======================================================================

  inline void EncodeMessageHeader_1_1
  (const MessageHeader_1_1 &from, Frame &tofrm, Uint32 &len) {
    IiopCodec :: Encode( (Byte *)&from, tofrm, 12, len );
  };
  
  inline void DecodeMessageHeader_1_1 
  (MessageHeader_1_1 &to, Frame &fromfrm, Uint32 &len){ 
  
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
  
  inline void EncodeRequestHeader_1_1
  (const RequestHeader_1_1 &from, Frame &tofrm, Uint32 &len) {
   
    IOP :: EncodeSequenceServiceContext( from.service_context, tofrm, len);
    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: Encode( (Byte) from.response_expected, tofrm, len );
    IiopCodec :: EncodeSequence( from.object_key, tofrm, len );
    IiopCodec :: EncodeString( from.operation, tofrm, len );
    IiopCodec :: EncodePrincipal( from.requesting_principal, tofrm, len );
  };
  
  inline void DecodeRequestHeader_1_1
  (RequestHeader_1_1 &to, Frame &fromfrm, bool swap, Uint32 &len) {

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
  //Giop_1_1::ReplyHeader
  //======================================================================
  inline void EncodeReplyHeader_1_1
  (const ReplyHeader_1_1 &from, Frame &tofrm, Uint32 &len) {
    
    IOP :: EncodeSequenceServiceContext( from.service_context, tofrm, len);
    IiopCodec :: Encode (from.request_id, tofrm, len );
    IiopCodec :: Encode((Uint32)from.reply_status, tofrm, len );
  };
  
  inline void DecodeReplyHeader_1_1
  (ReplyHeader_1_1 &to, Frame &fromfrm, bool swap, Uint32 &len){

    Sint32 frlen = fromfrm.length() + len;
    IOP :: DecodeSequenceServiceContext (to.service_context, fromfrm, swap, len);
    if((frlen - len) >= 8){
      IiopCodec :: Decode (to.request_id, fromfrm, swap, len);
      Uint32 *tmp = (Uint32 *) &to.reply_status;
      IiopCodec :: Decode (*tmp, fromfrm, swap, len);
    } 
      ERROR( "Giop_1_1::DecodeReplyHeader, Frame length " << frlen << ", " 
	     << "while it should be at least " << 8 << "." );
  };


  //======================================================================
  //Giop_1_1::SystemExceptionReplyBody 
  //======================================================================

  inline void EncodeSystemExceptionReplyBody_1_1
  (const GIOP::SystemExceptionReplyBody& repbody, Frame &tofrm, Uint32 &len) {

    IiopCodec :: EncodeString( repbody.exception_id, tofrm, len );
    IiopCodec :: Encode( repbody.minor_code_value, tofrm, len );
    IiopCodec :: Encode( repbody.completion_status, tofrm, len );
  };
  
  inline void DecodeSystemExceptionReplyBody_1_1
  (GIOP::SystemExceptionReplyBody& repbody, Frame &fromfrm, bool swap, Uint32 &len) {

    IiopCodec :: DecodeString( repbody.exception_id, fromfrm, swap, len);
    IiopCodec :: Decode( repbody.minor_code_value, fromfrm, swap, len);
    IiopCodec :: Decode( repbody.completion_status, fromfrm, swap, len);
  };
  

  //======================================================================
  //Giop_1_1::CancelRequestHeader
  //======================================================================
  inline void EncodeCancelRequestHeader_1_1
  (const CancelRequestHeader &from, Frame &tofrm, Uint32 &len) {
    IiopCodec :: Encode( from.request_id, tofrm, len );
  };

  inline void DecodeCancelRequestHeader_1_1
  (CancelRequestHeader &to, Frame &fromfrm, bool swap, Uint32 &len) {  
    Sint32 frlen = fromfrm.length();
    if(frlen  >= 4){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
    } 
      ERROR( "Giop_1_1:: DecodeCancelRequestHeader, Frame length " << frlen 
	     << ", " 
	     << "while it should be at least " << 4 << "." );
  };




  //======================================================================
  //Giop_1_1::LocateRequestHeader
  //======================================================================
   inline void EncodeLocateRequestHeader_1_1
  (const LocateRequestHeader_1_1 &from, Frame &tofrm, Uint32 &len) {

    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: EncodeSequence( from.object_key, tofrm, len );
  };

  inline void DecodeLocateRequestHeader_1_1
  (LocateRequestHeader_1_1 &to, Frame &fromfrm, bool swap, Uint32 &len) {  

    Sint32 frlen = fromfrm.length();
    if(frlen  >= 8){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
      IiopCodec :: DecodeSequence( to.object_key, fromfrm, swap, len ); 
    }
    ERROR( "Giop_1_1::DecodeLocateRequestHeader, Frame length " << frlen 
	   << ", " 
	   << "while it should be at least " << 8 << "." );
  };

  
  //======================================================================
  //Giop_1_1::LocateReplyHeader
  //======================================================================
  
  inline void EncodeLocateReplyHeader_1_1
  (const LocateReplyHeader_1_1 &from, Frame &tofrm, Uint32 &len) {

    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: Encode( (Uint32)from.locate_status, tofrm, len );
  };

  inline void DecodeLocateReplyHeader_1_1
  (LocateReplyHeader_1_1 &to, Frame &fromfrm, bool swap, Uint32 &len) {  

    Sint32 frlen = fromfrm.length();
    if(frlen  >= 8){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
      Uint32 *tmp = (Uint32 *)&to.locate_status;
      IiopCodec :: Decode( *tmp, fromfrm, swap, len ); 
    }
      ERROR( "Giop_1_1::DecodeLocateReplyHeader, Frame length " << frlen 
	     << ", " 
	     << "while it should be at least " << 8 << "." );
  };
  

  // Giop 1.1 Msg base class
  // This class is inherited by all other GIOP 1.1 PDU classes. 
  // It contains only the message header structure 
  // which is appropriately initialised.


  class GIOP_1_1_Msg {
  public:
    GIOP_1_1_Msg (void){};
    GIOP_1_1_Msg (CORBA::Octet mestype, CORBA::Octet byteord = 0,
	    CORBA::Octet moreflag = 0  ){
      memcpy(&(msghdr.magic[0]), "GIOP",4);
      msghdr.GIOP_version.major = 1;
      msghdr.GIOP_version.minor = 1;
      msghdr.flags = (Uint8)1 & byteord;
      msghdr.flags += (Uint8)2 & moreflag;
      msghdr.message_type = mestype;
      msghdr.message_size = 0;
    };

    MessageHeader_1_1 msghdr;
    Uint32 firstParamSize;
    Uint32 body_offset;

    Uint8 morebit(){
      return (Uint8)2 & msghdr.flags;
    }
  };
  
#if 0 // 
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

#endif // end of commented out block
#endif;
