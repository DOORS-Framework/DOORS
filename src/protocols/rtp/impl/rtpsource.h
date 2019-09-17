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

$Log: rtpsource.h,v $
Revision 1.2  2002/07/23 16:38:32  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.2  2001/06/08 16:45:57  bilhanan
Full Licence clause into DOORS 0.2


*/
//
//===========================================================================
//File:     rtpsource.h
//Date:     16.4.1999
//Subject:  RTP source information object
//
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#ifndef RTPSOURCE_H
#define RTPSOURCE_H

#include <doors/ptb.h>
#include <doors/addr.h>

#define MAX_DROPOUT    3000
#define MAX_MISORDER    100
#define MIN_SEQUENTIAL    2

/** RTP source information object
  */
class RtpSource {
public:
  /** Constructor
    */
  RtpSource();

  /** Initialise object
      @param seq The last sequence number received
  */
  void init(Uint16 seq);

  /** Update object state
      @param seq The last sequence number received
      @param timestamp The last timestamp
      @return Nonzero if successfully updated
  */
  int update(Uint16 seq, Uint32 timestamp);

  /** Return max sequence (=the latest sequence received)
      @return Max sequence number
  */
  Uint32 getMaxSeq() { return max_seq; }

  /** Return the number of packets received
      @return Number of packets received
  */
  Uint32 getReceived() { return received; }

  /** Return jitter
      @return Jitter
  */
  Uint32 getJitter() { return jitter; }

  /** Sets source identifier and address for this object. After this
      method is called, the object is set to be valid state object
      for the source that is given in parameters.
      @param s Source identifier
      @param a Source address
  */
  void setSrcAddr(Uint32 s, InetAddr* a) 
    { src = s; addr = *a; valid = 1; probation = MIN_SEQUENTIAL; }

  /** Return source identifier of this object
      @return Source identifier
  */
  Uint32 getSrcId() { return src; }

  /** Return source address of this object
      @return Source address
  */
  InetAddr getIpAddr() { return addr; }

  /** Return timestamp of the last update
      @return Timestamp of the last update
  */
  Uint32 getTimestamp() { return timestamp; }

  /** Return true, if this object is set to be valid state object for
      some source.
      @return true if this is a valid state object
  */
  int isValid() { return valid; }

  /** Set this object as invalid state object for current source
    */
  void setInvalid() { valid = 0; }

  /** Return sequence number for the next expected packet
      @return Sequence number
  */
  int getExpected() { return (cycles + max_seq) - base_seq + 1; }

  /** Get number of lost packets after last update
      @return Number of lost packets
  */
  int getLost() { return getExpected() - received; }

  /** Get fraction after last update
      @return Fraction
  */
  unsigned char getFraction();

  /** Get sequence number for the latest bad sequence
      @return Bad sequence number
  */
  Uint32 getBadSeq() { return bad_seq; }

  /** Set NTP timestamp
      @param m MSB of timestamp
      @param l LSB of timestamp
  */
  void setNTP(Uint32 m, Uint32 l) { ntp_msw = m; ntp_lsw = l; }

  /** Get LSR
      @return LSR
  */
  Uint32 getLSR() { return ((ntp_msw<<16&0xFFFF0000) | (ntp_lsw>>16&0xFFFF)); }

  /** Get DLSR
      @return DLSR
  */
  Uint32 getDLSR();

private:
  Uint32 src;            // SRC id
  InetAddr addr;         // Source IP address
  Uint32 timestamp;      // Last timestamp
  Uint32 ntp_msw;        // NTP Most Significant Bits
  Uint32 ntp_lsw;        // NTP Least Significant Bits
  Uint16 max_seq;        // Highest sequence number seen
  Uint32 cycles;         // Shifted count of seq. number cycles
  Uint32 base_seq;       // Base sequence number
  Uint32 bad_seq;        // Last bad sequence number + 1
  Uint32 probation;      // Seq. packets till source valid
  Uint32 received;       // Packets received
  Uint32 expected_prior; // Packet expected at last interval
  Uint32 received_prior; // Packet received at last interval
  Uint32 transit;        // Relative transit time for prev packet
  Uint32 jitter;         // Estimated jitter
  int valid;             // Set if this class is initialized 
  struct timeval latest; // Timestamp of the latest update
};
  
#endif
