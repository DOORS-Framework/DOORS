//Editor-Info: -*- C++ -*-
//
//  Copyright 2002 
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


// $Log: giop11codec.h,v $
// Revision 1.4  2003/01/23 14:43:29  bilhanan
// giop header codec now GiopMsg_1_1 class member funcs, moved to giopmsg11.C
//
// Revision 1.3  2003/01/21 13:59:13  bilhanan
// devt
//
// Revision 1.2  2002/09/23 09:33:51  bilhanan
// development
//
// Revision 1.1  2002/09/19 12:56:32  bilhanan
// initial revision
//


#ifndef GIOP1_1_H
#define GIOP1_1_H

#include <doors/debug.h>
#include <doors/giopmsg11.h>
#include <doors/cdrcodec.h>

// Contains encoding and decoding functions 
// for the GIOP 1.1 data structs in giop.h
// Also contains the base class for GIOP 1.1 PDUs
 
namespace GIOP {

  
  // GIOP EncodeMessageHeader_1_1 moved to giopmsg11.C
  
  // GIOP DecodeMessageHeader_1_1 moved to giopmsg11.C

  
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
  (RequestHeader_1_1 &to, Frame &fromfrm, Uint32 &len) {

    Sint32 frlen = fromfrm.length() + len;
    IOP :: DecodeSequenceServiceContext( to.service_context, fromfrm, to.swap, len);
    if((frlen - len) >= 20){
      IiopCodec :: Decode( to.request_id, fromfrm, to.swap, len);
      IiopCodec :: Decode( to.response_expected, fromfrm, to.swap, len);
      IiopCodec :: DecodeSequence( to.object_key, fromfrm,  to.swap, len);
      IiopCodec :: DecodeString( to.operation, fromfrm, to.swap, len );
      IiopCodec :: DecodePrincipal( to.requesting_principal, fromfrm, to.swap, len ); 
    } else
    _DOORS_DEBUG_(0,  "Giop_1_1::DecodeRequestHeader, Frame length " << frlen << ", " 
	   << "while it should be at least " << 20 << "." );
    return;
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
    } else
      _DOORS_DEBUG_(0,  "Giop_1_1::DecodeReplyHeader, Frame length " << frlen << ", " 
	     << "while it should be at least " << 8 << "." );
    return;
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
    } else
      _DOORS_DEBUG_(0,  "Giop_1_1:: DecodeCancelRequestHeader, Frame length " << frlen 
	     << ", " 
	     << "while it should be at least " << 4 << "." );
    return;
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
    } else
    _DOORS_DEBUG_(0,  "Giop_1_1::DecodeLocateRequestHeader, Frame length " << frlen 
	   << ", " 
	   << "while it should be at least " << 8 << "." );
    return;
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
    } else
      _DOORS_DEBUG_(0,  "Giop_1_1::DecodeLocateReplyHeader, Frame length " << frlen 
	     << ", " 
	     << "while it should be at least " << 8 << "." );
    return;
  };
  

#endif;
