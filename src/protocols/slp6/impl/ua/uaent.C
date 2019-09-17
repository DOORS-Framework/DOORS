//  Copyright 2003
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

$Log: uaent.C,v $
Revision 1.2  2004/04/08 10:53:04  hartmanj
Added support for servicetype request (Srvtype_req)

Revision 1.1  2004/04/05 06:58:54  hartmanj
Renamed uamain* to uaent*

Revision 1.20  2004/04/01 09:30:57  hartmanj
Store SLP port number from config file to sLPCommon::slpPort_

Revision 1.19  2004/03/31 08:44:01  hartmanj
Bug fixes

Revision 1.18  2004/03/30 08:07:52  hartmanj
Compile fixes and debug couts

Revision 1.17  2004/03/30 06:07:06  hartmanj
Fixes to rid compile errors

Revision 1.16  2004/03/27 12:46:00  bilhanan
rewritten ua functionality

Revision 1.15  2004/03/09 09:33:25  bilhanan
removed nonsensical comment right in the middle of the class constructor.

Revision 1.14  2004/03/09 09:29:42  bilhanan
bugfix

Revision 1.13  2004/03/09 09:22:22  bilhanan
reintroducing DAActiveDiscoveryInterval_, using UAConfigurator, changed
constructor parameter passing to SLPcommon.

Revision 1.12  2004/02/27 12:39:16  hartmanj
ServiceType member changed to std::string in UASAP.

Revision 1.11  2004/02/27 07:46:54  hartmanj
Similar hack introduced as in SAMain to perform joining after initializations. UAMain::joinAndGo()

Revision 1.10  2004/02/10 18:09:34  bilhanan
joining multicast groups changed.

Revision 1.9  2004/02/10 14:54:05  bilhanan
more bugfixing.

Revision 1.8  2004/02/10 14:43:45  bilhanan
progressive bugfixing

Revision 1.7  2004/02/04 15:01:51  hartmanj
DAFinder restructuring. Still needs fixing.

Revision 1.6  2003/08/27 06:36:45  kalliosa
added ready_daAdvert

Revision 1.5  2003/08/24 22:18:18  bilhanan
reduced number of parent classes, DAFinder now a member.

Revision 1.4  2003/08/12 00:10:48  bilhanan
const char* in constructor -> std::string

Revision 1.3  2003/06/04 11:10:00  kalliosa
Licence clause added

  
*/

#include <string>
#include <doors/hsi.h>
#include <doors/slptypes.h>
#include <doors/uaent.h>
#include <doors/uaconn.h>
#include <doors/uaentsm.h>
#include <doors/uaconnsm.h>
#include <doors/uasap.h>
#include <doors/uaconfigurator.h>
#include <doors/debug.h>

static bool isForUAConn (const EventTask *t, const Message *m) {

  UAConn *conn  = ( UAConn* ) t;
/*
  if (m->isForMany()) {  // broadcast message
    return true;         // just accept
  }
*/
  bool rc = false;

  Uint16 task_XID = conn->XID;
  Uint16 peer_XID;

  Uint32 taskid = conn->getptaskid();
  Uint32 up_handle;

  if (m->getTarget() == &conn->up) {
    switch (m->getType()) {

    case (UASAP :: srv_req):
      up_handle = ((UASAP :: Srv_req *) m)->SLPHandle;
      break;

    case (UASAP :: srvtype_req):
      up_handle = ((UASAP :: Srvtype_req *) m)->SLPHandle;
      break;

    case (UASAP :: attr_req):
      up_handle = ((UASAP :: Attr_req *) m)->SLPHandle;
      break;
    }

    if ((task_XID == 0) && (up_handle == taskid))
      rc = true;

  } else if (m->getTarget() == &conn->peer) {      
   
   switch (m->getType()) {
   case (SLPPeer :: srvrply):
     peer_XID = ((SLPPeer :: SRVRPLY *) m)->XID;
     break;

   case (SLPPeer :: srvtyperply):
     peer_XID = ((SLPPeer :: SRVTYPERPLY *) m)->XID;
     break;

   case (SLPPeer :: attrrply):
     peer_XID = ((SLPPeer :: ATTRRPLY *) m)->XID;
     break;
  }

   if (peer_XID == task_XID)
     rc = true;
  }

  return rc;
}


UAEnt :: UAEnt (std::string n, Scheduler *s, UAEntSM *sm)
  : PTask (n, s, sm), up (this), 
  down (this, &peer),
  SLPCommon(down),
  peer (this, &down),
  dafinder (),
  myconfig (UAConfigurator::Instance()),
  DATimer (this, myconfig->slp_DAActiveDiscoveryInterval),
  csm (new UAConnSM), 
  connections (this),
  connMux (new SinglePropagator (new PortList), new SinglePropagator (new PortList)),
  mcast_time (0) {

  connections.connect (connMux.createUser ());
  
  STATIC_SYMBOL (peer, this);
  STATIC_SYMBOL (down, this);
  STATIC_SYMBOL (up, this);
  state = UAEntSM :: closed;

  ip6scope_ = myconfig->slp_IPv6Scope;
  slpPort_ = myconfig->slp_SlpPort;

  if (myconfig->slp_DAAddresses.empty()){
    // need to use mcast, regardless of what config now says
    
    DATimer.init (this, myconfig->CONFIG_START_WAIT);
    DATimer.start();
  } else {
    
    dafinder.insertDA (myconfig->slp_DAAddresses, myconfig->slp_SlpPort, myconfig->slp_useScopes, "");
    
     if (!myconfig->slp_useMulticast) {

      state = UAEntSM::open;
    }  else {

      // wait for config_wait seconds and then mcast stuff

      DATimer.init (this, myconfig->CONFIG_START_WAIT);
      DATimer.start();
    }
  }
}

