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

$Log: uaconn.C,v $
Revision 1.19  2004/04/08 10:53:04  hartmanj
Added support for servicetype request (Srvtype_req)

Revision 1.18  2004/04/05 07:02:16  hartmanj
UAMain class renamed to UAEnt

Revision 1.17  2004/04/01 09:30:26  hartmanj
Rather send clone of retrymsg that retrymsg itself

Revision 1.16  2004/03/31 08:44:01  hartmanj
Bug fixes

Revision 1.15  2004/03/30 08:07:52  hartmanj
Compile fixes and debug couts

Revision 1.14  2004/03/30 06:07:06  hartmanj
Fixes to rid compile errors

Revision 1.13  2004/03/27 12:46:00  bilhanan
rewritten ua functionality

Revision 1.12  2004/03/09 09:20:53  bilhanan
following through from earlier code changes.

Revision 1.11  2004/03/09 09:12:50  bilhanan
changes in invoking UAMain's getMCAddr();

Revision 1.10  2004/03/01 07:26:53  hartmanj
DA address handling fixes

Revision 1.9  2004/02/27 12:38:50  hartmanj
Removed hardcoded service:blah. ServiceType member changed to std::string in UASAP.

Revision 1.8  2004/02/10 14:43:45  bilhanan
progressive bugfixing

Revision 1.7  2004/02/04 15:01:15  hartmanj
Debug msgs

Revision 1.6  2003/08/24 22:19:39  bilhanan
multiple -> single inheritance, uses uamain's dafinder now.

Revision 1.5  2003/08/12 00:10:47  bilhanan
const char* in constructor -> std::string

Revision 1.4  2003/06/04 11:10:00  kalliosa
Licence clause added

  
*/

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <doors/uaconn.h>
#include <doors/slppdu.h>
#include <doors/serviceurl.h>

#include <doors/debug.h>
#include <assert.h>

UAConn :: UAConn (std::string n, Uint32 slphandle, UAEnt *uaent_,  UAConnSM *sm )
  : PTask (n, uaent_->getScheduler(), sm), 
  up (uaent_->up),
  peer (uaent_->peer),
  entity (this),
  XID(0),
  uaent (uaent_),
  myconfig (UAConfigurator::Instance()),
  mcast_time (0),
  ucast_time(0), retrymsg(0) {
  
  setptaskid (slphandle);

  if (uaent->dafinder.isDAPresent()) {
    state = UAConnSM::daIdle;
  } else {
    state = UAConnSM::mcIdle;
  }
}


UAConn :: ~UAConn (void) {
  
  if (retrymsg)
    delete retrymsg;
  DELETE_SYMBOL (this);   
}


bool UAConn :: mcIdle_default (Message *msg){

  return false;
}

bool UAConn :: mcIdle_Srvreq (Message *msg){

   UASAP :: Srv_req *upmsg = (UASAP :: Srv_req *) msg;
   SLPPeer :: SRVRQST *peermsg = (SLPPeer::SRVRQST *) peer.create (SLPPeer::srvrqst);
   XID = uaent->getNewXID();
   InetAddr6 destaddress(myconfig->slp_SlpPort,uaent->getMCAddr(upmsg->sType));
   peermsg->dest = destaddress;

   peermsg->XID = XID;
   peermsg->servicetype = upmsg->sType;
   peermsg->scopeList.push_back(myconfig->slp_useScopes);

   if (retrymsg) {
     delete retrymsg;
     retrymsg = 0;
   }

   SLPPeer::SRVRQST *retrymsgtmp = (SLPPeer::SRVRQST *) peer.create (SLPPeer::srvrqst);
   retrymsgtmp->dest = peermsg->dest;
   retrymsgtmp->XID = peermsg->XID;
   retrymsgtmp->servicetype = peermsg->servicetype;
   if( !peermsg->scopeList.empty() )
	 retrymsgtmp->scopeList = peermsg->scopeList;

   retrymsg = retrymsgtmp;
   retrymsg->lock();

   state = UAConnSM :: mcWfSrv;
   peer.putMessage (peermsg);

   retTimer.init (this, OTime (myconfig->CONFIG_RETRY));
   mcast_time += myconfig->CONFIG_RETRY;
   retTimer.start();

   return false;

}

