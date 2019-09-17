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

$Log: receiver.h,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.4  2001/08/21 16:51:24  ik
Changes to AVStreams.

Revision 1.3  2001/08/14 15:26:50  ik
TCP- and UDP-links are partly rewritten and structure is changed.

Revision 1.2  2001/06/08 16:45:53  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:57  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef RECEIVER_H
#define RECEIVER_H

#include <string>

#define RCVR_MAX_BUFFER_SIZE 16384

/** Virtual class for all receivers
*/
class Receiver
{
 public:
  /** Fill buffer 
   */
  virtual int rcvr_get_buffer(unsigned char*& buffer, int size, 
			      int offset) = 0;

  /** Abstract method for setting the receiver to listen to sender 
      actions. 
      @param address Address string. For TCP and UDP it might be
      of the form "sender.addr:5678" etc.
      @return true if receiver is listening
  */
  virtual bool rcvr_listen(std::string address) = 0;

  /** Returns true if a connection is established to a sender
      specified in the parameter of listen(). 
      @return true if connection is opened
  */
  virtual bool rcvr_is_open() = 0;

  /** Returns the port number used in the opened link. If the port number
      is given when calling listen(), the result is usually the same
      but it may be other too if port was in use etc. 
      @return Port number in use
  */
  virtual int rcvr_get_port() = 0;

  /** Returns address of the server in the opened link. If sender
      address is given when calling listen(), the result is the same
      (without possible port number part). Otherwise, it is the
      address to the server that has opened a link or empty if
      there's no connection.
      @return Sender address
  */
  virtual std::string rcvr_get_address() = 0;

  /** Close the connection.
   */
  virtual bool rcvr_close() = 0;
};

#endif
