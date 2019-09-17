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

$Log: rtcptask.C,v $
Revision 1.7  2003/08/11 23:55:18  bilhanan
const char* in constructor -> std::string

Revision 1.6  2003/05/15 11:48:11  bilhanan
fixes for std c++ compliance

Revision 1.5  2002/07/25 15:53:25  ik
Use std::string.

Revision 1.4  2002/07/23 16:38:32  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.3  2002/07/23 13:54:26  hartmanj
Fixed headers

Revision 1.2  2002/06/14 16:23:44  ik
Changes for using the new RtpUdpTask.

Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.3  2001/06/08 16:47:06  bilhanan
Full Licence clause into DOORS 0.2


*/
//
//===========================================================================
//File:     rtcptask.C
//Date:     4.3.1999
//Desc:     State machine functions for RTCP
//
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#include <doors/defs.h>
#include <doors/hsi.h>
#include <doors/rtcpbase.h>
#include <doors/rtpsource.h>
#include <doors/rtptask.h>
#include <doors/rtcptask.h>
#include <doors/rtcpsm.h>
#include <string>
#include <iostream>
#include <sys/time.h>
#include <assert.h>
#include <stdlib.h>

/* Structure to contain information about address of a service provider.  */
#if 0
struct addrinfo
{
  int ai_flags;			/* Input flags.  */
  int ai_family;		/* Protocol family for socket.  */
  int ai_socktype;		/* Socket type.  */
  int ai_protocol;		/* Protocol for socket.  */
  int ai_addrlen;		/* Length of socket address.  */
  struct sockaddr *ai_addr;	/* Socket address for socket.  */
  char *ai_canonname;		/* Canonical name for service location.  */
  struct addrinfo *ai_next;	/* Pointer to next in list.  */
};
#endif

RtcpTask :: RtcpTask (std::string n, Scheduler *s, RtcpTaskSM *_sm, 
		      RtpUdpTask *udp, RtpTask *rtp, bool send_reports)
: PTask (n, s, _sm),
  udptask_(udp), rtptask_(rtp),
  up(this),
  peer (this, &down),
  down (this, &peer),
  terminated_ (false), all_sent_ (false),
  sendTimer_ (this, OTime (1, 0)),
  rtcp_timeout_(-1.0)
{

#ifdef WITH_UI
  STATIC_SYMBOL (up, this);
  STATIC_SYMBOL (peer, this);
  STATIC_SYMBOL (down, this);
  STATIC_SYMBOL (packetcount_, this);
  STATIC_SYMBOL (packets_lost_, this);
  STATIC_SYMBOL (fraction_lost_, this);
  STATIC_SYMBOL (sequence_number_, this);
  STATIC_SYMBOL (jitter_, this);
#endif

  if (send_reports) {
    sendTimer_.start();
  }
}

RtcpTask :: ~RtcpTask (void) {
#ifdef WITH_UI
  DELETE_SYMBOL (this); 
#endif
}

inline double RtcpTask::gettimeofday() const
{
  timeval tv;
  ::gettimeofday(&tv, 0);
  return (tv.tv_sec + 1e-6 * tv.tv_usec);
}

double RtcpTask::rtcp_interval(int members, int senders, double rtcp_bw, 
			       int we_sent, double avg_rtcp_size, int initial)
{
  // Fraction of the RTCP bandwidth to be shared among active senders
  double t; // Interval
  double rtcp_min_time = RTCP_MIN_TIME;
  int n; // # of members for computation

  // Very first call at application start-up uses half the min delay
  // for quicker notification
  if (initial)
    rtcp_min_time /= 2.0;

  // If there were active senders, give them at least a minimum
  // share of the RTCP bandwidth
  n = members;
  if (senders > 0 && senders < members * RTCP_SENDER_BW_FRACTION) {
    if (we_sent) {
      rtcp_bw *= RTCP_SENDER_BW_FRACTION;
      n = senders;
    } else {
      rtcp_bw *= RTCP_RCVR_BW_FRACTION;
      n -= senders;
    }
  }

  // The effective number of sites times the average packet size is
  // the total number of octets sent when each site sends a report
  t = avg_rtcp_size * n / rtcp_bw;
  if (t < rtcp_min_time) t = rtcp_min_time;

  // To avoid traffic bursts from unintended synchronisation with
  // other sites, we then pick our actual next report interval as a 
  // randon number uniformly distributed between 0.5*t and 1.5*t
  return t * (double)(random() + 0.5);
}

