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

$Log: repository.h,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:53  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.8  2001/06/08 08:40:11  bilhanan
Full licence clause added into doors 0.1


*/

// File repository.h
// by Yingfan Liu
// 04/07/98

#ifndef _Repository_h
#define _Repository_h 

#include <doors/hsi.h>
#include <doors/ptb.h>

//#include "/share/faster/Toolkit/ospace/stl/map.h"
#if (__SUNPRO_CC >= 0x500)
#include <map>
#else
#include <map.h>
#endif


//#include "../yingfans_iiop_v1.0/GIOP.h"
#include <doors/bastypes.h>
/*****************************************************
  The repository store an object and assign a id to it.
  The id starts from 1.
  ****************************************************/

typedef CORBA::Sequence <CORBA::Octet> Key;

class Repository {
public:
  class Object {
  public:
    Uint16                          id;
    Key                             objectKey;
    CORBA::Sequence <CORBA::iiopString>         operations;
  };

 protected:
  // class map;
#if (__SUNPRO_CC == 0x500)  
  typedef std::map<Key, Object, std::less<Key> > Base;
#else 
  typedef map<Key, Object, less<Key> > Base;
#endif
  Base        base_;
  Uint16      id_;

public:

  Repository() : id_(1) {};
  Uint16 Add(const Object& o);
  bool Del(Uint16 id);  
  Uint16 GetId(const Key& objectKey);
  bool HasOp(const Key& objectKey, const CORBA::iiopString& opcode);
};

extern Repository serviceRepository;

#endif


