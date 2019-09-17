
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

$Log: typecode.C,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:46:41  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.8  2001/06/08 08:39:47  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/any.h>
#include <doors/orb.h>
#include <doors/typecode.h>


namespace CORBA {

  void release (TypeCode_ptr p){
    if (p) delete p;
  }
  
  CORBA::Boolean is_nil (TypeCode_ptr tc) {
    return tc == 0;
  }
  
  TypeCode::TypeCode(CORBA::TCKind kind): kind_(kind){
  }

  
  // Copy Constructor. Hell, copy everything; may be faster than conditional checking.
  // Ok, that's a big assumption. Let's verify later.

  TypeCode :: TypeCode (const TypeCode_ptr tc) : 
    kind_ (tc->kind_), id_ (tc->id_), name_ (tc->name_),
    memberNames_ (tc->memberNames_), memberTypes_ (tc->memberTypes_),
    case_labels_ (tc->case_labels_), length_ (tc->length_), offset_ (tc->offset_)
  {
    if (!is_nil(tc->discriminatorType_))
      discriminatorType_ = TypeCode:: _duplicate(tc->discriminatorType_);
    if (!is_nil(tc->contentType_))
      contentType_ = TypeCode:: _duplicate(tc->contentType_);
  }

  TypeCode::~TypeCode(){
  }
  
  CORBA::Boolean TypeCode::equal(TypeCode_ptr p) const {

    if(p == this)
      return CORBA::TRUE;
    
    if(this->kind_ != p->kind())
      return CORBA::FALSE;
    
    if(this->kind_ == tk_objref ||
       this->kind_ == tk_struct ||
       this -> kind_ == tk_union ||
       this -> kind_ == tk_enum ||
       this -> kind_ == tk_alias ||
       this -> kind_ == tk_except)
      {
 	if((this->id_ != p->id_) ||
	   (this->name_ != p->name_))
	  return CORBA::FALSE;
      }
    
    if(this -> kind_ == tk_struct ||
       this -> kind_ == tk_union ||
       this -> kind_ == tk_enum)
      {
 	if(this->memberNames_.size() != p->memberNames_.size())
	  return CORBA::FALSE;
	
 	/* do individual element check here
	   for(ULong i = 0 ; i < tc1 -> memberNames_.length() ; i++)
	   if(strcmp(tc1 -> memberNames_[i],
	   tc2 -> memberNames_[i]) != 0)
	   return CORBA::FALSE;
	*/
      }
    
    if(this->kind_ == tk_struct ||
       this->kind_ == tk_union)
      {
	if(this->memberTypes_.size() != p->memberTypes_.size())
	  return CORBA::FALSE;
	
	/* do individual element check here
	   for(ULong i = 0 ; i < tc1 -> memberTypes_.length() ; i++)
	   if(!(tc1 -> memberTypes_[i] ->
	   equal(tc2 -> memberTypes_[i])))
	   return CORBA::FALSE;
	*/
      }
    
    if(this-> kind_ == tk_union)
      {
	if(this-> case_labels_.size() != p->case_labels_.size())
	  return CORBA::FALSE;
	
	/* do elementwise check here 
	   for(ULong i = 0 ; i < tc1 -> labels_.length() ; i++)
	   {
	   TypeCode_var ltc1 = tc1 -> labels_[i].type();
	   TypeCode_var ltc2 = tc2 -> labels_[i].type();
	   
	   if(!ltc1 -> equal(ltc2))
	   return CORBA::FALSE;
	   
	   const void* v1 = tc1 -> labels_[i].value();
	   const void* v2 = tc2 -> labels_[i].value();
	   
	   ltc1 = OBGetOrigType(ltc1);
	   switch(ltc1 -> kind())
	   {
	   case tk_short:
	   if(*(Short*)v1 != *(Short*)v2)
	   return CORBA::FALSE;
	   break;
	   
	   case tk_ushort:
	   if(*(UShort*)v1 != *(UShort*)v2)
	   return CORBA::FALSE;
	   break;
	   
	   case tk_long:
	   if(*(Long*)v1 != *(Long*)v2)
	   return CORBA::FALSE;
	   break;
	   
	   case tk_ulong:
	   case tk_enum:
	   if(*(ULong*)v1 != *(ULong*)v2)
	   return CORBA::FALSE;
	   break;
	   
	   case tk_char:
	   if(*(Char*)v1 != *(Char*)v2)
	   return CORBA::FALSE;
	   break;
	   
	   case tk_boolean:
	   if(*(CORBA::Boolean*)v1 != *(CORBA::Boolean*)v2)
	   return CORBA::FALSE;
	   break;
	   
	    case tk_octet:
	    break;
	    
	    default:
	    assert(CORBA::FALSE);
	    break;
	    }
	    } */ 
      }
    
    if(this-> kind_ == tk_union)
      {
	if(!(this->discriminatorType_->equal(p->discriminatorType_)))
	  return CORBA::FALSE;
      }
    
    if(this-> kind_ == tk_string ||
       this-> kind_ == tk_sequence ||
       this-> kind_ == tk_array)
      {
	if(this->length_ != p->length_)
	  return CORBA::FALSE;
      }
    
    if(this->kind_ == tk_array ||
       this->kind_ == tk_sequence ||
       this->kind_ == tk_alias)
      {
	if(!(this->contentType_->equal(p->contentType_)))
	  return CORBA::FALSE;
      }
    
    return CORBA::TRUE;
  }