void RtcpTask::calc_next_rtcp_timeout(double now, int initial) 
{
  int we_sent = rtptask_->isSending() ? 1 : 0;
  rtcp_timeout_ = now + rtcp_interval(rtptask_->getSourceCount(),
				      we_sent, 100000.0, we_sent, 
				      600, initial);
}

Report* RtcpTask::get_report()
{
  int source_count = rtptask_->getSourceCount();
  if (source_count <= 0)
    return NULL;
  Report* report = new Report[source_count];
  if (report == NULL)
    return NULL;
  for (register int i = 0; i < source_count; i++) {
    RtpSource* rtpsrc = rtptask_->get_source(i);
    if (rtpsrc == NULL)
      return NULL;
    Report* rprt = &(report[i]);
    rprt->ssrc = rtpsrc->getSrcId();
    rprt->fraction_lost = rtpsrc->getFraction();
    rprt->packets_lost = rtpsrc->getLost();
    rprt->highest_sequence = rtpsrc->getMaxSeq();
    rprt->jitter = rtpsrc->getJitter();
    rprt->lsr = rtpsrc->getLSR();
    rprt->dlsr = rtpsrc->getDLSR();
  }
  return report;
}

bool RtcpTask :: send_report()
{
  if (rtptask_->isSending()) {
    // SR
    RtcpPeer :: SR *data = (RtcpPeer :: SR *) peer.create(RtcpPeer :: sr);
    data->destIP = rtptask_->getRTCPDestAddr();
    data->padding = 0;
    data->sr_count = 0;
    data->ssrc = rtptask_->getSSRC();
    struct timeval tv;
    ::gettimeofday(&tv, 0);
    data->ntp_msw = tv.tv_sec;
    // NTP LSW goes from 0 to 2^32 as gettimeofday returns fraction
    // of seconds in microseconds so we make it fit a bit
    data->ntp_lsw = 4294 * tv.tv_usec;
    data->timestamp = tv.tv_sec;
    data->packetcount = rtptask_->getPackets();
    data->octetcount = rtptask_->getOctets();
    data->reportblocks == NULL;
    peer.putMessage(data);
  } else {
    // RR
    RtcpPeer :: RR *data = (RtcpPeer :: RR *) peer.create(RtcpPeer :: rr);
    data->destIP = rtptask_->getRTCPDestAddr();
    data->padding = 0;
    data->rr_count = rtptask_->getSourceCount();
    data->ssrc = ssrc_;
    data->reportblocks = get_report();
    if (data->reportblocks == NULL)
      return true;
    peer.putMessage(data);
    //    delete data->reportblocks;
  }
  return false;
}

void RtcpTask :: send_sdes()
{
  // SDES
  RtcpPeer :: SDES *data = (RtcpPeer :: SDES *) peer.create(RtcpPeer :: sdes);

  data->destIP = rtptask_->getRTCPDestAddr();
  data->padding = 0;
  SrcDesc* srcdesc = rtptask_->getSrcDesc();
  if (srcdesc == NULL) {
    data->source_count = 0;
    data->items = NULL;
  } else {
    data->source_count = 1;
    SrcDesc* srcdesc = rtptask_->getSrcDesc();
    data->items = &srcdesc->copy();
  }

  peer.putMessage(data);
}
    
void RtcpTask :: send_bye()
{
  // BYE
  RtcpPeer :: BYE *data = (RtcpPeer :: BYE *) peer.create(RtcpPeer :: bye);
  data->destIP = rtptask_->getRTCPDestAddr();
  data->padding = 0;
  data->source_count = 1;
  data->reason = new char[14];
  strcpy(data->reason, "Good reason!");
  data->srcs = new Uint32;
  *data->srcs = rtptask_->getSSRC();

  peer.putMessage(data);
}

void RtcpTask :: send_app(int subtype, char* name, long value)
{
  // APP
  RtcpPeer :: APP *data = (RtcpPeer :: APP *) peer.create(RtcpPeer :: app);
  data->destIP = rtptask_->getRTCPDestAddr();
  data->padding = 0;
  data->subtype = subtype;
  Frame appdata;
  long ndata = htonl(value);
  appdata.putLast(ndata >> 24 & 0xFF);
  appdata.putLast(ndata >> 16 & 0xFF);
  appdata.putLast(ndata >>  8 & 0xFF);
  appdata.putLast(ndata       & 0xFF);
  data->data = appdata;
  data->src = rtptask_->getSSRC();
  data->name = name;

  peer.putMessage(data);
}


