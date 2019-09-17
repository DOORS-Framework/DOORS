
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

$Log: object.C,v $
Revision 1.2  2002/08/14 11:51:41  bilhanan
development

Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:46:39  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.13  2001/06/08 08:39:43  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/object.h>
#include <doors/request.h>


namespace CORBA {

//    void release(Object_ptr ptr){
//      if (ptr) delete ptr;
//    }
  
  CORBA::Boolean is_nil(Object_ptr ptr){ 
    return (ptr == 0);
  }

//    Object :: Object (const char *n, Statemachine *m)
//      : PTask (n, Scheduler::Instance(), m){
    
//    }
  
  Object :: Object (IOP :: IOR *ior)
    :my_ior(ior){
      ior = 0;
  }

  Object :: ~Object (void){
    if (my_ior){
      delete my_ior;
    }
  }

  Object_ptr Object :: _duplicate (Object_ptr obj){
    
    if (obj){
    Object_ptr newobj = new Object (obj->my_ior);
    return newobj;
    }
  }

  Object_ptr Object :: _nil (void){
    return 0;
  }

  //  InterfaceDef_ptr Object :: _get_interface (void) {
  //}

  CORBA::Boolean Object :: _is_a (String logical_type_id){
    return CORBA::FALSE;
  }

  CORBA::Boolean Object :: _non_existent (void){

    if (CORBA :: is_nil (this))
        return CORBA::FALSE;

  }

  CORBA::Boolean Object :: _is_equivalent (Object_ptr other_object){

    if (other_object){
      if (other_object == this) 
	return CORBA::TRUE;
      if (other_object->my_ior == this->my_ior) 
	return CORBA::TRUE;
      return CORBA::FALSE;
    }
    return CORBA::FALSE;
  }

  // the hash function is based on a modulus calculation.

  ULong Object :: _hash (ULong maximum){
    
    if (maximum){
    ULong base_ = (ULong) this;
    return (base_%maximum);
    }
    return 0;
  }

  void Object :: _create_request (Context_ptr ctx, String operation, NVList_ptr arg_list, NamedValue_ptr result, Request_ptr request, Flags req_flags) {
    
    char cname [20];
    Uint16 reqid = ORB :: Instance ()->get_reqcnt();
    sprintf (cname, "RequestID.%d", reqid); 

    RequestTask *reqtask = new RequestTask (cname, ctx, operation, arg_list, result);

    reqtask->_target = Object :: _duplicate (this);
    reqtask->_reqid = reqid;

    ORB :: Instance ()->connectRequest (reqtask);
    request = reqtask;

  }
  
  Request_ptr Object :: _request(String operation) {

    char cname [20];
    Uint16 reqid = ORB :: Instance ()->get_reqcnt();
    sprintf (cname, "RequestID.%d", reqid); 

    RequestTask *reqtask  = new RequestTask (cname, operation);
    reqtask->_target = Object :: _duplicate (this);
    reqtask->_reqid = reqid;

    ORB :: Instance ()->connectRequest (reqtask);
    return reqtask;
  }


}
  

