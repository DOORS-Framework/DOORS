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
//  THIS SOFTWARE IS PROVIDED BY TAMPERE UNIVERSITY OF TECHNOaLOGY AND
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

$Log: vaent.C,v $
Revision 1.17  2007/10/04 05:41:31  bilhanan
devt

Revision 1.16  2007/10/03 09:06:05  bilhanan
devt

Revision 1.15  2007/10/02 21:40:36  bilhanan
devt

Revision 1.14  2007/10/02 20:18:52  bilhanan
devt

Revision 1.13  2007/06/19 09:22:54  bilhanan
bugfix

Revision 1.12  2007/06/19 08:04:36  bilhanan
testing rebinding

Revision 1.11  2007/02/23 10:43:10  bilhanan
supporting ll address retrieval from config

Revision 1.10  2007/01/15 11:03:19  bilhanan
devt

Revision 1.9  2007/01/11 20:29:27  bilhanan
devt

Revision 1.8  2007/01/11 11:35:49  bilhanan
devt

Revision 1.7  2007/01/11 10:06:06  bilhanan
devt

Revision 1.6  2007/01/11 08:08:33  bilhanan
devt

Revision 1.5  2007/01/08 14:31:37  bilhanan
devt

Revision 1.4  2006/12/23 21:45:03  bilhanan
devt

Revision 1.3  2006/12/21 13:13:57  bilhanan
devt

Revision 1.2  2006/12/13 22:00:58  bilhanan
development

Revision 1.1  2006/08/10 15:39:07  bilhanan
initial commit

Revision 1.10  2004/08/16 10:21:04  bilhanan
need not send UDP::Setup to rebind each time to current COA, because we
now use in6addr_any.

Revision 1.9  2004/07/16 09:08:08  hartmanj
Lot's of log messages added. Start timer at *_movement(), so solicitations are sent out after timeout

Revision 1.8  2004/07/14 10:30:48  bilhanan
prototype fix to use COA in FNs.

Revision 1.7  2004/07/12 07:26:00  hartmanj
General development

Revision 1.6  2004/06/17 08:56:30  hartmanj
Development.

Revision 1.5  2004/05/21 06:09:12  hartmanj
MobilitySAP added. Names of some handler functions changed

Revision 1.4  2004/05/20 07:00:09  hartmanj
Manual fix to last comment

Revision 1.3  2004/05/19 13:52:03  hartmanj
Update

Revision 1.2  2004/05/12 08:33:21  hartmanj
Development

Revision 1.1  2004/01/28 09:23:31  hartmanj
Initial revision. Compiles.


*/

#include <sstream>
#include <fstream>
#include <string>
#include <doors/udp6task.h>
#include <doors/ptb.h>
#include <doors/slppdu.h>
#include <doors/inetaddr6.h>
#include <doors/configreader.h> 
#include <doors/debug.h>
#include <doors/portmux.h>
#include "vaent.h"
#include "vaesm.h"
#include "vuaconn.h"
#include "vuaconnsm.h"
#include "agentconn.h"
#include "agentconnsm.h"
#include "servicereg.h"
#include "serviceregsm.h"
#include "vaconfigurator.h"
#include "test_logger.h"

static bool isForMovementTask (const EventTask *t, const Message *m) {

  if (m->isForMany())
    return true;

  return false;
  
}

static bool isForServiceReg(const EventTask *t, const Message *m) 
{
    _DOORS_DEBUG(0, "isForServiceReg");
    if( m->isForMany() )
        return true;

    ServiceReg *conn = (ServiceReg*) t;

    const SLPHeader *msg = dynamic_cast<const SLPHeader*>(m);

     if( msg ) {
         _DOORS_DEBUG(0, "Message from SLPPeer");

         if( (m->getType() == SLPPeer :: srvrqst ) ||
             (m->getType() == SLPPeer :: attrrqst ))
         {

             if( (m->getType() == SLPPeer :: srvrqst )
                 && conn->isOfType(((SLPPeer::SRVRQST*)m)->servicetype) ) {
                 return true;
             }

             if( (m->getType() == SLPPeer :: attrrqst)
                 && (conn->getURL() == ((SLPPeer::ATTRRQST*)m)->URL ) ) {
                 return true;
             }
         }

         else if ( (m->getType() == SLPPeer :: srvack) )
         {
             // message type is srvack
             // compare acks xid to messages xid
             if ( conn->usingXID( msg->XID ) )
                 return true;
         }  

         return false;
     }

     const VASAP :: registerService *reg = dynamic_cast<const VASAP :: registerService*>(m);

     if( reg ) {

         if( ((ServiceReg*)t)->isFree() )
             return true;

         return false;

     }

     _DOORS_DEBUG(0, "Unknown message in isForServiceReg");

     return false;
}