bool UAConn :: mcIdle_Srvtypereq (Message *msg){

   UASAP :: Srvtype_req *upmsg = (UASAP :: Srvtype_req *) msg;
   SLPPeer :: SRVTYPERQST *peermsg = (SLPPeer::SRVTYPERQST *) peer.create (SLPPeer::srvtyperqst);
   XID = uaent->getNewXID();
   InetAddr6 destaddress(myconfig->slp_SlpPort,uaent->getMCAddr("SVRLOC"));
   peermsg->dest = destaddress;

   peermsg->XID = XID;
   peermsg->namingA = "";
   peermsg->PRList.clear();
   peermsg->scopeList.push_back(myconfig->slp_useScopes);

   if (retrymsg) {
     delete retrymsg;
     retrymsg = 0;
   }

   SLPPeer::SRVTYPERQST *retrymsgtmp = (SLPPeer::SRVTYPERQST *) peer.create (SLPPeer::srvtyperqst);
   retrymsgtmp->dest = peermsg->dest;
   retrymsgtmp->XID = peermsg->XID;
   retrymsgtmp->namingA = peermsg->namingA;
   if( !peermsg->scopeList.empty() )
	 retrymsgtmp->scopeList = peermsg->scopeList;

   retrymsgtmp->PRList.clear();
   retrymsg = retrymsgtmp;
   retrymsg->lock();

   state = UAConnSM :: mcWfSrvtype;
   peer.putMessage (peermsg);

   retTimer.init (this, OTime (myconfig->CONFIG_RETRY));
   mcast_time += myconfig->CONFIG_RETRY;
   retTimer.start();

   return false;

}

bool UAConn :: mcIdle_Attrreq (Message *msg){

   UASAP :: Attr_req *upmsg = (UASAP :: Attr_req *) msg;
   SLPPeer :: ATTRRQST *peermsg = (SLPPeer::ATTRRQST *) peer.create (SLPPeer::attrrqst);
   XID = uaent->getNewXID();
   ServiceURL targeturl (upmsg->URL);
   InetAddr6 destaddress (myconfig->slp_SlpPort,uaent->getMCAddr (targeturl.getServiceType()));
   peermsg->dest = destaddress;
   peermsg->XID = XID;
   peermsg->URL = targeturl;
   peermsg->scopeList.push_back(myconfig->slp_useScopes);

   // Convert Frame to a vector
   frameToVector(upmsg->attrIDs,peermsg->tagList);

   if (retrymsg) {
     delete retrymsg;
     retrymsg = 0;
   }

   SLPPeer::ATTRRQST *retrymsgtmp = (SLPPeer::ATTRRQST*)peer.create (SLPPeer::attrrqst);
   retrymsgtmp->dest = peermsg->dest;
   retrymsgtmp->XID = peermsg->XID;
   retrymsgtmp->URL = peermsg->URL;
   retrymsgtmp->scopeList = peermsg->scopeList;
   if (!peermsg->tagList.empty())
     retrymsgtmp->tagList = peermsg->tagList;

   retrymsgtmp->lock();
   retrymsg = retrymsgtmp;

   state = UAConnSM :: mcWfAttr;
   peer.putMessage (peermsg);

   retTimer.init (this, OTime (myconfig->CONFIG_RETRY));
   mcast_time += myconfig->CONFIG_RETRY;
   retTimer.start();

   return false;

}

bool UAConn :: mcIdle_SLPClose (Message *msg){

  delete this;
  return false;

}

bool UAConn :: mcIdle_DAADVERT (Message *msg){ 

  state = UAConnSM :: daIdle;
  return false;
}

bool UAConn :: mcWfSrv_default (Message *msg){  // ok

  return false;
}

