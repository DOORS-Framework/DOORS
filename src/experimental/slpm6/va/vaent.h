// Editor-info: -*- C++ -*-
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

$Log: vaent.h,v $
Revision 1.6  2004/06/17 08:56:31  hartmanj
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

#ifndef _VAENT_H
#define _VAENT_H

#include <vector>
#include <string>
#include <doors/ptb.h>
#include <doors/udp6task.h>
#include <doors/slppdu.h>
#include <doors/serviceurl.h>
#include <doors/slpcommon.h>
#include "vasap.h"
#include "servicereg.h"
//#include "serviceregsm.h"
//#include "vuaconnsm.h"
#include "agentconn.h"
#include "mobilitysap.h"


class VAEntitySM;
class ServiceRegSM;
class VUAConnSM;
class ServiceReg;
class VUAConn;


class VAEntity : public PTask, public SLPCommon {

public :
    VAEntity (std::string n, Scheduler *s, VAEntitySM *sm, InetAddr6 &addr);


    virtual ~VAEntity();

    bool allstates_SLPOpen( Message * );
    bool allstates_SLPClose( Message * );

    // Peer replies
    bool allstates_SrvRply(Message *msg);
    bool allstates_srvack(Message *msg);
    bool allstates_AttrRply(Message *msg);
    bool allstates_SrvtypeRply(Message *msg);

    // Peer requests
    bool allstates_PeerRqst(Message *msg);
    bool allstates_SrvtypeRqst(Message *msg);

    // Up req
    bool allstates_Srv_req(Message *msg);
    bool allstates_Srvtype_req(Message *msg);
    bool allstates_Attr_req(Message *msg);
    bool allstates_registerService(Message *msg);
    bool allstates_deregisterService(Message *msg);
    bool allstates_addAttributes(Message *msg);
    bool allstates_deleteAttributes(Message *msg);


    // AAPresent
    bool aapresent_default(Message *msg);
    bool aapresent_aaadvert(Message *msg);
    bool aapresent_daadvert(Message *msg);
    bool aapresent_timeout(Message *msg);

    // DAPresent
    bool dapresent_default(Message *msg);
    bool dapresent_aaadvert(Message *msg);
    bool dapresent_daadvert(Message *msg);
    bool dapresent_timeout(Message *msg);

    // No AA | DA
    bool alone_default(Message *msg);
    bool alone_daadvert(Message *msg);
    bool alone_aaadvert(Message *msg);
    bool alone_timeout(Message *msg);

    // Both present
    bool bothpresent_default(Message *msg);
    bool bothpresent_daadvert(Message *msg);
    bool bothpresent_aaadvert(Message *msg);
    bool bothpresent_timeout(Message *msg);
    
    // Movement notification
    bool aapresent_movement(Message *);
    bool dapresent_movement(Message *);
    bool bothpresent_movement(Message *);
    bool alone_movement(Message *);

    // To access information on AAs and DAs the connection tasks contact
    // the entity task. Entity task holds the address and state information.
    bool isDAPresent(void) const;
    bool isAAPresent(void) const;
    InetAddr6 getAAAddr(void) const;
    InetAddr6 getDAAddr(void) const;
    InetAddr6 getDA() const;
    int getIP6Scope(void) const;
    int getSLPPort(void) const;
    InetAddr6 getCOAAddr(void) const;
    bool checkCOAChange(void) const;
     
    Udp6SAP :: User down;
    VASAP :: Provider up;
    SLPPeer peer;
    MobilitySAP :: User mobapi;
    ServiceRegSAP :: User sreg_sap;

    AgentConn* getNewAgentConn();
    void releaseAgent( AgentConn *agent );

private:

    void sendAAUpdateToRegs( InetAddr6 &a );
    void sendDAUpdateToRegs( InetAddr6 &a );
    void sendMovementToRegs( );

    void sendAASolicitation ( void );
    void sendDASolicitation ( void );

    bool checkForMovement(); // mobility check: compare old aa address with received address
    bool checkHandle( Uint32 h ) const;

    void createServiceReg( Uint32 slphandle );
    void createVUAConn( Uint32 slphandle );
    void join( );

    SLPPeer::SRVRQST *solicitation;

    Port vuaConns;
    Port serviceRegs;
    PortMux vuaConnMux;
    PortMux serviceRegMux;

    ServiceRegSM *serviceRegSM;
    AgentConnSM *agentConnSM;
    VUAConnSM *vuaConnSM;
    std::vector<AgentConn*> agentConnList;
    std::vector<AgentConn*> freeAgents;
    std::vector<ServiceReg*> serviceregs;

    bool isDAPresentflag;
    bool isAAPresentflag;

    InetAddr6 currentCOAAddr;
    InetAddr6 homeAddress;
    InetAddr6 aaAddr;
    InetAddr6 daAddr;
    Uint16 aaSolXid;
    Uint16 daSolXid;

    std::map<Uint32,std::string> openHandles;

    VAConfigurator *myconfig;
    int timerInterval;
    int maxTimeToSolicit;
    int timeUsed;
    bool inNewNetwork;
    Timer timer;
    int sinceLastAAAdvert;
    int AAAdvertTimeoutMax;
    int sinceLastDAAdvert;
    int DAAdvertTimeoutMax;

    OTime config_retry;
    OTime onesecond_timeout;
    OTime normal_timeout;

};



#endif //  VAENT_H
