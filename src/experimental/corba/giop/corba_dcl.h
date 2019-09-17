// Editor-info: -*- C++ -*-
//
//  Copyright 2002 
//      Telecom Lab, Tampere University of Technology.  
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

//  $Log: corba_dcl.h,v $
//  Revision 1.2  2002/08/13 10:10:41  bilhanan
//  Development
//
//  Revision 1.1  2002/07/25 09:50:31  bilhanan
//
//  moved from experimental/corba/broker/ to experimental/corba/giop/
//
//  Revision 1.1  2002/07/24 15:10:34  bilhanan
//  development.
//

#ifndef CORBA_DCL_H
#define CORBA_DCL_H

#include <string>
#include <vector>

namespace CORBA {

  class ORBTask;
  class ORBTaskSM;
  class RequestTask;
  class RequestTaskSM;
 
  typedef ORBTask ORB;
  typedef ORB* ORB_ptr;

  typedef RequestTask Request;
  typedef Request* Request_ptr;

  class Any;
  typedef Any* Any_ptr;

  class TypeCode;
  typedef TypeCode* TypeCode_ptr;
  
  class Object;
  typedef Object* Object_ptr;

  class RootPOA;
  typedef RootPOA* POA_ptr;


  class NVList;
  typedef NVList* NVList_ptr;

  class NamedValue;
  typedef NamedValue* NamedValue_ptr;

}

#endif
