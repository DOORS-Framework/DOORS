// Editor-info: -*- C++ -*-
//
//  Copyright 1999 
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
 
 
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

//  $Log: linkconn.h,v $
//  Revision 1.6  2003/11/14 10:20:25  bilhanan
//  CloseAll message handling and general cleanup.
//
//  Revision 1.5  2003/08/11 23:47:29  bilhanan
//  const char* in constructor -> std::string
//
//  Revision 1.4  2002/07/23 16:38:31  ik
//  Od{Boolean,True,False} -> {bool,true,false}
//
//  Revision 1.3  2002/07/12 08:47:43  bilhanan
//  linkrandpdu unneeded and therefore removed.
//
//  Revision 1.2  2002/07/12 06:31:46  bilhanan
//  Licence clause and Log keyword added
// 

#ifndef LINKTASK_H
#define LINKTASK_H

#include <string>
#include <doors/ptb.h>
#include <doors/link.h>
#include <doors/phys.h>
#include <doors/linkpdu.h>
#include <doors/sbuf.h>

class LinkEntity;
class LinkTaskSM;


class LinkTask : public PTask {
  public:
    LinkTask (std::string n, Uint16 id, LinkEntity *e, LinkTaskSM *lsm);
    virtual ~LinkTask (void);

    bool closed_Creq (Message *msg);
    bool closed_CR (Message *msg);
    bool closed_default (Message *msg);

    bool wfCresp_Cresp (Message *msg);
    bool wfCresp_timeout (Message *msg);
    bool wfCresp_default (Message *msg);

    bool wfCC_CC (Message *msg);
    bool wfCC_timeout (Message *msg);
    bool wfCC_default (Message *msg);

    bool idle_DATA (Message *msg);
    bool idle_Dtreq (Message *msg);
    bool idle_Dreq (Message *msg);
    bool idle_CloseAll (Message *msg);
    bool idle_DR (Message *msg);
    bool idle_default (Message *msg);

    bool wfACK_DATA (Message *msg);
    bool wfACK_DR (Message *msg);
    bool wfACK_Dtreq (Message *msg);
    bool wfACK_Dreq (Message *msg);
    bool wfACK_ACK (Message *msg);
    bool wfACK_timeout (Message *msg);
    bool wfACK_default (Message *msg);

    bool wfDR_DR (Message *msg);
    bool wfDR_timeout (Message *msg);
    bool wfDR_default (Message *msg);

    Uint16 getId (void);


    LinkSAP :: Provider &up;
    PhysSAP :: User &down;
    LinkPeer &peer;

    Port entity;

  private:
    void incSendNumber (void);
    void incRecNumber (void);

    Timer retTimer;
    Timer crespTimer;

    Uint16 cid;
    Uint16 seqNumber;
    Uint16 sendNumber;
    Uint16 recNumber; 

    SBuffer buffer;
};


#endif  // LINKTASK_H

