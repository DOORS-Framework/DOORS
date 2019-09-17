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

$Log: uaent.h,v $
Revision 1.2  2004/04/08 10:53:04  hartmanj
Added support for servicetype request (Srvtype_req)

Revision 1.1  2004/04/05 06:58:54  hartmanj
Renamed uamain* to uaent*

Revision 1.11  2004/03/30 06:07:06  hartmanj
Fixes to rid compile errors

Revision 1.10  2004/03/27 12:46:00  bilhanan
rewritten ua functionality

Revision 1.9  2004/03/09 09:22:22  bilhanan
reintroducing DAActiveDiscoveryInterval_, using UAConfigurator, changed
constructor parameter passing to SLPcommon.

REVISION 1.8  2004/02/27 07:46:55  hartmanj
Similar hack introduced as in SAMain to perform joining after initializations. UAMain::joinAndGo()

Revision 1.7  2004/02/10 14:54:05  bilhanan
more bugfixing.

Revision 1.6  2004/02/10 14:43:45  bilhanan
progressive bugfixing

Revision 1.5  2003/08/27 06:36:45  kalliosa
added ready_daAdvert

Revision 1.4  2003/08/24 22:18:18  bilhanan
reduced number of parent classes, DAFinder now a member.

Revision 1.3  2003/08/12 00:10:48  bilhanan
const char* in constructor -> std::string

Revision 1.2  2003/06/04 11:10:01  kalliosa
Licence clause added

  
*/


#ifndef UAENT_H
#define UAENT_H

#include <map>
#include <set>
#include <list>
#include <string>
#include <doors/ptb.h>
#include <doors/udp6.h>
#include <doors/slptypes.h>
#include <doors/uasap.h>
#include <doors/slppdu.h>
#include <doors/uaconnsm.h>
#include <doors/dafinder.h>
#include <doors/slpcommon.h>
#include <doors/uaconfigurator.h>


class UAEntSM;
class UAConnSM;

class UAEnt : public PTask, public SLPCommon {

 public:

   UAEnt( std::string n, Scheduler *s, UAEntSM *sm);
   virtual ~UAEnt (void);

   bool closed_default (Message *msg);
   bool closed_Timer (Message *msg);

   bool open_default (Message *msg);
   bool open_SLPOpen (Message *msg);
   bool open_SLPClose (Message *msg);
   bool open_Srvreq (Message *msg);
   bool open_Srvtypereq (Message *msg);
   bool open_Attrreq (Message *msg);
   bool open_DAAdvert (Message *msg);
   bool open_Timer( Message *msg);

   
   void sendDAInfoRqst();
   
   UASAP :: Provider up;
   Udp6SAP :: User down;

   SLPPeer peer;  

   DAFinder dafinder;

 private:

   void createConnection (Uint32 slphandle);

   UAConfigurator *myconfig;
   Timer DATimer;
   UAConnSM *csm;
   Port connections;
   PortMux connMux;

   Uint16 retrans_XID;
   int mcast_time;
   std::set <InetAddr6> DA_PRList;

};

#endif  // UAENT_H

