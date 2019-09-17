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

$Log: vuaconn.C,v $
Revision 1.6  2004/07/16 09:13:01  hartmanj
Added a small wait, before auto re-discovery is started. Must discover agents first

Revision 1.5  2004/07/12 07:26:00  hartmanj
General development

Revision 1.4  2004/05/20 07:00:09  hartmanj
Manual fix to last comment

Revision 1.3  2004/05/19 13:52:03  hartmanj
Update

Revision 1.2  2004/05/12 08:33:22  hartmanj
Development

Revision 1.1  2004/01/28 09:23:31  hartmanj
Initial revision. Compiles.

  
*/

#include <string>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "vuaconn.h"
#include "vuaconnsm.h"
#include "test_logger.h"
#include <doors/slptypes.h>
#include <doors/debug.h>

VUAConn::VUAConn( std::string n, Uint32 slphandle, VAEntity *vaentity,  VUAConnSM *sm )
     :  PTask (n, vaentity->getScheduler (), sm),
        up( vaentity->up ),
        peer( vaentity->peer ),
        mobapi( vaentity->mobapi ),
        entity ( this ),
        vaEntity_( vaentity ),
        request_(0),
        srequest_(0),
        strequest_(0),
        arequest_(0),
        myconfig(VAConfigurator::Instance()),
        timeUsed_(0),
        slpHandle_(slphandle),
        slpPort_(0),
        XID_(0),
        auto_discovery(false)
{
    _DOORS_DEBUG(0, "VUAConn::VUAConn: " << n);
    STATIC_SYMBOL(XID_, this);
    STATIC_SYMBOL(timeUsed_, this);
    STATIC_SYMBOL(slpHandle_, this);
    slpPort_ = myconfig->slp_SlpPort;
    state = VUAConnSM :: Idle;
    timer_.init( this, OTime( myconfig->CONFIG_RETRY ) );
    STATIC_SYMBOL(timer_, this);
}


// destructor
VUAConn :: ~VUAConn (void) 
{
   if( request_ )
      delete request_;

   // TODO: remove from mux (no support in MUX interface)
   // Note: destroying Port disconnects automagically..

   DELETE_SYMBOL (this);
}


Uint16 VUAConn :: getXID ( void )
{
     return XID_;
}


// message handlers
bool VUAConn :: mcWaitReply_Default (Message *)
{   
     _DOORS_DEBUG(0, "VUAConn::mcWaitReply_Default, dropping message" );

     // drop message
     return false;
}

bool VUAConn :: agentWaitReply_Default (Message *)
{   
     _DOORS_DEBUG(0, "VUAConn::agentWaitReply_Default, dropping message" );

     // drop message
     return false;
}

bool VUAConn :: idle_Default (Message *)
{
     _DOORS_DEBUG(0, "VUAConn::idle_Default, dropping message" );

     // drop message
     return false;
}

bool VUAConn :: idle_Srv_req( Message *msg )
{
    _DOORS_DEBUG(0, "VUAConn :: idle_Srv_req");

    VASAP :: Srv_req *req = (VASAP :: Srv_req*) msg;

    auto_discovery = req->auto_discovery;

    if( srequest_ ) {
        delete srequest_;
    }

    srequest_ = (SLPPeer::SRVRQST *) peer.create( SLPPeer::srvrqst );
    request_ = srequest_;

    srequest_->servicetype = req->sType;
    srequest_->scopeList.push_back("DEFAULT");
    srequest_->XID = XID_ = vaEntity_->getNewXID();
    srequest_->lock();

    if( vaEntity_->isDAPresent() ) {
        _DOORS_DEBUG(0, "Using DA to find service " << srequest_->servicetype);
        srequest_->dest = vaEntity_->getDAAddr();
        state = VUAConnSM :: agentWaitReply;

    } else if( vaEntity_->isAAPresent() ) {
        _DOORS_DEBUG(0, "Using AA to find service " << srequest_->servicetype);
        srequest_->dest = vaEntity_->getAAAddr();
        state = VUAConnSM :: agentWaitReply;

    } else {
        _DOORS_DEBUG(0, "Using multicast to find service " << srequest_->servicetype);
        srequest_->dest.set(slpPort_, vaEntity_->getMCAddr( srequest_->servicetype ));
        state = VUAConnSM :: mcWaitReply;
        _DOORS_DEBUG(0, "Multicast address " << srequest_->dest.getHostAddr());
    }

    TestLogger *log = TestLogger :: Instance();
    log->LogEvent( "VUAConn (" + getName() + "): sending service request for " + req->sType );

    peer.putMessage( request_->clone() );

    timeUsed_ = 0;
    timer_.start();
}

