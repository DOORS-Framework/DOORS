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


#ifndef SENDER_H
#define SENDER_H

#include <string>
#include <iostream>

/** Virtual class for all senders
*/
class Sender
{
 public:
  /** Abstract method for opening a connection to a receiver.
      @param address Address string. For TCP and UDP it might be
      of the form "receiver.addr:5678" etc.
      @return true if the link is opened
  */
  virtual bool sndr_open(const std::string address) = 0;

  /** Close the link opened by open().
      @param handle Handle to the connection to be closed
      @return true if connection is successfully closed
   */
  virtual bool sndr_close() = 0;

  /** Send next frame.
      @param buffer Send buffer
      @param size size of bytes to be sent in the buffer
      @return true if successfully sent
  */
  virtual bool sndr_send_frame(unsigned char* buffer, int size) = 0;

  /** Abstract method to return status of the link.
      @return true if the link is open
  */
  virtual inline bool sndr_is_open() = 0;

  /** Abstract method for getting interval of time to play the current
      frame.
      @param handle Handle to the connection
      @return Time to play the current frame (in microseconds)
  */
  virtual int sndr_get_timeout() = 0;

};

#endif
