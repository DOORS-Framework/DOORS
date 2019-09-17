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

  $Log: tcpmuxdemo.C,v $
  Revision 1.3  2005/03/02 14:32:01  bilhanan
  comments added.

  Revision 1.2  2005/02/21 13:59:45  bilhanan
  development

  Revision 1.1  2004/10/12 12:14:27  bilhanan
  Demo for tcpmux

  Revision 1.2  2002/08/19 12:22:51  hartmanj
  Licence added


*/

#include <string.h>
#include <stdlib.h>
#include <iostream>


#include <doors/osched.h>
#include <doors/inetaddr.h>
#include <doors/memmgr.h>
#include <doors/ptb.h>
#include <doors/lemon.h>

#include <doors/tcpmuxesm.h>
#include <doors/tcpmuxent.h>
#include <doors/tcpmuxconn.h>

int main(int argc, char *argv[])
{

  if (argc < 2) {
    std::cout << "Usage: tcpmuxdemo <listening_port>" << std::endl;
    return 0;
  }

  if( !getenv("HOST")) {
    std::cout << "Please specify environment variable HOST" << std::endl;
    return 0;
  }
  
  int listening_port = atoi (argv[1]);
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
  
  InetAddr ownaddr;
  ownaddr.set(listening_port, getenv("HOST"), 1);

  
  TcpmuxEntitySM tcpmuxesm;

  // we want tcpmux to act as both server and client.
  // for pure client mode, pass null pointer for address instead.

  TcpmuxEntity tcpmuxent("tcpmuxent", s, io, &ownaddr, &tcpmuxesm);
  
  
  STATIC_SYMBOL(tcpmuxent,0);
  
  TcpmuxSAP::User usr(&env,0);
  
  
  env.add(&usr, "usr");
  
  tcpmuxent.up.connect(&usr);
  
  
  while( ui.isTerminated() == OdFalse ) {
    s->run();
  }
  
  
  env.remove();
  
  return 0;
  
}
