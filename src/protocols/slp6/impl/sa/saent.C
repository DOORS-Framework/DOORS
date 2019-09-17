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

$Log: saent.C,v $
Revision 1.4  2004/04/14 09:15:54  hartmanj
Checking our address in PRList with attribute request

Revision 1.3  2004/04/08 10:52:45  hartmanj
Copy request's XID to reply packet. Added multicast join to SVRLOC group for servicetype request

Revision 1.2  2004/04/07 13:43:51  hartmanj
Some fixes

Revision 1.1  2004/04/05 06:55:57  hartmanj
Renamed samain* to saent*

Revision 1.22  2004/04/01 10:48:38  hartmanj
Moved sending indication of registration to SAMain. SAReg doesn't have up port.

Revision 1.21  2004/03/25 12:58:23  hartmanj
Adjusting to new DAFinder interface

Revision 1.20  2004/03/22 11:10:56  hartmanj
Moved attribute request handling from SAMain to SAReg.

Revision 1.19  2004/03/19 11:23:47  hartmanj
Adapting new SLPCommon API.

Revision 1.18  2004/03/15 15:21:29  hartmanj
Development

Revision 1.17  2004/03/09 09:54:06  bilhanan
reintroduced DAActiveDiscoveryInterval_, using SAConfigurator, changed
constructor parameter passing to SLPcommon.

Revision 1.16  2004/03/08 10:43:29  hartmanj
Old config handling removed

Revision 1.15  2004/03/01 07:28:56  hartmanj
Added a check to ready_srvRqst to see if we received our own packet

Revision 1.14  2004/02/25 13:08:47  hartmanj
Fixed to reflect API change in ServiceURL.

Revision 1.13  2004/02/20 12:40:01  hartmanj
Removed unnecessary comments

Revision 1.12  2004/02/18 14:59:21  hartmanj
Added msg->lock() and unlock() so message goes through PortMux

Revision 1.11  2004/02/16 13:01:32  bilhanan
join() introduced to handle joinmcgroup for DA multicast group.

Revision 1.10  2004/02/11 10:38:27  hartmanj
Fixed XID handling in isForSAReg()

Revision 1.9  2004/02/10 18:11:18  bilhanan
joining multicast groups changed
removed dependency to UDP6Task in constructor

Revision 1.8  2004/02/04 15:03:05  hartmanj
Complete rewrite of DA-SA interaction code.

Revision 1.7  2003/11/27 12:32:57  bilhanan
bug fixing

Revision 1.6  2003/10/15 10:47:57  bilhanan
modified broadcast message handling.

Revision 1.5  2003/10/03 14:26:37  bilhanan
csm_ wasn't initialised, map insertion improved.

Revision 1.4  2003/08/24 22:22:53  bilhanan
reduced number of parent classes, DAFinder now a member. isInPRList absorbed by samain.

Revision 1.3  2003/08/12 00:07:46  bilhanan
const char* in constructor -> std::string

Revision 1.2  2003/06/04 11:09:58  kalliosa
Licence clause added

  
*/

#include <set>
#include <string>
#include <doors/hsi.h>
#include <doors/slptypes.h>
#include <doors/saent.h>
#include <doors/sareg.h>
#include <doors/saentsm.h>
#include <doors/saregsm.h>
#include <doors/sasap.h>
#include <doors/saconfigurator.h>
#include <doors/debug.h>


static bool isForSAReg( const EventTask *t, const Message *m )
{
  if (m->isForMany())
    return true;

   SAReg *reggi = (SAReg*) t;
   
   if( (m->getType() == SLPPeer :: srvrqst ) ||
      (m->getType() == SLPPeer :: srvtyperqst ) ||
       (m->getType() == SLPPeer :: attrrqst ))
   {

	  if( m->getType() == SLPPeer :: srvtyperqst ) {
		 return true;
	  }

     if( (m->getType() == SLPPeer :: srvrqst )
         && reggi->isOfType(((SLPPeer::SRVRQST*)m)->servicetype) ) {
       return true;
     }

     if( (m->getType() == SLPPeer :: attrrqst)
         && (reggi->url_ == ((SLPPeer::ATTRRQST*)m)->URL ) ) {
       return true;
     }

   }
   else
     if ( (m->getType() == SLPPeer :: daadvert) ) 
     {
	// message type is daadvert
	// inform all saregs to register its service to new da
     	return true;

     }
   else
     if ( (m->getType() == SLPPeer :: srvack) )
     {
          SLPPeer :: SRVACK* ackCast = ( SLPPeer :: SRVACK* ) m;

          if (reggi->hasXID(ackCast->XID))
               return true;
	
     }  
      
   return false; // no, message was not for any sareg
}



