//  Copyright 2004
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

$Log: agentconn.C,v $
Revision 1.3  2007/01/15 11:03:19  bilhanan
devt

Revision 1.2  2007/01/11 11:35:48  bilhanan
devt

Revision 1.1  2006/08/10 15:39:06  bilhanan
initial commit

Revision 1.6  2004/07/16 09:04:28  hartmanj
A bugfix relating to releasing agent when ServiceReg is deleted (must unregister before releasing agent)

Revision 1.5  2004/07/12 07:26:00  hartmanj
General development

Revision 1.4  2004/06/17 08:56:29  hartmanj
Development.

Revision 1.3  2004/05/20 07:00:08  hartmanj
Manual fix to last comment

Revision 1.2  2004/05/19 13:52:01  hartmanj
Update

Revision 1.1  2004/05/12 08:33:20  hartmanj
Development


  
*/

#include <string>
#include <doors/slptypes.h>
#include "agentconn.h"
#include "agentconnsm.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <doors/debug.h>
#include "vaent.h"
#include "vasap.h"
#include "test_logger.h"



AgentConn :: AgentConn( std::string n, VAEntity *vaEntity,  AgentConnSM *sm )
     :  PTask (n, vaEntity->getScheduler (), sm), 
        servicereg( this ),
        up( vaEntity->up ),
        peer( vaEntity->peer ),
        vaEntity( vaEntity ),
        timer(this, OTime(3)),
        registration(0),
        myconfig( VAConfigurator::Instance() ),
        RegResendInterval(3),
        SinceLastReg(RegResendInterval+1),
        timeUsed(0),
        myXID(0),
        agentAddress(0,"::"),
        nextAgentAddress(0,"::"),
        nextAgentWaiting(false),
        agentIsAA(false),
        releaseWhenIdle(false)
{
     _DOORS_DEBUG(0,"AgentConn :: AgentConn: "<< n );
     TestLogger *log = TestLogger :: Instance();
     log->LogEvent( "AgentConn created: " + getName() );

     STATIC_SYMBOL( RegResendInterval, this );
     STATIC_SYMBOL( myXID, this );
     state = AgentConnSM :: Idle;
}

bool AgentConn :: idle_default( Message * )
{
    _DOORS_DEBUG(0, "AgentConn :: idle_default");
    return false;
}

bool AgentConn :: idle_timeout( Message * )
{
     _DOORS_DEBUG(0, "AgentConn::idle_timeout" );
     return false;
}

void AgentConn :: Initialise( SLPPeer :: SRVREG **reg, Uint32 lifet )
{
    _DOORS_DEBUG(0, "AgentConn :: Initialise");

    registration = new (SLPPeer :: SRVREG*);
    *registration  = *reg;
    lifetime = lifet;

    RegResendInterval = (Uint32)(lifet * .75);

    if( RegResendInterval < 1 )
        RegResendInterval = 1;

    _DOORS_DEBUG(0, "AgentConn re-registration interval set to " << RegResendInterval);

    if( state != AgentConnSM :: Idle ) {
        _DOORS_ERROR(0, "ASSERT failed, AgentConn not Idle");
    }

    state = AgentConnSM :: Idle;
}

bool AgentConn :: idle_agentupdate( Message *msg )
{
    _DOORS_DEBUG(0, "AgentConn :: idle_agentupdate");

    if( !*registration )
        _DOORS_ERROR(0, "ASSERT no registration saved and agentupdate received!");

    InetAddr6 newaddresstmp(0,"::d:0:0:2:5");

    if( msg->getType() == ServiceRegSAP :: aaupdate ) {
        newaddresstmp = ((ServiceRegSAP :: AAUpdate *)msg)->newaddr;
        _DOORS_DEBUG(0, "AAUpdate received [" << name << "]");
    } else if( msg->getType() == ServiceRegSAP :: daupdate ) {
        newaddresstmp = ((ServiceRegSAP :: DAUpdate *)msg)->newaddr;
    } else {
        _DOORS_DEBUG(0, "ASSERT failed, not AAUpdate nor DAUpdate!");
    }

    if( newaddresstmp.getPortNumber() == 0 )
        _DOORS_DEBUG(0, "ASSERT failed, agent address port is 0");

    _DOORS_DEBUG(0, "Agent at " << newaddresstmp.getHostAddr());

    agentAddress = newaddresstmp;
    myXID = vaEntity->getNewXID();
    registerWithAgent();
    timeUsed = 0;

    state = AgentConnSM :: wfRegAck;
    setRetransTimeout();
    timer.start();

    return false;
}

AgentConn :: ~AgentConn (void)
{
    if( registration )
        delete registration;

    DELETE_SYMBOL (this);   
}

