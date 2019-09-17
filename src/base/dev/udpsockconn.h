//Editor-Info: -*- C++ -*-

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

$Log: udpsockconn.h,v $
Revision 1.8  2003/04/09 14:41:20  ik
Multicast bugfixes.

Revision 1.7  2002/08/20 12:32:52  kalliosa
Added JoinMcast to Udp6Sap

Revision 1.6  2002/07/23 14:25:17  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.5  2002/06/19 16:19:20  ik
Multicast join moved to setDest.

Revision 1.4  2002/06/17 13:28:54  ik
Useless code removed. bind() fixes.

Revision 1.3  2002/06/14 14:55:32  ik
Multicast options moved to constructor.

Revision 1.2  2002/06/13 13:53:02  ik
Support for multicast join etc.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:07  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:29  bilhanan
Full licence clause added into doors 0.1


*/

 
//===========================================================================
//File:     udpsockconn.h
//Subject:  UDP socket device for DOORS
//Desc:     This file declares functions of udp (or SOCK_DGRAM) type sockets.
//          UdpSockConn is inherited from SocketConnectionAC and 
//          SockHelper. The device informs different event to the
//          controlling task by sending message with proper parameters. 
//          UdpSockConn is read-write device.
//===========================================================================

#ifndef UDPSOCKCONN_H
#define UDPSOCKCONN_H

#include <doors/hsi.h>
#include <doors/soconnac.h>
#include <doors/buffer.h>
#include <doors/sockhelper.h>

class UdpSockConn : public SocketConnectionAC, public SockHelper {
public:
  
  UdpSockConn (IoHandler *io, EventTask *t,
	       unsigned char ttl = 127, bool loopback = false);
  // Optional arguments ttl and loopback are only applicable
  // when connecting to a multicast address
  UdpSockConn (IoHandler *io, EventTask *t, Address *a,
	       unsigned char ttl = 127, bool loopback = false);
  ~UdpSockConn (void);

  // Optional arguments ttl and loopback are only applicable
  // when connecting to a multicast address
  bool connect (Address *a);
  bool connect (unsigned short port);

  virtual bool close (void);
  
  virtual int getFd (void) const;
  
  void callbackWrite (void);
  void callbackRead (void);
  
  bool setDest(InetAddr a);
  InetAddr getSource(void);

  bool isMulticast();

  int mcast_join( const struct sockaddr_in *addr);
  int mcast_leave( const struct sockaddr_in *addr);
   
protected:
  int mcast_join(int sockfd, const struct sockaddr_in *addr);
  int mcast_leave(int sockfd, const struct sockaddr_in *addr);
  int set_ttl(int sockfd, unsigned char ttl);
  int mcast_set_ttl(int sockfd, unsigned char ttl);
  int mcast_set_loop(int sockfd, bool onoff);

  int fd_;
  struct sockaddr_in to_; 
  struct sockaddr_in old_to_; 
  struct sockaddr_in from_;

private:
  unsigned char ttl_;
  bool loopback_;
};


#endif
