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

$Log: rtptask.C,v $
Revision 1.5  2003/08/11 23:50:44  bilhanan
const char* in constructor -> std::string

Revision 1.4  2003/05/15 11:59:04  bilhanan
deprecated header

Revision 1.3  2003/05/15 11:48:11  bilhanan
fixes for std c++ compliance

Revision 1.2  2002/07/23 16:38:33  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.3  2001/06/08 16:47:11  bilhanan
Full Licence clause into DOORS 0.2


*/
//
//===========================================================================
//File:     rtptask.C
//Date:     4.3.1999
//Desc:     State machine functions for RTP application part.
//
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#include <doors/defs.h>
#include <doors/hsi.h>
#include <doors/rtp.h>
#include <doors/rtpsource.h>
#include <doors/rtptask.h>
#include <doors/rtpsm.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>


RtpTask :: RtpTask (std::string n, Scheduler *s, RtpTaskSM *_sm, 
		    InetAddr *own, InetAddr *dest)
: RTTask (n, s, _sm, 0),
  up(this),
  peer (this, &down),
  down (this, &peer),
  terminated_ (false),
  ownaddr_(*own), destaddr_(*dest),
  marker_(0), items_(NULL),
  payload_type_(0), ttl_(0), all_sent_(false)
{

#ifdef WITH_UI
  STATIC_SYMBOL (up, this);
  STATIC_SYMBOL (peer, this);
  STATIC_SYMBOL (down, this);
  STATIC_SYMBOL (ownaddr_, this);
  STATIC_SYMBOL (destaddr_, this);
  STATIC_SYMBOL (payload_type_, this);
  STATIC_SYMBOL (ttl_, this);
  STATIC_SYMBOL (sequence_number_, this);
  STATIC_SYMBOL (timestamp_, this);
#endif

  // Set RTCP address which is RTP address and RTP port + 1
  rtcpdestaddr_.set(ntohs(destaddr_.getPortNumber()) + 1, 
		    destaddr_.getIpAddress(), 1);

  // Initialise random number
  struct timeval tv;
  ::gettimeofday(&tv, 0);
  srandom(tv.tv_sec + tv.tv_usec);

  terminated_ = false;
  all_sent_ = true;
  ssrc_ = random();
  InitTimestamp();
  InitSequence();
}

RtpTask :: ~RtpTask (void) {
#ifdef WITH_UI
  DELETE_SYMBOL (this); 
#endif
}

void RtpTask :: startSending()
{
  all_sent_ = false;
  terminated_ = false;
  startTimer();
  state = RtpTaskSM::Send;
}

void RtpTask :: InitTimestamp()
{
  timestamp_ = random();
}

void RtpTask :: InitSequence()
{
  sequence_ = random() % 0xFFFF;
}

int RtpTask::getSourceCount()
{
  int count = 0;
  for (register int i = 0; i < MAX_RTP_SRCS; i++)
    if (rtpsrc_[i].isValid())
      count++;
  return count;
}

void
RtpTask::update_data(Uint32 src, InetAddr* srcIP, Uint16 seq, 
		     Uint32 timestamp)
{
  register int i;
  for (i = 0; i < MAX_RTP_SRCS; i++) {
    if (rtpsrc_[i].isValid())
      if (rtpsrc_[i].getSrcId() == src) {
	rtpsrc_[i].update(seq, timestamp);
	return;
      }
  }
  // If this line is reached, this source is not initialised yet
  for (i = 0; i < MAX_RTP_SRCS; i++) {
    if (!rtpsrc_[i].isValid()) {
      rtpsrc_[i].init(seq);
      rtpsrc_[i].update(seq, timestamp);
      rtpsrc_[i].setSrcAddr(src, srcIP);
      return;
    }
  }
  // If this line is reached, the max number of sources is met
}

RtpSource* RtpTask::get_source(int index)
{
  if ((index < 0) || (index > MAX_RTP_SRCS))
    return NULL;
  register int i, j;
  for (i = 0, j = 0; i < MAX_RTP_SRCS; i++) {
    if (rtpsrc_[i].isValid()) {
      if (j == index) {
	return &(rtpsrc_[i]);
      }
      j++;
    }
  }
  return NULL;
}

RtpSource* RtpTask::get_source_by_id(Uint32 src)
{
  for (register int i = 0; i < MAX_RTP_SRCS; i++)
    if (rtpsrc_[i].isValid())
      if (rtpsrc_[i].getSrcId() == src)
	return &(rtpsrc_[i]);
  return NULL;
}

