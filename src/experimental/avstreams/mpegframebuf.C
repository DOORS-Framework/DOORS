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

$Log: mpegframebuf.C,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.1  2001/09/04 12:34:28  ik
framebuf.C renamed to mpegframebuf.C due to collision with mp3demo.
Makefiles modified accordingly.

Revision 1.2  2001/08/31 13:58:59  ik
AVStreams changes and modifications to configure.in accordingly.

Revision 1.1  2001/08/15 10:08:50  ik
Renaming some files to conform other DOORS files.

Revision 1.2  2001/06/08 16:46:46  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:53  bilhanan
Full licence clause added into doors 0.1


*/

#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <doors/mpegframebuf.h>

static pthread_mutex_t _mutex;

void
FrameItem::insert(short buffer[MFB_BUFSIZE], long size,
		   long counter)
{
  memcpy(_buffer, buffer, MFB_BUFSIZE * sizeof(short));
  _invalid = false;
  _size = size;
  _counter = counter;
}

long
FrameItem::get(short buffer[MFB_BUFSIZE])
{
  memcpy(buffer, _buffer, MFB_BUFSIZE * sizeof(short));
  return _size;
}

void
FrameItem::set_props(ItemProps& props)
{
  _props = props;
}

void
FrameItem::get_props(ItemProps* props)
{
  *props = _props;
}

MpegFrameBuf::MpegFrameBuf()
  : _readoffset(0), _writeoffset(0), _size(MFB_DEFFRAMES), _full(false),
    _is_blocking_read(true), _is_blocking_write(true), _capacity(0), 
    _counter(0)
{
  _enough = (75 * MFB_DEFFRAMES) / 100;
  pthread_mutex_init(&_mutex, NULL);
}

MpegFrameBuf::MpegFrameBuf(unsigned long size, 
			   bool is_blocking_read, bool is_blocking_write)
  : _readoffset(0), _writeoffset(0), _size(size), _full(false),
    _is_blocking_read(is_blocking_read), 
    _is_blocking_write(is_blocking_write), _capacity(0), _counter(0)
{
  _enough = (75 * _size) / 100;
  pthread_mutex_init(&_mutex, NULL);
}

MpegFrameBuf::~MpegFrameBuf()
{
  pthread_mutex_destroy(&_mutex);
}

bool
MpegFrameBuf::insert_back(short buffer[MFB_BUFSIZE], long size,
			  ItemProps props, long counter)
{
  register int i;
  FrameItem* item;

  if (_full) { // Vector is in its full size

    bool bufferfull;
    pthread_mutex_lock(&_mutex);
    bufferfull = (_capacity >= (_size - 2)) ? true : false;
    pthread_mutex_unlock(&_mutex);
    if (bufferfull) {
      if (_is_blocking_write) {
	do {
	  usleep(20000);
	  pthread_mutex_lock(&_mutex);
	  bufferfull = (_capacity >= (_size - 2)) ? true : false;
	  pthread_mutex_unlock(&_mutex);
	} while (bufferfull);
      } else {
	return false;
      }
    }

    pthread_mutex_lock(&_mutex);
    _vector[_writeoffset]->insert(buffer, size, _counter);
    ++_writeoffset;
    ++_capacity;
    if (_writeoffset >= _size) {
      _writeoffset = 0;
    }
    pthread_mutex_unlock(&_mutex);
 
    if (counter < 0) {
      ++_counter;
    } else {
      _counter = counter;
    }

  } else { // Vector is still growing

    item = new FrameItem;
    item->set_props(props);
    item->insert(buffer, size, _counter);

    if (counter < 0) {
      ++_counter;
    } else {
      _counter = counter;
    }

    pthread_mutex_lock(&_mutex);
    _vector.push_back(item);
    ++_writeoffset;
    ++_capacity;
    if (_writeoffset >= _size) {
      _full = true;
      _writeoffset = 0;
    }
    pthread_mutex_unlock(&_mutex);
  }

  return true;
}

bool
MpegFrameBuf::insert(short buffer[MFB_BUFSIZE], long size, 
		     ItemProps props, long counter)
{
  FrameItem* item;
  bool isfound = false;
  pthread_mutex_lock(&_mutex);
  if (find(item, counter)) {
    item->set_props(props);
    item->insert(buffer, size, counter);
    isfound = true;
  }
  pthread_mutex_unlock(&_mutex);

  return isfound;
}

long
MpegFrameBuf::get(long& counter, short buffer[MFB_BUFSIZE],
		  ItemProps& props)
{
  FrameItem* item;
  long size;

  // Check if buffer is still growing and not enough frames 
  // written, wait for signal from writer
  pthread_mutex_lock(&_mutex);
  if (!_full) {
    pthread_mutex_unlock(&_mutex);
    if (!_is_blocking_read) {
      bool filling = true;
      do {
	usleep(20000);
	pthread_mutex_lock(&_mutex);
	filling = (_capacity < _enough) ? true : false;
	pthread_mutex_unlock(&_mutex);
      } while(filling);
    } else {
      return 0;
    }
  }
  pthread_mutex_unlock(&_mutex);
  bool bufferempty;
  pthread_mutex_lock(&_mutex);
  bufferempty = (_capacity <= 0) ? true : false;
  pthread_mutex_unlock(&_mutex);
  if (bufferempty) {
    if (_is_blocking_read) {
      do {
	usleep(20000);
	pthread_mutex_lock(&_mutex);
	bufferempty = (_capacity <= 0) ? true : false;
	pthread_mutex_unlock(&_mutex);
      } while (bufferempty);
    } else {
      return 0;
    }
  }

  pthread_mutex_lock(&_mutex);
  item = _vector[_readoffset];
  item->get_props(&props);
  size = item->get(buffer);
  counter = item->get_counter();
  ++_readoffset; // Advance to next frame
  --_capacity;   // Decrease frame capacity
  if (_readoffset >= _size) {
    _readoffset = 0;
  }
  pthread_mutex_unlock(&_mutex);

  return size;
}

bool 
MpegFrameBuf::find(FrameItem* item, long counter)
{
  register int i;
  bool isfound = false;
  for (i = 0; i < _size; i++) {
    item = _vector[i];
    if (item->get_counter() == counter) {
      isfound = true;
      break;
    }
  }
  return isfound;
}

