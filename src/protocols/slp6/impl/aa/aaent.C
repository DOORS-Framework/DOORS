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

$Log: aaent.C,v $
Revision 1.3  2006/11/21 19:52:11  bilhanan
development

Revision 1.2  2006/07/29 21:42:09  bilhanan
AA improvements

Revision 1.1  2004/10/12 11:57:18  bilhanan
AA code merged into main tree from src/experimental

Revision 1.8  2004/08/03 05:36:41  hartmanj
Bug fixes

Revision 1.7  2004/07/12 07:26:46  hartmanj
Service deregistration

Revision 1.6  2004/06/16 13:17:34  hartmanj
ostringstream::flush() and/or ostringstream::clear() don't seem to work. Declared another ostringstream to go around this.

Revision 1.5  2004/05/25 09:41:32  hartmanj
Typo-fixes, removed use of deprecated classes/methods, general development.

Revision 1.4  2003/08/24 22:11:41  bilhanan
devt

Revision 1.3  2003/08/20 11:48:07  bilhanan
va<->aa<->network srv req/rply functionality coded.

Revision 1.2  2003/08/01 12:12:17  bilhanan
devt

Revision 1.1  2003/07/30 09:51:18  bilhanan
*** empty log message ***


*/

#include <sstream>
#include <fstream>
#include <doors/udp6task.h>
#include <doors/ptb.h>
#include <doors/aaent.h>
#include <doors/aaesm.h>
#include <doors/slppdu.h>
#include <doors/serviceurl.h>
#include <doors/inetaddr6.h>
#include <doors/debug.h>
#include <doors/uaproxy.h>
#include <doors/uaproxysm.h>
#include <doors/aaconfigurator.h>

#define ULOS(x) std::cout << (x) << std::endl

bool AAEntity :: isForUAProxy (const EventTask *t, const Message *m) {

    bool rc = false;
    UAProxy *task = (UAProxy *) t;
    Uint16 msg_xid = 0;

    //if (m->getTarget () == &task->peer) {

ULOS("switch (m->getType ()) {");
      switch (m->getType ()) {

      case (SLPPeer::srvrqst) :
ULOS("case (SLPPeer::srvrqst) :");
ULOS(task->va_addr.getHostAddr());
ULOS(((SLPPeer::SRVRQST *) m)->source.getHostAddr());
	msg_xid = ((SLPPeer::SRVRQST *) m)->XID;
	if ((task->va_addr == ((SLPPeer::SRVRQST *) m)->source) && (msg_xid == task->va_XID))
	  rc = true;
	break;

      case (SLPPeer::srvtyperqst) :
ULOS("case (SLPPeer::srvtyperqst) :");
	msg_xid = ((SLPPeer::SRVTYPERQST *) m)->XID;
	if ((task->va_addr == ((SLPPeer::SRVTYPERQST *) m)->source) && (msg_xid == task->va_XID))
	  rc = true;
	break;

      case (SLPPeer::srvrply) :
	msg_xid = ((SLPPeer::SRVRPLY *) m)->XID;
	if (msg_xid == task->va_XID)
	  rc = true;
	break;
	
      }
    //}
    
ULOS("rc:");
ULOS( rc ? "   true" : "   false" );
    return rc;
}

AAEntity :: AAEntity (std::string n, Scheduler *s, AAEntitySM *sm, 
		      Udp6Task *udp, InetAddr6 &addr)
  : PTask (n, s, sm), 
  peer (this,  &down), 
  down( this,  &peer ),
  SLPCommon(down),
  Services_( ),
  ownAddr_( addr ),
  heartBeat_(100 ),
  advTimer_( this, OTime(2) ),
  XID (2003),                  // just some random value
  to_uaproxy (this),
  uaproxyMux(new SinglePropagator(new PortList), new SinglePropagator(new PortList)),
  uaproxySM (new UAProxySM),
  myconfig( AAConfigurator::Instance( )), my_scope("DEFAULT"),
  dafinder() {

  to_uaproxy.connect(uaproxyMux.createUser());	


  const char *aaprefix_chars = myconfig->slpm6_AAPrefix.data();
  _frmaaprefix.putLast ((const Byte*)aaprefix_chars, myconfig->slpm6_AAPrefix.size());

  if( myconfig->slpm6_AAHeartBeat != 0 )
    {
        heartBeat_ = myconfig->slpm6_AAHeartBeat;
	advinterval = (OTime) (myconfig->slpm6_AAHeartBeat - myconfig->slpm6_AAMinDelay);
        OTime t (heartBeat_);
        advTimer_.stop();
        advTimer_.setInitial(t);
        advTimer_.start();
    }

  udp->up.connect( &down );

  // Bad: ip6scope_ not initialised anywhere, except inside parent SLPCommon!
  ip6scope_ = myconfig->slp_IPv6Scope;
  slpPort_ = myconfig->slp_SlpPort;

  adv_addr.set(slpPort_, getMCAddr ("service:access-agent"));
  
  joinMCGroup( "access-agent" );
  joinMCGroup( "SVRLOC-DA" );  
  joinMCGroup( "SVRLOC" );  

ULOS("AA->peer at");
ULOS(&peer);
}

