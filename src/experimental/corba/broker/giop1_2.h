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
Revision 1.1.1.1  2002/05/13 14:38:35  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:33  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.7  2001/06/08 08:40:16  bilhanan
Full licence clause added into doors 0.1


*/

//===========================================================================
//File:     giop1_2.h
//Date:     27.09.1999
//Subject:  IIOP ver 1.2
//Desc:   
//
//Author:   Janne Makihonka, TTKK/DMI
//===========================================================================


#ifndef GIOP1_2_H
#define GIOP1_2_H

#include <doors/giop1_1.h>
 
class Giop_1_2 : public Giop_1_1 {
public:

  //======================================================================
  //Giop_1_2::TargetAddress
  //======================================================================

  typedef short AddressingDisposition; 
  const short KeyAddr;
  const short ProfileAddr;
  const short ReferenceAddr;

  Giop_1_2(void) :
    KeyAddr(0), ProfileAddr(1), ReferenceAddr(2) {
  };
  
  struct IORAddressingInfo { 
    CORBA::ULong selected_profile_index; 
    IOP::IOR ior; 
  }; 

  class TargetAddress {
  public:
    AddressingDisposition addrdisp;
  };
  
  class KeyAddress : public TargetAddress {
  public:
    CORBA::Sequence <CORBA::Octet> object_key; 
  };
  
  class ProfileAddress : public TargetAddress {
  public:
    IOP::TaggedProfile profile; 
  };

  class ReferenceAddress : public TargetAddress {
  public:
    IORAddressingInfo ior; 
  }; 
  
  
  inline void 
  EncodeTargetAddress( const TargetAddress *from, Frame &tofrm, Uint32 &len) {
    DEBUGINFO("Giop_1_2::EncodeTargetAddress");
    IiopCodec :: Encode( (Uint16)from->addrdisp, tofrm, len);
    switch(from->addrdisp){
    case 0: // KeyAddr
      IiopCodec :: EncodeSequence( ((KeyAddress *)from)->object_key, tofrm, len); 
      break;
    case 1: //ProfileAddr
      IOP :: EncodeTaggedProfile(((ProfileAddress *)from)->profile, tofrm, len);
      break;
    case 2: //ReferenceAddr
      IiopCodec :: Encode( ((ReferenceAddress *)from)->ior.selected_profile_index, 
			   tofrm, len);
      IOP :: EncodeIor(((ReferenceAddress *)from)->ior.ior, tofrm, len);  
      break;
    default:
      ERROR( "Giop_1_2::EncodeTargetAddress, Unknown TargetAddress type");
      break;
    }
  };
  
  inline void 
  DecodeTargetAddress( TargetAddress* &to, Frame &fromfrm, 
		       bool swap, Uint32 &len) {
    DEBUGINFO("Giop_1_2::DecodeTargetAddress");
    IiopCodec :: Decode( (CORBA::UShort *) &(to->addrdisp),
			 fromfrm, swap, 1, len); 
    switch(to->addrdisp){
    case 0: // KeyAddr
      IiopCodec :: DecodeSequence(((KeyAddress *)to)->object_key, fromfrm, swap, 
				  len); 
      break; 
    case 1: //ProfileAddr
      IOP :: DecodeTaggedProfile(((ProfileAddress *)to)->profile, 
				 fromfrm, swap, len);
      break;
    case 2: //ReferenceAddr
      IiopCodec ::Decode((CORBA::ULong *)&((ReferenceAddress *)to)->ior.selected_profile_index, fromfrm, swap, 1, len);
      IOP :: DecodeIor(((ReferenceAddress *)to)->ior.ior, fromfrm, swap, len);
      break;
    default:
      ERROR( "Giop_1_2::DecodeTargetAddress, Unknown TargetAddress type");
      break;
    }
  };
  

  //======================================================================
  //Giop_1_2::RequestHeader
  //======================================================================
  
  struct RequestHeader { 
    CORBA::ULong request_id; 
    CORBA::Octet response_flags; 
    CORBA::Octet reserved[3]; 
    TargetAddress *target; 
    CORBA::iiopString operation; 
    IOP::ServiceContextList service_context; 
  }; 

  inline void 
  EncodeRequestHeader( const RequestHeader &from, Frame &tofrm, Uint32 &len) {
    DEBUGINFO("Giop_1_2::EncodeRequestHeader");
    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: Encode( (Byte) from.response_flags, tofrm, len );
    EncodeTargetAddress( from.target, tofrm, len ); 
    IiopCodec :: EncodeString( from.operation, tofrm, len );
    IOP :: EncodeSequenceServiceContext( from.service_context, tofrm, len);
  };
  
