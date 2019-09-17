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

$Log: iiop.h,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:21  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.9  2001/06/08 08:40:19  bilhanan
Full licence clause added into doors 0.1


*/
//  =========================================================================
//  File:     iiop.h
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================



#ifndef IIOP_H
#define IIOP_H


#include <doors/bascodec.h>

//======================================================================
// namespace IIOP
//======================================================================

namespace IIOP { 

  struct Version { 
    CORBA::UShort major; //??
    CORBA::UShort minor; //??
  }; 


  //======================================================================
  // IIOP::ProfileBody_1_0
  //======================================================================

  class ProfileBody_1_0 {
  public:
    Version iiop_version;
    CORBA::iiopString host; 
    CORBA::UShort port; 
    CORBA::Sequence <CORBA::Octet> object_key; 
  
    ProfileBody_1_0(){
      iiop_version.major = 1;
      iiop_version.minor = 0;
      port = DEFAULT_PORT;
    };
  }; 
  
  inline void EncodeProfileBody_1_0(const ProfileBody_1_0& from, Frame &tofrm,
				    Uint32 &len) {
    IiopCodec :: Encode( from.iiop_version.major,tofrm, len );
    IiopCodec :: Encode( from.iiop_version.minor,tofrm, len );
    IiopCodec :: EncodeString( from.host, tofrm, len );
    IiopCodec :: Encode( from.port, tofrm, len );
    IiopCodec :: EncodeSequence( from.object_key, tofrm, len ); 
  };

  inline void DecodeProfileBody_1_0( ProfileBody_1_0& to, Frame &fromfrm,
				     bool swap, Uint32 &len) {
    Sint32 frlen = fromfrm.length();
    if( frlen >= 12 ){ 
      IiopCodec :: Decode( to.iiop_version.major, fromfrm, swap, len );
      IiopCodec :: Decode( to.iiop_version.minor, fromfrm, swap, len );
      IiopCodec :: DecodeString( to.host, fromfrm, swap, len );
      IiopCodec :: Decode( to.port, fromfrm, swap, len );
      IiopCodec :: DecodeSequence( to.object_key,fromfrm, swap, len );
      return;
    }
    ERROR( "DecodeProfileBody_1_0, Frame length " << frlen << ", " 
	   << "while it should be at least " << 12 << "." );
  };
  
  
  //======================================================================
  // IIOP::ProfileBody_1_1
  //======================================================================

  struct ProfileBody_1_1 {
    Version iiop_version; 
    CORBA::iiopString host;
    unsigned short port; 
    CORBA::Sequence <CORBA::Octet> object_key;
    CORBA::Sequence <IOP::TaggedComponent> components; 
  }; 
  
  inline void EncodeProfileBody_1_1(const ProfileBody_1_1& from, Frame &tofrm,
				    Uint32 &len) {
    IiopCodec :: Encode( from.iiop_version.major,tofrm, len );
    IiopCodec :: Encode( from.iiop_version.minor,tofrm, len );
    IiopCodec :: EncodeString( from.host, tofrm, len );
    IiopCodec :: Encode( from.port, tofrm, len );
    IiopCodec :: EncodeSequence( from.object_key, tofrm, len ); 
    IOP :: EncodeSequenceTaggedComponent( from.components, tofrm, len ); 
  };
  
  inline void DecodeProfileBody_1_1( ProfileBody_1_1& to, Frame &fromfrm,
				     bool swap, Uint32 &len) {
    Sint32 frlen = fromfrm.length();
    if( frlen >= 16 ){ 
      IiopCodec :: Decode( to.iiop_version.major, fromfrm, swap, len );
      IiopCodec :: Decode( to.iiop_version.minor, fromfrm, swap, len );
      IiopCodec :: DecodeString( to.host, fromfrm, swap, len );
      IiopCodec :: Decode( to.port, fromfrm, swap, len );
      IiopCodec :: DecodeSequence( to.object_key,fromfrm, swap, len );
      IOP :: DecodeSequenceTaggedComponent( to.components, fromfrm, swap, len ); 
      return;
    }
    ERROR( "DecodeProfileBody_1_1, Frame length " << frlen << ", " 
	   << "while it should be at least " << 16 << "." );
  };


  //======================================================================
  // IIOP::BiDirIIOPServiceContext
  //======================================================================

  struct ListenPoint { 
    CORBA::iiopString host; 
    CORBA::UShort port; 
  }; 
  
  typedef CORBA::Sequence<ListenPoint> ListenPointList; 

  struct BiDirIIOPServiceContext {// BI_DIR_IIOP Service Context
    ListenPointList listen_points; 
  }; 
  
}; // end namespace IIOP

//=====================================================================


//======================================================================
// namespace BiDirPolicy
//======================================================================


namespace BiDirPolicy { 
  
  typedef CORBA :: UShort BidirectionalPolicyValue; 
  
  const BidirectionalPolicyValue NORMAL = 0; 
  const BidirectionalPolicyValue BOTH = 1; 
  const CORBA::PolicyType BIDIRECTIONAL_POLICY_TYPE = 36; 
  
  class BidirectionalPolicy : public CORBA::Policy { 
    BidirectionalPolicyValue value; 
  }; 
};


#endif
