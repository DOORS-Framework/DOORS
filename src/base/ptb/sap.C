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

$Log: sap.C,v $
Revision 1.3  2002/08/14 09:31:40  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/07/23 14:25:20  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:34  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:33  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/ocoremac.h>
#include <doors/sap.h>
#include <doors/debug.h>

ReqIface :: ReqIface (EventTask *host, MessageCreateFunction *funcs, Peer *peer)
: Iface (host, funcs), decoder (peer) {

    _MT_FORWARDLOCK_ASMONITOR(mon, task);
}

ReqIface :: ~ReqIface (void) {

    _MT_FORWARDLOCK_ASMONITOR(mon, task);
}

// - do not mark message in this function
bool ReqIface :: getMessage (Message *msg) {

    _MT_FORWARDLOCK_ASMONITOR(mon, task);

    bool rc = false;
    if (hasDecoder (msg)) {
	// msg->mark (this, decoder); may not used !
	rc = decoder->getMessage (msg);
    } else {
	if (task) {
	    rc = task->save (msg, UserEvent);
	} else {
	    _DOORS_FAULT(0,"request interface has no decoder");
	}
    }
    return rc;
}

#if CLASS_IFACE

IndIface :: IndIface (EventTask *host, MessageCreateFunction *funcs) 
: Iface (host, funcs) {

    _MT_FORWARDLOCK_ASMONITOR(mon, task);
}

IndIface :: ~IndIface (void) {

    _MT_FORWARDLOCK_ASMONITOR(mon, task);
}

#endif  // CLASS_IFACE




