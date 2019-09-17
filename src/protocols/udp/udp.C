
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

$Log: udp.C,v $
Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.2  2001/06/08 16:46:30  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:28  bilhanan
Full licence clause added into doors 0.1


*/

 

//
//File: gsrc/udp.C
//
//Date: Tue Jan 12 15:05:14 1999
//
//Commentary:
//  This is a machine generated file produced by 
// compiler. Editing is not recommended.
//
//Description:
//  This file implements functions of UdpSAP
//  service access point
//


#include <doors/hsi.h>
#include <doors/udp.h>


Message *createUdpSAPDtreq (void) {

  return new UdpSAP :: Dtreq;
};


Message *createUdpSAPSetup (void) {

  return new UdpSAP :: Setup;
};


Message *(*udpSAPUserMessages []) (void) = {
  createUdpSAPDtreq,
  createUdpSAPSetup,
  0
};


Message *createUdpSAPDtind (void) {

  return new UdpSAP :: Dtind;
};


Message *createUdpSAPErrind (void) {

  return new UdpSAP :: Errind;
};


Message *(*udpSAPProviderMessages []) (void) = {
  createUdpSAPDtind,
  createUdpSAPErrind,
  0
};


UdpSAP :: User :: User (EventTask *t, Peer *p)
: ReqIface (t, udpSAPUserMessages, p) {

}


UdpSAP :: User :: ~User (void) {

}


UdpSAP :: Provider :: Provider (EventTask *t)
: IndIface (t, udpSAPProviderMessages) {

}


UdpSAP :: Provider :: ~Provider (void) {

}


UdpSAP :: Dtreq :: Dtreq (void) {

  DYNAMIC_SYMBOL (this, "dtreq", 0);
  STATIC_SYMBOL (dest, this);
  STATIC_SYMBOL (data, this);
}


UdpSAP :: Dtreq :: Dtreq (const Dtreq &msg)
: dest (msg.dest), data (msg.data) {

  DYNAMIC_SYMBOL (this, "dtreq", 0);
  STATIC_SYMBOL (dest, this);
  STATIC_SYMBOL (data, this);
}


UdpSAP :: Dtreq :: ~Dtreq (void) {

  DELETE_SYMBOL (this);
}


Message * UdpSAP :: Dtreq :: clone (void) const {

  return new Dtreq (*this);
}


UdpSAP :: Setup :: Setup (void) {

  DYNAMIC_SYMBOL (this, "setup", 0);
  STATIC_SYMBOL (ownIp, this);
}


UdpSAP :: Setup :: Setup (const Setup &msg)
: ownIp (msg.ownIp) {

  DYNAMIC_SYMBOL (this, "setup", 0);
  STATIC_SYMBOL (ownIp, this);
}


UdpSAP :: Setup :: ~Setup (void) {

  DELETE_SYMBOL (this);
}


Message * UdpSAP :: Setup :: clone (void) const {

  return new Setup (*this);
}


UdpSAP :: Dtind :: Dtind (void) {

  DYNAMIC_SYMBOL (this, "dtind", 0);
  STATIC_SYMBOL (source, this);
  STATIC_SYMBOL (dest, this);
  STATIC_SYMBOL (data, this);
}


UdpSAP :: Dtind :: Dtind (const Dtind &msg)
: source (msg.source), dest (msg.dest), data (msg.data) {

  DYNAMIC_SYMBOL (this, "dtind", 0);
  STATIC_SYMBOL (source, this);
  STATIC_SYMBOL (dest, this);
  STATIC_SYMBOL (data, this);
}


UdpSAP :: Dtind :: ~Dtind (void) {

  DELETE_SYMBOL (this);
}


Message * UdpSAP :: Dtind :: clone (void) const {

  return new Dtind (*this);
}


UdpSAP :: Errind :: Errind (void) {

  DYNAMIC_SYMBOL (this, "errind", 0);
  STATIC_SYMBOL (code, this);
  STATIC_SYMBOL (data, this);
}


UdpSAP :: Errind :: Errind (const Errind &msg)
: code (msg.code), data (msg.data) {

  DYNAMIC_SYMBOL (this, "errind", 0);
  STATIC_SYMBOL (code, this);
  STATIC_SYMBOL (data, this);
}


UdpSAP :: Errind :: ~Errind (void) {

  DELETE_SYMBOL (this);
}


Message * UdpSAP :: Errind :: clone (void) const {

  return new Errind (*this);
}


