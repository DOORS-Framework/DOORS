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

$Log: tcpsender.h,v $
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

#ifndef TCPSENDER_H
#define TCPSENDER_H

#include <fstream>
#include <string>
#include <doors/sender.h>

class TCPSender : public Sender
{
 public:
  TCPSender();
  ~TCPSender();

  /** TCP implementation for opening a connection to a receiver. This
      method opens a socket and does connect() for it. After that,
      it returns.
      @param address Address to a receiver
      @return true if no link is opened, otherwise some error
  */
  bool sndr_open(const std::string address);

  /** TCP implementation for closing the connection to a receiver.
      @return true if successfully closed
  */
  bool sndr_close();

  /** TCP implementation of sending of the next frame.
      @param buffer Data buffer
      @param true if successfully sent
  */
  bool sndr_send_frame(unsigned char* buffer, int size);

  inline bool sndr_is_open() { return opened_; }

 private:
  int port_;
  std::string address_;
  int socket_;
  std::ofstream sockstream_;
  int opened_;

};

#endif
