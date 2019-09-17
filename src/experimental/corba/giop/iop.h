//Editor-Info: -*- C++ -*-
//
//  Copyright 2002 
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


// $Log: iop.h,v $
// Revision 1.1  2002/09/19 12:56:32  bilhanan
// initial revision
//

#ifndef IOP_H
#define IOP_H


#include <doors/cdrcodec.h>


//======================================================================
// namespace IOP
//======================================================================
  
namespace IOP {

  //======================================================================
  // IOP::TaggedComponent
  //======================================================================
  
  typedef CORBA::ULong ComponentId;
  
  const ComponentId TAG_ORB_TYPE = 0;
  const ComponentId TAG_CODE_SETS = 1; 
  const ComponentId TAG_POLICIES = 2; 
  const ComponentId TAG_ALTERNATE_IIOP_ADDRESS = 3; 
  const ComponentId TAG_ASSOCIATION_OPTIONS = 13;
  const ComponentId TAG_SEC_NAME = 14;
  const ComponentId TAG_SPKM_1_SEC_MECH = 15;
  const ComponentId TAG_SPKM_2_SEC_MECH = 16;
  const ComponentId TAG_KerberosV5_SEC_MECH = 17;
  const ComponentId TAG_CSI_ECMA_Secret_SEC_MECH = 18;
  const ComponentId TAG_CSI_ECMA_Hybrid_SEC_MECH = 19;
  const ComponentId TAG_SSL_SEC_TRANS = 20;
  const ComponentId TAG_CSI_ECMA_Public_SEC_MECH = 21;
  const ComponentId TAG_GENERIC_SEC_MECH = 22;
  const ComponentId TAG_JAVA_CODEBASE = 25;
  const ComponentId TAG_COMPLETE_OBJECT_KEY = 5;
  const ComponentId TAG_ENDPOINT_ID_POSITION = 6;
  const ComponentId TAG_LOCATION_POLICY = 12;
  const ComponentId TAG_DCE_STRING_BINDING = 100;
  const ComponentId TAG_DCE_BINDING_NAME = 101;
  const ComponentId TAG_DCE_NO_PIPES = 102;
  const ComponentId TAG_DCE_SEC_MECH = 103; // Security Service

  // Standard way of representing multicomponent profiles.
  // This would be encapsulated in a TaggedProfile.
  
  struct TaggedComponent {
    ComponentId tag;
    CORBA::Sequence <CORBA::Octet> component_data;
  };

  typedef CORBA::Sequence <TaggedComponent> MultipleComponentProfile;

  
  inline void EncodeTaggedComponent(const TaggedComponent& from,
				    Frame &tofrm, Uint32 &len){
    Uint32 complen = from.tag;
    IiopCodec :: Encode( complen, tofrm, len );
    IiopCodec :: EncodeSequence( from.component_data, tofrm, len ); 
  };

  inline void DecodeTaggedComponent(TaggedComponent& to, 
				    Frame &fromfrm, bool swap, Uint32 &len){ 
    Sint32 frlen = fromfrm.length();
    if( frlen >= 8 ){ 
      IiopCodec :: Decode( to.tag, fromfrm, swap, len );
      IiopCodec :: DecodeSequence( to.component_data, fromfrm, swap, len );
      return;
    }
    _DOORS_DEBUG_( 0, "DecodeTaggedComponent, Frame length " << frlen << ", " 
	   << "while it should be at least " << 8 << "." );
  };
 
  inline void 
    EncodeSequenceTaggedComponent( const CORBA::Sequence< TaggedComponent >& from, 
				   Frame &tofrm, Uint32 &len){ 
    Uint32 seqlen = from.Length();
    IiopCodec :: Encode( seqlen, tofrm, len );
    for( CORBA::ULong i = 0 ; i < seqlen ; i++){ 
      EncodeTaggedComponent(from[i], tofrm, len);
    } 
  };
  
  inline void 
    DecodeSequenceTaggedComponent( CORBA::Sequence< TaggedComponent >& to, 
				   Frame &fromfrm, bool swap, Uint32 &len){
    Sint32 frlen = fromfrm.length();
    CORBA::ULong length = 4;
    if( frlen >= 4 ){ 
      IiopCodec :: Decode( length, fromfrm, swap, len );
      to.Length(length); 
      frlen -= 4;
      if( frlen >= (length * 4)){
	for(CORBA::ULong i = 0 ; i < length ; i++){ 
	  DecodeTaggedComponent(to[i], fromfrm, swap, len);
	}
	return;
      }
    }
    _DOORS_DEBUG_(0, "DecodeSequenceTaggedComponent, Frame length " << frlen << ", " 
	  << "while it should be at least " << length << ".");
  }; 

 
  //======================================================================
  // IOP::TaggedProfile
  //======================================================================
  
