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

$Log: iiopsm1_0.h,v $
Revision 1.2  2002/07/23 14:32:47  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:38  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.6  2001/06/08 08:40:13  bilhanan
Full licence clause added into doors 0.1


*/
//  =========================================================================
//  File:     iiopsm1_0.h
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     Statemachine definition for GIOP/IIOP 1.0. 
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================



#ifndef IIOPSM_1_0_H
#define IIOPSM_1_0_H

#include <doors/iiopsm.h>
#include <doors/giop1_0.h>



class IiopSm_1_0 : public Giop_1_0, public IiopSm {
public: 
  
  IiopSm_1_0(void){
    version = 10;
  };
  
  bool run(Message *msg, Sint16 *iiopstate, IiopConnection* connection)
    {
      bool retu = true;
      DEBUGINFO("IiopSm_1_0::run, up");
      if(!connection || !msg || !state) {
	ERROREXIT("Internal error. Missing parameter in IiopSm_1_0::run()");
      }
      if(((IiopSAP::UpCloseConnection *)msg)->version != 0){
	ERROR("Wrong version. This is handler for version 1.0");
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
	default :
	  ERROR("Wrong message type. This is a version 1.0 client !");
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
	default:
	  ERROR("Wrong message type. This is a version 1.0 server !");
	  break;
	}
      } else {
	ERROR("Internal error. Are we version 1.0 client or a server ???");
      }
      
      return retu;
    };
  
  

  bool run(Frame *frm, Sint16 *iiopstate, 
		IiopConnection* connection)
    { 
      bool retu = true ;
      DEBUGINFO("IiopSm_1_0::run, peer");    
      
      if(!connection || !frm || !iiopstate) {
	ERROR("Internal error. Missing parameter in IiopSm_1_0::run");
	return retu;
      }     
      conn = connection;
      state = iiopstate;

      //if(*state != ready) return false;

      Giop_1_0 :: IiopMsg *msg;
      msg = (Giop_1_0 :: IiopMsg *)Decode(frm); 
      
      if(conn->mode == IiopConnection::MODE_CLIENT) {
	switch(msg->msghdr.message_type){
	case Giop_1_0::reply :
	  retu = ready_PEER_Reply (msg); break;
	case Giop_1_0::locateReply :
	  retu = ready_PEER_locateReply (msg); break;
	case Giop_1_0::closeConnection :
	  retu = ready_PEER_closeConnection (msg); break;
	case Giop_1_0::messageError :
	  retu = ready_PEER_messageError (msg); break;  
	default:
	  PEERERROR("Wrong message type " << (Uint32) msg->msghdr.message_type 
		    << ". This is a version 1.0 client !");
	  break;
	}
      } 
      else if(conn->mode == IiopConnection::MODE_SERVER) {
	switch(msg->msghdr.message_type){
	case Giop_1_0::request :
	  retu = ready_PEER_Request (msg); break;
	case Giop_1_0::cancelRequest :
	  retu = ready_PEER_cancelRequest (msg); break; 
	case Giop_1_0::locateRequest :
	  retu = ready_PEER_locateRequest (msg); break;
	case Giop_1_0::messageError :
	  retu = ready_PEER_messageError (msg); break; 
	default:
	  PEERERROR("Wrong message type " << (Uint32) msg->msghdr.message_type
		    << ". This is a version 1.0 server !");
	  break;
	}
      }
      return retu;
    };
  
protected:
  
  enum states { closed = 0, ready, other};
  
  Frame* Encode( Giop :: IiopMsg *msg );
  Giop::IiopMsg* Decode( Frame *msg );
  
  bool ready_UP_Request (Message *msg);
  bool ready_UP_Reply (Message *msg);
  bool ready_UP_cancelRequest (Message *msg);
  bool ready_UP_locateRequest (Message *msg);
  bool ready_UP_locateReply (Message *msg); 
  bool ready_UP_closeConnection (Message *msg);
  
  bool ready_PEER_Request (Giop::IiopMsg *msg);
  bool ready_PEER_Reply (Giop::IiopMsg *msg);
  bool ready_PEER_cancelRequest (Giop::IiopMsg *msg);
  bool ready_PEER_locateRequest (Giop::IiopMsg *msg);
  bool ready_PEER_locateReply (Giop::IiopMsg *msg);
  bool ready_PEER_cancelReply (Giop::IiopMsg *msg); 
  bool ready_PEER_closeConnection (Giop::IiopMsg *msg);
  bool ready_PEER_messageError (Giop::IiopMsg *msg);


  void SystemExceptionReply (
	    /*output parameters */
	    Giop_1_0::ReplyHeader& hdr, Frame& body, 
	    CORBA::ULong& firstByteSize,
	    /* input parameters, 2 are necessary */
	    CORBA::ULong request_id, 
	    CORBA::iiopString &exception_id,  
	    CORBA::ULong minor_code = 0,
	    CORBA::ULong completion = CORBA::COMPLETED_MAYBE,
	    IOP::ServiceContextList context = IOP::ServiceContextList(),
	    Giop_1_0::ReplyStatusType replyStatus = Giop_1_0::SYSTEM_EXCEPTION
	    )
    {
      hdr.service_context = context;
      hdr.request_id = request_id;
      hdr.reply_status = replyStatus;
      firstByteSize = sizeof(CORBA::ULong); /* the size of the string*/
      Giop_1_0::SystemExceptionReplyBody ebody(exception_id, minor_code, 
					       completion);
      Uint32 len;
      body.destroy();
      EncodeSysExRepBody(ebody,body,len);
    }; 
  
private:  

  inline bool peerPutMessage( Giop_1_0 :: IiopMsg *msg) {
    DEBUGINFO("IiopSm_1_0::peerPutMessage");
    return conn->peerPutMessage( Encode((Giop::IiopMsg *)msg) );
  };

};

#endif
