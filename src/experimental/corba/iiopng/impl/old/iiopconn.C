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

$Log: iiopconn.C,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:22  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.11  2001/06/08 08:40:08  bilhanan
Full licence clause added into doors 0.1


*/
//
//  =========================================================================
//  File:     iiopconn.C
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================


#include <doors/iiopdefs.h>


#if (USE_SYMBOLS && USE_SYMBOLS_IIOP) 
#include <doors/hsi.h>
#endif

#include <doors/util.h>
#include <doors/core.h>

#include <doors/iiopconn.h>
#include <doors/iiopsm1_0.h>

#include <doors/giop1_2.h>
#include <doors/iiopent.h>



IiopMsgBuffer :: IiopMsgBuffer (void)
  : buflen(0), swap(0)
{
}

void IiopMsgBuffer :: Add(Frame &frame)
{
  buflen = buflen + frame.length();
  buffer.combine(frame);
  return;
}


Frame* IiopMsgBuffer :: ExtractMsg(void)
{
  Frame* msg = NULL;
  Uint32 msglen = 0;
  
  if(buflen > 11){
    swap = (Uint8)1 & (Uint8)buffer.read (6);

    msglen = (Uint32) buffer.read (8) << 24 
      | (Uint32) buffer.read (9) << 16
      | (Uint32) buffer.read (10) << 8
      | (Uint32) buffer.read (11);

    if(swap) SWAP4(msglen);
    msglen += 12;
    if(buflen > msglen){
      msg = &(buffer.split(msglen));
      buflen = buflen - msglen;
    }
    else if(buflen == msglen){ 
      msg = &(buffer.copy());
      buffer.destroy();
      buflen = 0;
    }
  }
  return msg;
}


Frame* IiopMsgBuffer :: Operate(Frame &frame)
{
  Add(frame);
  return ExtractMsg();
}



IiopConnection :: IiopConnection (Uint32 id, IiopEntity *e):
  EventTask (0, e->getScheduler ()), 
  connection (this),
  cdevice(IoHandler::Instance(), this),
  entity (e),
  sm_1_0(&e->sm_1_0),
  sm_1_1(&e->sm_1_1),
  sm_1_2(&e->sm_1_2),
  respTimer (this, OTime (900)), 
  cep (id),
  up (e->up),
  mode(MODE_UNDEFINED), byteorder(e->ByteOrder()),
  state(closed), state_1_0(closed), state_1_1(closed), state_1_2(closed)
{

  char n[20];
  sprintf (n, "%s.conn.%d", entity->getName(), cep); 
  name.set (n);

  connection.connect(&cdevice.iFace);
  cdevice.connect(id);
  cdevice.readBytes(StreamDevice::immediate);

#if (USE_SYMBOLS && USE_SYMBOLS_IIOP) 
  DYNAMIC_SYMBOL (this, n, e);
  STATIC_SYMBOL (connection, this);
  STATIC_SYMBOL (cep, this);
  STATIC_SYMBOL (respTimer, this);
  STATIC_SYMBOL (state, this);
#endif;
}



IiopConnection :: IiopConnection (IiopEntity *e):
EventTask (0, e->getScheduler ()), 
connection (this),
cdevice (IoHandler::Instance(), this),
entity (e),
sm_1_0(&e->sm_1_0),
sm_1_1(&e->sm_1_1),
sm_1_2(&e->sm_1_2),
respTimer (this, OTime (900)),
cep (-1),  
up (e->up),
mode(MODE_UNDEFINED), byteorder(e->ByteOrder()),
state(closed), state_1_0(closed), state_1_1(closed), state_1_2(closed)
{
  char n[20];
  connection.connect(&cdevice.iFace);
  cep = cdevice.getFd();  
  sprintf (n, "%s.conn.%d", entity->getName(), cep); 
  name.set (n);

#if (USE_SYMBOLS && USE_SYMBOLS_IIOP) 
  DYNAMIC_SYMBOL (this, n, e);
  STATIC_SYMBOL (connection, this);
  STATIC_SYMBOL (cep, this);
  STATIC_SYMBOL (respTimer, this);
  STATIC_SYMBOL (state, this);
#endif
}


IiopConnection :: ~IiopConnection (void) 
{
#if (USE_SYMBOLS && USE_SYMBOLS_IIOP)  
  DELETE_SYMBOL (this);
#endif
}


