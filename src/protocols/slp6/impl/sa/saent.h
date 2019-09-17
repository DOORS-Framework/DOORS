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

$Log: saent.h,v $
Revision 1.2  2004/04/07 13:43:51  hartmanj
Some fixes

Revision 1.1  2004/04/05 06:55:57  hartmanj
Renamed samain* to saent*

Revision 1.8  2004/03/09 09:54:06  bilhanan
reintroduced DAActiveDiscoveryInterval_, using SAConfigurator, changed
constructor parameter passing to SLPcommon.

Revision 1.7  2004/02/16 13:01:32  bilhanan
join() introduced to handle joinmcgroup for DA multicast group.

Revision 1.6  2004/02/10 18:11:18  bilhanan
joining multicast groups changed
removed dependency to UDP6Task in constructor

Revision 1.5  2003/10/03 14:27:02  bilhanan
tidying up.

Revision 1.4  2003/08/24 22:22:53  bilhanan
reduced number of parent classes, DAFinder now a member. isInPRList absorbed by samain.

Revision 1.3  2003/08/12 00:07:46  bilhanan
const char* in constructor -> std::string

Revision 1.2  2003/06/04 11:09:58  kalliosa
Licence clause added

  
*/

#ifndef SAENT_H
#define SAENT_H

#include <string>
#include <map>
#include <list>
#include <set>
#include <doors/ptb.h>
#include <doors/udp6.h>
#include <doors/udp6task.h>
#include <doors/slptypes.h>
#include <doors/sasap.h>
#include <doors/slppdu.h>
#include <doors/sareg.h>
#include <doors/saregsm.h>
#include <doors/dafinder.h>
#include <doors/slpcommon.h>


class SAReg;
class SAEntSM;
class SARegSM;


class SAEnt : public PTask, public SLPCommon
{
 public:
   SAEnt( std::string n, Scheduler *s, SAEntSM *sm, InetAddr6 ownAddr );
   virtual ~SAEnt (void);

   bool ready_default (Message *msg);
   bool ready_SLPOpen (Message *msg);
   bool ready_SLPClose (Message *msg);
   bool ready_registerService (Message *msg);
   bool ready_deregisterService (Message *msg);
//   bool ready_daAdvert (Message *msg); in dafinder
   bool ready_srvRqst (Message *msg);
   bool ready_attrRqst (Message *msg);
   bool ready_srvTypeRqst (Message *msg);
   bool ready_srvAck (Message *msg); // not necessary?
   virtual bool ready_daAdvert (Message *msg);
   
   
   bool wfAck_Ack (Message *msg);
   bool wfAck_default (Message *msg);
   
   bool ready_addAttrs (Message *msg);
   bool ready_deleteAttrs (Message *msg);
   bool ready_addAttributes (Message *msg);
   bool ready_deleteAttributes (Message *msg);

   void addToRply( ServiceURL s );
   void sendSrvRply( SLPPeer::SRVRQST *rqst );
   bool isInPRList( InetAddr6 &addr, AddrVector &PRList );
   void join (void);
   
   SASAP :: Provider up_;
   Udp6SAP :: User down_;
   SLPPeer peer_;  

   DAFinder dafinder;
   static const InetAddr6 SERVLOCDA;
   static const InetAddr6 SERVLOC;

 private:

   int DAActiveDiscoveryInterval_;
   Timer DATimer_;
   bool isDADetected;
   InetAddr6 ownAddr_;
   SARegSM *csm_;
   Port regs_;                                                                 
   PortMux regMux_;     

   std::set<SAReg*> SARegs_;
   URLVector matchingURLs;
   void createSAReg(  SASAP :: registerService *reg);

};

#endif  // SAMAIN_H

