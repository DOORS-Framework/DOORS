
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

$Log: namedvalue.h,v $
Revision 1.3  2003/04/17 12:52:58  bilhanan
Encode() marshalling function call introduced.

Revision 1.2  2003/01/30 15:25:30  bilhanan
development

Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:45  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.11  2001/06/08 08:39:43  bilhanan
Full licence clause added into doors 0.1


*/



#ifndef _doors_namedvalue_
#define _doors_namedvalue_

#include <doors/datatypes.h>

namespace CORBA {

  class NamedValue;
  class NVList;
  class Any;

  typedef NamedValue* NamedValue_ptr;
  typedef NVList* NVList_ptr;


  class NamedValue{ 
    
    friend class NVList;
    String *name_;
    Any *value_;
    Flags flags_;

  protected:
    NamedValue (void);
    NamedValue (String *n, Any *v, Flags m);
    
  public: 

    ~NamedValue (void) { } // let garbage collection take care of destruction?

    String *name() const; 
    Any *value() const; 
    Flags flags() const; 
    static NamedValue_ptr _duplicate(NamedValue_ptr nv);
    static NamedValue_ptr _nil(void);
    bool Encode (Frame &dest, Uint32 &length);
  };

  class NVList {

    friend class ORB;
    NVList (void);
    NVList (const NVList &other);


    std::vector <NamedValue> listmgr;

  public: 
    ULong count(void) const; 
    NamedValue_ptr add(Flags m); 
    NamedValue_ptr add_item(const String s, Flags m); 
    NamedValue_ptr add_value(const String s, const Any& a, Flags m); 
    NamedValue_ptr add_item_consume(String *s, Flags m); 
    NamedValue_ptr add_value_consume(String *s, Any *a, Flags m); 
    NamedValue_ptr item(ULong element); 
    Status remove(ULong element);  // note: Status will become obsolete in corba 2.3, hence using void.
    static NVList_ptr _duplicate(NVList_ptr nv = 0);
    static NVList_ptr _nil(void);

  };


  CORBA::Boolean is_nil(NamedValue_ptr ptr);
  CORBA::Boolean is_nil(NVList_ptr ptr);
  void release(NamedValue_ptr ptr);
  void release(NVList_ptr ptr);

}

#endif