static bool isForVUAConn(const EventTask *t, const Message *m) 
{

     VUAConn *conn = (VUAConn*) t;
     Uint16 connTaskXID = conn->getXID();
     Uint32 up_handle = 0;

     if( m->getTarget() == &conn->vaEntity_->peer ) {
         _DOORS_DEBUG(0, "  isForVUAConn - message from peer");
         int msg_xid = -1;
         switch( m->getType() ) {
         case SLPPeer :: srvrply:
             msg_xid = ((SLPPeer :: SRVRPLY*)m)->XID;
         case SLPPeer :: srvtyperply:
             msg_xid = ((SLPPeer :: SRVTYPERPLY*)m)->XID;
         case SLPPeer :: attrrply:
             msg_xid = ((SLPPeer :: ATTRRPLY*)m)->XID;
             if( msg_xid == connTaskXID )
                 return true;
             break;
         default:
             break;
         }

         return false;
     }

     if (m->getTarget() == &conn->up) {
         switch (m->getType()) {

         case (VASAP :: srv_req):
             up_handle = ((VASAP :: Srv_req *) m)->SLPHandle;
             break;

         case (VASAP :: slpclose):
             up_handle = ((VASAP :: SLPClose *) m)->SLPHandle;
             break;

         case (VASAP :: srvtype_req):
             up_handle = ((VASAP :: Srvtype_req *) m)->SLPHandle;
             break;

         case (VASAP :: attr_req):
             up_handle = ((VASAP :: Attr_req *) m)->SLPHandle;
             break;
             
         default:
             break;
         }
     }

     if( connTaskXID == 0 && conn->getptaskid() == up_handle )
         return true;

     return false;
}

VAEntity::~VAEntity()
{

    for(unsigned int i=0; i < agentConnList.size(); i++)
        delete agentConnList[i];
}

VAEntity::VAEntity (std::string n, Scheduler *s, VAEntitySM *sm, InetAddr6 &addr) :
     PTask (n, s, sm),
     down( this,  &peer ),
     SLPCommon(down ),
     up(this),
     peer(this,  &down),
     sreg_sap(this),
     mdprovider (this),
     vuaConns(this),
     serviceRegs(this),
     MNMux (new SinglePropagator (new PortList), new MultiPropagator (new PortList)),
     vuaConnMux(new SinglePropagator (new PortList), new MultiPropagator (new PortList)),
     serviceRegMux(new SinglePropagator (new PortList), new MultiPropagator (new PortList)),
     serviceRegSM(new ServiceRegSM),
     vuaConnSM(new VUAConnSM),
     agentConnSM(new AgentConnSM),
     isDAPresentflag(false),
     isAAPresentflag(false),
     currAddr( addr ),
     homeAddr(addr),
     myconfig(VAConfigurator::Instance()),
     onesecond_timer( this, OTime(1)),
     normal_timer( this, OTime(10) ),
     closed_timer( this, OTime(myconfig->CONFIG_START_WAIT) ),
     configretry_timer( this, OTime(myconfig->CONFIG_RETRY) ),
    sinceLastAAAdvert(0),
     AAAdvertTimeoutMax(100),
     sinceLastDAAdvert(0),
     DAAdvertTimeoutMax(100),
     aaSolXid(0),daSolXid(0),
     inNewNetwork(true),
     timeUsed(0),
     maxTimeToSolicit(myconfig->CONFIG_MC_MAX),
     config_retry(myconfig->CONFIG_RETRY),
     onesecond_timeout(1),
     normal_timeout(10)
{
    STATIC_SYMBOL(isAAPresentflag,this);
    STATIC_SYMBOL(isDAPresentflag,this);
     slpPort_ = myconfig->slp_SlpPort;
     ip6scope_ = 2;

     agentConnList.clear();
     freeAgents.clear();

     if( myconfig->slpm6_HomeAddr != "" ) {
         homeAddr.set(myconfig->slp_SlpPort, myconfig->slpm6_HomeAddr);
     }
     if (myconfig->slpm6_LinkLocalAddr != "") {
       linksuffix = myconfig->slpm6_LinkLocalAddr;
     }
     if( myconfig->slpm6_AAAdvertTimeoutMax != 0 ) {
         AAAdvertTimeoutMax = myconfig->slpm6_AAAdvertTimeoutMax;
     }


     mdprovider.connect (MNMux.createUser());
     vuaConns.connect(vuaConnMux.createUser());
     serviceRegs.connect(serviceRegMux.createUser());

     state = VAEntitySM :: Closed;
     closed_timer.start();

     maxTimeToSolicit = 2;

     TestLogger *log = TestLogger :: Instance();
     log->LogEvent( "VAEntity constructed" );
}

