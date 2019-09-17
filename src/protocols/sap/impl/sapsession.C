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

$Log: sapsession.C,v $
Revision 1.10  2004/06/07 16:12:50  bilhanan
SAP header flags patch for IPv6

Revision 1.9  2003/08/11 13:20:50  bilhanan
now with ipv6 goodness.

Revision 1.8  2003/07/09 15:57:54  bilhanan
general cleanup and code rewrite.

Revision 1.7  2003/04/17 09:04:29  bilhanan
Forced commit, now properly supports multiple sessions, 1 udptask per session.

Revision 1.6  2003/04/17 08:54:32  bilhanan
charstring to std::string conversion

Revision 1.5  2003/04/16 11:57:16  bilhanan
Originating IP address bugfix, now sent in network byte order
timeout changed from 300 to 60 seconds

Revision 1.4  2002/07/23 16:38:34  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.3  2002/07/02 08:02:44  hartmanj
Removed sapentity dependency

Revision 1.2  2002/06/18 14:17:01  ik
Fixes: string->std::string etc. #includes added.

Revision 1.1  2002/06/17 06:05:38  hartmanj
Initial revision


*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <doors/sapsession.h>
#include <doors/sapsessionsm.h>
#include <doors/inetaddr.h>
#include <doors/udptask.h>
#include <doors/inetaddr6.h>
#include <doors/udp6task.h>
#include <doors/sapentity.h>

SAPSession::SAPSession (std::string n, Uint16 id, SAPEntity *e, SAPSessionSM *csm,
			Address *own, EventTask *udp, Uint8 ip_version)
	: PTask(n,  e->getScheduler (), csm),
	  up (e->up),
	  down4(udp),
	  down6(udp),
	  entity (this),
	  retTimer (this, OTime (15)),
	  cid(id),
	  dest_(e->dest_),
	  dest6_(e->dest6_),
	  sdpdata(0),
	  sdplen(0),
	  ipversion (ip_version)
{

    if (ip_version == 4) {
      own_ = (InetAddr *) own;
      udp_ = (UdpTask *) udp;
      STATIC_SYMBOL(down4,this);
    } else { // assume ipv6
      own6_ = (InetAddr6 *) own;
      udp6_ = (Udp6Task *) udp;
      STATIC_SYMBOL(down6,this);
    }      
    
    state = SAPSessionSM :: closed; // initial state
    
    STATIC_SYMBOL (cid, this);
    STATIC_SYMBOL (retTimer, this);
    
}

SAPSession::~SAPSession()
{

  DELETE_SYMBOL (this);

  if (own_) delete own_;
  
}

Uint16 SAPSession::getId(void)
{
  return cid;
}

bool SAPSession :: closed_default (Message *msg) {

    return false;
}

bool SAPSession :: active_default (Message *msg) {

    return false;
}

bool SAPSession :: paused_default (Message *msg) {

    return false;
}

bool SAPSession:: OpenSession (Message *msg) {

  SAPEntSAP::Sesreq *creq = (SAPEntSAP::Sesreq *) msg;
  
  Frame *sdpfrm = &(creq->sdpfile);
  
  std::string fname;
  char c;
  FILE *f;
  
  while((c=sdpfrm->getFirst())!=0) {
    fname += c;
  }
  
  f = fopen(fname.c_str(),"r");
  
  if(f!= NULL) {
    
    while(!feof(f)) { fgetc(f); sdplen++; }
    sdplen--;
    
    clearerr(f);
    fseek(f,SEEK_SET,0);
    sdpdata = new Byte[sdplen];
    fread(sdpdata, 1, sdplen, f); // Read SDP definition
    
    fclose(f);
    
  } else {
    vout << "Error: Couldn't open SDP-file " << fname.c_str() << ". Not sending anything.";
    return false;
  }

  Frame *udpdata = new Frame;

  // SAP-header flags (first byte)

  if (ipversion == 4) {
    udpdata->putLast(0x20);
  } else { //assume ipv6
    udpdata->putLast(0x30);
  }
  // auth len = 0x00 (8bits)
  udpdata->putLast(0x00);
  
  // msg id hash
  udpdata->putLast((Byte)((cid>>8)&0x00ff));
  udpdata->putLast((Byte)(cid&0x00ff));
  
  // originating source

  if (ipversion == 4) {
  Uint32 temp = htonl(own_->getIpAddress());
  
  udpdata->putLast((Byte)((temp>>24)&0x000000ff));
  udpdata->putLast((Byte)((temp>>16)&0x000000ff));
  udpdata->putLast((Byte)((temp>>8)&0x000000ff));
  udpdata->putLast((Byte)(temp)&0x000000ff);
  
  } else { //assume ipv6

      own6_->getIpAddress(udpdata);
  }

  // optional authentication data:
  // none
  
  // optional payload type. left out for sdr+nsdr compatibility
  //	const char *payload_type = "application/sdp";
  //	udpdata->putLast((Byte*)payload_type, strlen(payload_type));
  //	udpdata->putLast(0); // terminating zero

  // The payload

  udpdata->putLast(sdpdata,sdplen);

  buffer.add (udpdata);
  
  if (ipversion == 4) {
  UdpSAP::Dtreq *da = (UdpSAP::Dtreq *)down4.create(UdpSAP::dtreq);
  
  da->dest = dest_;
  Frame &frm = buffer.get ()->copy ();
  da->data = frm;
  delete &frm;

  down4.putMessage(da);

  } else { // assume ipv6

  Udp6SAP::Dtreq *da = (Udp6SAP::Dtreq *)down6.create(Udp6SAP::dtreq);
  
  da->dest = dest6_;
  Frame &frm = buffer.get ()->copy ();
  da->data = frm;
  delete &frm;

  down6.putMessage(da);
  }

  retTimer.start ();
  
  state = SAPSessionSM :: active;
  return false;

}

bool SAPSession::CloseSession(Message *msg) {

  retTimer.stop ();
  state = SAPSessionSM :: closed;
  delete this;
  return false;
}

bool SAPSession::PauseSession(Message *msg) {

  retTimer.stop();
  state = SAPSessionSM :: paused;
  return false;
}

bool SAPSession::ResumeSession(Message *msg) {

  retTimer.start();
  state = SAPSessionSM :: active;
  return false;
}

bool SAPSession::active_timeout(Message *msg)
{
    if (buffer.get ()) {

      if (ipversion == 4) {
	UdpSAP::Dtreq *da = (UdpSAP::Dtreq *)down4.create(UdpSAP::dtreq);
	
	da->dest = dest_;
	Frame &frm = buffer.get ()->copy ();
	da->data = frm;
	delete &frm;
	
	down4.putMessage(da);

      } else { //assume ipv6
	
	Udp6SAP::Dtreq *da = (Udp6SAP::Dtreq *)down6.create(Udp6SAP::dtreq);
		
	da->dest = dest6_;
	Frame &frm = buffer.get ()->copy ();
	da->data = frm;
	delete &frm;
	
	down6.putMessage(da);
      }
      
      retTimer.start();
    }
 
    return false;

}

