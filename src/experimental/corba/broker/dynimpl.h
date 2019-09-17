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

$Log: dynimpl.h,v $
Revision 1.1.1.1  2002/05/13 14:38:35  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:27  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.6  2001/06/08 08:39:42  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef _doors_dynimpl_
#define _doors_dynimpl_

#include <doors/datatypes.h>
#include <doors/serverrequest.h>
#include <doors/servantbase.h>

namespace PortableServer {
  
  class DynamicImplementation;
  typedef DynamicImplementation* DynamicImplementation_ptr;
  
  class DynamicImplementation : public virtual ServantBase {

    friend class CORBA::ServerRequest;
    CORBA::NVList_ptr _parameters;

  public:
    DynamicImplementation (CORBA::NVList_ptr _params, const char *n = "DynamicImplementation", StateMachine *m = 0);
    virtual ~DynamicImplementation () {};
    CORBA::Object_ptr _this ();
    virtual void invoke (CORBA::ServerRequest_ptr request) = 0;
    //    virtual RepositoryId _primary_interface (const CORBA::ObjectId& oid, POA_ptr poa) = 0;
  };
			      
  inline DynamicImplementation :: DynamicImplementation (CORBA::NVList_ptr _params, const char *n = "DynamicImplementation", StateMachine *m = 0)
    : PTask (n, Scheduler::Instance(), m),
      _parameters (_params){ 
  }

  inline CORBA::Object_ptr DynamicImplementation :: _this (void){

    if (is_activated) {
      return (RootPOA :: Instance()->id_to_reference(myid));
    } else {
      myid = RootPOA :: Instance()->activate(this);
      is_activated = TRUE;
      return (RootPOA :: Instance()->id_to_reference(myid));
  }
}


#endif