bool VUAConn :: idle_Srvtype_req(Message *msg)
{
    _DOORS_DEBUG(0, "VUAConn :: Idle_Srvtype_req" );

    VASAP :: Srvtype_req *m = ( VASAP :: Srvtype_req * ) msg;

    auto_discovery = m->auto_discovery;

    if( strequest_ ) {
        delete strequest_;
    }

    strequest_ = ( SLPPeer :: SRVTYPERQST * ) peer.create ( SLPPeer :: srvtyperqst );
     if( request_ ) {
         _DOORS_DEBUG(0, "Something fishy");
     }
    request_ = strequest_;
    
    strequest_->scopeList.push_back("DEFAULT");
    strequest_->XID = XID_ = vaEntity_->getNewXID();
    strequest_->lock();
    
    if( vaEntity_->isDAPresent() ) {

        strequest_->dest = vaEntity_->getDAAddr();
        state = VUAConnSM :: agentWaitReply;

    } else if( vaEntity_->isAAPresent() ) {

        strequest_->dest = vaEntity_->getAAAddr();
        state = VUAConnSM :: agentWaitReply;

    } else {

        strequest_->dest.set(slpPort_,vaEntity_->getMCAddr( "SVRLOC" ));
        state = VUAConnSM :: mcWaitReply;

    }

    TestLogger *log = TestLogger :: Instance();
    log->LogEvent( "VUAConn (" + getName() + "): sending service-type request to " + strequest_->dest.getHostAddr() );

    peer.putMessage(request_->clone());

    return false;
}

bool VUAConn :: idle_Attr_req(Message *msg)
{
    _DOORS_DEBUG(0,"VUAConn :: Idle_Attr_req");

     VASAP :: Attr_req *m = ( VASAP :: Attr_req * ) msg;

     if( arequest_ ) {
         _DOORS_DEBUG(0, "Something fishy");
         delete arequest_;
     }

     arequest_ = ( SLPPeer :: ATTRRQST * ) peer.create ( SLPPeer :: attrrqst );

     if( request_ ) {
         _DOORS_DEBUG(0, "Something fishy");
     }

     request_ = arequest_;
     arequest_->URL = m->URL;
     arequest_->scopeList.push_back("DEFAULT");
     //arequest_->tagList = m->attrIDs;
     arequest_->lock();
     if( vaEntity_->isDAPresent() ) {

         arequest_->dest = vaEntity_->getDAAddr();
         state = VUAConnSM :: agentWaitReply;

     } else if( vaEntity_->isAAPresent() ) {

         arequest_->dest = vaEntity_->getAAAddr();
         state = VUAConnSM :: agentWaitReply;

     } else {

         arequest_->dest.set(slpPort_,vaEntity_->getMCAddr( "SVRLOC" ));
         state = VUAConnSM :: mcWaitReply;

     }

     peer.putMessage(request_->clone());

    return false;
}

