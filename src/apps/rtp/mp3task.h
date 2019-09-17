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

$Log: mp3task.h,v $
Revision 1.3  2003/08/11 23:35:36  bilhanan
bundled maintenance commit:
	const char* in constructor -> std::string
	#include ".. " -> #include <doors/..>
	ONS_CF_ -> _CF_

Revision 1.2  2002/07/23 14:28:52  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.4  2001/09/04 12:17:32  ik
Test lines removed.

Revision 1.2  2001/06/08 16:45:43  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.8  2001/06/08 08:40:02  bilhanan
Full licence clause added into doors 0.1


*/
//
//===========================================================================
//File:     mp3task.h
//Date:     20.4.1999
//Desc:     Class definition for MPEG-audio application task
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#ifndef MP3TASK_H
#define MP3TASK_H

#include <string>
#include <sys/time.h>
#include <doors/ptb.h>
#include <doors/payload.h>
#include <doors/rtpsap.h>
#include <doors/rtcpsap.h>
#include <doors/rtcptask.h>
#include <doors/mp3sm.h>
#include <doors/addr.h>
#include <doors/frame.h> 
#include <doors/framebuf.h>
#include <doors/sounddev.h>
#if _CF_WITH_LAME
#include <lame.h>
#endif

class MP3TaskSM;

class MP3Task : public PTask {
public:
  MP3Task (std::string n, Scheduler *s, MP3TaskSM *sm, InetAddr *own,
	   InetAddr *dest, int ttl, bool start);
  virtual ~MP3Task (void);

  void set_rtcp(RtcpTask* rtcp) { rtcp_ = rtcp; }

  bool isTerminated (void) { return terminated_; };

  bool idle_Timeout (Message *msg);
  bool idle_Dti (Message *msg);
  bool idle_Sr (Message *msg);
  bool idle_Rr (Message *msg);
  bool idle_Sdes (Message *msg);
  bool idle_Bye (Message *msg);
  bool idle_App (Message *msg);

  RtpSAP :: User rtpdown;
  RtcpSAP :: User rtcpdown;

private:
  RtcpTask* rtcp_;
  InetAddr ownaddr_;
  InetAddr destaddr_;
  Timer sendTimer_;
  int ttl_;
  bool terminated_;  
  bool sent_;
  bool start_sending_; 
  bool setupround_;
  unsigned short sequence_;

  SoundDev sounddev_;
  FrameBuf framebuf_;
  SrcDesc* items_;

  bool set_audio(Frame& frame, bool &stereo);

};

#endif  // MP3TASK_H












