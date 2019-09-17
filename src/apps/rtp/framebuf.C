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

$Log: framebuf.C,v $
Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.2  2001/06/08 16:46:51  bilhanan
Full Licence clause into DOORS 0.2


*/
#include <vector>
#include <doors/frame.h>
#include <doors/framebuf.h>

FrameBuf :: FrameBuf()
  : _buf_counter(0), _buf_readoffset(0), _buf_writeoffset(0), 
    _buf_capacity(0), _buf_full(false)
{
  // Consider buffer filled enough if 75% of it is full
  _buf_enough = (75 * FBUF_BUFSIZE) / 100;
}

bool
FrameBuf :: insert_back(Frame& frame, long counter)
{
  if (_buf_full) { // Vector is in its full size
    bool bufferfull = (_buf_capacity >= (FBUF_DEFSIZE - 2)) ? true : false;
    if (bufferfull) { // If buffer is full, return false
      return false;
    }
    _vector[_buf_writeoffset]->frame = frame;
    _vector[_buf_writeoffset]->counter = _buf_counter;
    ++_buf_writeoffset;
    ++_buf_capacity;
    if (_buf_writeoffset >= FBUF_DEFSIZE) {
      _buf_writeoffset = 0;
    }
    // Set global counter if 'counter' is set (>=0)
    if (counter < 0) {
      ++_buf_counter;
    } else {
      _buf_counter = counter;
    }
  } else { // Vector is still growing
    frameitem* item = new frameitem;
    item->frame = frame;
    item->counter = _buf_counter;
    // Set global counter if 'counter' is set (>=0)
    if (counter < 0) {
      ++_buf_counter;
    } else {
      _buf_counter = counter;
    }
    _vector.push_back(item);
    ++_buf_writeoffset;
    ++_buf_capacity;
    // If in the end, go back to start and mark buffer full
    if (_buf_writeoffset >= FBUF_DEFSIZE) {
      _buf_full = true;
      _buf_writeoffset = 0;
    }
  }

  return true;
}

bool
FrameBuf :: insert(Frame& frame, long counter)
{
  frameitem* item;
  if (find_frame(item, counter)) {
    item->frame = frame;
    item->counter = _buf_counter;
    return true;
  }

  return false;
}

long
FrameBuf :: get_frame(long& counter, Frame& frame)
{
  // If buffer is not yet full, return zero
  if (!_buf_full) {
    return 0;
  }
  // If buffer is empty, return zero
  bool bufferempty = (_buf_capacity <= 0) ? true : false;
  if (bufferempty) {
    return 0;
  }
  // Return latest frame
  frameitem* item = _vector[_buf_readoffset];
  frame = item->frame;
  counter = item->counter;
  ++_buf_readoffset;
  --_buf_capacity;
  // If in the end, go back to start
 if (_buf_readoffset >= FBUF_DEFSIZE) {
    _buf_readoffset = 0;
  }

  return frame.length();
}

bool
FrameBuf :: find_frame(frameitem* item, long counter)
{
  for (register int i = 0; i < FBUF_DEFSIZE; i++) {
    item = _vector[i];
    if (item->counter == counter) {
      return true;
    }
  }
  return false;
}
  
