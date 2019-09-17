//Editor-Info: -*- C++ -*-
//
//  Copyright 2002 
//      Telecom Lab, Tampere University of Technology.  
//      All rights reserved.
 
 
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

//  $Log

#ifndef GIOPMSG11_H
#define GIOPMSG11_H

#include <doors/message.h>
#include <doors/datatypes.h>

namespace GIOP {
  
  struct Version { 
    CORBA::Octet major; 
    CORBA::Octet minor; 
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

  struct MessageHeader_1_1 { 
    char magic [4];  // The string "GIOP". 
    Version GIOP_version; 
    CORBA::Octet flags; 
    CORBA::Octet message_type; 
    CORBA::ULong message_size; 
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

  enum ReplyStatusType_1_1 {
    NO_EXCEPTION,
    USER_EXCEPTION,
    SYSTEM_EXCEPTION,
    LOCATION_FORWARD
  };
  

  struct ReplyHeader_1_1 {
    IOP::ServiceContextList service_context;
    CORBA::ULong request_id;
    ReplyStatusType_1_1 reply_status;
  };
  
  struct SystemExceptionReplyBody {
    CORBA::iiopString exception_id;
    CORBA::ULong minor_code_value;
    CORBA::ULong completion_status;
  };

   struct CancelRequestHeader {
    CORBA::ULong request_id;
  };

  struct LocateRequestHeader_1_1 {
    CORBA::ULong request_id;
    CORBA::Sequence <CORBA::Octet> object_key;
  };
  
  enum LocateStatusType_1_1 {
    UNKNOWN_OBJECT,
    OBJECT_HERE,
    OBJECT_FORWARD
  };
  
  struct LocateReplyHeader_1_1 {
    CORBA::ULong request_id;
    LocateStatusType_1_1 locate_status;
  };
  

}
  
class GiopMsg_1_1 : public Message {

public:

  /** Initialises all data members */
  
  GiopMsg_1_1 (void);

  /** Copy Constructor */

  GiopMsg_1_1 (const GiopMsg_1_1 &msg);


 protected:
  struct GIOP :: MessageHeader_1_1 giophdr;
  Uint32 tcp_cep; // for transport layer connection identifier
  bool swap;  // determines whether swapping is necessary during cdr decode.
  Uint32 len; // length of decoded/encoded number of octets thus far.
  Uint32 encmsgsize; // length of encoded giop body.

  bool EncodeMessageHeader_1_1 (Frame &frm, Uint32 &len);  
  bool DecodeMessageHeader_1_1 (Frame *frm);  
};

inline GiopMsg_1_1 ::  GiopMsg_1_1 (void) 
  : giophdr.magic ('GIOP'),
  giophdr.GIOP_version.major (0),
  giophdr.GIOP_version.minor (0),
  swap (false),
  len (0)
{
}

inline GiopMsg_1_1 :: GiopMsg_1_1 (const GiopMsg_1_1 &other)
  : giophdr (other.giophdr)
{
}

#endif
