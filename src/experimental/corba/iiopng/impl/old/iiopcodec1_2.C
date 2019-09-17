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

$Log: iiopcodec1_2.C,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:22  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.5  2001/06/08 08:40:16  bilhanan
Full licence clause added into doors 0.1


*/

//===========================================================================
//File:     iiopcodec1_2.C
//Date:     24.08.1999
//Subject:  IIOP ver 1.2
//Desc:
//
//Author:   Janne Makihonka, TTKK/DMI
//===========================================================================

#include <doors/iiopsm1_2.h>
#include <doors/frame.h>
#include <doors/giop1_2.h>
#include <doors/iop.h>

Frame* IiopSm_1_2 :: Encode( Giop :: IiopMsg *message)
{ 
  Giop_1_2 :: IiopMsg *msg = (Giop_1_2 :: IiopMsg *)message;
  
  Frame m, body;
  Uint32 msglen = 0;
  
  IiopCodec :: Encode((Byte *)&((Giop_1_2::Request *)msg)->msghdr, m, 12, msglen);
  
  switch(msg->msghdr.message_type){
  case Giop_1_2::request :
    DEBUGINFO("Giop_1_2::Encode, request");
    Giop_1_2 :: EncodeRequestHeader( ((Request *)msg)->reqhdr, m, msglen);
    body = ((Request *)msg)->body; 
    break;
  case Giop_1_2::reply :
    DEBUGINFO("Giop_1_2::Encode, reply");
    Giop_1_2 :: EncodeReplyHeader( ((Reply *)msg)->rephdr, m, msglen);
    body = ((Reply *)msg)->body; 
    break;
  case Giop_1_2::cancelRequest :
    DEBUGINFO("Giop_1_2::Encode, cancelRequest");
    Giop_1_2 :: EncodeCancelRequestHeader( ((CancelRequest *)msg)->reqhdr, m, msglen);
    break;
  case Giop_1_2::locateRequest :
    DEBUGINFO("Giop_1_2::Encode, locateRequest");
    Giop_1_2 :: EncodeLocateRequestHeader( ((LocateRequest *)msg)->reqhdr, m, msglen);
    break;
  case Giop_1_2::locateReply :
    DEBUGINFO("Giop_1_2::Encode, locateReply");
    Giop_1_2 :: EncodeLocateReplyHeader( ((LocateReply *)msg)->rephdr, m, msglen);
    body = ((LocateReply *)msg)->body;
    break;
  case Giop_1_2::closeConnection :
    DEBUGINFO("Giop_1_2::Encode, closeConnection");
    break;
  case Giop_1_2::messageError :
    DEBUGINFO("Giop_1_2::Encode, messageError"); 
    break;
  case Giop_1_2::fragment :
    DEBUGINFO("Giop_1_2::Encode, fragment"); 
    Giop_1_2 :: EncodeFragmentHeader( ((Fragment *)msg)->fraghdr, m , msglen);
    body = ((Fragment *)msg)->body;
    break;
  default :
    ERROR("Giop_1_2::Encode, Unknown message type.");
    return NULL;
    break;
  }
  
  int padding = 0;
  
  switch(msg->firstParamSize){
  case 0: break;
  case 1: break;
  case 2:
    padding = (unsigned int)msglen % 2; break;
  case 4:
    padding = 3 - (((unsigned int)(msglen+3)) % 4); break;
  case 8:
    padding = 7 - (((unsigned int)(msglen+7)) % 8); break; 
  default:
    throw "first parameter Size must be 0, 1, 2, 4 or 8"; break;  
  }
  
  Uint32 bodylen = body.length();
  msglen += padding + bodylen - 12;
  msglen = htonl(msglen);

  m.write((Byte) msglen, 11);
  m.write((Byte) (msglen >> 8), 10);
  m.write((Byte) (msglen >> 16), 9);
  m.write((Byte) (msglen >> 24), 8);
  
  Frame *pdu = new Frame;
  pdu->combine(m);  // Add message headers
  pdu->putLast((Byte *)ZEROS, padding); // Add null padding
  pdu->combine(body);                  // Add body
  
  delete msg;
  return pdu;

}


Giop::IiopMsg* IiopSm_1_2 :: Decode(Frame *msg){
 IiopMsg *imsg = NULL;
  CORBA::ULong msglen = msg->length();
  Uint32 declen = 0;
  
  Giop_1_2::MessageHeader msghdr;

  Giop_1_2 :: DecodeMessageHeader ( msghdr, *msg, declen );  
  
  bool swap = ((msghdr.flags & 0x01) == conn->byteorder) ? 0 : 1;
  
  if(swap) SWAP4(msghdr.message_size);

  switch(msghdr.message_type){
  case Giop_1_2::request :
    DEBUGINFO("Giop_1_2::Decode, request");
    imsg = new Giop_1_2::Request;
    
    Giop_1_2 :: DecodeRequestHeader(((Request *)imsg)->reqhdr, *msg, 
				    swap, declen);  
    ((Request *)imsg)->body = *msg;
    ((Request *)imsg)->body_offset = declen;
    break;
  case Giop_1_2::reply :
    DEBUGINFO("Giop_1_2::Decode, reply");
    imsg = new Reply;
    Giop_1_2 :: DecodeReplyHeader(((Reply *)imsg)->rephdr, *msg, 
				    swap, declen);  
    ((Reply *)imsg)->body = *msg;
    ((Reply *)imsg)->body_offset = declen;
    break;
  case Giop_1_2::cancelRequest :
    DEBUGINFO("Giop_1_2::Decode, cancelRequest");
    imsg = new CancelRequest;
    Giop_1_2 :: DecodeCancelRequestHeader(((CancelRequest *)imsg)->reqhdr, *msg, 
				    swap, declen);
    break;
  case Giop_1_2::locateRequest :
    DEBUGINFO("Giop_1_2::Decode, locateRequest");
    imsg = new LocateRequest;
    Giop_1_2 :: DecodeLocateRequestHeader(((LocateRequest *)imsg)->reqhdr, *msg, 
				    swap, declen);
    break;
  case Giop_1_2::locateReply :
    DEBUGINFO("Giop_1_2::Decode, locateReply");
    imsg = new LocateReply;
    Giop_1_2 :: DecodeLocateReplyHeader(((LocateReply *)imsg)->rephdr, *msg, 
				    swap, declen);
    break;
  case Giop_1_2::closeConnection :
    DEBUGINFO("Giop_1_2::Decode, closeConnection");
    imsg = new CloseConnection;
    break;
  case Giop_1_2::messageError :
    DEBUGINFO("Giop_1_2::Decode, messageError");
    imsg = new MessageError;
    break;
  case Giop_1_2::fragment :
    DEBUGINFO("Giop_1_2::Encode, fragment"); 
    imsg = new Fragment;
    Giop_1_2 :: DecodeFragmentHeader( ((Fragment *)imsg)->fraghdr, 
				      *msg , swap, declen);
    ((Fragment *)imsg)->body = *msg;
    ((Fragment *)imsg)->body_offset = declen;
    break;
  default :
    ERROR("Giop_1_2::Decode, Unknown message type.");
    return NULL;
    break;
  }

  imsg->msghdr = msghdr;

  delete msg;
  return (Giop::IiopMsg*)imsg;
}


