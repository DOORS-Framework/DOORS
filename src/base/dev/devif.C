// 
//  Copyright 1999 
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
 
 
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

$Log: devif.C,v $
Revision 1.2  2002/06/03 09:19:47  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:19  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:12  bilhanan
Full licence clause added into doors 0.1


*/
 

#include <doors/devif.h>
#include <doors/strdev.h>
#include <doors/hsi.h>

Message *(*devIfUserMessages []) (void) = {
 0
};

Message *createDevIfOpen (void) {

  return new DevIf :: Open;
};


Message *createDevIfClose (void) {

  return new DevIf :: Close;
};


Message *createDevIfTimeout (void) {

  return new DevIf :: Timeout;
};


Message *createDevIfRead (void) {

  return new DevIf :: Read;
};


Message *(*devIfProviderMessages []) (void) = {
  createDevIfOpen,
  createDevIfClose,
  createDevIfTimeout,
  createDevIfRead,
  0
};


DevIf :: User :: User (EventTask *t, Peer *p)
: ReqIface (t,devIfUserMessages, p)
{

}


DevIf :: User :: ~User (void) {

}


DevIf :: Provider :: Provider (EventTask *t)
: IndIface (t, devIfProviderMessages) {

}


DevIf :: Provider :: ~Provider (void) {

}


DevIf :: Open :: Open (void) {

  DYNAMIC_SYMBOL (this, "open", 0);
  STATIC_SYMBOL (fd, this);
  device = 0;
}


DevIf :: Open :: Open (const Open &msg)
: fd (msg.fd), device (msg.device) {

  DYNAMIC_SYMBOL (this, "open", 0);
  STATIC_SYMBOL (fd, this);
}


DevIf :: Open :: ~Open (void) {

  DELETE_SYMBOL (this);
}


Message * DevIf :: Open :: clone (void) const {

  return new Open (*this);
}


DevIf :: Close :: Close (void) {

  DYNAMIC_SYMBOL (this, "close", 0);
  STATIC_SYMBOL (fd, this);
  device = 0;
}


DevIf :: Close :: Close (const Close &msg)
: fd (msg.fd), device (msg.device) {

  DYNAMIC_SYMBOL (this, "close", 0);
  STATIC_SYMBOL (fd, this);
}


DevIf :: Close :: ~Close (void) {

  DELETE_SYMBOL (this);
}


Message * DevIf :: Close :: clone (void) const {

  return new Close (*this);
}


DevIf :: Timeout :: Timeout (void) {

  DYNAMIC_SYMBOL (this, "timeout", 0);
  STATIC_SYMBOL (fd, this);
  device = 0;
}


DevIf :: Timeout :: Timeout (const Timeout &msg)
: fd (msg.fd), device (msg.device) {

  DYNAMIC_SYMBOL (this, "timeout", 0);
  STATIC_SYMBOL (fd, this);
}


DevIf :: Timeout :: ~Timeout (void) {

  DELETE_SYMBOL (this);
}


Message * DevIf :: Timeout :: clone (void) const {

  return new Timeout (*this);
}


DevIf :: Read :: Read (void) {

  DYNAMIC_SYMBOL (this, "read", 0);
  STATIC_SYMBOL (fd, this);
  device = 0;
  STATIC_SYMBOL (data, this);
}


DevIf :: Read :: Read (const Read &msg)
: fd (msg.fd), device (msg.device), data (msg.data) {

  DYNAMIC_SYMBOL (this, "read", 0);
  STATIC_SYMBOL (fd, this);
  STATIC_SYMBOL (data, this);
}


DevIf :: Read :: ~Read (void) {

  DELETE_SYMBOL (this);
}


Message * DevIf :: Read :: clone (void) const {

  return new Read (*this);
}



