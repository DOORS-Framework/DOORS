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

$Log: iiopsm1_0.C,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:24  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.7  2001/06/08 08:40:13  bilhanan
Full licence clause added into doors 0.1


*/
//  =========================================================================
//  File:     iiopsm1_0.C
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================



#include <doors/iiopsm1_0.h>



OdBoolean IiopSm_1_0 :: ready_UP_Request (Message *msg)
{
  DEBUGINFO("IiopSm_1_0::ready_UP_Request");

  IiopSAP::UpRequest *upreq = (IiopSAP::UpRequest *) msg;
 
  Giop_1_0::Request *req = new Giop_1_0::Request(conn->byteorder);
  req->reqhdr = *(Giop_1_0::RequestHeader *)upreq->reqhdr;
  req->firstParamSize = upreq->firstParamSize;
  req->body = upreq->body; 
  
  peerPutMessage(req);
  *state = IiopSm_1_0::ready;
  return OdFalse;
}


OdBoolean IiopSm_1_0 :: ready_UP_Reply (Message *msg) 
{
  DEBUGINFO("IiopSm_1_0::ready_UP_Reply");

  IiopSAP::UpReply *rep = (IiopSAP::UpReply *) msg;
  
  Giop_1_0::Reply *rp = new Giop_1_0::Reply(conn->byteorder);
  rp->rephdr = *(Giop_1_0::ReplyHeader *)rep->rephdr;
  rp->firstParamSize = rep->firstParamSize;
  rp->body = rep->body;
    
  peerPutMessage (rp);
  *state = IiopSm_1_0::ready;
  return OdFalse;
};


OdBoolean IiopSm_1_0 :: ready_UP_cancelRequest(Message *msg) 
{
  DEBUGINFO("IiopSm_1_0::ready_UP_cancelRequest");

  IiopSAP::UpCancelRequest *crequest = (IiopSAP::UpCancelRequest *)msg;
  Giop_1_0::CancelRequest *creq = new Giop_1_0::CancelRequest(conn->byteorder);
  creq->reqhdr = *(Giop_1_0::CancelRequestHeader *)crequest->hdr;
  
  peerPutMessage(creq);
  *state = IiopSm_1_0::ready;
  return OdFalse;
}

OdBoolean IiopSm_1_0 :: ready_UP_locateRequest (Message *msg)
{
  DEBUGINFO("IiopSm_1_0::ready_UP_locateRequest");

  IiopSAP::UpLocateRequest *lrequest = (IiopSAP::UpLocateRequest *)msg;
  Giop_1_0::LocateRequest *lreq = new Giop_1_0::LocateRequest(conn->byteorder);
  lreq->reqhdr = *(Giop_1_0::LocateRequestHeader *)lrequest->reqhdr;
  
  peerPutMessage(lreq);
  *state = IiopSm_1_0::ready;
  return OdFalse;
}

OdBoolean IiopSm_1_0 :: ready_UP_locateReply (Message *msg)
{
  DEBUGINFO("IiopSm_1_0::ready_UP_locateReply");

  IiopSAP::UpLocateReply *lreply = (IiopSAP::UpLocateReply *)msg;
  Giop_1_0::LocateReply *lrep = new  Giop_1_0::LocateReply(conn->byteorder);
  lrep->rephdr = *(Giop_1_0::LocateReplyHeader *)lreply->rephdr;
  
  peerPutMessage(lrep);
  *state = IiopSm_1_0::ready;
  return OdFalse;
}


OdBoolean IiopSm_1_0 :: ready_UP_closeConnection (Message *msg) 
{
  DEBUGINFO("IiopSm_1_0::ready_UP_closeConnection");

  IiopSAP::UpCloseConnection *close = (IiopSAP::UpCloseConnection *) msg;

  Giop_1_0::CloseConnection *cl = 
    new Giop_1_0::CloseConnection(conn->byteorder);
  
  peerPutMessage (cl);

  IiopSAP::Dreq *dr = (IiopSAP::Dreq *) conn->up.create (IiopSAP::dreq);
  conn->save (dr);
  
  *state = IiopSm_1_0::closed;
  
  return OdFalse;
};


OdBoolean IiopSm_1_0 :: ready_PEER_Request (Giop::IiopMsg *msg) 
{
  DEBUGINFO("IiopSm_1_0::ready_PEER_Request");
  Giop_1_0 :: Request *req = (Giop_1_0 :: Request *) msg; 
  
  IiopSAP::Request *ri = 
    (IiopSAP::Request *) conn->up.create (IiopSAP::request);
  
  ri->version = 0;
  Giop_1_0::RequestHeader *reqhdr = new Giop_1_0::RequestHeader();
  *reqhdr = req->reqhdr;
  ri->reqhdr = (Giop::RequestHeader *)reqhdr;
  ri->body_offset = req->body_offset;
  ri->body = req->body;
  ri->cid = conn->getCep();
  //ri->serviceId = conn->serviceId;
  
  conn->up.putMessage (ri);
  *state = IiopSm_1_0 :: ready;
  
  return OdFalse;
};


OdBoolean IiopSm_1_0 :: ready_PEER_Reply (Giop::IiopMsg *msg) 
{
  DEBUGINFO("IiopSm_1_0::ready_PEER_Reply");
  Giop_1_0 :: Reply *rep = (Giop_1_0 :: Reply *) msg; 
  
  IiopSAP::Reply *re = 
      (IiopSAP::Reply *) conn->up.create (IiopSAP::reply);
    
  re->version = 0;
  Giop_1_0::ReplyHeader *rephdr = new Giop_1_0::ReplyHeader();
  *rephdr = rep->rephdr;
  re->rephdr = (Giop::ReplyHeader *)rephdr;
  re->body_offset = rep->body_offset;
  re->body = rep->body;
  re->cid = conn->getCep();
 
  
  conn->up.putMessage (re);
  *state = IiopSm_1_0 :: ready;

  return OdFalse;
}


