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

$Log: giop1_2.h,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:21  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.7  2001/06/08 08:40:16  bilhanan
Full licence clause added into doors 0.1


*/

// Original Author:   Janne Makihonka



#ifndef GIOP1_2_H
#define GIOP1_2_H

#include <doors/giop.h>

// Contains encoding and decoding functions 
// for the GIOP 1.2 data structs in giop.h
// Also contains the base class for GIOP 1.2 PDUs
 
namespace GIOP {

  //======================================================================
  //Giop_1_2::MessageHeader
  //======================================================================

  inline void EncodeMessageHeader_1_2
  (const MessageHeader_1_2 &from, Frame &tofrm, Uint32 &len) {
    IiopCodec :: Encode( (Byte *)&from, tofrm, 12, len );
  };
  
  inline void DecodeMessageHeader_1_2 
  (MessageHeader_1_2 &to, Frame &fromfrm, Uint32 &len){ 
  
    Sint32 frlen = fromfrm.length();
    if( frlen >= 12 ){ 
      IiopCodec :: Decode( (Byte *)&to, fromfrm, 0, 12, len );
      return;
    }
    ERROR( "Giop_1_2::DecodeMessageHeader, Frame length " << frlen << ", " 
	   << "while it should be at least " << 12 << "." );
  }; 

  
  //======================================================================
  //Giop_1_2::TargetAddress
  //======================================================================

  
  
  inline void 
  EncodeTargetAddress( const TargetAddress *from, Frame &tofrm, Uint32 &len) {

    IiopCodec :: Encode( (Uint16)from->addrdisp, tofrm, len);
    switch(from->addrdisp){
    case 0: // KeyAddr
      IiopCodec :: EncodeSequence( ((KeyAddress *)from)->val.object_key, tofrm, len); 
      break;
    case 1: //ProfileAddr
      IOP :: EncodeTaggedProfile(((ProfileAddress *)from)->val.profile, tofrm, len);
      break;
    case 2: //ReferenceAddr
      IiopCodec :: Encode( ((ReferenceAddress *)from)->val.ior.selected_profile_index, 
			   tofrm, len);
      IOP :: EncodeIor(((ReferenceAddress *)from)->val.ior.ior, tofrm, len);  
      break;
    default:
      ERROR( "Giop_1_2::EncodeTargetAddress, Unknown TargetAddress type");
      break;
    }
  };
  
  inline void 
  DecodeTargetAddress( TargetAddress* &to, Frame &fromfrm, 
		       bool swap, Uint32 &len) {

    IiopCodec :: Decode( (CORBA::UShort *) &(to->addrdisp),
			 fromfrm, swap, 1, len); 
    switch(to->addrdisp){
    case 0: // KeyAddr
      IiopCodec :: DecodeSequence(((KeyAddress *)to)->val.object_key, fromfrm, swap, 
				  len); 
      break; 
    case 1: //ProfileAddr
      IOP :: DecodeTaggedProfile(((ProfileAddress *)to)->val.profile, 
				 fromfrm, swap, len);
      break;
    case 2: //ReferenceAddr
      IiopCodec ::Decode((CORBA::ULong *)&((ReferenceAddress *)to)->val.ior.selected_profile_index, fromfrm, swap, 1, len);
      IOP :: DecodeIor(((ReferenceAddress *)to)->val.ior.ior, fromfrm, swap, len);
      break;
    default:
      ERROR( "Giop_1_2::DecodeTargetAddress, Unknown TargetAddress type");
      break;
    }
  };
  

  //======================================================================
  //Giop_1_2::RequestHeader
  //======================================================================
  
  inline void EncodeRequestHeader_1_2
  (const RequestHeader_1_2 &from, Frame &tofrm, Uint32 &len) {

    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: Encode( (Byte) from.response_flags, tofrm, len );
    EncodeTargetAddress( from.target, tofrm, len ); 
    IiopCodec :: EncodeString( from.operation, tofrm, len );
    IOP :: EncodeSequenceServiceContext( from.service_context, tofrm, len);
  };
  
