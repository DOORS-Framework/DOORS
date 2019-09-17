// -*- C++ -*-

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

$Log: typecode.h,v $
Revision 1.1  2002/08/15 06:57:28  bilhanan
development

Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:46:06  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.10  2001/06/08 08:39:47  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef _doors_typecode_
#define _doors_typecode_

#include <doors/datatypes.h>

namespace CORBA {
  
  class Any;
  class ORB;
  class TypeCode;
  typedef TypeCode* TypeCode_ptr;
  
  // class TypeCode : public gc_cleanup {

  class TypeCode {

    friend class CORBA :: Any;
    friend class ORB;
    
    
    enum CORBA::TCKind kind_;       // TypeCode kind
    
    // CORBA_tk_objref, CORBA_tk_struct, CORBA_tk_union, 
    // CORBA_tk_enum, CORBA_tk_alias, CORBA_tk_except
    String id_;
    String name_;
    
    
    std::vector<String> memberNames_;    // tk_struct, tk_union, tk_enum
    
    std::vector<TypeCode> memberTypes_;  // tk_struct, tk_union
    
    // for tk_union only
    std::vector<Any> case_labels_;
    TypeCode_ptr discriminatorType_;
    
    // tk_string, tk_sequence, tk_array
    ULong length_;
    
    // tk_sequence, tk_array, tk_alias
    TypeCode_ptr contentType_;
    
    // tk_sequence (for recursive sequence type codes)
    ULong offset_;
    
    // tk_fixed
    UShort digits_;
    UShort scale_;

    TypeCode (CORBA::TCKind kind);
    TypeCode (const TypeCode_ptr tc);
    
    
    
  public: 
    
    virtual ~TypeCode (void);    
    class Bounds { }; 
    class BadKind { }; 
    TCKind kind() const; 
    CORBA::Boolean equal(TypeCode_ptr) const; 
    
    const String id() const;  // was RepositoryId  id() const, Repository ID not yet implemented.
    const String name() const; // was Identifier name() const
    ULong member_count() const; 
    const String member_name(ULong index) const; // was const char* member_name(ULong index) const;
    TypeCode_ptr member_type(ULong index) const; 
    Any *member_label(ULong index) const; 
    TypeCode_ptr discriminator_type() const; 
    Long default_index() const; 
    ULong length() const; 
    TypeCode_ptr content_type() const; 
    UShort fixed_digits() const; 
    Short fixed_scale() const; 
    
    static TypeCode_ptr _duplicate (TypeCode_ptr tc);

    static TypeCode_ptr _nil (void);

  }; 
  
  void release (TypeCode_ptr p);
  CORBA::Boolean is_nil (TypeCode_ptr tc);
  
}

#endif
