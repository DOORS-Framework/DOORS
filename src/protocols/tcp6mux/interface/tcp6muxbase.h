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

$Log: tcp6muxbase.h,v $
Revision 1.2  2005/02/21 16:31:09  bilhanan
clean port of tcpmux to ipv6

Revision 1.1  2004/10/12 12:01:23  bilhanan
The protocol formerly known as tranmux

Revision 1.5  2003/05/07 13:26:32  bilhanan
hsi.h included.

Revision 1.4  2003/05/07 13:18:04  bilhanan
SAP base classes now need to be derived from Message. Constructors added inline for symbol handling.

Revision 1.3  2002/07/30 12:48:27  bilhanan
comment fix.

Revision 1.2  2002/07/30 12:36:56  bilhanan
Fix to handle upper layer identifier information.

Revision 1.1  2002/07/23 09:42:48  bilhanan
xcg modifications


*/


/* Tcp6muxbase is used as a base class for the protocol's
   SAP messages.
*/

#include <doors/obl_dcl.h>
#include <doors/message.h>
#include <doors/hsi.h>

class Tcp6muxbase : public Message {
      public:
        Tcp6muxbase (void);
        Tcp6muxbase (const Tcp6muxbase &other);

        Uint16 cep;
};


inline Tcp6muxbase :: Tcp6muxbase (void) {
  STATIC_SYMBOL (cep, this);
}

inline Tcp6muxbase :: Tcp6muxbase (const Tcp6muxbase &other) 
: cep (other.cep){
  STATIC_SYMBOL (cep, this);
}
