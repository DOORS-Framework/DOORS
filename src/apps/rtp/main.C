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

$Log: main.C,v $
Revision 1.4  2003/08/11 23:35:36  bilhanan
bundled maintenance commit:
	const char* in constructor -> std::string
	#include ".. " -> #include <doors/..>
	ONS_CF_ -> _CF_

Revision 1.3  2002/08/14 09:33:03  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/07/23 14:28:52  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.3  2002/02/25 17:08:33  ik
Changes due to UnixAddr/InetAddr structure change.

Revision 1.2  2001/06/08 16:46:51  bilhanan
Full Licence clause into DOORS 0.2


*/
//
//===========================================================================
//File:     main.C
//Date:     4.3.1999
//Desc:     main() for MPEG-audio DOORS/RTP sending/receiving program
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

//#include "defs.h"
#include <doors/hsi.h>
//#include "tpt.h"
#include <doors/ptb.h>
//#include "omm.h"

#include <doors/inetaddr.h>
#include <doors/mp3sm.h>
#include <doors/mp3task.h>
#include <doors/rtpsap.h>
#include <doors/rtp.h>
#include <doors/rtpsm.h>
#include <doors/filempegrtp.h>
#include <doors/rtcpsm.h>
#include <doors/rtcptask.h>
#include <doors/udpsockconn.h>
#include <doors/udptask.h>
#include <doors/mcudp.h>
#include <doors/udptask.h>
#include <doors/udp.h>
#include <iostream.h>

//const char *mmtype = "plain";

#define WITH_RTCP

#if _CF_WITH_LAME
#ifdef WITH_RTCP
#define OPTSTRING "hm:o:lt:p:b:r"
#else
#define OPTSTRING "hm:o:lt:p:b:"
#endif
#else
#ifdef WITH_RTCP
#define OPTSTRING "hm:o:lt:p:r"
#else
#define OPTSTRING "hm:o:lt:p:"
#endif
#endif

#if 0
void mminit (void) {

  const char *mmtype = "plain";

    if (strcmp (mmtype, "block") == 0) {
	BlockManager *bm = (BlockManager *) MemoryManager :: instance ();
	bm->initBlock (272, 230);
    }

    if (strcmp (mmtype, "statblock") == 0) {
	StatBlockManager *sbm = (StatBlockManager *) MemoryManager :: instance ();
	sbm->initBlock (272, 230);
    }

    if (strcmp (mmtype, "blockmem") == 0) {
	BlockMemManager *bm = (BlockMemManager *) MemoryManager :: instance ();
	bm->initBlock (16, 4000);
	bm->initBlock (64, 1000);
	bm->initBlock (128, 500);
	bm->initBlock (544, 2);  // for State machines
    }

    if (strcmp (mmtype, "statblockmem") == 0) {
	StatBlockMemManager *smb = (StatBlockMemManager *) MemoryManager :: instance ();
	smb->initBlock (16, 4000);
	smb->initBlock (64, 1000);
	smb->initBlock (128, 500);
	smb->initBlock (544, 2);  // for State machines
    }
}
#endif

void usage(char *app)
{
  fprintf(stderr, "\
Usage: %s [-%s] <destination address>:<destination port> <filename>\n\n\
          [-m <type>]      memory manager type (default: plain)\n\
          [-o <filename>]  output file (default: standard out)\n\
          [-p <tunnel address>:<tunnel port>]\n\
          [-l]             use loopback when multicasting (default: no)\n\
          [-t <ttl>]       TTL (default: 3)\n\
          [-h]             print this help, then exit\n", app, OPTSTRING);
#if _CF_WITH_LAME
  fprintf(stderr, "\
          [-b <bitrate>]   Re-encode file with given bitrate (default: no)\n");
#endif
#ifdef WITH_RTCP
  fprintf(stderr, "\
          [-r]             Send/receive RTCP reports (default: no)\n");
#endif
  exit(1);
}

