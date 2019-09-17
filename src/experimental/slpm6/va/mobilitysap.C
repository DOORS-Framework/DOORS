//  Copyright 2004
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

$Log: mobilitysap.C,v $
Revision 1.4  2004/06/17 08:56:30  hartmanj
Development.

Revision 1.3  2004/05/21 06:04:09  hartmanj
Call to MobilityModule :: RemoveListener() to release SAP from Mux

Revision 1.2  2004/05/19 13:55:01  hartmanj
License and log tag added


  
*/

#include <doors/hsi.h>
#include "mobilitysap.h"
#include "mobmodule.h"

Message *createMobilitySAPMovement (void)
{
  return new MobilitySAP :: Movement;
}

Message *(*mobilitySAPUserMessages []) (void) =
{
  createMobilitySAPMovement,
  0
};

Message *(*mobilitySAPProviderMessages []) (void) =
{
  0
};

MobilitySAP :: User :: User (EventTask *t, Peer *p)
: ReqIface (t, mobilitySAPUserMessages, p)
{
}

MobilitySAP :: User :: ~User(void)
{
}

bool MobilitySAP :: User :: notify()
{
    _DOORS_DEBUG(0, "MobilitySAP :: User :: notify() : " << other);
    return MobilityModule :: RegisterListener( this );
}

bool MobilitySAP :: User :: forget()
{
    _DOORS_DEBUG(0, "MobilitySAP :: User :: forget() : " << other);
    return MobilityModule :: RemoveListener( this );
}


MobilitySAP :: Movement :: Movement (void)
{
  DYNAMIC_SYMBOL (this, "movement", 0);
  STATIC_SYMBOL (coa, this);
  STATIC_SYMBOL (ha, this);
}

MobilitySAP :: Movement :: Movement(const Movement &msg)
: Message(msg), coa (msg.coa),ha (msg.ha)
{
  DYNAMIC_SYMBOL (this, "movement", 0);
  STATIC_SYMBOL (coa, this);
  STATIC_SYMBOL (ha, this);
}

Message *MobilitySAP :: Movement :: clone (void) const
{
  return new Movement(*this);
}

MobilitySAP :: Movement :: ~Movement (void)
{
  DELETE_SYMBOL (this);
}

MobilitySAP :: Provider :: Provider(EventTask *t)
: IndIface (t, mobilitySAPProviderMessages)
{
}

MobilitySAP :: Provider :: ~Provider(void)
{
}