  TCKind TypeCode::kind() const{
    return kind_;
  }
  
  const String TypeCode::id() const
    // throw(TypeCode::BadKind)
  {
    if(!(kind_ == tk_objref ||
	 kind_ == tk_struct ||
	 kind_ == tk_union ||
	 kind_ == tk_enum ||
	 kind_ == tk_alias ||
	 kind_ == tk_except))
      {
	// throw BadKind();
      }
    
    return id_;
  }
  
  const String TypeCode::name() const
    // throw(TypeCode::BadKind)
  {
    if(!(kind_ == tk_objref ||
	 kind_ == tk_struct ||
	 kind_ == tk_union ||
	 kind_ == tk_enum ||
	 kind_ == tk_alias ||
	 kind_ == tk_except))
      {
	// throw BadKind();
      }
    
    return name_;
  }

  ULong TypeCode::member_count() const
    // throw(TypeCode::BadKind)
  {
    if(!(kind_ == tk_struct ||
	 kind_ == tk_union ||
	 kind_ == tk_enum))
      {
	// throw BadKind();
      }
    
    return memberNames_.size();
  }

const String TypeCode::member_name(ULong index) const
    // throw(TypeCode::BadKind, TypeCode::Bounds)
{
    if(!(kind_ == tk_struct ||
	 kind_ == tk_union ||
	 kind_ == tk_enum))
      {
	// throw BadKind();
      }

    return memberNames_.at(index);

}

  TypeCode_ptr TypeCode::member_type(ULong index) const
    // throw(TypeCode::BadKind, TypeCode::Bounds)
  {
    if(!(kind_ == tk_struct ||
	 kind_ == tk_union))
      {
	// throw BadKind();
      }
    
       return TypeCode::_duplicate((CORBA::TypeCode*)&memberTypes_.at(index));
  }

  Any* TypeCode::member_label(ULong index) const
    // throw(TypeCode::BadKind, TypeCode::Bounds)
  {
    if(!(kind_ == tk_union))
      {
	// throw BadKind();
      }

      return new Any(case_labels_.at(index));

  }
  
  TypeCode_ptr TypeCode :: discriminator_type() const
    // throw(TypeCode::BadKind)
  {
    if(!(kind_ == tk_union))
      {
	// throw BadKind();
      }
    
    return TypeCode::_duplicate(discriminatorType_);
  }

  Long TypeCode :: default_index() const
    // throw(TypeCode::BadKind)
  {
    if(!(kind_ == tk_union))
      {
	// throw BadKind();
      }
    // retrieve default index 
    for(Long i = 0 ; i < (Long)case_labels_.size() ; i++)
    {
      TypeCode_ptr tc = case_labels_[i].type();
      if (tc->kind() == tk_octet)
	return i;
    }
    
    return -1;
    
  }
  
  ULong TypeCode::length() const
    // throw(TypeCode::BadKind)
  {
    if(!(kind_ == tk_string ||
	 kind_ == tk_sequence ||
	 kind_ == tk_wstring ||
	 kind_ == tk_array))
      {
	// throw BadKind();
      }
    
    return length_;
  }

  TypeCode_ptr TypeCode::content_type() const
    // throw(TypeCode::BadKind)
  {
    if(!(kind_ == tk_sequence ||
	 kind_ == tk_array ||
	 kind_ == tk_alias))
      {
	// throw BadKind();
      }
    
    return TypeCode::_duplicate(contentType_);
  }

   UShort TypeCode::fixed_digits() const
     // throw (TypeCode::BadKind) 
   {
     return digits_;
   }

   Short TypeCode::fixed_scale() const 
     // throw (TypeCode::BadKind) 
   {
     return scale_;
   }
  
  TypeCode_ptr TypeCode :: _duplicate (TypeCode_ptr tc) {

      if (!is_nil (tc)){
	return ::new_gc TypeCode(tc);
      } else return 0;
  }

  TypeCode_ptr TypeCode :: _nil (void){
    return 0;
  }
  
}
