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

$Log: rtpudptask.C,v $
Revision 1.6  2003/08/11 23:50:44  bilhanan
const char* in constructor -> std::string

Revision 1.5  2003/05/15 11:48:11  bilhanan
fixes for std c++ compliance

Revision 1.4  2002/08/14 09:32:29  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 16:38:33  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.2  2002/06/19 16:21:55  ik
TTL and loopback code.

Revision 1.1  2002/06/14 16:27:03  ik
Renamed McUdpTask to RtpUdpTask because multicast features are moved
to UdpTask and UdpSockConn.

Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.2  2001/06/08 16:47:10  bilhanan
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

#include <string>
#include <doors/hsi.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <doors/rtp.h>
#include <doors/rtpudptask.h>
#include <doors/udpsockconn.h>

RtpUdpTask :: RtpUdpTask(std::string n, Scheduler *s, IoHandler *io, 
			 Address *a1, Address *a2, Uint8 ttl, bool loop,
			 bool isrtp)
  : EventTask (n,s),
    udpdevice (io, this), tunneldevice (io, this),
    up(this), 
    down (this), tunnel (this), 
    ttl_(ttl), loop_(loop), isrtp_(isrtp), istunnel_(false)
{ 
  // Save address
  InetAddr* inetaddr = (InetAddr *) a2;
  destaddr_ = *inetaddr;

  down.connect (&udpdevice.iFace);
  //  tunnel.connect (&tunneldevice.iFace);

  bind_address(udpdevice, a2);

  STATIC_SYMBOL (down, this);
  STATIC_SYMBOL (up, this);    
}

RtpUdpTask :: ~RtpUdpTask (void) {
  DELETE_SYMBOL (this);
}

bool RtpUdpTask :: openTunnel(Address* addr)
{
  if (!bind_address(tunneldevice, addr)) {
    InetAddr* inetaddr = (InetAddr*)addr;
    tunneladdr_ = *inetaddr;
    istunnel_ = true;
    return true;
  }
  return false;
}

void RtpUdpTask :: closeTunnel()
{
  if (istunnel_) {
    if (isMulticastAddr(&tunneladdr_))
      ; // unjoin multicast...
    tunneldevice.close();
    istunnel_ = false;
  }
}

bool RtpUdpTask :: isMulticastAddr(Address* addr)
{
  InetAddr* inetaddr = (InetAddr*)addr;
  if (htonl(inetaddr->getIpAddress()) >> 28 == 0x0E)
    return true;
  return false;
}

bool RtpUdpTask :: bind_address(UdpSockConn& dev, Address* addr)
{
  // Set device packet size
  dev.setPacketSize(RTP_MTU); 

  // Try to connect() with given address using Internet addressing
  InetAddr* inetaddr = (InetAddr*)addr;
  if (dev.connect(addr)) {
    std::cerr << "Unable to bind to address " << inetaddr->getHostAddr();
    std::cerr << " port " << inetaddr->getPortNumber() << std::endl;
    return true;
  }

  // Set select() to trigger immediately when incoming UDP packet
  // is in the buffer
  dev.readBytes(StreamDevice::immediate);

  // Get handle to the socket
  int fd = dev.getFd();
  
  return false;
}

int RtpUdpTask::read_rtcp_header(Frame& frame, unsigned int len, 
				bool first_packet)
{
  // There should be at least an amount of bytes for
  // the common header
  if (len < 4) {
    std::cerr << "Error: Invalid RTCP frame size" << std::endl;
    return(0);
  }
  // Get frame type
  Uint8 type = frame.read(1);
  // If type is not between RTCP packet types, return error
  if ((type < RTCP_PT_MIN) || (type > RTCP_PT_MAX)) {
    std::cerr << "Error: Possible RTP packet received in RTCP channel" << std::endl;
    return(0);
  }
  if (first_packet) {
    // Get padding bit
    Uint8 padding = frame.read(0) >> 5 & 1/*bit*/;
    // Padding can't be set in the first RTCP packet
    if (padding) {
      std::cerr << "Error: Padding cannot be set in the first RTCP packet" << std::endl;
      return(0);
    }
#if 0
    // Check that type is between valid RTCP types. The first
    // RTCP packet must be (according to the protocol) RS or RR.
    if ((type != RTCP_PT_SR) && (type != RTCP_PT_RR)) {
      std::cerr << "Error: First RTCP packet must be SR or RR" << std::endl;
      return(0);
    }
#endif
  }
  // Get frame length
  Uint16 length = frame.read(2) << 8;
  length |= frame.read(3) & 0xFF;
  // Length is in 32 bit words minus one.
  length = 4 * (length + 1); // Resolve length in bytes
  // If the length claims to be more that bytes left, error
  if (length > len)
    return(0);
  return length;
}