inline double RtpTask::gettimeofday() const
{
  timeval tv;
  ::gettimeofday(&tv, 0);
  return (tv.tv_sec + 1e-6 * tv.tv_usec);
}

void RtpTask::fill_rtp_message(RtpPeer::DATA *dtr, Uint8 marker)
{
  dtr->srcIP = ownaddr_;
  dtr->destIP = destaddr_;
  dtr->ssrc = ssrc_;
  dtr->version = RTP_VERSION;
  dtr->padding = 0;
  dtr->extension = 0;
  dtr->csrc_count = 0;
  dtr->marker = getMarker();
  dtr->payload_type = getType();
  dtr->sequence_number = getSequence();
  dtr->timestamp = getTimestamp();
}

//===Idle======Idle======Idle======Idle======Idle======Idle======Idle===

bool RtpTask :: idle_Start (Message *msg) 
{
  RtpSAP :: Start *start = (RtpSAP :: Start *) msg;

  Uint32 flags = start->flags; // Not used at the moment

  startSending(); // Just start sending

  return false;
}


bool RtpTask :: idle_Setup (Message *msg) 
{
  RtpSAP :: Setup *setup = (RtpSAP :: Setup *) msg;

  destaddr_ = setup->dest;
  ttl_ = setup->ttl;
  items_ = &setup->items->copy();

  return false;
}

bool RtpTask :: idle_Close (Message *msg) 
{
  RtpSAP :: Close *close = (RtpSAP :: Close *) msg;

  if (close->reason) {
    reason_ = close->reason;
    all_sent_ = true;
    stopTimer();
  }

  state = RtpTaskSM::Closed;

  return false;
}

bool RtpTask :: idle_Data (Message *msg) 
{
  RtpPeer :: DATA *data = (RtpPeer :: DATA *) msg;

  RtpSAP :: Dti *dti = (RtpSAP :: Dti *) up.create (RtpSAP :: dti);
  dti->src = data->ssrc;
  dti->payload_type = data->payload_type;
  dti->sequence_number = data->sequence_number;
  dti->timestamp = data->timestamp;
  dti->payload = data->payload;
  dti->srcIP = data->srcIP;

  update_data(dti->src, &dti->srcIP, dti->sequence_number, dti->timestamp);

#ifdef TRACE_RTP
  char fromaddr[64];
  dti->srcIP.addrToString(fromaddr, 64);
  cout.precision(7);
  cout.setf(ios::fixed, ios::floatfield);
  cout << gettimeofday() << " RTP from=" << fromaddr;
  cout << " SRC=0x" << hex << dti->src << dec;
  cout << " TYPE=" << (int)dti->payload_type;
  cout << " SEQ=" << (int)dti->sequence_number;
  cout << " TS=" << dti->timestamp;
  cout << " LEN=" << dti->payload.length();
  cout << endl;
#endif

  up.putMessage(dti);

  return false;
}

bool RtpTask :: closed_Setup (Message *msg) 
{
  RtpSAP :: Setup *setup = (RtpSAP :: Setup *) msg;

  destaddr_ = setup->dest;
  ttl_ = setup->ttl;
  items_ = &setup->items->copy();

  state = RtpTaskSM::Idle;

  return false;
}

bool RtpTask :: send_Close (Message *msg) 
{
  RtpSAP :: Close *close = (RtpSAP :: Close *) msg;

  if (close->reason) {
    reason_ = close->reason;
    all_sent_ = true;
    stopTimer();
  }

  state = RtpTaskSM::Closed;

  return false;
}

int RtpTask :: RTCallback () 
{
  Frame frame;

  if (all_sent_) {
    state = RtpTaskSM::Idle;
    terminated_ = true;
    return 0; // Return zero to give a hint to stop the timer
  }

  // Get frame. If successfully written to 'frame', returns zero
  if (getFrame(frame) != 0) {
    all_sent_ = true; // Otherwise flag all sent
    return getTimeout();
  }

  // Check frame length, if more that zero bytes of size, continue
  // Below zero means some error
  if (frame.length() <= 0) {
    all_sent_ = true; // Some error or zero length frame; flag all sent
    return getTimeout();
  }

  // Create message for RTP peer
  RtpPeer :: DATA *data = (RtpPeer :: DATA *) peer.create(RtpPeer :: data);
    
  // Fill header items
  fill_rtp_message(data, 0);
    
  // Put the frame to message
  data->payload = frame;

  // Send message
  peer.putMessage(data);

  // Return with timeout value
  return getTimeout();
}