SAEnt :: SAEnt( std::string n, Scheduler *s, SAEntSM *sm, InetAddr6 ownAddr) 
  : PTask (n.c_str(), s, sm),
    up_ (this), 
    down_ (this, &peer_),
    SLPCommon(down_),
    peer_ (this, &down_),
    DATimer_ (this,  OTime (SAConfigurator::Instance()->slp_DAActiveDiscoveryInterval)),
    isDADetected( false ),
    ownAddr_( ownAddr ),
    csm_ (new SARegSM),
    regs_(this),
    regMux_(new SinglePropagator (new PortList), new MultiPropagator (new PortList) ),
    SARegs_()
{
   // connect regs_ port to mux
   regs_.connect (regMux_.createUser () );

   state = SAEntSM :: Ready;

   STATIC_SYMBOL (peer_, this);
   STATIC_SYMBOL (down_, this);
   STATIC_SYMBOL (up_, this);

   SAConfigurator *myconfig = SAConfigurator::Instance();

   slpPort_ = myconfig->slp_SlpPort;

   ip6scope_ = myconfig->slp_IPv6Scope;
   if (ip6scope_ > 5)
     ip6scope_ = 5;
   else if (ip6scope_ < 1)
     ip6scope_ = 1;

   // Join  servloc and servloc-da to receive srvtyperqsts and daadverts
   
   // moved to join()
   
}

SAEnt :: ~SAEnt( void )
{
   if( csm_ != 0 ) delete csm_;
}

bool SAEnt :: ready_SLPOpen (Message *msg) 
{
  // ignores language tag 
  // TODO: do something with langTag
  // returns an indication with a slp handle

   SASAP :: SLPOpen *open = (SASAP :: SLPOpen*) msg;
   SASAP :: SLPOpenInd* ind = new SASAP :: SLPOpenInd;
   
   ind->SLPHandle = getNewHandle();
   
   up_.putMessage( ind );
   
   return false;
}


bool SAEnt :: ready_SLPClose (Message *msg) 
{
  // gets slp handle and releases it
  // not implemented
  return false;
}


void SAEnt :: createSAReg(  SASAP :: registerService *reg )
{
     
   // initialize the created sareg with given url
   SAReg *reggi = new SAReg("sareg", reg, this, csm_);  
   _DOORS_DEBUG(0, "SAEnt :: createSAReg()");
   // create port in mux
   Port *p = regMux_.createProvider( isForSAReg );
   reggi->entity_.connect(p);

   // insert to
   if(reggi->isGood())
     SARegs_.insert( reggi );
//   DYNAMIC_SYMBOL( reggi, "reggi", this );
}

bool SAEnt :: ready_registerService (Message *msg) {

   // TODO:
   // get url, compare
   // if registration exists and registration has FRESH flag
   // then update it's reg info
   // else create new sareg or return an error that it already exists
   // 
   // 
   //  castaa message registerServiceksi
   SASAP :: registerService *r = ( SASAP :: registerService * ) msg;

   //   _DOORS_DEBUG(0, "registerService received. URL: " 
		//	  << r->URL.getServiceType().toString() );
   // create new SAReg
   //   ServiceURL url= r->URL;
   createSAReg( r ); 
   
   // why put 
   //   regs_.putMessage( msg );
   return false;
}