//===Idle======Idle======Idle======Idle======Idle======Idle======Idle===

bool RtcpTask :: idle_Default (Message *msg)
{
  return false;
}

bool RtcpTask :: idle_Timeout (Message *msg) 
{
  double now = gettimeofday();

  // Start timer again
  sendTimer_.start();

  // Check if timeout is expired. If so, calculate the next
  // timeout and continue to the function
  if (rtcp_timeout_ < 0.0) {
    calc_next_rtcp_timeout(now, 1);
    return false;
  }
  if (now < rtcp_timeout_)
    return false;
  calc_next_rtcp_timeout(now, 0);

  // If this line is reached, just send a report and a sdes
  if (send_report())
    return false;
  send_sdes();

  udptask_->sendMessage();

  return false;
}

bool RtcpTask :: idle_Srdtr (Message *msg) 
{
  timeval tv;

  RtcpSAP :: Srdtr *srdtr = (RtcpSAP :: Srdtr *) msg;
  RtcpPeer :: SR *data = (RtcpPeer :: SR *) peer.create(RtcpPeer :: sr);

  data->destIP = srdtr->destIP;
  data->padding = 0;
  data->sr_count = 1;
  data->ssrc = ssrc_;
  ::gettimeofday(&tv, 0);
  data->ntp_msw = tv.tv_sec;
  // NTP LSW goes from 0 to 2^32 as gettimeofday returns fraction
  // of seconds in microseconds so we make it fit a bit
  data->ntp_lsw = 4294 * tv.tv_usec;
  data->timestamp = srdtr->timestamp;
  data->packetcount = srdtr->packet_count;
  data->octetcount = srdtr->octet_count;
  data->reportblocks = srdtr->reportblocks;
  data->extension = srdtr->extension;

  peer.putMessage(data);
  return false;
}

bool RtcpTask :: idle_Rrdtr (Message *msg) 
{
  timeval tv;

  RtcpSAP :: Rrdtr *rrdtr = (RtcpSAP :: Rrdtr *) msg;
  // At least one report must be there
  if (rrdtr->reportblocks == NULL)
    return false;

  RtcpPeer :: RR *data = (RtcpPeer :: RR *) peer.create(RtcpPeer :: rr);

  data->destIP = rrdtr->destIP;
  data->padding = 0;
  data->rr_count = rrdtr->rr_count;
  data->ssrc = ssrc_;
  data->reportblocks = rrdtr->reportblocks;
  data->extension = rrdtr->extension;

  peer.putMessage(data);
  return false;
}

bool RtcpTask :: idle_Sdesdtr (Message *msg) 
{
  RtcpSAP :: Sdesdtr *sdesdtr = (RtcpSAP :: Sdesdtr *) msg;
  RtcpPeer :: SDES *data = (RtcpPeer :: SDES *) peer.create(RtcpPeer :: sdes);

  data->destIP = sdesdtr->destIP;
  data->padding = 0;
  // Resolve source count
  data->source_count = 1;
  data->items = sdesdtr->items;
  data->extension = sdesdtr->extension;

  peer.putMessage(data);
  return false;
}

bool RtcpTask :: idle_Byedtr (Message *msg) 
{
  RtcpSAP :: Byedtr *byedtr = (RtcpSAP :: Byedtr *) msg;
  RtcpPeer :: BYE *data = (RtcpPeer :: BYE *) peer.create(RtcpPeer :: bye);

  data->destIP = byedtr->destIP;
  data->padding = 0;
  data->source_count = 0;
  data->srcs = NULL;
  data->ssrc = ssrc_;
  data->reason = byedtr->reason;
  data->extension = byedtr->extension;

  peer.putMessage(data);
  return false;
}

bool RtcpTask :: idle_Appdtr (Message *msg) 
{
  RtcpSAP :: Appdtr *appdtr = (RtcpSAP :: Appdtr *) msg;
  RtcpPeer :: APP *data = (RtcpPeer :: APP *) peer.create(RtcpPeer :: app);

  data->destIP = appdtr->destIP;
  data->padding = 0;
  data->subtype = appdtr->subtype;
  data->src = ssrc_;
  data->data = appdtr->data;
  data->extension = appdtr->extension;

  peer.putMessage(data);
  return false;
}