  inline void 
  DecodeRequestHeader( RequestHeader &to, Frame &fromfrm, bool swap, Uint32 &len) {
    DEBUGINFO("Giop_1_2::DecodeRequestHeader");
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

  enum ReplyStatusType { 
    NO_EXCEPTION, 
    USER_EXCEPTION, 
    SYSTEM_EXCEPTION, 
    LOCATION_FORWARD, 
    LOCATION_FORWARD_PERM, 
    NEEDS_ADDRESSING_MODE 
  };
  
  struct ReplyHeader { 
    CORBA::ULong request_id; 
    ReplyStatusType reply_status; 
    IOP::ServiceContextList service_context; 
  }; 

  inline void 
  EncodeReplyHeader( const ReplyHeader &from, Frame &tofrm, Uint32 &len) {
    DEBUGINFO("Giop_1_2::EncodeReplyHeader");
    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: Encode( (Uint32)from.reply_status, tofrm, len );
    IOP :: EncodeSequenceServiceContext( from.service_context, tofrm, len);  
  };
  
  inline void 
  DecodeReplyHeader( ReplyHeader &to, Frame &fromfrm, bool swap, Uint32 &len) {  
    DEBUGINFO("Giop_1_2::DecodeReplyHeader");
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
  //Giop_1_2::LocateRequestHeader
  //======================================================================
  
  struct LocateRequestHeader { 
    CORBA::ULong request_id; 
    TargetAddress *target; 
  }; 
  
  inline void 
  EncodeLocateRequestHeader( const LocateRequestHeader &from, Frame &tofrm, 
			     Uint32 &len) {
    DEBUGINFO("Giop_1_2::EncodeLocateRequestHeader");
    IiopCodec :: Encode( from.request_id, tofrm, len );
    EncodeTargetAddress( from.target, tofrm, len );
  };

  inline void 
  DecodeLocateRequestHeader( LocateRequestHeader &to, Frame &fromfrm, 
			     bool swap, Uint32 &len) {  
    DEBUGINFO("Giop_1_2::DecodeLocateRequestHeader");
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
  //Giop_1_0::LocateReplyHeader
  //======================================================================
  
  enum LocateStatusType { 
    UNKNOWN_OBJECT, 
    OBJECT_HERE, 
    OBJECT_FORWARD, 
    OBJECT_FORWARD_PERM, 
    LOC_SYSTEM_EXCEPTION,
    LOC_NEEDS_ADDRESSING_MODE 
  }; 
  
  struct LocateReplyHeader { 
    CORBA::ULong request_id; 
    LocateStatusType locate_status; 
  }; 
  
  inline void 
  EncodeLocateReplyHeader( const LocateReplyHeader &from, Frame &tofrm, 
			     Uint32 &len) {
    DEBUGINFO("Giop_1_2::EncodeLocateReplyHeader");
    IiopCodec :: Encode( from.request_id, tofrm, len );
    IiopCodec :: Encode( (Uint32)from.locate_status, tofrm, len );
  };

  inline void 
  DecodeLocateReplyHeader( LocateReplyHeader &to, Frame &fromfrm, 
			     bool swap, Uint32 &len) {  
    DEBUGINFO("Giop_1_2::DecodeLocateReplyHeader");
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
  
  struct FragmentHeader { 
    unsigned long request_id; 
  }; 
  
  inline void 
  EncodeFragmentHeader( const FragmentHeader &from, Frame &tofrm, 
			Uint32 &len) {
    DEBUGINFO("Giop_1_2::EncodeFragmentHeader");
    IiopCodec :: Encode( from.request_id, tofrm, len );
  };
  
  inline void 
  DecodeFragmentHeader( FragmentHeader &to, Frame &fromfrm, 
			bool swap, Uint32 &len) {
    DEBUGINFO("Giop_1_2::DecodeFragmentHeader");
    Sint32 frlen = fromfrm.length();
    if(frlen  >= 4){
      IiopCodec :: Decode( to.request_id, fromfrm, swap, len);
      return;
    }
  };

  //======================================================================
  //======================================================================

  class IiopMsg {
  public:
    IiopMsg(void){};
    IiopMsg(CORBA::Octet mestype, CORBA::Octet byteord = 0, 
	    CORBA::Octet moreflag = 0 ){
      memcpy(&(msghdr.magic[0]), "GIOP",4);
      msghdr.GIOP_version.major = 1;
      msghdr.GIOP_version.minor = 2;
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
    Uint8 orderbit(){
      return (Uint8)1 & msghdr.flags;
    }

  };
  
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

  
#endif;
