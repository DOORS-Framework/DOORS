

//
//File: gioppdu1_0.i
//
//Date: Thu Aug 23 13:29:34 2001
//
//Commentary:
//  This is a machine generated file produced by 
// compiler. Editing is not recommended.
//
//Description:
//  This file implements part of the functions
//  of the GIOP_1_0_Peer interface. The rest of them must
//  be written manually in *.C file.
//


#include "gioppdu1_0.h"


Message *createGIOP_1_0_PeerRequest (void) {

  return new GIOP_1_0_Peer :: Request;
};


Message *createGIOP_1_0_PeerReply (void) {

  return new GIOP_1_0_Peer :: Reply;
};


Message *createGIOP_1_0_PeerCancelRequest (void) {

  return new GIOP_1_0_Peer :: CancelRequest;
};


Message *createGIOP_1_0_PeerLocateRequest (void) {

  return new GIOP_1_0_Peer :: LocateRequest;
};


Message *createGIOP_1_0_PeerLocateReply (void) {

  return new GIOP_1_0_Peer :: LocateReply;
};


Message *createGIOP_1_0_PeerCloseConnection (void) {

  return new GIOP_1_0_Peer :: CloseConnection;
};


Message *createGIOP_1_0_PeerMessageError (void) {

  return new GIOP_1_0_Peer :: MessageError;
};


Message *(*gIOP_1_0_PeerMessages []) (void) = {
  createGIOP_1_0_PeerRequest,
  createGIOP_1_0_PeerReply,
  createGIOP_1_0_PeerCancelRequest,
  createGIOP_1_0_PeerLocateRequest,
  createGIOP_1_0_PeerLocateReply,
  createGIOP_1_0_PeerCloseConnection,
  createGIOP_1_0_PeerMessageError,
  0
};


GIOP_1_0_Peer :: GIOP_1_0_Peer (EventTask *host, ReqIface *d)
: Peer (host, gIOP_1_0_PeerMessages, d) {

}


GIOP_1_0_Peer :: ~GIOP_1_0_Peer (void) {

}


GIOP_1_0_Peer :: Request :: Request (void) {

  DYNAMIC_SYMBOL (this, "request", 0);
  STATIC_SYMBOL (reqhdr, this);
  STATIC_SYMBOL (body, this);
}


GIOP_1_0_Peer :: Request :: Request (const Request &msg)
: reqhdr (msg.reqhdr), body (msg.body) {

  DYNAMIC_SYMBOL (this, "request", 0);
  STATIC_SYMBOL (reqhdr, this);
  STATIC_SYMBOL (body, this);
}


MessageType GIOP_1_0_Peer :: Request :: getType (void) const {

  return request;
}


Message * GIOP_1_0_Peer :: Request :: clone (void) const {

  return new Request (*this);
}


GIOP_1_0_Peer :: Request :: ~Request (void) {

  DELETE_SYMBOL (this);
}


GIOP_1_0_Peer :: Reply :: Reply (void) {

  DYNAMIC_SYMBOL (this, "reply", 0);
  STATIC_SYMBOL (rephdr, this);
  STATIC_SYMBOL (body, this);
}


GIOP_1_0_Peer :: Reply :: Reply (const Reply &msg)
: rephdr (msg.rephdr), body (msg.body) {

  DYNAMIC_SYMBOL (this, "reply", 0);
  STATIC_SYMBOL (rephdr, this);
  STATIC_SYMBOL (body, this);
}


MessageType GIOP_1_0_Peer :: Reply :: getType (void) const {

  return reply;
}


Message * GIOP_1_0_Peer :: Reply :: clone (void) const {

  return new Reply (*this);
}


GIOP_1_0_Peer :: Reply :: ~Reply (void) {

  DELETE_SYMBOL (this);
}


GIOP_1_0_Peer :: CancelRequest :: CancelRequest (void) {

  DYNAMIC_SYMBOL (this, "cancelrequest", 0);
  STATIC_SYMBOL (reqhdr, this);
}


