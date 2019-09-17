
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

$Log: servantbase.C,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:46:40  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.6  2001/06/08 08:39:46  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/servantbase.h>

namespace PortableServer {

  ServantBase :: ServantBase (const char *n, StateMachine *m)
    : PTask (n, Scheduler::Instance(), m),
  is_activated (CORBA::FALSE){
    
  }
  
//    ServantBase :: ServantBase (void){
//    }

//    ServantBase :: ServantBase (const ServantBase& other){
//    }

  ServantBase :: ~ServantBase (void){
  }

  POA_ptr ServantBase :: _default_POA (void){
    return RootPOA :: Instance (Scheduler::Instance());
  }

  CORBA::Boolean ServantBase :: _is_a (CORBA::String logical_type_id){
    return CORBA::FALSE;
  }

  CORBA::Boolean ServantBase :: _non_existent (void){
    return CORBA::FALSE;
  }

  void ServantBase :: delete_my_sm (void) {
    if (this->sm){
      delete this->sm;
    }
  }
}