void VAEntity::join(void)
{

  // We need to decouple the joining from the sending.
  _DOORS_DEBUG(0, "Joining multicast group for service:access-agent");
  TestLogger *log = TestLogger :: Instance();
  log->LogEvent( "VAEntity:" );
  log->LogEvent( " - Sending ICMPv6 to join mcast groups" );

  if (joinMCGroup ("service:access-agent")) {
        sendAASolicitation( );
        log->LogEvent( " - Sending AA solicitation" );
    

    } else {
        _DOORS_DEBUG(0,"Couldn't join service:access-agent multicast group!");
    }

    
    _DOORS_DEBUG(0, "Joining multicast group SVRLOC-DA");
    
    if(joinMCGroup("SVRLOC-DA")) {
        sendDASolicitation( );
        log->LogEvent( " - Sending DA solicitation" );

    } else {
        _DOORS_DEBUG(0,"Couldn't join SVRLOC-DA multicast group!");
    }


    _DOORS_DEBUG(0, "Joining multicast group SVRLOC");
    
    if( !joinMCGroup("SVRLOC") ) {
        _DOORS_DEBUG(0,"Couldn't join SVRLOC multicast group!");
    }

    log->LogEvent( " - joins sent to mcast groups" );
}

bool VAEntity::isDAPresent(void) const
{
     return isDAPresentflag;
}

bool VAEntity::isAAPresent(void) const
{
     return isAAPresentflag;
}

InetAddr6 VAEntity::getAAAddr(void) const
{
     return aaAddr;
}

InetAddr6 VAEntity::getDAAddr(void) const
{
     return daAddr;
}

InetAddr6 VAEntity::getCurrAddr(void) const
{
     return currAddr;
}

InetAddr6 VAEntity::getHomeAddr(void) const
{
     return homeAddr;
}

void VAEntity::constructAddr (void){

  std::string tmpaddrstr = newAAPrefix+linksuffix;
  newAddr.set(myconfig->slp_SlpPort, tmpaddrstr);
}

bool VAEntity :: checkHandle( Uint32 h ) const
{
    if( openHandles.count(h) == 1 )
        return true;

    return false;
}

AgentConn* VAEntity :: getNewAgentConn(void)
{
    AgentConn *agent = 0;

    if( freeAgents.size() > 0 ) {
        agent = freeAgents.back();
        freeAgents.pop_back();
        _DOORS_DEBUG(0, "Returning free agent [" << agent << "]");
        if( agent )
            return agent;
        _DOORS_ERROR(0, "Free agent didn't pop!");
    }

    std::ostringstream cname;
    cname << getName () << ".agentconn." << agentConnList.size()+1;
    agent = new AgentConn( cname.str(), this, agentConnSM );

    agentConnList.push_back( agent );

    if( !agent )
        _DOORS_ERROR(0, "Memory allocation failed!");

    _DOORS_DEBUG(0, "Returning free agent [" << agent << "]");
    return agent;
}

void VAEntity :: releaseAgent( AgentConn *agent )
{
    //DELETE_SYMBOL(agent);
    freeAgents.push_back(agent);
}

bool VAEntity::checkAddrChange(void)
{
  if (newAddr == currAddr)
    return false;

  currAddr.set(myconfig->slp_SlpPort, newAddr.getHostAddr());
    
  MoveNotifySAP::Movement *mdprovidermsg = (MoveNotifySAP::Movement *) mdprovider.create (MoveNotifySAP::movement);
  mdprovidermsg->setMultipleTargets();
  mdprovider.putMessage (mdprovidermsg);
  return true;

}

bool VAEntity :: isAtHome(void)
{

  if (newAddr != currAddr)
    currAddr.set(myconfig->slp_SlpPort, newAddr.getHostAddr());
    
 return (currAddr==homeAddr);
}

bool VAEntity :: aapresent_default( Message *msg )
{
    _DOORS_DEBUG(0, "VAEntity :: aapresent_default");
    return false;
}

bool VAEntity :: dapresent_default( Message *msg )
{
    _DOORS_DEBUG(0, "VAEntity :: dapresent_default");
    return false;
}

bool VAEntity :: bothpresent_default( Message *msg )
{
    _DOORS_DEBUG(0, "VAEntity :: bothpresent_default");
    return false;
}

bool VAEntity :: alone_default( Message *msg )
{
    _DOORS_DEBUG(0, "VAEntity :: alone_default");
    return false;
}

bool VAEntity::aapresent_aaadvert(Message *msg)
{
     _DOORS_DEBUG(0, "VAEntity::aapresent_aaadvert");
     SLPPeer::AAADVERT *adv = (SLPPeer::AAADVERT*)msg;
     _DOORS_DEBUG(0, "AA Advertisement received for AA "<<adv->aa_url.getHostAddr().getHostAddr());
     TestLogger *log = TestLogger :: Instance();
     log->LogEvent( "VAEntity: aaadvert received" );

     sinceLastAAAdvert = 0;

     if( !adv->da_url.isEmpty() ) {
       state = VAEntitySM :: BothPresent;
       isDAPresentflag = true;
       sinceLastDAAdvert = 0;
       if( daAddr != adv->da_url.getHostAddr() ) {
           daAddr = adv->da_url.getHostAddr();
           sendDAUpdateToRegs( daAddr );
       }
     }

     if(  adv->aa_url.getHostAddr() == aaAddr ) {
       // No aa-updates needed
       return false;
     }

     // otherwise this is a new AA
     aaAddr = adv->aa_url.getHostAddr();

     if (adv->AAPrefix.getString (newAAPrefix))
       constructAddr();

     checkAddrChange();

     // send info to vua also

     sendMovementToRegs();

     sendAAUpdateToRegs( aaAddr );

     return false;
}