bool SAEnt :: ready_deregisterService (Message *msg) 
{
   _DOORS_DEBUG(0, "SAEnt :: ready_deregisterService() - deregister received from app" );
   // check that url  exists
   // if it does, delete it
   // note: only one registration will be deregistered
   // 
   SASAP :: deregisterService *dreg = (SASAP :: deregisterService*)msg;
   
//   ServiceURL url = dreg->URL;

   for( std::set<SAReg*>::iterator i = SARegs_.begin(); i != SARegs_.end(); ++i )
   {
     Byte *urlBytes = (Byte*)dreg->URL;
     std::string urlStr((char*)urlBytes);
     delete urlBytes;
     ServiceURL reg_url = ServiceURL(urlStr);

      if(  (*i)->getURL() == reg_url )
      {	 
	 SARegs_.erase( i );
	 break;
      }
   }
   
   return false;
}

bool SAEnt :: ready_addAttributes (Message *msg) {
  // check that there is a registration for given url
  // get attribute list
  // replace/modify/add attributes to list
  return false;
}

bool SAEnt :: ready_deleteAttributes (Message *msg) {
  // check that there is a registration for given url 
  // get attribute list
  // delete all given attributes

  return false;
}


bool SAEnt :: ready_default (Message *msg)
{
   // just drops messages
  _DOORS_DEBUG(0, "ready default. dropping message [" << msg << "]" );
   return false;
}

bool SAEnt :: ready_srvRqst (Message *msg)
{
     SLPPeer :: SRVRQST *rqst = (SLPPeer :: SRVRQST*) msg;
     _DOORS_DEBUG(0, "SAEnt :: ready_srvRqst - Servicetype:" << rqst->servicetype );
     
     if( rqst->source == ownAddr_) {
          _DOORS_DEBUG(0, "Received our own packet. Discarding SRVRQST");
          return false;
     }

     std::cout << "Service request for " << rqst->servicetype << " from "
               << rqst->source.getHostAddr() << std::endl;

     matchingURLs.clear();
     if( isInPRList( ownAddr_, rqst->PRList ) )
          return false;
   
     for( std::set<SAReg*>::iterator i = SARegs_.begin(); i != SARegs_.end(); ++i )
     {
          if( (*i)->isOfType( rqst->servicetype ) )
          {
               _DOORS_DEBUG(0, "Url matches. adding to list" );
               matchingURLs.push_back( (*i)->getURL()  );
          }
          
     }
   
     sendSrvRply( rqst );
   
     return false;
}

void SAEnt :: sendSrvRply( SLPPeer::SRVRQST *rqst  )
{
   _DOORS_DEBUG(0, "SAEnt :: sendSrvRply() - XID: " << rqst->XID );     
   SLPPeer :: SRVRPLY *rply = 
     (SLPPeer :: SRVRPLY*) peer_.create( SLPPeer :: srvrply );
   
   // if request was unicast or matching urls were found.
   // (must reply with empty reply if request was unicast)
   if( !rqst->requestMcast || !matchingURLs.empty() )
   {
        _DOORS_DEBUG(0,"SAEnt :: sendSrvRply() - replying to addr " << rqst->source.addrToString()
                     << " with " << matchingURLs.size() << " URLs.");
        rply->dest = rqst->source;
        rply->urlEntries = matchingURLs;
        rply->errorCode = 0;
        rply->XID = rqst->XID;
        peer_.putMessage( rply );
   }
   else
   {
      // else  nothing to reply and reques was multicast -> nothing to send
      _DOORS_DEBUG(0, "Request prcessed. Nothing to reply" );
      rply->unlock();
   }  
   
}

bool SAEnt :: ready_daAdvert (Message *msg)
{
     _DOORS_DEBUG(0, "SAEnt :: ready_daAdvert ()");
   SLPPeer :: DAADVERT *Advert = (SLPPeer :: DAADVERT*) msg;
   dafinder.updateDAList( Advert );
   std::cout << "DA Advert received from " << Advert->url.getHost() << std::endl;
   msg->setMultipleTargets();
   msg->lock();
   if( !regs_.putMessage (msg))
   {
      _DOORS_DEBUG(0,"message not passed");
   }
   else
   {
      _DOORS_DEBUG(0,"Ready_daadvert: message passed on to SAReg");
   }
   msg->unlock();

   return false;
}

  

