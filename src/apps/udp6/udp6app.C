
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

$Log: udp6app.C,v $
Revision 1.1  2002/06/14 22:04:05  ik
Development. Initial commit for udp6 application.


*/

//
//Commentary:
//  This is a machine generated file produced by 
// compiler. Editing is not recommended.
//
//Description:
//  This file implements functions of Udp6AppSAP
//  service access point
//


#include <doors/hsi.h>
#include <doors/udp6app.h>


Message *createUdp6AppSAPStart (void) {

  return new Udp6AppSAP :: Start;
};


Message *(*appSAPUserMessages []) (void) = {
  createUdp6AppSAPStart,
  0
};


Message *createUdp6AppSAPInfomsg (void) {

  return new Udp6AppSAP :: Infomsg;
};


Message *(*appSAPProviderMessages []) (void) = {
  createUdp6AppSAPInfomsg,
  0
};


Udp6AppSAP :: User :: User (EventTask *t, Peer *p)
: ReqIface (t, appSAPUserMessages, p) {

}


Udp6AppSAP :: User :: ~User (void) {

}


Udp6AppSAP :: Provider :: Provider (EventTask *t)
: IndIface (t, appSAPProviderMessages) {

}


Udp6AppSAP :: Provider :: ~Provider (void) {

}


Udp6AppSAP :: Start :: Start (void) {

  DYNAMIC_SYMBOL (this, "start", 0);
  STATIC_SYMBOL (destaddr, this);
}


Udp6AppSAP :: Start :: Start (const Start &msg)
: destaddr (msg.destaddr) {

  DYNAMIC_SYMBOL (this, "start", 0);
  STATIC_SYMBOL (destaddr, this);
}


Udp6AppSAP :: Start :: ~Start (void) {

  DELETE_SYMBOL (this);
}


Message * Udp6AppSAP :: Start :: clone (void) const {

  return new Start (*this);
}


Udp6AppSAP :: Infomsg :: Infomsg (void) {

  DYNAMIC_SYMBOL (this, "infomsg", 0);
  STATIC_SYMBOL (info, this);
}


Udp6AppSAP :: Infomsg :: Infomsg (const Infomsg &msg)
: info (msg.info) {

  DYNAMIC_SYMBOL (this, "infomsg", 0);
  STATIC_SYMBOL (info, this);
}


Udp6AppSAP :: Infomsg :: ~Infomsg (void) {

  DELETE_SYMBOL (this);
}


Message * Udp6AppSAP :: Infomsg :: clone (void) const {

  return new Infomsg (*this);
}


