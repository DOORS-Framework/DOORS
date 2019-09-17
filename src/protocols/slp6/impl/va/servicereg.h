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

$Log: servicereg.h,v $
Revision 1.2  2007/01/11 11:35:49  bilhanan
devt

Revision 1.1  2006/08/10 15:39:06  bilhanan
initial commit

Revision 1.5  2004/07/12 07:26:00  hartmanj
General development

Revision 1.4  2004/06/17 08:56:30  hartmanj
Development.

Revision 1.3  2004/05/20 07:00:08  hartmanj
Manual fix to last comment

Revision 1.2  2004/05/19 13:52:02  hartmanj
Update

Revision 1.1  2004/05/12 08:33:20  hartmanj
Development


  
*/

#ifndef _SERVICEREG_H_
#define _SERVICEREG_H_

#include <string>
#include <doors/ptb.h>
#include "vasap.h"
#include <doors/slppdu.h>
#include <doors/slpcommon.h>
#include <doors/slptypes.h>
#include <doors/serviceurl.h>
#include "vaconfigurator.h"
#include "agentconn.h"
#include "serviceregsap.h"
#include "movenotifysap.h"

class VAEntity;
class ServiceRegSM;

class ServiceReg : public PTask
{
public:
    ServiceReg(std::string n, Uint32 slphandle, VAEntity *vaEnt, ServiceRegSM *sm);
    ~ServiceReg (void);

    ServiceRegSAP :: Provider entity;
    MoveNotifySAP :: User mdport;
    VASAP :: Provider &up;
    SLPPeer &peer;

    bool isOfType( const std::string &type );

    bool ready_registerservice( Message *msg );
    bool ready_deregisterservice( Message *msg );
    bool Initialise( VASAP :: registerService *RegMsg );

    bool ready_default( Message *msg );
    bool ready_timeout( Message *msg );

    bool ready_daupdate( Message *msg );
    bool ready_aaupdate( Message *msg );

    bool ready_deregister(Message *msg);
    bool ready_addattributes(Message *msg);
    bool ready_deleteattributes(Message *msg);
    bool ready_srvrqst(Message *msg);
    bool ready_srvack(Message *msg);
    bool ready_attrrqst(Message *msg);
    bool ready_movement(Message *msg);

    Uint32 getSlpHandle();
    ServiceURL getURL();
    std::string getServiceType();
    bool usingXID( Uint16 xid );

    bool isFree() const;

    InetAddr6 getAAAddr();
    InetAddr6 getDAAddr();
    
private:

    void createRegistration( void );

    ServiceURL url;
    Uint32 slpHandle;
    Sint32 lifetime;
    bool local_only;

    AttrVector attrs;
    SLPCommon::AttributeV attributes;

    VAEntity *vaEntity;

    SLPPeer::SRVREG  *registration;// servregistration for resend
    VAConfigurator *myconfig;

    std::string myScope;

    Uint32 taskid;
    AgentConn *aaconn;
    AgentConn *daconn;

    Port aaport;
    Port daport;
};

#endif

