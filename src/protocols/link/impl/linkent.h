// Editor info: -*- C++ -*-
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

//  $Log: linkent.h,v $
//  Revision 1.7  2003/11/14 10:20:25  bilhanan
//  CloseAll message handling and general cleanup.
//
//  Revision 1.6  2003/08/11 23:47:30  bilhanan
//  const char* in constructor -> std::string
//
//  Revision 1.5  2002/07/23 16:38:31  ik
//  Od{Boolean,True,False} -> {bool,true,false}
//
//  Revision 1.4  2002/07/22 22:33:38  bilhanan
//  Licence clause added.
//
//  Revision 1.3  2002/07/12 08:47:43  bilhanan
//  linkrandpdu unneeded and therefore removed.
//
//  Revision 1.2  2002/07/12 06:31:46  bilhanan
//  Licence clause and Log keyword added
// 

#ifndef LINKENT_H
#define LINKENT_H

#include <string>
#include <doors/ptb.h>
#include <doors/link.h>
#include <doors/phys.h>
#include <doors/linkpdu.h>

class LinkEntitySM;
class LinkTaskSM;

class LinkEntity : public PTask {
  public:
    LinkEntity (std::string n, Scheduler *s, LinkEntitySM *sm);
    virtual ~LinkEntity (void);

    bool ready_Creq (Message *msg);
    bool ready_CR (Message *msg);
    bool ready_CloseAll (Message *msg);
    bool ready_default (Message *msg);

    void createConnection (Uint16 cid);
    LinkSAP :: Provider up;
    PhysSAP :: User down;
    LinkPeer peer;

  private:
    Port connections;
    PortMux connMux;
    LinkTaskSM *connSM;
};

#endif  // LINKENT_H