bool UAConn :: mcWfSrv_SLPClose (Message *msg){ // ok

  retTimer.stop();
  delete this;
  return false;
}

bool UAConn :: mcWfSrv_Srvrply (Message *msg){

   SLPPeer::SRVRPLY *peermsg  = (SLPPeer :: SRVRPLY *) msg;
   UASAP::Srv_ind *upmsg = (UASAP::Srv_ind *) up.create (UASAP::srv_ind); // create
   upmsg->SLPHandle = getptaskid(); // set data
   upmsg->errorcode = 0;
   //upmsg->urls = peermsg->urlEntries;
   for(unsigned int i=0;i<peermsg->urlEntries.size();i++) {
	 std::string url = peermsg->urlEntries[i].getURL();
	 upmsg->urls.putLast((Byte*)url.c_str(),url.length());
	 upmsg->urls.putLast(' ');
   }

   ((SLPPeer :: SRVRQST *) retrymsg)->PRList.insert(peermsg->source);
   up.putMessage (upmsg); 
   
   // nothing else to do, continue waiting.

   return false;

}

bool UAConn :: mcWfSrv_DAADVERT (Message *msg){

  SLPPeer :: DAADVERT *peermsg = (SLPPeer :: DAADVERT *) msg;

  // don't stop retransmissions, but reset mcast_time and ucast_time to 0.
  mcast_time = 0;
  ucast_time = 0;

  if (retrymsg)
    // instead target to DA from now on. Should a leave_mcast group be issued?
    ((SLPPeer :: SRVRQST *) retrymsg)->dest = peermsg->source;

  state = UAConnSM :: daWfSrv;
  return false;

}

bool UAConn :: mcWfSrv_timeout (Message *msg){

  mc_timeout();
  return false;
  
}
bool UAConn :: mcWfSrvtype_default (Message *msg){  // ok

  return false;
}

bool UAConn :: mcWfSrvtype_SLPClose (Message *msg){ // ok

  retTimer.stop();
  delete this;
  return false;
}

bool UAConn :: mcWfSrvtype_Srvtyperply (Message *msg){

   SLPPeer::SRVTYPERPLY *peermsg  = (SLPPeer :: SRVTYPERPLY *) msg;
   UASAP::Srvtype_ind *upmsg = (UASAP::Srvtype_ind *) up.create (UASAP::srvtype_ind); // create
   upmsg->SLPHandle = getptaskid(); // set data
   upmsg->errorcode = 0;
   //upmsg->urls = peermsg->urlEntries;
   for(unsigned int i=0;i<peermsg->typeList.size();i++) {
	 std::string type = peermsg->typeList[i];
	 upmsg->types.putLast((Byte*)type.c_str(),type.length());
	 upmsg->types.putLast(' ');
   }

   ((SLPPeer :: SRVTYPERQST *) retrymsg)->PRList.insert(peermsg->source);
   up.putMessage (upmsg);
   
   // nothing else to do, continue waiting.

   return false;

}

bool UAConn :: mcWfSrvtype_DAADVERT (Message *msg){

  SLPPeer :: DAADVERT *peermsg = (SLPPeer :: DAADVERT *) msg;

  // don't stop retransmissions, but reset mcast_time and ucast_time to 0.
  mcast_time = 0;
  ucast_time = 0;

  if (retrymsg)
    // instead target to DA from now on. Should a leave_mcast group be issued?
    ((SLPPeer :: SRVTYPERQST *) retrymsg)->dest = peermsg->source;

  state = UAConnSM :: daWfSrvtype;
  return false;

}

bool UAConn :: mcWfSrvtype_timeout (Message *msg){

  mc_timeout();
  return false;
  
}

bool UAConn :: mcWfAttr_default (Message *msg){
  
  return false;
}

bool UAConn :: mcWfAttr_SLPClose (Message *msg){

  retTimer.stop();
  delete this;
  return false;
}

