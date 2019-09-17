// Editor-info: -*- C++ -*-

//  Copyright 2000 
//  	Telecoms Lab, Tampere University of Technology.  All rights reserved.

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

//===========================================================================
//File:     altbit.C
//Date:     13.3.2003
//Desc:     Alternating bit demo main file
//Author:   Ilkka Karvinen, TTY/DMI
//===========================================================================

/*
$Log: altbit.C,v $
Revision 1.3  2004/01/19 10:53:03  bilhanan
#include fixed.

Revision 1.2  2004/01/13 17:00:57  ik
Added command line parameter for port number.

Revision 1.1  2003/07/02 11:27:52  ik
Initial revision.

*/

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <doors/ptb.h>
#include <doors/lemon.h>
#include <doors/udp.h>
#include <doors/udptask.h>
#include <doors/inetaddr.h>
#include <doors/altbittask.h>

int
main (int argc, char** argv)
{
  int port_number = 32001;

  if (argc < 2) {
    std::cout << "Usage: altbit <remote address> [<port number>]" << std::endl;
    return 0;
  }

  if (argc > 2) {
    port_number = atoi(argv[2]);
    std::cout << port_number << std::endl;
  }

  // Create I/O Handler 
  IoHandler *io = IoHandler :: Instance ();

  // Create main scheduler
  Scheduler *s = Scheduler :: Instance ();

  // Create TimerTask
  TimerTask *tt = TimerTask :: instance (s, io);
  
  UiTask ui (s, io);

  // Environment task
  EnvTask env (s);

  // Scheduler for debugging
  DebugScheduler ds (0, 0, s);

  // Create address with the port 32001
  InetAddr addr(port_number, argv[1]);
  
  AltbitTaskSM altbitsm;
  AltbitTask altbit("altbit", &ds, &altbitsm, addr);

  // This notifies the debugger of a new task to trace
  STATIC_SYMBOL (altbit, 0);

  InetAddr own;
  own.set (port_number); // Just set the port number; any host will match
  UdpTask udp("udp", &ds, io, &own);

  STATIC_SYMBOL (udp, 0);
  
  AltbitSAP :: User *usr = new AltbitSAP :: User(&env, 0);
  env.add (usr, "usr");

  // Connect usr environment SAP to altbit's up SAP
  altbit.up.connect(usr);

  // Connect UDP task to Altbit
  udp.up.connect (&altbit.down);

  while (ui.isTerminated () == false) {
    s->run ();
  }

  DELETE_SYMBOL (&env);
  DELETE_SYMBOL (&altbit);
  DELETE_SYMBOL (&udp);
  
  env.remove ();
  
  return 0;
}




