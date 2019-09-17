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

$Log: mpegrtp.h,v $
Revision 1.3  2003/08/11 23:35:36  bilhanan
bundled maintenance commit:
	const char* in constructor -> std::string
	#include ".. " -> #include <doors/..>
	ONS_CF_ -> _CF_

Revision 1.2  2002/07/23 14:28:52  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.2  2001/06/08 16:45:44  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:40:02  bilhanan
Full licence clause added into doors 0.1


*/
//
//===========================================================================
//File:     mpegrtp.h
//Date:     11.5.1999
//Subject:  Descendant for RtpTask that supports MPEG-audio frames
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#ifndef MPEGRTP_H
#define MPEGRTP_H

#include <string>
#include <doors/frame.h>
#include <doors/rtptask.h>

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

/** Descendant for RtpTask to support MPEG-audio frames
  */
class MpegRtp : public RtpTask {
public:
  /** Constructor
      @param n Task name
      @param s Scheduler
      @param sm State machine
      @param own Our address
      @param dest Destination address
      @param start If set true, the timer is started (=start sending)
  */
  MpegRtp (std::string n, Scheduler *s, RtpTaskSM *sm, InetAddr *own,
	   InetAddr *dest);

  /** Method for getting the next frame to send. This is virtual
      method first declared in RtpPayload class.
      @param frame Frame to be written
      @return Zero if frame is successfully written to the parameter
  */
  virtual int getFrame(Frame& frame);

  /** Method for getting interval of time to play the current frame.
      @return Time to play the current frame in microseconds
  */
  virtual int getTimeout();

  /** Returns header of the current frame
      @param header Header to be returned
      @return Zero, if header if written to parameter
  */
  int getHeader(mpeg_header& header);

private:
  /** Parse RTP header
      @param frame frame to parse
      @return zero if header is found and parsed
  */
  int parse_header(Frame& frame);

  /** Adds RTP header for MPEG payload to a given frame
      @param frame frame to add RTP header for MPEG payload
      @return bytes added to frame
  */
  int add_rtp_header(Frame& frame);

  mpeg_header header_; // Holds the MPEG header information of current frame
};

#endif


