
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

$Log: orb.C,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:46:39  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.20  2001/06/08 08:39:43  bilhanan
Full licence clause added into doors 0.1


*/

#include <doors/osched.h>
#include <doors/orb.h>
#include <doors/object.h>
#include <doors/iiopsap.h>
//#include <doors/rootpoa.h>
#include <doors/typecode.h>
#include <doors/request.h>

namespace CORBA {
  
//    ORB :: ORB (void)
//      :rootpoa (RootPOA::Instance(Scheduler::Instance())){
//    }
  
  ORB :: ORB (void) :
    ownaddr (14285, getenv ("HOST")), iioptask ("iiop", &ownaddr), down (0)
  {
    iioptask.up.connect (&down);
  }

  ORB :: ~ORB (void) {
    _instance = 0;
  }
  
  void ORB :: marshal (Request_ptr requestobj){

  }

  String ORB :: object_to_string (Object_ptr obj){
      Frame iorfrm;
      Uint32 len = 0;
 
      IOP :: EncodeIor(*obj->my_ior, iorfrm, len);
      
      Frame striorfrm;
      IOP :: IorF2SF(striorfrm, iorfrm);  
  
      String stringifiedior ((char* )(Byte* ) striorfrm);
      return stringifiedior;
  }

  Object_ptr ORB :: string_to_object (const String& str){

    IOP::IOR iorified;
    Frame iorfrm;
    iorfrm.putFirst ((const Byte*) str.c_str(), (FrameSize) str.length());
    IOP :: IorSF2F(iorfrm, iorified);  
    
    return new Object (&iorified);
  }
    
  TypeCode_ptr ORB :: create_string_tc (ULong bound){
    
    TypeCode_ptr tc_ptr = new_gc TypeCode(CORBA :: tk_string);
    tc_ptr->length_ = bound;
    return tc_ptr;
  }
  
  TypeCode_ptr ORB :: create_wstring_tc (ULong bound){
    
    TypeCode_ptr tc_ptr = new_gc TypeCode(CORBA :: tk_wstring);
    tc_ptr->length_ = bound;
    return tc_ptr;
  }

  TypeCode_ptr ORB :: create_fixed_tc (UShort digits, UShort scale){

    TypeCode_ptr tc_ptr = new_gc TypeCode(CORBA :: tk_fixed);
    tc_ptr->digits_ = digits;
    tc_ptr->scale_ = scale;
    return tc_ptr;
  }
    

  TypeCode_ptr ORB :: create_sequence_tc (ULong bound, TypeCode_ptr element_type){

    TypeCode_ptr tc_ptr = new_gc TypeCode(CORBA :: tk_sequence);
    tc_ptr->length_ = bound;
    tc_ptr->contentType_ = element_type;
    return tc_ptr;
  }

  TypeCode_ptr ORB :: create_array_tc (ULong length, TypeCode_ptr element_type){

    TypeCode_ptr tc_ptr = new_gc TypeCode(CORBA :: tk_array);
    tc_ptr->length_ = length;
    tc_ptr->contentType_ = element_type;
    return tc_ptr;
  }
    
  TypeCode_ptr ORB :: create_typecode (CORBA::TCKind kind){

      return (new_gc TypeCode(kind));
  }
  
//    ObjectIdList ORB :: list_initial_services (void) {
//    }

//    Object_ptr ORB :: resolve_initial_references (ObjectId identifier){

//      if (!(identifier.compare("RootPOA"))){

//        return ((CORBA :: Object_ptr) PortableServer :: RootPOA :: Instance (Scheduler::Instance()));
//      }

//    }

  ORB* ORB :: Instance (void) {

    if (_instance == 0){
      _instance = new ORB ();
    }

    return _instance;
  }

  void ORB :: run(void){

    Scheduler *s = Scheduler :: Instance ();
    s->run();
  }

//    static ORB_ptr ORB_init (ORBid orb_identifier) {
    
//      if (!(orb_identifier.compare("doors_orb"))){
//        return (ORB :: Instance());
//      }
//    }
  
  
  ORB *ORB :: _instance = 0;

}
  

