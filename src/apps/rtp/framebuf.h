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

$Log: framebuf.h,v $
Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.5  2001/09/04 12:15:53  ik
Added mpegframebuf.h for AVStreams demo.

Revision 1.2  2001/06/08 16:45:31  bilhanan
Full Licence clause into DOORS 0.2


*/

#ifndef FRAMEBUF_H
#define FRAMEBUF_H

#include <vector>
#include <doors/frame.h>

// Samples in one layer II and III frame
#define FBUF_BUFSIZE 1152
// Default number of frames in frame buffer vector
#define FBUF_DEFSIZE 100

struct _frameitem {
  Frame frame;
  long counter;
};
typedef struct _frameitem frameitem;

/** Frame buffer class to hold MPEG audio frames in a circular buffer.
 */
class FrameBuf {
public:
  /** Constructor
   */
  FrameBuf();

  /** Find a frame from frame buffer according to frame counter value
      @param item Frame item to return
      @param counter Search key; frame counter value
      @return true if a frame found and pointer to it is saved to 'item'
  */
  bool find_frame(frameitem* item, long counter);

  /** Get the next frame out of the frame buffer. If there are no
      frames available (underflow situation), returns immediately zero.
      @param counter Return parameter to hold frame counter value
      @param frame Filled with the next frame if it is available
      @return Frame size or zero if no frames were available
  */
  long get_frame(long& counter, Frame& frame);

  /** Insert a frame in place of a frame with given counter value
      @param frame Frame to be inserted
      @param size Frame size
      @param counter The frame with this counter value should be replaced
      @return true if successfully replaced
  */
  bool insert(Frame& frame, long counter);

  /** Insert a frame in the end of frame buffer. If frame buffer is full
      and so it can't receive any more frames, returns false.
      @param frame Frame to be inserted
      @param size Frame size
      @param counter A frame counter value to be given for the frame. If
             counter is less than zero, counter value is calculated from
             previous frame counter value plus one or zero if there is none.
      @return true if successfully inserted
  */
  bool insert_back(Frame& frame, long counter);

private:
  // Frame buffer stuff
  std::vector<frameitem*> _vector;       // Frame buffer
  long _buf_capacity;               // Buffer capacity
  unsigned long _buf_enough;        // Number of frames enough for reading    
  unsigned long _buf_readoffset;    // Buffer offset for reading
  unsigned long _buf_writeoffset;   // Buffer offset for writing
  bool _buf_full;                   // Flag to indicate when buffer is full
  unsigned long _buf_counter;       // Counter value of the latest frame
};

#endif
