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

$Log: mp3task.C,v $
Revision 1.4  2003/08/11 23:35:36  bilhanan
bundled maintenance commit:
	const char* in constructor -> std::string
	#include ".. " -> #include <doors/..>
	ONS_CF_ -> _CF_

Revision 1.3  2002/08/14 09:33:03  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/07/23 14:28:52  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.2  2001/06/08 16:46:51  bilhanan
Full Licence clause into DOORS 0.2


*/
//
//===========================================================================
//File:     mp3task.C
//Date:     20.4.1999
//Desc:     State machine functions for MPEG-audio application task
//
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

//#include "defs.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <doors/hsi.h>
#include <doors/rtcp.h>
#include <doors/rtpsap.h>
#include <doors/rtcpsap.h>
#include <doors/mp3task.h>
#include <doors/mp3sm.h>
#include <doors/framebuf.h>
#include <arpa/inet.h>
#include <assert.h>

#if _CF_WITH_LAME
#include <lame.h>
#endif

static int sampling_freq[4][4] = {
  // MPEG2.5    res.  MPEG2   MPEG1
     11025,     -1,   22050,  44100,
     12000,     -1,   24000,  48000,
      8000,     -1,   16000,  32000,
     -1,        -1,      -1,     -1 };

MP3Task :: MP3Task (std::string n, Scheduler *s, MP3TaskSM *_sm, 
		    InetAddr *own, InetAddr *dest, int ttl, bool start)
: PTask (n, s, _sm),
  rtpdown(this), rtcpdown(this),
  terminated_(false), sent_(false), rtcp_(NULL),
  sendTimer_ (this, OTime (0, 1)), ttl_(ttl), start_sending_(start), 
  setupround_(true), ownaddr_(*own), destaddr_(*dest)
{

#ifdef WITH_UI
  STATIC_SYMBOL (rtpdown, this);
  STATIC_SYMBOL (rtcpdown, this);
#endif

  items_ = new SrcDesc();
  items_->src = 0;
  items_->cname = "DOORS RTP";
  items_->name = "Karzan";
  items_->email = "ik@iki.fi";
  items_->tool = "DOORS";
  items_->note = "Test run";

  // Start timer that calls the timeout function that 
  // sends our setup to RTP task
  sendTimer_.start();

#if _CF_WITH_LAME
  // Initialise lame decoder
  lame_decode_init();

#endif

  // Initialise audio device
  sounddev_.open_audio();
}

MP3Task :: ~MP3Task (void) {
  delete items_;
#ifdef WITH_UI
  DELETE_SYMBOL (this); 
#endif
}

bool
MP3Task :: set_audio(Frame& frame, bool& stereo)
{
  static int old_sampling = -1;
  static int old_mode = -1;

  // There must be at least bytes for one header
  if (frame.length() < 4) {
    return false;
  }

  unsigned short syncword = (unsigned short)frame.read(0);

  // Read syncword, 11 bits (0x7FF)
  // 0                   1 
  // 0 1 2 3 4 5 6 7 8 9 0 
  //+-+-+-+-+-+-+-+-+-+-+-+--
  //|   syncword = 0x7FF  |
  //+-+-+-+-+-+-+-+-+-+-+-+--
  syncword <<= 3;
  syncword |= (unsigned short)(frame.read(1) >> 4 & 7);
  if (syncword != 0x7FF) 
    return(false);

  // Version (I), Layer (L) and protection bit (P)
  //  1          
  //  0 1 2 3 4 5
  //--+-+-+-+-+-+--
  //  | I | L |P|
  //--+-+-+-+-+-+--
  int id = frame.read(1)>>3&3;
  int sampling =
    sampling_freq[frame.read(2)>>2&3][id];
  int mode = frame.read(3)>>6&3;

  if (sampling > 0) {
    // If old values apply, don't change anything
    if ((sampling == old_sampling) && (mode == old_mode)) {
      return true;
    }
    old_sampling = sampling;
    old_mode = mode;
    bool success = false;
    switch(mode) {
    case 3 :
      // Mono
      success = sounddev_.set_audio_parameters(0, sampling, 16);
      stereo = false;
      break;
    default :
      // 2 channels (set stereo on)
      success = sounddev_.set_audio_parameters(1, sampling, 16);
      stereo = true;
      break;
    }
    return success;
  }

  return false;
}