bool RtcpTask :: idle_SR (Message *msg) 
{
  RtcpPeer :: SR *data = (RtcpPeer :: SR *) msg;

  RtcpSAP :: Srdti *srdti = (RtcpSAP :: Srdti *) up.create (RtcpSAP :: srdti);

  srdti->srcIP = data->srcIP;
  srdti->destIP = data->destIP;
  srdti->ssrc = data->ssrc;
  srdti->ntp_msw = data->ntp_msw;
  srdti->ntp_lsw = data->ntp_lsw;
  srdti->timestamp = data->timestamp;
  srdti->packet_count = data->packetcount;
  srdti->octet_count = data->octetcount;
  srdti->sr_count = data->sr_count;
  srdti->reportblocks = data->reportblocks;
  srdti->extension = data->extension;

#ifdef TRACE_RTCP
  char fromaddr[64];
  srdti->srcIP.addrToString(fromaddr, 64);
  std::cerr.precision(7);
  std::cerr.setf(ios::fixed, ios::floatfield);
  std::cerr << gettimeofday() << " RTCP from=" << fromaddr << std::endl;
  std::cerr << "(SR count=" << (int)srdti->sr_count;
  std::cerr << " ssrc=0x" << hex << srdti->ssrc << dec;
  std::cerr << " ntp=" << srdti->ntp_msw << "." << srdti->ntp_lsw;
  std::cerr << " ts=" << srdti->timestamp;
  std::cerr << " pc=" << srdti->packet_count;
  std::cerr << " oc=" << srdti->octet_count;
  for (register int i = 0; i < srdti->sr_count; i++) {
    std::cerr << std::endl;
    Report* report = &(srdti->reportblocks[i]);
    std::cerr << "  (ssrc=0x" << hex << report->ssrc << dec;
    std::cerr << " fraction=" << (double)report->fraction_lost / 256.0;
    std::cerr << " lost=" << report->packets_lost;
    std::cerr << " last_seq=" << report->highest_sequence;
    std::cerr << " jit=" << report->jitter;
    std::cerr << " lsr=" << report->lsr;
    std::cerr << " dlsr=" << report->dlsr << ")";
  }
  std::cerr << ")" << std::endl;
#endif

  up.putMessage(srdti);

  return false;
}

bool RtcpTask :: idle_RR (Message *msg) 
{
  RtcpPeer :: RR *data = (RtcpPeer :: RR *) msg;
  RtcpSAP :: Rrdti *rrdti = 
    (RtcpSAP :: Rrdti *) up.create (RtcpSAP :: rrdti);

  rrdti->srcIP = data->srcIP;
  rrdti->destIP = data->destIP;
  rrdti->rr_count = data->rr_count;
  rrdti->reportblocks = data->reportblocks;
  rrdti->ssrc = data->ssrc;
  rrdti->extension = data->extension;

#ifdef TRACE_RTCP
  char fromaddr[64];
  rrdti->srcIP.addrToString(fromaddr, 64);
  std::cerr.precision(7);
  std::cerr.setf(ios::fixed, ios::floatfield);
  std::cerr << gettimeofday() << " RTCP from=" << fromaddr << std::endl;
  std::cerr << "(RR count=" << (int)rrdti->rr_count;
  std::cerr << " ssrc=0x" << hex << rrdti->ssrc << dec;
  for (register int i = 0; i < rrdti->rr_count; i++) {
    std::cerr << std::endl;
    Report* report = &(rrdti->reportblocks[i]);
    std::cerr << "  (ssrc=0x" << hex << report->ssrc << dec;
    std::cerr << " fraction=" << (double)report->fraction_lost / 256.0;
    std::cerr << " lost=" << report->packets_lost;
    std::cerr << " last_seq=" << report->highest_sequence;
    std::cerr << " jit=" << report->jitter;
    std::cerr << " lsr=" << report->lsr;
    std::cerr << " dlsr=" << report->dlsr << ")";
  }
  std::cerr << ")" << std::endl;
#endif

  up.putMessage(rrdti);

  return false;
}