bool AgentConn :: usingXID( Uint16 xid )
{
    _DOORS_DEBUG(0, "AgentConn :: usingXID -- myxid == " << myXID);
    if( myXID == xid )
        return true;

    return false;
}

bool AgentConn :: wfregack_default (Message *)
{   
    _DOORS_DEBUG(0, "AgentConn :: wfregack_default");
    return false;
}

bool AgentConn :: wfregack_srvack (Message *msg)
{
    _DOORS_DEBUG(0,"AgentConn :: wfregack_SrvAck");
    TestLogger *log = TestLogger :: Instance();
    log->LogEvent( "Srvack received in WfRegAck in AgentConn " + getName() );

    SLPPeer :: SRVACK *ack = (SLPPeer :: SRVACK *) msg;

     if( ack->XID== myXID ) {

         if( agentAddress != ack->source ) {
             _DOORS_DEBUG(0, "ASSERT, Received SRVACK from wrong agent with my XID!");
             return false;
         }

         _DOORS_DEBUG(0,"Registration done, moving to RegDone state.");
         myXID = 0;
         SinceLastReg = 0;
         state = AgentConnSM :: RegDone;
         setNormalTimeout();

     }

     timer.start();

     return false;
}



bool AgentConn :: wfunregack_srvack( Message *msg )
{
    _DOORS_DEBUG(0, "AgentConn :: wfunregack_srvack");
    TestLogger *log = TestLogger :: Instance();
    log->LogEvent( "Srvack received in WfUnregAck in AgentConn " + getName() );

    SLPPeer :: SRVACK *ack =  (SLPPeer :: SRVACK *) msg;

     if( ack->XID== myXID ) {

         if( agentAddress != ack->source ) {
             _DOORS_DEBUG(0, "ASSERT, Received SRVACK from wrong agent with my XID!");
             return false;
         }

         if( releaseWhenIdle ) {
             if( registration )
                 delete registration;
             registration = 0;
             myXID = 0;
             vaEntity->releaseAgent(this);
             releaseWhenIdle = false;
             nextAgentAddress.set(0, "::d:0:0:2:5");
             nextAgentWaiting = false;
             timeUsed = 0;
             state = AgentConnSM :: Idle;
             setNormalTimeout();
             return false;
         }

         if( nextAgentWaiting ) {
             if( nextAgentAddress == agentAddress ) {
                 _DOORS_DEBUG(0, "ASSERT failed, nextAgentAddress == agentAddress");
             }

             // We will immediately register with another agent
             agentAddress = nextAgentAddress;
             nextAgentAddress.set(0, "::d:0:0:2:5");
             nextAgentWaiting = false;
             myXID = vaEntity->getNewXID();
             timeUsed = 0;
             registerWithAgent();
             setRetransTimeout();
             timer.start();
             state = AgentConnSM :: wfRegAck;
             return false;
         }

         _DOORS_DEBUG(0,"Registration done, moving to RegDone state");
         myXID = 0;
         SinceLastReg = 0;
         state = AgentConnSM :: Idle;
         setNormalTimeout();

     }

     timer.start();
    
    return false;
}

bool AgentConn :: regdone_movement( Message *msg )
{
    _DOORS_DEBUG(0, "AgentConn :: regdone_movement");
    TestLogger *log = TestLogger :: Instance();
    log->LogEvent( "Movement received in AgentConn " + getName() );
    msg->unlock();

    myXID = vaEntity->getNewXID();
    unregisterWithAgent();

    state = AgentConnSM :: wfUnregAck;
    setRetransTimeout();
    timer.start();

    return false;
}

bool AgentConn :: wfregack_movement( Message *msg )
{
    _DOORS_DEBUG(0, "AgentConn :: wfregack_Movement");
    msg->unlock();

    myXID = vaEntity->getNewXID();
    unregisterWithAgent();

    state = AgentConnSM :: wfUnregAck;
    setRetransTimeout();
    timer.start();

    return false;
}

bool AgentConn :: wfunregack_movement( Message *msg )
{
    _DOORS_DEBUG(0, "AgentConn :: wfUnregAck_Movement");
    msg->unlock();

    return false;
}

bool AgentConn :: regdone_default (Message *)
{
    _DOORS_DEBUG(0, "AgentConn :: regdone_default");
    return false;
}

bool AgentConn :: wfunregack_default (Message *)
{
    _DOORS_DEBUG(0, "AgentConn :: wfUnregAck_default");
    return false;
}

bool AgentConn :: regdone_timeout( Message * )
{
    _DOORS_DEBUG(0, "AgentConn :: regdone_timeout");

    myXID = vaEntity->getNewXID();
    registerWithAgent();
    timeUsed = 0;
    state = AgentConnSM :: wfRegAck;
    setRetransTimeout();

    timer.start();
    return false;
}

