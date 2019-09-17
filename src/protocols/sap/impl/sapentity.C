//  Copyright 2000
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.

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

$Log: sapentity.C,v $
Revision 1.11  2003/08/11 13:20:50  bilhanan
now with ipv6 goodness.

Revision 1.10  2003/07/09 15:57:54  bilhanan
general cleanup and code rewrite.

Revision 1.9  2003/04/17 12:47:42  bilhanan
ostringstream bugfix.

Revision 1.8  2003/04/17 09:04:29  bilhanan
Forced commit, now properly supports multiple sessions, 1 udptask per session.

Revision 1.7  2003/04/17 08:55:53  bilhanan
charstring to std::string conversion,

Revision 1.6  2002/07/23 16:38:33  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.5  2002/07/02 08:03:04  hartmanj
Development

Revision 1.4  2002/06/28 14:02:30  bilhanan
global function isForTask renamed to isFor[ConnectionTaskName] to prevent
potential conflicts in future.

Revision 1.3  2002/06/18 14:38:59  ik
isForTask made static because it is used elsewhere too (link protocol).

Revision 1.2  2002/06/18 14:17:00  ik
Fixes: string->std::string etc. #includes added.

Revision 1.1  2002/06/17 06:05:37  hartmanj
Initial revision


*/

#include <sstream>
#include <string>
#include <doors/inetaddr.h>
#include <doors/udptask.h>
#include <doors/inetaddr6.h>
#include <doors/udp6task.h>
#include <doors/sapentity.h>
#include <doors/sapsession.h>
#include <doors/sapentitysm.h>
#include <doors/sapsessionsm.h>

static bool isForSAPSession (const EventTask *t, const Message *m) {

  bool rc = false;
  SAPSession *task = (SAPSession *) t;
  Uint16 tid = task->getId ();
  Uint16 connId = 0;

    if (m->getTarget () == &task->up) {
      switch (m->getType ()) {
      case (SAPEntSAP :: sesreq):
	connId = ((SAPEntSAP::Sesreq *) m)->session_id;
	break;
	
      case (SAPEntSAP :: closereq):
	connId = ((SAPEntSAP::Closereq *) m)->session_id;
	break;
       case (SAPEntSAP :: pause):
	connId = ((SAPEntSAP::Pause *) m)->session_id;
	break;
	
      case (SAPEntSAP :: resume):
	connId = ((SAPEntSAP::Resume *) m)->session_id;
	break;
      }
    }

    if (connId == tid) {
      rc = true;
    }
    
    return rc;
}

SAPEntity::SAPEntity( std::string n, Scheduler *s,SAPEntitySM *esm, const InetAddr *dest, const InetAddr6 *dest6)
  : PTask (n, s, esm),
    up(this),
    dest_(*dest),
    dest6_(*dest6),
    connections(this),
    connMux(new SinglePropagator(new PortList), new SinglePropagator(new PortList)),
    connSM(new SAPSessionSM) {

	connections.connect(connMux.createUser());	

	STATIC_SYMBOL(up,this);
}

SAPEntity::~SAPEntity() {
  
  if (connSM) {
    delete connSM;
    connSM = 0;
  }
  
}

bool SAPEntity :: SetupSession (Message *msg) {

    msg->lock ();

    if (connections.putMessage (msg) == false) {  // if there is already connection
	createConnection (((SAPEntSAP :: Sesreq *) msg)->session_id, ((SAPEntSAP :: Sesreq *) msg)->ip_version);
	connections.putMessage (msg);
    }

    return true;
}

bool SAPEntity::ready_default (Message *msg) {

    bool rc = true;
    msg->lock ();
    if (connections.putMessage (msg) == false) {
	msg->unlock ();  // Message can be deleted.
	rc = false;
    }
    
    return rc;
}

void SAPEntity:: createConnection (Uint16 cid, Uint8 ipversion)
{

  Port *p = connMux.createProvider(isForSAPSession);

  std::ostringstream sessname;
  std::ostringstream udpname;
  sessname << "SAPSession." << cid;

  if (ipversion == 4) {
  udpname << "UDPTask." << cid;
  
  InetAddr *srcaddr = new InetAddr;
  srcaddr->set (0, getenv("HOST"), 0); // bind to first free port
  
  UdpTask *udp_ = new UdpTask( "UDPTask", this->getScheduler(), IoHandler::Instance(),
			       srcaddr, 127, true );
  
  SAPSession *task = new SAPSession("SAPSession", cid, this, connSM,
				    srcaddr, udp_, ipversion);

  DYNAMIC_SYMBOL (task, sessname.str(), this);
  DYNAMIC_SYMBOL (udp_, udpname.str(), task);
  
  task->entity.connect(p);
  udp_->up.connect(&(task->down4));

  } else { //assume v6

  udpname << "UDP6Task." << cid;
  
  InetAddr6 *srcaddr6 = new InetAddr6;
  srcaddr6->set (0, getenv("HOST6"), 0); // bind to first free port
  
  Udp6Task *udp6_ = new Udp6Task( "UDPTask", this->getScheduler(), IoHandler::Instance(),
			       srcaddr6, 127, true );
  
  SAPSession *task = new SAPSession("SAPSession", cid, this, connSM,
				    srcaddr6, udp6_, ipversion);

  DYNAMIC_SYMBOL (task, sessname.str(), this);
  DYNAMIC_SYMBOL (udp6_, udpname.str(), task);
  
  task->entity.connect(p);
  udp6_->up.connect(&(task->down6));

  } 

  return;
}
