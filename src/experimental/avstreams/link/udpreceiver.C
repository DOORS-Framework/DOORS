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

$Log: udpreceiver.C,v $
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
#include <string>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <doors/udpreceiver.h>

#define ASSERT(param) assert((param))

UDPReceiver::UDPReceiver()
  : opened_(0), port_(0), buffer_size_(0), socket_(-1),
    is_multicast_(false), size_(0), bytes_read_(0)
{ 
  buffer_ = new unsigned char[RCVR_MAX_BUFFER_SIZE];
}

UDPReceiver::UDPReceiver(bool is_multicast)
  : opened_(0), port_(0), buffer_size_(0), socket_(-1),
    is_multicast_(is_multicast)
{ 
  buffer_ = new unsigned char[RCVR_MAX_BUFFER_SIZE];
}

UDPReceiver::~UDPReceiver()
{
  delete [] buffer_;
  rcvr_close();
}

int 
UDPReceiver::mcast_set_loop(int sockfd, int onoff)
{
  unsigned char flag;
  flag = onoff;
  return (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &flag, 
		       sizeof(flag)));
}

int 
UDPReceiver::mcast_join(int sockfd,
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

void
UDPReceiver::fill_buffer()
{
  struct sockaddr *safrom = NULL;
  socklen_t len;
  fd_set fdset;
  FD_ZERO(&fdset);
  FD_SET(socket_, &fdset);
  select(socket_+1, &fdset, NULL, NULL, NULL);
  size_ = recvfrom(socket_, buffer_, RCVR_MAX_BUFFER_SIZE, 0, safrom, &len);
  bytes_read_ = 0;
  if (size_ > 0) {
    int offset = parse_rtp_header(buffer_, size_);
    bytes_read_ = offset; // Advance offset beyond RTP header
  }
}

int
UDPReceiver::rcvr_get_buffer(unsigned char*& buffer, int size, 
			     int offset)
{
  // If not connected, return error
  if (!opened_) {
    return -1;
  }
  // If requesting too big a buffer, return error
  if (size + offset > RCVR_MAX_BUFFER_SIZE) {
    return -1;
  }
  // Set return parameter to point to our buffer
  buffer = buffer_;
  // Start writing to offset
  unsigned char* p = &buffer[offset];
  int bytes = 0;
  do {
    if (bytes_read_ >= size_) {
      fill_buffer();
    }
    // If size is greater than bytes currently available, set b to
    // hold number of bytes we currently have and otherwise all the
    // bytes are available. This is required when size is bigger than
    // currently left in the buffer. In that case we copy to result
    // buffer what is left in the read buffer buffer_ and call fill_buffer()
    // again.
    int b = (size > (size_ - bytes_read_)) ? size_ - bytes_read_ : size;
    memcpy(buffer+(offset+bytes), buffer_+bytes_read_, size - bytes);
    bytes_read_ += b;
    bytes += b;
  } while (size > bytes);
  return bytes;
}

bool
UDPReceiver::open_link()
{
  struct hostent *hp;
  socklen_t clilen;
  struct sockaddr cliaddr;
  struct sockaddr_in servaddr;

  opened_ = 1;

  socket_ = socket(AF_INET, SOCK_DGRAM, 0);
  
  ::memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port_);
  if (address_.empty()) {
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    std::cerr << "Any source address allowed on port " << port_ << std::endl;
  } else {
    /* Convert that to an IP address. */
    if ((hp = gethostbyname(address_.c_str())) == (struct hostent *)NULL) {
      return false;
    }
    ::memcpy((char *) &servaddr.sin_addr, hp->h_addr_list[0], hp->h_length);
    std::cerr << "Address: " << address_ << ", port: " << port_ << std::endl;
    std::cerr << "IP-address: " << ((servaddr.sin_addr.s_addr >> 0) & 255) << ".";
    std::cerr << ((servaddr.sin_addr.s_addr >> 8) & 255) << ".";
    std::cerr << ((servaddr.sin_addr.s_addr >> 16) & 255) << ".";
    std::cerr << ((servaddr.sin_addr.s_addr >> 24) & 255) << std::endl;
  }

  mcast_join(socket_, (struct sockaddr *) &servaddr, sizeof(servaddr));
  mcast_set_loop(socket_, 1);

  const int on = 1;
  setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  // Bind listen file descriptor to server's address
  ::bind(socket_, (struct sockaddr *) &servaddr, sizeof(servaddr));

  char buf[64];
  int bytes = recvfrom(socket_, buf, 64, 0, &cliaddr, &clilen);

#if 0
  if (strncmp(buf, "MORO", 4) == 0) {
    std::cerr << "Key accepted." << std::endl;
    return true;
  }

  return false;
#endif
  return true;
}

bool
UDPReceiver::rcvr_listen(std::string address)
{
  int port = 0;

  if (opened_) {
    return false;
  }
  // Address can't be zero length
  if (address.length() == 0) {
    return false;
  }
  int pos = address.find(":");
  // There should be a ':' between address and port
  if (pos == std::string::npos) {
    return false;
  }
  int size = address.size();
  // If there is no port number portion, return error
  if ((pos + 1) == size) {
    return false;
  }

  // Read port number
  port_ = (unsigned short)atoi(address.substr(pos + 1, size).c_str());

  // Read address
  if ((pos - 1) > 0) {
    address_ = address.substr(0, pos);
  } else {
    address_ = "";
  }

  return open_link();
}

bool
UDPReceiver::rcvr_is_open()
{
  return opened_ ? true : false;
}

int 
UDPReceiver::rcvr_get_port()
{
  if (!opened_) {
    return 0;
  }
  return (int)port_;
}

std::string
UDPReceiver::rcvr_get_address()
{
  if (!opened_) {
    return "";
  }
  return address_;
}

bool
UDPReceiver::rcvr_close()
{
  if (!opened_) {
    return false;
  }

  if (socket_ > 0) {
    ::close(socket_);
    socket_ = -1;
  }

  return true;
}