void VUAConn :: restartQuery() {

    _DOORS_DEBUG(0, "restartQuery in " << getName());
    TestLogger *log = TestLogger :: Instance();
    log->LogEvent( "VUAConn (" + getName() + "): auto-restarting query " );
    Message *peer_msg = 0;
    InetAddr6 dest_address;

    // Re-initiate the request
    // No agents should be present, but in case, let's check
    if( vaEntity_->isDAPresent() ) {
        _DOORS_DEBUG(0, "  restartQuery: Using DA");
        log->LogEvent( "  - using DA: " + vaEntity_->getDAAddr().getHostAddr());
        dest_address = vaEntity_->getDAAddr();
        state = VUAConnSM :: agentWaitReply;
        
    } else if( vaEntity_->isAAPresent() ) { 
        _DOORS_DEBUG(0, "  restartQuery: Using AA");
        log->LogEvent( "  - using AA: " + vaEntity_->getAAAddr().getHostAddr());
        dest_address = vaEntity_->getAAAddr();
        state = VUAConnSM :: agentWaitReply;
        
    } else {

        std::string service_type;

        if( request_ == srequest_ && srequest_ ) {
            service_type = srequest_->servicetype;
        } else if( request_ == strequest_ && strequest_ ) {
            service_type = "SVRLOC";
        }
        _DOORS_DEBUG(0, "  restartQuery: Using multicast to find " << service_type);
        log->LogEvent( "  - using mcast (SVRLOC)" );

        dest_address.set(slpPort_, vaEntity_->getMCAddr( service_type ));
        state = VUAConnSM :: mcWaitReply;
    }

    if( srequest_ ) {
        srequest_->XID = vaEntity_->getNewXID();
        XID_ = srequest_->XID;
        srequest_->dest = dest_address;
        srequest_->PRList.empty();
        peer_msg = srequest_->clone();
    }

    if( strequest_ ) {
        strequest_->XID = vaEntity_->getNewXID();
        XID_ = strequest_->XID;
        strequest_->dest = dest_address;
        strequest_->PRList.empty();
        peer_msg = strequest_->clone();
    }

    peer.putMessage( peer_msg );
    log->LogEvent( "  - request sent out." );

    timeUsed_ = 0;
    timer_.init( this, OTime( myconfig->CONFIG_RETRY ) );
    timer_.start();
}

bool VUAConn :: idle_movement( Message *msg ) {

    if( auto_discovery ) {

        // wait for 2 secs, so possible agents can be discovered and
        // sockets set to appropriate states
        timer_.init( this, OTime( 2 ) );
        state = VUAConnSM :: AutoRestartWait;
        timer_.start();
    }

    return false;
}

bool VUAConn :: autoRestartWait_Default( Message * ) {
    _DOORS_DEBUG(0, "VUAConn :: autoRestartWait_Default");
    return false;
}

bool VUAConn :: autoRestartWait_Timeout( Message * ) {
    _DOORS_DEBUG(0, "VUAConn :: autoRestartWait_Timeout");

    state = VUAConnSM :: Idle;

    restartQuery();

    return false;
}

// Just stop action, and restart if auto_discovery
bool VUAConn :: mcWaitReply_movement( Message *msg ) {

    if( auto_discovery ) {

        restartQuery();

    } else {

        if (request_) {
            delete request_;
            if( (strequest_&&srequest_)||(srequest_&&arequest_)||(arequest_&&strequest_) ) {
                _DOORS_DEBUG(0, "Something fishy");
            }
            request_ = 0; srequest_ = 0; strequest_ = 0; arequest_ = 0;
        }
        timeUsed_ = 0;
        XID_ = 0;
        state = VUAConnSM :: Idle;

    }

    return false;
}

// Just stop action, and restart if auto_discovery
bool VUAConn :: agentWaitReply_movement( Message *msg ) {

    if( auto_discovery ) {

        restartQuery();

    } else {

        if (request_) {
            delete request_;
            if( (strequest_&&srequest_)||(srequest_&&arequest_)||(arequest_&&strequest_) ) {
                _DOORS_DEBUG(0, "Something fishy");
            }
            request_ = 0; srequest_ = 0; strequest_ = 0; arequest_ = 0;
        }
        timeUsed_ = 0;
        XID_ = 0;
        state = VUAConnSM :: Idle;

    }

    return false;
}

bool VUAConn :: mcWaitReply_SrvRply (Message *msg)
{
     _DOORS_DEBUG(0, "VUAConn :: mcWaitReply_SrvRply" );

     if( request_ != srequest_ ) {
         _DOORS_DEBUG(0, "Not expecting SRVRPLY, discarding message");
         return false;
     }

     SLPPeer :: SRVRPLY* reply = (SLPPeer :: SRVRPLY*)msg;

     TestLogger *log = TestLogger :: Instance();
     log->LogEvent( "VUAConn (" + getName() + "): received service reply: " + reply->source.getHostAddr() );

     srequest_->PRList.insert( reply->source );
  
     _DOORS_DEBUG(0, "Received SRVRPLY from: " << reply->source.getHostAddr() );
   
     VASAP :: Srv_ind *ind = (VASAP :: Srv_ind *)up.create(VASAP :: srv_ind);
     ind->SLPHandle = slpHandle_; // set data
     ind->errorcode = 0;
     ind->urls.putLast((Byte*)reply->urlEntries[0].getURL().c_str(), reply->urlEntries[0].getURL().size());
   
     up.putMessage( ind ); // send message
     return false;
}

