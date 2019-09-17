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

$Log: defs.h,v $
Revision 1.2  2003/05/10 18:22:55  bilhanan
Obsolete references to OVOPS removed.

Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.2  2001/06/08 16:45:26  bilhanan
Full Licence clause into DOORS 0.2


*/
//
//===========================================================================
//File:     defs.h
//Date:     16.4.1999
//Subject:  DOORS/RTP implementation definitions
//Author:   Ilkka Karvinen, TTKK/DMI
//===========================================================================

#ifndef DEFS_H
#define DEFS_H

// There is a bug in Solaris 2.x (x < 6) in rawsocket, causing
// UDP and TCP checksums to corrupt. Because of this bug, TCP and UDP
// checksums must be set to same as length field, when protocol headers are
// included.
// Change this flag to 1 if bug is fixed, or version is 2.6 or higher,
// or if you are NOT using rawsockets.
#define SOL_RAWSOCK_BUG_FIXED 1  

// If you want to include LEMon, uncomment this:
//#define WITH_UI 

// If you want to have RTCP, uncomment this
#define WITH_RTCP 

// If you want to trace RTCP traffic, uncomment this
//#define TRACE_RTCP

// If you want to trace RTP traffic, uncomment this
//#define TRACE_RTP

#endif
