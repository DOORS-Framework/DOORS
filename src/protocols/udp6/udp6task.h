//Editor-Info: -*- C++ -*-
//
//  Copyright 2000 
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

$Log: udp6task.h,v $
Revision 1.5  2003/08/12 00:00:33  bilhanan
const char* in constructor -> std::string

Revision 1.4  2002/07/23 16:38:36  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.3  2002/06/19 16:21:24  ik
Hoplimit and loopback code.

Revision 1.2  2002/06/06 21:53:14  ik
Fixes to IPv6 UDP.

Revision 1.1  2002/06/06 12:52:56  ik
Initial files for IPv6 UdpTask.


*/

 
//===========================================================================
//File:     udp6task.h
//Date:     6.6.2002
//Subject:  IPv6 UDP socket task for DOORS
//Desc:     IPv6 version of UdpTask
//Author:   Janne Mäkihonka, TTKK/DMI
//Author:   Ilkka Karvinen, TTKK/DMI (IPv6 modifications)
//===========================================================================


#ifndef UDP6TASK_H
#define UDP6TASK_H

#include <string>
#include <doors/udp6.h>
#include <doors/util.h>
#include <doors/core.h>
#include <doors/strdev.h>
#include <doors/devif.h>
#include <doors/sockmsg.h>
#include <doors/udpsockconn6.h>
#include <doors/inetaddr6.h>


#define NO_SYMBOLS 0
#define NO_SYMBOLS_UDP 0

class Udp6Task : public EventTask {
public:
  Udp6Task (std::string n, Scheduler *s, IoHandler *io, Address *a1,
	    unsigned int hoplimit = 127, bool loopback = false);
  ~Udp6Task (void);
 
  virtual bool execute (EventType type, Message *msg);
 
  Udp6SAP :: Provider up;
  DevIf :: User down;
 
 protected:
 
  UdpSockConn6 udpdevice;
  Frame buffer;
};
 
#endif
