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

$Log: mpegrtp.C,v $
Revision 1.2  2003/08/11 23:35:36  bilhanan
bundled maintenance commit:
	const char* in constructor -> std::string
	#include ".. " -> #include <doors/..>
	ONS_CF_ -> _CF_

Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.3  2001/06/08 16:46:52  bilhanan
Full Licence clause into DOORS 0.2


*/
//
//===========================================================================
//File:     mpegrtp.C
//Date:     11.5.1999
//Subject:  Descendant for RtpTask that supports MPEG-audio frames
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#include <string>
#include <doors/frame.h>
#include <doors/rtptask.h>
#include <doors/mpegrtp.h>

// MPEG header
//
// 0                   1                   2                   3
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|    syncword = 0xFFF   |I| L |P|  BRI  |SF |D|R| M | X |C|H|EMP|
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
//  I   - ID=1
//  L   - layer (1, 2 or 3)
//  P   - protection bit (0)
//  BRI - bitrate index
//  SF  - sampling frequency
//  D   - padding bit
//  R   - private bit
//  M   - mode
//  X   - mode extension
//  C   - copyright
//  H   - original home
//  EMP - emphasis



static int v1_sample_rate[16][4] = {
  -1,      -1,      -1,      -1,
  -1,      32,      32,      32,
  -1,      64,      48,      40,
  -1,      96,      56,      48,
  -1,     128,      64,      56,
  -1,     160,      80,      64,
  -1,     192,      96,      80,
  -1,     224,     112,      96,
  -1,     256,     128,     112,
  -1,     288,     160,     128,
  -1,     320,     192,     160,
  -1,     352,     224,     192,
  -1,     384,     256,     224,
  -1,     416,     320,     256,
  -1,     448,     384,     320,
  -1,      -1,      -1,      -1 };

static int v2_sample_rate[16][4] = {
  -1,      -1,      -1,      -1,
  -1,      32,       8,       8,
  -1,      48,      16,      16,
  -1,      56,      24,      24, 
  -1,      64,      32,      32,
  -1,      80,      40,      40,
  -1,      96,      48,      48,
  -1,     112,      56,      56,
  -1,     128,      64,      64,
  -1,     144,      80,      80,
  -1,     160,      96,      96,
  -1,     176,     112,     112,
  -1,     192,     128,     128,
  -1,     224,     144,     144,
  -1,     256,     160,     160,
  -1,      -1,      -1,      -1 };

static int sampling_freq[4][4] = {
  // MPEG2.5    res.  MPEG2   MPEG1
     11025,     -1,   22050,  44100,
     12000,     -1,   24000,  48000,
      8000,     -1,   16000,  32000,
     -1,        -1,      -1,     -1 };

static int layer[4] = { -1, 3, 2, 1 };

static char *version[4] = { "2.5", "unknown", "2.0", "1.0" };

static char *mode[4] = { "stereo", "joint-stereo", "dual channel", "mono" };

static char *emphasis[4] = { "none", "50/15 ms", "unknown", "CCIT J.17" };

MpegRtp::MpegRtp(std::string n, Scheduler *s, RtpTaskSM *_sm, 
		 InetAddr *own, InetAddr *dest)
  : RtpTask(n, s, _sm, own, dest)
{
  memset(&header_, 0, sizeof(mpeg_header));
  setType(RTP_PT_MPA);
}


// MPEG audio RTP header
//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |             MBZ               |          Frag_offset          |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
//           Frag_offset: Byte offset into the audio frame for the data
//                        in this packet.

int
MpegRtp::add_rtp_header(Frame& frame)
{
  // Just four bytes of zero by now
  frame.putLast((Byte)0);
  frame.putLast((Byte)0);
  frame.putLast((Byte)0);
  frame.putLast((Byte)0);

  return 4;
}

