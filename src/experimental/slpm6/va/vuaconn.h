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

$Log: vuaconn.h,v $
Revision 1.4  2004/07/16 09:13:01  hartmanj
Added a small wait, before auto re-discovery is started. Must discover agents first

Revision 1.3  2004/07/12 07:26:01  hartmanj
General development

Revision 1.2  2004/05/12 08:33:22  hartmanj
Development

Revision 1.1  2004/01/28 09:23:31  hartmanj
Initial revision. Compiles.

  
*/


#ifndef _VUACONN_H_
#define _VUACONN_H_

#include <string>
#include <doors/ptb.h>
#include <doors/slppdu.h>
#include <doors/slptypes.h>
#include <doors/serviceurl.h>
#include "vaent.h"
#include "vaconfigurator.h"

class VUAConnSM;

class VUAConn : public PTask
{
   public:
   
      VUAConn( std::string n, Uint32 slphandle, VAEntity *vaentity,  VUAConnSM *sm ); 
      virtual ~VUAConn (void);
   
      // handler for messages from VAEntity
      bool idle_Srv_req(Message *msg);
      bool idle_Srvtype_req(Message *msg);
      bool idle_Attr_req(Message *msg);

      bool idle_Default(Message *msg);
      bool mcWaitReply_Default(Message *msg);
      bool agentWaitReply_Default(Message *msg);

      // movement
      bool idle_movement( Message *msg );
      bool mcWaitReply_movement( Message *msg );
      bool agentWaitReply_movement( Message *msg );

      // timeouts
      bool mcWaitReply_Timeout(Message *msg);
      bool agentWaitReply_Timeout(Message *msg);

      // peer message handlers
      bool mcWaitReply_SrvRply(Message *msg);
      bool mcWaitReply_SrvtypeRply(Message *msg);
      bool mcWaitReply_AttrRply(Message *msg);
      bool agentWaitReply_SrvRply(Message *msg);
      bool agentWaitReply_SrvtypeRply(Message *msg);
      bool agentWaitReply_AttrRply(Message *msg);

      // a small wait-state after movement, before autodiscovery is started
      bool autoRestartWait_Default( Message * );
      bool autoRestartWait_Timeout( Message * );

      Uint16 getXID ( void );
      Uint32 getptaskid() const { return slpHandle_; };
      
   VASAP :: Provider &up;
   SLPPeer &peer;
   MobilitySAP :: User &mobapi;
   Port entity;
   VAEntity *vaEntity_;
   
 private:

   Timer timer_;
   Uint32 timeUsed_;

   Message *request_;
   SLPPeer::SRVRQST *srequest_;
   SLPPeer::SRVTYPERQST *strequest_;
   SLPPeer::ATTRRQST *arequest_;
   VAConfigurator *myconfig;
      
   Uint32 slpHandle_;
   Uint16 slpPort_;
   Uint16 XID_;

    // new variable: auto-discovery flag
    // auto-discover this type of service upon movement,
    // or auto-discover service-types.
    bool auto_discovery;

    void restartQuery();
};

#endif