bool AgentConn :: wfack_timeout (Message *)
{
    _DOORS_DEBUG(0, "AgentConn :: wfAck_timeout, state is " << sm->stateName(this));

    timeUsed += myconfig->CONFIG_RETRY;

    if( timeUsed > myconfig->CONFIG_RETRY_MAX ) {

        timeUsed = 0;
        SinceLastReg = 0;
        myXID = 0;

        if( releaseWhenIdle ) {
             if( registration )
                 delete registration;
             registration = 0;
             myXID = 0;
             vaEntity->releaseAgent(this);
             releaseWhenIdle = false;
             nextAgentAddress.set(0, "::d:0:0:2:5");
             nextAgentWaiting = false;
             timeUsed = 0;
             state = AgentConnSM :: Idle;
             setNormalTimeout();
             return false;
         }

        if( state == AgentConnSM :: wfUnregAck && nextAgentWaiting ) {
            _DOORS_DEBUG(0, "Unreg timed out, starting registration with next agent "
                         << nextAgentAddress.getHostAddr() );
            myXID = vaEntity->getNewXID();
            agentAddress = nextAgentAddress;
            nextAgentAddress.set(0, "::d:0:0:2:5");
            nextAgentWaiting = false;
            registerWithAgent();
            state = AgentConnSM :: wfRegAck;
            timer.start();
            return false;
        }

        setNormalTimeout();
        timer.stop();
        state = AgentConnSM :: Idle;

        return false;
    }

    if( state == AgentConnSM :: wfRegAck ) {
        registerWithAgent();
    } else {
        unregisterWithAgent();
    }

    timer.start();
    return false;
}

void AgentConn :: setRetransTimeout()
{
    OTime t( myconfig->CONFIG_RETRY );
    timer.setInitial(t);
}

void AgentConn :: setNormalTimeout( )
{
    OTime t( RegResendInterval );
    timer.setInitial(t);
}

void AgentConn::registerWithAgent( )
{
     SLPPeer :: SRVREG *copy = (SLPPeer::SRVREG*)((*registration)->clone());

     copy->dest = agentAddress;
     copy->XID = myXID;
     _DOORS_DEBUG(0, "Registering with agent " << agentAddress.getHostAddr()
                  << " with XID " << myXID);

     peer.putMessage(copy);
}

void AgentConn::unregisterWithAgent( )
{
     SLPPeer :: SRVDEREG *dereg = (SLPPeer :: SRVDEREG *)peer.create(SLPPeer::srvdereg);

     dereg->dest = agentAddress;
     dereg->XID = myXID;
     _DOORS_DEBUG(0, "DEBUG: " << (*registration)->url.getURL());
     dereg->url = (*registration)->url;
     _DOORS_DEBUG(0, "Unregistering with agent " << agentAddress.getHostAddr()
                  << " with XID " << myXID);

     dereg->scopeList = (*registration)->scopeList;
     // TODO: taglist?

     peer.putMessage(dereg);
}

bool AgentConn :: regdone_agentupdate(Message *msg)
{
    _DOORS_DEBUG(0, "AgentConn :: regdone_agentupdate " <<msg->getType() << " " << msg);

    _DOORS_DEBUG(0, "ServiceRegSAP :: daupdate " << ServiceRegSAP :: daupdate);
    if( (agentIsAA && msg->getType() == ServiceRegSAP :: daupdate)
        || (!agentIsAA && msg->getType() == ServiceRegSAP :: aaupdate) ) {
        msg->unlock();
        return false;
    }

    InetAddr6 newaddresstmp(0,"::d:0:0:2:5");

    if( msg->getType() == ServiceRegSAP :: aaupdate ) {
        newaddresstmp = ((ServiceRegSAP :: AAUpdate *)msg)->newaddr;
        _DOORS_DEBUG(0, "AAUpdate received [" << name << "]");
    } else if( msg->getType() == ServiceRegSAP :: daupdate ) {
        newaddresstmp = ((ServiceRegSAP :: DAUpdate *)msg)->newaddr;
    } else {
        _DOORS_DEBUG(0, "ASSERT failed, not AAUpdate nor DAUpdate!");
    }

    if( newaddresstmp.getPortNumber() == 0 )
        _DOORS_DEBUG(0, "ASSERT failed, agent address port is 0");

    _DOORS_DEBUG(0, "Agent at " << newaddresstmp.getHostAddr());

    if( agentAddress != newaddresstmp ) {
        nextAgentAddress = newaddresstmp;
        nextAgentWaiting = true;

        myXID = vaEntity->getNewXID();
        unregisterWithAgent();

        state = AgentConnSM :: wfUnregAck;
        timeUsed = 0;
        setRetransTimeout();
        timer.start();
    }

    return false;
}