bool RtcpTask :: idle_SDES (Message *msg) 
{
  RtcpPeer :: SDES *data = (RtcpPeer :: SDES *) msg;
  RtcpSAP :: Sdesdti *sdesdti = 
    (RtcpSAP :: Sdesdti *) up.create (RtcpSAP :: sdesdti);

  sdesdti->srcIP = data->srcIP;
  sdesdti->destIP = data->destIP;
  sdesdti->source_count = data->source_count;
  sdesdti->items = data->items;
  sdesdti->extension = data->extension;

#ifdef TRACE_RTCP
  char fromaddr[64];
  sdesdti->srcIP.addrToString(fromaddr, 64);
  std::cerr.precision(7);
  std::cerr.setf(ios::fixed, ios::floatfield);
  std::cerr << gettimeofday() << " RTCP from=" << fromaddr << std::endl;
  std::cerr << "(SDES count=" << (int)sdesdti->source_count;
  for (register int i = 0; i < sdesdti->source_count; i++) {
    std::cerr << std::endl;
    SrcDesc* item = &(sdesdti->items[i]);
    std::cerr << " (src=0x" << hex << item->src << dec;
    if (!item->cname.empty())
      std::cerr << " CNAME=\"" << item->cname << "\"";
    if (!item->name.empty())
      std::cerr << " NAME=\"" << item->name << "\"";
    if (!item->email.empty())
      std::cerr << " EMAIL=\"" << item->email << "\"";
    if (!item->phone.empty())
      std::cerr << " PHONE=\"" << item->phone << "\"";
    if (!item->loc.empty())
      std::cerr << " LOC=\"" << item->loc << "\"";
    if (!item->tool.empty())
      std::cerr << " TOOL=\"" << item->tool << "\"";
    if (!item->note.empty())
      std::cerr << " NOTE=\"" << item->note << "\"";
    if (!item->priv_prefix.empty()) {
      std::cerr << " PRIV=[" << item->priv_prefix << "=";
      std::cerr << item->priv_value << "]";
    }
    std::cerr << ")";
  }
  std::cerr << ")" << std::endl;
#endif

  up.putMessage(sdesdti);

  // TODO: delete items

  return false;
}

bool RtcpTask :: idle_BYE (Message *msg) 
{
  RtcpPeer :: BYE *data = (RtcpPeer :: BYE *) msg;
  RtcpSAP :: Byedti *byedti = 
    (RtcpSAP :: Byedti *) up.create (RtcpSAP :: byedti);

  byedti->srcIP = data->srcIP;
  byedti->destIP = data->destIP;
  byedti->source_count = data->source_count;
  byedti->reason = data->reason;
  byedti->srcs = data->srcs;
  byedti->extension = data->extension;

#ifdef TRACE_RTCP
  char fromaddr[64];
  byedti->srcIP.addrToString(fromaddr, 64);
  std::cerr.precision(7);
  std::cerr.setf(ios::fixed, ios::floatfield);
  std::cerr << gettimeofday() << " RTCP from=" << fromaddr << std::endl;
  std::cerr << "(BYE count=" << (int)byedti->source_count;
  std::cerr << " reason=\"" << byedti->reason << "\"";
  for (register int i = 0; i < byedti->source_count; i++) {
    std::cerr << std::endl;
    std::cerr << "  (ssrc=0x" << hex << byedti->srcs[i] << dec << ")";
  }
  std::cerr << ")" << std::endl;
#endif

  up.putMessage(byedti);

  return false;
}

bool RtcpTask :: idle_APP (Message *msg) 
{
  RtcpPeer :: APP *data = (RtcpPeer :: APP *) msg;
  RtcpSAP :: Appdti *appdti = 
    (RtcpSAP :: Appdti *) up.create (RtcpSAP :: appdti);

  appdti->srcIP = data->srcIP;
  appdti->destIP = data->destIP;
  appdti->data = data->data;
  appdti->name = data->name;
  appdti->extension = data->extension;

  //#ifdef TRACE_RTCP
#if 1
  std::string fromaddr = appdti->srcIP.addrToString();
  std::cerr.precision(7);
  std::cerr.setf(std::ios::fixed, std::ios::floatfield);
  std::cerr << gettimeofday() << " RTCP from=" << fromaddr << std::endl;
  int datalen = appdti->data.length();
  const int showbytes = 16;
  std::cerr << "(APP data length=" << datalen;
  if (appdti->name != NULL) {
    std::cerr << " name=" << appdti->name[0] << appdti->name[1];
    std::cerr << appdti->name[2] << appdti->name[3] << std::endl;
  }
  std::cerr << " (";
  for (register int i = 0; 
       (i < appdti->data.length()) && (i < showbytes); i++) {
    std::cerr << " ";
    std::cerr << std::hex << (Uint16)appdti->data.read(i) << std::dec;
  }
  if (datalen > showbytes)
    std::cerr << " ...";
  std::cerr << " )" << std::endl;
#endif

  up.putMessage(appdti);

  return false;
}
