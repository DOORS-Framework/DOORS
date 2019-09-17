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

$Log: poa.h,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:51  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:44  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef _doors_poa
#define _doors_poa

#include <doors/datatypes.h>

namespace PortableServer {
  
  class POA;
  typedef POA* POA_ptr;
  
  enum ThreadPolicyValue {
    ORB_CTRL_MODEL,
    SINGLE_THREAD_MODEL
  };

//    class ThreadPolicy;
//    typedef ThreadPolicy* ThreadPolicy_ptr;

//    class ThreadPolicy : 
//      virtual public ::CORBA::Policy
//    {
//    public:
//      virtual ~ThreadPolicy();
 
//      static ThreadPolicy_ptr _duplicate(ThreadPolicy_ptr _obj)
//      {
//        CORBA::Object::_duplicate(_obj);
//        return _obj;
//      }
    
//      static ThreadPolicy_ptr _nil()
//      {
//        return 0;
//      }
    
//      virtual ThreadPolicyValue value() = 0;
//    protected:
//      ThreadPolicy() {};
//    private:
//      ThreadPolicy( const ThreadPolicy& );
//      void operator=( const ThreadPolicy& );
//    };
  
  
  enum LifespanPolicyValue {
    TRANSIENT = 0,
    PERSISTENT
  }; 
  
  class LifespanPolicy;
  typedef LifespanPolicy* LifespanPolicy_ptr;

  class LifespanPolicy : 
    virtual public ::CORBA::Policy
  {
  public:
    virtual ~LifespanPolicy();
    
    static LifespanPolicy_ptr _duplicate( LifespanPolicy_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }
    
    static LifespanPolicy_ptr _nil()
    {
      return 0;
    }
    
    virtual LifespanPolicyValue value() = 0;
 
  protected:
    LifespanPolicy() {};
  private:
    LifespanPolicy( const LifespanPolicy& );
    void operator=( const LifespanPolicy& );
  };
  
  
  enum IdUniquenessPolicyValue {
    UNIQUE_ID = 0,
    MULTIPLE_ID
  };
  
  enum IdAssignmentPolicyValue {
    USER_ID = 0,
    SYSTEM_ID
  };

  enum ImplicitActivationPolicyValue {
  IMPLICIT_ACTIVATION = 0,
  NO_IMPLICIT_ACTIVATION
};

  enum ServantRetentionPolicyValue {
    RETAIN = 0,
    NON_RETAIN
  };
  
  enum RequestProcessingPolicyValue {
    USE_ACTIVE_OBJECT_MAP_ONLY = 0,
    USE_DEFAULT_SERVANT,
    USE_SERVANT_MANAGER
  };
  
  
  class POA : virtual public CORBA::Object {
    
  public:
    virtual POA_ptr create_POA( const char* adapter_name, POAManager_ptr a_POAManager, const CORBA::PolicyList& policies ) = 0;
    virtual POA_ptr find_POA( const char* adapter_name, CORBA::Boolean activate_it ) = 0;
    virtual void destroy( CORBA::Boolean etherealize_objects, CORBA::Boolean wait_for_completion ) = 0;
    virtual ThreadPolicy_ptr create_thread_policy( ThreadPolicyValue value ) = 0;
    virtual LifespanPolicy_ptr create_lifespan_policy( LifespanPolicyValue value ) = 0;
    virtual IdUniquenessPolicy_ptr create_id_uniqueness_policy( IdUniquenessPolicyValue value ) = 0;
    virtual IdAssignmentPolicy_ptr create_id_assignment_policy( IdAssignmentPolicyValue value ) = 0;
    virtual ImplicitActivationPolicy_ptr create_implicit_activation_policy( ImplicitActivationPolicyValue value ) = 0;
    virtual ServantRetentionPolicy_ptr create_servant_retention_policy( ServantRetentionPolicyValue value ) = 0;
    virtual RequestProcessingPolicy_ptr create_request_processing_policy( RequestProcessingPolicyValue value ) = 0;
    virtual ServantManager_ptr get_servant_manager() = 0;
    virtual void set_servant_manager( ServantManager_ptr imgr ) = 0;
    virtual Servant get_servant() = 0;
    virtual void set_servant( Servant p_servant ) = 0;
    virtual ObjectId* activate_object( Servant p_servant ) = 0;
    virtual void activate_object_with_id( const ObjectId& id, Servant p_servant ) = 0;
    virtual void deactivate_object( const ObjectId& oid ) = 0;
    virtual CORBA::Object_ptr create_reference( const char* intf ) = 0;
    virtual CORBA::Object_ptr create_reference_with_id( const ObjectId& oid, const char* intf ) = 0;
    virtual ObjectId* servant_to_id( Servant p_servant ) = 0;
    virtual CORBA::Object_ptr servant_to_reference( Servant p_servant ) = 0;
    virtual Servant reference_to_servant( CORBA::Object_ptr reference ) = 0;
    virtual ObjectId* reference_to_id( CORBA::Object_ptr reference ) = 0;
    virtual Servant id_to_servant( const ObjectId& oid ) = 0;
    virtual CORBA::Object_ptr id_to_reference( const ObjectId& oid ) = 0;

    typedef String ObjectId;
  };

  String ObjectId_to_string(const ObjectId&);
  WString ObjectId_to_wstring (const ObjectId&);

  ObjectId* string_to_ObjectId(const String);
  ObjectId* wstring_to_ObjectId(const WString);
  
}
#endif



