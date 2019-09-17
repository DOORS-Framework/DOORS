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

$Log: giop1_0.h,v $
Revision 1.1.1.1  2002/05/13 14:38:35  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:32  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.9  2001/06/08 08:40:12  bilhanan
Full licence clause added into doors 0.1


*/
//
//  =========================================================================
//  File:     giop1_0.h
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     Giop 1.0 headers and their encoding/decoding functions.
//            Giop 1.0 message definitions.
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================



#ifndef GIOP1_0_H
#define GIOP1_0_H


#include <doors/bascodec.h>
#include <doors/iop.h>
#include <doors/giop.h>



//======================================================================
// class Giop_1_0
//======================================================================

class Giop_1_0 : public Giop { 
public:

  //======================================================================
  //Giop_1_0::MessageHeader
  //======================================================================

  enum MsgType {    
    request, 
    reply, 
    cancelRequest, 
    locateRequest, 
    locateReply, 
    closeConnection, 
    messageError 
  };

  struct MessageHeader {
    char magic [4];
    Version GIOP_version;
    CORBA::Boolean byte_order;
    CORBA::Octet message_type;
    CORBA::ULong message_size;
  };

  inline void 
  EncodeMessageHeader( const MessageHeader &from,
			Frame &tofrm, Uint32 &len) {
    DEBUGINFO("Giop_1_0::EncodeMessageHeader");
    IiopCodec :: Encode( (Byte *)&from, tofrm, 12, len );
  };
  
  inline void DecodeMessageHeader (MessageHeader &to, Frame &fromfrm, 
				   Uint32 &len){ 
    DEBUGINFO("Giop_1_0::DecodeMessageHeader");
    Sint32 frlen = fromfrm.length();
    if( frlen >= 12 ){ 
      IiopCodec :: Decode( (Byte *)&to, fromfrm, 0, 12, len );
       return;
    }
    ERROR( "Giop_1_0::DecodeMessageHeader, Frame length " << frlen << ", " 
	   << "while it should be at least " << 12 << "." );
  };
  

  //======================================================================
  //Giop_1_0::RequestHeader
  //======================================================================
  
  struct RequestHeader {
    IOP::ServiceContextList service_context;
    CORBA::ULong request_id;
    CORBA::Boolean response_expected;
    CORBA::Sequence <CORBA::Octet> object_key;
    CORBA::iiopString operation;
    CORBA_Principal requesting_principal;
  };

  inline void 
  EncodeRequestHeader( const RequestHeader &from, Frame &tofrm, Uint32 &len) {
    DEBUGINFO("Giop_1_0::EncodeRequestHeader");
    IOP :: EncodeSequenceServiceContext( from.service_context, tofrm, len);
    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: Encode( from.response_expected, tofrm, len );
    IiopCodec :: EncodeSequence( from.object_key, tofrm, len );
    IiopCodec :: EncodeString( from.operation, tofrm, len );
    IiopCodec :: EncodePrincipal( from.requesting_principal, tofrm, len );
  };
  
  inline void 
  DecodeRequestHeader( RequestHeader &to, Frame &fromfrm, bool swap, 
		       Uint32 &len) {  
    DEBUGINFO("Giop_1_0::DecodeRequestHeader");
    Sint32 frlen = fromfrm.length() + len;
    IOP :: DecodeSequenceServiceContext( to.service_context, fromfrm, 
					 swap, len);
    if((frlen - len) >= 20){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
      IiopCodec :: Decode( to.response_expected, fromfrm, swap, len);
      IiopCodec :: DecodeSequence( to.object_key, fromfrm,  swap, len);
      IiopCodec :: DecodeString( to.operation, fromfrm, swap, len );
      IiopCodec :: DecodePrincipal( to.requesting_principal, fromfrm, 
				    swap, len ); 
      return;
    }
    ERROR( "Giop_1_0::DecodeRequestHeader, Frame length " << frlen << ", " 
	   << "while it should be at least " << 20 << "." );
  };


  //======================================================================
  //Giop_1_0::ReplyHeader
  //======================================================================

  enum ReplyStatusType {
    NO_EXCEPTION,
    USER_EXCEPTION,
    SYSTEM_EXCEPTION,
    LOCATION_FORWARD
  };

  struct ReplyHeader {
    IOP::ServiceContextList service_context;
    CORBA::ULong request_id;
    ReplyStatusType reply_status;
  };

