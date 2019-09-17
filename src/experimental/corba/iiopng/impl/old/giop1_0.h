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
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:20  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.9  2001/06/08 08:40:12  bilhanan
Full licence clause added into doors 0.1


*/


// Original Author:   Janne Makihonka


#ifndef GIOP1_0_H
#define GIOP1_0_H


#include <doors/bascodec.h>
#include <doors/iop.h>
#include <doors/giop.h>
#include <doors/pdu.h>

// Contains encoding and decoding functions 
// for the GIOP 1.0 data structs in giop.h
// Also contains the base class for GIOP 1.0 PDUs


namespace GIOP { 

  //======================================================================
  //Giop_1_0::MessageHeader
  //======================================================================

  inline void EncodeMessageHeader_1_0
  (const MessageHeader_1_0 &from, Frame &tofrm, Uint32 &len) {

    IiopCodec :: Encode( (Byte *)&from, tofrm, 12, len );
    
  };
  
  inline void DecodeMessageHeader_1_0 
  (MessageHeader_1_0 &to, Frame &fromfrm, Uint32 &len){ 
    
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
  
  inline void EncodeRequestHeader_1_0
  (const RequestHeader_1_0 &from, Frame &tofrm, Uint32 &len) {

    IOP :: EncodeSequenceServiceContext( from.service_context, tofrm, len);
    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: Encode( from.response_expected, tofrm, len );
    IiopCodec :: EncodeSequence( from.object_key, tofrm, len );
    IiopCodec :: EncodeString( from.operation, tofrm, len );
    IiopCodec :: EncodePrincipal( from.requesting_principal, tofrm, len );
  };
  
  inline void DecodeRequestHeader_1_0
  (RequestHeader_1_0 &to, Frame &fromfrm, bool swap, Uint32 &len) {  
    
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

  inline void EncodeReplyHeader_1_0
  (const ReplyHeader_1_0 &from, Frame &tofrm, Uint32 &len) {
    
    IOP :: EncodeSequenceServiceContext( from.service_context, tofrm, len);
    IiopCodec :: Encode (from.request_id, tofrm, len );
    IiopCodec :: Encode((Uint32)from.reply_status, tofrm, len );
  };
  
  inline void DecodeReplyHeader_1_0
  (ReplyHeader_1_0 &to, Frame &fromfrm, bool swap, Uint32 &len){

    Sint32 frlen = fromfrm.length() + len;
    IOP :: DecodeSequenceServiceContext (to.service_context, fromfrm, swap, len);
    if((frlen - len) >= 8){
      IiopCodec :: Decode (to.request_id, fromfrm, swap, len);
      Uint32 *tmp = (Uint32 *) &to.reply_status;
      IiopCodec :: Decode (*tmp, fromfrm, swap, len);
    } 
      ERROR( "Giop_1_0::DecodeReplyHeader, Frame length " << frlen << ", " 
	     << "while it should be at least " << 8 << "." );
  };


  //======================================================================
  //Giop_1_0::SystemExceptionReplyBody 
  //======================================================================

  inline void EncodeSystemExceptionReplyBody_1_0
  (const GIOP::SystemExceptionReplyBody& repbody, Frame &tofrm, Uint32 &len) {
 
    IiopCodec :: EncodeString( repbody.exception_id, tofrm, len );
    IiopCodec :: Encode( repbody.minor_code_value, tofrm, len );
    IiopCodec :: Encode( repbody.completion_status, tofrm, len );
  };
  
  inline void DecodeSystemExceptionReplyBody_1_0
  (GIOP::SystemExceptionReplyBody& repbody, Frame &fromfrm, bool swap, Uint32 &len) {

    IiopCodec :: DecodeString( repbody.exception_id, fromfrm, swap, len);
    IiopCodec :: Decode( repbody.minor_code_value, fromfrm, swap, len);
    IiopCodec :: Decode( repbody.completion_status, fromfrm, swap, len);
  };
  

  //======================================================================
  //Giop_1_0::CancelRequestHeader
  //======================================================================


  
  inline void EncodeCancelRequestHeader_1_0
  (const CancelRequestHeader &from, Frame &tofrm, Uint32 &len) {
    IiopCodec :: Encode( from.request_id, tofrm, len );
  };

  inline void DecodeCancelRequestHeader_1_0
  (CancelRequestHeader &to, Frame &fromfrm, bool swap, Uint32 &len) {  
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
  
   inline void EncodeLocateRequestHeader_1_0
  (const LocateRequestHeader_1_0 &from, Frame &tofrm, Uint32 &len) {

    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: EncodeSequence( from.object_key, tofrm, len );
  };

  inline void DecodeLocateRequestHeader_1_0
  (LocateRequestHeader_1_0 &to, Frame &fromfrm, bool swap, Uint32 &len) {  

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
  
  inline void EncodeLocateReplyHeader_1_0
  (const LocateReplyHeader_1_0 &from, Frame &tofrm, Uint32 &len) {

    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: Encode( (Uint32)from.locate_status, tofrm, len );
  };

  inline void DecodeLocateReplyHeader_1_0
  (LocateReplyHeader_1_0 &to, Frame &fromfrm, bool swap, Uint32 &len) {  

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
  


  // Giop 1.0 Msg base class
  // This class is inherited by all other GIOP 1.0 PDU classes. 
  // It contains only the message header structure 
  // which is appropriately initialised.

  
  class GIOP_1_0_Msg : public ::PDU {

  public:

    GIOP_1_0_Msg(void){};
    
    GIOP_1_0_Msg(CORBA::Octet mestype, CORBA::Octet byteord = 0){

      memcpy(&(msghdr.magic[0]), "GIOP",4);
      msghdr.GIOP_version.major = 1;
      msghdr.GIOP_version.minor = 0;
      msghdr.byte_order = byteord;
      msghdr.message_type = mestype;
      msghdr.message_size = 0;
    }

    virtual Uint16 decode (Message *msg) {return 0};
    virtual Message *encode (void) {};

    MessageHeader_1_0 msghdr;
    Uint32 firstParamSize;
    Uint32 body_offset;
  };

  
  //======================================================================
  //Giop_1_0::Request
  //======================================================================
  
#if 0

  class Request_1_0 : public GIOP_1_0_MsgHdr {

  public:

    Request_1_0 (void){};

    Request_1_0 (CORBA::Octet byteord) 
      : GIOP_1_0_MsgHdr((CORBA::Octet)request, byteord){
    };
    
    RequestHeader_1_0 reqhdr;
     Frame body;
  };

  //======================================================================
  //Giop_1_0::Reply
  //======================================================================
  
  class Reply_1_0 : public GIOP_1_0_MsgHdr {

  public:

    Reply_1_0 (void){};

    Reply_1_0 (CORBA::Octet byteord) 
      : GIOP_1_0_MsgHdr((CORBA::Octet)reply, byteord){
    };

    ReplyHeader_1_0 rephdr;
    Frame body;
  };
  
  //======================================================================
  //Giop_1_0::CancelRequest
  //======================================================================
  
  class CancelRequest_1_0 : public GIOP_1_0_MsgHdr {

  public:

    CancelRequest_1_0 (void){};

    CancelRequest_1_0 (CORBA::Octet byteord) 
      : GIOP_1_0_MsgHdr((CORBA::Octet)cancelRequest, byteord){
    };

    CancelRequestHeader_1_0 reqhdr;
  };
  
  //======================================================================
  //Giop_1_0::LocateRequest
  //======================================================================
  
  class LocateRequest_1_0 : public GIOP_1_0_MsgHdr {

  public:

    LocateRequest_1_0 (void){};

    LocateRequest_1_0 (CORBA::Octet byteord) 
      : GIOP_1_0_MsgHdr((CORBA::Octet)locateRequest, byteord){
    };
    LocateRequestHeader_1_0 reqhdr;
  };
  
  //======================================================================
  //Giop_1_0::LocateReply
  //======================================================================
  
  class LocateReply_1_0 : public GIOP_1_0_MsgHdr {

  public: 
    
    LocateReply_1_0 (void){};
    
    LocateReply_1_0 (CORBA::Octet byteord) 
      : GIOP_1_0_MsgHdr((CORBA::Octet)locateReply, byteord){
    };

    LocateReplyHeader_1_0 rephdr;
    Frame body;
  };
  
  //======================================================================
  //Giop_1_0::CloseConnection
  //======================================================================
  
  class CloseConnection_1_0 : public GIOP_1_0_MsgHdr {

  public:
    
    CloseConnection_1_0 (void){};

    CloseConnection_1_0 (CORBA::Octet byteord) 
      : GIOP_1_0_MsgHdr((CORBA::Octet)closeConnection, byteord){
    };
  };
  
  //======================================================================
  //Giop_1_0::MessageError
  //======================================================================
 
  class MessageError_1_0 : public GIOP_1_0_MsgHdr {

  public:

    MessageError_1_0 (void){};

    MessageError_1_0 (CORBA::Octet byteord) 
      : GIOP_1_0_MsgHdr((CORBA::Octet)messageError, byteord){
    };
  };
  
};

#endif // end of commented out block
#endif
