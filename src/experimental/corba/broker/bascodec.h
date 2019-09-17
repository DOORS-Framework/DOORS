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

$Log: bascodec.h,v $
Revision 1.1.1.1  2002/05/13 14:38:35  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:23  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.9  2001/06/08 08:40:10  bilhanan
Full licence clause added into doors 0.1


*/
//  =========================================================================
//  File:     bascodec.h
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================


#ifndef BASCODEC_H 
#define BASCODEC_H 
 

#include <doors/iiopdefs.h>
#include <doors/bastypes.h> 
#include <doors/principal.h> 

namespace IiopCodec{ 
 
#define SWAP4(x) x = ((x << 24) | ((x & 0xff00) << 8) | \
		      ((x >> 8) & 0xff00) | (x >> 24)); 
  
#define SWAP2(x) x = ((x << 8) | ( x >> 8));  
  
#define ZEROS "\0\0\0\0\0\0\0\0"

  inline void Encode(Uint32 val, Frame &tofrm, Uint32 &len )
    {
      int padding = 3 - ((len+3) % 4);
      tofrm.putLast((Byte *)ZEROS, padding);
      tofrm.putLast((Byte *)&val, 4);
      len += padding + 4;
    };
  
  inline void Encode(Uint16 val, Frame &tofrm, Uint32 &len )
    {
      int padding = len % 2;
      tofrm.putLast((Byte *)ZEROS, padding);
      tofrm.putLast((Byte *)&val, 2);
      len += padding + 2;
    };
  
  inline void Encode(Uint8 val, Frame &tofrm, Uint32 &len )
    {
      tofrm.putLast(val);
      len += 1;
    };
  
  
  inline void Decode(Uint32 &val, Frame &fromfrm, bool swap, Uint32 &len)
    {
      int padding = 3 - ((len+3) % 4);
      fromfrm.delFirst(padding);
      fromfrm.getFirst(4, (Byte *)&val);
      len += padding + 4; 
      if(swap) { 
	SWAP4(val);
      }
    };
  
  inline void Decode(Uint16 &val, Frame &fromfrm, bool swap, Uint32 &len)
    {
      int padding = len % 2;
      fromfrm.delFirst(padding);
      fromfrm.getFirst(2, (Byte *)&val);
      len += padding + 2; 
      if(swap) { 
	SWAP2(val);
      }
    };
  
  inline void Decode(Uint8 &val, Frame &fromfrm, bool swap, Uint32 &len)
    {
      val = fromfrm.getFirst();
      len += 1; 
    };
  
  
  template<class T> void  
    Encode(const T *from, Frame &tofrm, Uint32 n, Uint32 &len) 
    { 
      int Align = sizeof(T); 
      int padding = 0;
      if(n){ 
	switch(Align){ 
	case 1:
	  tofrm.putLast((Byte *)from, n);
	  break; 
	case 2: 
	  padding = len % 2; 
	  tofrm.putLast((Byte *)ZEROS, padding);
	  tofrm.putLast((Byte *)from, n * 2);  
	  break; 
	case 4: 
	  padding = 3 - ((len + 3) % 4);
	  tofrm.putLast((Byte *)ZEROS, padding);
	  tofrm.putLast((Byte *)from, n * 4);
	  break; 
	case 8: 
	  padding = 7 - ((len + 7) % 8);
	  tofrm.putLast((Byte *)ZEROS, padding);
	  tofrm.putLast((Byte *)from, n * 8);
	  break; 
	default: 
	  n = 0;
	  break; 
	} 
	len += n*Align + padding; 
      } 
    }
  
  
  
  template<class T> void Decode(T *to, Frame &fromfrm, bool swap, 
				Uint32 n, Uint32 &len) 
    { 
      int Align = sizeof(T); 
      int padding = 0;
      if(n){
	int amount = n;
	switch(Align){ 
	case 1:
	  fromfrm.getFirst(n,(Byte *) to);
	  break; 
	case 2: 
	  padding = len % 2; 
	  fromfrm.delFirst(padding);
	  fromfrm.getFirst(n * 2, (Byte *) to);
	  if(swap) { 
	    Uint16 *v = (Uint16 *)to; 
	    while(n-- > 0) { 
	      SWAP2(*v);
	      ++v;
	    }
	  } 
	  break; 
	case 4: 
	  padding = 3 - ((len+3) % 4);
	  fromfrm.delFirst(padding);
	  fromfrm.getFirst(n * 4, (Byte *) to);
	  if(swap) { 
	    Uint32* v = (Uint32 *)to; 
	    while(n-- > 0) { 
	      SWAP4(*v);
	      ++v;
	    }
	  } 
	  break; 
	case 8: 
	  padding = 7 - ((len+7) % 8);
	  fromfrm.delFirst(padding);
	  fromfrm.getFirst(n * 8, (Byte *) to);
	  if(swap) { 
	    Uint32* v = (Uint32 *)to; 
	    while(n-- > 0) { 
	      Uint32 v0 = v[0]; 
	      Uint32 v1 = v[1]; 
	      SWAP4(v0); SWAP4(v1);
	      v[0] = v1; 
	      v[1] = v0; 
	      v += 2; 
	    } 
	  } 
	  break; 
	default: 
	  n = 0;
	  break; 
	} 
	len += amount*Align + padding; 
      } 
    } 
  
  
  template<class T> inline void  
    DecodeSequence( CORBA::Sequence< T > &to, Frame &fromfrm, 
		    bool swap, Uint32 &len) 
    { 
      Sint32 frlen = fromfrm.length();
      CORBA::ULong length = 4;
      if( frlen >= 4 ){ 
	Decode(length, fromfrm, swap, len);
	to.Length(length); 
	frlen -= 4;
	if( frlen >= (length * sizeof(T))){
	  Decode(to.Data(), fromfrm, swap, length, len); 
	  return;
	}
      }
      ERROR("DecodeSequence, Frame length " << frlen << ", " 
	    << "while it should be at least " << length * sizeof(T) << ".");
    } 
  
  
  template<class T> inline void  
    EncodeSequence(const CORBA::Sequence< T >& from, Frame &tofrm,  
		   CORBA::ULong &len) 
    { 
      Uint32 seqlen = from.Length();
      Encode(seqlen, tofrm, len);
      Encode(from.Data(), tofrm, seqlen, len); 
    } 
  
  
  inline void DecodeString(CORBA::iiopString& str, Frame &fromfrm, bool swap, 
			   Uint32 &len) 
    { 
      Sint32 frlen = fromfrm.length();
      CORBA::ULong length = 4;
      if( frlen >= 4 ){ 
	Decode(length, fromfrm, swap, len);
	str.Length(length); 
	frlen -= 4;
	if( frlen >= (length)){
	  //char *tmpbuf = new char[length];
	  //Decode(tmpbuf, fromfrm, swap, length, len);
	  Decode(str.Data(), fromfrm, swap, length, len); 
	  //str.insert(0,tmpbuf,length);
	  return;
	}
      }
      ERROR("DecodeString, Frame length " << frlen << ", " 
	    << "while it should be at least " << length << ".");
    }; 
  
  
  inline void EncodeString(const CORBA::iiopString& str, Frame &tofrm, 
			   Uint32 &len) 
    { 
      Uint32 strlen = str.Length();
      Encode(strlen, tofrm, len);
      Encode(str.Data(), tofrm, strlen, len); 
      //Encode(str.data(), tofrm, strlen, len); 
    }; 
  
  
  inline void EncodePrincipal(const CORBA_Principal& pri, Frame &tofrm,  
			      Uint32 &len) 
    { 
      EncodeSequence(pri.seq, tofrm, len); 
    }; 
  
  
  inline void DecodePrincipal(CORBA_Principal& pri, Frame &fromfrm, 
			      bool swap, Uint32 &len) 
    { 
      DecodeSequence(pri.seq, fromfrm, swap, len); 
    }; 
  
  
  inline void EncodePrincipal(const Principal_Ptr& pri,Frame &tofrm,  
			      Uint32 &len) 
    { 
      EncodeSequence(pri->seq, tofrm, len); 
    }; 
  
  
  inline void DecodePrincipal(Principal_Ptr& pri, Frame &fromfrm,
			      bool swap, Uint32 &len) 
    { 
      //Sequence<CORBA::Octet> seq; 
      DecodeSequence(pri->seq, fromfrm, swap, len); 
    }; 
  
  inline void EncodeSequencePrincipal(const Principal_Seq& from, 
				      Frame &tofrm, Uint32 &len) 
    { 
      Uint32 seqlen = from.Length();
      Encode(seqlen, tofrm, len);
      for(CORBA::ULong i = 0 ; i < seqlen ; i++){ 
	EncodePrincipal(from[i], tofrm, len); 
      } 
    }; 
  
  inline void DecodeSequencePrincipal(Principal_Seq& to, Frame &fromfrm,
				      bool swap, Uint32 &len) 
    { 
      Sint32 frlen = fromfrm.length();
      CORBA::ULong length = 4;
      if( frlen >= 4 ){ 
	Decode(length, fromfrm, swap, len);
	to.Length(length); 
	frlen -= 4;
	if( frlen >= (length * 4)){
	  for(CORBA::ULong i = 0 ; i < length ; i++){ 
	    DecodePrincipal(to[i], fromfrm, swap, len); 
	  }
	  return;
	}
      }
      ERROR("DecodeSequencePrincipal, Frame length " << frlen << ", " 
	    << "while it should be at least " << length << ".");
    }; 
  
}; //namespace IiopCodec

#endif 
 
 