//===Idle======Idle======Idle======Idle======Idle======Idle======Idle===

bool MP3Task :: idle_Timeout (Message *msg) 
{
  if (setupround_) { // If this is the first (setup) round, send setup
    setupround_ = false;

    RtpSAP :: Setup *setup = (RtpSAP :: Setup *) rtpdown.create(RtpSAP :: setup);
    
    setup->dest = destaddr_;
    setup->ttl = ttl_;
    setup->items = &items_->copy();
    
    rtpdown.putMessage(setup);
    
    // Send "start sending"-message if needed
    if (start_sending_) {
      // Start message
      RtpSAP :: Start *start = 
	(RtpSAP :: Start *) rtpdown.create(RtpSAP :: start);
      
      start->flags = 0;
      
      rtpdown.putMessage(start);

      // Return without starting timer again because we are only sending
      return false;
    }
  }

#if _CF_WITH_LAME
  Byte* mp3buffer = NULL;
  short samplebuf[2][FBUF_BUFSIZE];
  unsigned long stereobuf[2 * FBUF_BUFSIZE];

  long counter;
  Frame frame;
  long size = framebuf_.get_frame(counter, frame);
  if (size) {
    bool stereo;
    if (set_audio(frame, stereo)) {
      // Copy frame to array of bytes
      mp3buffer = (Byte*)frame; 

      // Decode the frame
      lame_decode((char*)mp3buffer, size, samplebuf[0], samplebuf[1]);
      
      // Delete buffer
      delete mp3buffer;

      if (stereo) { // Stereo
	for (register int j = 0; j < 1152; j++) {
	  stereobuf[j] = samplebuf[0][j] << 16 & 0xFFFF0000;
	  stereobuf[j] |= samplebuf[1][j] & 0xFFFF;
	}
	sounddev_.write_audio((char*)stereobuf, 4 * 1152);
      } else { // Mono
	sounddev_.write_audio((char*)samplebuf[0], 2 * 1152);
      }

    }
  }

  // Start timer again to call this function for the next frame
  sendTimer_.start();
#endif

  return false;
}

bool MP3Task :: idle_Sr (Message *msg)
{
  RtcpSAP :: Srdti *sr = (RtcpSAP :: Srdti *) msg;

  return false;
}

bool MP3Task :: idle_Rr (Message *msg)
{
  RtcpSAP :: Rrdti *rr = (RtcpSAP :: Rrdti *) msg;

  return false;
}

bool MP3Task :: idle_Sdes (Message *msg)
{
  RtcpSAP :: Sdesdti *sdes = (RtcpSAP :: Sdesdti *) msg;
  
  return false;
}
bool MP3Task :: idle_Bye (Message *msg)
{
  RtcpSAP :: Byedti *bye = (RtcpSAP :: Byedti *) msg;

  return false;
}
bool MP3Task :: idle_App (Message *msg)
{
  RtcpSAP :: Appdti *app = (RtcpSAP :: Appdti *) msg;

  return false;
}

bool MP3Task :: idle_Dti (Message *msg)
{
  RtpSAP :: Dti *dti = (RtpSAP :: Dti *) msg;

  int len = dti->payload.length();

  if (dti->sequence_number == sequence_) {
    // Sometimes due to e.g. misconfigured router, multicast
    // packets are received twice or even more times. If
    // previous packet had the same sequence number, drop
    // this and wait for a new one.
    dti->payload.destroy();
    return false;
  } 
  if (dti->sequence_number == (sequence_ + 2)) {
    // A packet has dropped from stream, request it to be resent
    if (rtcp_) {
      rtcp_->send_app(110, "RSND", sequence_ + 1);
    }
    cerr << "Sequences: " << dti->sequence_number << ", " << sequence_;
    cerr << endl;
  }
  sequence_ = dti->sequence_number;
  
  // Remove RTP MPEG-specific header (4 bytes)
  dti->payload.split(4);

  // Insert frame to the end of frame buffer
  framebuf_.insert_back(dti->payload, (long)dti->sequence_number);

  dti->payload.destroy();

  return false;
}


