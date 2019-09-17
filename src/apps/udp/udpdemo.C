//Editor-Info: -*- C++ -*-

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
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/*

$Log: udpdemo.C,v $
Revision 1.8  2003/05/15 19:57:30  bilhanan
std namespace prefix added.

Revision 1.7  2003/05/15 18:08:01  bilhanan
fixes for std c++ headers

Revision 1.6  2002/07/26 11:02:14  ik
Back to const char* due to problems in std::string as a function
parameter.

Revision 1.5  2002/07/23 14:28:54  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.4  2002/06/19 16:23:47  ik
Bind to local address not remote.

Revision 1.3  2002/06/17 14:06:18  ik
Start using system rand(). Buggy file reading flag.

Revision 1.2  2002/06/14 14:57:49  ik
Wrong address for UdpTask.

Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.4  2002/02/25 17:08:33  ik
Changes due to UnixAddr/InetAddr structure change.

Revision 1.3  2001/12/13 10:22:45  ik
No changes; CVS update.

Revision 1.2  2001/06/08 16:46:54  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:40:05  bilhanan
Full licence clause added into doors 0.1


*/
//
//===========================================================================
//File:     udplg.C
//Date:     22.12.1998
//Desc:     main()
//
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#include <doors/ptb.h>
//#include "tpt.h"
//#include "omm.h"

#include <doors/inetaddr.h>
#include <doors/udpsockconn.h>
#include <doors/udpapp.h>
#include <doors/udpappsm.h>
#include <doors/udpapptask.h>
#include <doors/udptask.h>
#include <iostream>


#define OPTSTRING "f:p:hm:o:i:l:d:"

void usage(char *app)
{
  fprintf(stderr, "\
Usage: %s [-%s]\n\n\
          [-m <type>]      memory manager type (default: plain)\n\
          [-f <size>]      frame size (default: 5000)\n\
          [-p <port>]      sending/receiving port (default: 50000)\n\
          [-o <filename>]  file to save the received UDP packets\n\
          [-i <filename>]  file to send (default: send random data)\n\
          [-l <loops>]     number of loops, zero means infinite (default: 1)\n\
          [-d <hostname>]  receiving hostname or IP-address\n\
          [-h]             print this help, then exit\n", app, OPTSTRING);
  exit(1);
}

int
main (int argc, char **argv) 
{
  int optc;
  Uint16 port, ownport, destport;
  Uint16 framesz;
  int loops;
  char *ofilename, *ifilename;
  char *desthost;
  InetAddr addr1;
  InetAddr addr2;

  port = 50000;
  framesz = 5000;
  ifilename = ofilename = NULL;
  desthost = NULL;
  loops = 1;
  while ((optc = getopt(argc, argv, OPTSTRING)) != EOF) {
    switch(optc) {
    case 'l' :
      loops = atoi(optarg);
      if (loops == 0)
	loops = LOOP_FOREVER;
      break;
    case 'p' :
      port = atoi(optarg);
      break;
    case 'f' :
      framesz = atoi(optarg);
      break;
    case 'o' :
      ofilename = optarg;
      break;
    case 'i' :
      ifilename = optarg;
      break;
    case 'm' :
      break;
    case 'd' :
      desthost = optarg;
      break;
    case 'h' :
      usage(argv[0]);
      break;
    default :
      usage(argv[0]);
      break;
    }
  }
  
    // create input-output handler for tasks
    IoHandler *io = IoHandler :: Instance ();

    // create main scheduler
    Scheduler *s = Scheduler :: Instance();

    // create task for managing timers
    TimerTask *tt  = TimerTask :: instance (s, io);
    
    if (!getenv ("HOST")){
      std::cerr << "ERROR: Environment variable HOST needed!!!" << std::endl;
      return 0;} 

    if (desthost == NULL) {
      std::cout << "Destination host was not given. Starting receive mode..."
	   << std::endl;
      // Set frame size to zero to notify apptask to do only receiving
      framesz = 0;
      ownport = port;
      destport = 0;
      if (ofilename == NULL) {
	ofilename = "-";
      }
      std::cout << "Listening to port " << port << std::endl;
    } else {
      ownport = 0;
      destport = port;
      std::cout << "Starting send mode..." << std::endl;
      std::cout << "Destination: " << desthost << ":" << port
	   << std::endl;
      std::cout << "Frame size: " << framesz << std::endl;
      if (ifilename != NULL) {
	std::cout << "Sending file '" << ifilename << "' ";
      } else {
	std::cout << "Sending random data frames ";
      }
      if (loops == LOOP_FOREVER) {
	std::cout << "forever." << std::endl;
      } else {
	if (loops == 1) {
	  std::cout << "once." << std::endl;
	} else {
	  std::cout << loops << " times." << std::endl;
	}
      }
      if (ofilename != NULL) {
	std::cout << "Output file name is ignored in send mode." << std::endl;
	ofilename = NULL;
      }
    }

    addr1.set (ownport, getenv ("HOST"));
    if (desthost) {
      addr2.set (destport, desthost);
    } else {
      addr2.set (destport);
    }

    UdpAppTaskSM atsm;
    // create application task
    UdpAppTask app ("app", s, &atsm, &addr1, &addr2, framesz, loops, 
	  	 ifilename, ofilename);

    // create UDP task
    UdpTask udp ("udp", s, io, &addr1);


    // connect up SAP of udp task to created SAP on environment task
    udp.up.connect (&app.down);

    // run user interface until it is terminated

    while (app.isTerminated () == false) {
	s->run ();
    }

    // that's it
    return 0;

}

