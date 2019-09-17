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
$Log: mpegtcpfilesender.h,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/08/21 16:51:23  ik
Changes to AVStreams.

Revision 1.1  2001/08/15 12:57:05  ik
Added some include files for AVStreams/link.

*/


#ifndef MPEGTCPFILESENDER_H
#define MPEGTCPFILESENDER_H

#include <string>
#include <doors/filesender.h>
#include <doors/mpegaudio.h>
#include <doors/tcpsender.h>

/** Special version of file server to send MPEG audio frames.
    Implements only mpegaudio_get_buffer() for MPEGAudio class and
    sndr_get_timeout() for Sender class.
 */
class MpegTCPFileSender : public FileSender,
			  public MPEGAudio,
			  public TCPSender
{
 public:
  MpegTCPFileSender(std::string filename) : FileSender(filename) { }
  MpegTCPFileSender(int fd) : FileSender(fd) { }
  MpegTCPFileSender() : FileSender() { }

  /** Destructor. Nothing to do here but declared to avoid certain
      problems in inheriting.
  */
  virtual ~MpegTCPFileSender() { }

  /** Declared first in MPEGAudio class. Returns 'size' bytes from a file
      to 'buffer' with 'offset' to it.
  */
  int mpegaudio_get_buffer(unsigned char*& buffer, int size,
			   int offset);

  int sndr_get_timeout() { return mpegaudio_get_timeout(); }

};

#endif