bool AgentConn :: wfregack_agentupdate(Message *msg)
{
    _DOORS_DEBUG(0, "AgentConn :: wfregack_agentupdate: " << msg->getType());
    if( (agentIsAA && msg->getType() == ServiceRegSAP :: daupdate)
        || (!agentIsAA && msg->getType() == ServiceRegSAP :: aaupdate) ) {
        msg->unlock();
        return false;
    }

    InetAddr6 newaddresstmp(0,"::d:0:0:2:5");

    if( msg->getType() == ServiceRegSAP :: aaupdate ) {
        newaddresstmp = ((ServiceRegSAP :: AAUpdate *)msg)->newaddr;
        _DOORS_DEBUG(0, "AAUpdate received [" << name << "]");
    } else if( msg->getType() == ServiceRegSAP :: daupdate ) {
        newaddresstmp = ((ServiceRegSAP :: DAUpdate *)msg)->newaddr;
        _DOORS_DEBUG(0, "DAUpdate received [" << name << "]");
    } else {
        _DOORS_DEBUG(0, "ASSERT failed, not AAUpdate nor DAUpdate!");
    }

    if( newaddresstmp.getPortNumber() == 0 )
        _DOORS_DEBUG(0, "ASSERT failed, agent address port is 0");

    _DOORS_DEBUG(0, "Agent at " << newaddresstmp.getHostAddr());

    if( agentAddress != newaddresstmp ) {
        nextAgentAddress = newaddresstmp;
        nextAgentWaiting = true;

        myXID = vaEntity->getNewXID();
        unregisterWithAgent();

        state = AgentConnSM :: wfUnregAck;
        timeUsed = 0;
        setRetransTimeout();
        timer.start();
    }

    return false;
}

bool AgentConn :: wfunregack_agentupdate(Message *msg)
{
    _DOORS_DEBUG(0, "AgentConn :: wfunregack_agentupdate");

    if( (agentIsAA && msg->getType() == ServiceRegSAP :: daupdate)
        || (!agentIsAA && msg->getType() == ServiceRegSAP :: aaupdate) ) {
        msg->unlock();
        return false;
    }

    InetAddr6 newaddresstmp(0,"::d:0:0:2:5");

    if( msg->getType() == ServiceRegSAP :: aaupdate ) {
        newaddresstmp = ((ServiceRegSAP :: AAUpdate *)msg)->newaddr;
        _DOORS_DEBUG(0, "AAUpdate received [" << getName() << "]");
    } else if( msg->getType() == ServiceRegSAP :: daupdate ) {
        newaddresstmp = ((ServiceRegSAP :: DAUpdate *)msg)->newaddr;
        _DOORS_DEBUG(0, "DAUpdate received [" << getName() << "]");
    } else {
        _DOORS_DEBUG(0, "ASSERT failed, not AAUpdate nor DAUpdate!");
    }

    if( newaddresstmp.getPortNumber() == 0 )
        _DOORS_DEBUG(0, "ASSERT failed, agent address port is 0");

    _DOORS_DEBUG(0, "Agent at " << newaddresstmp.getHostAddr());

    if( agentAddress != newaddresstmp ) {
        nextAgentAddress = newaddresstmp;
        nextAgentWaiting = true;
    }

    return false;
}

bool AgentConn :: idle_release( Message * ) {
    _DOORS_DEBUG(0, "AgentConn :: idle_release");

    vaEntity->releaseAgent( this );

    if( registration )
        delete registration;

    registration = 0;

    myXID = 0;

    return false;
}

bool AgentConn :: regdone_release( Message * ) {
    _DOORS_DEBUG(0, "AgentConn :: regdone_release");

    myXID = vaEntity->getNewXID();
    unregisterWithAgent();

    state = AgentConnSM :: wfUnregAck;
    setRetransTimeout();
    timer.start();

    releaseWhenIdle = true;

    return false;
}

bool AgentConn :: wfregack_release( Message * ) {
    _DOORS_DEBUG(0, "AgentConn :: wfregack_release");

    releaseWhenIdle = true;

    return false;
}

bool AgentConn :: wfunregack_release( Message * ) {
    _DOORS_DEBUG(0, "AgentConn :: wfunregack_release");

    myXID = vaEntity->getNewXID();
    unregisterWithAgent();

    state = AgentConnSM :: wfUnregAck;
    setRetransTimeout();
    timer.start();

    releaseWhenIdle = true;

    return false;
}