bool VUAConn :: agentWaitReply_SrvRply (Message *msg)
{
     _DOORS_DEBUG(0, "VUAConn :: agentWaitReply_SrvRply" );
     timer_.stop();

     if( request_ != srequest_ ) {
         _DOORS_DEBUG(0, "Not expecting SRVRPLY, discarding message");
         return false;
     }

     SLPPeer :: SRVRPLY* reply = (SLPPeer :: SRVRPLY*)msg;

     _DOORS_DEBUG(0, "Received SRVRPLY from: " << reply->source.getHostAddr() );

     VASAP :: Srv_ind *ind = (VASAP :: Srv_ind *)up.create(VASAP :: srv_ind);
     ind->SLPHandle = slpHandle_; // set data
     ind->errorcode = 0;
     ind->urls.putLast((Byte*)reply->urlEntries[0].getURL().c_str(), reply->urlEntries[0].getURL().size());
   
     up.putMessage( ind ); // send message

     if( request_ ) {
         request_->unlock();
         delete request_;
         request_ = 0;
         srequest_ = 0;
         strequest_ = 0;
         arequest_ = 0;
     }

     XID_ = 0;
     state = VUAConnSM :: Idle;
     return false;
}

bool VUAConn :: agentWaitReply_SrvtypeRply (Message *msg)
{
     _DOORS_DEBUG(0, "VUAConn :: agentWaitReply_SrvtypeRply" );
     timer_.stop();

     if( request_ != strequest_ ) {
         _DOORS_DEBUG(0, "Not expecting SRVTYPERPLY, discarding message");
         return false;
     }

     SLPPeer :: SRVTYPERPLY* reply = (SLPPeer :: SRVTYPERPLY*)msg;

     _DOORS_DEBUG(0, "Received SRVTYPERPLY from: " << reply->source.getHostAddr() );

     VASAP :: Srvtype_ind *ind = (VASAP :: Srvtype_ind *)up.create(VASAP :: srvtype_ind);
     ind->SLPHandle = slpHandle_;
     ind->errorcode = 0;
     for( unsigned int i = 0; i < reply->typeList.size(); i++ ) {
         ind->types.putLast((Byte*)reply->typeList[i].c_str(), reply->typeList[i].size());
         ind->types.putLast(' ');
     }
   
     up.putMessage( ind ); // send message

     if( auto_discovery ) {
         state = VUAConnSM :: Idle;
         return false;
     }

     if( request_ ) {
         request_->unlock();
         delete request_;
         request_ = 0;
         srequest_ = 0;
         strequest_ = 0;
         arequest_ = 0;
     }

     XID_ = 0;
     state = VUAConnSM :: Idle;
     return false;
}

bool VUAConn :: agentWaitReply_AttrRply (Message *msg)
{
     _DOORS_DEBUG(0, "VUAConn :: agentWaitReply_AttrRply" );
     timer_.stop();

     if( request_ != srequest_ ) {
         _DOORS_DEBUG(0, "Not expecting ATTRRPLY, discarding message");
         return false;
     }

     SLPPeer :: ATTRRPLY* reply = (SLPPeer :: ATTRRPLY*)msg;

     _DOORS_DEBUG(0, "Received ATTRRPLY from: " << reply->source.getHostAddr() );

     VASAP :: Attr_ind *ind = (VASAP :: Attr_ind *)up.create(VASAP :: attr_ind);
     ind->SLPHandle = slpHandle_;
     ind->errorcode = 0;
     for( unsigned int i = 0; i < reply->attributeList.size(); i++ ) {
         ind->attributes.putLast((Byte*)reply->attributeList[i].c_str(), reply->attributeList[i].size());
         ind->attributes.putLast(' ');
     }
     up.putMessage( ind ); // send message

     if( request_ ) {
         request_->unlock();
         delete request_;
         request_ = 0;
         srequest_ = 0;
         strequest_ = 0;
         arequest_ = 0;
     }

     XID_ = 0;
     state = VUAConnSM :: Idle;
     return false;
}

