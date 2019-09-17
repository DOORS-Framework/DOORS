// -*- C++ -*-

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

$Log: object.h,v $
Revision 1.2  2002/08/14 11:51:41  bilhanan
development

Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:46  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.13  2001/06/08 08:39:43  bilhanan
Full licence clause added into doors 0.1


*/



#ifndef _doors_object_
#define _doors_object_

#include <doors/datatypes.h>
#include <doors/corba_dcl.h>
#include <doors/any.h>
#include <doors/iop.h>
//#include <doors/namedvalue.h>

namespace CORBA {
  
  class Object {

   public: 

    virtual ~Object (void);
    static Object_ptr _duplicate(Object_ptr obj); 
    static Object_ptr _nil(); 
    // InterfaceDef_ptr _get_interface(); 
    CORBA::Boolean _is_a(String logical_type_id); 
    CORBA::Boolean _non_existent(); 
    CORBA::Boolean _is_equivalent(Object_ptr other_object); 
    ULong _hash(ULong maximum); 
    Status _create_request(Context_ptr ctx, String operation, 
			   NVList_ptr arg_list, NamedValue_ptr result, 
			   Request_ptr request, Flags req_flags = 0 ); 
    Status _create_request(Context_ptr ctx, String operation, 
  			   NVList_ptr arg_list, NamedValue_ptr result, 
  			   ExceptionList_ptr, ContextList_ptr, 
  			   Request_out request, Flags req_flags ); 
    Request_ptr _request(String operation); 


  protected:

    IOP :: IOR *my_ior;
    RequestTask *my_req;

  private:

    friend class ORBTask;
    friend class RequestTask;
    Object (IOP :: IOR *ior = 0);

  };

  void release(Object_ptr ptr);
  CORBA::Boolean is_nil(Object_ptr ptr);
  
}
#endif
