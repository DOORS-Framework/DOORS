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

$Log: rtcppdu.C,v $
Revision 1.3  2003/05/15 11:48:10  bilhanan
fixes for std c++ compliance

Revision 1.2  2002/07/23 13:56:06  hartmanj
Fixed headers

Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.3  2001/06/08 16:47:06  bilhanan
Full Licence clause into DOORS 0.2


*/

#include <assert.h>
#include <string>
//#include <netinet/in.h>
#include <doors/ptb.h>
#include <doors/rtcpbase.h>
#include <doors/rtcpsap.h>
#include <doors/rtcppdu.h>
#include <doors/rtcppdu.i>
#include <doors/udp.h>
#include <doors/rtp.h>
#include <doors/rtcp.h>

Frame* rtcpframe;

static int put_sdes_item(Frame& frame, Uint8 type, std::string& item);

#ifndef GET_UINT32
#define GET_UINT32(frame,offset) \
  (((Uint32)((frame).read((offset)) << 24)) | \
   ((Uint32)((frame).read((offset) + 1) << 16)) | \
   ((Uint32)((frame).read((offset) + 2) <<  8)) | \
   ((Uint32)((frame).read((offset) + 3))))
#endif

#ifndef GET_UINT24
#define GET_UINT24(frame,offset) \
  (((Uint32)((frame).read((offset)) << 16)) | \
   ((Uint32)((frame).read((offset) + 1) << 8)) | \
   ((Uint32)((frame).read((offset) + 2))))
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

#ifndef PUT_UINT24
#define PUT_UINT24(frame,value) \
  (frame).putLast((Uint8)((value)>>16&0xFF)); \
  (frame).putLast((Uint8)((value)>> 8&0xFF)); \
  (frame).putLast((Uint8)((value)    &0xFF));
#endif

#ifndef PUT_UINT16
#define PUT_UINT16(frame,value) \
  (frame).putLast((Uint8)((value)>> 8&0xFF)); \
  (frame).putLast((Uint8)((value)    &0xFF));
#endif

int put_sdes_item(Frame& frame, Uint8 type, std::string& item)
{
  if (item.empty())
    return 0;
  frame.putLast(type);
  frame.putLast(item.size());
  int bytes = 2;
  for (register int i = 0; (i < item.size()) && (i < 255); i++) {
    frame.putLast(item[i]);
    bytes++;
  }
  return bytes;
}