  inline void DecodeRequestHeader_1_2
  (RequestHeader_1_2 &to, Frame &fromfrm, bool swap, Uint32 &len) {

    Sint32 frlen = fromfrm.length() + len;
    if((frlen - len) >= 20){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
      Byte *tmp = (Byte *)&to.response_flags;
      IiopCodec :: Decode( *tmp, fromfrm, swap, len);
      DecodeTargetAddress( to.target, fromfrm, swap, len);
      IiopCodec :: DecodeString( to.operation, fromfrm, swap, len );
      IOP :: DecodeSequenceServiceContext( to.service_context, fromfrm, swap, len);
      return;
    }
    ERROR( "Giop_1_2::DecodeRequestHeader, Frame length " << frlen << ", " 
	   << "while it should be at least " << 20 << "." );
  };

  //======================================================================
  //Giop_1_2::ReplyHeader
  //======================================================================

 inline void EncodeReplyHeader_1_2
 (const ReplyHeader_1_2 &from, Frame &tofrm, Uint32 &len) {

    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: Encode( (Uint32)from.reply_status, tofrm, len );
    IOP :: EncodeSequenceServiceContext( from.service_context, tofrm, len);  
  };
  
  inline void DecodeReplyHeader_1_2
  (ReplyHeader_1_2 &to, Frame &fromfrm, bool swap, Uint32 &len) {  

    Sint32 frlen = fromfrm.length();
    if( frlen >= 12){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
      Uint32 *tmp = (Uint32 *)&to.reply_status;
      IiopCodec :: Decode( *tmp, fromfrm, swap, len);
      IOP :: DecodeSequenceServiceContext( to.service_context, fromfrm, swap, len);
      return;
    }
    ERROR( "Giop_1_2::DecodeReplyHeader, Frame length " << frlen << ", " 
	   << "while it should be at least " << 12 << "." );
  };

  //======================================================================
  //Giop_1_2::SystemExceptionReplyBody 
  //======================================================================

  inline void EncodeSystemExceptionReplyBody_1_2
  (const GIOP::SystemExceptionReplyBody& repbody, Frame &tofrm, Uint32 &len) {
 
    IiopCodec :: EncodeString( repbody.exception_id, tofrm, len );
    IiopCodec :: Encode( repbody.minor_code_value, tofrm, len );
    IiopCodec :: Encode( repbody.completion_status, tofrm, len );
  };
  
  inline void DecodeSystemExceptionReplyBody_1_2
  (GIOP::SystemExceptionReplyBody& repbody, Frame &fromfrm, bool swap, Uint32 &len) {

    IiopCodec :: DecodeString( repbody.exception_id, fromfrm, swap, len);
    IiopCodec :: Decode( repbody.minor_code_value, fromfrm, swap, len);
    IiopCodec :: Decode( repbody.completion_status, fromfrm, swap, len);
  };
  

  //======================================================================
  //Giop_1_2::CancelRequestHeader
  //======================================================================


  
  inline void EncodeCancelRequestHeader_1_2
  (const CancelRequestHeader &from, Frame &tofrm, Uint32 &len) {
    IiopCodec :: Encode( from.request_id, tofrm, len );
  };

  inline void DecodeCancelRequestHeader_1_2
  (CancelRequestHeader &to, Frame &fromfrm, bool swap, Uint32 &len) {  
    Sint32 frlen = fromfrm.length();
    if(frlen  >= 4){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
    } 
      ERROR( "Giop_1_2:: DecodeCancelRequestHeader, Frame length " << frlen 
	     << ", " 
	     << "while it should be at least " << 4 << "." );
  };


  //======================================================================
  //Giop_1_2::LocateRequestHeader
  //======================================================================
  
  struct LocateRequestHeader { 
    CORBA::ULong request_id; 
    TargetAddress *target; 
  }; 
  
