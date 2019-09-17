//Editor-Info: -*- C++ -*-

//  Copyright 2000 
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

$Log: udptask.C,v $
Revision 1.7  2003/08/11 23:58:53  bilhanan
const char* in constructor -> std::string

Revision 1.6  2003/05/15 11:48:12  bilhanan
fixes for std c++ compliance

Revision 1.5  2002/08/14 09:32:30  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.4  2002/07/23 16:38:35  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.3  2002/06/19 16:21:02  ik
TTL and loopback code.

Revision 1.2  2002/06/14 14:57:00  ik
Multicast options to the constructor.

Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.2  2001/06/08 16:46:30  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:28  bilhanan
Full licence clause added into doors 0.1


*/

 
//===========================================================================
//File:     udptask.C
//Date:     14.09.1998
//Subject:  UDP socket task for DOORS
//Desc:     UdpTask controls usockcos -device and provides UdpSap -services
//          to user.
//Author:   Janne Makihonka, TTKK/DMI
//===========================================================================

#include <string> 
#include <doors/udptask.h>


UdpTask :: UdpTask(std::string n, Scheduler *s, IoHandler *io, Address *a1,
		   unsigned char ttl, bool loopback)

  : EventTask (n,s),
    udpdevice (io, this, ttl, loopback),
    up(this), 
    down (this)
{

    down.connect (&udpdevice.iFace);
    
    udpdevice.setPacketSize(65000); 
    udpdevice.connect(a1);
    udpdevice.readBytes(-1);
    STATIC_SYMBOL (down, this);
    STATIC_SYMBOL (up, this);    
}  

UdpTask :: ~UdpTask (void) {
  DELETE_SYMBOL (this);
}


bool UdpTask::execute(EventType type, Message *msg)
{
  _DOORS_ERROR(msg,"no message");
  msg->unlock();
  Message *bMsg = 0;
  
  if (msg->getTarget() == &up) {
    switch (msg->getType ()) {
    case UdpSAP::dtreq: {
      UdpSAP::Dtreq *dtr = (UdpSAP::Dtreq*)msg;
      udpdevice.setDest(dtr->dest);
      udpdevice.write(dtr->data);
    } break;
    case UdpSAP::setup: {
      UdpSAP::Setup *se = (UdpSAP::Setup*)msg;
      std::cerr << "Can't change IP -address in this version !" << std::endl;
    } break;
    default:
      UdpSAP :: Errind *er = (UdpSAP :: Errind *) up.create (UdpSAP :: errind);
      er->code = 1;
      const char reason1[] = "ERROR: Wrong Messagetype!!!";
      er->data.putLast((Byte*)reason1,strlen(reason1)+1);
      bMsg = (Message *) er;    
      up.putMessage (bMsg); 
    }
  }
  else  if (msg->getTarget() ==&down) {
    switch (msg->getType()) {
    case (DevIf :: open) : {
      break;
    }
    case (DevIf :: close) : {
      udpdevice.close();
      getIoHandler()->forget(&udpdevice);
      UdpSAP :: Errind *er = (UdpSAP :: Errind *) up.create (UdpSAP :: errind);
      er->code = 2;
      const char reason2[] = "ERROR: Socket closed!!!";
      er->data.putLast((Byte*)reason2,strlen(reason2)+1);
      bMsg = (Message *) er;    
      up.putMessage (bMsg);
      break;
    }
    case (DevIf :: read) : {
      buffer = ((DevIf::Read *) msg)->data;
      udpdevice.readBytes (-1); 
      UdpSAP :: Dtind *di = (UdpSAP :: Dtind *) up.create (UdpSAP :: dtind);
      di->source = udpdevice.getSource();
      di->data = buffer;
      bMsg = (Message *) di;    
      up.putMessage (bMsg);
    }
    }
  }   
  delete msg;
  return true;
}

 
