// Editor-info: -*- C++ -*-
// 
//  Copyright 2002 
//      Telecom Lab, Tampere University of Technology.  
//      All rights reserved.
 
 
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

//  $Log: gioppeer.C,v $
//  Revision 1.7  2003/04/17 12:56:02  bilhanan
//  typo fix.
//
//  Revision 1.6  2003/01/30 15:24:10  bilhanan
//  devt
//
//  Revision 1.5  2003/01/23 14:44:17  bilhanan
//  development
//
//  Revision 1.4  2003/01/21 13:59:13  bilhanan
//  devt
//
//  Revision 1.3  2002/09/23 09:33:51  bilhanan
//  development
//
//  Revision 1.2  2002/09/19 12:55:52  bilhanan
//  initial revision
//
//  Revision 1.1  2002/08/14 05:56:04  bilhanan
//  initial version
//

#include <doors/transap.h>
#include <doors/giopmsg11.h>
#include <doors/gioppeer.h>
#include <doors/iop.h>
#include <doors/giop11codec.h>
#include <doors/cdrcodec.h>

Uint16 GiopPeer :: Request :: decode (Message *msg) {

  TranSAP::Dtind *tranmsg = msg;
  tcp_cep = tranmsg->cep;
  Frame b = tranmsg->data;

  // Decode GIOP Message Header

  DecodeMessageHeader_1_1 (b, len); 

  // Decode Request Message header
  DecodeRequestHeader_1_1 (reqhdr, b, len);

  // We can't really decode arguments here, so the rest goes to the body
  // and let serverrequest task decode it.

  body = b;

  return len;
}

Message *GiopPeer :: Request :: encode (void) {

  TranSAP::Dtreq *msg = new TranSAP::Dtreq;
  msg->cep = tcp_cep;

  Frame encfrm;
  Uint32 encmsglen = 12; //assume that GIOP message header already encoded.

  // encode request header.

  EncodeRequestHeader_1_1 (reqhdr, encfrm, encmsglen);
    
  // encode request body.

  std::vector<NamedValue>::iterator iter = args->begin();
  std::vector<NamedValue>::iterator iter_end = args->end();

  for (; iter =! iter_end; ++iter)
    iter->Encode(encfrm, encmsglen);

  giop.message_size = encmsglen;

  encmsglen -= 12; // let's ready for msg header encoding.

  // Do a small integrity check

  if (encmsglen != ((Uint32)encfrm.length()))
    _DOORS_DEBUG (0, "GIOP Request Encoding mismatch");

  // Let's encode MessageHeader now

  EncodeMessageHeader_1_1 (msg->data, encmsglen);

  // Let's combine the 2 frames now

  msg->data.combine (encfrm);
  
    // ensure no request args get deleted accidentally.

  this->args = null;


  return msg;
}

Uint16 GiopPeer :: Reply :: decode (Message *msg) {

  TranSAP::Dtind *tranmsg = msg;
  tcp_cep = tranmsg->cep;
  Frame b = tranmsg->data;

  // Decode GIOP Message Header

  DecodeMessageHeader_1_1 (giophdr, b, len); 

  // Decode Reply Message header
  DecodeReplyHeader_1_1 (rephdr, b, len);
  // The rest goes to the body
  body = b;
  return len;
}

Message *GiopPeer :: Reply :: encode (Message *msg) {

  TranSAP::Dtreq *msg = new TranSAP::Dtreq;

  msg->cep = cep;
  //  msg->data = body;

  Uint32 encmsglen;
  return msg;
}

Uint16 GiopPeer :: CancelRequest :: decode (Message *msg) {

  TranSAP::Dtind *tranmsg = msg;
  tcp_cep = tranmsg->cep;
  Frame b = tranmsg->data;

  // Decode GIOP Message Header

  DecodeMessageHeader_1_1 (giophdr, b, len); 

  // Decode CancelRequest Message header
  DecodeCancelRequestHeader_1_1 (cancreqhdr, b, len);
  // The rest goes to the body
  body = b;
  return len;
}