bool UAConn :: mcWfAttr_Attrrply (Message *msg){

   SLPPeer::ATTRRPLY *peermsg  = (SLPPeer :: ATTRRPLY *) msg;
   UASAP::Attr_ind *upmsg =  (UASAP::Attr_ind *)up.create (UASAP::attr_ind); // create
   upmsg->SLPHandle = getptaskid(); // set data
   upmsg->errorcode = peermsg->errorCode;
	 //upmsg->attributes = peermsg->attributeList;
   for(unsigned int i=0;i<peermsg->attributeList.size();i++) {
	 std::string url = peermsg->attributeList[i];
	 upmsg->attributes.putLast((Byte*)url.c_str(),url.length());
	 upmsg->attributes.putLast(' ');
   }

   ((SLPPeer :: ATTRRQST *) retrymsg)->PRList.insert(peermsg->source);
   up.putMessage (upmsg); 
   
   // nothing else to do, continue waiting.

   return false;

}

bool UAConn :: mcWfAttr_DAADVERT (Message *msg){

  SLPPeer :: DAADVERT *peermsg = (SLPPeer :: DAADVERT *) msg;

  // don't stop retransmissions, but reset mcast_time to 0.
  mcast_time = 0;
  ucast_time = 0;
  if (retrymsg)
    // instead target to DA from now on. Should a leave_mcast group be issued?
    ((SLPPeer :: SRVRQST *) retrymsg)->dest = peermsg->source;

  state = UAConnSM :: daWfAttr;
  return false;
}

bool UAConn :: mcWfAttr_timeout (Message *msg) {

  mc_timeout();
  return false;
}

bool UAConn :: daIdle_default (Message *msg){

return false;
}

bool UAConn :: daIdle_Srvtypereq (Message *msg){

   UASAP :: Srvtype_req *upmsg = (UASAP :: Srvtype_req *) msg;
   SLPPeer :: SRVTYPERQST *peermsg = (SLPPeer::SRVTYPERQST *) peer.create (SLPPeer::srvtyperqst);
   XID = uaent->getNewXID();
   peermsg->dest = uaent->dafinder.getDA();
   peermsg->XID = XID;
   peermsg->namingA = "";
   peermsg->scopeList.push_back(myconfig->slp_useScopes);

   if (retrymsg) {
     delete retrymsg;
     retrymsg = 0;
   }

   SLPPeer::SRVTYPERQST *retrymsgtmp = (SLPPeer::SRVTYPERQST *) peer.create (SLPPeer::srvtyperqst);
   retrymsgtmp->dest = peermsg->dest;
   retrymsgtmp->XID = peermsg->XID;
   retrymsgtmp->namingA = "";
   if( !peermsg->scopeList.empty() )
	 retrymsgtmp->scopeList = peermsg->scopeList;

   retrymsgtmp->lock();
   retrymsgtmp->PRList.clear();
   retrymsg = retrymsgtmp;

   state = UAConnSM :: daWfSrvtype;
   peer.putMessage (peermsg);

   retTimer.init (this, OTime (myconfig->CONFIG_RETRY));
   ucast_time += myconfig->CONFIG_RETRY;
   retTimer.start();

   return false;

}

bool UAConn :: daIdle_Srvreq (Message *msg){

   UASAP :: Srv_req *upmsg = (UASAP :: Srv_req *) msg;
   SLPPeer :: SRVRQST *peermsg = (SLPPeer::SRVRQST *) peer.create (SLPPeer::srvrqst);
   XID = uaent->getNewXID();
   peermsg->dest = uaent->dafinder.getDA();
   peermsg->XID = XID;
   peermsg->servicetype = upmsg->sType;
   peermsg->scopeList.push_back(myconfig->slp_useScopes);

   if (retrymsg) {
     delete retrymsg;
     retrymsg = 0;
   }

   SLPPeer::SRVRQST *retrymsgtmp = (SLPPeer::SRVRQST *) peer.create (SLPPeer::srvrqst);
   retrymsgtmp->dest = peermsg->dest;
   retrymsgtmp->XID = peermsg->XID;
   retrymsgtmp->servicetype = peermsg->servicetype;
   retrymsgtmp->scopeList = peermsg->scopeList;
   retrymsgtmp->lock();
   retrymsg = retrymsgtmp;

   state = UAConnSM :: daWfSrv;
   peer.putMessage (peermsg);

   retTimer.init (this, OTime (myconfig->CONFIG_RETRY));
   ucast_time += myconfig->CONFIG_RETRY;
   retTimer.start();

   return false;

}

