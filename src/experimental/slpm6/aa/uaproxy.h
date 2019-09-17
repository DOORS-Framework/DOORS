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
$Log: uaproxy.h,v $
Revision 1.5  2004/08/03 05:36:41  hartmanj
Bug fixes

Revision 1.4  2004/05/25 09:41:32  hartmanj
Typo-fixes, removed use of deprecated classes/methods, general development.

Revision 1.3  2003/08/20 11:48:07  bilhanan
va<->aa<->network srv req/rply functionality coded.

Revision 1.2  2003/08/19 12:38:42  bilhanan
development & retransmit capability removed.

Revision 1.1  2003/08/18 10:54:25  bilhanan
initial version.

*/

#ifndef _uaproxy_h_
#define _uaproxy_h_

#include <string>
#include <doors/ptb.h>
#include <doors/slppdu.h>

class AAEntity;
class UAProxySM;

class UAProxy : public PTask {

 public:

  UAProxy (std::string n, Uint16 AA_XID, Uint16 VA_XID, InetAddr6 source, AAEntity *e, UAProxySM *sm);
  virtual ~UAProxy (void);

  bool ready_srvRqst (Message *msg);
  bool ready_srvTypeRqst (Message *msg);
  bool ready_default (Message *msg);

  bool wfSrvRply_srvRply (Message *msg);
  bool wfSrvRply_srvRqst (Message *msg);

  bool wfSrvRply_default (Message *msg);

  bool wfSrvRply_timeout (Message *msg);

  bool wfSrvTypeRply_srvTypeRply (Message *msg);

  bool wfSrvTypeRply_timeout (Message *msg);



  SLPPeer &peer;
  Port entity;

private:

  SLPPeer :: SRVRQST *va_msg;
  SLPPeer :: SRVTYPERQST *va_type_msg;

  friend class AAEntity;
  AAEntity *ent;
  Uint16 aa_XID;
  Uint16 va_XID;
  InetAddr6 va_addr;

  Timer timer;
  Uint32 time_used;
  Uint32 max_time_try;
};


#endif
