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

$Log: mpegaudio.C,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/08/31 13:59:01  ik
AVStreams changes and modifications to configure.in accordingly.

Revision 1.1  2001/08/14 15:29:19  ik
TCP- and UDP-links are partly rewritten and structure is changed.

Revision 1.2  2001/06/08 16:46:47  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:55  bilhanan
Full licence clause added into doors 0.1


*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <doors/mpegaudio.h>

#define ASSERT(param) assert((param))

const char *genres[] =
{
    "Blues",
    "Classic Rock",
    "Country",
    "Dance",
    "Disco",
    "Funk",
    "Grunge",
    "Hip-Hop",
    "Jazz",
    "Metal",
    "New Age",
    "Oldies",
    "Other",
    "Pop",
    "R&B",
    "Rap",
    "Reggae",
    "Rock",
    "Techno",
    "Industrial",
    "Alternative",
    "Ska",
    "Death Metal",
    "Pranks",
    "Soundtrack",
    "Euro-Techno",
    "Ambient",
    "Trip-Hop",
    "Vocal",
    "Jazz+Funk",
    "Fusion",
    "Trance",
    "Classical",
    "Instrumental",
    "Acid",
    "House",
    "Game",
    "Sound Clip",
    "Gospel",
    "Noise",
    "AlternRock",
    "Bass",
    "Soul",
    "Punk",
    "Space",
    "Meditative",
    "Instrumental Pop",
    "Instrumental Rock",
    "Ethnic",
    "Gothic",
    "Darkwave",
    "Techno-Industrial",
    "Electronic",
    "Pop-Folk",
    "Eurodance",
    "Dream",
    "Southern Rock",
    "Comedy",
    "Cult",
    "Gangsta",
    "Top 40",
    "Christian Rap",
    "Pop/Funk",
    "Jungle",
    "Native American",
    "Cabaret",
    "New Wave",
    "Psychadelic",
    "Rave",
    "Showtunes",
    "Trailer",
    "Lo-Fi",
    "Tribal",
    "Acid Punk",
    "Acid Jazz",
    "Polka",
    "Retro",
    "Musical",
    "Rock & Roll",
    "Hard Rock",
    "Unknown"
};