// SR: Sender report RTCP packet
//
// 0                   1                   2                   3
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|V=2|P|    RC   |   PT=SR=200   |             length            | header
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|                         SSRC of sender                        |
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//|              NTP timestamp, most significant word             | sender
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ info
//|             NTP timestamp, least significant word             |
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|                         RTP timestamp                         |
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|                     sender's packet count                     |
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|                      sender's octet count                     |
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//|                 SSRC_1 (SSRC of first source)                 | report
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ block
//| fraction lost |       cumulative number of packets lost       |   1
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|           extended highest sequence number received           |
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|                      interarrival jitter                      |
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|                         last SR (LSR)                         |
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|                   delay since last SR (DLSR)                  |
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//|                 SSRC_2 (SSRC of second source)                | report
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ block
//:                               ...                             :   2
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//|                  profile-specific extensions                  |
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Uint16 RtcpPeer :: SR :: decode (Message *msg)
{
  register int i;
  Uint8 padding_length, packet_type, version;
  Uint8 tmp;
  Uint16 tmp16;
  Uint32 tmp32;
  Uint8 count;

  Frame b = ((UdpSAP::Dtind *) msg)->data;
  int packet_length = b.length();

  // Save destination and source addresses
  destIP = ((UdpSAP::Dtind *) msg)->dest;
  srcIP = ((UdpSAP::Dtind *) msg)->source;

  // Get header and check its validity
  // Get first octet
  tmp = b.read(0);
  // Check version
  if ((version = tmp>>6) != RTP_VERSION) {
    std::cout << "Unsupported RTP version (" << version << ")." << std::endl;
    return 0;
  }
  // Is there a padding after the payload?
  padding = tmp >> 5 & 1/*bit*/;
  // Number of CSRC identifiers
  sr_count = tmp & 0x1F/*5 bits*/;

  // Get second octet, packet type
  packet_type = b.read(1);
  if (packet_type != RTCP_PT_SR) {
    std::cout << "Unknown packet type (" << (int)packet_type << ")." << std::endl;
    return 0;
  }

  // Get third and fourth octets, header length
  length = GET_UINT16(b, 2);
  
  // Get source identifier of packet's originator
  //  ssrc = GET_UINT32(b, 4);
  ssrc = GET_UINT32(b,4);

  // Get NTP timestamp, MSW
  //  ntp_msw = GET_UINT32(b, 8);
  ntp_msw = GET_UINT32(b,8);

  // Get NTP timestamp, LSW
  //  ntp_lsw = GET_UINT32(b, 12);
  ntp_lsw = GET_UINT32(b,12);

  // Get RTP timestamp
  //  timestamp = GET_UINT32(b, 16);
  timestamp = GET_UINT32(b,16);

  // Get sender's packetcount
  //  packetcount = GET_UINT32(b, 20);
  packetcount = GET_UINT32(b,20);

  // Get sender's octetcount
  //  octetcount = GET_UINT32(b, 24);
  octetcount = GET_UINT32(b,24);

  // Read sender reports (SR) and use them to fill Report-structures
  reportblocks = NULL;
  if (sr_count > 0) {
    reportblocks = new Report[sr_count];
    assert(reportblocks != NULL);
    for (i = 0; i < sr_count; i++) {
      Report* srtmp = &reportblocks[i];
      // Calculate offset for this report in the frame
      int sr_offset = 28 + 24 * i; // Fixed frame + report# * report size
      srtmp->ssrc = GET_UINT32(b, sr_offset);
      srtmp->fraction_lost = b.read(sr_offset + 4);
      // Get cumulative number of packets lost (24 bits)
      srtmp->packets_lost = GET_UINT24(b, sr_offset + 5);
      srtmp->highest_sequence = GET_UINT32(b, sr_offset + 8);
      srtmp->jitter = GET_UINT32(b, sr_offset + 12);
      srtmp->lsr = GET_UINT32(b, sr_offset + 16);
      srtmp->dlsr = GET_UINT32(b, sr_offset + 20);
    }
  }
  
  nextpdu = b.split(28 + 24 * sr_count);

  // TODO: read padding (= profile-specific extensions)

  return packet_length;
}


