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

$Log: any.h,v $
Revision 1.2  2003/04/17 12:52:58  bilhanan
Encode() marshalling function call introduced.

Revision 1.1.1.1  2002/05/13 14:38:35  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:22  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.12  2001/06/08 08:39:41  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef _doors_any_
#define _doors_any_

#include <doors/datatypes.h>


namespace CORBA {

  class Object;
  typedef Object* Object_ptr;
  class TypeCode;
  typedef TypeCode* TypeCode_ptr;

  class Any { 

  public: 
    Any(); 
    Any(const Any&); 
    Any(TypeCode_ptr tc, void *value, CORBA::Boolean should_release = CORBA::FALSE); 
    virtual ~Any(void); 
    Any &operator=(const Any&);

    void insert (int kind, void *value);
    void move (TypeCode_ptr tc, void *value, CORBA::Boolean should_release = CORBA::TRUE);

    struct from_boolean { 
      from_boolean(CORBA::Boolean b) : val(b) {} 
      CORBA::Boolean val; 
    };

    struct from_octet { 
      from_octet(Octet o) : val(o) {} 
      Octet val; 
    };

    struct from_char { 
      from_char(Char c) : val(c) {} 
      Char val; 
    }; 

    struct from_string { 
      from_string(String s, ULong b, CORBA::Boolean n = CORBA::FALSE) : val(s), bound(b), nocopy(n) {} 
      String val; 
      ULong bound;
      CORBA::Boolean nocopy;
    }; 

    void operator<<=(Short); 
    void operator<<=(UShort); 
    void operator<<=(Long); 
    void operator<<=(ULong); 
    void operator<<=(Float);
    void operator<<=(Double); 
    void operator<<=(const Any&);  // copying 
    void operator<<=(Any*);        // non-copying 
    void operator<<=(const String);

    void operator<<=(from_boolean); 
    void operator<<=(from_char); 
    void operator<<=(from_octet); 
    void operator<<=(from_string); 

    struct to_boolean { 
      to_boolean(CORBA::Boolean &b) : ref(b) {} 
      Boolean &ref; 
    }; 

    struct to_char { 
      to_char(Char &c) : ref(c) {} 
      Char &ref; 
    }; 

    struct to_octet { 
      to_octet(Octet &o) : ref(o) {} 
      Octet &ref; 
    }; 

    struct to_object { 
      to_object(Object_ptr &obj) : ref(obj) {} 
      Object_ptr &ref; 
    }; 

    struct to_string {
      to_string(String &s, ULong b) : val(s), bound(b) {} 
      String &val; 
      ULong bound; 
    }; 

    CORBA::Boolean operator>>=(Short&) const; 
    CORBA::Boolean operator>>=(UShort&) const;
    CORBA::Boolean operator>>=(Long&) const; 
    CORBA::Boolean operator>>=(ULong&) const;
    CORBA::Boolean operator>>=(Float&) const; 
    CORBA::Boolean operator>>=(Double&) const;
    CORBA::Boolean operator>>=(Any*&) const; 
    CORBA::Boolean operator>>=(String&) const;

    CORBA::Boolean operator>>=(to_boolean) const; 
    CORBA::Boolean operator>>=(to_char) const; 
    CORBA::Boolean operator>>=(to_octet) const; 
    CORBA::Boolean operator>>=(to_object) const; 
    CORBA::Boolean operator>>=(to_string) const; 

    void replace(TypeCode_ptr tc, void *value, CORBA::Boolean should_release = CORBA::FALSE); 
    TypeCode_ptr type() const; 
    const void *value() const; 

    bool Encode (Frame &dest, Uint32 &length);

  private: 

    TypeCode_ptr tc_;
    void *value_;

    void operator<<=(unsigned char) {}; 
    CORBA::Boolean operator>>=(unsigned char&) const {return CORBA::FALSE;}; 

    void deletevalue(void);
  };

};

#endif  
