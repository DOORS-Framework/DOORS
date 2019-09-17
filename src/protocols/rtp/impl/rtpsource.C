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

$Log: rtpsource.C,v $
Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.2  2001/06/08 16:47:11  bilhanan
Full Licence clause into DOORS 0.2


*/
//
//===========================================================================
//File:     rtpsource.C
//Date:     16.4.1999
//Subject:  RTP source information object
//
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#include <doors/rtp.h>
#include <doors/rtpsource.h>
#include <sys/time.h>
#include <unistd.h>

#ifndef timersub
#define timersub(a, b, result)                                                \
  do {                                                                        \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;                             \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;                          \
    if ((result)->tv_usec < 0) {                                              \
      --(result)->tv_sec;                                                     \
      (result)->tv_usec += 1000000;                                           \
    }                                                                         \
  } while (0)
#endif

RtpSource::RtpSource()
:  base_seq(0), max_seq(0), bad_seq(RTP_SEQ_MOD + 1), cycles(0),
   received(0), received_prior(0), expected_prior(0), transit(0),
   jitter(0), probation(0/*MIN_SEQUENTAL - 1*/), valid(0),
   ntp_msw(0), ntp_lsw(0)
{
}

void RtpSource::init(Uint16 seq)
{
  base_seq = seq - 1;
  max_seq = seq;
  bad_seq = RTP_SEQ_MOD + 1;
  cycles = 0;
  received = 0;
  received_prior = 0;
  expected_prior = 0;
}

int RtpSource::update(Uint16 seq, Uint32 ts)
{
  Uint16 udelta = seq - max_seq;

  // Save timestamp
  timestamp = ts;

  // Save the time of this update for counting of DLSR
  ::gettimeofday(&latest, 0);

  // Source is not valid until MIN_SEQUENTAL packets with
  // sequental sequence numbers have been received
  if (probation) {
    // Packet is in sequence
    if (seq == max_seq + 1) {
      probation--;
      max_seq = seq;
      if (probation == 0) {
	init(seq);
	received++;
	return 1;
      }
    } else {
      probation = MIN_SEQUENTIAL - 1;
      max_seq = seq;
    }
    return 0;
  } else if (udelta < MAX_DROPOUT) {
    // In order, with permissable gap
    if (seq < max_seq) {
      // Sequence number wrapped - count another 64k cycle
      cycles += RTP_SEQ_MOD;
    }
    max_seq = seq;
  } else if (udelta <= RTP_SEQ_MOD - MAX_MISORDER) {
    // The sequence number made a very large jump
    if (seq == bad_seq) {
      // Two sequental packets. Assume that the other side
      // restarted without telling us so just re-sync
      // (i.e. pretend this was the first packet)
      init(seq);
    } else {
      bad_seq = (seq + 1) & (RTP_SEQ_MOD - 1);
      return 0;
    }
  } else {
    // Duplicate or reordered packet
  }
  received++;
  return 1;
}

unsigned char RtpSource::getFraction() 
{
  if (expected_prior == 0) {
    expected_prior = getExpected();
    received_prior = getReceived();
    return 0;
  }
  int expected = getExpected();
  int expected_interval = expected - expected_prior;
  expected_prior = expected;
  int received_interval = received - received_prior;
  received_prior = received;
  int lost_interval = expected_interval - received_interval;
  unsigned char fraction;
  if (expected_interval == 0 || lost_interval <= 0) fraction = 0;
  else fraction = (lost_interval << 8) / expected_interval;
  return fraction;
}

Uint32 RtpSource::getDLSR() 
{
  struct timeval now, sub;

  ::gettimeofday(&now, 0);

  timersub(&now, &latest, &sub);
  Uint32 tmp = 65536 * sub.tv_sec + (65536 * sub.tv_usec) / 1000000;

  return tmp;
}