OdBoolean IiopConnection :: execute (EventType type, Message *msg) 
{
#if (USE_SYMBOLS && USE_SYMBOLS_IIOP) 
  TRACE_MSG(msg);
#endif

  Message *bMsg = 0;
  
  if (type == SystemEvent) {
    DEBUGINFO("IiopConnection::execute, " << state << ", SYSEVT"); 
    switch(state){
    case closed:
      closed_default(msg); break;
    case wfSopen:
      wfSopen_timeout(msg); break;
    case wfCresp:
      wfCresp_timeout(msg); break;
    case open:
      open_default(msg); break;
    default:
      default_handler(msg); break;
    }    
  } 
  else if (msg->getTarget() == &up) {
    
    DEBUGINFO("IiopConnection::execute, " << state << ", UP");
    
    int msgtype = msg->getType();
    if(state == open && msgtype < 8){
      Frame *pdu = NULL;
      switch(((IiopSAP::UpCloseConnection *)msg)->version){
      case 0:
	sm_1_0->run(msg, &state_1_0, this);
	break;
      case 1:
	sm_1_1->run(msg, &state_1_1, this);
	break;
      case 2:
	sm_1_2->run(msg, &state_1_2, this);
	break;
      default:
	ERROR("IiopConnection :: execute, Message from Up,"
	     << "Unsupported version\n");
      }
    }
    else if(msgtype == IiopSAP::creq && state == closed){
      closed_Creq (msg);
    }
    switch (msg->getType()) {     
    case IiopSAP::creq:
      break;
    case IiopSAP::dreq:
      if(state == open){
	open_Dreq(msg);
      }
      break;
    default:
      break;
    }  
  }
  else if (msg->getTarget() == &connection) {
    
    DEBUGINFO("IiopConnection::execute, " << state << ", CONN");
    
    switch (msg->getType()) { 
    case (DevIf :: read) :
      cdevice.readBytes(StreamDevice::immediate);
      DEBUGINFO("IiopConnection, connection, DevIf::read");
      Frame *frm;
      while((frm = buffer.Operate(((DevIf::Read *) msg)->data)) != NULL){ 
	DEBUGINFO("IiopConnection, connection, Giop -message extracted.");
	::FRAME_PRINT(*frm);
	Giop::MessageHeader msghdr;
	
	Byte *tmpptr = (Byte *)&msghdr;
	for(int i = 0; i < 12; ++i){
	  *tmpptr = frm->read(i);
	  ++tmpptr;
	}
	int val = memcmp(&msghdr,"GIOP",4);
	val += (msghdr.GIOP_version.major == 1 ) ? 0 : 1;
	if(val) {
	  sm_1_2->send_MessageError();
	  ERROR("IiopConnection::execute, Wrong 'magic' or major version");
	  delete frm;
	  continue;
	}
		
	switch(msghdr.GIOP_version.minor){
	case 0:
	  sm_1_0->run(frm, &state_1_0, this); break;
	case 1:
	  sm_1_1->run(frm, &state_1_1, this); break;
	case 2:
	  sm_1_2->run(frm, &state_1_2, this); break; 
	default:
	  sm_1_2->send_MessageError();
	  DEBUGINFO("IiopConnection :: execute, Message from Peer,"
		    << "Unsupported version\n");
	  break;
	} // switch 
      } // while
      break;
      
    case (DevIf :: open) :
      DEBUGINFO("IiopConnection, connection, DevIf::open");
      switch(state){
      case closed:
	closed_OPEN(msg); break;
      case wfSopen:
	wfSopen_OPEN(msg); break;
      case open:
	open_OPEN(msg); break;
      default:
	default_handler(msg); break;
      }
      break;
      
    case (DevIf :: close) :
      DEBUGINFO("IiopConnection, connection, DevIf::close");
      switch(state){
      case closed:
	closed_default(msg); break;
      case wfSopen:
	wfSopen_CLOSE(msg); break;
      case open:
	open_CLOSE(msg); break;
      default:
	default_handler(msg);break;
      }
      break;
      
    }
    
  }
  else {
    cout << "ERROR" << endl; cout.flush(); 
  }
  if(!msg->isLocked()){
    delete msg;
  }
  return OdTrue;
}    


OdBoolean IiopConnection :: peerPutMessage(Frame *pdu)
{
  DEBUGINFO("IiopConnection::peerPutMessage");
  OdBoolean retu = OdTrue;
  ::FRAME_PRINT(*pdu);
  retu = cdevice.write(*pdu);
  //delete psu;
  return retu;
}



OdBoolean IiopConnection :: default_handler (Message *msg) 
{
  return OdFalse;
}


OdBoolean IiopConnection :: closed_Creq (Message *msg) 
{
  DEBUGINFO("IiopConnection :: closed_Creq");

  mode = MODE_CLIENT;
  IiopSAP :: Creq *cr = (IiopSAP :: Creq *) msg;
  respTimer.start ();

  cdevice.connect(&(cr->destination));
  cdevice.readBytes(StreamDevice :: immediate);

  state = IiopConnection :: wfSopen;
  return OdFalse;
}


OdBoolean IiopConnection :: closed_OPEN (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection :: closed_OPEN");

  mode = MODE_SERVER;
  IiopSAP :: Cind *ci = (IiopSAP :: Cind *) up.create (IiopSAP :: cind);
  ci->cid = cep;
  //ci->addr = *(new InetAddr(4000, getenv("HOST")));
  ci->source = *(InetAddr*)cdevice.getRemoteAddress();
  up.putMessage(ci);

  //state = IiopConnection :: wfCresp;
  //We accept all
  state = IiopConnection :: open;
  state_1_0 = state_1_1 = state_1_2 = IiopConnection :: open;
  
  return OdFalse;
}




