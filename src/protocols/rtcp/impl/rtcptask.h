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

$Log: rtcptask.h,v $
Revision 1.6  2003/08/11 23:55:18  bilhanan
const char* in constructor -> std::string

Revision 1.5  2003/05/15 11:48:11  bilhanan
fixes for std c++ compliance

Revision 1.4  2002/07/23 16:38:32  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.3  2002/07/23 13:48:43  bilhanan
fixed headers

Revision 1.2  2002/06/14 16:23:44  ik
Changes for using the new RtpUdpTask.

Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.2  2001/06/08 16:45:55  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:40:25  bilhanan
Full licence clause added into doors 0.1


*/
//===========================================================================
//File:     rtcptask.h
//Date:     16.4.1999
//Subject:  DOORS/RTP RTCP statemachine task
//
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================


#ifndef RTCPTASK_H
#define RTCPTASK_H

#include <string>
#include <doors/ptb.h>
#include <doors/rtp.h>
#include <doors/rtcp.h>
#include <doors/rtcpsm.h>
#include <doors/rtptask.h>
#include <doors/rtcpsap.h>
#include <doors/rtcppdu.h>
#include <doors/addr.h>
#include <doors/rtpudptask.h>

#include <doors/frame.h> 
#include <iostream>
#include <fstream>

class RtcpTaskSM;

/** RTCP statemachine
  */
class RtcpTask : public PTask {
public:
  /** Constructor
      @param n Task name
      @param s Pointer to a DOORS scheduler
      @param sm Pointer to a DOORS/RTCP scheduler
      @param udp Pointer to a multicast-UDP task
      @param rtp Pointer to a RTP task that this RTCP task is to accompany
      @param own Source (own) internet-address
      @param dest Destination internet-address
      @param send_reports Flag defining whether to send RTCP reports or not,
                          default is yes
  */
  RtcpTask (std::string n, Scheduler *s, RtcpTaskSM *sm, RtpUdpTask *udp,
	    RtpTask *rtp, bool send_reports = true);

  /** Destructor
   */
  virtual ~RtcpTask (void);

  /** Returns nonzero if this task has terminated its job
   */
  bool isTerminated (void) { return terminated_; };

  /** Handler function for upper layer's SETUP-message
      @param msg Message
      @return true, if the message is handled
  */
  bool idle_Setup (Message *msg);

  /** Default handler function for unknown messages
      @param msg Message
      @return true, if the message is handled
  */
  bool idle_Default (Message *msg);

  /** Handler function for timeout-messages
      @param msg Message
      @return true, if the message is handled
  */
  bool idle_Timeout (Message *msg);

  /** Handler function for upper layer's SR-message
      @param msg Message
      @return true, if the message is handled
  */
  bool idle_Srdtr (Message *msg);

  /** Handler function for upper layer's RR-message
      @param msg Message
      @return true, if the message is handled
  */
  bool idle_Rrdtr (Message *msg);

  /** Handler function for upper layer's SDES-message
      @param msg Message
      @return true, if the message is handled
  */
  bool idle_Sdesdtr (Message *msg);

  /** Handler function for upper layer's BYE-message
      @param msg Message
      @return true, if the message is handled
  */
  bool idle_Byedtr (Message *msg);

  /** Handler function for upper layer's APP-message
      @param msg Message
      @return true, if the message is handled
  */
  bool idle_Appdtr (Message *msg);

  /** Handler function for lower (UDP) layer's SR-message
      @param msg Message
      @return true, if the message is handled
  */
  bool idle_SR (Message *msg);

  /** Handler function for lower (UDP) layer's RR-message
      @param msg Message
      @return true, if the message is handled
  */
  bool idle_RR (Message *msg);

  /** Handler function for lower (UDP) layer's SDES-message
      @param msg Message
      @return true, if the message is handled
  */
  bool idle_SDES (Message *msg);

  /** Handler function for lower (UDP) layer's BYE-message
      @param msg Message
      @return true, if the message is handled
  */
  bool idle_BYE (Message *msg);

  /** Handler function for lower (UDP) layer's APP-message
      @param msg Message
      @return true, if the message is handled
  */
  bool idle_APP (Message *msg);

  UdpSAP :: User down;      // SAP to UDP task
  RtcpSAP :: Provider up;   // SAP to RTCP task
  RtcpPeer peer;            // RTCP peer

private:
  RtpTask* rtptask_;        // Accompanying RTP task
  RtpUdpTask* udptask_;     // UDP task for RTP
  InetAddr ownaddr_;        // Own address
  InetAddr destaddr_;       // Destination address
  Uint32 ssrc_;             // Our SSRC id number
  bool terminated_;    // Terminated-flag
  Timer sendTimer_;         // Sending timeout -timer object
  double rtcp_timeout_;     // Next time to send RTCP packets
  bool all_sent_;      // Set true by timeout function when all is sent

  /** Returns the current time using double precision type.
      @return Current time. Decimal part is in microseconds.
  */
  inline double gettimeofday() const;

  /** Return interval for the next RTCP packet
      @param members Members in the RTP conference including us
      @param senders Senders in the RTP conference including us if we are
                     sending
      @param rtcp_bw RTCP bandwidth
      @param we_sent TRUE if we sent RTP packets after sending of the last
                     RTCP report
      @param avg_rtcp_size Average size of a RTCP packet
      @param initial TRUE if this is the first call of this method
      @return Delay to wait before sending of the next RTCP packet
  */
  double rtcp_interval(int members, int senders, double rtcp_bw, int we_sent,
		       double avg_rtcp_size, int initial);

  /** Calculate timestamp for the next RTCP packet
      @param now The current time
      @param initial TRUE if this the first call
  */
  void calc_next_rtcp_timeout(double now, int initial);

  /** Return a report to send
      @return A report according to the current state
  */
  Report* get_report();

  /** Send RTCP BYE message
    */
  void send_bye();

  /** Send an RTCP report according to the current state
      @return false if report is sent
  */
  bool send_report();

  /** Send RTCP SDES message
    */
  void send_sdes();

public:
  /** Send RTCP APP (application specific) message
    */
  void send_app(int subtype, char* name, long data);

};

#endif  // RTCPTASK_H