bool VAEntity::aapresent_daadvert(Message *msg)
{
    _DOORS_DEBUG(0, "VAEntity::allstates_daadvert");
    SLPPeer::DAADVERT *adv = (SLPPeer::DAADVERT*)msg;
     TestLogger *log = TestLogger :: Instance();
     log->LogEvent( "VAEntity: daadvert received" );

    isDAPresentflag = true;
    state = VAEntitySM :: BothPresent;
    sinceLastDAAdvert = 0;

    if( daAddr == adv->url.getHostAddr() ) {
        return false;
    }

    daAddr = adv->url.getHostAddr();
    _DOORS_DEBUG(0, "DA address now " << daAddr.getHostAddr());

    sendDAUpdateToRegs( daAddr );

    return false;
}

bool VAEntity::dapresent_daadvert(Message *msg)
{
    _DOORS_DEBUG(0, "VAEntity::allstates_DaAdvert");
    SLPPeer::DAADVERT *adv = (SLPPeer::DAADVERT*)msg;
     TestLogger *log = TestLogger :: Instance();
     log->LogEvent( "VAEntity: daadvert received" );

    sinceLastDAAdvert = 0;

    if( daAddr == adv->url.getHostAddr() ) {
        _DOORS_DEBUG(0, "Da advert refresh received");
        return false;
    }

    daAddr = adv->url.getHostAddr();
    _DOORS_DEBUG(0, "New DA found. DA address now " << daAddr.getHostAddr());

    sendDAUpdateToRegs( daAddr );

    return false;
}

bool VAEntity::dapresent_aaadvert(Message *msg)
{
    _DOORS_DEBUG(0, "VAEntity::allstates_DaAdvert");
    SLPPeer::AAADVERT *adv = (SLPPeer::AAADVERT*)msg;
     TestLogger *log = TestLogger :: Instance();
     log->LogEvent( "VAEntity: aaadvert received" );

    sinceLastAAAdvert = 0;
    isAAPresentflag = true;

    aaAddr = adv->aa_url.getHostAddr();
    if (adv->AAPrefix.getString (newAAPrefix)) {
      constructAddr();
      checkAddrChange();
    }

    if( !adv->da_url.isEmpty() && adv->da_url.getHostAddr() == daAddr ) {
        // AA started in our network, no movement
        sinceLastDAAdvert = 0;
        state = VAEntitySM :: BothPresent;
        sendAAUpdateToRegs( aaAddr );
        return false;
    }


    if( adv->da_url.isEmpty() || adv->da_url.getHostAddr() != daAddr ) {

        sendAAUpdateToRegs( aaAddr );

        state = VAEntitySM :: AAPresent;

        if( !adv->da_url.isEmpty() ) {
            daAddr = adv->da_url.getHostAddr();
            sendDAUpdateToRegs( daAddr );
            state = VAEntitySM :: BothPresent;
        }

    }

    return false;
}

bool VAEntity::bothpresent_daadvert(Message *msg)
{
    _DOORS_DEBUG(0, "VAEntity::bothpresent_daadvert");
    SLPPeer::DAADVERT *adv = (SLPPeer::DAADVERT*)msg;
     TestLogger *log = TestLogger :: Instance();
     log->LogEvent( "VAEntity: daadvert received" );

    sinceLastDAAdvert = 0;

    if( daAddr == adv->url.getHostAddr() ) {
        // Nothing new here
        _DOORS_DEBUG(0, "DA advert refresh");
        return false;
    }

    daAddr = adv->url.getHostAddr();
    sendDAUpdateToRegs( daAddr );

    _DOORS_DEBUG(0, "New DA. Address now " << daAddr.getHostAddr());

    return false;
}

