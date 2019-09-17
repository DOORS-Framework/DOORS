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

//  $Log: orb.h,v $
//  Revision 1.4  2002/08/13 10:10:41  bilhanan
//  Development
//
//  Revision 1.3  2002/08/02 13:39:11  bilhanan
//  development
//
//  Revision 1.2  2002/07/29 12:08:03  bilhanan
//  *** empty log message ***
//
//  Revision 1.1  2002/07/25 19:28:21  bilhanan
//  Initial revision
//

#ifndef ORB_H
#define ORB_H

#include <doors/datatypes.h>
#include <doors/ptb.h>
#include <doors/corba_dcl.h>

namespace CORBA {


  class ORBTask :: public PTask {

  public:
    
    static ORBTask* Instance (void); 
    virtual ~ORBTask (void);
    
    GiopPeer peer;
    Tran::User down;
    
     // --- 
    
    // put state machine functions here
    
    // ---
    
    
   // From this point down, all functions are from CORBA specs.
    
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
    
    void run(void);
    
    
  private: 

    Uint16 reqcnt;
    friend class Object;
    friend class RequestTask;
    friend class ORBTaskSM;
    
    ORBTask ();  // construct singleton style

    Uint16 get_reqcnt ();
    void connectRequest (Request_ptr ptr);

    // from transport layer
    bool active_cind (Message *msg);
    bool active_cconf (Message *msg);
    bool active_dind (Message *msg);
    
    // from peer
    bool active_Request (Message *msg);
    bool active_Reply (Message *msg);
    bool active_CancelRequest (Message *msg);
    bool active_LocateRequest (Message *msg);
    bool active_LocateReply (Message *msg);
    bool active_Close (Message *msg);
    bool active_Error (Message *msg);
    bool active_Fragment (Message *msg);
    
    Port requests;
    Port ToPOA;
    PortMux reqmux;
    
    static ORBTask* _instance;
    InetAddr ownaddr;




  };


    typedef String ORBid;
  
  static ORB_ptr ORB_init (ORBid orb_identifier) {
    
    if (!(orb_identifier.compare("doors_orb"))){
      return (ORB :: Instance());
    }
  }

}

#endif