Message *GiopPeer :: CancelRequest :: encode (void) {

  TranSAP::Dtreq *msg = new TranSAP::Dtreq;

  msg->cep = cep;
  msg->data = body; // used abit differently here, body contains *entire* encoded msg
  return msg;

}

Uint16 GiopPeer :: LocateRequest :: decode (Message *msg) {

  TranSAP::Dtind *tranmsg = msg;
  tcp_cep = tranmsg->cep;
  Frame b = tranmsg->data;

  // Decode GIOP Message Header

  DecodeMessageHeader_1_1 (giophdr, b, len); 

  // Decode LocateRequest Message header
  DecodeLocateRequestHeader_1_1 (lreqhdr, b, len);

}

Message *GiopPeer :: LocateRequest :: encode (void) {

  TranSAP::Dtreq *msg = new TranSAP::Dtreq;

  msg->cep = cep;

  Uint32 encmsglen;

  EncodeMessageHeader_1_1 (giophdr, msg->data, encmsglen);
  EncodeLocateRequestHeader_1_1 (lreqhdr, msg->data, encmsglen);
  
  return msg;

}

Uint16 GiopPeer :: LocateReply :: decode (Message *msg) {

  TranSAP::Dtind *tranmsg = msg;
  tcp_cep = tranmsg->cep;
  Frame b = tranmsg->data;


  // Decode GIOP Message Header

  DecodeMessageHeader_1_1 (giophdr, b, len); 

  // Decode LocateReply Message header
  DecodeLocateReplyHeader_1_1 (lrephdr, b, len);
  // The rest goes to the body
  body = b;
  return len;
}

Message *GiopPeer :: LocateReply :: encode (Message *msg) {

  TranSAP::Dtreq *msg = new TranSAP::Dtreq;

  msg->cep = cep;
  msg->data = body; // used abit differently here, body contains *entire* encoded msg
  return msg;

}

Uint16 GiopPeer :: CloseConnection :: decode (Message *msg) {


  TranSAP::Dtind *tranmsg = msg;
  tcp_cep = tranmsg->cep;


  // Decode GIOP Message Header

  DecodeMessageHeader_1_1 (giophdr, b, len); 

  return len;
}

Message *GiopPeer :: CloseConnection :: encode (Message *msg) {

  TranSAP::Dtreq *msg = new TranSAP::Dtreq;

  msg->cep = cep;
  msg->data = body; // used abit differently here, body contains *entire* encoded msg
  return msg;

}

Uint16 GiopPeer :: MessageError :: decode (Message *msg) {


  TranSAP::Dtind *tranmsg = msg;
  tcp_cep = tranmsg->cep;

  // Decode GIOP Message Header

  DecodeMessageHeader_1_1 (giophdr, b, len); 

  return len;
}

Message *GiopPeer :: MessageError :: encode (Message *msg) {

  TranSAP::Dtreq *msg = new TranSAP::Dtreq;

  msg->cep = cep;
  return msg;

}

Uint16 GiopPeer :: Fragment :: decode (Message *msg) {

}

Message *GiopPeer :: Fragment :: encode (Message *msg) {


}

Sint16 GiopPeer :: identify (Message *msg) {
  
  Sint16 type;
  //Checking the message type. Only data indications are
  //decoded. Others are saved to task as they are.

  type = -1;
  switch (msg->getType()) 
    {
    case TranSAP::cind :
      return -1;
      break;
    case TranSAP::cconf :
      return -1;
      break;
    case TranSAP::dind :
      return -1;
      break;
    case TranSAP::dtind :      
      //Taking data to frame b and getting first out of frame to identify
      //the message.
      Frame &b = ((TranSAP::Dtind *) msg)->data;
      type = b.read (7);
      break;
    }
  return type;  
}


