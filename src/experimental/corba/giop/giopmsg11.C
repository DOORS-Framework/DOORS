//Editor-Info: -*- C++ -*-
//
//  Copyright 2003 
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


// $Log: giopmsg11.C,v $
// Revision 1.1  2003/01/23 14:41:39  bilhanan
// Initial revision
//


#include <doors/doorsconf.h>
#include <doors/debug.h>
#include <doors/giopmsg11.h>
#include <doors/cdrcodec.h>

void GiopMsg_1_1 :: EncodeMessageHeader_1_1 (Frame &destfrm, Uint32 &len) {

  IiopCodec :: Encode( (Byte *)&giophdr.magic, destfrm, 4, len );
  IiopCodec :: Encode( (Byte *)&giophdr.GIOP_version.major, destfrm, 1, len );
  IiopCodec :: Encode( (Byte *)&giophdr.GIOP_version.minor, destfrm, 1, len );
  IiopCodec :: Encode( (Byte *)&giophdr.flags, destfrm, 1, len );
  IiopCodec :: Encode( (Byte *)&giophdr.message_type, destfrm, 1, len );
  IiopCodec :: Encode( (Byte *)&giophdr.message_size, destfrm, 4, len );
  
};
  

void GiopMesg_1_1 :: DecodeMessageHeader_1_1 (Frame &srcfrm, Uint32 &len){ 
  
    Sint32 frlen = srcfrm.length();
    if( frlen < 12 ){ 
      
      _DOORS_DEBUG_(0,  "GIOP MessageHeader length " << frlen << "but should be 12." );
      return;
      
    } else {
      
      IiopCodec :: Decode( (Byte *)&giophdr.magic, srcfrm, 0, 4, len );
      IiopCodec :: Decode( (Byte *)&giophdr.GIOP_version.major, srcfrm, 0, 1, len );
      IiopCodec :: Decode( (Byte *)&giophdr.GIOP_version.minor, srcfrm, 0, 1, len );
      IiopCodec :: Decode( (Byte *)&giophdr.flags, srcfrm, 0, 1, len );
      IiopCodec :: Decode( (Byte *)&giophdr.message_type, srcfrm, 0, 1, len );
      
      if (giophdr.flags != LOCAL_BYTE_ORDER)
	swap = true;
      
      IiopCodec :: Decode( (Byte *)&giophdr.message_size, srcfrm, swap, 4, len );
      return;
    }
    
}; 