OdBoolean IiopConnection :: closed_default (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection::closed_default");
  return OdFalse;
}



OdBoolean IiopConnection :: wfSopen_OPEN (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection::wfSopen_OPEN");

  char buf[80];
  DevIf :: Open *tmp = (DevIf :: Open *) msg;    
  
  cep = tmp->fd;
  sprintf (buf, "%s.conn.%d", getName (), cep);
  name.set (buf);
  IiopSAP :: Cconf *cc = (IiopSAP :: Cconf *) up.create (IiopSAP :: cconf);
  cc->cid = cep;
//  cc->addr = *(new InetAddr(4001, getenv("HOST")));
  cc->source = *(InetAddr*)cdevice.getRemoteAddress();
  Message *bMsg = (Message *) cc;
  up.putMessage (bMsg);

  msg->lock();
  entity->save (msg);
  
  respTimer.start ();
  state = IiopConnection :: open;
  state_1_0 = state_1_1 = state_1_2 = IiopConnection :: open;
  return OdTrue;
}


OdBoolean IiopConnection :: wfSopen_CLOSE (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection::wfSopen_CLOSE");

  DevIf :: Close *sDisc = (DevIf :: Close *) msg;
  
  IiopSAP :: Dind *di = (IiopSAP :: Dind *) up.create (IiopSAP :: dind);
  di->cid = cep;
  up.putMessage (di);

  msg->lock ();
  entity->save (msg);
  
  mode = MODE_UNDEFINED;
  state = IiopConnection :: closed;
  return OdTrue;
}


OdBoolean IiopConnection :: wfSopen_timeout (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection::wfSopen_timeout");

  DevIf :: Close *sMsg = new DevIf :: Close;
  sMsg->fd = cep;
  sMsg->device = &cdevice;
  sMsg->mark (&cdevice.iFace, &cdevice.iFace);
  sMsg->lock ();

  Message *bMsg = (Message *) sMsg;
  return wfSopen_CLOSE (bMsg);
}


OdBoolean IiopConnection :: wfSopen_default (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection::wfSopen_default");
  return OdFalse;
}


OdBoolean IiopConnection :: wfCresp_Cresp (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection::wfCresp_Cresp");
  IiopSAP::Cresp *cr = (IiopSAP::Cresp *) msg;
  respTimer.stop ();
  cdevice.readBytes (StreamDevice :: immediate);

  state = IiopConnection :: open;
  state_1_0 = state_1_1 = state_1_2 = open;
  return OdFalse;
}


OdBoolean IiopConnection :: wfCresp_Dreq (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection::wfCresp_Dreq");

  return open_Dreq (msg);
}


OdBoolean IiopConnection :: wfCresp_CLOSE (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection :: wfCresp_CLOSE");

  DevIf :: Close *sDisc = (DevIf :: Close *) msg;
  Message *bMsg = 0;
  
  IiopSAP :: Dind *di = (IiopSAP :: Dind *) up.create (IiopSAP :: dind);
  di->cid = cep;

  bMsg = (Message *) di;
  up.putMessage (bMsg);

  msg->lock ();
  entity->save (msg);

  state = IiopConnection :: closed;
  return OdTrue;
}


OdBoolean IiopConnection :: wfCresp_timeout (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection :: wfCresp_timeout");

  DevIf :: Close *sMsg = new DevIf :: Close;
  sMsg->fd = cep;
  sMsg->device = &cdevice;
  sMsg->mark (&cdevice.iFace, &cdevice.iFace);
  sMsg->lock ();
  Message *bMsg = (Message *) sMsg;

  return wfSopen_CLOSE (bMsg);
}


OdBoolean IiopConnection :: wfCresp_default (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection :: wfCresp_default");

  return OdFalse;
}

OdBoolean IiopConnection :: open_OPEN (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection :: open_OPEN");

  return OdFalse;
}

OdBoolean IiopConnection :: open_Dreq (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection :: open_Dreq");
  
  OdBoolean rc = OdFalse;
  
  if (cdevice.writeBuffer() == 0) {
    ONS_MT_KILLTHREAD(&cdevice);
    ONS_MT_WAIT_EXITBARRIER(&cdevice);
    cdevice.close();
  } else {
    msg->lock();
    save (msg);
    rc = OdTrue;
  }
  
  return rc;  
}

OdBoolean IiopConnection :: open_CLOSE (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection :: open_CLOSE");

  printf ("Disconn received for %d \n", cep);
  return wfSopen_CLOSE (msg);
}


OdBoolean IiopConnection :: open_default (Message *msg) 
{
  DEBUGINFO( THIS << "IiopConnection :: open_default");

  return OdFalse;
}


Sint32 IiopConnection :: getCep (void) 
{
  return cep;
}



