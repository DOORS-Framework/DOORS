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

$Log: udpapptask.C,v $
Revision 1.8  2003/12/16 12:59:29  bilhanan
bugfix for sending file multiple times.

Revision 1.7  2003/08/11 23:40:31  bilhanan
const char* in constructor -> std::string

Revision 1.6  2003/05/15 19:57:30  bilhanan
std namespace prefix added.

Revision 1.5  2003/05/15 18:08:01  bilhanan
fixes for std c++ headers

Revision 1.4  2002/07/23 14:28:54  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.3  2002/06/18 19:12:08  ik
File openness checks.

Revision 1.2  2002/06/17 14:06:17  ik
Start using system rand(). Buggy file reading flag.

Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.4  2002/02/25 17:06:06  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.

Revision 1.3  2001/12/11 12:29:40  ik
Initial DEMON commit.

Revision 1.2  2001/06/08 16:46:54  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.6  2001/06/08 08:40:05  bilhanan
Full licence clause added into doors 0.1


*/
//
//===========================================================================
//File:     apptask.C
//Date:     22.12.1998
//Desc:     State machine functions for loadgenerator application part.
//
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================
#define WITH_UI

#include <string>
#include <stdlib.h>
#include <doors/hsi.h>

#include <doors/udpapptask.h>
#include <doors/udpappsm.h>
#include <arpa/inet.h>
#include <iostream>

UdpAppTask :: UdpAppTask (std::string n, Scheduler *s, UdpAppTaskSM *sm, InetAddr *own, InetAddr *dest, Uint16 fs, int l, char *ifn, char *ofn)
: PTask (n, s, sm),
  up(this),
  down (this),
  terminated (false),
  sendTimer (this, OTime (10, 10000)),
  framesz(fs), ifilename(ifn), ofilename(ofn),
  sent(false), loops(l), tostdout(false)
{
#ifdef WITH_UI
  STATIC_SYMBOL (down, this);
  STATIC_SYMBOL (ownIP, this);
  STATIC_SYMBOL (destIP, this);
#endif
  ownIP.set(own->getPortNumber(), own->getIpAddress());
  destIP.set(dest->getPortNumber(), dest->getIpAddress());

  // "Initialise" random generator

  unsigned int *randptr = (unsigned int *)dest;
  srand((unsigned int) *randptr);

  if (fs != 0)
    StartSending();

  if (ifn != NULL) {
    ifile.open(ifn, std::ios::in);
  }
  if (ofn != NULL) {
    if (strcmp(ofn, "-") == 0) {
      tostdout = true;
      std::cout << "Writing to standard output" << std::endl;
    } else {
      ofile.open(ofn, std::ios::out|std::ios::trunc);
      std::cout << "Writing to file '" << ofn << "'." << std::endl;
    }
  }
}

UdpAppTask :: ~UdpAppTask (void) {
  if (ifile.is_open())
    ifile.close();
  if (ofile.is_open())
    ofile.close();
#ifdef WITH_UI
  DELETE_SYMBOL (this); 
#endif
}

//===Idle======Idle======Idle======Idle======Idle======Idle======Idle===

bool UdpAppTask :: idle_Start (Message *msg){
  UdpAppSAP::Start *start = (UdpAppSAP::Start *) msg;
  destIP.set(start->destaddr.getPortNumber(), start->destaddr.getIpAddress());
  StartSending();
  return false;}

bool UdpAppTask :: idle_default (Message *msg){
  return false;}

bool UdpAppTask :: idle_Dtind (Message *msg){
  UdpSAP::Dtind *da = (UdpSAP::Dtind *) msg;
  if (ofile.is_open()) {
    ofile.write((const char*)((Byte*)da->data), da->data.length());
    ofile.flush();
  } else if (tostdout) {
    std::cout.write((const char*)((Byte*)da->data), da->data.length());
    std::cout.flush();
  }
  return false;}

bool UdpAppTask :: idle_Errind (Message *msg){
  UdpSAP::Errind *err = (UdpSAP::Errind *) msg;
  UdpAppSAP::Infomsg *info = (UdpAppSAP::Infomsg *) up.create(UdpAppSAP::infomsg);

  char buf[500];
  sprintf(buf, "Error: %s (%d).", (char*)(Byte*)err->data, err->code);
  info->info.putLast((Byte*)buf, strlen(buf));
  up.putMessage(info);

  return false;}

bool UdpAppTask :: idle_timeout (Message *msg){
  if (sent) {
    terminated = true;
    return(false);
  }
  int i;
  char ch;
  UdpSAP::Dtreq *da = (UdpSAP::Dtreq *) down.create (UdpSAP::dtreq);
  da->dest = destIP;
  if (ifile.is_open()) {
    for (i = 0; (i < framesz) && ((ch = ifile.get()) != EOF); i++) 
      da->data.putLast((Byte)ch);
    down.putMessage (da);
    if (ch == EOF) {
      StopSending();
    }
  } else {
    for(int i = 0; i < framesz; i++){ da->data.putLast((Byte)(rand()&0xff));}
    TRACE_MSG(da);
    down.putMessage (da);
    StopSending();
  }
  StartSending();
  return false;
}

//========================================================================

void UdpAppTask :: StartSending (void) {
  sendTimer.start();
}

void UdpAppTask :: StopSending (void) {
  if (loops == 1) {
    sent = true;
    return;
  } 
  if (loops != LOOP_FOREVER) 
    --loops;
  if (ifile.is_open()){
    //    ifile.seekg(0, std::ios_base::beg); <-- does not work as expected, postype not set. -Bill
    ifile.close();                               //   instead, close and re-open. kludgy fix for now. 
    ifile.open(ifilename, std::ios::in);
  }
}








