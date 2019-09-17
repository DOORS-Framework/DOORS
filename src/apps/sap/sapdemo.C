//Editor-Info: -*- C++ -*-

//  Copyright 2002
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

$Log: sapdemo.C,v $
Revision 1.10  2005/05/04 11:33:37  bilhanan
bugfix: encode bit was previously unset

Revision 1.9  2003/08/11 13:21:42  bilhanan
from ipv4 to ipv4+ipv6

Revision 1.8  2003/07/10 12:04:10  bilhanan
*** empty log message ***

Revision 1.7  2003/04/17 09:06:13  bilhanan
Forced commit to document multiple sessionhandling, udptask moved away from here to protocol.

Revision 1.6  2003/04/17 07:39:53  bilhanan
slightly saner names.

Revision 1.5  2003/04/16 12:02:50  bilhanan
originating address/port binding bugfix

Revision 1.4  2003/04/09 14:39:33  ik
Binding address changed to own address.

Revision 1.3  2002/07/23 14:28:53  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.2  2002/06/17 12:24:10  hartmanj
New UdpTask to use

Revision 1.1  2002/06/14 16:02:25  hartmanj
Initial


*/

#include <doors/ptb.h>
#include <doors/lemon.h>

#include <doors/sapentity.h>
#include <doors/sapentitysm.h>

#include <netinet/in.h>

int main (void) {

	if (!getenv("DMMTYPE")) {
		std::cout << "Please specify environment variable DMMTYPE(=plain, for example)." << std::endl;
		return 0;
	}
	if (!getenv("HOST")) {
		std::cout << "Please specify environment variable HOST." << std::endl;
		return 0;
	}
	if (!getenv("HOST6")) {
		std::cout << "Please specify environment variable HOST6" << std::endl;
		return 0;
	}
	// create input-output handler for tasks
    IoHandler *io = IoHandler::Instance();

    // create main scheduler
    Scheduler *s = Scheduler::Instance();

    // create task for managing timers
    TimerTask *tt  = TimerTask::instance(s, io);

	MemoryManager *memmgr = MemoryManager::instance();
	UiTask ui(s, io);
	ui.notify( memmgr );
	DebugScheduler ds(0, 0, s);

	EnvTask env(s);

	InetAddr destaddr;
	destaddr.set(9875, "224.2.127.254");

	InetAddr6 destaddr6;
	destaddr6.set(9875, "FF05:0:0:0:0:0:2:7FFE");

	SAPEntitySM sapsm;

	SAPEntity sape( "SAPEntity1", &ds, &sapsm, &destaddr, &destaddr6);

	STATIC_SYMBOL(sape,0);

	SAPEntSAP::User *usr = new SAPEntSAP::User(&env,0);

	env.add(usr, "user1");

	sape.up.connect(usr);

    while( ui.isTerminated() == false ) {
        s->run();
    }

	DELETE_SYMBOL(&sape);

	env.remove();

    return 0;
}