Message *RtcpPeer :: SR :: encode (void) 
{
  register int i;
  Uint8 tmp;
  Uint16 tmp16;
  Uint32 tmp32;
  
  UdpSAP::Dtreq *dtreq = new UdpSAP::Dtreq;
  Frame b;
  
  // First octet: Version, padding and report count.
  tmp = 0;
  tmp = (RTP_VERSION&3)<<6;
  tmp |= (padding&1)<<5;
  tmp |= sr_count&0x1F;
  b.putLast(tmp);

  // Second octet: Packet type
  b.putLast(RTCP_PT_SR);

  // Packet length. Filled later.
  PUT_UINT16(b, 0);

  // SSRC
  PUT_UINT32(b, ssrc);

  // NTP timestamp, MSW
  PUT_UINT32(b, ntp_msw);

  // NTP timestamp, LSW
  PUT_UINT32(b, ntp_lsw);

  // RTP timestamp
  PUT_UINT32(b, timestamp);

  // Sender's packet count
  PUT_UINT32(b, packetcount);

  // Sender's octet count
  PUT_UINT32(b, octetcount);

  // If report blocks are set, write them one by one
  if (sr_count > 0) {
    for (i = 0; i < sr_count; i++) {
      // SSRC identity
      PUT_UINT32(b, (&reportblocks[i])->ssrc);
      // Fraction lost
      b.putLast((&reportblocks[i])->fraction_lost);
      // Packets lost (24 bits)
      PUT_UINT24(b, (&reportblocks[i])->packets_lost);
      // Highest sequence
      PUT_UINT32(b, (&reportblocks[i])->highest_sequence);
      // Jitter
      PUT_UINT32(b, (&reportblocks[i])->jitter);
      // Last SR
      PUT_UINT32(b, (&reportblocks[i])->lsr);
      // Delay since last SR
      PUT_UINT32(b, (&reportblocks[i])->dlsr);
    }
  }

  // Add padding if required
  int bytes = 0;
  if (padding&1) {
    int len = extension.length();
    for (i = 0; i < len; i++)
      { b.putLast(extension[i]); bytes++; }
    // If we won't meet 32 bit boundary when padding length is
    // added, add more nulls.
    while((bytes+1) % 4 != 0)
      { b.putLast(0); bytes++; len++; }
    // Add length of padding to the end including length byte
    b.putLast(len+1); bytes++;
  }

  // Put packet length to its place
  tmp16 = 6 + 6 * sr_count + bytes / 4;
  b.write((Uint8)(tmp16>>8&0xFF), 2);
  b.write((Uint8)(tmp16   &0xFF), 3);

  dtreq->dest = destIP;
  dtreq->data = b;
    
  return dtreq;
}

// RR: Receiver report RTCP packet
//
// 0                   1                   2                   3
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|V=2|P|    RC   |   PT=RR=201   |             length            | header
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|                     SSRC of packet sender                     |
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//|                 SSRC_1 (SSRC of first source)                 | report
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ block
//| fraction lost |       cumulative number of packets lost       |   1
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|           extended highest sequence number received           |
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|                      interarrival jitter                      |
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|                         last SR (LSR)                         |
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|                   delay since last SR (DLSR)                  |
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//|                 SSRC_2 (SSRC of second source)                | report
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ block
//:                               ...                             :   2
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//|                  profile-specific extensions                  |
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Uint16 RtcpPeer :: RR :: decode (Message *msg) 
{
  register int i;
  Uint8 padding_length, packet_type, version;
  Uint8 tmp;
  Uint16 tmp16;
  Uint32 tmp32;
  Uint8 count;

  Frame b = ((UdpSAP::Dtind *) msg)->data;
  int packet_length = b.length();

  // Save destination and source addresses
  destIP = ((UdpSAP::Dtind *) msg)->dest;
  srcIP = ((UdpSAP::Dtind *) msg)->source;

  // Get header and check its validity
  // Get first octet
  tmp = b.read(0);
  // Check version
  if ((version = tmp>>6) != RTP_VERSION) {
    std::cout << "Unsupported RTP version (" << version << ")." << std::endl;
    return 0;
  }
  // Is there a padding after the payload?
  padding = tmp >> 5 & 1/*bit*/;
  // Number of CSRC identifiers
  rr_count = tmp & 0x1F/*5 bits*/;

  // Get second octet, packet type
  packet_type = b.read(1);
  if (packet_type != RTCP_PT_RR) {
    std::cout << "Unknown packet type (" << (int)packet_type << ")." << std::endl;
    return 0;
  }

  // Get third and fourth octets, header length
  length = GET_UINT16(b, 2);

  // Get source identifier of packet's originator
  ssrc = GET_UINT32(b, 4);

  // Read receiver reports (RR) and use them to fill 
  // ReceiverReport-structures
  reportblocks = NULL;
  if (rr_count > 0) {
    reportblocks = new Report[rr_count];
    assert(reportblocks != NULL);
    for (i = 0; i < rr_count; i++) {
      Report* rrtmp = &reportblocks[i];
      // Calculate offset for this report in the frame
      int sr_offset = 8 + 24 * i; // Fixed frame + report# * report size
      rrtmp->ssrc = GET_UINT32(b, sr_offset);
      rrtmp->fraction_lost = b.read(sr_offset + 4);
      // Get cumulative number of packets lost (24 bits)
      rrtmp->packets_lost = GET_UINT24(b, sr_offset + 5);
      rrtmp->highest_sequence = GET_UINT32(b, sr_offset + 8);
      rrtmp->jitter = GET_UINT32(b, sr_offset + 12);
      rrtmp->lsr = GET_UINT32(b, sr_offset + 16);
      rrtmp->dlsr = GET_UINT32(b, sr_offset + 20);
    }
  }

  nextpdu = b.split(8 + 24 * rr_count);

  // TODO: read padding (= profile-specific extensions)

  return packet_length;
}