bool VUAConn :: mcWaitReply_SrvtypeRply (Message *msg)
{ 
     _DOORS_DEBUG(0, "VUAConn :: mcWaitReply_SrvtypeRply" );

     if( request_ != strequest_ ) {
         _DOORS_DEBUG(0, "Not expecting SRVTYPERPLY, discarding message");
         return false;
     }
     
     SLPPeer :: SRVTYPERPLY *rply = (SLPPeer :: SRVTYPERPLY *) msg;

     strequest_->PRList.insert( rply->source );

     VASAP :: Srvtype_ind *ind =
         (VASAP :: Srvtype_ind*)up.create(VASAP :: srvtype_ind);

     ind->SLPHandle = slpHandle_;
     ind->errorcode = 0;
     ind->types.putLast((Byte*)rply->typeList[0].c_str(), rply->typeList[0].size());
   
     up.putMessage( ind );
     return false;
}

bool VUAConn :: mcWaitReply_AttrRply (Message *msg)
{
     _DOORS_DEBUG(0, "VUAConn :: wfAttrRply_attrRply" );

     if( request_ != arequest_ ) {
         _DOORS_DEBUG(0, "Not expecting ATTRRPLY, discarding message");
         return false;
     }

     SLPPeer :: ATTRRPLY *rply = (SLPPeer :: ATTRRPLY *) msg;
     VASAP :: Attr_ind *ind = (VASAP :: Attr_ind *)up.create(VASAP :: attr_ind);
     ind->SLPHandle = slpHandle_;
     ind->errorcode = 0;
     ind->attributes.putLast((Byte*)rply->attributeList[0].c_str(),rply->attributeList[0].size());

     up.putMessage( ind );
     return false;
}

bool VUAConn :: agentWaitReply_Timeout(Message *)
{
    _DOORS_DEBUG(0, "VUAConn :: agentWaitReply_Timeout" );

    if( timeUsed_ <= myconfig->CONFIG_RETRY_MAX ) {
        
        request_->lock();
        peer.putMessage( request_->clone() );
        timeUsed_ += myconfig->CONFIG_RETRY;
        timer_.start();
    } else {

        // request timed out
        
        if( request_ != srequest_ && request_ != strequest_ && request_ != arequest_ )
            _DOORS_DEBUG(0, "Something fishy!");
        if( ( srequest_ && strequest_ ) || ( arequest_ && strequest_ ) || ( srequest_ && arequest_ ) )
            _DOORS_DEBUG(0, "Something fishy!");

        if( auto_discovery == true ) {
            // don't delete request_, go to "sleep"
            // don't reset XID, so conn-task stays reserved
            timeUsed_ = 0;
            state = VUAConnSM :: Idle;
            _DOORS_DEBUG(0, "Service request(Agent) with auto-discovery finished for now, going to sleep");
            return false;
        }

        if( request_ ) {
            request_->unlock();
            delete request_;
            request_ = 0; srequest_ = 0; strequest_ = 0; arequest_ = 0;
        }
        timeUsed_ = 0;
        XID_ = 0;
        state = VUAConnSM :: Idle;
    }

    return false;
}

bool VUAConn :: mcWaitReply_Timeout(Message *)
{
    _DOORS_DEBUG(0, "VUAConn :: mcWaitReply_timeout" );
    
    timer_.stop();

    if (timeUsed_ <= myconfig->CONFIG_MC_MAX) {
        
        peer.putMessage ((request_->clone()));
        timeUsed_ += myconfig->CONFIG_RETRY;
        timer_.start();
    } else {

        if( auto_discovery == true ) {
            // don't delete request_, go to "sleep"
            // don't reset XID, so conn-task stays reserved
            timeUsed_ = 0;
            state = VUAConnSM :: Idle;
            _DOORS_DEBUG(0, "Service request(MC) with auto-discovery finished for now, going to sleep");
            return false;
        }

        if (request_) {
            delete request_;
            if( (strequest_&&srequest_)||(srequest_&&arequest_)||(arequest_&&strequest_) ) {
                _DOORS_DEBUG(0, "Something fishy");
            }
            request_ = 0; srequest_ = 0; strequest_ = 0; arequest_ = 0;
        }
        timeUsed_ = 0;
        XID_ = 0;
        state = VUAConnSM :: Idle;
    }

    return false;
}
