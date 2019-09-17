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

$Log: udp6task.C,v $
Revision 1.11  2007/10/11 07:18:37  bilhanan
debug & devt

Revision 1.10  2007/10/04 08:18:22  bilhanan
mcast_leave bugfix

Revision 1.9  2004/07/14 10:24:26  bilhanan
Udp6SAP::Setup now can change IP address in this version. Use with
caution.

Revision 1.8  2003/08/12 00:00:32  bilhanan
const char* in constructor -> std::string

Revision 1.7  2003/05/15 11:48:12  bilhanan
fixes for std c++ compliance

Revision 1.6  2002/08/20 12:32:53  kalliosa
Added JoinMcast to Udp6Sap

Revision 1.5  2002/08/14 09:32:30  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.4  2002/07/23 16:38:36  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.3  2002/06/19 16:21:24  ik
Hoplimit and loopback code.

Revision 1.2  2002/06/06 21:53:14  ik
Fixes to IPv6 UDP.

Revision 1.1  2002/06/06 12:52:55  ik
Initial files for IPv6 UdpTask.


*/

 
//===========================================================================
//File:     udp6task.C
//Date:     6.6.2002
//Subject:  IPv6 UDP socket task for DOORS
//Desc:     IPv6 version of UdpTask
//Author:   Janne Mäkihonka, TTKK/DMI
//Author:   Ilkka Karvinen, TTKK/DMI (IPv6 modifications)
//===========================================================================

#include <string> 
#include <doors/udp6task.h>


Udp6Task :: Udp6Task(std::string n, Scheduler *s, IoHandler *io, Address *a1,
		     unsigned int hoplimit, bool loopback):
  EventTask (n,s),
  udpdevice (io, this, hoplimit, loopback),
  up(this), 
  down (this){

    down.connect (&udpdevice.iFace);
    
    udpdevice.setPacketSize(65000); 
    udpdevice.connect(a1);
    udpdevice.readBytes(-1);
    STATIC_SYMBOL (down, this);
    STATIC_SYMBOL (up, this);    
}  

Udp6Task :: ~Udp6Task (void) {
  DELETE_SYMBOL (this);
}


bool Udp6Task::execute(EventType type, Message *msg)
{
  _DOORS_ERROR(msg,"no message");
  msg->unlock();
  Message *bMsg = 0;
  
  if (msg->getTarget() == &up) {
    switch (msg->getType ()) {
    case Udp6SAP::dtreq: {
      Udp6SAP::Dtreq *dtr = (Udp6SAP::Dtreq*)msg;
      udpdevice.setDest(dtr->dest);
      udpdevice.write(dtr->data);
    } break;
    case Udp6SAP::setup: {
      Udp6SAP::Setup *se = (Udp6SAP::Setup*)msg;
      udpdevice.connect(&(se->ownIp));
      // getIoHandler()->inform(&udpdevice);
    } break;
    case Udp6SAP::joinmcast: {
      Udp6SAP::JoinMcast *jm = (Udp6SAP::JoinMcast*)msg;
      udpdevice.mcast_join(  (sockaddr_in6* )jm->addr.getAddr() );
    } break;
    case Udp6SAP::leavemcast: {
      Udp6SAP::LeaveMcast *lm = (Udp6SAP::LeaveMcast*)msg;
      udpdevice.mcast_leave(  (sockaddr_in6* )lm->addr.getAddr() );
    } break;
    default:
      Udp6SAP :: Errind *er = (Udp6SAP :: Errind *) up.create (Udp6SAP :: errind);
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
      Udp6SAP :: Errind *er = (Udp6SAP :: Errind *) up.create (Udp6SAP :: errind);
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
      Udp6SAP :: Dtind *di = (Udp6SAP :: Dtind *) up.create (Udp6SAP :: dtind);
      InetAddr6* src = udpdevice.getSource();
      di->source.set(*src);
      di->data = buffer;
      bMsg = (Message *) di;    
      up.putMessage (bMsg);
    }
    }
  }   
  delete msg;
  return true;
}

 
