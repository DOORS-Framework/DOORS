//Editor-Info: -*- C++ -*-


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

$Log: rootpoa.h,v $
Revision 1.2  2002/07/23 14:32:47  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:54  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.15  2001/06/08 08:39:45  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef _doors_rootpoa_
#define _doors_rootpoa_

#include <map.h>
#include <doors/datatypes.h>
#include <doors/ptb.h>
#include <doors/iiopsap.h>
//#include <doors/orb.h>
#include <doors/object.h>

class ORB;
namespace PortableServer{

  class RootPOAManager;
  class RootPOA;
  typedef RootPOA POA;
  class ServantBase;
  typedef RootPOA* POA_ptr; //let's typedef it to rootpoa for now.
  typedef ServantBase* Servant;

  class RootPOA : virtual public PTask{
    
    // RootPOA supports default policies as follows.
    // Thread Policy: ORB_CTRL_MODEL
    // Lifespan Policy: TRANSIENT
    // Object Id Uniqueness Policy: UNIQUE_ID
    // Id Assignment Policy: SYSTEM_ID
    // Server Retention Policy: RETAIN
    // Request Processing Policy: USE_ACTIVE_OBJECT_MAP_ONLY
    // Implicit Activation Policy: IMPLICIT_ACTIVATION
    
    friend class CORBA::ORB;
    friend class RootPOAManager;
  public:
    static RootPOA* Instance (Scheduler *s = NULL);
    static POA_ptr _duplicate(void);
    virtual ~RootPOA (void);
    
    bool holding_default (Message *msg);
    bool active_default (Message *msg);
    bool discarding_default (Message *msg);
    bool inactive_default (Message *msg);
    
    CORBA::String the_name (void);

    RootPOAManager* the_POAManager (void);

  CORBA::ObjectId activate_object (Servant p_servant);
  void activate_object_with_id (CORBA::ObjectId Id, Servant p_servant);
  void deactivate_object (CORBA::ObjectId oid);
  CORBA::Object_ptr create_reference (CORBA::RepositoryId intf);
  CORBA::Object_ptr create_reference_with_id (CORBA::ObjectId oid, CORBA::RepositoryId intf);
  CORBA::ObjectId servant_to_id (Servant p_servant);
  CORBA::Object_ptr servant_to_reference (Servant p_servant);
  Servant reference_to_servant (CORBA::Object_ptr reference);
  CORBA::ObjectId reference_to_id (CORBA::Object_ptr reference);
  Servant id_to_servant (CORBA::ObjectId oid);
  CORBA::Object_ptr id_to_reference (CORBA::ObjectId oid); 
 
  // POA shouldn't be a corba object, really. for now at least.
  // POA_ptr narrow (CORBA::Object_ptr objptr);
  IiopSAP::User down;  //typedef OrbSAP somewhere to RootPOA::User
  //  ServantSAP up; //typedef ServantSAP somewhere to RootPOA::Provider

private:
  RootPOA (const char *n = "RootPOA");
  void setState (int newstate){
  };
  //  Timer discard_timer;
  map <CORBA::ObjectId, Servant> ActiveObjectMap;
  static RootPOA* _instance;

  CORBA::ULong servantcount;
};

};
#endif 