  typedef CORBA::ULong ProfileId;
  
  const ProfileId TAG_INTERNET_IOP = 0;
  const ProfileId TAG_MULTIPLE_COMPONENTS = 1;

  class TaggedProfile {
  public:
    ProfileId tag;
    //Sequence <CORBA::Octet> profile_data;
    Frame *profile_data;
 
    TaggedProfile() : tag(0), profile_data(0) {};
    
    ~TaggedProfile() { 
      if(profile_data) delete profile_data;
    }
  };
    
  
  inline void EncodeTaggedProfile( const TaggedProfile& from, Frame &tofrm, 
				   Uint32 &len ) {
    Uint32 proftag = from.tag;
    IiopCodec :: Encode( proftag, tofrm, len );
    //IiopCodec :: EncodeSequence( from.profile_data, tofrm, len ); 
    IiopCodec :: Encode( (Uint32)(from.profile_data->length()) , tofrm, len );
    tofrm.combine(*from.profile_data);
  };
  
  inline void DecodeTaggedProfile( TaggedProfile& to, Frame &fromfrm, 
				   bool swap, Uint32 &len ) {
    Sint32 frlen = fromfrm.length();
    if( frlen >= 8 ){ 
      IiopCodec :: Decode( to.tag, fromfrm, swap, len );
      //IiopCodec :: DecodeSequence( to.profile_data, fromfrm, swap, len );
      Uint32 profilelen;
      IiopCodec :: Decode( profilelen, fromfrm, swap, len );
      to.profile_data = &fromfrm.split(profilelen);
      return;
    }
    _DOORS_DEBUG_(0,  "DecodeTaggedProfile, Frame length " << frlen << ", " 
	   << "while it should be at least " << 8 << "." );
  };
  
  inline void
    EncodeSequenceTaggedProfile( const CORBA::Sequence< TaggedProfile >& from, 
				 Frame &tofrm, Uint32 &len){
    Uint32 seqlen = from.Length();
    IiopCodec::Encode(seqlen, tofrm, len);
    for(CORBA::ULong i = 0 ; i < seqlen ; i++){ 
      EncodeTaggedProfile(from[i], tofrm, len);
    }
  };

  inline void 
    DecodeSequenceTaggedProfile( CORBA::Sequence< IOP::TaggedProfile >& to, 
				 Frame &fromfrm, bool swap, Uint32 &len){
    Sint32 frlen = fromfrm.length();
    CORBA::ULong length = 4;
    if( frlen >= 4 ){ 
      IiopCodec::Decode(length, fromfrm, swap, len);
      to.Length(length); 
      frlen -= 4;
      if( frlen >= (length * 4)){
	  for(CORBA::ULong i = 0 ; i < length ; i++){ 
	    DecodeTaggedProfile(to[i], fromfrm, swap, len);
	  }
	  return;
	}
      }
      _DOORS_DEBUG_(0, "DecodeSequencePrincipal, Frame length " << frlen << ", " 
	    << "while it should be at least " << length << ".");
    }; 
  
  //======================================================================
  // IOP::IOR
  //======================================================================
  
  class IOR {
  public:
    Uint32 byte_order; // Own Addition
    CORBA::iiopString type_id;
    CORBA::Sequence <TaggedProfile> profiles;
  };

  inline void EncodeIor(const IOR& from, Frame &tofrm,
			Uint32 &len) {
    IiopCodec::EncodeString(from.type_id, tofrm, len);
    EncodeSequenceTaggedProfile(from.profiles, tofrm, len);
  };

  inline void DecodeIor(IOP::IOR& to, Frame &fromfrm,
  			bool swap, Uint32 &len) {
    IiopCodec::DecodeString(to.type_id, fromfrm, swap, len);
    DecodeSequenceTaggedProfile(to.profiles, fromfrm, swap, len);
  };

  inline Uint8 ByteOrder(void) {
    Uint16 test = 0x0102;
    if (((char *)&test)[0] == 1){
      return 0;
    }
    return 1;
  };
  
