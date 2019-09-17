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


#include <doors/doorsconf.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/select.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <doors/tcpreceiver.h>

#define ASSERT(param) assert((param))

TCPReceiver::TCPReceiver()
  : opened_(false), port_(0), buffer_size_(0), fd_(-1)
{ 
  buffer_ = new unsigned char[RCVR_MAX_BUFFER_SIZE];
}

TCPReceiver::~TCPReceiver()
{
  delete [] buffer_;
  rcvr_close();
}
  
int 
TCPReceiver::rcvr_get_buffer(unsigned char*& buffer, int size, 
			     int offset)
{
  // If closed or some error, return error
  if ((sockstream_.eof()) || (!sockstream_.good())) {
    return -1;
  }
  // If requesting too big a buffer, return error
  if (size + offset > RCVR_MAX_BUFFER_SIZE) {
    return -1;
  }
  // Set return parameter to point to our buffer
  buffer = buffer_;
  // Start writing to offset
  char* p = (char*)&buffer[offset];
  sockstream_.read(p, size);
  return sockstream_.gcount();
}

bool
TCPReceiver::open_link()
{
  struct hostent *hp;
  socklen_t clilen;
  struct sockaddr_in cliaddr;
  struct sockaddr_in servaddr;
  int listenfd;

  opened_ = true;

  // Get listen socket fd. Third parameter is protocol and zero means
  // choose the protocol automatically (TCP).
  listenfd = ::socket(AF_INET, SOCK_STREAM, 0);

  ::memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port_);
  if (address_.length() == 0) {
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

  // Bind listen file descriptor to server's address
  ::bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

  // Wait for connection
  if (::listen(listenfd, 5) < 0) {
    return false;
  }

  std::cerr << "listen: " << listenfd << std::endl;

  // Accept connection and get the file descriptor for it
  clilen = sizeof(cliaddr);
  fd_ = ::accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);

  std::cerr << "accept from ";
  std::cerr << ((cliaddr.sin_addr.s_addr >> 0) & 255) << ".";
  std::cerr << ((cliaddr.sin_addr.s_addr >> 8) & 255) << ".";
  std::cerr << ((cliaddr.sin_addr.s_addr >> 16) & 255) << ".";
  std::cerr << ((cliaddr.sin_addr.s_addr >> 24) & 255) << std::endl;

  // Close listen file descriptor
  ::close(listenfd);

  // Attach socket file descriptor to our stream object
  // ARGH! Solaris doesn't seem to have attach()?? 
  //  sockstream_.attach(fd_);

  return true;
}

bool
TCPReceiver::rcvr_listen(std::string address)
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

  // Allocate space for receiving buffer
  // NOT in Solaris!
#if 0
  buffer_size_ = TCPRCVR_DEFAULT_BUFFER;
  char* buffer = new char[buffer_size_];
  ASSERT(buffer != NULL);
  sockstream_.setbuf(buffer, buffer_size_);
#endif

  return open_link();
}

bool
TCPReceiver::rcvr_is_open()
{
  return opened_;
}

int 
TCPReceiver::rcvr_get_port()
{
  if (!opened_) {
    return 0;
  }
  return (int)port_;
}

std::string
TCPReceiver::rcvr_get_address()
{
  if (!opened_) {
    return (std::string)"";
  }
  return address_;
}

bool
TCPReceiver::rcvr_close()
{
  if (!opened_) {
    return false;
  }

  sockstream_.close();
  if (fd_ > 0) {
    ::close(fd_);
    fd_ = -1;
  }

  return true;
}

