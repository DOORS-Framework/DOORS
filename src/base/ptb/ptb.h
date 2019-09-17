//
//  Copyright 1999
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
//
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

$Log: ptb.h,v $
Revision 1.4  2003/03/10 11:30:44  ik
IPv6 conditional.

Revision 1.3  2002/06/06 14:34:42  ik
Typo in #defines.

Revision 1.2  2002/06/03 09:19:51  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.3  2002/02/25 17:06:05  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.

Revision 1.2  2001/06/08 16:45:53  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:32  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef PTB_H
#define PTB_H

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <doors/core.h>

#include <doors/ptask.h>
#include <doors/iface.h>
#include <doors/sap.h>
#include <doors/portmux.h>
#include <doors/portset.h>
#include <doors/peer.h>
#include <doors/pdu.h>
#include <doors/sm.h>
#include <doors/addr.h>
#include <doors/unixaddr.h>
#include <doors/inetaddr.h>
#if HAVE_IPV6
#include <doors/inetaddr6.h>
#endif

#endif  // PTB_H
