
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

$Log: any.C,v $
Revision 1.2  2003/04/17 12:52:58  bilhanan
Encode() marshalling function call introduced.

Revision 1.1.1.1  2002/05/13 14:38:35  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:46:38  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.13  2001/06/08 08:39:41  bilhanan
Full licence clause added into doors 0.1


*/



#include <doors/typecode.h>
#include <doors/object.h>
#include <doors/any.h>
#include <doors/cdrcodec.h>

CORBA :: Any :: Any ()
  : tc_(0), value_(0){
}

CORBA :: Any :: Any (const Any& a)
  : tc_(CORBA :: TypeCode :: _duplicate(a.tc_)), value_ (a.value_){
}

CORBA :: Any :: Any(TypeCode_ptr tc, void *value, CORBA :: Boolean should_release)
  : tc_(CORBA :: TypeCode :: _duplicate(tc)), value_(value){
  
  if (should_release) {
    value = 0;
  }
}

CORBA :: Any :: ~Any(void){
  
  //value_ may be shared area. Leave its destruction to gc. 
  value_ = 0; 
}

CORBA :: Any& CORBA :: Any :: operator=(const CORBA :: Any& other)
{
  if(this != &other)
    {
      tc_ = CORBA :: TypeCode :: _duplicate(other.type());
      
      switch(tc_-> kind())
	{
	case CORBA :: tk_null:
	case CORBA :: tk_void:
	  break;
	  
	case CORBA :: tk_short:
	  value_ = new_gc CORBA :: Short(*(const CORBA :: Short*)other.value());
	  break;
	  
	case CORBA :: tk_long:
	  value_ = new_gc CORBA :: Long(*(const CORBA :: Long*)other.value());
	  break;
	  
	case CORBA :: tk_ushort:
	  value_ = new_gc CORBA :: UShort(*(const CORBA :: UShort*)other.value());
	  break;
	  
	case CORBA :: tk_ulong:
	case CORBA :: tk_enum:
	  value_ = new_gc CORBA :: ULong(*(const CORBA :: ULong*)other.value());
	  break;
	  
	case CORBA :: tk_float:
	  value_ = new_gc CORBA :: Float(*(const CORBA :: Float*)other.value());
	  break;
	  
	case CORBA :: tk_double:
	  value_ = new_gc CORBA :: Double(*(const CORBA :: Double*)other.value());
	  break;
	  
	case CORBA :: tk_boolean:
	  value_ = new_gc CORBA :: Boolean(*(const CORBA :: Boolean*)other.value());
	  break;
	  
	case CORBA :: tk_char:
	  value_ = new_gc CORBA :: Char(*(const CORBA :: Char*)other.value());
	  break;
	  
	case CORBA :: tk_octet:
	  value_ = new_gc CORBA :: Octet(*(const CORBA :: Octet*)other.value());
	  break;
	  
	case CORBA :: tk_any:
	  value_ = new_gc CORBA :: Any(*(const CORBA :: Any*)other.value());
	  break;
	  
	case CORBA :: tk_TypeCode:
	  value_ = CORBA :: TypeCode :: _duplicate((CORBA :: TypeCode_ptr)other.value());
	  break;
	  
	  //  	case CORBA :: tk_Principal:
	  //  	value_ = Principal::_duplicate((Principal_ptr)other.value());
	  //  	break;
	  
	case CORBA :: tk_objref:
	  value_ = CORBA :: Object:: _duplicate((CORBA :: Object_ptr)other.value());
	  break;
	  
	case CORBA :: tk_struct:
	case CORBA :: tk_sequence:
	case CORBA :: tk_union:
	case CORBA :: tk_array:
	case CORBA :: tk_except:
	  
	  break;
	  
	case CORBA :: tk_string:
	  value_ = new CORBA :: String(((CORBA :: String *)other.value_)->c_str());
	  break;
	  
	case CORBA :: tk_alias: // No alias possible here,
	  break;     // this is to keep the compiler happy
	}
      
      //	replace(other.type_, val, CORBA::TRUE, other.info_);
    }
  
  return *this;
}

