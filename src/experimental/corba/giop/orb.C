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

//  $Log: orb.C,v $
//  Revision 1.3  2002/08/13 10:10:41  bilhanan
//  Development
//
//  Revision 1.2  2002/08/02 13:39:11  bilhanan
//  development
//
//  Revision 1.1  2002/07/25 19:28:21  bilhanan
//  Initial revision
//

#include <doors/hsi.h>
#include <doors/orb.h>
#include <doors/object.h>
#include <doors/transap.h>
#include <doors/typecode.h>
#include <doors/request.h>

namespace CORBA {

  static bool isForRequestTask (const EventTask *t, const Message *m) {

    bool rc = false;
    RequestTask *task = (RequestTask *) t;
    Uint16 task_reqid = task->getReqId();
    Uint16 msg_reqid = 0;
    
    // ---
    // perform id checking here
    // ---

    if (msg_reqid == tid) {
	rc = true;
    }

    return rc;
  }
  
  ORBTask :: ORBTask (void)
    :PTask ("doors_orb", Scheduler :: Instance (), new ORBTaskSM()),
     down (this, &peer),
     peer (this, &down),
     requests (this),
     reqmux (new SinglePropagator (new PortList), new SinglePropagator (new PortList)),
     reqcnt (0)
  {

    requests.connect (reqmux.createUser ());
    _instance = this;

    STATIC_SYMBOL (peer, this);
    STATIC_SYMBOL (down, this);
  }

  ORBTask :: ~ORBTask (void) {
    _instance = 0;
  }
  

  String ORBTask :: object_to_string (Object_ptr obj){
      Frame iorfrm;
      Uint32 len = 0;
 
      IOP :: EncodeIor(*obj->my_ior, iorfrm, len);
      
      Frame striorfrm;
      IOP :: IorF2SF(striorfrm, iorfrm);  
  
      String stringifiedior ((char* )(Byte* ) striorfrm);
      return stringifiedior;
  }

  Object_ptr ORBTask :: string_to_object (const String& str){

    IOP::IOR iorified;
    Frame iorfrm;
    iorfrm.putFirst ((const Byte*) str.c_str(), (FrameSize) str.length());
    IOP :: IorSF2F(iorfrm, iorified);  
    
    return new Object (&iorified);
  }
    
  TypeCode_ptr ORBTask :: create_string_tc (ULong bound){
    
    TypeCode_ptr tc_ptr = new TypeCode(CORBA :: tk_string);
    tc_ptr->length_ = bound;
    return tc_ptr;
  }
  
  TypeCode_ptr ORBTask :: create_wstring_tc (ULong bound){
    
    TypeCode_ptr tc_ptr = new TypeCode(CORBA :: tk_wstring);
    tc_ptr->length_ = bound;
    return tc_ptr;
  }

  TypeCode_ptr ORBTask :: create_fixed_tc (UShort digits, UShort scale){

    TypeCode_ptr tc_ptr = new TypeCode(CORBA :: tk_fixed);
    tc_ptr->digits_ = digits;
    tc_ptr->scale_ = scale;
    return tc_ptr;
  }
    

  TypeCode_ptr ORBTask :: create_sequence_tc (ULong bound, TypeCode_ptr element_type){

    TypeCode_ptr tc_ptr = new TypeCode(CORBA :: tk_sequence);
    tc_ptr->length_ = bound;
    tc_ptr->contentType_ = element_type;
    return tc_ptr;
  }

  TypeCode_ptr ORBTask :: create_array_tc (ULong length, TypeCode_ptr element_type){

    TypeCode_ptr tc_ptr = new TypeCode(CORBA :: tk_array);
    tc_ptr->length_ = length;
    tc_ptr->contentType_ = element_type;
    return tc_ptr;
  }
    
  TypeCode_ptr ORBTask :: create_typecode (CORBA::TCKind kind){

      return (new TypeCode(kind));
  }
  
  ORBTask* ORBTask :: Instance (void) {

    if (_instance == 0){
      _instance = new ORB ();
    }

    return _instance;
  }

  void ORB :: run(void){

    Scheduler *s = Scheduler :: Instance ();
    s->run();
  }

  bool ORBTask :: active_cind (Message *msg) { 

    // we have remote incoming
    return false; 
  }

  bool ORBTask :: active_cconf (Message *msg) { 

    // remote says yes!
    bool rc = true;
    msg->lock ();
    
    if (requests.putMessage (msg) == -1) {
      msg->unlock ();  // Message can be deleted.
	rc = false;
    }
    
    return rc;
  }

  bool ORBTask :: active_dind (Message *msg) { 

    // connection disconnects
    bool rc = true;
    msg->lock ();
    
    if (requests.putMessage (msg) == -1) {
      msg->unlock ();  // Message can be deleted.
	rc = false;
    }
    
    return rc;
  }
  
  bool ORBTask :: active_Request (Message *msg) { 

    // remote sends Request
    return false; 
  }

  bool ORBTask :: active_Reply (Message *msg) { 
    
    // remote sends Reply
    bool rc = true;
    msg->lock ();
    
    if (requests.putMessage (msg) == -1) {
      msg->unlock ();  // Message can be deleted.
	rc = false;
    }
    
    return rc;
  }
  
  bool ORBTask :: active_CancelRequest (Message *msg) { 

    // remote cancels request
    return false; 
  }

  bool ORBTask :: active_LocateRequest (Message *msg) { 

    // remote sends locate request
    return false; 
  }

  bool ORBTask :: active_LocateReply (Message *msg) { 

    // remote sends locate reply
    bool rc = true;
    msg->lock ();
    
    if (requests.putMessage (msg) == -1) {
      msg->unlock ();  // Message can be deleted.
	rc = false;
    }
    
    return rc;
  }

  bool ORBTask :: active_Close (Message *msg) { 

    // remote closes giop connection
    bool rc = true;
    msg->lock ();
    
    if (requests.putMessage (msg) == -1) {
      msg->unlock ();  // Message can be deleted.
	rc = false;
    }
    
    return rc;
  }

  bool ORBTask :: active_Error (Message *msg) { 

    // remote signals error
    bool rc = true;
    msg->lock ();
    
    if (requests.putMessage (msg) == -1) {
      msg->unlock ();  // Message can be deleted.
	rc = false;
    }
    
    return rc;
  }

  bool ORBTask :: active_Fragment (Message *msg) { 

    // remote sends fragmented request
    return false; 
  }

  ORB *ORB :: _instance = 0;

  Uint16 ORBTask :: get_reqcnt(){
    return ++reqcnt;
  }

  Uint16 ORBTask :: get_reqcnt(){

    return ++reqcnt;
  }

  void ORBTask :: connectRequest (Request_ptr ptr) {

    // note: ORB doesn't create Request objects
    // CORBA::Objects create them, then request ORB
    // to connect to them

    Port *to_orb = reqmux.createProvider (isForRequestTask);
    ptr->entity.connect (to_orb);
}
  

