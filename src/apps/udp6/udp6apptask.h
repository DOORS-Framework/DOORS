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

$Log: udp6apptask.h,v $
Revision 1.7  2003/08/11 23:42:31  bilhanan
const char* in constructor -> std::string

Revision 1.6  2003/05/16 13:30:20  bilhanan
Uint16 type changed to Sint16 to prevent comparison mismatch

Revision 1.5  2003/05/15 20:00:10  bilhanan
std namespace prefix added.

Revision 1.4  2003/05/15 18:08:02  bilhanan
fixes for std c++ headers

Revision 1.3  2002/07/23 14:28:55  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.2  2002/06/17 14:06:34  ik
Start using system rand(). Buggy file reading flag.

Revision 1.1  2002/06/14 22:04:06  ik
Development. Initial commit for udp6 application.


*/
//
//===========================================================================
//File:     udp6apptask.h
//Date:     15.6.2002
//Desc:     Class definition for udp6demo application task
//
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#ifndef UDP6APPTASK_H
#define UDP6APPTASK_H

#include <string>
#include <doors/ptb.h>
#include <doors/udp6app.h>
#include <doors/udp6appsm.h>
#include <doors/udp6.h>

#include <doors/frame.h>
#include <iostream>
#include <fstream>

#define LOOP_FOREVER -1

class Udp6AppTaskSM;

class Udp6AppTask : public PTask {
public:
  Udp6AppTask (std::string n, Scheduler *s, Udp6AppTaskSM *sm, InetAddr6 *own, InetAddr6 *dest, Uint16 framesz, int loops, char *ifilename, char *ofilename);
  virtual ~Udp6AppTask (void);

  bool isTerminated (void) { return terminated; };
 
  bool idle_timeout (Message *msg);
  bool idle_Start (Message *msg);
  bool idle_Dtind (Message *msg);
  bool idle_Errind (Message *msg);
  bool idle_default (Message *msg);

  Udp6SAP :: User down;
  Udp6AppSAP :: Provider up;

private:

  InetAddr6 ownIP;
  InetAddr6 destIP;

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

#endif  // UDP6APPTASK_H












