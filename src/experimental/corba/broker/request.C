
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

$Log: request.C,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:46:40  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.13  2001/06/08 08:39:44  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/typecode.h>
#include <doors/namedvalue.h>
#include <doors/object.h>
#include <doors/request.h>
#include <doors/orb.h>

namespace CORBA {

  CORBA::Boolean is_nil(Request_ptr ptr){
    return ptr == 0;
  }

//    void release(Request_ptr ptr){
//      if (ptr) delete ptr;
//    }

  Request :: Request (Object_ptr ptr) 
    : _arguments (0), _result (0), _target (Object :: _duplicate (ptr)){
  }

  Request :: Request (String operation, Object_ptr ptr, NVList_ptr arglist) 
  :_operation (operation), _arguments (NVList :: _duplicate (arglist)), _result (0), _target (Object :: _duplicate (ptr)){
  
  }
  
  Request :: Request (const Request& req)
  :_operation (req._operation){
    _arguments = NVList :: _duplicate (req._arguments);
    _result = NamedValue :: _duplicate (req._result);
    _target = Object :: _duplicate (req._target);
  }

  Object_ptr Request :: target() const {
    return _target;
  }
  
  const String* Request :: operation() const {
    return &_operation;
  }

  NVList_ptr Request :: arguments (void){
    return _arguments;
  }
  
  NamedValue_ptr Request :: result(void){   

    return _result;
  }

  Any& Request :: add_in_arg(void){
    NamedValue_ptr ptr = _arguments-> add(ARG_IN);
    return *(ptr->value());
  }
 
  Any& Request :: add_in_arg(const String name){
    NamedValue_ptr ptr = _arguments-> add_item(name, ARG_IN);
    return *(ptr->value());
  }

  Any& Request :: add_inout_arg(void){
    NamedValue_ptr ptr = _arguments-> add(ARG_INOUT);
    return *(ptr->value());
  }

  Any& Request :: add_inout_arg(const String name){
    NamedValue_ptr ptr = _arguments-> add_item(name, ARG_INOUT);
    return *(ptr->value());
  }

  Any& Request :: add_out_arg(void){ 
    NamedValue_ptr ptr = _arguments-> add(ARG_OUT);
    return *(ptr->value());
  }

  Any& Request :: add_out_arg(String name){ 
    NamedValue_ptr ptr = _arguments-> add_item(name, ARG_OUT);
    return *(ptr->value());
  }

  void Request :: set_return_type(TypeCode_ptr tc){
    _result->value()->replace(TypeCode::_duplicate(tc), 0);
  }

  Any& Request :: return_value(void) {
    return *(_result->value());
  }


  Status Request :: invoke (void){

    CORBA :: ORB_ptr myorb = CORBA :: ORB :: Instance ();
    myorb->marshal (this);
  }

  Request_ptr Request :: _duplicate(Request_ptr req){
    if (!is_nil(req)){
      return new_gc Request(*req);
    }
    
  }

  Request_ptr Request :: _nil(void){ 
    return 0;
  }



}


