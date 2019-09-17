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

$Log: repository.C,v $
Revision 1.1.1.1  2002/05/13 14:38:37  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:26  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.5  2001/06/08 08:40:11  bilhanan
Full licence clause added into doors 0.1


*/
// repository.C
// simple repository management
// by Yingfan Liu
// 04/07/98

#include <doors/repository.h>

Repository serviceRepository;


Uint16 
Repository::Add(const Object& o)
{
  Object oo = o;
  oo.id = id_++;
#if (__SUNPRO_CC >= 0x500)
  std::pair<Base::iterator, bool> result = base_.insert (Base::value_type(oo.objectKey,oo));
#else
  pair<Base::iterator, bool> result = base_.insert (Base::value_type(oo.objectKey,oo));
#endif
  if (result.second)
    return oo.id;
  else
    return 0;
};


bool 
Repository::Del(Uint16 id)
{
  for (Base::iterator i = base_.begin(); i != base_.end(); i++)
    if ((*i).second.id == id) {
      base_.erase(i);
      return true;
    }
  return false;   
};

Uint16 
Repository::GetId(const Key& objectKey)
{
  Base::iterator i = base_.find (objectKey);
  if (i != base_.end())
    return (*i).second.id;
  else
    return 0;
};

bool 
Repository::HasOp(const Key& id, const CORBA::iiopString & opcode)
{
  Base::iterator i = base_.find (id);
  if (i != base_.end()) {
    for (int j = 0; j < (*i).second.operations.Length(); j++) {
      if (opcode == (*i).second.operations[j])
	return true;
    }
  }
  return false;
};



