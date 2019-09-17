
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

$Log: namedvalue.C,v $
Revision 1.2  2003/04/17 12:52:58  bilhanan
Encode() marshalling function call introduced.

Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:46:39  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.8  2001/06/08 08:39:43  bilhanan
Full licence clause added into doors 0.1


*/



#include <doors/namedvalue.h>
#include <doors/any.h>

namespace CORBA {

  CORBA::Boolean is_nil(NamedValue_ptr ptr){
    return ptr == 0;
  }
  
  CORBA::Boolean is_nil(NVList_ptr ptr) {
    return ptr == 0;
  }

//    void release(NamedValue_ptr ptr){
//      if (ptr) delete ptr;
//    }

//    void release(NVList_ptr ptr){
//      if (ptr) delete ptr;
//    }

  NamedValue :: NamedValue (void) 
    : name_ (new_gc String), value_ (new_gc Any), flags_ (0) { 
  }

  NamedValue :: NamedValue (String *n, Any *v, Flags m)
    : name_ (n), value_ (v), flags_ (m) { 
  }

  String* NamedValue :: name() const {
    return this->name_;
  }

  Any* NamedValue :: value() const {
    return this->value_;
  }

  Flags NamedValue :: flags() const {
    return flags_;
  }

  NamedValue_ptr NamedValue :: _duplicate(NamedValue_ptr nv){
    if (!is_nil(nv)){
      return new_gc NamedValue(*nv);
    }
    else {
      return new_gc NamedValue;
    }
  }
  
  NamedValue_ptr NamedValue :: _nil(void){ 
    return 0;
  }

  bool NamedValue :: Encode (Frame &dest, Uint32 &length){

    return value_->Encode (dest, length);
  }

  NVList :: NVList (void){
  }

  NVList :: NVList (const NVList &other) : listmgr (other.listmgr){
  }

  ULong NVList :: count (void) const {
    return ((ULong) listmgr.size());
  }

  NamedValue_ptr NVList :: add(Flags m){ 

    NamedValue_ptr p = new_gc NamedValue (new_gc String, new_gc Any, m);
    listmgr.insert (listmgr.end(), *p);
    return p;
  } 

  NamedValue_ptr NVList :: add_item(const String s, Flags m){ 
    NamedValue_ptr p = new_gc NamedValue (new_gc String(s), new_gc Any, m);
    listmgr.insert (listmgr.end(), *p);
    return p;
  } 
  
  NamedValue_ptr NVList :: add_value(const String s, const Any& a, Flags m){ 
    NamedValue_ptr p = new_gc NamedValue (new_gc String(s), new_gc Any(a), m);
    listmgr.insert (listmgr.end(), *p);
    return p;
  } 
  
  NamedValue_ptr NVList :: add_item_consume(String *s, Flags m){ 
    NamedValue_ptr p = new_gc NamedValue (s, new_gc Any, m);
    listmgr.insert (listmgr.end(), *p);
    s = 0;
    return p;
  } 
  
  NamedValue_ptr NVList :: add_value_consume(String *s, Any *a, Flags m){ 
    NamedValue_ptr p = new_gc NamedValue (s, a, m);
    listmgr.insert (listmgr.end(), *p);
    s = 0;
    a = 0;
    return p;
  } 
  
  NamedValue_ptr NVList :: item(ULong element){ 
    return &listmgr[element];
  } 
  
  Status NVList :: remove (ULong element) {
    std::vector <NamedValue>::iterator iter = listmgr.begin() + element;
    listmgr.erase (iter);
  }

  NVList_ptr NVList :: _duplicate(NVList_ptr nv) {
    if (!is_nil(nv)){
      return new_gc NVList(*nv);
    }
    else{
      return new_gc NVList;
    }
  } 

  NVList_ptr NVList :: _nil(void){
    return 0;
  }
}
  
