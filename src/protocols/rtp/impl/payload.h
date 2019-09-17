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

$Log: payload.h,v $
Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.2  2001/06/08 16:45:49  bilhanan
Full Licence clause into DOORS 0.2


*/
//
//===========================================================================
//File:     payload.h
//Date:     16.4.1999
//Subject:  Abstract class for every RTP payload type for use with DOORS
//          RTP implementation.
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <doors/frame.h>

// Some mnemonics
#define RTP_ERROR_INVALID_HEADER        1
#define RTP_ERROR_INVALID_FRAME         2

class RtpPayload {
 public:
  /** The constructor of a payload type.
   */
  RtpPayload()
    : payload_type_(0), delay_(0), timestamp_(0), ticks_(0),
      marker_(0), sequence_(0), octets_(0) { }

  /** Virtual method for getting data for the payload class
      to handle. This should be implemented by the end user to
      return data from a payload source. Default method returns
      zero.
      @param frame Data buffer
      @param size Number of bytes needed
      @return Number of bytes written into frame buffer. Zero if
      no bytes are available.
  */
  virtual int getBuffer(Frame& frame, int size) { return 0; }

  /** Virtual method for getting the next frame to send.
      @param frame Frame to be written
      @return Zero if frame successfully written to the parameter
  */
  virtual int getFrame(Frame& frame) { return 0; }

  /** Virtual method for getting interval of time to play the current
      frame. Default method returns zero.
      @return Time to play the current frame (in microseconds)
  */
  virtual int getTimeout() { return 0; }

  /** Sets payload type number
      @param type Payload type number
  */
  void setType(int type) { payload_type_ = type; }
  
  /** Gets payload type number
      @return Payload type number
  */
  int getType() { return payload_type_; }

  /** Sets the marker bit
   */
  void setMarker() { marker_ = 1; }

  /** Resets the marker bit
   */
  void resetMarker() { marker_ = 0; }

  /** Returns nonzero, if marker bit is set, otherwise zero. Marker
      bit can be set using setMarker(). Calling this method resets
      the marker bit and subsequent calls return zero unless setMarker()
      is called before.
      @return Nonzero, if marker bit is set
  */
  int getMarker() 
    { int tmp = marker_; marker_ = 0; return tmp; }

  /** Resets octet count (number of bytes handled and sent)
   */
  void ResetOctetCount() { octets_ = 0; }

  /** Returns octet count (number of bytes handled and sent since beginning
      or counter reset)
      @return octet count
  */
  unsigned long getOctets() { return octets_; }

  /** Reset sequence number. Should be overloaded by the descendant
      to find some random value.
   */
  virtual void InitSequence() { sequence_ = 0; }

  /** Reset packet count to zero.
   */
  void ResetPacketCount() { packets_ = 0; }

  /** Returns sequence number.
      @return sequence
  */
  unsigned long getSequence() { return sequence_; }

  /** Returns packet count (number of packets sent)
      @return packet count
  */
  unsigned long getPackets() { return packets_; }

  /** Reset octet and packet counters to zero.
   */
  void ResetCounters() { octets_ = packets_ = 0; }

  /** Returns timestamp. Adds timer ticks value to timestamp when
      called. The ticks value can be set using setTicks().
  */
  long getTimestamp() { return timestamp_; }

  /** Set timestamp to initial value. This virtual method should be
      overloaded by the descendant to find some random value.
  */
  virtual void InitTimestamp() { timestamp_ = 0; }

  /** Sets timestamp ticks (=value to add to timestamp for each packet)
      @param value Timestamp ticks
  */
  void setTicks(long value) { ticks_ = value; }

#if 0
  /** This method should be called each time a packet is successfully sent
      to update the state
      @param packet_size Size of the successfully sent packet in bytes
   */
  void update_state(int packet_size)
    { octets_ += packet_size; timestamp_ += ticks_; packets_++; }
#endif

 protected:
  long ticks_;       // Ticks to increase timestamp for every RTP packet
  int payload_type_; // RTP payload type number
  int delay_;        // Delay between RTP packets
  int marker_;       // Marker bit for RTP packet
  unsigned long octets_;      // Octet count
  unsigned long timestamp_;   // Timestamp
  unsigned long packets_;     // Packet count
  unsigned short sequence_;   // Sequence number
};

#endif