Message *RtcpPeer :: RR :: encode (void) 
{
  register int i;
  Uint8 tmp;
  Uint16 tmp16;
  Uint32 tmp32;
  
  UdpSAP::Dtreq *dtreq = new UdpSAP::Dtreq;
  Frame b;

  // First octet: Version, padding and report count.
  tmp = 0;
  tmp = (RTP_VERSION&3)<<6;
  tmp |= (padding&1)<<5;
  tmp |= rr_count&0x1F;
  b.putLast(tmp);

  // Second octet: Packet type
  b.putLast(RTCP_PT_RR);

  // Third and fourth: Packet length. Filled later.
  PUT_UINT16(b, 0);

  // SSRC
  PUT_UINT32(b, ssrc);

  // If report blocks are set, write them one by one
  if (rr_count > 0) {
    for (i = 0; i < rr_count; i++) {
      // SSRC identity
      PUT_UINT32(b, (&reportblocks[i])->ssrc);
      // Fraction lost
      b.putLast((&reportblocks[i])->fraction_lost);
      // Packets lost (24 bits)
      PUT_UINT24(b, (&reportblocks[i])->packets_lost);
      // Highest sequence
      PUT_UINT32(b, (&reportblocks[i])->highest_sequence);
      // Jitter
      PUT_UINT32(b, (&reportblocks[i])->jitter);
      // Last SR
      PUT_UINT32(b, (&reportblocks[i])->lsr);
      // Delay since last SR
      PUT_UINT32(b, (&reportblocks[i])->dlsr);
    }
  }

  // Add padding if required
  int bytes = 0;
  if (padding&1) {
    int len = extension.length();
    for (i = 0; i < len; i++)
      { b.putLast(extension[i]); bytes++; }
    // If we won't meet 32 bit boundary when padding length is
    // added, add more nulls.
    while((bytes+1) % 4 != 0)
      { b.putLast(0); bytes++; len++; }
    // Add length of padding to the end including length byte
    b.putLast(len+1); bytes++;
  }

  // Put packet length to its place
  tmp16 = 1 + 6 * rr_count + bytes / 4;
  b.write((Uint8)(tmp16>>8&0xFF), 2);
  b.write((Uint8)(tmp16   &0xFF), 3);

  dtreq->dest = destIP;
  dtreq->data = b;
    
  return dtreq;
}

// SDES: Source description RTCP packet
//
// 0                   1                   2                   3
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//|V=2|P|    SC   |  PT=SDES=202  |             length            | header
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//|                          SSRC/CSRC_1                          | chunk
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   1
//|                           SDES items                          |
//|                              ...                              |
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//|                          SSRC/CSRC_2                          | chunk
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   2
//|                           SDES items                          |
//|                              ...                              |
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

