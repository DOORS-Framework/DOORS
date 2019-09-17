
//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
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

$Log: protocols.C,v $
Revision 1.4  2002/08/19 08:09:52  hartmanj
Added protocols/tranmux to Makefiles

Revision 1.3  2002/06/17 10:50:41  hartmanj
*** empty log message ***

Revision 1.2  2002/06/17 06:02:45  hartmanj
Added #include sap .C files

Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.2  2001/06/08 16:46:55  bilhanan
Full Licence clause into DOORS 0.2


*/

// If size-optimised library is requested, include all the C-files here
#include <doors/doorsconf.h>
#if WITH_FINAL
#include "rtp/interface/rtpsap.C"
#include "rtp/impl/mcudp.C"
#include "rtp/impl/rtpsource.C"
#include "rtp/impl/rtppdu.C"
#include "rtp/impl/rtpsm.C"
#include "rtp/impl/rtptask.C"
#include "rtcp/interface/rtcpsap.C"
#include "rtcp/impl/rtcp.C"
#include "rtcp/impl/rtcptask.C"
#include "rtcp/impl/rtcppdu.C"
#include "rtcp/impl/rtcpsm.C"
#include "udp/udp.C"
#include "udp/udptask.C"
#include "phys_virtual/interface/phys.C"
#include "phys_virtual/impl/physent.C"
#include "phys_virtual/impl/physsm.C"
#include "link/interface/link.C"
#include "link/impl/linkconn.C"
#include "link/impl/linkcsm.C"
#include "link/impl/linkent.C"
#include "link/impl/linkesm.C"
#include "link/impl/linkpdu.C"
#include "link/impl/sbuf.C"
#include "sap/impl/sapentity.C"
#include "sap/impl/sapsession.C"
#include "sap/impl/sapentitysm.C"
#include "sap/impl/sapsessionsm.C"
#include "sap/interface/sapentitysap.C"
#include "tranmux/impl/tranmuxent.C"
#include "tranmux/impl/tranmuxconn.C"
#include "tranmux/impl/tranmuxcsm.C"
#include "tranmux/impl/tranmuxesm.C"
#include "tranmux/impl/tran.C"
#endif
