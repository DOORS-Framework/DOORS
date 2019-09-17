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

$Log: tcp6muxsapbase.h,v $
Revision 1.1  2004/08/18 06:22:17  bilhanan
TCP IPv6 multiplexer


*/


/* Tcp6muxSAPBase is used as a base class for the protocol's
   SAP messages.
*/

#include <doors/obl_dcl.h>
#include <doors/message.h>
#include <doors/hsi.h>

class Tcp6muxSAPBase : public Message {
      public:
        Tcp6muxSAPBase (void);
        Tcp6muxSAPBase (const Tcp6muxSAPBase &other);

        Uint16 cep;
};


inline Tcp6muxSAPBase :: Tcp6muxSAPBase (void) {
  STATIC_SYMBOL (cep, this);
}

inline Tcp6muxSAPBase :: Tcp6muxSAPBase (const Tcp6muxSAPBase &other) 
: cep (other.cep){
  STATIC_SYMBOL (cep, this);
}