  inline void 
  EncodeLocateRequestHeader( const LocateRequestHeader &from, Frame &tofrm, 
			     Uint32 &len) {

    IiopCodec :: Encode( from.request_id, tofrm, len );
    EncodeTargetAddress( from.target, tofrm, len );
  };

  inline void 
  DecodeLocateRequestHeader( LocateRequestHeader &to, Frame &fromfrm, 
			     bool swap, Uint32 &len) {  

    Sint32 frlen = fromfrm.length();
    if(frlen  >= 8){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
      DecodeTargetAddress( to.target, fromfrm, swap, len);
      return;
    }
    ERROR( "Giop_1_2::DecodeLocateRequestHeader, Frame length " << frlen << ", " 
	   << "while it should be at least " << 8 << "." );
  };

  //======================================================================
  //Giop_1_2::LocateReplyHeader
  //======================================================================
  
  inline void EncodeLocateReplyHeader_1_2
  (const LocateReplyHeader_1_2 &from, Frame &tofrm, Uint32 &len) {

    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: Encode( (Uint32)from.locate_status, tofrm, len );
  };

  inline void DecodeLocateReplyHeader_1_2
  (LocateReplyHeader_1_2 &to, Frame &fromfrm, bool swap, Uint32 &len) {  

    Sint32 frlen = fromfrm.length();
    if(frlen  >= 8){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
      Uint32 *tmp = (Uint32 *)&to.locate_status;
      IiopCodec :: Decode( *tmp, fromfrm, swap, len ); 
      return;
    }
    ERROR( "Giop_1_2::DecodeLocateReplyHeader, Frame length " << frlen << ", " 
	   << "while it should be at least " << 8 << "." );
  };
  

  //======================================================================
  //Giop_1_2::FragmentHeader
  //======================================================================
  
  inline void EncodeFragmentHeader_1_2
  (const FragmentHeader_1_2 &from, Frame &tofrm, Uint32 &len) {

    IiopCodec :: Encode( from.request_id, tofrm, len );
  };
  
  inline void DecodeFragmentHeader_1_2
  (FragmentHeader_1_2 &to, Frame &fromfrm, bool swap, Uint32 &len) {

    Sint32 frlen = fromfrm.length();
    if(frlen  >= 4){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
      return;
    }
  };

  // Giop 1.2 Msg base class
  // This class is inherited by all other GIOP 1.2 PDU classes. 
  // It contains only the message header structure 
  // which is appropriately initialised.

  class GIOP_1_2_Msg {
  public:
    GIOP_1_2_Msg(void){};
    GIOP_1_2_Msg(CORBA::Octet mestype, CORBA::Octet byteord = 0, 
	    CORBA::Octet moreflag = 0 ){
      memcpy(&(msghdr.magic[0]), "GIOP",4);
      msghdr.GIOP_version.major = 1;
      msghdr.GIOP_version.minor = 2;
      msghdr.flags = (Uint8)1 & byteord;
      msghdr.flags += (Uint8)2 & moreflag;
      msghdr.message_type = mestype;
      msghdr.message_size = 0;
    };

    MessageHeader_1_2 msghdr;
    Uint32 firstParamSize;
    Uint32 body_offset;

    Uint8 morebit(){
      return (Uint8)2 & msghdr.flags;
    }
    Uint8 orderbit(){
      return (Uint8)1 & msghdr.flags;
    }

  };
  

#if 0 //we don't need all this now.

  class Request : public IiopMsg {
  public:
    Request(void){};
    Request(CORBA::Octet byteord) : IiopMsg((CORBA::Octet)request, byteord){
    };
    
    RequestHeader reqhdr;
    Frame body;
  };

 
  class Reply : public IiopMsg {
  public:
    Reply(void){};
    Reply(CORBA::Octet byteord) : IiopMsg((CORBA::Octet)reply, byteord){
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
    
    FragmentHeader fraghdr;
    Frame body;
  };

};

#endif // commenting out  
#endif;
