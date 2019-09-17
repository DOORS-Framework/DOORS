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

$Log: main.C,v $
Revision 1.2  2004/01/19 11:09:22  bilhanan
#include fixed.

Revision 1.1  2003/11/17 12:37:35  hartmanj
Initial commit


*/


#include <doors/mc6joinent.h>
#include <doors/mc6joinesm.h>
#include <doors/mc6joinsap.h>
#include <doors/udp6.h>
#include <doors/udp6task.h>

/*
 * Usage for this program is: mcjoiner multicast-address [port]
 * In main( ) we check that a HOST environment variable is present
 * as we set it as source address. One or two arguments must also
 * be present. Port is optional as it doesn't really matter anyhow.
 * If port argument is omitted, a default value of 20005 is used.
 */

int main (int argc, char *argv[]) {

  if (!getenv ("HOST")) {
       std::cerr << "ERROR: Environment variable HOST needed!!!" << std::endl;
       return 0;
  }

  std::string mcAddr;
  int portnum=20005;
  
  if(argc == 2) {
       std::cout << "Setting multicast address " << argv[1] << std::endl;
       mcAddr = argv[1];
  } else if(argc == 3) {
       portnum = strtol(argv[2],0,10);
       mcAddr = argv[1];

       if(portnum == EINVAL || portnum == 0 || portnum == ERANGE) {
            std::cerr << "Invalid port number. Not sending anything. Check your params!" << std::endl;
            return 0;
       }
       
       std::cout << "Setting multicast address " << argv[1];
       std::cout << " and port " << portnum << std::endl;
  } else {
       std::cerr << "Error: missing arguments." << std::endl;
       std::cout << "Usage: mcjoiner multicast-address [port]" << std::endl;
       return 0;
  }

  InetAddr6 ownaddr6;
  ownaddr6.set(50002, getenv("HOST"));

  InetAddr6 daMcAddr;
  daMcAddr.set(portnum, mcAddr, 0);

  // create main scheduler
  Scheduler *s = Scheduler::Instance();
  
  IoHandler *io = IoHandler :: Instance ();

  MC6JoinerEntitySM mc6sm;
  Udp6Task udp6task("udp6task",s,io,&ownaddr6);

  MC6JoinerEntity ent("mc6joiner", s, &mc6sm,
                      ownaddr6, &udp6task);

  MC6JoinerSAP :: User *usr = new MC6JoinerSAP :: User(&ent, 0);
  ent.up.connect(usr);


  MC6JoinerSAP :: Open *openmsg = (MC6JoinerSAP :: Open*) usr->create(MC6JoinerSAP :: open);
  openmsg->mc_addr = daMcAddr;
  usr->putMessage(openmsg);

  while(1) {
    s->run();
  }

  return 0;
}
