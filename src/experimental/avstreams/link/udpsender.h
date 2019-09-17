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

$Log: udpsender.h,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/08/21 16:51:25  ik
Changes to AVStreams.

Revision 1.1  2001/08/15 12:57:07  ik
Added some include files for AVStreams/link.

Revision 1.2  2001/06/08 16:45:45  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:57  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <fstream>
#include <string>
#include <sys/time.h>
#include <sys/socket.h>
#include <doors/sender.h>

// Assume 64kbytes for the maximum
#define MAXFRAME 65536

class UDPSender : public Sender
{
 public:
  UDPSender();
  ~UDPSender();

  /** UDP implementation for opening a connection to a receiver. This
      method opens a socket and does connect() for it. After that,
      it returns.
      @param address Address to a receiver
      @return true if link is opened
  */
  bool sndr_open(const std::string address);

  /** UDP implementation for closing the connection to a receiver.
      @return true if successfully closed
  */
  bool sndr_close();

  /** UDP implementation of sending of the next frame.
      @param buffer Data buffer
      @param true if successfully sent
  */
  bool sndr_send_frame(unsigned char* buffer, int size);

  inline bool sndr_is_open() { return opened_; }

protected:
  /** Descendant may implement this method to write an RTP header
      according to the stream. This method may just return zero if
      no RTP header is needed and written to the buffer and that
      is what the default implementation does.
      @param buffer buffer where to write the RTP header
      @return number of bytes written to buffer (offset to data)
  */
  virtual int write_rtp_header(unsigned char* buffer) { return 0; }

private:
  int port_;
  std::string address_;
  int socket_;
  int opened_;
  socklen_t _lenp;
  struct sockaddr* _saptr;
  unsigned short _counter;
  struct timeval lasttime_;

  bool wait_for_sendtime(unsigned int usecs);

  int mcast_join(int sockfd, const struct sockaddr *sa, socklen_t salen);
  int mcast_leave(int sockfd, const struct sockaddr *sa, socklen_t salen);
  int mcast_set_loop(int sockfd, int onoff);

};

#endif
