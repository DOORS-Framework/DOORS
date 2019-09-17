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

$Log: udptask.h,v $
Revision 1.4  2003/08/11 23:58:54  bilhanan
const char* in constructor -> std::string

Revision 1.3  2002/07/23 16:38:36  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.2  2002/06/14 14:57:00  ik
Multicast options to the constructor.

Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.3  2002/02/25 17:06:05  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.

Revision 1.2  2001/06/08 16:46:07  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:28  bilhanan
Full licence clause added into doors 0.1


*/

 
//===========================================================================
//File:     udptask.h
//Date:     14.09.1998
//Subject:  UDP socket task for DOORS
//Desc:     UdpTask controls UdpSocketConnectionOS -device defined in
//          files usockcos.c and usockcos.h and provides 
//          UdpSap -services to user. 
//Author:   Janne Makihonka, TTKK/DMI
//===========================================================================


#ifndef UDPTASK_H
#define UDPTASK_H

#include <string>
#include <doors/udp.h>
#include <doors/util.h>
#include <doors/core.h>
#include <doors/strdev.h>
#include <doors/devif.h>
#include <doors/sockmsg.h>
#include <doors/udpsockconn.h>
#include <doors/inetaddr.h>


#define NO_SYMBOLS 0
#define NO_SYMBOLS_UDP 0

class UdpTask: public EventTask {
public:
  // Optional arguments ttl and loopback are only applicable
  // when connecting to a multicast address
  UdpTask (std::string n, Scheduler *s, IoHandler *io, Address *a1,
	   unsigned char ttl = 127, bool loopback = false);
  ~UdpTask (void);
 
  virtual bool execute (EventType type, Message *msg);
 
  UdpSAP :: Provider up;
  DevIf :: User down;
 
 protected:
 
  UdpSockConn udpdevice;
  Frame buffer;
};
 
#endif
