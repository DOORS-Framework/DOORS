//  Copyright 2002
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.

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

$Log: sapsession.h,v $
Revision 1.8  2003/08/11 13:20:51  bilhanan
now with ipv6 goodness.

Revision 1.7  2003/07/09 15:57:54  bilhanan
general cleanup and code rewrite.

Revision 1.6  2003/04/17 09:04:29  bilhanan
Forced commit, now properly supports multiple sessions, 1 udptask per session.

Revision 1.5  2003/04/17 08:55:05  bilhanan
charstring to std::string conversion

Revision 1.4  2002/07/23 16:38:34  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.3  2002/07/02 08:02:44  hartmanj
Removed sapentity dependency

Revision 1.2  2002/06/18 14:17:01  ik
Fixes: string->std::string etc. #includes added.

Revision 1.1  2002/06/17 06:05:38  hartmanj
Initial revision


*/

#ifndef _sapses_h_
#define _sapses_h_

#include <string>
#include <doors/ptb.h>
#include <doors/udp.h>
#include <doors/udp6.h>
#include <doors/sapentitysap.h>
#include <doors/sbuf.h>

class Address;
class InetAddr;
class UdpTask;
class InetAddr6;
class Udp6Task;
class SAPEntity;
class SAPSessionSM;

class SAPSession : public PTask {


 public:
	
	SAPSession (std::string n, Uint16 id, SAPEntity *e, SAPSessionSM *csm,
		    Address *own, EventTask *udp, Uint8 ip_version);
	virtual ~SAPSession (void);

	bool closed_default (Message *msg);
	bool OpenSession (Message *msg);

	bool active_default (Message *msg);
	bool CloseSession (Message *msg);
	bool PauseSession (Message *msg);

	bool active_timeout (Message *msg);

	bool paused_default (Message *msg);
	bool ResumeSession (Message *msg);

	Uint16 getId (void);

	SAPEntSAP::Provider &up;

	UdpSAP::User down4;
	Udp6SAP::User down6;

	//	ReqIface &down;

	Port entity;

 private:

	int mcast_join(int sockfd, const InetAddr *addr );
	int mcast_set_loop(int sockfd, unsigned char onoff);
	int mcast_set_ttl(int sockfd, unsigned char ttl);
	int set_ttl(int sockfd, int ttl);

	Timer retTimer;
	
	Uint16 cid;
	InetAddr dest_, *own_;
	InetAddr6 dest6_, *own6_;
	UdpTask *udp_;
	Udp6Task *udp6_;
	Byte *sdpdata;
	Uint32 sdplen;
	Uint8 ipversion;
	SBuffer buffer;
};

#endif
