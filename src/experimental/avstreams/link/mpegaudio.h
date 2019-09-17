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

$Log: mpegaudio.h,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.4  2001/08/21 16:51:23  ik
Changes to AVStreams.

Revision 1.3  2001/08/14 15:26:47  ik
TCP- and UDP-links are partly rewritten and structure is changed.

Revision 1.2  2001/06/08 16:45:43  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:55  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef MPEGAUDIO_H
#define MPEGAUDIO_H

#include <fstream>
#include <string>

// Maximum size for a audio MPEG frame
#define MPEGAUDIO_MAXFRAME 1152

// Structure to hold information about a MPEG frame
typedef struct mpeg_header_ {
  int id;
  int layer;
  int protection_bit;
  int bitrate;
  int sampling_freq;
  int padding_bit;
  int private_bit;
  int mode;
  int extension;
  int copyright_bit;
  int original_bit;
  int emphasis;
} mpeg_header;

class ID3tag {
 public:
  ID3tag() : year(0) { }
  std::string title;
  std::string artist;
  std::string album;
  int year;
  std::string comment;
  std::string genre;
};

class MPEGAudio 
{
 public:
  MPEGAudio();
  ~MPEGAudio();

  /** Virtual method for getting data for the mpegaudio_get_frame()
      to handle. This should be implemented by the descendant to
      return data from a stream source.
      @param handle Handle to the connection
      @param buffer Data buffer
      @param size Number of bytes needed
      @return Number of bytes written into frame buffer. Zero if
      no bytes are available.
  */
  virtual int mpegaudio_get_buffer(unsigned char*& buffer, int size, 
				   int offset = 0) = 0;

  /** Method for getting the next frame to send.
      @param frame Frame to be written
      @return Zero if there are no frames to send anymore, otherwise 
      the frame size.
  */
  int mpegaudio_get_frame(unsigned char*& frame);

  mpeg_header mpegaudio_get_header() { return header_; }

  int mpegaudio_get_timeout();

 protected:
  int mpegaudio_get_id3tag(std::ifstream& stream, ID3tag*& tag);

 private:
  unsigned long oldhead_;
  int parse_header(unsigned char*& frame);
  mpeg_header header_;
  unsigned char* frame_;
};
  
#endif
