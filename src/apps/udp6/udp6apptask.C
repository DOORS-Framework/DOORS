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

$Log: udp6apptask.C,v $
Revision 1.7  2003/12/16 12:59:29  bilhanan
bugfix for sending file multiple times.

Revision 1.6  2003/08/11 23:42:31  bilhanan
const char* in constructor -> std::string

Revision 1.5  2003/05/15 20:00:10  bilhanan
std namespace prefix added.

Revision 1.4  2002/07/23 14:28:55  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.3  2002/06/18 19:14:07  ik
File openness checks.

Revision 1.2  2002/06/17 14:06:34  ik
Start using system rand(). Buggy file reading flag.

Revision 1.1  2002/06/14 22:04:06  ik
Development. Initial commit for udp6 application.


*/
//
//===========================================================================
//File:     udp6apptask.C
//Date:     15.6.2002
//Desc:     Implementation for udp6demo application task
//
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================
//#define WITH_UI

#include <string>
#include <stdlib.h>
#include <doors/hsi.h>

#include <doors/udp6apptask.h>
#include <doors/udp6appsm.h>
#include <arpa/inet.h>
#include <iostream>

Udp6AppTask :: Udp6AppTask (std::string n, Scheduler *s, Udp6AppTaskSM *sm, InetAddr6 *own, InetAddr6 *dest, Uint16 fs, int l, char *ifn, char *ofn)
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
  ownIP.set(*own);
  destIP.set(*dest);

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

Udp6AppTask :: ~Udp6AppTask (void) {
  if (ifile.is_open())
    ifile.close();
  if (ofile.is_open())
    ofile.close();
#ifdef WITH_UI
  DELETE_SYMBOL (this); 
#endif
}

//===Idle======Idle======Idle======Idle======Idle======Idle======Idle===

bool Udp6AppTask :: idle_Start (Message *msg){
  Udp6AppSAP::Start *start = (Udp6AppSAP::Start *) msg;
  destIP.set(start->destaddr);
  StartSending();
  return false;}

bool Udp6AppTask :: idle_default (Message *msg){
  return false;}

bool Udp6AppTask :: idle_Dtind (Message *msg){
  Udp6SAP::Dtind *da = (Udp6SAP::Dtind *) msg;
  if (ofile.is_open()) {
    ofile.write((const char*)((Byte*)da->data), da->data.length());
    ofile.flush();
  } else if (tostdout) {
    std::cout.write((const char*)((Byte*)da->data), da->data.length());
    std::cout.flush();
  }
  return false;}

bool Udp6AppTask :: idle_Errind (Message *msg){
  Udp6SAP::Errind *err = (Udp6SAP::Errind *) msg;
  Udp6AppSAP::Infomsg *info = (Udp6AppSAP::Infomsg *) up.create(Udp6AppSAP::infomsg);

  char buf[500];
  sprintf(buf, "Error: %s (%d).", (char*)(Byte*)err->data, err->code);
  info->info.putLast((Byte*)buf, strlen(buf));
  up.putMessage(info);

  return false;}

bool Udp6AppTask :: idle_timeout (Message *msg){
  if (sent) {
    terminated = true;
    return(false);
  }
  int i;
  char ch;
  Udp6SAP::Dtreq *da = (Udp6SAP::Dtreq *) down.create (Udp6SAP::dtreq);
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
    //    TRACE_MSG(da);
    down.putMessage (da);
    StopSending();
  }
  StartSending();
  return false;
}

//========================================================================

void Udp6AppTask :: StartSending (void) {
  sendTimer.start();
}

void Udp6AppTask :: StopSending (void) {
  if (loops == 1) {
    sent = true;
    return;
  } 
  if (loops != LOOP_FOREVER) 
    --loops;
  if (ifile.is_open()) {
    //    ifile.seekg(0, std::ios_base::beg); <-- does not work as expected, postype not set. -Bill
    ifile.close();                               //   instead, close and re-open. kludgy fix for now. 
    ifile.open(ifilename, std::ios::in);
  }
}








