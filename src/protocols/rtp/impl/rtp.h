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

$Log: rtp.h,v $
Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.2  2001/06/08 16:45:56  bilhanan
Full Licence clause into DOORS 0.2


*/
//
//===========================================================================
//File:     rtp.h
//Date:     16.4.1999
//Subject:  RTP common things
//
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#ifndef RTP_H
#define RTP_H

#include <sys/time.h>

#define RTP_SEQ_MOD             (1<<16)
#define RTP_MAX_SDES            255

#define RTP_PT_RAW		126	/* XXX */
#define RTP_PT_BVC		22	/* Berkeley video codec */

/* RTP standard content encodings for video */
#define RTP_PT_RGB8		23 	/* 8-bit dithered RGB */
#define RTP_PT_HDCC		24 	/* SGI proprietary */
#define RTP_PT_CELLB		25 	/* Sun CellB */
#define RTP_PT_JPEG		26	/* JPEG */
#define RTP_PT_CUSEEME		27	/* Cornell CU-SeeMe */
#define RTP_PT_NV		28	/* Xerox PARC nv */
#define RTP_PT_PICW		29	/* BB&N PictureWindow */
#define RTP_PT_CPV		30	/* Concept/Bolter/Viewpoint codec */
#define RTP_PT_H261		31	/* ITU H.261 */
#define RTP_PT_MPEG		32 	/* MPEG-I & MPEG-II */
#define RTP_PT_MP2T		33 	/* MPEG-II either audio or video */
#define RTP_PT_H263P	42	/* ITU H.263 */
#define RTP_PT_H263		34 	/* ITU H.263 */

/* backward compat hack for decoding RTPv1 ivs streams */
#define RTP_PT_H261_COMPAT 127

/* RTP standard content encodings for audio */
#define RTP_PT_PCMU		0
#define RTP_PT_CELP		1
#define RTP_PT_GSM		3
#define RTP_PT_DVI		5
#define RTP_PT_LPC		7
#define RTP_PT_MPA             14

/* Offset from UNIX's epoch to the NTP epoch in seconds (NTP's JAN_1970) */
#define RTP_EPOCH_OFFSET	2208988800UL
#define RTP_VERSION 2

#define RTP_M	0x0080	/* Marker: significant event <e.g. frame boundary> */
#define RTP_P	0x2000	/* Padding is present */
#define RTP_X	0x1000	/* Extension Header is present */

#define RTCP_PT_SR	200	/* sender report */
#define RTCP_PT_RR	201	/* receiver report */
#define RTCP_PT_SDES	202	/* source description */
#define 	RTCP_SDES_CNAME	1	/* official name (mandatory) */
#define 	RTCP_SDES_NAME	2	/* personal name (optional) */
#define 	RTCP_SDES_EMAIL	3	/* e-mail addr (optional) */
#define 	RTCP_SDES_PHONE	4	/* telephone # (optional) */
#define 	RTCP_SDES_LOC	5	/* geographical location */
#define 	RTCP_SDES_TOOL	6	/* name/(vers) of app */
#define 	RTCP_SDES_NOTE	7	/* transient messages */
#define 	RTCP_SDES_PRIV	8	/* private SDES extensions */
#define RTCP_PT_BYE	203	/* end of participation */
#define RTCP_PT_APP	204	/* application specific functions */
#define RTCP_PT_MIN     200
#define RTCP_PT_MAX     204

#define		RTCP_SDES_MIN	1
#define		RTCP_SDES_MAX	7

/*
 * Parameters controling the RTCP report rate timer.
 */
#define RTCP_SESSION_BW_FRACTION (0.05)
#define RTCP_MIN_TIME (5.)
#define RTCP_SENDER_BW_FRACTION (0.25)
#define RTCP_RCVR_BW_FRACTION (1. - RTCP_SENDER_BW_FRACTION)
#define RTCP_SIZE_GAIN (1./8.)

/*
 * Largest (user-level) packet size generated by our rtp applications.
 * Individual video formats may use smaller mtu's.
 */
#define RTP_MTU 1024

#define MAXHDR 24

#define COMPENSATION (2.71828 - 1.5)

/* How many packets are sent on one time */
#define RTP_PACKETSONROUND 1

#endif