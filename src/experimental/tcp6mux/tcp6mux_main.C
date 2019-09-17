//  Copyright 2002
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.

//  Redistribution and use in source and binary forms, with or without
//  mo	${DOORS_BUILD_DIR}/tools-offline/xcg/dsapg/dsapg --cc-file tcp6mux.C \
	--hh-file tcp6mux.h tcp6mux.sap	${DOORS_BUILD_DIR}/tools-offline/xcg/dsapg/dsapg --cc-file tcp6mux.C \
	--hh-file tcp6mux.h tcp6mux.sapdification, are permitted provided that the following conditions
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

  $Log: tcp6mux_main.C,v $
  Revision 1.1  2004/08/18 06:22:16  bilhanan
  TCP IPv6 multiplexer


*/

// This application is just a test hack for now

#include <string.h>
#include <stdlib.h>
#include <iostream>


#include <doors/osched.h>
#include <doors/inetaddr6.h>
#include <doors/memmgr.h>
#include <doors/ptb.h>
#include <doors/lemon.h>

#include "tcp6muxesm.h"
#include "tcp6muxent.h"
#include "tcp6muxconn.h"


int main(int argc, char *argv[])
{
	int idletimeout = 300;

    IoHandler *io = IoHandler::Instance();
    Scheduler *s = Scheduler::Instance();
    TimerTask *tt  = TimerTask::instance(s, io);
    MemoryManager *memmgr = MemoryManager::instance();

    UiTask ui(s, io);
    ui.notify( memmgr );
    DebugScheduler ds(0, 0, s);
    
    EnvTask env(s);
    
    InetAddr6 ownaddr;
    ownaddr.set(5738);

    Tcp6muxEntitySM tcp6muxesm;
    Tcp6muxEntity tcp6muxent("tcp6muxent", s, io, &ownaddr, &tcp6muxesm, idletimeout);

    STATIC_SYMBOL(tcp6muxent,0);

    Tcp6muxSAP::User usr(&env,0);

    env.add(&usr, "usr");

    tcp6muxent.up.connect(&usr);
	
    while( ui.isTerminated() == OdFalse ) {
        s->run();
    }

    env.remove();
    return 0;
}
