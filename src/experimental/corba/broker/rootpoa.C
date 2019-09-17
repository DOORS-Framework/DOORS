
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

$Log: rootpoa.C,v $
Revision 1.2  2002/07/23 14:32:47  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:46:40  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.14  2001/06/08 08:39:45  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/orb.h>
#include <doors/rootpoa.h>

namespace PortableServer {

  RootPOA :: RootPOA (const char *n) :
    Object (n, RootPOAManager::Instance(this)),
    down (this), objcount(0)
  {
    state = RootPOAManager::HOLDING; //page 11-15, CORBA2.3, section 11.3.2.1
  }
  
  RootPOA :: ~RootPOA (void) {
    
    _instance = 0;
    
    if (sm)
      delete_my_sm(void);
    
  }
  
  bool RootPOA :: holding_default (Message *msg){
    
  }

  bool RootPOA :: active_default (Message *msg){
    
  }
  
  bool RootPOA :: discarding_default (Message *msg){
    
  }
  
  bool RootPOA :: inactive_default (Message *msg){
    
  }
  
  
//    POA_ptr RootPOA :: narrow (Object_ptr objptr) {
//      if (objptr == this) {
//        return (POA_ptr) this;
//      }
//      return 0;
//    }
  
  CORBA::String RootPOA :: the_name (void){
    return ("RootPOA");
  };

  RootPOAManager* RootPOA :: the_POAManager (void){
    return (RootPOAManager::Instance(this));
  };

  RootPOA* RootPOA :: Instance (Scheduler *s){
    if (_instance == 0){
      _instance = new RootPOA (s);
    }
    
    return _instance;
  }
  
  // since RootPOA inherits from PTask and not from Object,
  // _duplicate() is implemented as just a singleton wrapper.

  POA_ptr RootPOA :: _duplicate (void){
    return RootPOA::Instance(Scheduler::Instance());
  }
  
   CORBA::ObjectId RootPOA :: activate_object (Servant p_servant) {
    
    if (p_servant->is_activated == CORBA::TRUE)
      return p_servant->myid;
    
    OSTRINGSTREAM gen_id;
    gen_id << "Servant." << p_servant->getName() << "." << ++servantcount;
    CORBA::ObjectId objid = gen_id.str();
    
    ActiveObjectMap.insert (map<CORBA::ObjectId, Servant>::value_type (CORBA::ObjectId (objid), p_servant));
    
    p_servant->myid = objid;
    p_servant->is_activated = CORBA::TRUE;
    return objid;
    
  }
  
  void RootPOA :: activate_object_with_id (CORBA::ObjectId Id, Servant p_servant){
    
    if (p_servant->is_activated == CORBA::TRUE)
      return p_servant->myid;
    else {
      ActiveObjectMap.insert (map<CORBA::ObjectId, Servant>::value_type (CORBA::ObjectId (Id), p_servant));
      p_servant->is_activated = CORBA::TRUE;
      return;
    }			 
    
  }
  
  void RootPOA :: deactivate_object (CORBA::ObjectId oid){
  
    int count = 0;
    
    if (ActiveObjectMap.count (oid)){    // search active object map for oid
      Servant p_servant = ActiveObjectMap [oid];     // if found
      if (ActiveObjectMap.erase (oid))
	p_servant->is_activated = CORBA::FALSE;
    }
  }
  
  CORBA::ObjectId RootPOA :: servant_to_id (Servant p_servant){

    if (p_servant->is_activated == CORBA::TRUE)
      return p_servant->myid;
    
    OSTRINGSTREAM gen_id;
    gen_id << "Servant." << p_servant->getName() << "." << ++servantcount;
    CORBA::ObjectId objid = gen_id.str();
    
    return objid;

  }

  Servant RootPOA :: id_to_servant (CORBA::ObjectId oid){

    // hmmm, real problem: how to locate inactive servant?

    int count = 0;
    
    if (ActiveObjectMap.count (oid)){    // search active object map for oid
      return (ActiveObjectMap [oid]);     // if found
  }

  void RootPOA :: setState (int newstate){
    
    switch (newstate){
    case 0:
      state = RootPOAManager :: HOLDING;
      break;
    case 1:
      state = RootPOAManager :: ACTIVE;
      break;
    case 2:
      state = RootPOAManager :: DISCARDING;
      break;
    case 3:
      state = RootPOAManager :: INACTIVE;
      break;
    default:
      state = RootPOAManager :: other;
      break;
    }
    
  }
  
  RootPOA *RootPOA :: _instance = 0;
}
