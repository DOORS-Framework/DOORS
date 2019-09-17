
  //  Copyright 2000
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
  
$Log: mc6joinent.C,v $
Revision 1.2  2004/01/19 11:10:28  bilhanan
#include fixed.

Revision 1.1  2003/11/17 12:37:35  hartmanj
Initial commit


*/

#include <iostream>
#include <doors/mc6joinent.h>
#include <doors/mc6joinsap.h>
#include <doors/mc6joinesm.h>

MC6JoinerEntity::MC6JoinerEntity(std::string n, Scheduler *s, MC6JoinerEntitySM *esm,
                      const InetAddr6 addr, Udp6Task *udp )
     :PTask(n,s,esm), up(this), down(udp), addr_(addr), udp6task_(udp), esm_(esm)
{
     down.connect(&udp6task_->up);
}

MC6JoinerEntity::~MC6JoinerEntity()
{
}

bool MC6JoinerEntity::ready_default(Message * )
{
     return false;
}

bool MC6JoinerEntity::OpenMC6Group ( Message *msg )
{
     MC6JoinerSAP::Open *openmsg = (MC6JoinerSAP::Open*) msg;
     Udp6SAP :: JoinMcast *joinmsg = (Udp6SAP :: JoinMcast*) down.create(Udp6SAP :: joinmcast);

     joinmsg->addr = openmsg->mc_addr;

     down.putMessage(joinmsg);

     return false;
}
