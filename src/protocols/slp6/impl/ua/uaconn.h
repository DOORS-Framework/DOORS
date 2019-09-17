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

$Log: uaconn.h,v $
Revision 1.9  2004/04/08 10:53:04  hartmanj
Added support for servicetype request (Srvtype_req)

Revision 1.8  2004/04/05 07:02:16  hartmanj
UAMain class renamed to UAEnt

Revision 1.7  2004/03/31 08:44:01  hartmanj
Bug fixes

Revision 1.6  2004/03/30 08:07:52  hartmanj
Compile fixes and debug couts

Revision 1.5  2004/03/27 12:46:00  bilhanan
rewritten ua functionality

Revision 1.4  2003/08/24 22:19:39  bilhanan
multiple -> single inheritance, uses uamain's dafinder now.

Revision 1.3  2003/08/12 00:10:48  bilhanan
const char* in constructor -> std::string

Revision 1.2  2003/06/04 11:10:00  kalliosa
Licence clause added

  
*/


/**
 * Uaconn sends slp request when it gets UASAP-message from UASAP.
 * Uaconn also resends messages when needed.  
 */ 

#ifndef UACONN_H
#define UACONN_H

#include <string>
#include <doors/ptb.h>
#include <doors/uasap.h>
#include <doors/slptypes.h>
#include <doors/uaconnsm.h>
#include <doors/uaent.h>
#include <doors/uaconfigurator.h>

class UAEnt;
class UAConnSM;

class UAConn : public PTask
{
 public:
   

   UAConn( std::string n, Uint32 slphandle, UAEnt *uaent_,  UAConnSM *sm ); 

   virtual ~UAConn (void);
   

   bool mcIdle_default (Message *msg);
   bool mcIdle_Srvreq (Message *msg);
   bool mcIdle_Srvtypereq (Message *msg);
   bool mcIdle_Attrreq (Message *msg);
   bool mcIdle_SLPClose (Message *msg);
   bool mcIdle_DAADVERT (Message *msg);
   
   bool mcWfSrv_default (Message *msg);
   bool mcWfSrv_SLPClose (Message *msg);
   bool mcWfSrv_Srvrply (Message *msg);
   bool mcWfSrv_DAADVERT (Message *msg);
   bool mcWfSrv_timeout (Message *msg);

   bool mcWfSrvtype_default (Message *msg);
   bool mcWfSrvtype_SLPClose (Message *msg);
   bool mcWfSrvtype_Srvtyperply (Message *msg);
   bool mcWfSrvtype_DAADVERT (Message *msg);
   bool mcWfSrvtype_timeout (Message *msg);

   bool mcWfAttr_default (Message *msg);
   bool mcWfAttr_SLPClose (Message *msg);
   bool mcWfAttr_Attrrply (Message *msg);
   bool mcWfAttr_DAADVERT (Message *msg);
   bool mcWfAttr_timeout (Message *msg);

   bool daIdle_default (Message *msg);
   bool daIdle_Srvreq (Message *msg);
   bool daIdle_Srvtypereq (Message *msg);
   bool daIdle_Attrreq (Message *msg);
   bool daIdle_SLPClose (Message *msg);
   
   bool daWfSrv_default (Message *msg);
   bool daWfSrv_SLPClose (Message *msg);
   bool daWfSrv_Srvrply (Message *msg);
   bool daWfSrv_timeout (Message *msg);

   bool daWfSrvtype_default (Message *msg);
   bool daWfSrvtype_SLPClose (Message *msg);
   bool daWfSrvtype_Srvtyperply (Message *msg);
   bool daWfSrvtype_timeout (Message *msg);

   bool daWfAttr_default (Message *msg);
   bool daWfAttr_SLPClose (Message *msg);
   bool daWfAttr_Attrrply (Message *msg);
   bool daWfAttr_timeout (Message *msg);

   void mc_timeout (void);
   void da_timeout (void);
   UASAP :: Provider &up;
   SLPPeer &peer;
   Port entity;
   
   UAEnt *uaent;
      
   Uint16 XID;

 private:

   int frameToVector( Frame &f, StringVector &v );

   Timer retTimer;
   UAConfigurator *myconfig;
   
   Message *retrymsg;
   
   int mcast_time;
   int ucast_time;
   std::set <InetAddr6> known_PRList;
   
};

#endif  // UACONN_H