  inline void 
  EncodeReplyHeader( const ReplyHeader &from, Frame &tofrm, Uint32 &len) {
    DEBUGINFO("Giop_1_0::EncodeReplyHeader");
    IOP :: EncodeSequenceServiceContext( from.service_context, tofrm, len);
    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: Encode( (Uint32)from.reply_status, tofrm, len );
  };

  inline void 
  DecodeReplyHeader( ReplyHeader &to, Frame &fromfrm, bool swap, Uint32 &len){
    DEBUGINFO("Giop_1_0::DecodeReplyHeader");
    Sint32 frlen = fromfrm.length() + len;
    IOP :: DecodeSequenceServiceContext( to.service_context, fromfrm, 
					 swap, len);
    if((frlen - len) >= 8){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
      Uint32 *tmp = (Uint32 *)&to.reply_status;
      IiopCodec :: Decode( *tmp, fromfrm, swap, len);
    }
    ERROR( "Giop_1_0::DecodeReplyHeader, Frame length " << frlen << ", " 
	   << "while it should be at least " << 8 << "." );
  };


  //======================================================================
  //Giop_1_0::SystemExceptionReplyBody
  //======================================================================

  class SystemExceptionReplyBody {
  public:
    SystemExceptionReplyBody() : minor_code_value(0), completion_status(0) { 
    };
    
    SystemExceptionReplyBody( CORBA::iiopString &exc, CORBA::ULong min,
			      CORBA::ULong comp ) 
      : exception_id(), minor_code_value(min), completion_status(comp){
      exception_id = exc;
    };

    CORBA::iiopString exception_id;
    CORBA::ULong minor_code_value;
    CORBA::ULong completion_status;
  };

  inline void 
  EncodeSysExRepBody(const Giop_1_0::SystemExceptionReplyBody& repbody, 
		     Frame &tofrm, Uint32 &len) {
    DEBUGINFO("Giop_1_0::EncodeSysExRepBody");
    IiopCodec :: EncodeString( repbody.exception_id, tofrm, len );
    IiopCodec :: Encode( repbody.minor_code_value, tofrm, len );
    IiopCodec :: Encode( repbody.completion_status, tofrm, len );
  };
  
  inline void 
  DecodeSysExRepBody(Giop_1_0::SystemExceptionReplyBody& repbody, 
		     Frame &fromfrm, bool swap, Uint32 &len) {
    DEBUGINFO("Giop_1_0::DecodeSysExRepBody");
    IiopCodec :: DecodeString( repbody.exception_id, fromfrm, swap, len);
    IiopCodec :: Decode( repbody.minor_code_value, fromfrm, swap, len);
    IiopCodec :: Decode( repbody.completion_status, fromfrm, swap, len);
  };
  

  //======================================================================
  //Giop_1_0::CancelRequestHeader
  //======================================================================

  struct CancelRequestHeader {
    CORBA::ULong request_id;
  };

  
  inline void 
  EncodeCancelRequestHeader( const CancelRequestHeader &from, Frame &tofrm, 
			     Uint32 &len) {
    DEBUGINFO("Giop_1_0::EncodeCancelRequestHeader");
    IiopCodec :: Encode( from.request_id, tofrm, len );
  };

  inline void 
  DecodeCancelRequestHeader( CancelRequestHeader &to, Frame &fromfrm, 
			     bool swap, Uint32 &len) {  
    DEBUGINFO("Giop_1_0::DecodeCancelRequestHeader");
    Sint32 frlen = fromfrm.length();
    if(frlen  >= 4){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
    }
    ERROR( "Giop_1_0:: DecodeCancelRequestHeader, Frame length " << frlen 
	   << ", " 
	   << "while it should be at least " << 4 << "." );
  };


  //======================================================================
  //Giop_1_0::LocateRequestHeader
  //======================================================================
  
  struct LocateRequestHeader {
    CORBA::ULong request_id;
    CORBA::Sequence <CORBA::Octet> object_key;
  };
  
  inline void 
  EncodeLocateRequestHeader( const LocateRequestHeader &from, Frame &tofrm, 
			     Uint32 &len) {
    DEBUGINFO("Giop_1_0::EncodeLocateRequestHeader");
    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: EncodeSequence( from.object_key, tofrm, len );
  };

  inline void 
  DecodeLocateRequestHeader( LocateRequestHeader &to, Frame &fromfrm, 
			     bool swap, Uint32 &len) {  
    DEBUGINFO("Giop_1_0::DecodeLocateRequestHeader");
    Sint32 frlen = fromfrm.length();
    if(frlen  >= 8){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
      IiopCodec :: DecodeSequence( to.object_key, fromfrm, swap, len ); 
    }
    ERROR( "Giop_1_0::DecodeLocateRequestHeader, Frame length " << frlen 
	   << ", " 
	   << "while it should be at least " << 8 << "." );
  };


