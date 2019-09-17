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

$Log: daent.C,v $
Revision 1.3  2004/07/11 11:25:08  hartmanj
Copy request's XID to reply in SRVTYPERQST

Revision 1.2  2004/04/14 09:17:18  hartmanj
Bug fixing

Revision 1.1  2004/04/05 06:53:45  hartmanj
Renamed damain* to daent*

Revision 1.25  2004/03/25 12:51:59  hartmanj
Service type can be set different from url scheme when registering a service.

Revision 1.24  2004/03/22 11:08:09  hartmanj
Moved createAttributeList() to SLPCommon

Revision 1.23  2004/03/19 11:22:44  hartmanj
Adapting new SLPCommon API. Service type request and attribute request implemented (but not yet tested)

Revision 1.22  2004/03/16 08:52:55  hartmanj
Loads of cleaning up and bugfixing. Added lifetime expiration detection. Indentation cleaned. Mcast joining properly implemented etc etc

Revision 1.21  2004/03/12 13:14:34  hartmanj
Correcting myself, forgot '[]':s in service url with numerical IPv6 address

Revision 1.20  2004/03/11 14:10:41  hartmanj
Variant removed

Revision 1.19  2004/03/10 07:11:20  bilhanan
commented DOORS DEBUG call owing to nonexistent Services_

Revision 1.18  2004/03/09 11:16:01  hartmanj
Changed services data structure. Added std::couts to report actual DA operations when debug-messages are not enabled

Revision 1.17  2004/03/09 09:43:06  bilhanan
changed call to getMCAddr, using DAConfigurator and removed
parameter passing to SLPCommon in constructor

Revision 1.16  2004/03/01 20:39:20  bilhanan
forced commit owing to empty log in previous commit: removed joining DA group in
constructor, UDP6Task joins Da group by default at startup in main file.

Revision 1.15  2004/03/01 20:36:23  bilhanan