bool VAEntity::bothpresent_aaadvert(Message *msg)
{
    _DOORS_DEBUG(0, "VAEntity::bothpresent_aaadvert");
     SLPPeer::AAADVERT *adv = (SLPPeer::AAADVERT*)msg;
     TestLogger *log = TestLogger :: Instance();
     log->LogEvent( "VAEntity: aaadvert received" );

     _DOORS_DEBUG(0, "AA Advertisement received. AA at "<<adv->aa_url.getHostAddr().getHostAddr());

     sinceLastAAAdvert = 0;

     if( aaAddr != adv->aa_url.getHostAddr() ) {
         aaAddr = adv->aa_url.getHostAddr();
         state = VAEntitySM :: AAPresent;
         isDAPresentflag = false;
	 if (adv->AAPrefix.getString (newAAPrefix)) {
	   constructAddr();
	   checkAddrChange();
	 }
	 
         sendMovementToRegs();
         sendAAUpdateToRegs( aaAddr );
     }

     if( !adv->da_url.isEmpty() ) {
         sinceLastDAAdvert = 0;
         isDAPresentflag = true;
         state = VAEntitySM :: BothPresent;

         if( adv->da_url.getHostAddr() != daAddr ) {
             daAddr = adv->da_url.getHostAddr();
             sendDAUpdateToRegs( daAddr );
         }
     }

     return false;
}

bool VAEntity::alone_daadvert(Message *msg)
{
    _DOORS_DEBUG(0, "VAEntity::alone_DaAdvert");
     TestLogger *log = TestLogger :: Instance();
     log->LogEvent( "VAEntity: daadvert received" );

    state = VAEntitySM :: DAPresent;
    sinceLastDAAdvert = 0;
    daSolXid = 0;
    isDAPresentflag = true;

    daAddr = ((SLPPeer::DAADVERT*)msg)->url.getHostAddr();

    _DOORS_DEBUG(0, "DA now at " << daAddr.getHostAddr());

    sendDAUpdateToRegs( daAddr );

    normal_timer.start();

    return false;
}

bool VAEntity::alone_aaadvert(Message *msg)
{
    _DOORS_DEBUG(0, "VAEntity::alone_aaadvert");
     SLPPeer::AAADVERT *adv = (SLPPeer::AAADVERT*)msg;
     TestLogger *log = TestLogger :: Instance();
     log->LogEvent( "VAEntiy: aaadvert received" );

     isAAPresentflag = true;
     state = VAEntitySM :: AAPresent;
     sinceLastAAAdvert = 0;
     aaSolXid = 0;

     aaAddr = adv->aa_url.getHostAddr();
     if (adv->AAPrefix.getString (newAAPrefix)) {
       constructAddr();
       checkAddrChange();
     }

     _DOORS_DEBUG(0, "AA URL is now " << adv->aa_url.getURL());
     _DOORS_DEBUG(0, "AA Advertisement received from " << aaAddr.getHostAddr() );
     if( !adv->da_url.isEmpty() ) {
         state = VAEntitySM :: BothPresent;
         _DOORS_DEBUG(0, "DA is present at addr " << adv->da_url.getURL());
         daAddr = adv->da_url.getHostAddr();
         isDAPresentflag = true;

         sendDAUpdateToRegs( daAddr );

     }

     sendAAUpdateToRegs( aaAddr );

     return false;
}

bool VAEntity::closed_timeout(Message *)
{
    _DOORS_DEBUG(0, "VAEntity::closed_timeout");

    join();
    inNewNetwork = false;
    state = VAEntitySM :: Alone;
    configretry_timer.start();
    return false;
}

bool VAEntity::alone_timeout(Message *)
{
    _DOORS_DEBUG(0, "VAEntity::alone_timeout");
    timeUsed += configretry_timer.getInitial().getSeconds();

    if( inNewNetwork ) {

        join();
        inNewNetwork = false;
    }

    if( timeUsed < maxTimeToSolicit ) {
        sendAASolicitation();
        sendDASolicitation();

        configretry_timer.start();
    }

    return false;
}

bool VAEntity::aapresent_timeout(Message *)
{
    _DOORS_DEBUG(0, "VAEntity::aapresent_timeout");

    /* commenting out as this hasn't been debugged yet
    sinceLastAAAdvert += timer.getInitial().getSeconds();

    if( isDAPresentflag ) {
        _DOORS_DEBUG(0, "ASSERT: State Machine not working properly!!! aapresent_timer() && DAPresentflag");
    }

    if(sinceLastAAAdvert > AAAdvertTimeoutMax) {
        isAAPresentflag = false;
        state = VAEntitySM :: Alone;
        timeUsed = 0;
        timer.setInitial(onesecond_timeout);
        timer.start();
    }
    */
    return false;
}

bool VAEntity::dapresent_timeout(Message *)
{
    _DOORS_DEBUG(0, "VAEntity::dapresent_timeout");
    /* commenting out as this hasn't been debugged yet
    sinceLastDAAdvert += timer.getInitial().getSeconds();

     if(sinceLastDAAdvert > DAAdvertTimeoutMax) {
          if( isAAPresentflag ) {
              state = VAEntitySM :: AAPresent;
          } else {
              state = VAEntitySM :: Alone;
          }

          isDAPresentflag = false;
          timeUsed = 0;
          timer.setInitial(onesecond_timeout);
          timer.start();
     }

     timer.start();
    */
     return false;
}