  inline void IorF2SF(Frame &tofrm, Frame &fromfrm) {
    Sint32 len = fromfrm.length();
    Sint32 count = 0;
    char tmp = 0;
    Byte val = 0;
    tofrm.putFirst((Byte *)"IOR:",4);
    if(ByteOrder()){
      tofrm.putLast('1');
      tofrm.putLast((Byte *)"0000000",7);
    } else {
      tofrm.putLast((Byte *)"00000000",8);
    }
    while( count < len ){
      val = fromfrm.getFirst();
      tmp = (val >> 4) + '0';
      if(tmp > '9'){
	tmp += ('a' - '9' - 1);}
      tofrm.putLast(tmp);
      tmp = (val & 0x0f) + '0';
      if(tmp > '9'){
	tmp += ('a' - '9' - 1);}
      tofrm.putLast(tmp);
      count++;
    }
    len = tofrm.length();
    int padding = 3 - ((len+3) % 4);
    tofrm.putLast((Byte *)ZEROS, padding);
  };

  
  inline Byte halfbyte(Byte val){
    Byte tmp = 0;
    if((val >= 'a') && (val <= 'f')){     
      tmp = val - 'a' + 10;
    }
    else if((val >= 'A') && (val <= 'F')){     
      tmp = val - 'A' + 10;
    }
    else if((val >= '0') && (val <= '9')){     
      tmp = val - '0';
    }
    else {
      _DOORS_DEBUG_(0, "IOP::IorSF2F, Invalid character in IOR:" << (int)val);
    }
    return tmp;
  };

  
  inline void IorSF2F(Frame &fromfrm, IOP :: IOR &ior) {
    Sint32 count = 0;
    Byte tmp = 0;
    Byte val = 0;
    Byte buf[12];
    for( tmp = 0; tmp < 12; ++tmp){
      buf[tmp] = fromfrm.getFirst();
    }
    if(memcmp(&buf[0],"IOR:",4)){
      _DOORS_DEBUG_(0, "IOP::IorSF2F, Wrong prefix in IOR");
    }
    bool swap;
    
    (buf[4] == '1') ? (ior.byte_order = 1) : (ior.byte_order = 0);
    (ior.byte_order == ByteOrder()) ? swap = 0 : swap = 1;
    

    Frame tofrm;
    Sint32 len = fromfrm.length() / 2;
    while( count < len ){
      tmp = fromfrm.getFirst();
      val = (halfbyte(tmp)) << 4;
      tmp = fromfrm.getFirst();
      val = val | halfbyte(tmp);
      tofrm.putLast(val);
      count++;
    }
    
    Uint32 declen = 0;
    DecodeIor( ior, tofrm, swap, declen); 

  };



 
  //======================================================================
  // IOP::ServiceContext
  //======================================================================

  typedef CORBA::Long ServiceId;

  const ServiceId TransactionService = 0;
  const ServiceId CodeSets = 1;
  const ServiceId ChainBypassCheck = 2;
  const ServiceId ChainBypassInfo = 3;
  const ServiceId LogicalThreadId = 4;
  const ServiceId BI_DIR_IIOP = 5;
  const ServiceId SendingContextRunTime = 6;
  const ServiceId INVOCATION_POLICIES = 7;
  const ServiceId FORWARDED_IDENTITY = 8;
  const ServiceId UnknownExceptionInfo= 9;

  struct ServiceContext {
    ServiceId context_id;
    CORBA::Sequence <CORBA::Octet> context_data;
  };

  typedef CORBA::Sequence <ServiceContext>ServiceContextList;


  inline void EncodeServiceContext(const ServiceContext& from,
				   Frame &tofrm, Uint32 &len){
    Uint32 ctxid = from.context_id;
    IiopCodec :: Encode( ctxid, tofrm, len );
    IiopCodec :: EncodeSequence( from.context_data, tofrm, len );
  };
  
  inline void DecodeServiceContext(ServiceContext& to, Frame &fromfrm, 
				   bool swap, Uint32 &len){ 
    Sint32 frlen = fromfrm.length();
    if( frlen >= 8 ){ 
      Uint32 tmp;
      IiopCodec :: Decode(tmp, fromfrm, swap, len ); 
      to.context_id = (Sint32)tmp;
      IiopCodec :: DecodeSequence( to.context_data, fromfrm, swap, len );
      return;
    }
    _DOORS_DEBUG_(0,  "DecodeTaggedProfile, Frame length " << frlen << ", " 
	   << "while it should be at least " << 8 << "." );
  };
 
