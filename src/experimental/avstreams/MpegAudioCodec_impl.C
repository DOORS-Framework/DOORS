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

$Log: MpegAudioCodec_impl.C,v $
Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/08/31 13:58:57  ik
AVStreams changes and modifications to configure.in accordingly.

Revision 1.1  2001/08/15 10:08:47  ik
Renaming some files to conform other DOORS files.

Revision 1.2  2001/06/08 16:46:45  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.2  2001/06/08 08:39:51  bilhanan
Full licence clause added into doors 0.1


*/

#define LAMELIB
#include <lame.h>
#include <doors/MpegAudioCodec_impl.h>

// Implementation for interface MpegAudioCodec

MpegAudioCodec_impl::MpegAudioCodec_impl()
{
  lame_init(&lame_global_flags_);
  lame_decode_init();
}

MpegAudioCodec_impl::~MpegAudioCodec_impl()
{
  // TODO: Lame cleanup
}

CORBA::Boolean
MpegAudioCodec_impl::init_encoding( const EncodingParameters& param )
{
  // Bitrate
  if (param.bitrate) {
    lame_global_flags_.brate = param.bitrate;
  } else {
    lame_global_flags_.brate = 128000;
  }

  // Sampling frequency must be set
  if (param.sfreq) {
    lame_global_flags_.in_samplerate = param.sfreq;
    lame_global_flags_.out_samplerate = param.sfreq;
  } else {
    return FALSE;
  }

  // Mode
  switch(param.mode) {
  case stereo :
    lame_global_flags_.mode = 0;
    break;
  case dual_channel :
    lame_global_flags_.mode = 2;
    break;
  case mono :
    lame_global_flags_.mode = 3;
    break;
  default : // Default to joint-stereo
    lame_global_flags_.mode = 1;
    break;
  }

  // VBR
  if (param.vbr) {
    lame_global_flags_.VBR = vbr_default;
    if (param.vbr_quality) {
      lame_global_flags_.VBR_q = param.vbr_quality;
    }
  }

  // Hi-quality flag
  if (param.hi_quality) {
    lame_global_flags_.quality = 0;
  }

  // Faster encoding -flag
  if (param.fast_mode) {
    lame_global_flags_.quality = 9;
  }

  // Suppress any outputs
  lame_global_flags_.silent = 1;
  lame_global_flags_.brhist_disp = 0;

  // Input is raw pcm
  lame_global_flags_.input_format = sf_raw;

  return TRUE; 
}

CORBA::Boolean 
MpegAudioCodec_impl::encode(const CORBA::Any& feed, 
			    CORBA::Any_out result)  
{
  Unencoded inframe;

  // Transform from Any to Unencoded buffer type
  feed >>= inframe;

  // Encode buffer
  int size = ::lame_encode_buffer(&lame_global_flags_, 
			   inframe.buffer[0], inframe.buffer[1], 1152,
			   mp3buffer, 0);

  // Create Any and Encoded types for result
  result = new CORBA::Any;
  Encoded outframe;

  // If the size is over zero, copy LAME's result buffer to
  // our Encoded-buffer and save size, otherwise say that buffer
  // is zero sized.
  if (size > 0) {
    // Copy encoded buffer back to outframe
    memcpy(outframe.buffer, mp3buffer, size);
    outframe.length = size;
  } else {
    outframe.length = 0;
  }

  // Save our result buffer to the new Any type
  *result <<= outframe;

  return TRUE;
}

CORBA::Boolean 
MpegAudioCodec_impl::reencode(const CORBA::Any& feed, 
			    CORBA::Any_out result)  
{
  Encoded inframe;
  Unencoded middleframe;
  Encoded outframe;
  int size;

  // Transform from Any to Encoded buffer type
  feed >>= inframe;

  result = new CORBA::Any;
  size = inframe.length;
  if (size > 0) {
    memcpy(mp3buffer, inframe.buffer, size);
    // Decode frame
    int samples = ::lame_decode(mp3buffer, size, middleframe.buffer[0],
				                 middleframe.buffer[1]);
    if (samples > 0) {
      middleframe.samples = samples;
      // Encode it back
      int size = ::lame_encode_buffer(&lame_global_flags_,
			       middleframe.buffer[0], middleframe.buffer[1],
			       1152, mp3buffer, 0);
      // If resulting size is more than zero, copy to outframe
      if (size > 0) {
	// Copy encoded buffer back to outframe
	memcpy(outframe.buffer, mp3buffer, size);
	outframe.length = size;
      } else {
	// If lame_encode returns zero, it means that not enough
	// bytes was given to encoder. Set length to zero and
	// return TRUE.
	outframe.length = 0;
      }

      *result <<= outframe;

      return TRUE;
    }
  }
  
  outframe.length = 0;
  *result <<= outframe;

  // If input buffer size is zero, return FALSE
  return FALSE;
}

CORBA::Boolean 
MpegAudioCodec_impl::decode(const CORBA::Any& feed, 
			    CORBA::Any_out result)
{
  Encoded inframe;
  Unencoded outframe;

  // Transform from Any to Encoded buffer type
  feed >>= inframe;

  int size = inframe.length;

  result = new CORBA::Any;
  if (size > 0) {
    memcpy(mp3buffer, inframe.buffer, size);
    // Decode frame
    int samples = ::lame_decode(mp3buffer, size, 
				outframe.buffer[0], outframe.buffer[1]);
    if (samples > 0) {
      outframe.samples = samples;
      *result <<= outframe;
      return TRUE;
    }
  }

  outframe.samples = 0;
  *result <<= outframe;

  return FALSE;
}


