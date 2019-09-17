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

$Log: servantbase.h,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:59  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.9  2001/06/08 08:39:46  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef _doors_servantbase_
#define _doors_servantbase_

#include <doors/datatypes.h>
#include <doors/serverrequest.h>
#include <doors/ptask.h>
#include <doors/rootpoa.h>

namespace PortableServer {
  
  class ServantBase;
  typedef ServantBase* ServantBase_ptr;
  
  class ServantBase : public ::PTask {

    friend class RootPOA;
  public:
    ServantBase (const char *n = "Servant", StateMachine *m = 0);
    virtual ~ServantBase (void); 
    virtual POA_ptr _default_POA (); 
    // virtual InterfaceDef_ptr _get_interface() throw(SystemException); 
    virtual CORBA::Boolean _is_a (CORBA::String logical_type_id); // throw(SystemException); 
    virtual CORBA::Boolean _non_existent (); // throw(SystemException); 
    virtual void _add_ref (void) { }; 
    virtual void _remove_ref (void){ }; 
    void delete_my_sm (void);

  protected: 

    ServantBase (void); 
    ServantBase (const ServantBase& other); 
    ServantBase& operator= (const ServantBase& other); 
    // ...all other constructors... 

    CORBA::Boolean is_activated;
    CORBA::ObjectId myid;
  }; 

  typedef ServantBase* Servant;
			      
}
#endif
