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

$Log: iiopent.C,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:23  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.9  2001/06/08 08:40:09  bilhanan
Full licence clause added into doors 0.1


*/
//
//  =========================================================================
//  File:     iiopent.C
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================


#include <doors/util.h>


#include <doors/iiopdefs.h>

#if (USE_SYMBOLS && USE_SYMBOLS_IIOP)
#include <doors/hsi.h>
#endif

#include <doors/iiopent.h>
#include <doors/iiopconn.h>


IiopEntity :: IiopEntity(const char *n, Address *listenaddr) : 
  EventTask (n, Scheduler::Instance()),
  edevice(IoHandler::Instance(), this),
  entity (this), 
  down (this, &peer), 
  peer (this, &down),
  connections (this),
  connMux (new SinglePropagator (new PortList), new SinglePropagator (new PortList)),
  up (this)
{
  //  entity.connect(&edevice.iFace);
  // edevice.connect(listenaddr);
  
  // memset (connections, 0, sizeof (connections)); 
  
#if (USE_SYMBOLS && USE_SYMBOLS_IIOP)
  STATIC_SYMBOL (entity, this);
  STATIC_SYMBOL (up, this);
#endif
}


IiopEntity :: ~IiopEntity (void) 
{
  for (int i = 0; i < 256; i++) {
    if (connections [i] != 0) {
      delete connections [i];
      connections [i] = 0;
    }
  }
#if (USE_SYMBOLS && USE_SYMBOLS_IIOP) 
  DELETE_SYMBOL (this);
#endif 
}

OdBoolean IiopEntity :: execute (EventType type, Message *msg) 
{
  
#if (USE_SYMBOLS && USE_SYMBOLS_IIOP) 
  TRACE_MSG(msg);
#endif
  
  Message *bMsg = 0;

  if (msg->getTarget() == &up) {
    IiopSAP :: Req *req = NULL;

    switch (msg->getType()) 
      {
      case IiopSAP::upRequest:
      case IiopSAP::upReply:
      case IiopSAP::upCancelRequest:
      case IiopSAP::upLocateRequest: 
      case IiopSAP::upLocateReply:
      case IiopSAP::upCloseConnection:
      case IiopSAP::upFragment:  
	DEBUGINFO("IiopEntity,up,IiopSAP::upMsg");
	req = (IiopSAP :: Req *) msg;
	if (connections[req->cid] != 0) {
	  msg->lock ();
	  connections[req->cid]->save (msg);
	}
	break;
      
      case (IiopSAP::creq): { 
	DEBUGINFO("IiopEntity,up,IiopSAP::Creq");
	IiopSAP :: Creq *cr = (IiopSAP :: Creq *) msg;
	IiopConnection *conn = new IiopConnection (0,this);
	msg->lock();
	conn->save (msg);
	break;
      }
      case (IiopSAP::cresp):  
	DEBUGINFO("IiopEntity,up,IiopSAP::CResp");
	req = (IiopSAP :: Req *) msg;
	if (connections[req->cid] != 0) {
	  msg->lock ();
	  connections[req->cid]->save (msg);
	}
	break;

      case (IiopSAP::dreq):
	DEBUGINFO("IiopEntity,up,IiopSAP::Dreq");
	req = (IiopSAP :: Req *) msg;
	if (connections[req->cid] != 0) {
	  msg->lock ();
	  connections[req->cid]->save (msg);
	}
	break;
	
      default:
	break;
      }
  }
  
  else if (msg->getTarget() == &entity) {
    
    switch (msg->getType()) 
      {
      case (DevIf :: open) : {
	DEBUGINFO("IiopEntity,entity,DevIf::Open");
	DevIf::Open *tmp = (DevIf::Open *) msg;
	connections [tmp->fd] = new IiopConnection (tmp->fd, this); 
	if (connections[tmp->fd] != 0) {
	  Port *t = msg->getSender();
	  msg->mark (t, &connections[tmp->fd]->connection);
	  msg->lock ();
	  connections[tmp->fd]->save (msg);
	}
	break;
      }
      case  (DevIf :: close) : 
	DEBUGINFO("IiopEntity,entity,DevIf::Close");
	break;
	
      case  (DevIf :: read) : 
	break;
	
      default:
	break;
      }
  } 
  else {
    
    DevIf::Open *tmpo = NULL;
    DevIf :: Close *tmp = NULL;
    DEBUGINFO(msg->getType());
    DEBUGINFO("IiopEntity,other");
    if (!msg) return OdFalse;
    
    switch (msg->getType ()) 
      {
      case (DevIf :: open) :  
	DEBUGINFO("IiopEntity,other,DevIf::Open");
	tmpo = (DevIf::Open*) msg;
	if (connections [tmpo->fd] == 0) {
	  // everything is OK and instance is moved to the table
	  connections [tmpo->fd] = (IiopConnection *) tmpo->device->getTask();
	} else {
	  // error, this should never happen
	}
	break;
	
      case (DevIf :: close) : 
	DEBUGINFO("IiopEntity,other,DevIf::close");
	tmp = (DevIf :: Close *) msg;      
	
	if (tmp->fd > 0 && connections [tmp->fd]) {
	  // normal closing of the connection
#if (USE_SYMBOLS && USE_SYMBOLS_IIOP) 
	  DELETE_SYMBOL (connections [tmp->fd]);
#endif
	  delete connections [tmp->fd];
	  connections [tmp->fd] = 0;
	} 
	else if (tmp->fd <0 ){
	  // error in socket and file descriptor is no longer valid.
	  // we must check every connection to find the one to be
	  // deleted.
	  for (int i = 0; i < 256; i++) {
	    if (connections [i] == (tmp->device->getTask())) {
#if (USE_SYMBOLS && USE_SYMBOLS_IIOP) 
	      DELETE_SYMBOL (connections [i]);
#endif 
	      delete connections [i];
	      connections [i] = 0;
	    }
	  }
	} else {
	  Otask *t = tmp->device->getTask();
	  if (t) {
#if (USE_SYMBOLS && USE_SYMBOLS_IIOP)   
	    DELETE_SYMBOL (t);
#endif
	    delete t;
	  }
	}
	break;
	
      default:
	DEBUGINFO(msg->getType());
	break;
      } // switch  
  } 
  
  if(!msg->isLocked())
    delete msg;
  return OdFalse;
}



IiopConnection *IiopEntity :: getConnection (Uint32 i) 
{
  return connections [i];
}


Uint8 IiopEntity :: ByteOrder(void) 
{
  Uint16 test = 0x0102;
  
  if (((char *)&test)[0] == 1){
    return 0;
  }
  return 1;
}




