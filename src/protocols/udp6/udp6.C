
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

$Log: udp6.C,v $
Revision 1.4  2002/08/27 11:26:20  kalliosa
STATIC_SYMBOL added

Revision 1.3  2002/08/20 12:32:53  kalliosa
Added JoinMcast to Udp6Sap

Revision 1.2  2002/06/06 21:53:14  ik
Fixes to IPv6 UDP.

Revision 1.1  2002/06/06 12:52:55  ik
Initial files for IPv6 UdpTask.


*/

 

//
//File: gsrc/udp6.C
//
//Date: Tue Jan 12 15:05:14 1999
//
//Commentary:
//  This is a machine generated file produced by 
// compiler. Editing is not recommended.
//
//Description:
//  This file implements functions of Udp6SAP
//  service access point
//


#include <doors/hsi.h>
#include <doors/udp6.h>


Message *createUdp6SAPDtreq (void) {

  return new Udp6SAP :: Dtreq;
};


Message *createUdp6SAPSetup (void) {

  return new Udp6SAP :: Setup;
};

Message *createUdp6SAPJoinMcast (void) {

  return new Udp6SAP :: JoinMcast;
};
Message *createUdp6SAPLeaveMcast (void) {

  return new Udp6SAP :: LeaveMcast;
};


Message *(*udp6SAPUserMessages []) (void) = {
  createUdp6SAPDtreq,
  createUdp6SAPSetup,
  createUdp6SAPJoinMcast,
  createUdp6SAPLeaveMcast,   
  0 
};


Message *createUdp6SAPDtind (void) {

  return new Udp6SAP :: Dtind;
};


Message *createUdp6SAPErrind (void) {

  return new Udp6SAP :: Errind;
};


Message *(*udp6SAPProviderMessages []) (void) = {
  createUdp6SAPDtind,
  createUdp6SAPErrind,
  0
};


Udp6SAP :: User :: User (EventTask *t, Peer *p)
: ReqIface (t, udp6SAPUserMessages, p) {

}


Udp6SAP :: User :: ~User (void) {

}


Udp6SAP :: Provider :: Provider (EventTask *t)
: IndIface (t, udp6SAPProviderMessages) {

}


Udp6SAP :: Provider :: ~Provider (void) {

}


Udp6SAP :: Dtreq :: Dtreq (void) {

  DYNAMIC_SYMBOL (this, "dtreq", 0);
  STATIC_SYMBOL (dest, this);
  STATIC_SYMBOL (data, this);
}


Udp6SAP :: Dtreq :: Dtreq (const Dtreq &msg)
: dest (msg.dest), data (msg.data) {

  DYNAMIC_SYMBOL (this, "dtreq", 0);
  STATIC_SYMBOL (dest, this);
  STATIC_SYMBOL (data, this);
}


Udp6SAP :: Dtreq :: ~Dtreq (void) {

  DELETE_SYMBOL (this);
}


Message * Udp6SAP :: Dtreq :: clone (void) const {

  return new Dtreq (*this);
}


Udp6SAP :: Setup :: Setup (void) {

  DYNAMIC_SYMBOL (this, "setup", 0);
  STATIC_SYMBOL (ownIp, this);
}


Udp6SAP :: Setup :: Setup (const Setup &msg)
: ownIp (msg.ownIp) {

  DYNAMIC_SYMBOL (this, "setup", 0);
  STATIC_SYMBOL (ownIp, this);
}


Udp6SAP :: Setup :: ~Setup (void) {

  DELETE_SYMBOL (this);
}


Message * Udp6SAP :: Setup :: clone (void) const {

  return new Setup (*this);
}

Udp6SAP :: JoinMcast :: JoinMcast (void) {

  DYNAMIC_SYMBOL (this, "JoinMcast", 0);
  STATIC_SYMBOL (addr, this);
}


Udp6SAP :: JoinMcast :: JoinMcast (const JoinMcast &msg)
: addr( msg.addr ) {

  DYNAMIC_SYMBOL (this, "JoinMcast", 0);
  STATIC_SYMBOL (addr, this);
}


Udp6SAP :: JoinMcast :: ~JoinMcast (void) {

  DELETE_SYMBOL (this);
}


Message * Udp6SAP :: JoinMcast :: clone (void) const {

  return new JoinMcast (*this);
}

Udp6SAP :: LeaveMcast :: LeaveMcast (void) {
   DYNAMIC_SYMBOL (this, "LeaveMcast", 0);
   STATIC_SYMBOL (addr, this);
}


Udp6SAP :: LeaveMcast :: LeaveMcast (const LeaveMcast &msg)
  : addr( msg.addr ) {

   STATIC_SYMBOL (addr, this);
   DYNAMIC_SYMBOL (this, "LeaveMcast", 0);
}


Udp6SAP :: LeaveMcast :: ~LeaveMcast (void) {

  DELETE_SYMBOL (this);
}


Message * Udp6SAP :: LeaveMcast :: clone (void) const {

  return new LeaveMcast (*this);
}


Udp6SAP :: Dtind :: Dtind (void) {

  DYNAMIC_SYMBOL (this, "dtind", 0);
  STATIC_SYMBOL (source, this);
  STATIC_SYMBOL (dest, this);
  STATIC_SYMBOL (data, this);
}


Udp6SAP :: Dtind :: Dtind (const Dtind &msg)
: source (msg.source), dest (msg.dest), data (msg.data) {

  DYNAMIC_SYMBOL (this, "dtind", 0);
  STATIC_SYMBOL (source, this);
  STATIC_SYMBOL (dest, this);
  STATIC_SYMBOL (data, this);
}


Udp6SAP :: Dtind :: ~Dtind (void) {

  DELETE_SYMBOL (this);
}


Message * Udp6SAP :: Dtind :: clone (void) const {

  return new Dtind (*this);
}


Udp6SAP :: Errind :: Errind (void) {

  DYNAMIC_SYMBOL (this, "errind", 0);
  STATIC_SYMBOL (code, this);
  STATIC_SYMBOL (data, this);
}


Udp6SAP :: Errind :: Errind (const Errind &msg)
: code (msg.code), data (msg.data) {

  DYNAMIC_SYMBOL (this, "errind", 0);
  STATIC_SYMBOL (code, this);
  STATIC_SYMBOL (data, this);
}


Udp6SAP :: Errind :: ~Errind (void) {

  DELETE_SYMBOL (this);
}


Message * Udp6SAP :: Errind :: clone (void) const {

  return new Errind (*this);
}


