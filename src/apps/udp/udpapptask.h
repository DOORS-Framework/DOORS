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

$Log: udpapptask.h,v $
Revision 1.6  2003/08/11 23:40:31  bilhanan
const char* in constructor -> std::string

Revision 1.5  2003/05/16 13:30:20  bilhanan
Uint16 type changed to Sint16 to prevent comparison mismatch

Revision 1.4  2003/05/15 12:13:23  bilhanan
fixes for std c++ compliance.

Revision 1.3  2002/07/23 14:28:54  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.2  2002/06/17 14:06:17  ik
Start using system rand(). Buggy file reading flag.

Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.2  2001/06/08 16:45:23  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:40:05  bilhanan
Full licence clause added into doors 0.1


*/
//
//===========================================================================
//File:     apptask.h
//Date:     22.12.1998
//Desc:     Class definition for loadgenerator statemachine functions 
//
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#ifndef UDPAPPTASK_H
#define UDPAPPTASK_H

#include <string>
#include <doors/ptb.h>
#include <doors/udpapp.h>
#include <doors/udpappsm.h>
#include <doors/udp.h>

#include <doors/frame.h>
#include <iostream>
#include <fstream>

#define LOOP_FOREVER -1

class UdpAppTaskSM;

class UdpAppTask : public PTask {
public:
  UdpAppTask (std::string n, Scheduler *s, UdpAppTaskSM *sm, InetAddr *own, InetAddr *dest, Uint16 framesz, int loops, char *ifilename, char *ofilename);
  virtual ~UdpAppTask (void);

  bool isTerminated (void) { return terminated; };
 
  bool idle_timeout (Message *msg);
  bool idle_Start (Message *msg);
  bool idle_Dtind (Message *msg);
  bool idle_Errind (Message *msg);
  bool idle_default (Message *msg);

  UdpSAP :: User down;
  UdpAppSAP :: Provider up;

private:

  InetAddr ownIP;
  InetAddr destIP;

  bool terminated;  
  Timer sendTimer;       // Sending timeout

  char ifname[256];          // inputfilename
  Byte wbuf[256];           // writebuffer

  void StartSending ();      // Starts datasending 
  void StopSending ();       // Stops datasending

  void Init();

  Sint16 loops;
  Uint16 framesz;
  char *ifilename;
  char *ofilename;
  bool sent;
  
  std::ifstream ifile;
  std::ofstream ofile;
  bool tostdout;
  Frame buffer;
};

#endif  // UDPAPPTASK_H