  inline void 
    EncodeSequenceServiceContext( const CORBA::Sequence< ServiceContext >& from, 
				  Frame &tofrm, Uint32 &len){ 
    Uint32 seqlen = from.Length();
    IiopCodec :: Encode( seqlen, tofrm, len );
    for(CORBA::ULong i = 0 ; i < seqlen ; i++){ 
      EncodeServiceContext(from[i], tofrm, len);
    }
  };
 
  inline void 
    DecodeSequenceServiceContext( CORBA::Sequence< ServiceContext >& to, 
				  Frame &fromfrm, 
				  bool swap, Uint32 &len){
    Sint32 frlen = fromfrm.length();
    CORBA::ULong length = 4;
    if( frlen >= 4 ){ 
      IiopCodec :: Decode( length, fromfrm, swap, len );
      to.Length(length); 
      frlen -= 4;
      if( frlen >= (length * 4)){
	for(CORBA::ULong i = 0 ; i < length ; i++){ 
	  DecodeServiceContext(to[i], fromfrm, swap, len);
	}
	return;
      }
    }
    _DOORS_DEBUG_(0, "DecodeSequencePrincipal, Frame length " << frlen << ", " 
	  << "while it should be at least " << length << ".");
  };
  
} // end namespace IOP

//======================================================================


//======================================================================
// namespace CONV_FRAME
//======================================================================


namespace CONV_FRAME 
{ 
  
  //======================================================================
  // CONV_FRAME::CodeSetContext
  //======================================================================


  typedef CORBA::ULong CodeSetId; 
  
  struct CodeSetContext { 
    CodeSetId char_data; 
    CodeSetId wchar_data; 
  };

  inline void EncodeCodeSetContext(const CodeSetContext& from,
				    Frame &tofrm, Uint32 &len){
    IiopCodec :: Encode( from.char_data, tofrm, len );
    IiopCodec :: Encode( from.wchar_data, tofrm, len );
  };

  inline void DecodeCodeSetContext(CodeSetContext& to,
				   Frame &fromfrm, bool swap, Uint32 &len){
    Sint32 frlen = fromfrm.length();
    if( frlen >= 8 ){ 
      IiopCodec :: Decode( to.char_data, fromfrm, swap, len );
      IiopCodec :: Decode( to.wchar_data, fromfrm, swap, len );
      return;
    }
    _DOORS_DEBUG_(0,  "DecodeCodeSetContext, Frame length " << frlen << ", " 
	   << "while it should be at least " << 8 << "." );
  };
  
  //======================================================================
  // CONV_FRAME::CodeSetComponent
  //======================================================================

  struct CodeSetComponent { 
    CodeSetId native_code_set; 
    CORBA::Sequence<CodeSetId> conversion_code_sets;
  };
  
  inline void EncodeCodeSetComponent(const CodeSetComponent& from,
				    Frame &tofrm, Uint32 &len){
    Uint32 ncs = from.native_code_set;
    IiopCodec :: Encode( ncs, tofrm, len );
    IiopCodec :: EncodeSequence( from.conversion_code_sets, tofrm, len ); 
  };

  inline void DecodeCodeSetComponent(CodeSetComponent& to, 
				     Frame &fromfrm, bool swap, Uint32 &len){ 
    Sint32 frlen = fromfrm.length();
    if( frlen >= 8 ){ 
      IiopCodec :: Decode( to.native_code_set, fromfrm, swap, len );
      IiopCodec :: DecodeSequence( to.conversion_code_sets, fromfrm, 
				   swap, len );
      return;
    }
    _DOORS_DEBUG_(0,  "DecodeCodeSetComponent, Frame length " << frlen << ", " 
	   << "while it should be at least " << 8 << "." );
  };

  //======================================================================
  // CONV_FRAME::CodeSetComponentInfo
  //======================================================================

  
  struct CodeSetComponentInfo { 
    CodeSetComponent ForCharData; 
    CodeSetComponent ForWcharData;
  };

  inline void EncodeCodeSetComponentInfo(const CodeSetComponentInfo& from,
					 Frame &tofrm, Uint32 &len){
    EncodeCodeSetComponent(from.ForCharData, tofrm, len );
    EncodeCodeSetComponent(from.ForWcharData, tofrm, len );
  };

  inline void DecodeCodeSetComponentInfo(CodeSetComponentInfo& to,
					 Frame &fromfrm, 
					 bool swap, Uint32 &len){
    DecodeCodeSetComponent(to.ForCharData, fromfrm, swap, len );
    DecodeCodeSetComponent(to.ForWcharData, fromfrm, swap, len );
  };

};

#endif