AAEntity::~AAEntity() {

  if (uaproxySM) {
    delete uaproxySM;
    uaproxySM = 0;
  }

}

bool AAEntity ::  ready_srvRqst (Message *msg) { 
std::cout << "AAEntity ::  ready_srvRqst" << std::endl;
  // First check if AA discovery is in progress
  
  SLPPeer :: SRVRQST *rqst = (SLPPeer :: SRVRQST *) msg;
  
  if (rqst->servicetype == "service:access-agent") {
    if (advinterval >= advTimer_.delta()) 
      sendAAAdv() ;
    return false;
  }   

  if (rqst->source == ownAddr_) {
    // Discard message that looped back to own interface
    msg->unlock();
    return false;
  }

  // Then check if unicast srvrqst was issued (assumption: sent by a VA)
  // If so, pass it to uaproxy to rebroadcast request with site-local mcast.

  if (!(IN6_IS_ADDR_MULTICAST( &( ( ( sockaddr_in6*)(rqst->source.getAddr()))->sin6_addr) ) ) ) {


    // only allowed to rebroadcast if AA can be proxy
    if (myconfig->_srvproxy == false)
      return false;  // drop message

     msg->lock();
     if (to_uaproxy.putMessage (rqst) == false) {
       XID++;
       createuaproxy (rqst, XID);
       to_uaproxy.putMessage (rqst);
     }
     return true;
   }

   // If not AA discovery and not VA request, must be from a normal UA.
   // Check registered services.

   else {
     
     for (AddrVector::iterator idx = rqst->PRList.begin(); idx != rqst->PRList.end(); ++idx){      
       
       if ((*idx) == ownAddr_) {
	 return false;  // don't answer if UA already knows
       }      
     }   
     
     URLVector matchingURLs;
     Uint32 datagram_size = 14;
     // iterate all registered urls
     for (std::vector<ServiceURL>::iterator i = Services_.begin();
          i != Services_.end() && datagram_size < 1400; ++i)
     {
         if ( (*i).isOfServiceType( rqst->servicetype ) ) {
             datagram_size += (*i).getURL().size();

             if( datagram_size < 1400 )
                 matchingURLs.push_back( *i );
         } 
     }

     sendReply( rqst->source, matchingURLs );  
     return false; 
   }
}

bool AAEntity :: ready_attrRqst (Message *msg) {
  
  // to implement only AA attributes for now
  
  return false; 
}

bool AAEntity :: ready_srvTypeRqst (Message *msg) {
std::cout << "AAEntity ::  ready_srvTypeRqst" << std::endl;

    SLPPeer :: SRVTYPERQST *req = (SLPPeer :: SRVTYPERQST*) msg;

    // Then check if unicast srvrqst was issued (assumption: sent by a VA)
    // If so, pass it to uaproxy to rebroadcast request with site-local mcast.

    if (!(IN6_IS_ADDR_MULTICAST( &( ( ( sockaddr_in6*)(req->source.getAddr()))->sin6_addr) ) ) ) {


    // only allowed to rebroadcast if AA can be proxy
    if (myconfig->_srvproxy == false)
      return false;  // drop message

      msg->lock();
      if (to_uaproxy.putMessage (req) == false) {
        XID++;
        createuaproxy (req, XID);
        to_uaproxy.putMessage (req);
      }
      return true;
    }

    SLPPeer :: SRVTYPERPLY *reply = (SLPPeer :: SRVTYPERPLY*)peer.create(SLPPeer :: srvtyperply);

    unsigned int stypelistlength = 50; // TODO: ?

    for( unsigned int i=0; i < Services_.size() && stypelistlength < 1300; i++ ) {
        reply->typeList.push_back(Services_[i].getServiceType());
        if( reply->typeList.size() ) // double checking
            stypelistlength += reply->typeList.back().size();
    }

    reply->dest = req->source;
    reply->errorCode = 0;

    peer.putMessage( reply );

    return false; 
}

bool AAEntity :: ready_daAdvert (Message *msg) {
  
  return dafinder.updateDAList ((SLPPeer :: DAADVERT*)msg); 
}

bool AAEntity :: ready_srvRply (Message *msg) {  
  
  // pass to uaproxy
  return foruaproxy (msg);
}