OdBoolean IiopSm_1_0 :: ready_PEER_cancelRequest (Giop::IiopMsg *msg) 
{
  DEBUGINFO("IiopSm_1_0::ready_PEER_cancelRequest");

  Giop_1_0::CancelRequest *dr = (Giop_1_0::CancelRequest *) msg; 
   
  /*
    if (!conn->serviceId) {
    // there is nothing to cancel, so we close the connection
    // we do not report any error (according to standard)
      IiopSAP::CloseConnection *cls = 
        (IiopSAP::CloseConnection*) conn->up.create(IiopSAP::closeConnection);
      cls->cid = conn->getCep();
      ready_UP_closeConnection (cls);
    } else {  
  */
  
  IiopSAP::CancelRequest *canr = 
    (IiopSAP::CancelRequest *) conn->up.create (IiopSAP::cancelRequest);
  
  canr->version = 0;
  
  Giop_1_0::CancelRequestHeader *canreqhdr = 
    new Giop_1_0::CancelRequestHeader();
  *canreqhdr = dr->reqhdr;
  canr->hdr = (Giop::CancelRequestHeader *)canreqhdr;
  canr->cid = conn->getCep();
  
  conn->up.putMessage (canr);
  *state = IiopSm_1_0:: ready;   
  
  return OdFalse;
};


OdBoolean IiopSm_1_0 :: ready_PEER_locateRequest (Giop::IiopMsg *msg)
{
  DEBUGINFO("IiopSm_1_0::ready_PEER_locateRequest");
  
  Giop_1_0::LocateRequest *lreq = (Giop_1_0 :: LocateRequest *) msg; 
  
  IiopSAP::LocateRequest *req = 
    (IiopSAP::LocateRequest *) conn->up.create (IiopSAP::locateRequest);
  req->version = 0;

  Giop_1_0::LocateRequestHeader *lreqhdr = 
    new Giop_1_0::LocateRequestHeader();
  *lreqhdr = lreq->reqhdr;
  req->reqhdr = (Giop::LocateRequestHeader *)lreqhdr;
  req->cid = conn->getCep();

  conn->up.putMessage(req);
  
  /*
    Uint16 id = serviceRepository.GetId(lreq->reqhdr.object_key);
    
    Giop_1_0::LocateReply *lreply = new Giop_1_0::LocateReply();
    lreply->rephdr.request_id = lreq->reqhdr.request_id;
    // now we do not support OBJECT_FORWARD yet !!!
    if (id)
      lreply->rephdr.locate_status = Giop_1_0::OBJECT_HERE;
    else
      lreply->rephdr.locate_status = Giop_1_0::UNKNOWN_OBJECT;
  
    if (!conn->serviceId) {
    // there is nothing under processing, let's close this connection
      IiopSAP::CloseConnection *cls = 
        (IiopSAP::CloseConnection*) conn->up.create(IiopSAP::closeConnection); 
      cls->cid = conn->getCep();
      ready_UP_closeConnection (cls);
    }
    else {
      //state = IiopSm_1_0:: ready;
    }
  */
  
  return OdTrue;
};


OdBoolean IiopSm_1_0 :: ready_PEER_locateReply (Giop::IiopMsg *msg)
{
  DEBUGINFO("IiopSm_1_0::ready_PEER_locateReply");

  Giop_1_0::LocateReply *lrep = (Giop_1_0 :: LocateReply *) msg; 
  
  IiopSAP::LocateReply *rep = 
    (IiopSAP::LocateReply *) conn->up.create (IiopSAP::locateReply);
  rep->version = 0;

  Giop_1_0::LocateReplyHeader *lrephdr = 
    new Giop_1_0::LocateReplyHeader();
  *lrephdr = lrep->rephdr;
  rep->rephdr = (Giop::LocateReplyHeader *)lrephdr;
  rep->body_offset = lrep->body_offset;
  rep->body = lrep->body;
  rep->cid = conn->getCep();

  conn->up.putMessage(rep);

  return OdFalse;
}


OdBoolean IiopSm_1_0 :: ready_PEER_closeConnection (Giop::IiopMsg *msg)
{
  DEBUGINFO("IiopSm_1_0::ready_PEER_closeConnection");

  Giop_1_0::CloseConnection *cl = (Giop_1_0 :: CloseConnection *) msg; 
  
  IiopSAP::CloseConnection *close = 
      (IiopSAP::CloseConnection *) conn->up.create (IiopSAP::closeConnection);
    
  close->version = 0;
  close->cid = conn->getCep();
  conn->up.putMessage(close);
  
  IiopSAP::CloseConnection *cls = 
    (IiopSAP::CloseConnection*) conn->up.create(IiopSAP::closeConnection); 
  cls->cid = conn->getCep();
  ready_UP_closeConnection (cls);
  
  return OdFalse;
}


OdBoolean IiopSm_1_0 :: ready_PEER_messageError (Giop::IiopMsg *msg) 
{
  DEBUGINFO("IiopSm_1_0::ready_PEER_messageError");
  
  Giop_1_0::MessageError *merr = (Giop_1_0::MessageError *) msg;

  IiopSAP::MessageError *err = 
    (IiopSAP::MessageError *) conn->up.create (IiopSAP::messageError);
  err->version = 0;
  err->cid = conn->getCep();
  conn->up.putMessage(err);

  return OdFalse;
};





