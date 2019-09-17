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

$Log: aaent.h,v $
Revision 1.3  2006/11/21 19:52:11  bilhanan
development

Revision 1.2  2006/07/29 21:42:09  bilhanan
AA improvements

Revision 1.1  2004/10/12 11:57:18  bilhanan
AA code merged into main tree from src/experimental

Revision 1.6  2004/08/03 05:36:41  hartmanj
Bug fixes

Revision 1.5  2004/05/25 09:41:32  hartmanj
Typo-fixes, removed use of deprecated classes/methods, general development.

Revision 1.4  2003/08/24 22:11:41  bilhanan
devt

Revision 1.3  2003/08/20 11:48:07  bilhanan
va<->aa<->network srv req/rply functionality coded.

Revision 1.2  2003/08/01 12:12:17  bilhanan
devt

Revision 1.1  2003/07/30 09:51:18  bilhanan
*** empty log message ***

  
*/

#ifndef _AAENT_H
#define _AAENT_H

#include <vector>
#include <string>
#include <doors/ptb.h>
#include <doors/udp6task.h>
#include <doors/slppdu.h>
#include <doors/serviceurl.h>
#include <doors/slpcommon.h>
#include <doors/dafinder.h>
#include <doors/aaconfigurator.h>

class AAEntitySM;
class UAProxySM;
//class AAConfigurator;


class AAEntity : public PTask, public SLPCommon {

public :
  AAEntity();
  AAEntity (std::string n, Scheduler *s, AAEntitySM *sm, Udp6Task *udp, InetAddr6 &addr);
  
  
  virtual ~AAEntity();

  bool ready_srvRqst (Message *msg);
  bool ready_attrRqst (Message *msg);
  bool ready_srvTypeRqst (Message *msg);
  bool ready_daAdvert (Message *msg);

  bool ready_srvRply (Message *msg);
  bool ready_srvReg (Message *msg);
  bool ready_srvDeReg (Message *msg);
  
  bool ready_AATimer (Message *msg); 
  bool ready_default (Message *msg);
  
  SLPPeer peer;
  Udp6SAP :: User down;
  

private:

  friend class UAProxy;
  void sendReply (InetAddr6 &dest, URLVector &urls);
  void sendAAAdv (void);
  bool foruaproxy (Message *msg);
  static bool isForUAProxy( const EventTask *t, const Message *m);

  void createuaproxy (Message *msg, Uint16 xid);
  std::vector<ServiceURL> Services_;

  AAConfigurator *myconfig;

  DAFinder dafinder;
  InetAddr6 ownAddr_;
  std::string my_scope;

  InetAddr6 adv_addr;
  int heartBeat_;
  OTime advinterval;
  Timer advTimer_;

  Uint16 XID;

 
  Frame _frmaaprefix;

  Port to_uaproxy;
  PortMux uaproxyMux;
  UAProxySM *uaproxySM;
};



#endif //  AAENT_H