int
main (int argc, char **argv) 
{
  bool start_sending, istunnel;
  int optc;
  Uint16 port, ownport, destport, tunnelport;
  int loops;
  Uint8 ttl;
  bool mcastloop;
  bool send_reports = false;
  char *ofilename;
  char tunneladdr[64] = {0};
  char desthost[64] = {0};
  char hostname[64] = {0};
  char *p;
  int fileindex;
  int bitrate;

  bitrate = 0;
  port = 50000;
  ofilename = NULL;
  loops = 1;
  ttl = 3;
  mcastloop = false;
  istunnel = false;
  while ((optc = getopt(argc, argv, OPTSTRING)) != EOF) {
    switch(optc) {
    case 'p' :
      if (strchr(optarg, ':') == NULL) {
	// Both address and port number are to be given
	usage(argv[0]);
      } else {
	strcpy(tunneladdr, optarg);
	p = strchr(tunneladdr, ':');
	*p = '\0'; // Set NULL in place of ':'
	tunnelport = atoi(p+1);
	istunnel = true;
      }
      break;
    case 'o' :
      ofilename = optarg;
      break;
#if 0
    case 'm' :
      mmtype = optarg;
      break;
#endif
    case 'h' :
      usage(argv[0]);
      break;
    case 'l' :
      mcastloop = true;
      break;
    case 't' :
      ttl = atoi(optarg);
      break;
#if _CF_WITH_LAME
    case 'b' :
      bitrate = atoi(optarg);
      break;
#endif
#ifdef WITH_RTCP
    case 'r' :
      send_reports = true;
      break;
#endif
    default :
      usage(argv[0]);
      break;
    }
  }

  if (optind >= argc)
    usage(argv[0]);

  if (strchr(argv[optind], ':') == NULL) {
    // Both address and port number are to be given
    usage(argv[0]);
  } else {
    strcpy(desthost, argv[optind]);
    p = strchr(desthost, ':');
    *p = '\0'; // Set NULL in place of ':'
    port = atoi(p+1);
  }

  fileindex = 0;
  if (argc-optind > 1) {
    fileindex = optind + 1;
  }

#if 0
    // initilize memory manager
    mminit ();
#endif

    // create memory optimizer functionality
    //    MemoryOptimizer mopt("mem.log", MemoryOptimizer :: all);

    // create input-output handler for tasks
    IoHandler *io = IoHandler :: Instance();

    // create main scheduler
    Scheduler *s = Scheduler :: Instance();

    // create task for managing timers
    TimerTask *tt  = TimerTask :: instance (s, io);

#ifdef WITH_UI
    // create user interface task
    UiTask ui (&s, &io);
    // tell ui about memory manager
    //    ui.notify (MemoryManager::instance());

    // create environment task
    EnvTask env (&s);
#endif

    // create scheduler for debugging
    //    DebugScheduler ds (0, 0, &s);

    gethostname(hostname, 64);
    //    strcpy(hostname, "isokiuru");

    destport = port;
    if (fileindex == 0) {
      cerr << "File not given. Starting receive only mode..." << endl;
      start_sending = false;
    } else {
      start_sending = true;
      ownport = 0;
      cerr << "Starting send mode..." << endl;
      cerr << "Destination: " << desthost << ":" << port
	   << endl;
    }

    ownport = 8888;
    InetAddr addr1;
    addr1.set(ownport, hostname, 1); // 1 = encode the port number
    InetAddr addr2;
    addr2.set(destport, desthost, 1);

    // Create RTP layer
    RtpTaskSM rtpsm;
    FileMpegRtp rtp ("rtp", s, &rtpsm, &addr1, &addr2);

    // If input file is given (sending requested, open file and start 
    // send timer
    char* ifilename = NULL;
    if (start_sending) {
      ifilename = argv[fileindex];
#if _CF_WITH_LAME
      if (bitrate) {
	cerr << "Re-encoding frames with bitrate " << bitrate << " kbps";
	cerr << endl;
	rtp.open_reencode(ifilename, bitrate);
      } else {
#endif
	rtp.open(ifilename);
#if _CF_WITH_LAME
      }
#endif
    }

#if 0
  // If output file is given, open file
  if (ofn != NULL) {
    if (strcmp(ofn, "-") == 0) {
      tostdout_ = true;
      cerr << "Writing to standard output" << endl;
    } else {
      ofile_.open(ofn, ios::out|ios::trunc);
      cerr << "Writing to file '" << ofn << "'." << endl;
    }
  }
#endif

    // create an UDP task
    McUdpTask rtp_udp = McUdpTask("rtp_udp", s, io, 
				  &addr1, &addr2, ttl, mcastloop);

    if (istunnel) {
      InetAddr taddr;
      taddr.set(tunnelport, tunneladdr, 1);
      rtp_udp.openTunnel(&taddr);
    }

#ifdef WITH_RTCP
    // Find RTCP address
    InetAddr rtcp_addr1;
    rtcp_addr1.set(ownport + 1, hostname, 1);
    InetAddr rtcp_addr2;
    rtcp_addr2.set(destport + 1, desthost, 1);
    // create an RTCP/UDP task
    McUdpTask rtcp_udp = McUdpTask("rtcp_udp", s, io,
				   &rtcp_addr1, &rtcp_addr2, ttl, mcastloop,
				   false);
    // Open tunnel if required
    if (istunnel) {
      InetAddr taddr;
      taddr.set(tunnelport + 1, tunneladdr, 1);
      rtcp_udp.openTunnel(&taddr);
    }
#endif

#ifdef WITH_RTCP
    RtcpTaskSM rtcpsm;
    RtcpTask rtcp ("rtcp", s, &rtcpsm, &rtcp_udp, &rtp, send_reports);
#endif

    // Create Application layer task
    MP3TaskSM appsm;
    MP3Task app("app", s, &appsm, &addr1, &addr2, 3,
		start_sending);

#ifdef WITH_RTCP
    app.set_rtcp(&rtcp);
#endif

#ifdef WITH_UI
#ifdef WITH_RTCP
    STATIC_SYMBOL (rtcp, 0);
#endif
    STATIC_SYMBOL (rtp, 0);

    // create user SAP to environment task
    RtpSAP :: User *usr1 = new RtpSAP :: User (&env, 0);

    // add user sap to environment by name
    env.add (usr1, "usr1");
    rtp.up.connect (usr1);

#ifdef WITH_RTCP
    // create user SAP to environment task
    RtcpSAP :: User *usr2 = new RtcpSAP :: User (&env, 0);
    env.add (usr2, "usr2");
    rtcp.up.connect (usr2);
#endif
#endif

    // Make connections between layers
    rtp.up.connect (&app.rtpdown);       // App -> RTP
    rtp_udp.up.connect (&rtp.down);   // RTP -> RTP-UDP
    STATIC_SYMBOL (rtp_udp, 0);
#ifdef WITH_RTCP
    rtcp.up.connect (&app.rtcpdown);     // App -> RTCP
    rtcp_udp.up.connect (&rtcp.down); // RTCP -> RTCP->UDP
    STATIC_SYMBOL (rtcp_udp, 0);
#endif

    // Tell about the first file if given
    if (fileindex != 0) {
      cerr << "Sending file '" << argv[fileindex] << "' " << endl;
    }

    // run user interface until it is terminated
#ifdef WITH_UI
    while (ui.isTerminated () == false) {
	s.run ();
    }
#else
    //    while (rtp.isTerminated () == false) 
    for ( ; ; )
      {
	if (rtp.isTerminated () == true) {
	  if ((fileindex) && (++fileindex < argc)) {
	    rtp.close();
	    rtp.open(argv[fileindex]);
	    rtp.startSending();
	    cerr << "Sending file '" << argv[fileindex] << "' " << endl;
	  } else
	    break;
	}
	s->run ();
      }
#endif

#ifdef WITH_RTCP
    if (istunnel)
      rtcp_udp.closeTunnel();
#endif
    if (istunnel)
      rtp_udp.closeTunnel();

    rtp.close();

#ifdef WITH_UI
    // remove used symbol
#ifdef WITH_RTCP
    DELETE_SYMBOL (&rtcp);
#endif
    DELETE_SYMBOL (&rtp);

#ifdef WITH_RTCP
    DELETE_SYMBOL (rtcp_udp);
#endif
    DELETE_SYMBOL (rtp_udp);

    // clean up environment
    env.remove ();
#endif

    // that's it
    return 0;

    // destroy automatically created tasks here
}