Uint16 RtcpPeer :: SDES :: decode (Message *msg) 
{
  register int i, j;
  Uint8 padding_length, packet_type, version;
  Uint8 tmp;
  Uint16 tmp16;
  Uint32 tmp32;
  Uint8 count;

  Frame b = ((UdpSAP::Dtind *) msg)->data;
  int packet_length = b.length();

  // Save destination and source addresses
  destIP = ((UdpSAP::Dtind *) msg)->dest;
  srcIP = ((UdpSAP::Dtind *) msg)->source;

  // Get header and check its validity
  // Get first octet
  tmp = b.read(0);
  // Check version
  if ((version = tmp>>6) != RTP_VERSION) {
    std::cout << "Unsupported RTP version (" << version << ")." << std::endl;
    return 0;
  }
  // Is there a padding after the payload?
  padding = tmp >> 5 & 1/*bit*/;
  // Number of CSRC identifiers
  source_count = tmp & 0x1F/*5 bits*/;

  // Get second octet, packet type
  packet_type = b.read(1);
  if (packet_type != RTCP_PT_SDES) {
    std::cout << "Unknown packet type (" << (int)packet_type << ")." << std::endl;
    return 0;
  }

  // Get third and fourth octets, header length
  length = GET_UINT16(b, 2);

  // Read receiver reports (RR) and use them to fill 
  // ReceiverReport-structures
  items = NULL;
  if (source_count > 0) {
    SrcDesc* lastitem = NULL;
    int offset = 4;
    for (i = 0; i < source_count; i++) {
      SrcDesc* item = new SrcDesc;
      assert(item != NULL);
      if (items == NULL) { // First item, set as list root
	items = item;
      } else { // Not first item, set to the end of the list
	lastitem->setNext(item);
      }
      lastitem = item;
      // Get SSRC/CSRC source identity
      item->src = GET_UINT32(b, offset); offset += 4;
      // Loop thru the SDES items
      for ( ; ; ) {
	// Get item id
	Uint8 id = b.read(offset++);
	// Get item's length in octets
	Uint8 len = b.read(offset++);
	// If this is a private extension item, there is a prefix before
	// value string. Read it first.
	if (id == RTCP_SDES_PRIV) {
	  Uint8 prefix_length = b.read(offset++);
	  // Fill string from the frame
	  for (j = 0; j < prefix_length; j++) {
	    char c = (char)b.read(offset++);
	    // Don't try to add zeros; assume end of string
	    if (c == '\0') 
	      continue; // go thru characters to calculate offset
	    item->priv_prefix += c;
	  }
	}
	// Fill string from the frame
	for (j = 0; j < len; j++) {
	  char c = (char)b.read(offset++);
	  // Don't try to add zeros; assume end of string
	  if (c == '\0') 
	    continue;  // go thru characters to calculate offset
	  // Add character to string
	  switch(id) {
	  case RTCP_SDES_CNAME :
	    item->cname += c;
	    break;
	  case RTCP_SDES_NAME :
	    item->name += c;
	    break;
	  case RTCP_SDES_EMAIL :
	    item->email += c;
	    break;
	  case RTCP_SDES_PHONE :
	    item->phone += c;
	    break;
	  case RTCP_SDES_LOC :
	    item->loc += c;
	    break;
	  case RTCP_SDES_TOOL :
	    item->tool += c;
	    break;
	  }
	}
	// If current offset is zero or packet end is reached,
	// check for the 32 bit boundary and exit item loop
	if ((b.read(offset) == 0) || (offset >= packet_length)) {
	  while(offset % 4 != 0) { offset++; }
	  break;
	}
      }
    }
  }

  return packet_length;
}

