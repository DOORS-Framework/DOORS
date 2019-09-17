
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

$Log: base.C,v $
Revision 1.2  2002/08/14 09:31:35  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:10  bilhanan
Full Licence clause into DOORS 0.2


*/

// If size-optimised library is requested, include all the C-files here
#include <doors/doorsconf.h>

// Include headers with templates here to make sure they are 
// generated and possibly in only this one file to avoid multiple
// instances
#include <doors/tqueue.h>
#include <doors/handler.h>
#include <doors/name.h>
#include <doors/osched.h>
#include <doors/portset.h>
#include <doors/symbol.h>
#include <doors/symhc.h>
#include <doors/ttask.h>
#include <doors/vstream.h>

#if WITH_FINAL
#if _CF_MEM
#include "mem/bheader.C"
#include "mem/blockm.C"
#include "mem/blockmm.C"
#include "mem/bstat.C"
#include "mem/memmgr.C"
#include "mem/memopt.C"
#include "mem/plainmm.C"
#include "mem/sbheader.C"
#include "mem/stat.C"
#include "mem/statbm.C"
#include "mem/statbmm.C"
#include "mem/statpmm.C"
#endif
#include "hsi/symbol.C"
#include "hsi/symhand.C"
#include "hsi/symhc.C"
#include "hsi/symiter.C"
#include "hsi/symtable.C"
#include "sys/addr.C"
#include "sys/buffer.C"
#include "sys/device.C"
#include "sys/devif.C"
#include "sys/etask.C"
#include "sys/filedev.C"
#include "sys/handler.C"
#include "sys/hsockos.C"
#include "sys/iface.C"
#include "sys/logger.C"
#include "sys/message.C"
#include "sys/nethdr.C"
#include "sys/nhdros.C"
#include "sys/otask.C"
#include "sys/osched.C"
#include "sys/pipedev.C"
#include "sys/pipeport.C"
#include "sys/port.C"
#include "sys/socentac.C"
#include "sys/sockaddr.C"
#include "sys/sockcos.C"
#include "sys/sockeos.C"
#include "sys/sockhos.C"
#include "sys/sockmsg.C"
#include "sys/soconnac.C"
#include "sys/strdev.C"
#include "sys/timer.C"
#include "sys/ttask.C"
#include "sys/ttydev.C"
#include "sys/udp.C"
#include "sys/udptask.C"
#include "sys/usockcos.C"
#include "sys/vstream.C"
#include "ptb/pdu.C"
#include "ptb/peer.C"
#include "ptb/portmux.C"
#include "ptb/portset.C"
#include "ptb/ptask.C"
#include "ptb/rttask.C"
#include "ptb/sap.C"
#include "ptb/sm.C"
#include "util/cell.C"
#include "util/debug.C"
#include "util/errmgmt.C"
#include "util/frame.C"
#include "util/llist.C"
#include "util/obl_process.C"
#include "util/obl_string.C"
#include "util/otime.C"
#include "util/vdebug.C"
#endif