bool SAEnt :: ready_attrRqst (Message *msg)
{
   SLPPeer :: ATTRRQST * rqst = (SLPPeer :: ATTRRQST *) msg;

   InetAddr6 ourHost(ownAddr_);
   ourHost.setPort(0);
   if( rqst->PRList.count(ourHost) ) {
      return false;
   }
   
   if( rqst->URL.isPureType() ) {
      msg->setMultipleTargets(); // Everyone's got to see this one
   }
   msg->lock();
   regs_.putMessage( msg );
   msg->unlock();
   return false;
}

bool SAEnt :: ready_srvTypeRqst (Message *msg)
{
   // Handled here in the entity task, not passed forward to conns
   SLPPeer :: SRVTYPERQST *rqst = (SLPPeer::SRVTYPERQST*)msg;

   if( isInPRList( ownAddr_, rqst->PRList ) ) {
	 return false;
   }

   SLPPeer :: SRVTYPERPLY *reply = 
	 (SLPPeer :: SRVTYPERPLY*)peer_.create( SLPPeer::srvtyperply );

   reply->dest = rqst->source;
   reply->errorCode = 0;
   reply->XID = rqst->XID;

	 // A unique list of our service types. We don't want
	 // each one several times in our reply, although we might
	 // have several services of some type
   std::set<std::string> types;
	 
   for( std::set<SAReg*>::iterator i = SARegs_.begin();
		i != SARegs_.end(); i++) {

	 types.insert(std::set<std::string>::value_type(((*i)->url_).getServiceType()));
   }

   for( std::set<std::string>::iterator j = types.begin();
		j != types.end(); j++ ) {
	 reply->typeList.push_back(*j);
   }

   peer_.putMessage(reply);

   return false;
}

bool SAEnt :: ready_srvAck (Message *msg)
{
   _DOORS_DEBUG(0, "forwarding message to regs" );

   SASAP :: registerInd *ind = (SASAP :: registerInd *) up_.create(  SASAP :: registerind );
   ind->errorcode = ((SLPPeer::SRVACK*)msg)->errorCode;
   up_.putMessage(ind);

   msg->lock();
   regs_.putMessage( msg );
   msg->unlock();
   return false;
}
bool SAEnt :: ready_addAttrs (Message *msg)
{
  return false;
}

bool SAEnt :: ready_deleteAttrs (Message *msg)
{
  return false;
}

bool SAEnt :: wfAck_Ack (Message *msg)
{
  return false;
}

bool SAEnt :: wfAck_default (Message *msg)
{
  return false;
}


bool SAEnt :: isInPRList( InetAddr6 &addr, AddrVector &PRList )
{
     _DOORS_DEBUG(0,"SAEnt :: isInPRList - PRList.size() == " << PRList.size() );
     
   for( AddrVector::iterator i = PRList.begin(); 
	i != PRList.end(); ++i )
   { 
      _DOORS_DEBUG(0,"Comparing own address: " << addr.addrToString() 
	    << " and address in list: " << (*i).addrToString() );
      
      if( *i == addr )
      {
	 _DOORS_DEBUG(0, "Own address in PRList.") ;
	 return true;
      }
   }
   _DOORS_DEBUG(0,"returning false" );
   return false;
}


void SAEnt :: join (void) {   //ugly hack -- remove at first chance. -Bill

   if (joinMCGroup ("service:directory-agent")) {
     
     _DOORS_DEBUG(0, "Sending JoinMcast to udp6task.");  

     // create sareg which advertizes sa itself
     // STILL TODO
     // send DA query

     solicitDirectoryAgent( );
     
     _DOORS_DEBUG(0, "samain constructed" );
   } else {
     _DOORS_DEBUG(0,"Didn't join SERVLOC-DA mcast group!");
   }

   joinMCGroup("SVRLOC");

}
