//Editor-Info: -*- C++ -*-

//  Copyright 2000 
//  	Telecoms Lab, Tampere University of Technology.  All rights reserved.

//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
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

$Log: filempegrtp.C,v $
Revision 1.3  2004/01/19 10:54:48  bilhanan
#include fixed.

Revision 1.2  2002/08/14 09:33:03  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.2  2001/06/08 16:46:50  bilhanan
Full Licence clause into DOORS 0.2


*/
//
//===========================================================================
//File:     filempegrtp.C
//Date:     11.5.1999
//Desc:     Descendant for MpegRtp to support specifically sending of
//          MPEG-audio *files*
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#include <stdio.h>
#include <doors/filempegrtp.h>
#if _CF_WITH_LAME
#include <lame.h>
#endif

#if _CF_WITH_LAME
static char mp3buffer[LAME_MAXMP3BUFFER];
static char inputbuffer[LAME_MAXMP3BUFFER];
static lame_global_flags gf;

bool
FileMpegRtp :: mp3_init(int bitrate)
{
  gf.brate = bitrate;       // Bitrate
  gf.silent = 1;            // Try to be silent
  gf.brhist_disp = 0;       // Do not display VBR histogram
  gf.gtkflag = 0;           // No gtk frame analyzer
  gf.lowpassfreq = -1;      // No low pass filter
  gf.highpassfreq = -1;     // No high pass filter
  gf.quality = 7;           // Lower quality for speed

  if (lame_init_params(&gf) < 0) {
    cerr << "Lame initialisation failed." << endl;
    return false;
  }

  lame_print_config(&gf);

  return true;
}

// read one frame and encode it 
int 
FileMpegRtp :: makeframe(int& bytes)
{
  mp3data_struct mp3data;
  int iread;
  static short int Buffer[2][1152];
  int bytesread;

  // Get initial 64 bytes for decoder
  mp3file.read(inputbuffer, 64);
  bytesread = mp3file.gcount();
  if (bytesread <= 0) {
    return 0; // File is in EOF or some error; return zero
  }
  
  // Try to decode
  iread = lame_decode1(inputbuffer, bytesread, Buffer[0], Buffer[1]);
  if (iread < 0) {
    cerr << "Error: decoder failed." << endl;
    return 0;
  }
  
  // If more data is needed, loop until we get full buffer
  while (iread == 0/*=need more data*/) {
    mp3file.read(inputbuffer, 100);
    bytesread = mp3file.gcount();
    if (bytesread <= 0) {
      return 0; // File is in EOF or some error; give up
    }
    // Try to decode
    iread = lame_decode1(inputbuffer, bytesread, Buffer[0], Buffer[1]);
  }
  if (iread <= 0) {
    cerr << "Error: decoder failed." << endl;
    return 0;
  }

  // Encode buffer
  bytes = lame_encode_buffer(&gf, Buffer[0], Buffer[1], iread,
			     mp3buffer, (int)sizeof(mp3buffer));

  if (bytes < 0) {
    if (bytes == -1) {
      cerr << "Fatal: MP3-buffer is too small." << endl;
    } else {
      cerr << "Fatal: Internal error in Lame: Error code: ";
      cerr << hex << bytes << dec << endl;
    }
    return 0;
  }

  return iread;
}
#endif

int
FileMpegRtp :: open(char* ifilename)
{
  // Just open a file for delivery
  mp3file.open(ifilename, ios::in|ios::binary);
  return(0);
}

int
FileMpegRtp :: open_reencode(char* ifilename, int bitrate)
{
#if _CF_WITH_LAME
  lame_decode_init();
  if (lame_init(&gf) < 0) {
    cerr << "Lame initialisation failed." << endl;
  } else {
    if (!mp3_init(bitrate)) {
      cerr << "Encoder initialisation failed." << endl;
    }
    _bitrate = bitrate;
  }
#else
  // There are no lamers available, set reencoding disabled
  _bitrate = 0;
#endif
  // Open input file
  mp3file.open(ifilename, ios::in|ios::binary);

  return(0);
}

void
FileMpegRtp :: close()
{
#if _CF_WITH_LAME
  if (_bitrate) {
    //    lame_cleanup(mp3buffer);
    _bitrate = 0;
    return;
  }
#endif
  mp3file.close();
}

int FileMpegRtp :: getBuffer(Frame& frame, int size)
{
  Byte byte; 
  int bytes = 0;
  for (; bytes < size; ) {
#if _CF_WITH_LAME
    if (_bitrate) {
      if (_offset >= _size) {
	if (!makeframe(_size)) {
	  makeframe(_size);
	  return 0;
	}
	if (_size == 0) { // If no bytes received, retry
	  continue;
	}
	_offset = 0;
      }
      frame.putLast(mp3buffer[_offset++]);
      bytes++;
      continue;
    } else {
#endif
      if (mp3file.eof())
	return bytes;
      mp3file >> byte;
      frame.putLast(byte);
      bytes++;
#if _CF_WITH_LAME
    }
#endif
  }
  return bytes;
}

