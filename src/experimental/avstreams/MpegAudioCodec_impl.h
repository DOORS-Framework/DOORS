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

$Log: MpegAudioCodec_impl.h,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.5  2001/08/21 16:51:20  ik
Changes to AVStreams.

Revision 1.4  2001/08/17 13:41:39  ik
Changes to #include's. There was an old version of filemas.h, changed.

Revision 1.3  2001/08/15 12:19:18  ik
Makefiles added to avstreams.

Revision 1.2  2001/06/08 16:45:21  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:51  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef __MPEGAUDIOCODEC_IMPL_H__
#define __MPEGAUDIOCODEC_IMPL_H__

#include <MpegAudioCodec.h>
#include <Codec.h>
#define LAMELIB
#include <lame.h>

// Maximum size of mp3buffer would be
// MPEG1:
//    num_samples*(bitrate/8)/samplerate + 4*1152*(bitrate/8)/samplerate + 512
// MPEG2:
//    num_samples*(bitrate/8)/samplerate + 4*576*(bitrate/8)/samplerate + 256
// (from Lame source comments)
// We define some safe value
#define MAX_MP3_BUFFER 7000

// Implementation for interface MpegAudioCodec
class MpegAudioCodec_impl : 
  virtual public Codec,
  virtual public POA_MpegAudioCodec
{
  public:

  MpegAudioCodec_impl();
  ~MpegAudioCodec_impl();

  virtual CORBA::Boolean encode(const CORBA::Any& inbuffer, CORBA::Any_out outbuffer);
  
  virtual CORBA::Boolean decode(const CORBA::Any& inbuffer, CORBA::Any_out outbuffer);

  virtual CORBA::Boolean reencode(const CORBA::Any& inbuffer, CORBA::Any_out outbuffer);

  CORBA::Boolean init_encoding( const EncodingParameters& param );

 private:
  char mp3buffer[MAX_MP3_BUFFER];
  PortableServer::POA_var mypoa;
  lame_global_flags lame_global_flags_;
};


#endif
