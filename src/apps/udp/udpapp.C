
//  Copyright 2000 
//  	Telecoms Lab, Tampere University of Technology.  All rights reserved.

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

$Log: udpapp.C,v $
Revision 1.1.1.1  2002/04/30 12:32:53  bilhanan
Development

Revision 1.2  2001/06/08 16:46:53  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:40:04  bilhanan
Full licence clause added into doors 0.1


*/

//
//Commentary:
//  This is a machine generated file produced by 
// compiler. Editing is not recommended.
//
//Description:
//  This file implements functions of UdpAppSAP
//  service access point
//


#include <doors/hsi.h>
#include <doors/udpapp.h>


Message *createUdpAppSAPStart (void) {

  return new UdpAppSAP :: Start;
};


Message *(*appSAPUserMessages []) (void) = {
  createUdpAppSAPStart,
  0
};


Message *createUdpAppSAPInfomsg (void) {

  return new UdpAppSAP :: Infomsg;
};


Message *(*appSAPProviderMessages []) (void) = {
  createUdpAppSAPInfomsg,
  0
};


UdpAppSAP :: User :: User (EventTask *t, Peer *p)
: ReqIface (t, appSAPUserMessages, p) {

}


UdpAppSAP :: User :: ~User (void) {

}


UdpAppSAP :: Provider :: Provider (EventTask *t)
: IndIface (t, appSAPProviderMessages) {

}


UdpAppSAP :: Provider :: ~Provider (void) {

}


UdpAppSAP :: Start :: Start (void) {

  DYNAMIC_SYMBOL (this, "start", 0);
  STATIC_SYMBOL (destaddr, this);
}


UdpAppSAP :: Start :: Start (const Start &msg)
: destaddr (msg.destaddr) {

  DYNAMIC_SYMBOL (this, "start", 0);
  STATIC_SYMBOL (destaddr, this);
}


UdpAppSAP :: Start :: ~Start (void) {

  DELETE_SYMBOL (this);
}


Message * UdpAppSAP :: Start :: clone (void) const {

  return new Start (*this);
}


UdpAppSAP :: Infomsg :: Infomsg (void) {

  DYNAMIC_SYMBOL (this, "infomsg", 0);
  STATIC_SYMBOL (info, this);
}


UdpAppSAP :: Infomsg :: Infomsg (const Infomsg &msg)
: info (msg.info) {

  DYNAMIC_SYMBOL (this, "infomsg", 0);
  STATIC_SYMBOL (info, this);
}


UdpAppSAP :: Infomsg :: ~Infomsg (void) {

  DELETE_SYMBOL (this);
}


Message * UdpAppSAP :: Infomsg :: clone (void) const {

  return new Infomsg (*this);
}