Message *RtcpPeer :: SDES :: encode (void) 
{
  register int i, j;
  Uint16 packet_length;
  Uint8 tmp;
  Uint16 tmp16;
  Uint32 tmp32;

  UdpSAP::Dtreq *dtreq = new UdpSAP::Dtreq;
  Frame b;
  
  // First octet: Version, padding and report count.
  tmp = 0;
  tmp = (RTP_VERSION&3)<<6;
  tmp |= (padding&1)<<5;
  tmp |= source_count&0x1F;
  b.putLast(tmp);

  // Second octet: Packet type
  b.putLast(RTCP_PT_SDES);

  // 3rd and 4th octet: Packet length, filled later
  PUT_UINT16(b, 0);

  // If report blocks are set, write them one by one
  int bytes = 0;
  if (source_count > 0) {
    SrcDesc* item = items;
    for (i = 0; i < source_count; i++) {
      if (item == NULL)
	break;
      // SSRC/CSRC identity
      PUT_UINT32(b, item->src); bytes+=4;
      // We are counting how many bytes are added to the packet from
      // now on because itemlist should be in the 32 bit boundary
      // Add a SDES item one by one. If item is set (non-NULL),
      // put_sdes_item write it to the packet
      bytes += ::put_sdes_item(b, RTCP_SDES_NAME, item->name);
      bytes += ::put_sdes_item(b, RTCP_SDES_EMAIL, item->email);
      bytes += ::put_sdes_item(b, RTCP_SDES_PHONE, item->phone);
      bytes += ::put_sdes_item(b, RTCP_SDES_LOC, item->loc);
      bytes += ::put_sdes_item(b, RTCP_SDES_TOOL, item->tool);
      bytes += ::put_sdes_item(b, RTCP_SDES_NOTE, item->note);
      if (!item->priv_prefix.empty()) {
	b.putLast(RTCP_SDES_PRIV); bytes++;
	int prefix_length = item->priv_prefix.size();
	int value_length = item->priv_value.size();
	b.putLast(value_length); bytes++;
	b.putLast(prefix_length); bytes++;
	for (j = 0; j < prefix_length; j++)
	  { b.putLast(item->priv_prefix[j]); bytes++; }
	for (j = 0; j < value_length; j++)
	  { b.putLast(item->priv_value[j]); bytes++; }
      }
      // Add zero to indicate end of itemlist
      b.putLast(0); bytes++;
      // If we are not now in the 32 bit boundary, add zeros to meet it
      while(bytes % 4 != 0)
	{ b.putLast(0); bytes++; }
      item = item->getNext();
    }
  }

  // Add padding if required
  if (padding&1) {
    int len = extension.length();
    for (i = 0; i < len; i++)
      { b.putLast(extension[i]); bytes++; }
    // If we won't meet 32 bit boundary when padding length is
    // added, add more nulls.
    while((bytes+1) % 4 != 0)
      { b.putLast(0); bytes++; len++; }
    // Add length of padding to the end including length byte
    b.putLast(len+1); bytes++;
  }

  // Put packet length to its place
  tmp16 = bytes / 4;
  b.write((Uint8)(tmp16>>8&0xFF), 2);
  b.write((Uint8)(tmp16   &0xFF), 3);

  dtreq->dest = destIP;
  dtreq->data = b;

  return dtreq;
}

// BYE: Goodbye RTCP packet
//
//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |V=2|P|    SC   |   PT=BYE=203  |             length            |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |                           SSRC/CSRC                           |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   :                              ...                              :
//   +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//   |     length    |               reason for leaving             ... (opt)
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Uint16 RtcpPeer :: BYE :: decode (Message *msg) 
{
  register int i;
  Uint8 padding_length, packet_type, version;
  Uint8 tmp;
  Uint16 tmp16;
  Uint32 tmp32;
  Uint8 count;

  Frame b = ((UdpSAP::Dtind *) msg)->data;
  int packet_length = b.length();

  // Save destination and source addresses
  destIP = ((UdpSAP::Dtind *) msg)->dest;
  srcIP = ((UdpSAP::Dtind *) msg)->source;

  // Get header and check its validity
  // Get first octet
  tmp = b.read(0);
  // Check version
  if ((version = tmp>>6) != RTP_VERSION) {
    std::cout << "Unsupported RTP version (" << (int)version << ")." << std::endl;
    return 0;
  }

  // Is there a padding after the payload?
  padding = tmp >> 5 & 1/*bit*/;

  // Number of SSRC/CSRC identifiers
  source_count = tmp & 0x1F/*5 bits*/;

  // Get second octet, packet type
  packet_type = b.read(1);
  if (packet_type != RTCP_PT_BYE) {
    std::cout << "Unknown packet type (" << (int)packet_type << ")." << std::endl;
    return 0;
  }

  // Get third and fourth octets, header length
  length = GET_UINT16(b, 2);

  // Get source identifiers and fill them to a vector if Uint32's
  srcs = NULL;
  for (i = 0; i < source_count; i++) {
    srcs = new Uint32[source_count];
    assert(srcs != NULL);
    srcs[i] = GET_UINT32(b, 4 + i * 4);
  }

  // Get length of the reason string
  tmp = b.read(4 + source_count * 4);
  // If length is nonzero, allocate reason string and fill it
  if (tmp > 0) {
    reason = new char[tmp+1];
    assert(reason != NULL);
    for (i = 0; i < tmp; i++)
      reason[i] = (char)b.read(4 + source_count * 4 + i + 1);
  }

  // TODO: check padding

  return packet_length;
}

