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

$Log: tcpreceiver.h,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/08/21 16:51:25  ik
Changes to AVStreams.

Revision 1.1  2001/08/15 12:57:06  ik
Added some include files for AVStreams/link.

Revision 1.2  2001/06/08 16:45:44  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:56  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef TCPRECEIVER_H
#define TCPRECEIVER_H

#include <fstream>
#include <string>
#include <doors/receiver.h>

#define TCPRCVR_DEFAULT_FRAME     1024
#define TCPRCVR_DEFAULT_BUFFER    (64 * 1024)
#define TCPRCVR_DEFAULT_RETRIES   30

class TCPReceiver : public Receiver
{
 public:
  TCPReceiver();
  ~TCPReceiver();

  virtual int rcvr_get_buffer(unsigned char*& buffer, int size, 
			      int offset);

  /** TCP implementation of listen() virtual method first declared
      in Receiver class.
      @address Listening address. Examples: "address:5678" or
      ":5678". The first one means that connections should only
      be allowed from host 'address' using port number 5678. The
      second one means that the host can be any but port number
      must be 5678. No other forms for this address string are
      allowed. This TCP implementation launches a thread that
      calls TCP/IP listen() command. When a connection is requested
      by sender, a separate thread is started for receiving and
      this particular connection is flagged as opened (ref. is_open()).
      @return Handle to a connection to be opened. Zero if some
      error.
  */
  bool rcvr_listen(std::string address);

  /** TCP implementation of open connection query method. 
      @param handle Handle to a connection
      @return Non-zero if connection is opened
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

 protected:
  std::ifstream sockstream_;

 private:
  unsigned char* buffer_;
  int buffer_size_;
  bool opened_;
  std::string address_;
  int port_;
  int fd_;

  bool open_link();

};

#endif
