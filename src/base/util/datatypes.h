
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

$Log: datatypes.h,v $
Revision 1.2  2003/01/30 15:25:57  bilhanan
c99 types introduced.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:26  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.8  2001/06/08 08:39:42  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef _doors_datatypes_h_
#define _doors_datatypes_h_

#if 0
#include <gc.h>
#include <gc_cpp.h>
#endif

//#include "gc/gc_cpp.h"

#define new_gc new

#include <string>
#include <vector>
#include <stdint.h>
//#include <cwchar>

namespace CORBA {

  typedef unsigned char Boolean; 
  typedef unsigned char Char; 
  typedef wchar_t WChar;  
  typedef unsigned char Octet; 
  typedef int16_t Short; 
  typedef uint16_t UShort; 
  typedef int32_t Long; 
  //  typedef ...Long Long; 
  typedef uint32_t ULong; 
  //  typedef ...ULong Long; 
  typedef float Float; 
  typedef uint64_t Double; 
  typedef long double LongDouble; 
  typedef Boolean& Boolean_out; 
  typedef Char& Char_out; 
  typedef WChar& WChar_out; 
  typedef Octet& Octet_out; 
  typedef Short& Short_out; 
  typedef UShort& UShort_out; 
  typedef Long& Long_out; 
  //  typedef LongLong& LongLong_out; 
  typedef ULong& ULong_out; 
  //  typedef ULongLong& ULongLong_out; 
  typedef Float& Float_out; 
  typedef Double& Double_out; 
  //  typedef LongDouble& LongDouble_out;
  typedef std::string String;

  // some necessary typedefs

  typedef void Status;     // <-- Status will become obsolete in future specs
  typedef ULong Flags;

  // why these flag values? coz they're bitmasks.

  const Flags ARG_IN = 1;
  const Flags ARG_OUT = 2;
  const Flags ARG_INOUT = 3;

  const Flags OUT_LIST_MEMORY = 8;
  const Flags IN_COPY_VALUE = 16;

  const Boolean TRUE = 1;
  const Boolean FALSE = 0;

  typedef std::string Context;
  typedef Context* Context_ptr;

  enum TCKind {tk_null, tk_void, tk_short, tk_long, tk_ushort, tk_ulong, tk_float, tk_double, tk_boolean, tk_char, tk_octet, tk_any, tk_TypeCode, tk_Principal, tk_objref, tk_struct, tk_union, tk_enum, tk_string, tk_sequence, tk_array, tk_alias, tk_except, tk_longlong, tk_ulonglong, tk_longdouble, tk_wchar, tk_wstring, tk_fixed};

  typedef std::string ObjectId;
  typedef std::string RepositoryId;
  
  typedef unsigned long PolicyType;

  const CORBA::PolicyType THREAD_POLICY_ID = 16;
  const CORBA::PolicyType LIFESPAN_POLICY_ID = 17;
  const CORBA::PolicyType ID_UNIQUENESS_POLICY_ID = 18;
  const CORBA::PolicyType ID_ASSIGNMENT_POLICY_ID = 19;
  const CORBA::PolicyType IMPLICIT_ACTIVATION_POLICY_ID = 20;
  const CORBA::PolicyType SERVANT_RETENTION_POLICY_ID = 21;
  const CORBA::PolicyType REQUEST_PROCESSING_POLICY_ID = 22;

  class Policy {
  public:
    PolicyType policy_type;
  };
 
};

  
#endif