static const int v1_sample_rate[16][4] = {
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

static const int v2_sample_rate[16][4] = {
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

static const int sampling_freq[4][4] = {
  // MPEG2.5    res.  MPEG2   MPEG1
     11025,     -1,   22050,  44100,
     12000,     -1,   24000,  48000,
      8000,     -1,   16000,  32000,
     -1,        -1,      -1,     -1 };

static const int layer[4] = { -1, 3, 2, 1 };

static const char *version[4] = { "2.5", "unknown", "2.0", "1.0" };

static const char *mode[4] = 
  { "stereo", "joint-stereo", "dual channel", "mono" };

static const char *emphasis[4] = 
  { "none", "50/15 ms", "unknown", "CCIT J.17" };

MPEGAudio::MPEGAudio()
  : oldhead_(0)
{
  frame_ = new unsigned char[MPEGAUDIO_MAXFRAME];
  ASSERT(frame_ != (unsigned char*)0);
}

MPEGAudio::~MPEGAudio()
{
  delete [] frame_;
}

int
MPEGAudio::mpegaudio_get_id3tag(std::ifstream& stream, ID3tag*& tag)
{
  // Save stream position
  std::streampos pos = stream.tellg();
  if (pos < 0) { // If can't tell the position, not seekable stream
    return 0;
  }
  // Seek to end of file where ID3 tag should reside
  stream.seekg(-128, std::ios::end);
  if ((stream.bad()) || (stream.tellg() < 0)) {
    return 0;
  }

  char tmp[30];
  tag = new ID3tag;
  ASSERT(tag != NULL);
  stream.read(tmp, 3);
  if (stream.gcount() == 3) {
    // If tag is not there, return error
    if ((tmp[0] != 'T') || (tmp[1] != 'A') || (tmp[2] != 'G')) {
      stream.seekg(pos, std::ios::beg);
      return 0;
    }
    // title
    stream.read(tmp, 30);
    ASSERT(stream.gcount() == 30);
    tag->title.append(tmp, 30);
    // artist
    stream.read(tmp, 30);
    ASSERT(stream.gcount() == 30);
    tag->artist.append(tmp, 30);
    // album
    stream.read(tmp, 30);
    ASSERT(stream.gcount() == 30);
    tag->album.append(tmp, 30);
    // year
    stream.read(tmp, 4);
    ASSERT(stream.gcount() == 4);
    tmp[4] = '\0'; // Add end of line for atoi
    tag->year = atoi(tmp);
    // comment
    stream.read(tmp, 30);
    ASSERT(stream.gcount() == 30);
    tag->comment.append(tmp, 30);
    stream.seekg(pos, std::ios::beg);
    return 1;
  }
  stream.seekg(pos, std::ios::beg);
  return 0;
}
  
int
MPEGAudio::mpegaudio_get_frame(unsigned char*& frame)
{
  int bytes;
  int offset;
  int crcskip, framesize;
  
  offset = 0;
  int searchskip = 0;
  // Get header (4 bytes)
  bytes = mpegaudio_get_buffer(frame_, 4);
  // If can't get full header, give up
  if (bytes < 4) {
    return 0;
  }

  for ( ; ; ) {

    // Check if we have a correct header
    if (!parse_header(frame_)) {
      break;
    }

    // Add one byte to search counter
    ++searchskip;

    // We don't have a correct header; advance to next byte and
    // try it up to end of the stream until we get something 

    // Move header bytes to left
    frame_[0] = frame_[1];
    frame_[1] = frame_[2];
    frame_[2] = frame_[3];

    // Get the last byte
    bytes = mpegaudio_get_buffer(frame_, 1, 3);


    // If can't get it, give up
    if (bytes < 1) {
      return 0;
    }
  }

  // We have a correct header, read the rest of the frame

  // Check CRC. If it is there, add it and crcskip
  crcskip = 0;
  if (header_.protection_bit == 0) { // Is there a CRC?
    bytes = mpegaudio_get_buffer(frame_, 2, 4);
    if (bytes < 2) {
      return 0;
    }
    crcskip = bytes;
  }

  // Calculate frame size
  framesize = 0;
  if (header_.layer == 1)
    framesize = 12 * header_.bitrate / header_.sampling_freq + 
      4 * header_.padding_bit;
  else
    framesize = 144 * header_.bitrate / header_.sampling_freq + 
      header_.padding_bit;

  // Calculate bytes still left in the frame
  int skip = framesize - crcskip - 4;

  // Check for sanity. There should be something after header
  // and possible CRC AND the whole frame can't be bigger than
  // uncompressed size of one frame. Failing this usually means
  // that we have a corrupted file or not an MPEG audio file at all.
  if ((skip <= 0) || (framesize > MPEGAUDIO_MAXFRAME)) { 
    return 0;
  }

  // Get the rest of the frame
  bytes = mpegaudio_get_buffer(frame_, skip, 4 + crcskip);

  // If the full frame was not read, give up
  if (bytes < skip) {
    return 0;
  }

  // Save frame pointer to parameter
  frame = frame_;

  return framesize;
}

int
MPEGAudio::parse_header(unsigned char*& frame)
{
  // Read header to newhead
  unsigned long newhead = 
    ((unsigned long) frame[0] << 24) | 
    ((unsigned long) frame[1] << 16) |
    ((unsigned long) frame[2] <<  8) | 
     (unsigned long) frame[3];

  // TODO: Is there any sense in the header check?

  // If syncword is not valid, return error
  if ((newhead & 0xffe00000) != 0xffe00000) {
    return(1);
  }

  // If old syncword is different from the new, VBR file or invalid?
  int headerchange = 0;
  if ((oldhead_) && (oldhead_ != newhead)) {
    headerchange = 1;
  }

  // Save the present one as old
  oldhead_ = newhead;

  header_.id = newhead>>19&3;
  header_.layer = layer[newhead>>17&3];
  header_.protection_bit = newhead>>16&1;
  
  if (header_.layer > 0) {
    switch(header_.id) {
    case 0 : // Version 2.5
    case 2 : // Version 2.0
      header_.bitrate = 
	1000 * v2_sample_rate[newhead>>12&0xF][header_.layer];
      break;
    case 3 : // Version 1.0
      header_.bitrate = 
	1000 * v1_sample_rate[newhead>>12&0xF][header_.layer];
      break;
    default :
      header_.bitrate = 0; // Unknown version
      break;
    }
  }

  if (header_.bitrate < 0) { // Unknown bitrate?
    return 0;
  }

  header_.sampling_freq =
    sampling_freq[newhead>>10&3][header_.id];
  header_.padding_bit = newhead>>9&1;
  header_.private_bit = newhead>>8&1;
  header_.mode = newhead>>6&3;
  header_.extension = newhead>>4&3;
  header_.copyright_bit = newhead>>3&1;
  header_.original_bit = newhead>>2&1;
  header_.emphasis = newhead&3;
  
  return(0);
}

int
MPEGAudio::mpegaudio_get_timeout()
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
