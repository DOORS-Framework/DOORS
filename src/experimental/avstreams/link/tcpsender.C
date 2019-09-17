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

$Log: tcpsender.C,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/08/31 13:59:01  ik
AVStreams changes and modifications to configure.in accordingly.

Revision 1.1  2001/08/14 15:29:20  ik
TCP- and UDP-links are partly rewritten and structure is changed.

Revision 1.2  2001/06/08 16:46:48  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:56  bilhanan
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
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <doors/tcpsender.h>

#define ASSERT(param) assert((param))

TCPSender::TCPSender()
  : opened_(0), port_(0), socket_(-1)
{ 
}

TCPSender::~TCPSender()
{
  sndr_close();
}

bool
TCPSender::sndr_open(const std::string address)
{
  struct hostent *hp;
  struct sockaddr_in servaddr;
  size_t p;
  int port;

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

  // Create a socket
  if ((socket_ = ::socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    return false;
  }

  // Convert address to IP-address
  if ((hp = ::gethostbyname(address_.c_str())) == (struct hostent *)NULL) {
    perror("gethostbyname");
    return false;
  }
  ::memcpy((char *) &servaddr.sin_addr, hp->h_addr_list[0], hp->h_length);
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port_);
  std::cerr << "Address: " << address_ << ", port: " << port_ << std::endl;
  std::cerr << "IP: " << ((servaddr.sin_addr.s_addr >> 0) & 255) << ".";
  std::cerr << ((servaddr.sin_addr.s_addr >> 8) & 255) << ".";
  std::cerr << ((servaddr.sin_addr.s_addr >> 16) & 255) << ".";
  std::cerr << ((servaddr.sin_addr.s_addr >> 24) & 255) << std::endl;

  // Connect
  int retries = 0;
  for ( ; ; ) {
    std::cerr << "Trying connection to " << address_ << std::endl;
    int result = ::connect(socket_, (struct sockaddr *) &servaddr, 
			   sizeof(servaddr));
    if (result == 0) {
      break;
    }
    if (result < 0) {
      if (++retries > 10) {
	perror("connect");
	return false;
      }
      ::sleep(1);
    }
  }

  std::cerr << "Connected!" << std::endl;

  // Attach socket to the stream
  // Solaris doesn't have attach()!
  //  sockstream_.attach(socket_);

  opened_ = 1;

  return true;
}

bool
TCPSender::sndr_send_frame(unsigned char* buffer, int size)
{
  if (!sockstream_.is_open()) {
    return false;
  }
  sockstream_.write((char*)buffer, size);
  return true;
}

inline bool
TCPSender::sndr_close()
{
  if (!opened_) {
    return false;
  }
  sockstream_.close();
  if (socket_ > 0) {
    ::close(socket_);
    socket_ = -1;
  }
  opened_ = 0;
  return true;
}





