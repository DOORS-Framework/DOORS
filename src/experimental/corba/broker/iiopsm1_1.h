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

$Log: iiopsm1_1.h,v $
Revision 1.2  2002/07/23 14:32:47  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:38  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.8  2001/06/08 08:40:15  bilhanan
Full licence clause added into doors 0.1


*/
//  =========================================================================
//  File:     iiopsm1_1.h
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================



#ifndef IIOPSM_1_1_H
#define IIOPSM_1_1_H

#include <doors/iiopsm.h>
#include <doors/giop1_1.h>


class IiopSm_1_1 : public Giop_1_1, public IiopSm {
public: 
    
  IiopSm_1_1(void){
    version = 11;
  };
  
  bool run(Message *msg, Sint16 *iiopstate, IiopConnection* connection)
    {
      bool retu = true;
      DEBUGINFO("IiopSm_1_1::run, up");
      if(!connection || !msg || !state) {
	ERROREXIT("Internal error. Missing parameter in IiopSm_1_1::run()");
      }
      if(((IiopSAP::UpCloseConnection *)msg)->version != 1){
	ERROR("Wrong version. This is handler for version 1.1");
      }   
      conn = connection;
      state = iiopstate;
      if(conn->mode == IiopConnection::MODE_CLIENT) {
	switch(msg->getType()){
	case IiopSAP::upRequest :
	  retu = ready_UP_Request (msg); break;
	case IiopSAP::upCancelRequest :
	  retu = ready_UP_cancelRequest (msg); break;      
	case IiopSAP::upLocateRequest :
	  retu = ready_UP_locateRequest (msg); break;
	case IiopSAP::upFragment :
	  retu = ready_UP_fragment (msg); break;
      default :
	ERROR("Wrong message type. This is a version 1.1 client !");
	break;
	}
      } else if(conn->mode == IiopConnection::MODE_SERVER) {
	switch(msg->getType()){
	case IiopSAP::upReply :
	  retu = ready_UP_Reply (msg); break;
	case IiopSAP::upLocateReply :
	  retu = ready_UP_locateReply (msg); break;
	case IiopSAP::upCloseConnection :
	  retu = ready_UP_closeConnection (msg); break;
	case IiopSAP::upFragment :
	  retu = ready_UP_fragment (msg); break;
	default:
	  ERROR("Wrong message type. This is a version 1.1 server !");
	  break;
	}
      } else {
	ERROR("Internal error. Are we version 1.1 client or a server ???");
      }
      
      return retu;
    };
  
  
  
  bool run(Frame *frm, Sint16 *iiopstate, IiopConnection* connection)
    { 
      bool retu = true;
      DEBUGINFO("IiopSm_1_1::run, peer");    
      
      if(!connection || !frm || !iiopstate) {
	ERROR("Internal error. Missing parameter in IiopSm_1_1::run");
	return retu;
      }     
      conn = connection;
      state = iiopstate;
      
      
      Giop_1_1::IiopMsg mh;
      if(PeekMsgHeader(frm, (Giop::MessageHeader &)(mh.msghdr)) != false){
	//ERROR in header
	return true;
      }
      if(mh.morebit() && 
	 (mh.msghdr.message_type == Giop_1_1::request ||
	  mh.msghdr.message_type == Giop_1_1::reply)){
	conn->fragbuf_1_1.msg = frm;
	return retu;
      }
      if(mh.msghdr.message_type == Giop_1_1::fragment){
	frm->delFirst(12);
	conn->fragbuf_1_1.msg->combine(*frm);
	if(mh.morebit()){
	  return retu;
	}
	frm = conn->fragbuf_1_1.msg;
      }
      
      Giop::IiopMsg *msg;
      msg = Decode(frm);
      
      if(((Giop_1_1::MessageHeader *)msg)->GIOP_version.minor != 1){
	ERROR("Wrong version. This is handler for version 1.1");
      }
      
      if(conn->mode == IiopConnection::MODE_CLIENT) {
	switch(msg->msghdr.message_type){
	case Giop_1_1::reply :
	  retu = ready_PEER_Reply (msg); break;
	case Giop_1_1::locateReply :
	  retu = ready_PEER_locateReply (msg); break;
	case Giop_1_1::closeConnection :
	  retu = ready_PEER_closeConnection (msg); break;
	case Giop_1_1::messageError :
	  retu = ready_PEER_messageError (msg); break;  
	case Giop_1_1::fragment :
	  // retu = ready_PEER_fragment (msg); break;
	default:
	  PEERERROR("Wrong message type. This is a version 1.1 client !");
	  break;
	}
      } 
      else if(conn->mode == IiopConnection::MODE_SERVER) {
	switch(msg->msghdr.message_type){
	case Giop_1_1::request :
	  retu = ready_PEER_Request (msg); break;
	case Giop_1_1::cancelRequest :
	  retu = ready_PEER_cancelRequest (msg); break; 
	case Giop_1_1::locateRequest :
	  retu = ready_PEER_locateRequest (msg); break;
	case Giop_1_1::messageError :
	  retu = ready_PEER_messageError (msg); break;
	case Giop_1_1::fragment :
	  // retu = ready_PEER_fragment (msg); break;  
	default:
	  PEERERROR("Wrong message type. This is a version 1.1 server !");
	  break;
	}
      }
      return retu;
    };
  
protected:
  
  enum states { closed = 0, ready, wfFragment, other};
  
  Frame* Encode( Giop :: IiopMsg *msg );
  Giop::IiopMsg* Decode( Frame *msg );
  
  bool ready_UP_Request (Message *msg);
  bool ready_UP_Reply (Message *msg);
  bool ready_UP_cancelRequest (Message *msg);
  bool ready_UP_locateRequest (Message *msg);
  bool ready_UP_locateReply (Message *msg); 
  bool ready_UP_closeConnection (Message *msg);
  bool ready_UP_fragment (Message *msg);

  bool ready_PEER_Request (Giop::IiopMsg *msg);
  bool ready_PEER_Reply (Giop::IiopMsg *msg);
  bool ready_PEER_cancelRequest (Giop::IiopMsg *msg);
  bool ready_PEER_locateRequest (Giop::IiopMsg *msg);
  bool ready_PEER_locateReply (Giop::IiopMsg *msg);
  bool ready_PEER_cancelReply (Giop::IiopMsg *msg); 
  bool ready_PEER_closeConnection (Giop::IiopMsg *msg);
  bool ready_PEER_messageError (Giop::IiopMsg *msg);
  //bool ready_PEER_fragment (Giop::IiopMsg *msg);

  void SystemExceptionReply (
	    /*output parameters */
	    Giop_1_1::ReplyHeader& hdr, Frame& body, 
	    CORBA::ULong& firstByteSize,
	    /* input parameters, 2 are necessary */
	    CORBA::ULong request_id, 
	    CORBA::iiopString &exception_id,  
	    CORBA::ULong minor_code = 0,
	    CORBA::ULong completion = CORBA::COMPLETED_MAYBE,
	    IOP::ServiceContextList context = IOP::ServiceContextList(),
	    Giop_1_1::ReplyStatusType replyStatus = Giop_1_1::SYSTEM_EXCEPTION
		  ); 
private:  

  inline bool peerPutMessage( Giop_1_1 :: IiopMsg *msg) {
    DEBUGINFO("IiopSm_1_1::peerPutMessage");
    return conn->peerPutMessage( Encode( (Giop::IiopMsg *)msg) );
  };

};

#endif