bool AAEntity ::  ready_srvReg (Message *msg) {
  
  SLPPeer :: SRVREG *reg = ( SLPPeer :: SRVREG *) msg;


    // only allowed to register if AA can be proxy
    if (myconfig->_srvproxy == false)
      return false;  // drop message

  ServiceURL service = reg->url;
  service.setServiceType(reg->serviceType);
  Services_.push_back (service);

  joinMCGroup( reg->serviceType );

  // create and send ack

  SLPPeer :: SRVACK *ack = (SLPPeer :: SRVACK *) peer.create(SLPPeer::srvack);
  ack->dest = reg->source;
  ack->XID = reg->XID;
  ack->errorCode = 0;   

  peer.putMessage (ack);

  return false;
}


bool AAEntity ::  ready_srvDeReg (Message *msg) {

  SLPPeer :: SRVDEREG *unreg = ( SLPPeer :: SRVDEREG *) msg;

  for( unsigned int i=0;i < Services_.size(); i++) {
    if( Services_[i] == unreg->url ) {
      _DOORS_DEBUG(0, "Removed service " << unreg->url.getURL() << " from services list.");
      Services_.erase( Services_.begin() + i );
      break;
    }
  }

  SLPPeer :: SRVACK *ack = (SLPPeer :: SRVACK *) peer.create(SLPPeer::srvack);
  ack->dest = unreg->source;
  ack->XID = unreg->XID;
  ack->errorCode = 0;   

  peer.putMessage (ack);
  _DOORS_DEBUG(0, "Sent SRVACK to unregistration from " << unreg->source.getHostAddr());

  return false;
}

bool AAEntity :: ready_AATimer (Message *msg) {
   
   sendAAAdv();
   
   advTimer_.start();

  return false;
}

bool AAEntity :: ready_default (Message *msg) {

  // just drop

   return false;
}
   
void AAEntity :: sendReply (InetAddr6 &dest, URLVector &urls) {

    SLPPeer::SRVRPLY *rply = (SLPPeer::SRVRPLY *) peer.create (SLPPeer::srvrply);

    rply->dest = dest;
    rply->urlEntries  = urls;
    rply->errorCode = 0;

    peer.putMessage (rply);
}


void AAEntity :: sendAAAdv (void) {

  SLPPeer::AAADVERT *adv_msg = (SLPPeer::AAADVERT *) peer.create (SLPPeer::aaadvert );

  adv_msg->AAPrefix = _frmaaprefix.copy();

  adv_msg->Capabilities = myconfig->_netcapabilities;


  adv_msg->dest = adv_addr;

   // concatenating AA IPv6 address to servicetype
   std::ostringstream tmpAddr;
   tmpAddr << '[' << ownAddr_.getHostAddr() << ']' << ":" << ownAddr_.getPortNumber();
ULOS("Sending out AAADVERT");
   _DOORS_DEBUG(0, "Putting address to aaadv: " << tmpAddr.str() );
   adv_msg->aa_url = ServiceURL (  "service:access-agent://" + tmpAddr.str() );
   
   // concatenating DA IPv6 address to servicetype

   if (dafinder.isDAPresent()) {

       std::ostringstream tmpAddr2;
       InetAddr6 da_address = dafinder.getDA(my_scope);
       tmpAddr2 << '[' << da_address.getHostAddr() << ']' << ":" << da_address.getPortNumber();
       _DOORS_DEBUG(0, "Putting da-address to aaadv: " << tmpAddr2.str() );

       adv_msg->da_url = ServiceURL ("service:directory-agent://" + tmpAddr2.str() );

   }

   peer.putMessage( adv_msg );
}


bool AAEntity :: foruaproxy (Message *msg) {

    bool rc = true;
    msg->lock ();
    if (to_uaproxy.putMessage (msg) == false) {
	msg->unlock ();  // Message can be deleted.
	rc = false;
    }
 
    return rc;
}
 
void AAEntity :: createuaproxy (Message *msg, Uint16 xid) {

  InetAddr6 rqst_source;
  Uint16 rqst_xid = 0;
  std::ostringstream cname;
  cname << getName () << ".request." << xid; 

  Port *p = uaproxyMux.createProvider(AAEntity :: isForUAProxy);

  if( msg->getType() == SLPPeer :: srvrqst ) {

    SLPPeer :: SRVRQST *srvrqst = (SLPPeer :: SRVRQST*)msg;
    rqst_source = srvrqst->source;
    rqst_xid = srvrqst->XID;
  } else if( msg->getType() == SLPPeer :: srvtyperqst ) {

    SLPPeer :: SRVTYPERQST *srvtyperqst = (SLPPeer :: SRVTYPERQST*)msg;
    rqst_source = srvtyperqst->source;
    rqst_xid = srvtyperqst->XID;
  }

ULOS("Creating UAProxy task for service request from: " + rqst_source.getHostAddr());

  UAProxy *task = new UAProxy ("UAProxy", xid, rqst_xid, rqst_source, this, uaproxySM);
  DYNAMIC_SYMBOL (task, cname.str(), this);
  task->entity.connect (p);

}