bool VAEntity::bothpresent_timeout(Message *)
{
    _DOORS_DEBUG(0, "VAEntity::bothpresent_timeout");
    /* commenting out as this hasn't been debugged yet
    sinceLastDAAdvert += timer.getInitial().getSeconds();
    sinceLastAAAdvert += timer.getInitial().getSeconds();

     if(sinceLastDAAdvert > DAAdvertTimeoutMax) {
         state = VAEntitySM :: AAPresent;

         isDAPresentflag = false;
         timeUsed = 0;
         timer.start();
         sendDASolicitation();
     }

     if(sinceLastAAAdvert > AAAdvertTimeoutMax) {

         if( state == VAEntitySM :: BothPresent) {
             state = VAEntitySM :: DAPresent;
             sendAASolicitation();
         } else {
             state = VAEntitySM :: Alone;
             sendAASolicitation();
         }

         isAAPresentflag = false;
         timeUsed = 0;
         timer.setInitial(onesecond_timeout);
         timer.start();
     }

    */
     return false;
}

bool VAEntity::allstates_SLPOpen(Message *msg)
{
     VASAP :: SLPOpen *open = (VASAP :: SLPOpen*) msg;
     VASAP :: SLPOpenInd* ind = (VASAP :: SLPOpenInd*)up.create(VASAP::slpopenind);

     ind->SLPHandle = getNewHandle();

     std::map<Uint32,std::string>::value_type newHand(ind->SLPHandle,open->langTag);
     openHandles.insert(newHand);

     up.putMessage(ind);

     return false;
}

bool VAEntity::allstates_SLPClose(Message *msg)
{


    _DOORS_DEBUG(0, "VAEntity::allstates_SLPClose");
    VASAP :: SLPClose *closemsg = (VASAP :: SLPClose*)msg;

    if( openHandles.erase(closemsg->SLPHandle) == false ) {
        _DOORS_ERROR(0, "A non-existing SLP handle closed!");
    } else {
        _DOORS_DEBUG(0, "SLP handle " << closemsg->SLPHandle << " closed.");
    }


    bool rc = true;
    msg->lock();

    if (vuaConns.putMessage (msg) == false) {
        msg->unlock ();
        rc = false;
    }

    return rc;
}

bool VAEntity::allstates_SrvRply(Message *msg)
{
  TestLogger *log = TestLogger :: Instance();
  log->LogEvent( "VAEntity: service reply received" );

    bool rc = true;
    msg->lock();

    if (vuaConns.putMessage (msg) == false) {
        msg->unlock ();
        rc = false;
        _DOORS_DEBUG(0, "vuaConns_ rejected SRVRPLY [" << msg << "]");
    }

    return rc;
}

bool VAEntity::allstates_srvack(Message *msg)
{
    _DOORS_DEBUG(0, "VAEntity::allstates_srvack");
  TestLogger *log = TestLogger :: Instance();
  log->LogEvent( "VAEntity: service ack received" );
    bool rc = true;
    msg->lock();

    if (serviceRegs.putMessage (msg) == false) {
        msg->unlock ();
        rc = false;
        _DOORS_DEBUG(0, "serviceRegs rejected SRVACK [" << msg << "]");
        _DOORS_DEBUG(0, "Trying to pass ACK to freeAgents");

        Uint16 ack_xid = ((SLPPeer::SRVACK*)msg)->XID;
        for( unsigned int i = 0  ; i < freeAgents.size(); i++ ) {
            if( freeAgents[i]->usingXID(ack_xid) ) {
                _DOORS_DEBUG(0, "Freeagent accepted XID, success!!");
                freeAgents[i]->servicereg.getMessage(msg);
                rc = true;
                break;
            }
        }
    }

    return rc;
}

bool VAEntity :: allstates_SrvtypeRqst( Message *msg )
{
    TestLogger *log = TestLogger :: Instance();
    log->LogEvent( "VAEntity: service-type request received" );
    SLPPeer::SRVTYPERQST *rqst = (SLPPeer::SRVTYPERQST*)msg;

    if( serviceregs.size() == 0 )
        return false;

    if( rqst->PRList.count(currAddr) == 1 )
        return false;

    SLPPeer::SRVTYPERPLY *reply = (SLPPeer::SRVTYPERPLY*)
        peer.create(SLPPeer::srvtyperply);

    reply->dest = rqst->source;
    reply->XID = rqst->XID;
    reply->errorCode = 0;

    unsigned int dgramsize = 24;
    unsigned int srvcount = 0;

    while( srvcount < serviceregs.size() && dgramsize < 1400 ) {
        std::string nextstype = serviceregs[srvcount]->getServiceType();
        dgramsize += nextstype.size()+1; // including comma: "stype1,stype2.."
        if( dgramsize < 1400 )
            reply->typeList.push_back(nextstype);

        srvcount++;
    }

    peer.putMessage(reply);
    log->LogEvent( " - Sent service-type reply" );

    return false;
}

