//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
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

$Log: principal.h,v $
Revision 1.1.1.1  2002/05/13 14:38:37  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:25  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.5  2001/06/08 08:40:11  bilhanan
Full licence clause added into doors 0.1


*/
// Principal.h
// Yingfan Liu
// Mar, 1998

#ifndef __principal_h__
#define __principal_h__

#include <doors/refcount.h>

//
// The Principal class
//
class CORBA_Principal;
typedef CORBA_Principal* CORBA_Principal_ptr;

class CORBA_Principal : public TRCObject {
public:
  CORBA_Principal(const CORBA_Principal&) {};
  void operator=(const CORBA_Principal&) {};

  CORBA::Sequence<CORBA::Octet> seq;

  CORBA_Principal() { };

  friend inline void CORBA_release(CORBA_Principal_ptr p)
    { if(p) p -> DelRef(); }
  friend inline CORBA::Boolean CORBA_is_nil(CORBA_Principal_ptr p)
    { return p == 0; }
  static inline CORBA_Principal_ptr _duplicate(CORBA_Principal_ptr p)
    { if(p) p -> AddRef(); return p; }
  static inline CORBA_Principal_ptr _nil()
    { return 0; }
};

// ----------------------------------------------------------------------
// CDR functions for Principal
// ----------------------------------------------------------------------

typedef TRCPtr<CORBA_Principal> Principal_Ptr;
typedef CORBA::Sequence<Principal_Ptr > Principal_Seq;



#endif
