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

$Log: udpreceiver.h,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/08/21 16:51:25  ik
Changes to AVStreams.

Revision 1.1  2001/08/15 12:57:06  ik
Added some include files for AVStreams/link.

Revision 1.2  2001/06/08 16:45:45  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:57  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <string>
#include <sys/socket.h>
#include <doors/receiver.h>

#define UDPRCVR_DEFAULT_FRAME     1024
#define UDPRCVR_DEFAULT_BUFFER    (64 * 1024)
#define UDPRCVR_DEFAULT_RETRIES   30

#define UDP_BUFFER_SIZE           16384

class UDPReceiver : public Receiver
{
 public:
  UDPReceiver();
  UDPReceiver(bool is_multicast);
  ~UDPReceiver();

  int rcvr_get_buffer(unsigned char*& buffer, int size, 
		      int offset = 0);

  /** UDP implementation of listen() virtual method first declared
      in Receiver class.
      @address Listening address. Examples: "address:5678" or
      ":5678". The first one means that connections should only
      be allowed from host 'address' using port number 5678. The
      second one means that the host can be any but port number
      must be 5678. No other forms for this address string are
      allowed. 
      @return true if the connection is successfully opened
  */
  bool rcvr_listen(std::string address);

  /** TCP implementation of open connection query method. 
      @param handle Handle to a connection
      @return true if connection is opened
  */
  bool rcvr_is_open();

  /** TCP implementation of port number query method. 
      @return Listen port number of the connection
  */
  int rcvr_get_port();

  /** TCP implementation of address query method.
      @return Listen address of the connection
  */
  std::string rcvr_get_address();

  bool rcvr_close();

  /** Virtual method to parse RTP header information. Default method
      returns zero which means no RTP header found. Descendants may
      overload this method to fit their special needs.
      @param buffer data buffer to examine
      @param size data buffer size
      @return RTP header size in bytes
  */
  virtual int parse_rtp_header(unsigned char* buffer, int size) { return 0; }

 private:
  int buffer_size_;
  int opened_;
  std::string address_;
  int port_;
  int socket_;
  bool is_multicast_;
  unsigned char* buffer_;
  int size_;
  int bytes_read_;

  bool open_link();
  void fill_buffer();

  int mcast_join(int sockfd, const struct sockaddr *sa, socklen_t salen);
  int mcast_set_loop(int sockfd, int onoff);

};

#endif
