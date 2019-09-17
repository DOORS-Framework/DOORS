//Editor-Info: -*- C++ -*-
//
 //  Copyright 1999 
//      Telecom Lab, Lappeenranta University of Technology.  
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

//  $Log: linkent.C,v $
//  Revision 1.16  2003/11/14 10:20:25  bilhanan
//  CloseAll message handling and general cleanup.
//
//  Revision 1.15  2003/10/15 10:53:07  bilhanan
//  added (buggy) prototype feature for sending broadcast message from entity
//  to connection tasks.
//
//  Revision 1.14  2003/08/11 23:47:30  bilhanan
//  const char* in constructor -> std::string
//
//  Revision 1.13  2003/05/16 13:31:10  bilhanan
//  -1 changed to boolean value false.
//
//  Revision 1.12  2003/05/15 11:48:10  bilhanan
//  fixes for std c++ compliance
//
//  Revision 1.11  2003/05/08 14:21:48  bilhanan
//  ugly cout statement removed.
//
//  Revision 1.10  2003/04/25 12:03:10  bilhanan
//  using ostringstream instead of sprintf
//
//  Revision 1.9  2002/08/14 09:32:29  bilhanan
//  ONS_ macros are now renamed. Refer to top-level ChangeLog for details.
//
//  Revision 1.8  2002/07/23 16:38:31  ik
//  Od{Boolean,True,False} -> {bool,true,false}
//
//  Revision 1.7  2002/07/23 16:34:12  hartmanj
//
//  Development
//
//  Revision 1.6  2002/07/22 22:33:38  bilhanan
//  Licence clause added.
//
//  Revision 1.5  2002/07/12 08:47:43  bilhanan
//  linkrandpdu unneeded and therefore removed.
//
//  Revision 1.4  2002/07/12 06:31:46  bilhanan
//  Licence clause and Log keyword added
//

#include <string>
#include <sstream>
#include <doors/ocoremac.h>
#include <doors/hsi.h>
#include <doors/linksapbase.h>
#include <doors/linkent.h>
#include <doors/linkconn.h>
#include <doors/linkesm.h>
#include <doors/linkcsm.h>

static bool isForLinkTask (const EventTask *t, const Message *m) {

  if (m->isForMany()) {  // broadcast message
    return true;         // just accept
  }

  bool rc = false;
  LinkTask *task = (LinkTask *) t;
  Uint16 tid = task->getId ();
  Uint16 connId = 0;
  
  if (m->getTarget () == &task->up) {
    connId = ((LinkSAPBase *) m)->cid;
    
  } else if (m->getTarget () == &task->peer) {
    switch (m->getType ()) {
    case (LinkPeer :: cr):
      connId = ((LinkPeer::CR *) m)->cid;
      break;
      
    case (LinkPeer :: cc):
      connId = ((LinkPeer::CC *) m)->cid;
      break;
      
    case (LinkPeer :: data):
      connId = ((LinkPeer::DATA *) m)->cid;
      break;
      
    case (LinkPeer :: ack):
      connId = ((LinkPeer::ACK *) m)->cid;
      break;
	    
    case (LinkPeer :: dr):
      connId = ((LinkPeer::DR *) m)->cid;
      break;
    }
  }
  
  if (connId == tid || tid == 0) {
    rc = true;
  }
  
  return rc;
}


LinkEntity :: LinkEntity (std::string n, Scheduler *s, LinkEntitySM *esm) 
: PTask (n, s, esm), 
  up (this), 
  down (this, &peer), 
  peer (this, &down),
  connections (this),
  connMux (new SinglePropagator (new PortList), new SinglePropagator (new PortList)),
  connSM (new LinkTaskSM) {

    connections.connect (connMux.createUser ());

    STATIC_SYMBOL (peer, this);
    STATIC_SYMBOL (down, this);
    STATIC_SYMBOL (up, this);
}


LinkEntity :: ~LinkEntity (void) {

    _MT_SYNCMONITOR;

    if (connSM) {
	delete connSM;
        connSM = 0;
    }
}


bool LinkEntity :: ready_Creq (Message *msg) {

    msg->lock ();

    if (connections.putMessage (msg) == false) {  // if there is already connection
	createConnection (((LinkSAP :: Creq *) msg)->cid);
	connections.putMessage (msg);
    }

    return true;
}


bool LinkEntity :: ready_CR (Message *msg) {

    msg->lock ();

    if (connections.putMessage (msg) == false) {  // if there is already connection
	createConnection (((LinkPeer :: CR *) msg)->cid);
	connections.putMessage (msg);
    }

    return true;
}


// example of how to send broadcast msg from entity to connection tasks.

bool LinkEntity :: ready_CloseAll (Message *msg) {

    bool rc = true;
    msg->lock ();
    
    msg->setMultipleTargets();

    if (connections.putMessage (msg) == false) {
	msg->unlock ();  // Message can be deleted.
	rc = false;
    }
    
    return rc;
}

bool LinkEntity :: ready_default (Message *msg) {

    bool rc = true;
    msg->lock ();
    
    if (connections.putMessage (msg) == false) {
	msg->unlock ();  // Message can be deleted.
	rc = false;
    }
    
    return rc;
}

void LinkEntity :: createConnection (Uint16 cid) {

  std::ostringstream cname;
  cname << getName () << ".conn." << cid; 

  Port *p = connMux.createProvider(isForLinkTask);
  
  LinkTask *task = new LinkTask ("LC", cid, this, connSM);
  DYNAMIC_SYMBOL (task, cname.str(), this);
  task->entity.connect (p);

}







