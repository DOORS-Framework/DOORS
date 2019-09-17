//Editor-Info: -*- C++ -*-
//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
//
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

$Log: giop.h,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.2  2002/03/19 14:24:02  bilhanan
*** empty log message ***

Revision 1.1  2002/01/23 13:10:20  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.5  2001/06/08 08:40:18  bilhanan
Full licence clause added into doors 0.1

*/

// Original Author: Janne Makihonka

#ifndef GIOP_H
#define GIOP_H


#include <doors/bastypes.h>
#include <doors/iop.h>

// Contains the GIOP module specified in page 15-59 of CORBA v2.4.2
// Basic datamembers that will be used by GIOP PDUs

namespace GIOP {
  
  struct Version { 
    CORBA::Octet major; 
    CORBA::Octet minor; 
  };

  enum MsgType_1_0 {    
    Request, 
    Reply, 
    CancelRequest, 
    LocateRequest, 
    LocateReply, 
    CloseConnection, 
    MessageError 
  };

  enum MsgType_1_1 {
    Request, 
    Reply, 
    CancelRequest, 
    LocateRequest, 
    LocateReply, 
    CloseConnection, 
    MessageError, 
    Fragment     
  }; 

  struct MessageHeader_1_0 {
    char magic [4];  // The string "GIOP".
    GIOP::Version GIOP_version;
    CORBA::Boolean byte_order;
    CORBA::Octet message_type;
    CORBA::ULong message_size;
  };


  struct MessageHeader_1_1 { 
    char magic [4];  // The string "GIOP". 
    Version GIOP_version; 
    CORBA::Octet flags; 
    CORBA::Octet message_type; 
    CORBA::ULong message_size; 
  };

  typedef MessageHeader_1_1 MessageHeader_1_2;

  struct RequestHeader_1_0 {
    
    IOP::ServiceContextList service_context;
    CORBA::ULong request_id;
    CORBA::Boolean response_expected;
    CORBA::Sequence <CORBA::Octet> object_key;
    CORBA::iiopString operation;
    CORBA::OctetSeq requesting_principal;
  };
  
  struct RequestHeader_1_1 {     
    IOP::ServiceContextList service_context; 
    CORBA::ULong request_id; 
    CORBA::Boolean response_expected; 
    CORBA::Octet reserved[3]; 
    CORBA::Sequence <CORBA::Octet> object_key; 
    CORBA::iiopString operation; 
    CORBA::OctetSeq requesting_principal; 
  };

  typedef short AddressingDisposition; 
  const short KeyAddr = 0;
  const short ProfileAddr = 1;
  const short ReferenceAddr = 2;

  struct IORAddressingInfo { 
    CORBA::ULong selected_profile_index; 
    IOP::IOR ior; 
  }; 

  union TargetAddressunion {                   // switches are
    CORBA::Sequence <CORBA::Octet> object_key; // KeyAddr
    IOP::TaggedProfile profile;                // ProfileAddr
    IORAddressingInfo ior;                     // ReferenceAddr
  };

  class TargetAddress {
  public:
    
    AddressingDisposition addrdisp;  //discriminator for Target Address
    TargetAddressunion val;
    
  };

  struct RequestHeader_1_2 { 
    CORBA::ULong request_id; 
    CORBA::Octet response_flags; 
    CORBA::Octet reserved[3]; 
    TargetAddress target; 
    CORBA::iiopString operation; 
    IOP::ServiceContextList service_context; 
  }; 
  
  enum ReplyStatusType_1_0 {
    NO_EXCEPTION,
    USER_EXCEPTION,
    SYSTEM_EXCEPTION,
    LOCATION_FORWARD
  };
  

  struct ReplyHeader_1_0 {
    IOP::ServiceContextList service_context;
    CORBA::ULong request_id;
    ReplyStatusType_1_0 reply_status;
  };
  
  typedef ReplyHeader_1_0 ReplyHeader_1_1;

  enum ReplyStatusType_1_2 { 
    NO_EXCEPTION, 
    USER_EXCEPTION, 
    SYSTEM_EXCEPTION, 
    LOCATION_FORWARD, 
    LOCATION_FORWARD_PERM, 
    NEEDS_ADDRESSING_MODE 
  };
  
  struct ReplyHeader_1_2 { 
    CORBA::ULong request_id; 
    ReplyStatusType_1_2 reply_status; 
    IOP::ServiceContextList service_context; 
  }; 

  struct SystemExceptionReplyBody {
    CORBA::iiopString exception_id;
    CORBA::ULong minor_code_value;
    CORBA::ULong completion_status;
  };

   struct CancelRequestHeader {
    CORBA::ULong request_id;
  };

  struct LocateRequestHeader_1_0 {
    CORBA::ULong request_id;
    CORBA::Sequence <CORBA::Octet> object_key;
  };
  
  
  typedef LocateRequestHeader_1_0 LocateRequestHeader_1_1;

  enum LocateStatusType_1_0 {
    UNKNOWN_OBJECT,
    OBJECT_HERE,
    OBJECT_FORWARD
  };
  
  struct LocateReplyHeader_1_0 {
    CORBA::ULong request_id;
    LocateStatusType_1_0 locate_status;
  };
  
  typedef LocateReplyHeader_1_0 LocateReplyHeader_1_1;

  enum LocateStatusType_1_2 { 
    UNKNOWN_OBJECT, 
    OBJECT_HERE, 
    OBJECT_FORWARD, 
    OBJECT_FORWARD_PERM, 
    LOC_SYSTEM_EXCEPTION,
    LOC_NEEDS_ADDRESSING_MODE 
  }; 
  
  struct LocateReplyHeader_1_2 { 
    CORBA::ULong request_id; 
    LocateStatusType_1_2 locate_status; 
  }; 
  
  struct FragmentHeader_1_2 { 
    unsigned long request_id; 
  }; 
  

  // no clue yet wtf this does for which area of CDR/IIOP now, 
  //but let's keep it here. - Bill

  inline void RemoveLeading(Frame& frm, CORBA::ULong firstSize) {
  if (frm.length() > 0) {

  CORBA::ULong byteCounter,len;
  byteCounter = * (CORBA::ULong*) frm.getFirst(sizeof(CORBA::ULong));


  // here we could only deal with first parameter
  //   of size 0 1 2 4 8 

  len = byteCounter;

  // the purpose of the next 2 lines is too profound for me to grasp. -Bill  
  if (firstSize == 0);
  else if (firstSize == 1);
  else if (firstSize == 2)
  len += len % 2;
  else if (firstSize == 4) {
  len += 3;
  len -= len % 4;
  }
  else if (firstSize == 8) {
  len += 7;
  len -= len % 8;
  }
  else
  throw "first Size must be 0, 1, 2, 4 or 8";
  frm.getFirst(len - byteCounter);
  }
  


#endif
