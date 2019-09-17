//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
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

$Log: udpsender.C,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/08/31 13:59:02  ik
AVStreams changes and modifications to configure.in accordingly.

Revision 1.1  2001/08/14 15:29:20  ik
TCP- and UDP-links are partly rewritten and structure is changed.

Revision 1.2  2001/06/08 16:46:48  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:57  bilhanan
Full licence clause added into doors 0.1


*/

#include <doors/doorsconf.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <doors/udpsender.h>

#define ASSERT(param) assert((param))

#ifndef timersub
#define timersub(a, b, result)                                                \
  do {                                                                        \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;                             \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;                          \
    if ((result)->tv_usec < 0) {                                              \
      --(result)->tv_sec;                                                     \
      (result)->tv_usec += 1000000;                                           \
    }                                                                         \
  } while (0)
#endif
#ifndef timeradd
#define timeradd(a, b, result)                                                \
  do {                                                                        \
    (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;                             \
    (result)->tv_usec = (a)->tv_usec + (b)->tv_usec;                          \
    if ((result)->tv_usec >= 1000000)                                         \
      {                                                                       \
        ++(result)->tv_sec;                                                   \
        (result)->tv_usec -= 1000000;                                         \
      }                                                                       \
  } while (0)
#endif

UDPSender::UDPSender()
  : opened_(0), port_(0), socket_(-1)
{ 
  lasttime_.tv_sec = 0;
  lasttime_.tv_usec = 0;
  srand(time(NULL));
}

UDPSender::~UDPSender()
{
  sndr_close();
}

int 
UDPSender::mcast_set_loop(int sockfd, int onoff)
{
  unsigned char flag;
  flag = onoff;
  return (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &flag, 
		       sizeof(flag)));
}

int 
UDPSender::mcast_join(int sockfd,
		      const struct sockaddr *sa, socklen_t salen)
{
  switch(sa->sa_family) {
  case AF_INET : {
    struct ip_mreq mreq;
    struct ifreq ifreq;
    
    memcpy(&mreq.imr_multiaddr, 
	   &((struct sockaddr_in *) sa)->sin_addr,
	   sizeof(struct in_addr));

    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    return(setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		      &mreq, sizeof(mreq)));
  }

  default :
    return(-1);
  }
}

int 
UDPSender::mcast_leave(int sockfd,
		       const struct sockaddr *sa, socklen_t salen)
{
  switch(sa->sa_family) {
  case AF_INET : {
    struct ip_mreq mreq;
    struct ifreq ifreq;
    
    memcpy(&mreq.imr_multiaddr, 
	   &((struct sockaddr_in *) sa)->sin_addr,
	   sizeof(struct in_addr));

    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    return(setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		      &mreq, sizeof(mreq)));
  }

  default :
    return(-1);
  }
}

bool 
UDPSender::sndr_open(const std::string address)
{
  struct hostent *hp;
  struct addrinfo hints, *res;
  struct sockaddr_in servaddr;
  size_t p;
  int port;
  char* strport;

  // Address can't be zero length
  int len = address.length();
  if (len == 0) {
    return false;
  }
  p = address.find(":");
  // There should be a ':' between address and port
  if (p == std::string::npos) {
    return false;
  }
  // If there is no port number portion, return error
  if (p + 1 >= len) {
    return false;
  }

  // Close first in case we're still open
  sndr_close();

  // Save address and port
  port_ = atoi(address.substr(p + 1, std::string::npos).c_str());
  address_ = address.substr(0, p);
  strport = (char*)address.substr(p + 1, std::string::npos).c_str();

  std::cerr << "Address: " << address_ << ":" << port_ << std::endl;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ((getaddrinfo(address_.c_str(), strport, &hints, &res)) != 0) {
    return false;
  }

  do {
    socket_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_ >= 0)
      break;
  } while ((res = res->ai_next) != NULL);

  if (res == NULL) {
    return false;
  }

  _saptr = (struct sockaddr*)malloc(res->ai_addrlen);
  memcpy(_saptr, res->ai_addr, res->ai_addrlen);
  _lenp = res->ai_addrlen;

  mcast_join(socket_, _saptr, _lenp);
  mcast_set_loop(socket_, 1);

  // Send MORO to receiver to test the link
  // If all characters are sent, we assume that the link works
  if (sendto(socket_, "MORO", 4, 0, _saptr, _lenp) == 4) {
    std::cout << "Key sent." << std::endl;
    opened_ = 1;
    return true;
  }

  return false;
}

bool
UDPSender::wait_for_sendtime(unsigned int usecs)
{
  struct timeval now, subtime, futuretime;
  struct timeval zerotime = {0, 0};
  bool in_sync = false;

  if (lasttime_.tv_sec == 0)
    ::gettimeofday(&lasttime_, 0);

  ::gettimeofday(&now, 0);
  // Convert useconds to struct timeval
  subtime.tv_sec  = usecs / 1000000;
  subtime.tv_usec = usecs % 1000000;
  // Resolve the timestamp in the future adding wanted timeout
  // to the last timestamp.
  timeradd(&lasttime_, &subtime, &futuretime);
  // Resolve time that we need to wait
  timersub(&futuretime, &now, &subtime);
  // If sending time is in the future, have a nap
  if (timercmp(&subtime,&zerotime,>)) {
    select(0, NULL, NULL, NULL, &subtime);
    in_sync = true;
  }
  lasttime_.tv_sec = futuretime.tv_sec;
  lasttime_.tv_usec = futuretime.tv_usec;
  return in_sync;
}

bool
UDPSender::sndr_send_frame(unsigned char* buffer, int size)
{
  if (socket_ < 0) {
    return false;
  }

  unsigned char buf[MAXFRAME];

  // Wait for sending time. Timeout is calculated from the latest
  // frame to be sent and so sndr_get_frame must be called before.
  bool in_sync = false;
  // wait_for_sendtime() returns true if the stream is in sync with
  // real time
  in_sync = wait_for_sendtime(sndr_get_timeout());
  int offset = write_rtp_header(buf);
  memcpy(buf + offset, buffer, size);
  sendto(socket_, buf, size + offset, 0, _saptr, _lenp);

  return in_sync;
}

bool
UDPSender::sndr_close()
{
  if (!opened_) {
    return false;
  }
  mcast_leave(socket_, _saptr, _lenp);
  if (socket_ > 0) {
    ::close(socket_);
    socket_ = -1;
  }
  opened_ = 0;
  return true;
}