void CORBA :: Any :: insert (int kind, void *value){
  
  if (tc_) {
    if (value_){
      this->deletevalue();
    }
    delete tc_;
  }
  
  tc_ = new_gc CORBA :: TypeCode((CORBA :: TCKind) kind);
  value_ = value;
}

void CORBA :: Any :: move (CORBA :: TypeCode_ptr tc, void *value, CORBA :: Boolean should_release) {
  
  if (tc_)
    delete tc_;
  
  tc_ = tc;
  value_ = value;
  
  if (should_release == CORBA :: TRUE){
    tc = 0;
    value = 0;
  }
}

void CORBA :: Any :: operator<<=(CORBA :: Short v)
{
  insert(CORBA :: tk_short, new_gc CORBA :: Short(v));
}

void CORBA :: Any :: operator<<=(CORBA :: UShort v)
{
  insert(CORBA :: tk_ushort, new_gc CORBA :: UShort(v));
}

void CORBA :: Any :: operator<<=(CORBA :: Long v)
{
  insert(CORBA :: tk_long, new_gc CORBA :: Long(v));
}

void CORBA :: Any :: operator<<=(CORBA :: ULong v)
{
  insert(CORBA::tk_ulong, new_gc CORBA :: ULong(v));
}

void CORBA :: Any :: operator<<=(CORBA :: Float v)
{
  insert(CORBA :: tk_float, new_gc CORBA :: Float(v));
}

void CORBA :: Any :: operator<<=(CORBA :: Double v)
{
  insert(CORBA :: tk_double, new_gc CORBA :: Double(v));
}

void CORBA :: Any :: operator<<=(const CORBA :: Any& v)
{
  insert(CORBA :: tk_any, new CORBA :: Any(v));
}

void CORBA :: Any :: operator<<=(CORBA :: Any* v)
{
  insert(CORBA :: tk_any, v);
  v = 0;
}

void CORBA :: Any :: operator<<=(const CORBA :: String v)
{
  insert(CORBA :: tk_string, new_gc CORBA :: String (v));
}

void CORBA :: Any :: operator<<=(CORBA :: Any :: from_boolean v)
{
  replace((new CORBA :: TypeCode(CORBA :: tk_boolean)), new CORBA :: Boolean(v.val), CORBA :: TRUE);
}

void CORBA :: Any :: operator<<=(CORBA :: Any :: from_char v)
{
  replace((new CORBA :: TypeCode(CORBA :: tk_char)), new CORBA :: Char(v.val), CORBA :: TRUE);
}

void CORBA :: Any :: operator<<=(CORBA :: Any :: from_octet v)
{
  replace((new CORBA :: TypeCode(CORBA :: tk_octet)), new CORBA :: Octet(v.val), CORBA :: TRUE);
}

void CORBA :: Any :: operator<<=(CORBA :: Any :: from_string v)
{
  if(v.nocopy == CORBA :: TRUE)
    replace((new CORBA :: TypeCode(CORBA :: tk_string)), &v.val, CORBA :: TRUE);
  else
    replace((new CORBA :: TypeCode(CORBA :: tk_string)), new CORBA :: String (v.val), CORBA :: TRUE);
}

CORBA :: Boolean CORBA :: Any :: operator>>=(CORBA :: Short& v) const
{
  if(this->tc_->kind_ == CORBA :: tk_short)
    {
      v = *(CORBA :: Short*)value_;
      return CORBA :: TRUE;
    }
  else
    return CORBA :: FALSE;
}

CORBA :: Boolean CORBA :: Any :: operator>>=(CORBA :: UShort& v) const
{
  if(this->tc_->kind_ == CORBA :: tk_ushort)
    {
      v = *(CORBA :: UShort*)value_;
      return CORBA :: TRUE;
    }
  else
    return CORBA :: FALSE;
}

CORBA :: Boolean CORBA :: Any :: operator>>=(CORBA :: Long& v) const
{
  if(this->tc_->kind_ == CORBA :: tk_long)
    {
      v = *(CORBA :: Long*)value_;
      return CORBA :: TRUE;
    }
  else
    return CORBA :: FALSE;
}

