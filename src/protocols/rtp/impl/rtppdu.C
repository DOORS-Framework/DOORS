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

$Log: rtppdu.C,v $
Revision 1.3  2003/05/15 11:48:11  bilhanan
fixes for std c++ compliance

Revision 1.2  2002/06/14 20:16:00  ik
#include fix.

Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.3  2001/06/08 16:47:10  bilhanan
Full Licence clause into DOORS 0.2


*/
//
//===========================================================================
//File:     rtppdu.C
//Date:     16.4.1999
//Subject:  RTP PDU encoding and decoding methods
//
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#include <doors/rtpsap.h>
#include <doors/rtppdu.h>
#include <doors/rtppdu.i>
#include <doors/rtp.h>
#include <doors/udp.h>

#ifndef GET_UINT32
#define GET_UINT32(frame,offset) \
  (((Uint32)((frame).read((offset)) << 24)) | \
   ((Uint32)((frame).read((offset) + 1) << 16)) | \
   ((Uint32)((frame).read((offset) + 2) <<  8)) | \
   ((Uint32)((frame).read((offset) + 3))))
#endif

#ifndef GET_UINT16
#define GET_UINT16(frame,offset) \
  (((Uint32)((frame).read((offset)) << 8)) | \
   ((Uint32)((frame).read((offset) + 1))))
#endif

#ifndef PUT_UINT32
#define PUT_UINT32(frame,value) \
  (frame).putLast((Uint8)((value)>>24&0xFF)); \
  (frame).putLast((Uint8)((value)>>16&0xFF)); \
  (frame).putLast((Uint8)((value)>> 8&0xFF)); \
  (frame).putLast((Uint8)((value)    &0xFF)); 
#endif

#ifndef PUT_UINT16
#define PUT_UINT16(frame,value) \
  (frame).putLast((Uint8)((value)>> 8&0xFF)); \
  (frame).putLast((Uint8)((value)    &0xFF));
#endif

// RTP data frame
//
//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |V=2|P|X|  CC   |M|     PT      |       sequence number         |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |                           timestamp                           |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |           synchronization source (SSRC) identifier            |
//   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//   |            contributing source (CSRC) identifiers             |
//   |                             ....                              |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Uint16 RtpPeer :: DATA :: decode (Message *msg) 
{
    register int i;
    Uint8 padding_length;

    Frame b = ((UdpSAP::Dtind *) msg)->data;

    int frame_length = b.length();

    // Save destination and source addresses
    destIP = ((UdpSAP::Dtind *) msg)->dest;
    srcIP = ((UdpSAP::Dtind *) msg)->source;

    // Get header and check its validity
    // Get first octet
    Uint8 tmp = b.read(0);
    // Check version
    if ((version = tmp>>6) != RTP_VERSION) {
      std::cout << "Error: Unsupported RTP version (" << (int)version << ").";
      std::cout << std::endl;
      return 0;
    }
    // Is there a padding after the payload?
    padding = tmp >> 5 & 1/*bit*/;
    // Is there an extension?
    extension = tmp >> 4 & 1/*bit*/;
    if (extension) {
      std::cout << "Error: Extension bit cannot be set in this implementation.";
      std::cout << std::endl;
      return 0;
    }
    // Number of CSRC identifiers
    csrc_count = tmp & 0xF/*4 bits*/;
    if (frame_length < (12 + 4 * csrc_count)) {
      std::cout << "Error: Invalid frame length." << std::endl;
      return 0;
    }
    // Get second octet
    tmp = b.read(1);
    // Frame marker bit
    marker = tmp >> 7 & 1/*bit*/;
    // Payload type
    payload_type = tmp & 0x7F/*7 bits*/;
    if ((payload_type >= RTCP_PT_MIN) && (payload_type <= RTCP_PT_MAX)) {
      std::cout << "Error: Possible RTCP header received in RTP channel." << std::endl;
      return 0;
    }

    // Get third and fourth octet (16 bits)
    // Sequence number
    sequence_number = GET_UINT16(b, 2);

    // Get 5th to 8th octet (32 bits)
    // Timestamp
    timestamp = GET_UINT32(b, 4);

    // Get 9th to 12th octet (32 bits)
    // SSRC
    ssrc = GET_UINT32(b, 8);

    // If padding bit is set, resolve its length (last byte of frame)
    padding_length = 0;
    if (padding) {
      padding_length = b.read(b.length()-1);
      if (padding_length > (frame_length - 12)) {
	std::cout << "Error: Invalid padding length." << std::endl;
	return 0;
      }
    }

    int payload_start = 12 + 4 * csrc_count;

    // Copy payload from RTP frame to payload variable
    Byte* tmpbuf = (Byte*)b;
    payload.putLast(tmpbuf + payload_start, 
		    b.length() - padding_length - payload_start);
    delete tmpbuf;

    return frame_length;
}


Message *RtpPeer :: DATA :: encode (void) 
{
    Uint32 tmp32;
    register int i;

    UdpSAP::Dtreq *dtreq = new UdpSAP::Dtreq;
    Frame b(1152);/* = data*/;

    Uint8 tmp;
    // Add version. Padding, extension and CSRC count are zero
    tmp = (Uint8)(RTP_VERSION<<6);
    // Write to the frame
    b.putLast(tmp);

    // Add marker bit
    tmp = (Uint8)((marker&1)<<7);
    // Add payload type
    tmp |= payload_type&0x7F;
    // Write to the frame
    b.putLast(tmp);

    // Write sequence#
    PUT_UINT16(b, sequence_number);

    // Write timestamp
    PUT_UINT32(b, timestamp);

    // Write SSRC
    PUT_UINT32(b, ssrc);

    // Add payload to PDU
    b.combine(payload);

    // Set destination and data to message
    dtreq->dest = destIP;
    dtreq->data = b;

    return dtreq;
}


Sint16 RtpPeer :: identify (Message *msg) {
    return data;
}

#undef GET_UINT16
#undef GET_UINT32
#undef PUT_UINT16
#undef PUT_UINT32

