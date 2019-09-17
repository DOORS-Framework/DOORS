//Editor-Info: -*- C -*-

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

$Log: orb.h,v $
Revision 1.2  2002/07/24 15:10:34  bilhanan
development.

Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:48  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.23  2001/06/08 08:39:44  bilhanan
Full licence clause added into doors 0.1


*/



#ifndef ORB_H
#define ORB_H

#include <doors/datatypes.h>
#include <doors/ptb.h>
#include <doors/corba_dcl.h>

namespace CORBA {


  class ORB :: public PTask {

   public:
 
   ORBTask (std::string n, Scheduler *s, ORBTaskSM *sm);

   static ORBTask* Instance (void); 
   virtual ~ORBTask (void);
    
    GIOPPeer peer;
    Tran::User down;

    Request_ptr createRequest (void);




   // From this point down, all functions are from CORBA specs.

    //    POA_ptr rootpoa;
    String object_to_string (CORBA::Object_ptr obj);
    Object_ptr string_to_object (const String& str);
    
    // Some TypeCode creation operations.
    
    TypeCode_ptr create_string_tc (ULong bound);    
    TypeCode_ptr create_wstring_tc (ULong bound);
    TypeCode_ptr create_fixed_tc (UShort digits, UShort scale);
    TypeCode_ptr create_sequence_tc (ULong bound, TypeCode_ptr element_type);
    TypeCode_ptr create_array_tc (ULong length, TypeCode_ptr element_type);

    //To create all other kinds of typecodes defined in TypeCode.h, use the
    //function below. No typechecking is done.
    // eg CORBA :: ORB :: create_typecode (CORBA :: tk_boolean);

    TypeCode_ptr create_typecode (CORBA::TCKind kind);


    // Status create_list
    // Status create_operation
    // Status get_default_context (out Context ctx);
    
    // Current get_current();
    
    //Obtaining Initial Object References
    
    // typedef sequence<ObjectId> ObjectIdList;
    
    //    exception InvalidName {};
    
    // ObjectIdList list_initial_services (void);
    
//      Object_ptr resolve_initial_references (ObjectId identifier);  // raises (InvalidName);
    
    // boolean work_pending ();
    // void perform_work ();
    // void shutdown (in boolean wait_for_completion);
    
    void run(void);


  private: 

    Port requests;
    Port ToPOA;
    PortMux reqmux;

    static ORBTask* _instance;
    InetAddr ownaddr;

    friend class Request;

  };


  
  //ORB Initialization
  typedef String ORBid;
  
  static ORB_ptr ORB_init (ORBid orb_identifier) {
    
    if (!(orb_identifier.compare("doors_orb"))){
      return (ORB :: Instance());
    }
  }

}

#endif
