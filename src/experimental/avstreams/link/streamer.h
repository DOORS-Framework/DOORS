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
$Log: streamer.h,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.3  2001/08/21 16:51:24  ik
Changes to AVStreams.

Revision 1.2  2001/08/17 13:41:40  ik
Changes to #include's. There was an old version of filemas.h, changed.

Revision 1.1  2001/08/15 12:57:05  ik
Added some include files for AVStreams/link.

*/

#ifndef STREAMER_H
#define STREAMER_H

#include <fstream>
#include <string>

/** Virtual class for all streamers
*/
class Streamer
{
 public:
  /** Abstract method to ask streaming status.
      @return true if currently streaming
  */
  virtual inline bool sndr_is_streaming() = 0;

  /** Start streaming.
   */
  virtual void sndr_start() = 0;

  /** Pause streaming.
   */
  virtual void sndr_pause() = 0;

  /** Stop streaming.
   */
  virtual void sndr_stop() = 0;

  /** Rewind the stream.
   */
  virtual void sndr_rewind() = 0;

  /** Position the stream using an absolute position value.
   */
  virtual void sndr_repos_abs(std::streampos pos) = 0;

  /** Position the stream using a relative position value.
   */
  virtual void sndr_repos_rel(std::streampos pos) = 0;

  /** Return true if there is currently nothing to stream.
      @return true If the stream is paused or there is no stream, return true
  */
  virtual bool sndr_is_naptime() { return true; }

  /** Try to reopen the stream using the given url.
      @url URL for the stream
      @return true if successfully reopened
  */
  virtual bool sndr_reopen(std::string url) = 0;

};

#endif