bool VAEntity::allstates_PeerRqst(Message *msg)
{
    _DOORS_DEBUG(0, "VAEntity::allstates_PeerRqst. Request from "
                 << ((SLPHeader*)msg)->source.getHostAddr());

    bool rc = true;
    msg->lock();
    SLPHeader *head = (SLPHeader*)msg;

    if( head->source.getHostAddr() == currAddr.getHostAddr()
        || head->source.getHostAddr() == homeAddr.getHostAddr() ) {
      _DOORS_DEBUG(0, "Received own request, rejecting.");
      return false;
    }

    if( msg->getType() == SLPPeer :: srvrqst ) {

        SLPPeer :: SRVRQST *req = (SLPPeer :: SRVRQST*)msg;

        if( req->PRList.size() ) {
            for( AddrVector::iterator i = req->PRList.begin(); i != req->PRList.end(); i++ ) {
                if( (*i).getHostAddr() == currAddr.getHostAddr() ||
                    (*i).getHostAddr() == homeAddr.getHostAddr() ) {
                    return true;
                }
            }
        }

    }

    if( serviceRegs.putMessage(msg) == false ) {
        msg->unlock();
        rc = false;
        _DOORS_DEBUG(0, "serviceRegs rejected " <<
                     (msg->getType() == SLPPeer::srvrqst ? "SRVRQST" :
                      (msg->getType() == SLPPeer::srvtyperqst ? "SRVTYPERQST" :
                       (msg->getType() == SLPPeer::attrrqst ? "ATTRRQST" : "UNKNOWN"))));
    }

    return rc;
}

bool VAEntity::allstates_AttrRply(Message *msg)
{
    bool rc = true;
    msg->lock();

    if (vuaConns.putMessage (msg) == false) {
        msg->unlock ();
        rc = false;
        _DOORS_DEBUG(0, "vuaConns rejected ATTRRPLY [" << msg << "]");
    }

    return rc;
}

bool VAEntity::allstates_SrvtypeRply(Message *msg)
{
    bool rc = true;
    msg->lock();

    _DOORS_DEBUG(0, "VAENTITY: " << ((SLPPeer::SRVTYPERPLY*)msg)->XID);

    if (vuaConns.putMessage (msg) == false) {
        msg->unlock ();
        rc = false;
        _DOORS_DEBUG(0, "vuaConns rejected SRVTYPERPLY [" << msg << "]");
    }

    return rc;
}

bool VAEntity::allstates_Srv_req(Message *msg)
{
    TestLogger *log = TestLogger :: Instance();
    log->LogEvent( "VAEntity: service request initiated" );

    VASAP :: Srv_req *req = (VASAP::Srv_req*)msg;

    if( !checkHandle(req->SLPHandle) ) {
        _DOORS_ERROR(0, "SLP handle not opened!");
        return false;
    }

    msg->lock();

    if (vuaConns.putMessage (msg) == false) {
        createVUAConn( req->SLPHandle );
        vuaConns.putMessage (msg);
    }

    return true;
}

bool VAEntity::allstates_Srvtype_req(Message *msg)
{
    TestLogger *log = TestLogger :: Instance();
    log->LogEvent( "VAEntity: service-type request initiated" );

    VASAP :: Srvtype_req *req = (VASAP::Srvtype_req*)msg;

    if( !checkHandle(req->SLPHandle) ) {
        _DOORS_ERROR(0, "SLP handle not opened!");
        return false;
    }

    msg->lock();

    if (vuaConns.putMessage (msg) == false) {
        createVUAConn( req->SLPHandle );
        vuaConns.putMessage (msg);
    }

    return true;
}

bool VAEntity::allstates_Attr_req(Message *msg)
{
    VASAP :: Attr_req *req = (VASAP::Attr_req*)msg;

    if( !checkHandle(req->SLPHandle) ) {
        _DOORS_ERROR(0, "SLP handle not opened!");
        return false;
    }

    msg->lock();

    if (vuaConns.putMessage (msg) == false) {
        createVUAConn( req->SLPHandle );
        vuaConns.putMessage (msg);
    }

    return true;
}

bool VAEntity::allstates_registerService(Message *msg)
{
    TestLogger *log = TestLogger :: Instance();
    log->LogEvent( "VAEntity: registerService received" );
    msg->lock();

    if (serviceRegs.putMessage (msg) == false) {
        createServiceReg( 1 );
        serviceRegs.putMessage (msg);
    }

    return true;
}

bool VAEntity::allstates_deregisterService(Message *msg)
{
    bool rc = true;
    
    msg->lock();
    if (serviceRegs.putMessage (msg) == false) {
        msg->unlock();
        rc = false;
        _DOORS_DEBUG(0, "serviceRegs rejected deregisterService [" << msg << "]");
    }

    return rc;
}

