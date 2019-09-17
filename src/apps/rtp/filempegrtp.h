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

$Log: filempegrtp.h,v $
Revision 1.2  2003/08/11 23:35:36  bilhanan
bundled maintenance commit:
	const char* in constructor -> std::string
	#include ".. " -> #include <doors/..>
	ONS_CF_ -> _CF_

Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.2  2001/06/08 16:45:30  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.7  2001/06/08 08:40:00  bilhanan
Full licence clause added into doors 0.1


*/
//
//===========================================================================
//File:     filempegrtp.h
//Date:     11.5.1999
//Desc:     Descendant for MpegRtp to support specifically sending of
//          MPEG-audio *files*
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#ifndef FILEMPEGRTP_H
#define FILEMPEGRTP_H

#include <string>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <doors/mpegrtp.h>

/** Descendant for MpegRtp to support specifically sending of MPEG-audio
  *files*.
  */
class FileMpegRtp : public MpegRtp {
public:
  FileMpegRtp (std::string n, Scheduler *s, RtpTaskSM *_sm, InetAddr *own,
	     InetAddr *dest)
    : MpegRtp(n, s, _sm, own, dest) 
  { _bitrate = 0; 
#if _CF_WITH_LAME
    _offset = _size = 0; 
#endif
}

  /** Implementation of RtpPayload::getBuffer that reads its data
      from a file.
      @param frame Data buffer where data is copied
      @param size Bytes to read
      @return Number of bytes written into frame.
  */
  virtual int getBuffer(Frame& frame, int size);

  /** Open a file that is to be used by getBuffer.
      @param ifilename Name of the read buffer file
      @return Zero if successfully opened.
  */
  int open(char* ifilename);

  /** Open a file and reencode frames with given bitrate
      @param ifilename Name of the read buffer file
      @param bitrate Requested bitrate
      @return Zero if successfully opened.
  */
  int open_reencode(char* ifilename, int bitrate);

  /** Close the input buffer file
   */
  void close();

private:
#if _CF_WITH_LAME
  bool mp3_init(int bitrate);
  int makeframe(int& bytes);
  int _offset;
  int _size;
#endif
  std::fstream mp3file; // Input file stream
  int _bitrate;    // !=0 if reencoding is wanted (and encoder is available)
};

#endif
