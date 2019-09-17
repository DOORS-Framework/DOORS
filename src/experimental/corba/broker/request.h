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

$Log: request.h,v $
Revision 1.2  2002/07/24 15:10:34  bilhanan
development.

Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:54  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.13  2001/06/08 08:39:44  bilhanan
Full licence clause added into doors 0.1


*/



#ifndef _doors_request_
#define _doors_request_

#include <doors/datatypes.h>
#include <doors/corba_dcl.h>
#include <doors/any.h>

namespace CORBA {

 
  class Request {

    Request (Object_ptr ptr);
    Request (String operation, Object_ptr ptr, NVList_ptr arglist = 0);
    Request (const Request& req);
    String _operation;
    NVList_ptr _arguments;
    NamedValue_ptr _result;
    Object_ptr _target;

    friend class Object;
    friend class ORB;

  public: 

    virtual ~Request (void) { } // let garbage collection take care of destruction.

    Object_ptr target() const; 
    const String *operation() const; 
    NVList_ptr arguments(); 
    NamedValue_ptr result(); 
//     Environment_ptr env(); 
//     ExceptionList_ptr exceptions(); 
//     ContextList_ptr contexts(); 
//     void ctx(Context_ptr); 
//     Context_ptr ctx() const; 
    Any& add_in_arg(); 
    Any& add_in_arg(const String name); 
    Any& add_inout_arg();
    Any& add_inout_arg(const String name); 
    Any& add_out_arg(); 
    Any& add_out_arg(const String name); 
    void set_return_type(TypeCode_ptr tc); 
    Any& return_value(); 
    void invoke(); 
//     void send_oneway(); 
//     void send_deferred(); 
//     void get_response(); 
//     CORBA::Boolean poll_response(); 

    static Request_ptr _duplicate(Request_ptr req);
    static Request_ptr _nil(void);
  };


  class Request_out { 

  public: 
    
    Request_out(Request_ptr& p) 
      : ptr_(p) { 
      ptr_ = Request::_nil(); 
    } 

    Request_out(Request_out& a) 
      : ptr_(a.ptr_) {} 

    Request_out& operator=(Request_out& a) { 
      ptr_ = a.ptr_; return *this; 
    } 

    Request_out& operator=(Request_ptr p) { 
      ptr_ = p; 
      return *this; 
    } 

    operator Request_ptr&() { 
      return ptr_; 
    } 

    Request_ptr& ptr() { 
      return ptr_; 
    } 

    Request_ptr operator->() { 
      return ptr_; 
    } 

  private: 
    Request_ptr& ptr_; 

  };

  CORBA::Boolean is_nil(Request_ptr ptr);
  void release(Request_ptr ptr);
}

#endif