  //======================================================================
  //Giop_1_0::LocateReplyHeader
  //======================================================================
  
  enum LocateStatusType {
    UNKNOWN_OBJECT,
    OBJECT_HERE,
    OBJECT_FORWARD
  };
  
  struct LocateReplyHeader {
    CORBA::ULong request_id;
    LocateStatusType locate_status;
  };

  inline void 
  EncodeLocateReplyHeader( const LocateReplyHeader &from, Frame &tofrm, 
			     Uint32 &len) {
    DEBUGINFO("Giop_1_0::EncodeLocateReplyHeader");
    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: Encode( (Uint32)from.locate_status, tofrm, len );
  };

  inline void 
  DecodeLocateReplyHeader( LocateReplyHeader &to, Frame &fromfrm, 
			     bool swap, Uint32 &len) {  
    DEBUGINFO("Giop_1_0::DecodeLocateReplyHeader");
    Sint32 frlen = fromfrm.length();
    if(frlen  >= 8){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
      Uint32 *tmp = (Uint32 *)&to.locate_status;
      IiopCodec :: Decode( *tmp, fromfrm, swap, len ); 
    }
    ERROR( "Giop_1_0::DecodeLocateReplyHeader, Frame length " << frlen 
	   << ", " 
	   << "while it should be at least " << 8 << "." );
  };
  
  //======================================================================

  //======================================================================
  //Giop_1_0::IiopMsg
  //======================================================================
  
  class IiopMsg : public Giop::IiopMsg{
  public:
    IiopMsg(void){};
    IiopMsg(CORBA::Octet mestype, CORBA::Octet byteord = 0){
      memcpy(&(msghdr.magic[0]), "GIOP",4);
      msghdr.GIOP_version.major = 1;
      msghdr.GIOP_version.minor = 0;
      msghdr.byte_order = byteord;
      msghdr.message_type = mestype;
      msghdr.message_size = 0;
    }

    MessageHeader msghdr;
    Uint32 firstParamSize;
    Uint32 body_offset;
  };

  
  //======================================================================
  //Giop_1_0::Request
  //======================================================================
  
  class Request : public IiopMsg {
  public:
    Request(void){};
    Request(CORBA::Octet byteord) : IiopMsg((CORBA::Octet)request, byteord){
    };
    
    RequestHeader reqhdr;
     Frame body;
  };

  //======================================================================
  //Giop_1_0::Reply
  //======================================================================
  
  class Reply : public IiopMsg {
  public:
    Reply(void){};
    Reply(CORBA::Octet byteord) : IiopMsg((CORBA::Octet)reply, byteord){
    };

    ReplyHeader rephdr;
    Frame body;
  };
  
  //======================================================================
  //Giop_1_0::CancelRequest
  //======================================================================
  
  class CancelRequest : public IiopMsg {
  public:
    CancelRequest(void){};
    CancelRequest(CORBA::Octet byteord) : IiopMsg((CORBA::Octet)cancelRequest, 
						  byteord){
    };

    CancelRequestHeader reqhdr;
  };
  
  //======================================================================
  //Giop_1_0::LocateRequest
  //======================================================================
  
  class LocateRequest : public IiopMsg {
  public:
    LocateRequest(void){};
    LocateRequest(CORBA::Octet byteord) : IiopMsg((CORBA::Octet)locateRequest,
						  byteord){
    };
    LocateRequestHeader reqhdr;
  };
  
  //======================================================================
  //Giop_1_0::LocateReply
  //======================================================================
  
  class LocateReply : public IiopMsg {
  public: 
    LocateReply(void){};
    LocateReply(CORBA::Octet byteord) : IiopMsg((CORBA::Octet)locateReply, 
						byteord){
    };

    LocateReplyHeader rephdr;
    Frame body;
  };
  
  //======================================================================
  //Giop_1_0::CloseConnection
  //======================================================================
  
  class CloseConnection : public IiopMsg {
  public:
    CloseConnection(void){};
    CloseConnection(CORBA::Octet byteord) : 
      IiopMsg((CORBA::Octet)closeConnection, byteord){
    };
  };
  
  //======================================================================
  //Giop_1_0::MessageError
  //======================================================================
 
  class MessageError : public IiopMsg {
  public:
    MessageError(void){};
    MessageError(CORBA::Octet byteord) : 
      IiopMsg((CORBA::Octet)messageError, byteord){
    };
  };
  
};

#endif