CORBA :: Boolean CORBA :: Any :: operator>>=(CORBA :: ULong& v) const
{
  if(this->tc_->kind_ == CORBA :: tk_ulong)
    {
      v = *(CORBA :: ULong*)value_;
      return CORBA :: TRUE;
    }
  else
    return CORBA :: FALSE;
}

CORBA :: Boolean CORBA :: Any::operator>>=(CORBA :: Float& v) const
{
  if(this->tc_->kind_ == CORBA :: tk_float)
    {
      v = *(CORBA :: Float*)value_;
      return CORBA :: TRUE;
    }
  else
    return CORBA :: FALSE;
}

CORBA :: Boolean CORBA :: Any :: operator>>=(CORBA :: Double& v) const
{
  if(this->tc_->kind_ == CORBA :: tk_double)
    {
      v = *(CORBA :: Double*)value_;
      return CORBA :: TRUE;
    }
  else
    return CORBA :: FALSE;
}

CORBA :: Boolean CORBA :: Any :: operator>>=(CORBA :: String& v) const
{
  if(this->tc_->kind_ == CORBA :: tk_string)
    {
      v = *(CORBA :: String*)value_;
      return CORBA :: TRUE;
    }
  else
    return CORBA :: FALSE;
}

CORBA :: Boolean CORBA :: Any::operator>>=(CORBA :: Any*& v) const
{
  if(this->tc_->kind_ == CORBA :: tk_any)
    {
      v = (CORBA :: Any*)value_;
      return CORBA :: TRUE;
    }
  else
    return CORBA :: FALSE;
}

//    CORBA::Boolean Any::operator>>=(TypeCode_ptr& v) const
//    {
//      if(this->tc_->kind_ == CORBA::tk_TypeCode)
//      {
//        v = (TypeCode_ptr)value_;
//        return CORBA::TRUE;
//      }
//      else
//      return CORBA::FALSE;
//    }

//    CORBA::Boolean Any::operator>>=(Principal_ptr& v) const
//    {
//      if(this->tc_->kind_ == CORBA::tk_Principal)
//      {
//        v = (Principal_ptr)value_;
//        return CORBA::TRUE;
//      }
//      else
//      return CORBA::FALSE;
//    }

//    CORBA::Boolean Any::operator>>=(Object_ptr& v) const
//    {
//      if(this->tc_->kind_ == CORBA::tk_Object)
//      {
//        v = value_;
//        return CORBA::TRUE;
//      }
//      else
//      return CORBA::FALSE;
//    }

CORBA :: Boolean CORBA :: Any :: operator>>=(CORBA :: Any :: to_boolean v) const
{
  if(this->tc_->kind_ == CORBA :: tk_boolean)
    {
      v.ref = *(CORBA :: Boolean*)value_;
      return CORBA :: TRUE;
    }
  else
    return CORBA :: FALSE;
}

CORBA :: Boolean CORBA :: Any :: operator>>=(CORBA :: Any :: to_char v) const
{
  if(this->tc_->kind_ == CORBA :: tk_char)
    {
      v.ref = *(CORBA :: Char*)value_;
      return CORBA :: TRUE;
    }
  else
    return CORBA :: FALSE;
}

CORBA :: Boolean CORBA :: Any :: operator>>=(CORBA :: Any :: to_octet v) const
{
  if(this->tc_->kind() == CORBA :: tk_octet)
    {
      v.ref = *(CORBA :: Octet*)value_;
      return CORBA :: TRUE;
    }
  else return CORBA :: FALSE;
}

CORBA :: Boolean CORBA :: Any :: operator>>=(CORBA :: Any :: to_string v) const
{
  if(this->tc_->kind() == CORBA :: tk_string && this->tc_->length() == v.bound)
    {
      v.val = *(CORBA :: String*)value_;
      return CORBA :: TRUE;
    }
  else
    return CORBA :: FALSE;
}

CORBA :: Boolean CORBA :: Any :: operator>>=(CORBA :: Any :: to_object v) const
{
  if(this->tc_->kind() == CORBA :: tk_objref)
    {
      v.ref = (CORBA :: Object_ptr)value_;
      return CORBA :: TRUE;
    }
  else
    return CORBA :: FALSE;
}

