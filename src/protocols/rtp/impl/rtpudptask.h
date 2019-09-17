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

$Log: rtpudptask.h,v $
Revision 1.3  2003/08/11 23:50:44  bilhanan
const char* in constructor -> std::string

Revision 1.2  2002/07/23 16:38:33  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.1  2002/06/14 16:27:03  ik
Renamed McUdpTask to RtpUdpTask because multicast features are moved
to UdpTask and UdpSockConn.

Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.3  2002/02/25 17:06:05  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.

Revision 1.2  2001/06/08 16:45:41  bilhanan
Full Licence clause into DOORS 0.2


*/

//
//===========================================================================
//File:     rtpudptask.C
//Date:     12.6.2002
//Subject:  Replacement of DOORS udptask.C to support:
//            1. Packing of several packets in one UDP packet
//            2. "Tunnel"-address. It is an address where all of the incoming
//               packets are written immediately to a given tunnel address
//               when they come. This feature allows this object to act as a
//               multicast tunneling program.
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#ifndef RTPUDPTASK_H
#define RTPUDPTASK_H

#include <string>
#include <doors/addr.h>
#include <doors/udp.h> 
#include <doors/util.h>
#include <doors/core.h>
#include <doors/strdev.h>
#include <doors/devif.h>
#include <doors/sockmsg.h>
#include <doors/inetaddr.h>
#include <doors/udpsockconn.h>

class RtpUdpTask : public EventTask {
public:
  /** Constructor
      @param n DOORS task name
      @param s Pointer to a DOORS scheduler
      @param io Pointer to a DOORS I/O-handler
      @param src Source (our) address
      @param dest Destination address or multicast group address
      @param ttl TTL (Time To Live)
      @param loop (Multicast only) If nonzero, multicast sending is 
             looped back
      @param isrtp If nonzero, the object is set to work with an RTP
             layer, meaning: One UDP packet can contain only one RTP
	     packet. If zero, one UDP packet may contain two or more
	     RTCP packets.
  */
  RtpUdpTask(std::string n, Scheduler *s, IoHandler *io,
	     Address *src, Address *dest, Uint8 ttl, bool loop, 
	     bool isrtp = true);

  /** Destructor
   */
  ~RtpUdpTask();

  /** Task executor
   */
  virtual bool execute (EventType type, Message *msg);

  /** (RTCP only) Deliver the composed send-buffer down to the UDP-layer.
   */
  void sendMessage();

  /** Open an RTP/RTCP tunnel to specified address
      @param addr Tunnel address
      @return TRUE if tunnel is opened
  */
  bool openTunnel(Address* addr);

  /** Close the opened RTP/RTCP tunnel
   */
  void closeTunnel();

  UdpSAP :: Provider up;
  DevIf :: User down;
  DevIf :: User tunnel;

private:
  /** Reads RTCP header from a frame and returns its length in bytes
      @param frame RTCP frame
      @param len Frame length in bytes
      @param first_packet If nonzero, the frame should contain the
             first RTCP packet in the UDP pdu which should be, by
             definition, RR or SR. 
      @return RTCP packet length in bytes. Zero if len=0 or no valid
              RTCP packets are found.
  */
  int read_rtcp_header(Frame& frame, unsigned int len, bool first_packet);

  /** Bind an address to specified socket device
      @param dev DOORS socket device
      @param addr Address to be bind
  */
  bool bind_address(UdpSockConn& dev, Address* addr);

  /** Return TRUE if the given address in multicast address space
      @param addr Address
      @return TRUE if the parameter in multicast address space
  */
  bool isMulticastAddr(Address* addr);

  bool loop_;        // True if UDP packets are looping back
  Uint8 ttl_;             // TTL
  InetAddr destaddr_;     // Destination address
  InetAddr tunneladdr_;   // Tunnel address
  bool isrtp_;       // True if this object works for RTP layer
  bool istunnel_;    // True if tunnel address is set and connected

protected:
  UdpSockConn udpdevice;    // UDP device object
  UdpSockConn tunneldevice; // UDP tunnel device object
  Frame sendbuffer_;                // Buffer for sending groups of RTCP pkts
  Frame recvbuffer_;                // Buffer for receiving    -"-
};

#endif