void RtpUdpTask::sendMessage()
{
  if (isrtp_)
    return;
  if (sendbuffer_.length() > 0) {
    // Send composed buffer down to UDP device
    udpdevice.write(sendbuffer_);
    // Destroy the sent buffer
    sendbuffer_.destroy();
  }
}

bool RtpUdpTask::execute(EventType type, Message *msg)
{
  _DOORS_ERROR(msg,"no message");
  msg->unlock();
  Message *bMsg = 0;
  
  if (msg->getTarget() == &up) {
    switch (msg->getType ()) {
    case UdpSAP::dtreq: {
      UdpSAP::Dtreq *dtr = (UdpSAP::Dtreq*)msg;
      // Set destination address
      udpdevice.setDest(dtr->dest);
      if (isrtp_) {
	// Send data down to UDP task
	udpdevice.write(dtr->data);
      } else {
	// Add data to the end of buffer
	sendbuffer_.combine(dtr->data);
      }
    } break;
    case UdpSAP::setup: {
      UdpSAP::Setup *se = (UdpSAP::Setup*)msg;
      std::cerr << "Can't change IP -address in this version !" << std::endl;
    } break;
    default:
      UdpSAP :: Errind *er = (UdpSAP :: Errind *) up.create (UdpSAP :: errind);
      er->code = 1;
      const char reason1[] = "ERROR: Wrong Messagetype!!!";
      er->data.putLast((Byte*)reason1,strlen(reason1)+1);
      bMsg = (Message *) er;    
      up.putMessage (bMsg); 
    }
  }
  else  if (msg->getTarget() ==&down) {
    switch (msg->getType()) {
    case (DevIf :: open) : {
      break;
    }
    case (DevIf :: close) : {
      udpdevice.close();
      getIoHandler()->forget(&udpdevice);
      UdpSAP :: Errind *er = (UdpSAP :: Errind *) up.create (UdpSAP :: errind);
      er->code = 2;
      const char reason2[] = "ERROR: Receive socket closed!!!";
      er->data.putLast((Byte*)reason2,strlen(reason2)+1);
      bMsg = (Message *) er;    
      up.putMessage (bMsg);
      break;
    }
    case (DevIf :: read) : {
      Frame c(RTP_MTU);
      c = ((DevIf::Read *) msg)->data;
      Frame b = c;
      //      udpdevice.readmore();
      udpdevice.readBytes (-1); 
      int framesize = c.length();
      if (istunnel_) {
	// Set destination address
	tunneldevice.setDest(tunneladdr_);
	// Send data down to UDP task
	tunneldevice.write(c);
	break;
      }
      framesize = b.length();
      // If we are receiving RTP, send the frame as one packet, RTCP
      // should be sent PDU by PDU as they usually appear in groups
      if (isrtp_) {
	UdpSAP :: Dtind *di = (UdpSAP :: Dtind *) up.create (UdpSAP :: dtind);
	di->source = udpdevice.getSource();
	di->data = b;
	bMsg = (Message *) di;    
	up.putMessage (bMsg);
      } else {
	bool first_packet = true;
	for ( ;; ) {
	  int size = read_rtcp_header(b, framesize, first_packet);
	  first_packet = false;
	  // On some error or no bytes left, go away
	  if (size <= 0)
	    break;
	  // Get frame to send up
	  Frame pdu = b.split(size);
	  UdpSAP :: Dtind *di = 
	    (UdpSAP :: Dtind *) up.create (UdpSAP :: dtind);
	  di->source = udpdevice.getSource();
	  di->data = pdu;
	  bMsg = (Message *) di;    
	  up.putMessage (bMsg);
	  framesize -= size;
	  // If the size of the rest of the frame is less than
	  // size of RTCP common header, stop here
	  if (framesize < 4)
	    break;
	}
      }
    }
    }
  } else if (msg->getTarget() == &tunnel) {
    switch (msg->getType()) {
    case (DevIf :: open) : {
      break;
    }
    case (DevIf :: close) : {
      udpdevice.close();
      getIoHandler()->forget(&tunneldevice);
      UdpSAP :: Errind *er = (UdpSAP :: Errind *) up.create (UdpSAP :: errind);
      er->code = 2;
      const char reason2[] = "ERROR: Receive tunnel socket closed!!!";
      er->data.putLast((Byte*)reason2,strlen(reason2)+1);
      bMsg = (Message *) er;    
      up.putMessage (bMsg);
      break;
    }
    case (DevIf :: read) : {
      Frame b = ((DevIf::Read *) msg)->data;
      //      tunneldevice.readmore();
      tunneldevice.readBytes (-1); 
      // Just redirect this to the other UDP address
      // Set destination address
      udpdevice.setDest(destaddr_);
      // Send data down to UDP task
      udpdevice.write(b);
      
    }
    }
  }
  delete msg;
  return true;
}