Message *RtcpPeer :: BYE :: encode (void) 
{
  register int i, j;
  Uint8 tmp;
  Uint16 tmp16;
  Uint32 tmp32;
  
  UdpSAP::Dtreq *dtreq = new UdpSAP::Dtreq;
  Frame b;
  
  // First octet: Version, padding and report count.
  tmp = 0;
  tmp = (RTP_VERSION&3)<<6;
  tmp |= (padding&1)<<5;
  tmp |= source_count&0x1F;
  b.putLast(tmp);

  // Second octet: Packet type
  b.putLast(RTCP_PT_BYE);

  // 3rd and 4th octet: Packet length, filled later
  PUT_UINT16(b, 0);

  // If sources exist, add them to packet
  if (source_count > 0) {
    for (i = 0; i < source_count; i++)
      PUT_UINT32(b, srcs[i]);
  }

  // If reason is given, add it, otherwise add a 32 bit null value
  // to the frame which is interpreted as a null string. This is,
  // however, not quite clear to me [ik], do you need to add that
  // 32 bit zero or not. At least this must meet the 32 bit boundary.
  int bytes = 0;
  if (reason != NULL) {
    int reason_length = strlen(reason);
    b.putLast(reason_length); bytes++;
    for (j = 0; j < reason_length; j++)
      { b.putLast(*(reason+i)); bytes++; }
    // If we are not now in the 32 bit boundary, add zeros to meet it
    while(bytes % 4 != 0)
      { b.putLast(0); bytes++; }
  } else {
    PUT_UINT32(b, 0);
  }

  // Add padding if required
  if (padding&1) {
    int len = extension.length();
    for (i = 0; i < len; i++)
      { b.putLast(extension[i]); bytes++; }
    // If we won't meet 32 bit boundary when padding length is
    // added, add more nulls.
    while((bytes+1) % 4 != 0)
      { b.putLast(0); bytes++; len++; }
    // Add length of padding to the end including length byte
    b.putLast(len+1); bytes++;
  }

  // Put packet length to its place
  tmp16 = source_count + bytes / 4;
  b.write((Uint8)(tmp16>>8&0xFF), 2);
  b.write((Uint8)(tmp16   &0xFF), 3);

  dtreq->dest = destIP;
  dtreq->data = b;

  return dtreq;
}