bool UAConn :: daIdle_Attrreq (Message *msg){

   UASAP :: Attr_req *upmsg = (UASAP :: Attr_req *) msg;
   SLPPeer :: ATTRRQST *peermsg = (SLPPeer::ATTRRQST *) peer.create (SLPPeer::attrrqst);
   XID = uaent->getNewXID();
   ServiceURL targeturl (upmsg->URL);
   peermsg->dest = uaent->dafinder.getDA();
   peermsg->XID = XID;
   peermsg->URL = targeturl;
   peermsg->scopeList.push_back(myconfig->slp_useScopes);

   frameToVector(upmsg->attrIDs,peermsg->tagList);

   if (retrymsg) {
     delete retrymsg;
     retrymsg = 0;
   }

   SLPPeer::ATTRRQST *retrymsgtmp = (SLPPeer::ATTRRQST *) peer.create (SLPPeer::attrrqst);
   retrymsgtmp->dest = peermsg->dest;
   retrymsgtmp->XID = peermsg->XID;
   retrymsgtmp->URL = peermsg->URL;
   retrymsgtmp->scopeList = peermsg->scopeList;
   if (!peermsg->tagList.empty())
     retrymsgtmp->tagList = peermsg->tagList;

   retrymsgtmp->lock();
   retrymsg = retrymsgtmp;

   state = UAConnSM :: daWfAttr;
   peer.putMessage (peermsg);

   retTimer.init (this, OTime (myconfig->CONFIG_RETRY));
   ucast_time += myconfig->CONFIG_RETRY;
   retTimer.start();

   return false;

}

bool UAConn :: daIdle_SLPClose (Message *msg){

  retTimer.stop();
  delete this;
  return false;
}

bool UAConn :: daWfSrv_default (Message *msg){

return false;
}

bool UAConn :: daWfSrv_SLPClose (Message *msg){

  retTimer.stop();
  delete this;
  return false;
}

bool UAConn :: daWfSrv_Srvrply (Message *msg){

  retTimer.stop(); // stop retransmissions once we get an answer from DA

  SLPPeer::SRVRPLY *peermsg  = (SLPPeer :: SRVRPLY *) msg;
  UASAP::Srv_ind *upmsg =  (UASAP::Srv_ind *)up.create (UASAP::srv_ind); 
  upmsg->SLPHandle = getptaskid();
  upmsg->errorcode = 0;

  for(unsigned int i=0;i<peermsg->urlEntries.size();i++) {
	std::string url = peermsg->urlEntries[i].getURL();
	upmsg->urls.putLast((Byte*)url.c_str(),url.length());
	upmsg->urls.putLast(' ');
  }

  if (retrymsg) {
    delete retrymsg;
    retrymsg = 0;
  }

  up.putMessage( upmsg );
  
  XID = 0;
  state = UAConnSM :: daIdle;
  return false;

}

bool UAConn :: daWfSrv_timeout (Message *msg) { 

  da_timeout();
  return false;

}

bool UAConn :: daWfSrvtype_default (Message *msg){

  return false;
}

bool UAConn :: daWfSrvtype_SLPClose (Message *msg){

  retTimer.stop();
  delete this;
  return false;
}

