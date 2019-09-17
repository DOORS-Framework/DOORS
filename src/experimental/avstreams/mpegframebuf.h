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

$Log: mpegframebuf.h,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.1  2001/09/04 12:15:53  ik
Added mpegframebuf.h for AVStreams demo.

Revision 1.4  2001/08/21 16:51:23  ik
Changes to AVStreams.

Revision 1.3  2001/08/17 13:41:40  ik
Changes to #include's. There was an old version of filemas.h, changed.

Revision 1.2  2001/06/08 08:39:53  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef MPEGFRAMEBUF_H
#define MPEGFRAMEBUF_H

#include <vector>

// Default number of frames in vector
#define MFB_DEFFRAMES 100
// Longest possible encoded MPEG-audio frame
#define MFB_BUFSIZE   1152
// If blocking, how long should wait until next try
#define MFB_WAITTIME  20000
// If blocking, how many tries should be made
#define MFB_TRIES     500

class ItemProps {
public:
  ItemProps(long sp, long st, short ch) 
    : speed(sp), stereo(st), channel(ch) { }
  ItemProps() : speed(44100), stereo(1), channel(0) { }
  ItemProps operator=(ItemProps& src) {
    speed = src.speed;
    stereo = src.stereo;
    channel = src.channel;
    return *this;
  }
  long speed;
  long stereo;
  long channel;
};

class FrameItem {
public:
  FrameItem() : _invalid(true) { }
  void set_invalid() { _invalid = true; }
  void insert(short buffer[MFB_BUFSIZE], long size, long counter = 0);
  long get(short buffer[MFB_BUFSIZE]);
  unsigned long get_counter() { return _counter; }
  void set_counter(unsigned long c) { _counter = c; }
  void set_props(ItemProps& props);
  void get_props(ItemProps* props);

private:
  short _buffer[MFB_BUFSIZE];
  int _size;
  bool _invalid;
  long _counter;
  ItemProps _props;
};

class MpegFrameBuf {
public:
  MpegFrameBuf(); 
  MpegFrameBuf(unsigned long size, 
	       bool is_blocking_read = true, bool is_blocking_write = true);
  ~MpegFrameBuf();

  // Insert one frame to end of buffer
  // num is frame number. if not set or -1, it is set to be plus one
  // currently last packet's number.
  // Returns true if successful.
  bool insert_back(short buffer[MFB_BUFSIZE], long size,
                   ItemProps props, long counter = -1);

  // Insert one frame to the place of a previously inserted frame
  // If frame is successfully inserted and there was a packet with
  // counter-value, returns true.
  bool insert(short buffer[MFB_BUFSIZE], long size, 
              ItemProps props, long counter);

  // Get one frame 
  // Returns number of bytes written to buffer
  long get(long& num, short buffer[MFB_BUFSIZE], 
           ItemProps& props);

  // Finds an element in frame vector according to frame number
  // Returns true if vector index with that number was found
  bool find(FrameItem* item, long num);

  // Checks writing situation for reader
  // If ok to read, returns true
  bool check_writeoffset();

  // Checks reading situation for writer
  // If ok to write, returns true
  bool check_readoffset();

  void reset_counter(unsigned long init_value = 0) { _counter = init_value; }

private:
  std::vector<FrameItem*> _vector;
  unsigned long _size;
  long _capacity;
  unsigned long _enough;
  unsigned long _readoffset;
  unsigned long _writeoffset;
  bool _full;
  bool _is_blocking_read;
  bool _is_blocking_write;
  unsigned long _counter;
};

#endif
  