// APP: Application-defined RTCP packet
//
//    0                   1                   2                   3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |V=2|P| subtype |   PT=APP=204  |             length            |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |                           SSRC/CSRC                           |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |                          name (ASCII)                         |
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |                   application-dependent data                 ...
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Uint16 RtcpPeer :: APP :: decode (Message *msg) 
{
  register int i;
  Uint8 padding_length, version, packet_type;
  Uint8 tmp;
  Uint16 tmp16;
  Uint32 tmp32;
  Uint8 count;

  Frame b = ((UdpSAP::Dtind *) msg)->data;
  int packet_length = b.length();

  // Save destination and source addresses
  destIP = ((UdpSAP::Dtind *) msg)->dest;
  srcIP = ((UdpSAP::Dtind *) msg)->source;

  // Get header and check its validity
  // Get first octet
  tmp = b.read(0);
  // Check version
  if ((version = tmp>>6) != RTP_VERSION) {
    std::cout << "Unsupported RTP version (" << (int)version << ")." << std::endl;
    return 0;
  }

  // Is there a padding after the payload?
  padding = tmp >> 5 & 1/*bit*/;

  // Number of SSRC/CSRC identifiers
  subtype = tmp & 0x1F/*5 bits*/;

  // Get second octet, packet type
  packet_type = b.read(1);
  if (packet_type != RTCP_PT_APP) {
    std::cout << "Unknown packet type (" << (int)packet_type << ")." << std::endl;
    return 0;
  }

  // Get third and fourth octets, header length
  length = GET_UINT16(b, 2);

  // Get source identity
  src = GET_UINT32(b, 4);

  // Get application name
  name = new char[4];
  name[0] = (char)b.read(8);
  name[1] = (char)b.read(9);
  name[2] = (char)b.read(10);
  name[3] = (char)b.read(11);

  // Write application specific data to a frame
  for (i = 12; i < packet_length; i++)
    data.putLast(b.read(i));

  // TODO: Handle extension

  return packet_length;
}

Message *RtcpPeer :: APP :: encode (void) 
{
  register int i, j;
  Uint8 tmp;
  Uint16 tmp16;
  Uint32 tmp32;
  
  UdpSAP::Dtreq *dtreq = new UdpSAP::Dtreq;
  Frame b;
  
  // First octet: Version, padding and report count.
  tmp = 0;
  tmp = (RTP_VERSION&3)<<6;
  tmp |= (padding&1)<<5;
  tmp |= subtype&0x1F;
  b.putLast(tmp);

  // Second octet: Packet type
  b.putLast(RTCP_PT_APP);

  // 3rd and 4th octet: Packet length, filled later
  PUT_UINT16(b, 0);

  // SSRC/CSRC
  PUT_UINT32(b, src);

  if (name != NULL) {
    b.putLast((Byte)name[0]);
    b.putLast((Byte)name[1]);
    b.putLast((Byte)name[2]);
    b.putLast((Byte)name[3]);
  } else {
    PUT_UINT32(b, 0);
  }

  int bytes = 0;
  for (j = 0; j < data.length(); j++)
    { b.putLast(data.read(j)); bytes++; }

  // If we are not now in the 32 bit boundary, add zeros to meet it
  // The user data should be readily in that boundary, if it is not,
  // it will have leading zeros...
  while(bytes % 4 != 0)
    { b.putLast(0); bytes++; }

  // Add padding if required
  if (padding&1) {
    int len = extension.length();
    for (i = 0; i < len; i++)
      { b.putLast(extension[i]); bytes++; }
    // If we won't meet 32 bit boundary when padding length is
    // added, add more nulls.
    while((bytes+1) % 4 != 0)
      { b.putLast(0); bytes++; len++; }
    // Add length of padding to the end including length byte
    b.putLast(len+1); bytes++;
  }

  // Put packet length to its place
  tmp16 = 2 + bytes / 4;
  b.write((Uint8)(tmp16>>8&0xFF), 2);
  b.write((Uint8)(tmp16   &0xFF), 3);
  
  dtreq->dest = destIP;
  dtreq->data = b;

  return dtreq;
}
  
Sint16 RtcpPeer :: identify (Message *msg) {
  Frame b = ((UdpSAP::Dtind *) msg)->data;
  Uint8 tmp8 = b.read(1);
  switch(tmp8) {
  case RTCP_PT_SR :
    return sr;
  case RTCP_PT_RR :
    return rr;
  case RTCP_PT_SDES :
    return sdes;
  case RTCP_PT_BYE :
    return bye;
  case RTCP_PT_APP :
    return app;
  default :
    return other;
  }
}

#undef GET_UINT16
#undef GET_UINT24
#undef GET_UINT32
#undef PUT_UINT16
#undef PUT_UINT24
#undef PUT_UINT32