bool UAConn :: daWfSrvtype_Srvtyperply (Message *msg){

  retTimer.stop(); // stop retransmissions once we get an answer from DA

  SLPPeer::SRVTYPERPLY *peermsg  = (SLPPeer :: SRVTYPERPLY *) msg;
  UASAP::Srvtype_ind *upmsg =  (UASAP::Srvtype_ind *)up.create (UASAP::srvtype_ind); 
  upmsg->SLPHandle = getptaskid();
  upmsg->errorcode = 0;

  for(unsigned int i=0;i<peermsg->typeList.size();i++) {
	std::string type = peermsg->typeList[i];
	upmsg->types.putLast((Byte*)type.c_str(),type.length());
	upmsg->types.putLast(' ');
  }

  if (retrymsg) {
    delete retrymsg;
    retrymsg = 0;
  }

  up.putMessage( upmsg );
  
  XID = 0;
  state = UAConnSM :: daIdle;
  return false;

}

bool UAConn :: daWfSrvtype_timeout (Message *msg) { 

  da_timeout();
  return false;

}

bool UAConn :: daWfAttr_default (Message *msg){

return false;
}

bool UAConn :: daWfAttr_SLPClose (Message *msg){

  retTimer.stop();
  delete this;
  return false;
}

bool UAConn :: daWfAttr_Attrrply (Message *msg){

  retTimer.stop(); // stop retransmissions once we get an answer from DA

  SLPPeer::ATTRRPLY *peermsg  = (SLPPeer :: ATTRRPLY *) msg;
  UASAP::Attr_ind *upmsg =  (UASAP::Attr_ind *)up.create (UASAP::attr_ind); 
  upmsg->SLPHandle = getptaskid(); 
  upmsg->errorcode = peermsg->errorCode;

  for(unsigned int i=0;i<peermsg->attributeList.size();i++) {
	std::string url = peermsg->attributeList[i];
	upmsg->attributes.putLast((Byte*)url.c_str(),url.length());
	upmsg->attributes.putLast(' ');
  }

  up.putMessage (upmsg); 

  XID = 0;
  state = UAConnSM :: daIdle;
  return false;
}

bool UAConn :: daWfAttr_timeout (Message *msg) { 

  da_timeout();
  return false;

}

void UAConn :: mc_timeout (void) {

  retTimer.stop();
  if (mcast_time <= CONFIG_MC_MAX) {

    peer.putMessage ((retrymsg->clone()));
    retTimer.stop();
    retTimer.init (this, OTime (myconfig->CONFIG_RETRY));
    mcast_time += myconfig->CONFIG_RETRY;
    retTimer.start();
  } else {

    retTimer.stop();
    if (retrymsg) {
	  retrymsg->unlock();
      delete retrymsg;
      retrymsg = 0;
    }
    mcast_time = 0;
    XID = 0;
    state = UAConnSM :: mcIdle;
  }

}

void UAConn :: da_timeout (void) {

  retTimer.stop();
  if (ucast_time <= CONFIG_RETRY_MAX) {

    retrymsg->lock();
    peer.putMessage (retrymsg->clone());
    retTimer.stop();
    retTimer.init (this, OTime (myconfig->CONFIG_RETRY));
    ucast_time += myconfig->CONFIG_RETRY;
    retTimer.start();
  } else {

    retTimer.stop();
    if (retrymsg) {
      delete retrymsg;
      retrymsg = 0;
    }
    ucast_time = 0;
    XID = 0;
    state = UAConnSM :: daIdle;
  }

}

int UAConn :: frameToVector( Frame &f, StringVector &v ) {

	unsigned int itemCount = 0;

	if (f.length()) {
		char *attrName = new char[f.length()];
		memset((void*)attrName,0,f.length());
		while(f.length()) {
			attrName[0] = f.getFirst();
			unsigned int i = 1;
			while(f.length() && attrName[i-1] != ',') {
				attrName[i] = f.getFirst();
				i++;
			}
			if(attrName[i-1] == ',')
				attrName[i-1] = 0;
			else
				attrName[i] = 0;
			v.push_back(std::string(attrName));
			itemCount++;
		}
	}

	return itemCount;
}