GIOP_1_0_Peer :: CancelRequest :: CancelRequest (const CancelRequest &msg)
: reqhdr (msg.reqhdr) {

  DYNAMIC_SYMBOL (this, "cancelrequest", 0);
  STATIC_SYMBOL (reqhdr, this);
}


MessageType GIOP_1_0_Peer :: CancelRequest :: getType (void) const {

  return cancelrequest;
}


Message * GIOP_1_0_Peer :: CancelRequest :: clone (void) const {

  return new CancelRequest (*this);
}


GIOP_1_0_Peer :: CancelRequest :: ~CancelRequest (void) {

  DELETE_SYMBOL (this);
}


GIOP_1_0_Peer :: LocateRequest :: LocateRequest (void) {

  DYNAMIC_SYMBOL (this, "locaterequest", 0);
  STATIC_SYMBOL (reqhdr, this);
}


GIOP_1_0_Peer :: LocateRequest :: LocateRequest (const LocateRequest &msg)
: reqhdr (msg.reqhdr) {

  DYNAMIC_SYMBOL (this, "locaterequest", 0);
  STATIC_SYMBOL (reqhdr, this);
}


MessageType GIOP_1_0_Peer :: LocateRequest :: getType (void) const {

  return locaterequest;
}


Message * GIOP_1_0_Peer :: LocateRequest :: clone (void) const {

  return new LocateRequest (*this);
}


GIOP_1_0_Peer :: LocateRequest :: ~LocateRequest (void) {

  DELETE_SYMBOL (this);
}


GIOP_1_0_Peer :: LocateReply :: LocateReply (void) {

  DYNAMIC_SYMBOL (this, "locatereply", 0);
  STATIC_SYMBOL (rephdr, this);
  STATIC_SYMBOL (body, this);
}


GIOP_1_0_Peer :: LocateReply :: LocateReply (const LocateReply &msg)
: rephdr (msg.rephdr), body (msg.body) {

  DYNAMIC_SYMBOL (this, "locatereply", 0);
  STATIC_SYMBOL (rephdr, this);
  STATIC_SYMBOL (body, this);
}


MessageType GIOP_1_0_Peer :: LocateReply :: getType (void) const {

  return locatereply;
}


Message * GIOP_1_0_Peer :: LocateReply :: clone (void) const {

  return new LocateReply (*this);
}


GIOP_1_0_Peer :: LocateReply :: ~LocateReply (void) {

  DELETE_SYMBOL (this);
}


GIOP_1_0_Peer :: CloseConnection :: CloseConnection (void) {

  DYNAMIC_SYMBOL (this, "closeconnection", 0);
  STATIC_SYMBOL (, this);
}


GIOP_1_0_Peer :: CloseConnection :: CloseConnection (const CloseConnection &msg)
:  (msg.) {

  DYNAMIC_SYMBOL (this, "closeconnection", 0);
  STATIC_SYMBOL (, this);
}


MessageType GIOP_1_0_Peer :: CloseConnection :: getType (void) const {

  return closeconnection;
}


Message * GIOP_1_0_Peer :: CloseConnection :: clone (void) const {

  return new CloseConnection (*this);
}


GIOP_1_0_Peer :: CloseConnection :: ~CloseConnection (void) {

  DELETE_SYMBOL (this);
}


GIOP_1_0_Peer :: MessageError :: MessageError (void) {

  DYNAMIC_SYMBOL (this, "messageerror", 0);
  STATIC_SYMBOL (, this);
}


GIOP_1_0_Peer :: MessageError :: MessageError (const MessageError &msg)
:  (msg.) {

  DYNAMIC_SYMBOL (this, "messageerror", 0);
  STATIC_SYMBOL (, this);
}


MessageType GIOP_1_0_Peer :: MessageError :: getType (void) const {

  return messageerror;
}


Message * GIOP_1_0_Peer :: MessageError :: clone (void) const {

  return new MessageError (*this);
}


GIOP_1_0_Peer :: MessageError :: ~MessageError (void) {

  DELETE_SYMBOL (this);
}