bool VAEntity::allstates_addAttributes(Message *msg)
{
    bool rc = true;
    
    msg->lock();
    if (serviceRegs.putMessage (msg) == false) {
        msg->unlock();
        rc = false;
        _DOORS_DEBUG(0, "serviceRegs rejected addAttributes [" << msg << "]");
    }

    return rc;
}

bool VAEntity::allstates_deleteAttributes(Message *msg)
{
    bool rc = true;
    
    msg->lock();
    if (serviceRegs.putMessage (msg) == false) {
        msg->unlock();
        rc = false;
        _DOORS_DEBUG(0, "serviceRegs rejected deleteAttributes [" << msg << "]");
    }

    return rc;
}

bool VAEntity::allstates_rebind (Message *msg) {

  /*
  Udp6SAP::Errind *errmsg = (Udp6SAP::Errind *) msg;
  if (errmsg->code != 2) // meaning error was smth else than a closed socket
    return false;

  // socket's closed. Let's try to rebind to current address. Note: How to rejoin mcast groups? 

  Udp6SAP :: Setup *downmsg = (Udp6SAP::Setup *) down.create (Udp6SAP::setup);
  downmsg->ownIp = currAddr;
  downmsg->ownIp.setPort (myconfig->slp_SlpPort);
  down.putMessage (downmsg);

  */
  return false;
}

void VAEntity :: sendDAUpdateToRegs( InetAddr6 &a )
{
    _DOORS_DEBUG(0, "VAEntity :: sendDAUpdateToRegs");

    ServiceRegSAP :: DAUpdate *u =
        (ServiceRegSAP :: DAUpdate*)sreg_sap.create(ServiceRegSAP :: daupdate);

    u->newaddr = a;
    _DOORS_DEBUG(0, "DAUpdate->newaddr: " << u->newaddr.getHostAddr());
    u->setMultipleTargets();
    serviceRegs.putMessage(u);
    _DOORS_DEBUG(0, "DAUpdate sent to serviceRegs");
}

void VAEntity :: sendAAUpdateToRegs( InetAddr6 &a )
{
    _DOORS_DEBUG(0, "VAEntity :: sendAAUpdateToRegs");

    ServiceRegSAP :: AAUpdate *u =
        (ServiceRegSAP :: AAUpdate*)sreg_sap.create(ServiceRegSAP::aaupdate);

    u->newaddr = a;
    _DOORS_DEBUG(0, "AAUpdate->newaddr: " << u->newaddr.getHostAddr());

    u->setMultipleTargets();
    serviceRegs.putMessage(u);
    _DOORS_DEBUG(0, "AAUpdate sent to serviceRegs");
}

void VAEntity :: sendMovementToRegs( )
{
    VASAP :: Movement *m = (VASAP :: Movement*)up.create( VASAP :: movement );

    m->setMultipleTargets();
    m->lock();
    serviceRegs.putMessage(m);
}

void VAEntity :: createVUAConn( Uint32 slphandle )
{
    std::ostringstream cname;
    cname << getName () << ".uconn." << slphandle;
    VUAConn *newConn  = new VUAConn( cname.str(), slphandle, this,  vuaConnSM );
    //DYNAMIC_SYMBOL( newConn, cname.str(), (this) );

    Port *p = vuaConnMux.createProvider( isForVUAConn );
    newConn->entity.connect (p); 
    Port *q = MNMux.createProvider ( isForMovementTask );
    newConn->mdport.connect (q);
}

void VAEntity :: createServiceReg( Uint32 slphandle )
{
    std::ostringstream cname;
    cname << getName () << ".sreg." << slphandle;
    ServiceReg *newConn = new ServiceReg(cname.str(), slphandle, this, serviceRegSM);
    //DYNAMIC_SYMBOL( newConn, cname.str(), (this) );

    Port *p = serviceRegMux.createProvider (isForServiceReg);
    newConn->entity.connect(p);
    Port *q = MNMux.createProvider ( isForMovementTask );
    newConn->mdport.connect (q);
}

void VAEntity::sendAASolicitation(void)
{
     SLPPeer::SRVRQST *solicitation = (SLPPeer::SRVRQST*)peer.create(SLPPeer::srvrqst);
     solicitation->servicetype = "service:access-agent";
     solicitation->dest.set(slpPort_,"ff02::1:259");

     if( aaSolXid == 0 ) {
         aaSolXid = getNewXID();
     }

     solicitation->XID = aaSolXid;

     peer.putMessage(solicitation);
}

void VAEntity::sendDASolicitation(void)
{
     SLPPeer::SRVRQST *solicitation = (SLPPeer::SRVRQST*)peer.create(SLPPeer::srvrqst);
     solicitation->servicetype = "service:directory-agent";
     solicitation->dest.set(slpPort_,"ff02::123");

     if( daSolXid == 0 ) {
         daSolXid = getNewXID();
     }

     solicitation->XID = daSolXid;

     peer.putMessage(solicitation);
}
