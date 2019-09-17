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

$Log: agentconn.h,v $
Revision 1.3  2007/01/15 11:03:19  bilhanan
devt

Revision 1.2  2007/01/11 11:35:48  bilhanan
devt

Revision 1.1  2006/08/10 15:39:06  bilhanan
initial commit

Revision 1.5  2004/07/16 09:04:28  hartmanj
A bugfix relating to releasing agent when ServiceReg is deleted (must unregister before releasing agent)

Revision 1.4  2004/06/17 08:56:30  hartmanj
Development.

Revision 1.3  2004/05/20 07:00:08  hartmanj
Manual fix to last comment

Revision 1.2  2004/05/19 13:52:01  hartmanj
Update

Revision 1.1  2004/05/12 08:33:20  hartmanj
Development

  
*/

#ifndef _AGENTCONN_H_
#define _AGENTCONN_H_

#include <string>
#include <doors/ptb.h>
#include "vasap.h"
#include <doors/slppdu.h>
#include <doors/slpcommon.h>
#include <doors/slptypes.h>
#include <doors/serviceurl.h>
#include "vaconfigurator.h"
#include "serviceregsap.h"


class AgentConnSM;
class VAEntity;

class AgentConn : public PTask
{
public:
    AgentConn(std::string n, VAEntity *vaEntity,  AgentConnSM *sm );
    ~AgentConn (void);

    ServiceRegSAP :: Provider servicereg;
    VASAP :: Provider &up;
    SLPPeer &peer;


    bool idle_default( Message * );
    bool idle_agentupdate( Message * );
    bool idle_timeout( Message * );

    bool wfregack_default (Message *);
    bool wfregack_srvack (Message * );
    bool wfunregack_srvack (Message * );
    bool regdone_default (Message * );
    bool wfunregack_default (Message * );
    bool regdone_timeout (Message * );
    bool wfack_timeout (Message * );
    bool regdone_agentupdate (Message * );
    bool wfregack_agentupdate (Message * );
    bool wfunregack_agentupdate (Message * );
    bool regdone_release (Message * );
    bool wfregack_release (Message * );
    bool idle_release (Message * );
    bool wfunregack_release (Message * );

    //movement

    bool regdone_movement (Message * );
    bool wfregack_movement (Message * );
    bool wfunregack_movement (Message * );


    bool usingXID( Uint16 xid );
    void Initialise( SLPPeer::SRVREG  **reg, Uint32 lifet );
    void setAgentHandleAA(void) { agentIsAA=true; }
    void setAgentHandleDA(void) { agentIsAA=false; }

private:

    // Sets timeout to be 75% url:s lifetime
    void setNormalTimeout();
    // Sets timeout to fire in retransmission freq
    void setRetransTimeout();

    void registerWithAgent( );
    void unregisterWithAgent( );

    VAEntity *vaEntity;
    Timer timer;
    Uint32 lifetime;

    SLPPeer::SRVREG  **registration; // servregistration for resend

    InetAddr6 agentAddress;
    InetAddr6 nextAgentAddress;
    bool nextAgentWaiting;
    bool agentIsAA;
    bool releaseWhenIdle;

    VAConfigurator *myconfig;

    Uint32 RegResendInterval;
    Uint32 SinceLastReg;
    Sint32 timeUsed;

    Uint32 myXID;

};

#endif