UAEnt :: ~UAEnt (void) {

  if (csm) {
    delete csm;
    csm = 0;
  }
  
  DELETE_SYMBOL (this);
}

bool UAEnt :: closed_default (Message *msg) {

   return false;
}


bool UAEnt :: closed_Timer (Message *msg) {

  if (joinMCGroup ("service:directory-agent")) {
    
    if (!(myconfig->slp_passiveDADetection)) {

      solicitDirectoryAgent (myconfig->slp_useScopes);
      DATimer.init(this, OTime(myconfig->slp_DAActiveDiscoveryInterval));
      DATimer.start();
      mcast_time = myconfig->slp_DAActiveDiscoveryInterval;
    }

  } else {
    _DOORS_DEBUG(0,"Didn't join SERVLOC-DA mcast group!");
  }
   
  state = UAEntSM :: open;
  return false;
  
}

bool UAEnt :: open_default( Message *msg ) {

  bool rc = true;
  msg->lock();

  if (connections.putMessage (msg) == false) {
    msg->unlock ();  
    rc = false;
  }
  return rc;      
}   


bool UAEnt :: open_SLPOpen (Message *msg) {
   
  UASAP::SLPOpen_ind *m = (UASAP::SLPOpen_ind *) up.create (UASAP::slpopen_ind);
  
  m->SLPHandle = getNewHandle();
  
  up.putMessage (m);
  return false;
}

bool UAEnt :: open_SLPClose (Message *msg) {

  UASAP :: SLPClose_req *upmsg = (UASAP :: SLPClose_req *) msg;
  upmsg->lock ();
  upmsg->setMultipleTargets();
  connections.putMessage (upmsg);
  
  return true;
}

bool UAEnt :: open_Srvreq ( Message *msg ) {

	//UASAP :: Srv_req *upmsg = (UASAP :: Srv_req *) msg;
  msg->lock();

  if (connections.putMessage (msg) == false) {
    createConnection (((UASAP :: Srv_req *)msg)->SLPHandle);
    connections.putMessage (msg);
  }

  return true;      
}   

bool UAEnt :: open_Attrreq ( Message *msg ) {

  UASAP :: Attr_req *upmsg = (UASAP :: Attr_req *) msg;
  upmsg->lock();

  if (connections.putMessage (upmsg) == false) {
    createConnection (upmsg->SLPHandle);
    connections.putMessage (upmsg);
  }
  return true;      
}   

bool UAEnt :: open_Srvtypereq ( Message *msg ) {

  UASAP :: Srvtype_req *upmsg = (UASAP :: Srvtype_req *) msg;
  upmsg->lock();

  if (connections.putMessage (upmsg) == false) {
    createConnection (upmsg->SLPHandle);
    connections.putMessage (upmsg);
  }
  return true;      
}   

bool UAEnt :: open_DAAdvert (Message *msg) {

  SLPPeer :: DAADVERT *peermsg = (SLPPeer :: DAADVERT *) msg;

  if (peermsg->XID == retrans_XID) {
    // update PR list
    DA_PRList.insert (std::set<InetAddr6>::value_type(peermsg->source));
  }

  if (dafinder.updateDAList (peermsg)) {
    peermsg->lock ();
    peermsg->setMultipleTargets();
    connections.putMessage (peermsg);
    peermsg->unlock();
    }

    return false;
}


bool UAEnt :: open_Timer (Message *msg) {

  DATimer.stop();
  if (mcast_time <= CONFIG_MC_MAX) {

    SLPPeer :: SRVRQST *peermsg = (SLPPeer::SRVRQST *) peer.create (SLPPeer :: srvrqst);

    if (retrans_XID == 0) 
      retrans_XID = getNewXID();
    
    peermsg->XID = retrans_XID;
    peermsg->dest = InetAddr6(myconfig->slp_SlpPort,getMCAddr ("service:directory-agent"));
    peermsg->langTag = myconfig->slp_locale;
    
    if (!DA_PRList.empty()) {
      peermsg->PRList = DA_PRList;
    }
	
    peermsg->servicetype = "service:directory-agent";
    peermsg->scopeList.push_back(myconfig->slp_useScopes);
    
    peer.putMessage (peermsg);

    DATimer.init (this, OTime(myconfig->slp_DAActiveDiscoveryInterval));
    DATimer.start();
    mcast_time += myconfig->slp_DAActiveDiscoveryInterval;

  } else {

    mcast_time = 0;
    retrans_XID = 0;
  }

  return false;
}




void UAEnt :: createConnection (Uint32 slphandle) {

  std::ostringstream cname;
  cname << getName () << ".conn." << slphandle; 

  Port *p = connMux.createProvider (isForUAConn);

  UAConn *task  = new UAConn (cname.str(), slphandle, this, csm);
  DYNAMIC_SYMBOL (task, cname.str(), this);
  task->entity.connect (p);        
     
}
