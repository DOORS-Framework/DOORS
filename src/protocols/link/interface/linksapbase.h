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

$Log: linksapbase.h,v $
Revision 1.4  2003/05/07 13:26:32  bilhanan
hsi.h included.

Revision 1.3  2003/05/07 13:18:03  bilhanan
SAP base classes now need to be derived from Message. Constructors added inline for symbol handling.

Revision 1.2  2002/07/23 16:34:15  hartmanj

Development

Revision 1.1  2002/07/08 11:35:23  hartmanj
Initial revision


*/


/* LinkSAPBase is used as a base class for link protocol's
   messages.
*/

#include <doors/obl_dcl.h>
#include <doors/message.h>
#include <doors/hsi.h>

class LinkSAPBase : public Message {
      public:
        LinkSAPBase (void);
        LinkSAPBase (const LinkSAPBase &other);

        Uint16 cid;
};

inline LinkSAPBase :: LinkSAPBase (void) {
  STATIC_SYMBOL (cid, this);
}

inline LinkSAPBase :: LinkSAPBase (const LinkSAPBase &other) 
: cid (other.cid){
  STATIC_SYMBOL (cid, this);
}