with `CVS:' are removed automatically CVS: CVS: Committing in .  CVS: CVS: Modified Files:  CVS: damain.C

Revision 1.14  2004/03/01 18:25:14  bilhanan
readConfig() disappears in a puff of smoke. DAConfigurator now used.

Revision 1.13  2004/03/01 11:28:03  bilhanan
removed URLVector Services_ initialisation to 0, to prevent SUN CC crashes.

Revision 1.12  2004/03/01 09:04:44  hartmanj
Fixed XID handling when replying to SRVRQST

Revision 1.11  2004/02/27 10:22:39  hartmanj
Append servicetype string with service: in DAMain::ready_srvReg in getMCAddr() call to get correct address

Revision 1.10  2004/02/26 10:58:45  hartmanj
Fixing and rewriting

Revision 1.9  2004/02/12 11:44:11  bilhanan
DA Stateless Boot Timestap fix

Revision 1.8  2004/02/12 10:26:48  bilhanan
fixed for multicasting DA adverts properly.

Revision 1.7  2004/02/10 18:11:48  bilhanan
joining multicast groups changed
removed dependency to UDP6Task in constructor

Revision 1.6  2003/11/27 12:32:57  bilhanan
bug fixing

Revision 1.5  2003/08/12 00:05:01  bilhanan
const char* in constructor -> std::string

Revision 1.4  2003/06/13 12:51:17  bilhanan
test registration removed.

Revision 1.3  2003/06/04 11:09:57  kalliosa
Licence clause added

  
*/

#include <sys/types.h>
#include <time.h>
#include <string>
#include <sstream>
#include <fstream>
#include <doors/udp6task.h>
#include <doors/ptb.h>
#include <doors/daent.h>
#include <doors/daentsm.h>
#include <doors/slppdu.h>
#include <doors/inetaddr6.h>
#include <doors/daconfigurator.h> 
#include <doors/debug.h>

DAEnt ::  DAEnt( std::string n, 
		   Scheduler *s, 
		   DAEntSM *sm, 
		   InetAddr6 &addr )
    :PTask (n, s, sm),
     peer_( this,  &down_ ),
     down_( this,  &peer_ ),
     SLPCommon(down_),
     ownAddr_( addr ),
     daTime_(0),
     firstAdv_(true),
     loggedadvsend_(0)
{
    // TODO: timer should be randomized

    DAConfigurator *myconfig = DAConfigurator::Instance();

    heartBeat_ = myconfig->slp_DAHeartBeat;
    slpPort_ = myconfig->slp_SlpPort;
   
    ip6scope_ = myconfig->slp_IPv6Scope;
    if (ip6scope_ > 5)
        ip6scope_ = 5;
    else if (ip6scope_ < 1)
        ip6scope_ = 1;

    bootstamp = (Uint32) time (0);

    SRVLOCDAGroup_.set(slpPort_, getMCAddr ("service:directory-agent"));

    // to force sending first advert
    advTimer_.init(this, OTime(0,1));
    advTimer_.start();
}

DAEnt::~DAEnt()
{

}

bool DAEnt :: ready_default(Message *msg )
{
    _DOORS_DEBUG(0, "ready_default." );
    // just drop message.
    return false;
}

bool DAEnt :: ready_DATimer( Message *msg )
{
    loggedadvsend_++;
    if( loggedadvsend_ < 2 ) {
        std::cout << "* Sending DA advert to scope " << ip6scope_ << " .";
        std::cout.flush();
    } else {
        std::cout << ".";
        std::cout.flush();
    }

    _DOORS_DEBUG(0, "sending DAAdvert... " );

    daTime_ += advTimer_.getInitial().getSeconds();

    sendDAAdv (SRVLOCDAGroup_);

    if( firstAdv_ ) {

        joinMCGroup("service:directory-agent");
        joinMCGroup("SVRLOC");
        
        //advTimer_.init( this, OTime(heartBeat_) );
        OTime o(heartBeat_);
        advTimer_.setInitial(o);
        firstAdv_ = false;
    }
    
    //   generates internal compiler error.. strange
    advTimer_.start ();

    return false;
}

   
// searches all stored urls and returns any matching urls
// 
bool DAEnt ::  ready_srvRqst (Message *msg )
{
    SLPPeer :: SRVRQST *rqst = ( SLPPeer :: SRVRQST *) msg;

    _DOORS_DEBUG(0,"ready_srvrqst. Received rqst from: " << rqst->source.addrToString());
    if(loggedadvsend_ > 2) {
        std::cout << std::endl;
        loggedadvsend_ = 0;
    }
    std::cout << "* Service request for " << rqst->servicetype <<
        " received from " << rqst->source.getHostAddr() << std::endl;
   
    // check PRList 
  
    for(  AddrVector::iterator idx = rqst->PRList.begin(); 
          idx != rqst->PRList.end(); ++idx )
    {      
   
        if( (*idx) == ownAddr_ )
        {
            _DOORS_DEBUG(0, "Own address in PRList" );
            std::cout << "  - Reply already sent to this request" << std::endl;
            return false;
        }      
    }   

    if( rqst->servicetype == std::string("service:directory-agent") )
    {
        _DOORS_DEBUG(0, "replying with DAAdvert");
        std::cout << "  - Replying with DAADVERT" << std::endl;
        sendDAAdv( rqst->source ) ;
        return false;
    }   
   
   
    URLVector matchingURLs;
   
    // iterate all registered urls
    // TODO: language tag and scope
    for( unsigned int i=0;i<services_.size();i++)
    {      
        if( services_[i].url.isOfServiceType( rqst->servicetype ) )
        {
            // Check if service registration is still valid. If not, remove from list
            if( services_[i].lastRegTime + services_[i].url.getLifetime() < daTime_ ) {
                std::cout << "  - Erased service " << services_[i].url.getServiceType()
                          << " because lifetime expired." << std::endl;
                services_.erase( services_.begin() + i );
                // TODO: should mcast leave
            } else {
                matchingURLs.push_back( services_[i].url  );
                std::cout << "  - Replying with " << services_[i].url.getURL() << std::endl;
            }
        } 
    }
    // no size check yet 
    _DOORS_DEBUG(0, "sending srvrply to: " << rqst->source.addrToString() );
    if( !matchingURLs.empty()) {
        sendReply( rqst->source, matchingURLs, rqst->XID );
    }

    return false; 
}


void DAEnt :: sendDAAdv( InetAddr6 &dest ) 
{
    _DOORS_DEBUG(0, "sending DAAdv" );
   
    SLPPeer :: DAADVERT *adv
        = (SLPPeer :: DAADVERT *) peer_.create (SLPPeer :: daadvert );

    adv->XID = getNewXID();
    adv->errorCode = 0;
    adv->DABootStamp = bootstamp;


    // concatenating IPv6 address to servicetype
    std::ostringstream tmpAddr;
    tmpAddr << '[' << ownAddr_.getHostAddr() << ']' << ":" << ownAddr_.getPortNumber();
    _DOORS_DEBUG(0, "putting address to daadv: " << tmpAddr.str() );
    adv->url = ServiceURL (  "service:directory-agent://" + tmpAddr.str() );

    adv->scopeList.push_back("DEFAULT");
    adv->dest = dest;

    peer_.putMessage( adv );
}


void DAEnt :: sendReply ( InetAddr6 dest, URLVector &URLs, Uint16 replyXID )
{
    SLPPeer :: SRVRPLY *rply =
        ( SLPPeer :: SRVRPLY * ) peer_.create(  SLPPeer :: srvrply );

    rply->dest = dest;
    rply->urlEntries  = URLs;
    rply->errorCode = 0;
    rply->XID = replyXID;
    _DOORS_DEBUG(0,"Sending srvrply to: " << rply->dest.addrToString() << " with XID " << rply->XID );
    if(loggedadvsend_ > 2) {
        std::cout << std::endl;
        loggedadvsend_ = 0;
    }
    peer_.putMessage( rply );
}


// saves given url and sends acknowledgement
bool DAEnt ::  ready_srvReg(Message *msg )
{ 
    SLPPeer :: SRVREG *reg = ( SLPPeer :: SRVREG *) msg;

    for(unsigned int i = 0; i < services_.size(); i++) {
        if( services_[i].url == reg->url ) {
            // TODO: scope handling
            // re-registration or incremental
            services_[i].url.setLifetime(reg->url.getLifetime());
            services_[i].url.setServiceType( reg->serviceType );
            services_[i].languageTag = reg->langTag;
            createAttributeList(services_[i].attributes,reg->attributeList);
            services_[i].lastRegTime = daTime_;
            if(loggedadvsend_ > 2) {
                std::cout << std::endl;
                loggedadvsend_ = 0;
            }
            std::cout << "* Service " << services_[i].url.getURL() << " re-registered" << std::endl;
            // create and send ack
            SLPPeer :: SRVACK *ack = 
                ( SLPPeer :: SRVACK * ) peer_.create(  SLPPeer :: srvack );
            ack->dest = reg->source;
            ack->XID = reg->XID;
            ack->errorCode = 0;   
            peer_.putMessage( ack );
            return false;
        }
    }

    ServiceInfo newService;

    newService.url = reg->url;
    newService.url.setServiceType( reg->serviceType );
    newService.languageTag = reg->langTag;
    createAttributeList(newService.attributes,reg->attributeList);
    newService.lastRegTime = daTime_;

    services_.push_back(newService);

    if(loggedadvsend_ > 2) {
        std::cout << std::endl;
        loggedadvsend_ = 0;
    }
    std::cout << "* Service " << reg->url.getServiceType() << " registered by " << reg->source.getHostAddr() << std::endl;
	std::cout << "*   Service attributes: ";
	for(unsigned int i=0;i<newService.attributes.size();i++)
	  std::cout << "[" << i << "]: " << newService.attributes[i].AttributeName << " - ";
	std::cout << std::endl;
    

    // Join multicast groups
    joinMCGroup( reg->url.getServiceType() );
    if( reg->url.getServiceType() != reg->url.getAbstractType() ) {
        joinMCGroup( reg->url.getAbstractType() );
    }
    
    for(unsigned int i=0;i<services_.size();i++) {
        //        _DOORS_DEBUG(0, "Registered Service nro." << i << " is " << Services_[i].getURL());
        std::cout << " * Registered Service nro." << i << " is " << services_[i].url.getURL() << std::endl;
    }   
   
    // create and send ack
    SLPPeer :: SRVACK *ack = 
        ( SLPPeer :: SRVACK * ) peer_.create(  SLPPeer :: srvack );
    ack->dest = reg->source;
    ack->XID = reg->XID;
    ack->errorCode = 0;   
    peer_.putMessage( ack );
    return false;
}

bool DAEnt ::  ready_srvDeReg(Message *msg )
{
    _DOORS_DEBUG(0, "DAEnt ::  ready_srvDeReg");
    SLPPeer :: SRVDEREG *dereg_msg = (SLPPeer :: SRVDEREG*)msg;

    std::cout << "* Service-agent (" << dereg_msg->source.getHostAddr() << ")"
              << " sent un-registration for service URL: " << dereg_msg->url.getURL() << std::endl;

    bool found_service = false;
    for(unsigned int i = 0; i < services_.size(); i++) {
        if( services_[i].url == dereg_msg->url ) {
            // Remove service from list
            services_.erase( services_.begin() + i );
            found_service = true;
            break;
        }
    }

    if( found_service ) {
        std::cout << "*  - Service removed from list" << std::endl;
        SLPPeer :: SRVACK *ack = 
            ( SLPPeer :: SRVACK * ) peer_.create(  SLPPeer :: srvack );
        ack->dest = dereg_msg->source;
        ack->XID = dereg_msg->XID;
        ack->errorCode = 0;   
        peer_.putMessage( ack );
    } else {
        std::cout << "*  - Service NOT removed from list, no such registration found." << std::endl;
    }
    return false; 
}

bool DAEnt ::  ready_attrRqst(Message *msg )
{
    SLPPeer :: ATTRRQST *req = (SLPPeer :: ATTRRQST*) msg;

    unsigned int attrlistlength = 0;
    unsigned int serviceIdx = 0;

    if( services_.size() ) {
        while( serviceIdx < services_.size() ) {
            if( services_[serviceIdx].url == req->URL ) {
                break;
            }
            serviceIdx++;
        }
    }

    if( serviceIdx < services_.size() ) {
        SLPPeer :: ATTRRPLY *reply = (SLPPeer :: ATTRRPLY*) peer_.create ( SLPPeer :: attrrply );

        for( unsigned int j = 0; j < req->tagList.size(); j++ ) {
            // TODO: Choose one value for attribute (possibly of several values arbitrarily)
            for( unsigned int p = 0; p < services_[serviceIdx].attributes.size(); p++) {
                if( req->tagList[j] == services_[serviceIdx].attributes[p].AttributeName ) {

                   if( services_[serviceIdx].attributes[p].AttributeValue.length() ) {
                      // Attribute with a value
                       reply->attributeList.push_back(
                            std::string( services_[serviceIdx].attributes[p].AttributeName + '='
                                         + services_[serviceIdx].attributes[p].AttributeValue )
                            );
                    } else {
                       // Just a tag
                       reply->attributeList.push_back(
                          std::string( services_[serviceIdx].attributes[p].AttributeName)
                          );
                       
                    }
                }
            }
        }

        reply->dest = req->source;
        reply->XID = req->XID;
        peer_.putMessage( reply );
    }

    return false; 
}
bool DAEnt ::  ready_srvTypeRqst(Message *msg )
{
    SLPPeer :: SRVTYPERQST *req = (SLPPeer :: SRVTYPERQST*) msg;
    SLPPeer :: SRVTYPERPLY *reply = (SLPPeer :: SRVTYPERPLY*)peer_.create(SLPPeer :: srvtyperply);
    
    // TODO: check a good value for MTU
    unsigned int stypelistlength = 0;
    
    for( unsigned int i=0; i < services_.size() && stypelistlength < 1000; i++ ) {
        reply->typeList.push_back(services_[i].url.getServiceType());
        if( reply->typeList.size() ) // double checking
            stypelistlength += reply->typeList.back().size();
    }

    reply->dest = req->source;
    reply->XID = req->XID;
    reply->errorCode = 0;

    peer_.putMessage( reply );

    return false; 
}
   
