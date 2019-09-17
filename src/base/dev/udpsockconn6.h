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

$Log: udpsockconn6.h,v $
Revision 1.7  2003/04/09 14:41:21  ik
Multicast bugfixes.

Revision 1.6  2002/08/20 12:32:52  kalliosa
Added JoinMcast to Udp6Sap

Revision 1.5  2002/07/23 14:25:17  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.4  2002/06/19 16:19:01  ik
Multicast join moved to setDest().

Revision 1.3  2002/06/17 13:28:54  ik
Useless code removed. bind() fixes.

Revision 1.2  2002/06/14 20:56:40  ik
Support for hoplimit and loopback.

Revision 1.1  2002/06/06 12:19:12  ik
IPv6 UdpSockConn.


*/

 
//===========================================================================
//File:     udpsockconn6.h
//Subject:  IPv6 UDP socket device for DOORS
//Desc:     This file declares functions of udp (or SOCK_DGRAM) type sockets.
//          UdpSockConn is inherited from SocketConnectionAC and 
//          SockHelper. The device informs different event to the
//          controlling task by sending message with proper parameters. 
//          UdpSockConn is read-write device.
//          Modified from UdpSockConn to support IPv6.
//===========================================================================

#ifndef UDPSOCKCONN6_H
#define UDPSOCKCONN6_H

#include <doors/hsi.h>
#include <doors/soconnac.h>
#include <doors/buffer.h>
#include <doors/sockhelper.h>
#include <doors/inetaddr6.h>

class UdpSockConn6 : public SocketConnectionAC, public SockHelper {
public:
  
  UdpSockConn6 (IoHandler *io, EventTask *t,
		unsigned int hoplimit = 127, bool loopback = false);
  UdpSockConn6 (IoHandler *io, EventTask *t, Address *a,
		unsigned int hoplimit = 127, bool loopback = false);
  ~UdpSockConn6 (void);
  
  bool connect (Address *a);
  bool connect (unsigned short port);
  virtual bool close (void);
  
  virtual int getFd (void) const;
  
  void callbackWrite (void);
  void callbackRead (void);
  
  bool setDest(InetAddr6 a);
  InetAddr6* getSource(void);
  bool isMulticast();

  int mcast_join( const struct sockaddr_in6 *addr);
  int mcast_leave( const struct sockaddr_in6 *addr);
   
protected:
  int mcast_set_hoplimit(int sockfd, unsigned int hoplimit);

  /** Set multicast packets to loop back or not
      @param sockfd Socket file descriptor to a multicast address
      @param onoff If nonzero, looping is enabled. Otherwise it is disabled.
      @return Zero if the job is successfully done
  */
  int mcast_set_loop(int sockfd, int onoff);

  /** Join to a multicast group
      @param sockfd Socket file descriptor to a multicast address
      @param addr Multicast address
      @return Zero if the job is successfully done
  */
  int mcast_join(int sockfd, const struct sockaddr_in6 *addr);

  /** Leave a multicast group
      @param sockfd Socket file descriptor to a multicast address
      @param addr Multicast address
      @return Zero if the job is successfully done
  */
  int mcast_leave(int sockfd, const struct sockaddr_in6 *addr);

protected:
  
  struct sockaddr_in6 to_; 
  struct sockaddr_in6 from_;
  struct sockaddr_in6 old_to_;

private:
  unsigned int hoplimit_;
  bool loopback_;
};


#endif
