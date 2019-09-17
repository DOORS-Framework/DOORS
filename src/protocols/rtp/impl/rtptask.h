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

$Log: rtptask.h,v $
Revision 1.5  2003/08/11 23:50:44  bilhanan
const char* in constructor -> std::string

Revision 1.4  2002/07/23 16:38:33  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.3  2002/07/23 09:42:25  ik
Return statements to state functions.

Revision 1.2  2002/07/23 09:13:53  bilhanan
default state functions added.

Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.2  2001/06/08 16:45:57  bilhanan
Full Licence clause into DOORS 0.2


*/
//
//===========================================================================
//File:     rtptask.h
//Date:     4.3.1999
//Desc:     Class definition for RTP statemachine functions 
//
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#ifndef RTPTASK_H
#define RTPTASK_H

#include <string>
//#include <iostream>
#include <doors/rttask.h>
#include <doors/rtcp.h>
#include <doors/rtp.h>
#include <doors/rtpsource.h>
#include <doors/rtpsm.h>
#include <doors/payload.h>
#include <doors/rtpsap.h>
#include <doors/rtppdu.h>
#include <doors/addr.h>
#include <doors/udp.h>

#include <doors/frame.h> 

#define MAX_RTP_SRCS 32

class RtpTaskSM;

/** RTP statemachine
  */
class RtpTask : public RTTask, public RtpPayload {
public:
  /** Constructor
      @param n Task name
      @param s Scheduler
      @param sm State machine
      @param own Our address
      @param dest Destination address
  */
  RtpTask (std::string n, Scheduler *s, RtpTaskSM *sm, InetAddr *own,
	   InetAddr *dest);

  /** Destructor
    */
  virtual ~RtpTask (void);

  /** Returns the number of RTP clients 
      @param Number of RTP clients found
  */
  int getSourceCount();

  /** Start sending (= start timer)
    */
  void startSending();

  /** Stop sending
    */
  void stopSending() { all_sent_ = true; }

  /** Return true if sending is going on
      @return true if sending is going on
  */
  bool isSending() { return all_sent_ ? false : true; }

  /** Return true if the object is ready for terminating
      @return true if the object idling and ready to stop
  */
  bool isTerminated (void) { return terminated_; }

  /** Returns RTP source by index
      @param index Index of RTP source
      @return Pointer to SSRC/CSRC source identifier in the index of 'index'
  */
  RtpSource* get_source(int index);
  
  /** Returns RTP source by SSRC/CSRC identifier
      @param src SSRC/CSRC identifier
      @return Pointer to SSRC/CSRC source with SSRC/CSRC identifier of 'src'
  */
  RtpSource* get_source_by_id(Uint32 src);

  /** Close session callback function at send state
      @param msg DOORS message
      @return If false, message is to be deleted by DOORS
  */
  bool send_Close (Message *msg);

  bool send_Default (Message *msg) { return false; }

  /** Setup message callback function at closed state
      @param msg DOORS message
      @return If false, message is to be deleted by DOORS
  */
  bool closed_Setup (Message *msg);

  bool closed_Default (Message *msg) { return false; }

   /** Setup message callback function at idle state
      @param msg DOORS message
      @return If false, message is to be deleted by DOORS
  */
  bool idle_Setup (Message *msg);

 /** Start sending callback function
      @param msg DOORS message
      @return If false, message is to be deleted by DOORS
  */

  bool idle_Start (Message *msg);

  /** Data indication callback function
      @param msg DOORS message
      @return If false, message is to be deleted by DOORS
  */
  bool idle_Data (Message *msg);

  /** Close session callback function at idle state
      @param msg DOORS message
      @return If false, message is to be deleted by DOORS
  */
  bool idle_Close (Message *msg);

  bool idle_Default (Message *msg) { return false; }
  UdpSAP :: User down;   // SAP to UDP task
  RtpSAP :: Provider up; // SAP to RTP task
  RtpPeer peer;          // RTP peer

  /** Returns our SSRC identifier
      @return Our SSRC identifier
  */
  Uint32 getSSRC() { return ssrc_; }

  /** Returns our TTL
      @return TTL
  */
  Uint8 getTTL() { return ttl_; }

  /** Returns our address
      @return Our address
  */
  InetAddr getOwnAddr() { return ownaddr_; }

  /** Returns destination address
      @return Destination address
  */
  InetAddr getDestAddr() { return destaddr_; }

  /** Returns RTCP destination address. This is usually the same except
      for port number which is calculated as RTP-port number incremented
      with one (1).
  */
  InetAddr getRTCPDestAddr() { return rtcpdestaddr_; }

  /** Returns payload type of the current RTP payload
      @return Payload type number
  */
  Uint8 getPayloadType() { return payload_type_; }

  /** Returns our source description items
      @return Pointer to our source description items
  */
  SrcDesc* getSrcDesc() { return items_; }

  /** Overloaded function from RTTask that is called by RTTask on
      every clock tick or at least with best efforts.
  */
  int RTCallback();

protected:
  SrcDesc* items_;                 // Our source description items

private:
  RtpSource rtpsrc_[MAX_RTP_SRCS]; // Array of RTP sources
  InetAddr ownaddr_;               // Our address
  InetAddr destaddr_;              // Destination address
  InetAddr rtcpdestaddr_;          // RTCP destination address
  Uint8 payload_type_;             // Payload type
  Uint8 ttl_;                      // TTL
  Uint8 marker_;                   // Marker bit
  Uint32 ssrc_;                    // Our SSRC identifier
  std::string reason_;             // Reason for RTCP BYE packet
  bool terminated_;           // true if we are waiting for the end
  bool all_sent_;             // true if all RTP packets sent

  /** Return the current system time using floating point type
      @return The current system time in double type. Decimal values
              represent fraction of second.
  */
  double gettimeofday() const;
  
  /** Fills RTP message using the current state
      @param dtr Data request message
      @param marker Marker bit
  */
  void fill_rtp_message(RtpPeer::DATA *dtr, Uint8 marker);

  /** Updates the current state of a RTP client
      @param src Source identifier
      @param srcIP Source address
      @param seq Sequence number
      @param timestamp Timestamp
  */
  void update_data(Uint32 src, InetAddr* srcIP, Uint16 seq, 
		   Uint32 timestamp);

  /** Give a random initial number for timestamp
    */
  virtual void InitTimestamp();

  /** Give a random initial number for sequence number
    */
  virtual void InitSequence();
};

#endif  // RTPTASK_H