int
MpegRtp::parse_header(Frame& frame)
{
  unsigned short syncword = 0;

  // Read syncword, 11 bits (0x7FF)
  // 0                   1 
  // 0 1 2 3 4 5 6 7 8 9 0 
  //+-+-+-+-+-+-+-+-+-+-+-+--
  //|   syncword = 0x7FF  |
  //+-+-+-+-+-+-+-+-+-+-+-+--
  syncword = (unsigned short)frame.read(0);
  syncword <<= 3;
  syncword |= ((frame.read(1) >> 4) & 7);
  
  if (syncword != 0x7FF) {
    return(1);
  }

  // Version (I), Layer (L) and protection bit (P)
  //  1          
  //  0 1 2 3 4 5
  //--+-+-+-+-+-+--
  //  | I | L |P|
  //--+-+-+-+-+-+--
  header_.id = frame.read(1)>>3&3;
  header_.layer = layer[frame.read(1)>>1&3];
  header_.protection_bit = frame.read(1)&1;
  
  if (header_.layer > 0) {
    switch(header_.id) {
    case 0 : // Version 2.5
    case 2 : // Version 2.0
      header_.bitrate = 
	1000 * v2_sample_rate[frame.read(2)>>4&0xF][header_.layer];
      break;
    case 3 : // Version 1.0
      header_.bitrate = 
	1000 * v1_sample_rate[frame.read(2)>>4&0xF][header_.layer];
      break;
    default :
      header_.bitrate = 0; // Unknown version
      break;
    }
  }
  
  header_.sampling_freq =
    sampling_freq[frame.read(2)>>2&3][header_.id];
  header_.padding_bit = frame.read(2)>>1&1;
  header_.private_bit = frame.read(2)&1;
  header_.mode = frame.read(3)>>6&3;
  header_.extension = frame.read(3)>>4&3;
  header_.copyright_bit = frame.read(3)>>3&1;
  header_.original_bit = frame.read(3)>>2&1;
  header_.emphasis = frame.read(3)&3;
  
  return(0);
}

int
MpegRtp::getFrame(Frame& frame) 
{
  Frame tmpframe(1152);
  Frame tmp;
  int bytes, framesize;
  int crcskip, skip;
  const int headerskip = 4;

  bytes = getBuffer(tmpframe, 4);

  // If there are no four bytes, frame is invalid
  if (bytes != 4)
    return RTP_ERROR_INVALID_FRAME;

  // If header is invalid, try to syncronise to stream by walking
  // it byte by byte forward
  while (parse_header(tmpframe)) {
    // Rotate three bytes left
    tmpframe.write(tmpframe.read(1), 0);
    tmpframe.write(tmpframe.read(2), 1);
    tmpframe.write(tmpframe.read(3), 2);
    // Remove last byte to make room for a new byte
    tmpframe.getLast(1);
    // Get the fourth byte to the end
    if (getBuffer(tmpframe, 1) != 1) {
      return RTP_ERROR_INVALID_HEADER;
    }
    cout << hex << (int)tmpframe.read(0) << dec << ":";
    cout << hex << (int)tmpframe.read(1) << dec << ":";
    cout << hex << (int)tmpframe.read(2) << dec << ":";
    cout << hex << (int)tmpframe.read(3) << dec << endl;
  }      

  // Add packet count and sequence
  packets_++;
  sequence_++;

  // Add timestamp with sampling frequency
  timestamp_ += header_.sampling_freq;

  // Add header bytes to number of octets
  octets_ += bytes;

  // Add RTP header and add its size to number of octets
  octets_ += add_rtp_header(frame);

  // Add header to frame
  frame.combine(tmpframe);

  crcskip = 0;
  if (header_.protection_bit == 0) {
    bytes = getBuffer(tmpframe, 2);
    if (bytes != 2)
      return RTP_ERROR_INVALID_FRAME;
    octets_ += bytes;
    crcskip = bytes;
    // Add CRC to frame
    frame.combine(tmpframe);
  }

  // Calculate frame size
  framesize = 0;
  if (header_.layer == 1)
    framesize = 12 * header_.bitrate / header_.sampling_freq + 
      4 * header_.padding_bit;
  else
    framesize = 144 * header_.bitrate / header_.sampling_freq + 
      header_.padding_bit;

  // Skip is number of octets to jump to the next frame
  skip = framesize - crcskip - headerskip;

  bytes = getBuffer(tmpframe, skip);

  // If not enough bytes given, the stream is malformed somehow
  if (bytes < skip) 
    return RTP_ERROR_INVALID_FRAME;

  // Add data to frame
  frame.combine(tmpframe);
  // Add number of octets
  octets_ += bytes;
  // Add one sequence, this
  packets_++;

  return(0);
}

int
MpegRtp::getHeader(mpeg_header& header)
{
  memcpy(&header, &header_, sizeof(mpeg_header));
  return(0);
}

int 
MpegRtp::getTimeout()
{
  if (header_.sampling_freq != 0) {
    if (header_.layer == 1)
      // In layer I, frame size if 384 bytes
      return 1000000 * 384 / header_.sampling_freq;
    else 
      // In layer II and III, frame size is 1152 bytes
      return 1000000 * 1152 / header_.sampling_freq;
  }
  // If timeout cannot be resolved, give 20ms to 
  // make sure that everything works although maybe badly :-)
  return 20000;
}

