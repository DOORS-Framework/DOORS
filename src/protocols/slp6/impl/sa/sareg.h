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

$Log: sareg.h,v $
Revision 1.12  2004/04/05 06:55:59  hartmanj
Renamed samain* to saent*

Revision 1.11  2004/03/22 11:10:56  hartmanj
Moved attribute request handling from SAMain to SAReg.

Revision 1.10  2004/03/19 11:24:03  hartmanj
Adapting new SLPCommon API.

Revision 1.9  2004/03/16 06:54:43  hartmanj
isOfType() type now const

Revision 1.8  2004/02/18 14:54:17  hartmanj
Yet Another XID handling fix

Revision 1.7  2004/02/12 08:04:51  hartmanj
Changed XID to be handled in SLPCommon. Removed lastregxid_ and added bool hasXID(Uint16 XID)

Revision 1.6  2004/02/04 15:03:06  hartmanj
Complete rewrite of DA-SA interaction code.

Revision 1.5  2003/08/24 22:23:53  bilhanan
multiple -> single inheritance, uses samain's dafinder now.

Revision 1.4  2003/08/12 00:07:46  bilhanan
const char* in constructor -> std::string

Revision 1.3  2003/06/04 11:09:59  kalliosa
Licence clause added

  
*/

#ifndef SAREG_H
#define SAREG_H

#include <string>
#include <doors/ptb.h>
#include <doors/sasap.h>
#include <doors/slppdu.h>
#include <doors/slptypes.h>
#include <doors/serviceurl.h>
#include <doors/saregsm.h>
#include <doors/saent.h>
#include <doors/slpcommon.h>
#include <doors/dafinder.h>

class SAEnt;
class SARegSM;

class SAReg : public PTask
{
 public:
   SAReg( std::string n, SASAP::registerService *reg , SAEnt *saent,  SARegSM *sm ); 
   virtual ~SAReg (void);
   SAEnt *saent_;   
   Port entity_;
   Udp6SAP :: User &down_;
   SLPPeer &peer_;

   
   ServiceURL url_;

   std::vector<SLPCommon::Attribute> attributes_;
   AttrVector attrs_;
  
   bool isOfType( const std::string &type ) const;
   
   bool ready_default(Message *);
   bool ready_daAdvert(Message *);
   bool ready_timer( Message *msg );
   bool ready_srvAck( Message *msg );
   bool ready_attrRqst( Message *msg );

   bool hasXID(Uint16 XID);
   ServiceURL getURL();
	 //TODO: get rid of getMCaddr, used in saent.C:isforsareg
   InetAddr6 getMCaddr(void);

   bool isGood() const;
  
 private:
  
     void updateDARegistrations( void );
     void sendRegToDA( const DAFinder::DAInfo *DA, Uint16 sendXID );
     void createRegistration( void );

     static const int MulticastTimeouts[5];

     // TImer to time the re-registrations. Beats at some sane interval
     Timer regTimer_;

     SLPPeer::SRVREG  *registration_;// servregistration for resend


     struct myDARegInfo {
          Sint32 lifetime;
          Uint32 refreshFreq;
          Uint16 resendCount; // in secs
          bool wfAck;
          Timer resendTimer;
          Uint16 XID;
     };

     // These two must be in sync always as they should correspond
     // myDAs_[x] --> myDAInfos[x]
     std::vector <DAFinder::DAInfo*> myDAs_;
     std::vector <myDARegInfo> myDAInfos_;
     std::string myScope_;
     Uint32 defaultRefreshFreq;
     
     bool newReplies_;
     Uint16 resendTime_;

     InetAddr6 myMCaddr_;
   
     /**
      * @returns time to wait for next reply. zero if CONFIG_MC_MAX already
      * has already elapsed
      */
     Uint32  getNextTimeout( );

     void setDAaddr( );
   
     /** 
      * (re)Sends request which is stored in registration_
      */
     void send( void );

  bool good_;
};

#endif  // SAREG_H