// Replace type

void CORBA :: Any :: replace(CORBA :: TypeCode_ptr tc, void* val, CORBA :: Boolean rel)
{
  //    tc_ = CORBA :: TypeCode ::_duplicate (tc);
  CORBA :: release (tc_);
  tc_ = tc;
  tc = 0;
  value_ = val;
  if (rel == CORBA :: TRUE) {
    val = 0;
  }
  
}

// Return type and value

CORBA :: TypeCode_ptr CORBA :: Any :: type() const
{
  return CORBA :: TypeCode :: _duplicate(tc_);
}

const void* CORBA :: Any:: value() const
{
  return value_;
}


bool CORBA :: Any :: Encode (Frame &dest, Uint32 & length){
  
  bool retval = false;
  
  switch(tc_->kind())
    {
    case CORBA :: tk_null:
    case CORBA :: tk_void:
      retval = true;
      break;
      
    case CORBA :: tk_char:
    case CORBA :: tk_boolean:
    case CORBA :: tk_octet:

      IiopCodec :: Encode ((Uint8) *value_, dest, len);
      retval = true;
      break;

    case CORBA :: tk_short:
    case CORBA :: tk_ushort:
      
      IiopCodec :: Encode ((Uint16) *value_, dest, len);
      retval = true;
      break;
      
    case CORBA :: tk_long:
    case CORBA :: tk_ulong:
    case CORBA :: tk_enum:
    case CORBA :: tk_float:
       
      IiopCodec :: Encode ((Uint32) *value_, dest, len);
      retval = true;
      break;
      
    case CORBA :: tk_string:

      IiopCodec :: EncodeString ((const std::string &) *value_, dest, len);
      retval = true;
      break;

    case CORBA :: tk_double:

      IiopCodec :: Encode ((Int64) *value_, dest, len);
      retval = true;
      break;
      
    case CORBA :: tk_any:
      break;
      
    case CORBA :: tk_TypeCode:
      break;
      
    case CORBA :: tk_objref:
      break;
      
    case CORBA :: tk_struct:
    case CORBA :: tk_sequence:
    case CORBA :: tk_union:
    case CORBA :: tk_array:
    case CORBA :: tk_except:
      
      break;
      
      
    }
  
  return retval;
}

void CORBA :: Any :: deletevalue (void){
  switch(tc_->kind())
    {
    case CORBA :: tk_null:
    case CORBA :: tk_void:
      break;
      
    case CORBA :: tk_short:
      delete (CORBA :: Short *) value_;
      break;
      
    case CORBA :: tk_long:
      delete (CORBA :: Long *) value_;
      break;
      
    case CORBA :: tk_ushort:
      delete (CORBA :: UShort *) value_;
      break;
      
    case CORBA :: tk_ulong:
    case CORBA :: tk_enum:
      delete (CORBA :: ULong *) value_;
      break;
      
    case CORBA :: tk_float:
      delete (CORBA :: Float *) value_;
      break;
      
    case CORBA :: tk_double:
      delete (CORBA :: Double *) value_;
      break;
      
      case CORBA :: tk_boolean:
	delete (CORBA :: Boolean *) value_;
	break;
	
    case CORBA :: tk_char:
      delete (CORBA :: Char *) value_;
      break;
      
    case CORBA :: tk_octet:
      delete (CORBA :: Octet *) value_;
      break;
      
    case CORBA :: tk_any:
      delete (CORBA :: Any *) value_;
      break;
      
    case CORBA :: tk_TypeCode:
      delete (CORBA :: TypeCode_ptr) value_;
      break;
      
    case CORBA :: tk_objref:
      delete (CORBA :: Object_ptr) value_;
      break;
      
    case CORBA :: tk_struct:
    case CORBA :: tk_sequence:
    case CORBA :: tk_union:
    case CORBA :: tk_array:
    case CORBA :: tk_except:
      
      break;
      
    case CORBA :: tk_string:
      delete (CORBA :: String *) value_;
      break;
      
      
    }
}


