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

$Log: PropertyService_impl.C,v $
Revision 1.1.1.1  2002/05/13 14:30:34  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:48  bilhanan
Development

Revision 1.5  2001/08/31 15:02:15  ik
Typos in Makefile.am's of AVStreams and PropertyService. TRACE-
commands removed from PropertyService_impl.C because SunC could
not understand them like gcc.

Revision 1.4  2001/08/03 15:22:09  ik
PropertyService uses SLP to resolve NamingService and binds itself
there. Added SLP wrapper and CosNaming files temporarily so that
propertyd compiles.

Revision 1.3  2001/07/27 12:24:39  ik
Makefiles added for propertyservice. Some lines of DSI code there.

Revision 1.2  2001/06/08 16:46:42  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:48  bilhanan
Full licence clause added into doors 0.1


*/
//
// +------------------------------------------------------------------------+
// !  file:     PropertyService_impl.C
// !  date:     24.3.2000
// !  subject:  Implementation for DOORS PropertyService
// !  desc:     
// !  author:   ilkka karvinen, TTKK/DMI
// +------------------------------------------------------------------------+

#ifdef DOORS_TRACE_PROPERTYSERVICE
// For printf of DOORS_TRACE
#include <stdio.h>
#endif
#include <string.h>
#include <doors/PropertyService_impl.h>

#define DOORS_THROW_EXCEPTION(exp) mico_throw(exp)

// Implementation for interface PropertySetFactory

PropertySetFactory_impl::PropertySetFactory_impl(PortableServer::POA_ptr poa)
{
  _poa = PortableServer::POA::_duplicate (poa);
}
 
PropertyService::PropertySet_ptr
PropertySetFactory_impl::create_propertyset()
{
  PropertySet_impl* pset = new PropertySet_impl(_poa);
  PortableServer::ObjectId* oid = _poa->activate_object(pset);
  PropertyService::PropertySet_ptr ref = pset->_this();

  return PropertyService::PropertySet::_duplicate(ref);
}


PropertyService::PropertySet_ptr
PropertySetFactory_impl::create_constrained_propertyset( 
	  const PropertyService::PropertyTypes& allowed_property_types, 
	  const PropertyService::Properties& allowed_properties)
{
  DOORS_THROW_EXCEPTION(::PropertyService::ConstraintNotSupported());
  return (PropertyService::PropertySet_ptr)0;
}

PropertyService::PropertySet_ptr
PropertySetFactory_impl::create_initial_propertyset( 
            const PropertyService::Properties& initial_properties)
{
  PropertySet_impl* pset = new PropertySet_impl(_poa, initial_properties);
  PortableServer::ObjectId* oid = _poa->activate_object(pset);
  PropertyService::PropertySet_ptr ref = pset->_this();

  return PropertyService::PropertySet::_duplicate(ref);
}

// Implementation for interface PropertySetDefFactory

PropertySetDefFactory_impl::PropertySetDefFactory_impl(PortableServer::POA_ptr poa)
{
  _poa = PortableServer::POA::_duplicate (poa);
}

PropertyService::PropertySetDef_ptr
PropertySetDefFactory_impl::create_propertysetdef()
  throw(
    ::CORBA::SystemException)

{
  PropertySetDef_impl* psetdef = new PropertySetDef_impl(_poa);
  PortableServer::ObjectId* oid = _poa->activate_object(psetdef);
  PropertyService::PropertySetDef_ptr ref = psetdef->_this();

  return PropertyService::PropertySetDef::_duplicate(ref);
}


PropertyService::PropertySetDef_ptr
PropertySetDefFactory_impl::create_constrained_propertysetdef( const PropertyService::PropertyTypes& allowed_property_types, const PropertyService::PropertyDefs& allowed_property_defs )
  throw(
    ::CORBA::SystemException,
    ::PropertyService::ConstraintNotSupported)

{
  DOORS_THROW_EXCEPTION(::PropertyService::ConstraintNotSupported());
  return (PropertyService::PropertySetDef_ptr)0;
}


PropertyService::PropertySetDef_ptr
PropertySetDefFactory_impl::create_initial_propertysetdef( 
       const PropertyService::PropertyDefs& initial_property_defs )
  throw(
    ::CORBA::SystemException,
    ::PropertyService::MultipleExceptions)

{
  PropertySetDef_impl* psetdef = 
    new PropertySetDef_impl(_poa, initial_property_defs);
  PortableServer::ObjectId* oid = _poa->activate_object(psetdef);
  PropertyService::PropertySetDef_ptr ref = psetdef->_this();

  return PropertyService::PropertySetDef::_duplicate(ref);
}


// Implementation for interface PropertySet

PropertySet_impl::PropertySet_impl(PortableServer::POA* poa,
                                   const PropertyService::Properties&
				   initial_properties)
{
  _poa = poa;
  this->define_properties(initial_properties);
}

void
PropertySet_impl::define_property(const char* property_name, 
				  const CORBA::Any& property_value)
  throw(
    ::CORBA::SystemException,
    ::PropertyService::InvalidPropertyName,
    ::PropertyService::ConflictingProperty,
    ::PropertyService::UnsupportedTypeCode,
    ::PropertyService::UnsupportedProperty,
    ::PropertyService::ReadOnlyProperty)

{
  _define_property(property_name, property_value);
}


void
PropertySet_impl::define_properties( 
                      const PropertyService::Properties& nproperties )
  throw(
    ::CORBA::SystemException,
    ::PropertyService::MultipleExceptions)

{
  for (register CORBA::ULong i = 0; i < nproperties.length(); i++) {
    try {
      _define_property(nproperties[i].property_name,
		       nproperties[i].property_value);
    } catch ( ... ) {
      DOORS_THROW_EXCEPTION(::PropertyService::MultipleExceptions());
    }
  }
}


CORBA::ULong
PropertySet_impl::get_number_of_properties()
  throw(
    ::CORBA::SystemException)

{
  return _properties.size();
}


void
PropertySet_impl::get_all_property_names( CORBA::ULong how_many, 
                       PropertyService::PropertyNames_out property_names, 
                       PropertyService::PropertyNamesIterator_out rest )
  throw(
    ::CORBA::SystemException)

{

  CORBA::ULong len = get_number_of_properties ();

  property_names = new PropertyService::PropertyNames ();

  if (how_many < len) {
    property_names->length(how_many);
  } else {
    property_names->length(len);
  }

  PSVectorIterator iter = _properties.begin();
  for (register CORBA::ULong i = 0; i < property_names->length(); i++) {
    // Copy property name to sequence
    (*property_names)[i] = CORBA::string_dup(_properties[i]->property_name);
    cerr << (*property_names)[i] << endl;
  }
  
  PropertyNamesIterator_impl* pnamesiter;
  if (how_many >= len) {
    // All properties copied, just return an empty iterator
    pnamesiter = new PropertyNamesIterator_impl ();
  } else {
    // Create an iterator to rest of the names
    pnamesiter = new PropertyNamesIterator_impl(this, how_many);
  }

  // Activate and get reference to it
  PortableServer::ObjectId* oid = _poa->activate_object(pnamesiter);
  PropertyService::PropertyNamesIterator_ptr refiter = pnamesiter->_this();
  rest = PropertyService::PropertyNamesIterator::_duplicate(refiter);

}


CORBA::Any*
PropertySet_impl::get_property_value(const char* property_name)
  throw(
    ::CORBA::SystemException,
    ::PropertyService::InvalidPropertyName,
    ::PropertyService::PropertyNotFound)

{
  // Check validity
  if (strlen(property_name) == 0) {
    DOORS_THROW_EXCEPTION (PropertyService::InvalidPropertyName ());
  }

  PSVectorIterator iter = get_iterator(property_name);
  if (iter != _properties.end())
  {
    // Property found, create a new Any variable, set and return
    CORBA::Any *any = new CORBA::Any((*iter)->property_value);
 
    return any;
  } 

  // Not found, throw
  DOORS_THROW_EXCEPTION(PropertyService::PropertyNotFound());

  return new CORBA::Any;
}


CORBA::Boolean
PropertySet_impl::get_properties( 
		      const PropertyService::PropertyNames& property_names,
		      PropertyService::Properties_out nproperties)
  throw(
    ::CORBA::SystemException)

{
  CORBA::ULong len = property_names.length ();

  // Create an sequence
  nproperties = new PropertyService::Properties;

  if (len == 0) {
    return FALSE;
  }
  
  // Set sequence size
  nproperties->length(len);

  CORBA::Boolean allok = TRUE;

  for (CORBA::ULong i = 0; i < len; i++) {
    // Copy property name
    (*nproperties)[i].property_name = CORBA::string_dup(property_names[i]);

    CORBA::Any *property_value;

    try {
      // Try to get the value
      property_value = get_property_value(property_names[i]);
    } catch ( ... ) {
      property_value = new CORBA::Any;
      allok = FALSE;
    }

    // Copy value
    (*nproperties)[i].property_value = *property_value;
  }
  return allok;
}


void
PropertySet_impl::get_all_properties( CORBA::ULong how_many, 
                     PropertyService::Properties_out nproperties,
		     PropertyService::PropertiesIterator_out rest )
  throw(
    ::CORBA::SystemException)

{
  CORBA::ULong len = get_number_of_properties();

  nproperties = new PropertyService::Properties;

  if (how_many < len) {
    nproperties->length(how_many);
  } else {
    nproperties->length(len);
  }

  PSVectorIterator iter = _properties.begin();
  for (register CORBA::ULong i = 0; i < nproperties->length(); i++) {
    // Copy property name to sequence
    (*nproperties)[i].property_name = 
      CORBA::string_dup((*iter)->property_name);
    (*nproperties)[i].property_value = (*iter)->property_value;
  }
  
  if (how_many >= len) {
    // All properties copied, just return an empty iterator
    rest = new PropertiesIterator_impl ();
    return;
  }

  // Return an iterator for the rest of the properties
  rest = PropertyService::PropertiesIterator::_duplicate 
           (new PropertiesIterator_impl (&_properties, how_many));

}


void
PropertySet_impl::delete_property(const char* property_name)
  throw(
    ::CORBA::SystemException,
    ::PropertyService::InvalidPropertyName,
    ::PropertyService::PropertyNotFound,
    ::PropertyService::FixedProperty)

{
  // Null strings are not allowed
  if (strlen(property_name) == 0) {
    DOORS_THROW_EXCEPTION(::PropertyService::InvalidPropertyName());
  }
  PSVectorIterator i = get_iterator(property_name);
  if (i != _properties.end()) {
    // Check if fixed property and throw if so
    if (is_fixed(i)) {
      DOORS_THROW_EXCEPTION(PropertyService::FixedProperty ());
    }
    // Property found, erase it
    _properties.erase(i);
  } 
  // Property not found
  DOORS_THROW_EXCEPTION(::PropertyService::PropertyNotFound());
}


void
PropertySet_impl::delete_properties(const PropertyService::PropertyNames& property_names)
  throw(
    ::CORBA::SystemException,
    ::PropertyService::MultipleExceptions)

{
  CORBA::ULong len = property_names.length();
  if (len == 0) {
    return;
  }
  for (register CORBA::ULong i = 0; i < len; i++) {
    // Try to delete this property, if delete_property()
    // throws something, stop looping and return MultipleExceptions
    // (PropertyService documentation v1.0 page 13-13)
    try {
      this->delete_property(property_names[i]);
    } catch ( ... ) {
      DOORS_THROW_EXCEPTION(::PropertyService::MultipleExceptions());
      return;
    }      
  }
}


CORBA::Boolean
PropertySet_impl::delete_all_properties()
  throw(
    ::CORBA::SystemException)

{
  CORBA::ULong size = _properties.size();
  if (size == 0) {
    return TRUE;
  }

  // Search through the properties vector and if some
  // of them is fixed, don't remove it but only set allok
  // flag to FALSE for return value.
  CORBA::Boolean allok = TRUE;
  for (CORBA::Long i = size - 1; i >= 0; i--) { 
    if (!is_fixed(i)) {
      _properties.erase(_properties.begin() + i);
    } else {
      allok = FALSE;
    }
  }

  return allok;
}


CORBA::Boolean
PropertySet_impl::is_property_defined(const char* property_name)
  throw(
    ::CORBA::SystemException,
    ::PropertyService::InvalidPropertyName)

{
  if (strlen(property_name) == 0) {
    DOORS_THROW_EXCEPTION(::PropertyService::InvalidPropertyName());
  }

  PSVectorIterator i = get_iterator(property_name);
  // If iterator is pointing somewhere, property exists
  if (i != _properties.end()) {
    return TRUE;
  }

  // Not found, return FALSE
  return FALSE;
}

CORBA::Boolean
PropertySet_impl::is_readonly (PSVectorIterator i)
{
  if ((*i)->property_mode == PropertyService::read_only ||
      (*i)->property_mode == PropertyService::fixed_readonly) {
    return TRUE;
  }
  return FALSE;
}

CORBA::Boolean
PropertySet_impl::is_fixed (PSVectorIterator i)
{
  if ((*i)->property_mode == PropertyService::fixed_normal ||
      (*i)->property_mode == PropertyService::fixed_readonly) {
    return TRUE;
  }
  return FALSE;
}

CORBA::Boolean
PropertySet_impl::is_fixed (CORBA::Long i)
{
  if (_properties[i]->property_mode == PropertyService::fixed_normal ||
      _properties[i]->property_mode == PropertyService::fixed_readonly) {
    return TRUE;
  }
  return FALSE;
}

CORBA::Boolean
PropertySet_impl::is_type_allowed (CORBA::TypeCode_ptr property_type)
{
  CORBA::ULong len = _typecodes.size (); 
  
  if (len == 0) {
    return TRUE;
  }

  // Search through the vector of typecodes for a match
  for (PSTypeCodeVectorIterator i = _typecodes.begin(); 
       i != _typecodes.end(); i++)
  {
    if (property_type->equal (*i))
      return TRUE;
  }

  return FALSE;
}

CORBA::Boolean
PropertySet_impl::is_property_allowed (const char *name,
                          const CORBA::Any &value,
                          const PropertyService::PropertyModeType &mode_type)
{
  CORBA::ULong len = _allowed_properties.size ();

  if (len == 0)
    return TRUE;

  CORBA::TypeCode_ptr test_type = value.type();
  PSVectorIterator i = _allowed_properties.begin();
  for (; i != _allowed_properties.end(); i++)
  {
    // Check name
    if (strcmp (name, (*i)->property_name) == 0) {
      CORBA::TypeCode_ptr type = (*i)->property_value.type();

      // Check mode. If type is in the allowed properties or
      // one of the allowed modes is undefined, return TRUE.
      if (type->equal (value.type())) {
        if ((*i)->property_mode == mode_type ||
            (*i)->property_mode == PropertyService::undefined)
          return TRUE;
      }
    }
  }
  return FALSE;
}

PSVectorIterator 
PropertySet_impl::get_iterator(const char* property_name)
{
  PSVectorIterator i = _properties.begin();
  for ( ; i != _properties.end(); i++) {
    if (strcmp((*i)->property_name, property_name) == 0) {
      return i;
    }
  }
  return _properties.end();
}

CORBA::Long
PropertySet_impl::get_index (const char *property_name)
{
  CORBA::UShort max = get_number_of_properties ();
  for(CORBA::UShort i = 0; i < max; i++)
  {
    if (strcmp(_properties[i]->property_name, property_name) == 0) {
      // property_name found, return it
      return i;
    }
  }  
  // Not found, return invalid position
  return -1;
}

void 
PropertySet_impl::_define_property(const char *property_name,
			 const CORBA::Any &property_value,
			 const PropertyService::PropertyModeType &mode_type)
{
  if (strlen(property_name) == 0) {
    DOORS_THROW_EXCEPTION(PropertyService::InvalidPropertyName());
    return;
  }

  CORBA::TypeCode_ptr property_type = property_value.type();

  // Check if of the allowed type
  if (!is_type_allowed(property_type)) {
    DOORS_THROW_EXCEPTION(PropertyService::UnsupportedTypeCode());
  }

  // Check if the property is allowed
  if (!is_property_allowed(property_name, property_value, mode_type)) {
    DOORS_THROW_EXCEPTION(PropertyService::UnsupportedProperty());
  }

  PSVectorIterator iter = get_iterator(property_name);
  if (iter != _properties.end()) {
    // Property already exists

    // If property is readonly, throw
    if (is_readonly(iter)) {
      DOORS_THROW_EXCEPTION(PropertyService::ReadOnlyProperty ());
    }

    // Check if the old new types conflict
    CORBA::TypeCode_ptr old_type = (*iter)->property_value.type();
    if (!(old_type->equal(property_type))) {
      DOORS_THROW_EXCEPTION(PropertyService::ConflictingProperty ());
    }

    // Replace the old value
    (*iter)->property_value = property_value;
  } else {
    // Insert a new property
    PropertyService::PropertyDef* new_property =
      new PropertyService::PropertyDef();
    new_property->property_name  = CORBA::string_dup (property_name);
    new_property->property_value = property_value;
    new_property->property_mode  = mode_type;
    // Put the new property to the of the property vector
    char* str = new_property->property_name;
    _properties.push_back (new_property);
#ifdef DOORS_TRACE_PROPERTYSERVICE
    cerr << "Properties now: ";
    cerr << _properties.size() << endl;
#if 0
    PSVectorIterator iter;
    for (iter = _properties.begin(); iter != _properties.end(); iter++) {
      cerr << "\"" << (*iter)->property_name << "\" = ";
      CORBA::Long longval;
      CORBA::ULong ulongval;
      CORBA::String_var strval;
      CORBA::Short shortval;
      CORBA::UShort ushortval;
      CORBA::Boolean boolval;
      CORBA::Float floatval;
      CORBA::Double doubleval;
      switch((*iter)->property_value.type()->kind()) {
      case CORBA::tk_boolean :
	(*iter)->property_value >>= CORBA::Any::to_boolean(boolval);
	cerr << "(Boolean)" << (boolval?"TRUE":"FALSE") << endl;
	break;
      case CORBA::tk_double :
	(*iter)->property_value >>= doubleval;
	cerr << "(Double)" << doubleval << endl;
	break;
      case CORBA::tk_float :
	(*iter)->property_value >>= floatval;
	cerr << "(Float)" << floatval << endl;
	break;
      case CORBA::tk_short :
	(*iter)->property_value >>= shortval;
	cerr << "(Short)" << shortval << endl;
	break;
      case CORBA::tk_ushort :
	(*iter)->property_value >>= ushortval;
	cerr << "(UShort)" << ushortval << endl;
	break;
      case CORBA::tk_long :
	(*iter)->property_value >>= longval;
	cerr << "(Long)" << longval << endl;
	break;
      case CORBA::tk_ulong :
	(*iter)->property_value >>= ulongval;
	cerr << "(ULong)" << ulongval << endl;
	break;
      case CORBA::tk_string :
	(*iter)->property_value >>= strval;
	cerr << "(String)\"" << strval << "\"" << endl;
	break;
      default :
	cerr << "Unknown type number " << (*iter)->property_value.type()->kind() << endl;
	break;
      }
    }
#endif
#endif
  }
}


// Implementation for interface PropertySetDef

PropertySetDef_impl::PropertySetDef_impl(PortableServer::POA* poa,
       const PropertyService::PropertyDefs& initial_property_defs)
  throw(
        ::CORBA::SystemException,
        ::PropertyService::MultipleExceptions )
{
  _poa = poa;
  try {
    this->define_properties_with_modes(initial_property_defs);
  } catch ( ... ) {
    DOORS_THROW_EXCEPTION(::PropertyService::MultipleExceptions());
  }
}

void
PropertySetDef_impl::get_allowed_property_types( 
                      PropertyService::PropertyTypes_out property_types)
  throw(
    ::CORBA::SystemException)

{
  CORBA::ULong len = _typecodes.size();

  property_types = new PropertyService::PropertyTypes;
  property_types->length (len);

  for (register CORBA::ULong i = 0; i < len; i++) {
    (*property_types)[i] = _typecodes[i];
  }
}


void
PropertySetDef_impl::get_allowed_properties( PropertyService::PropertyDefs_out property_defs )
  throw(
    ::CORBA::SystemException)

{
  CORBA::ULong len = _allowed_properties.size();

  for (CORBA::ULong i = 0; i < len; i++) {
    (*property_defs) [i].property_name = 
      _allowed_properties[i]->property_name;

    (*property_defs) [i].property_value = 
      _allowed_properties[i]->property_value;

    (*property_defs) [i].property_mode = 
      _allowed_properties[i]->property_mode;
  }
    
}


void
PropertySetDef_impl::define_property_with_mode( const char* property_name, 
        const CORBA::Any& property_value, 
        PropertyService::PropertyModeType property_mode)
  throw(
    ::CORBA::SystemException,
    ::PropertyService::InvalidPropertyName,
    ::PropertyService::ConflictingProperty,
    ::PropertyService::UnsupportedTypeCode,
    ::PropertyService::UnsupportedProperty,
    ::PropertyService::UnsupportedMode,
    ::PropertyService::ReadOnlyProperty)

{
  // Zero length name is not allowed
  if (strlen(property_name) == 0) {
    DOORS_THROW_EXCEPTION(::PropertyService::InvalidPropertyName());
  }

  _define_property(property_name, property_value, property_mode);
}


void
PropertySetDef_impl::define_properties_with_modes( 
             const PropertyService::PropertyDefs& property_defs )
  throw(
    ::CORBA::SystemException,
    ::PropertyService::MultipleExceptions)

{
  for (register CORBA::ULong i = 0; i < property_defs.length(); i++) {
    try {
      this->define_property_with_mode(property_defs[i].property_name,
				      property_defs[i].property_value,
				      property_defs[i].property_mode);
    } catch ( ... ) {
      DOORS_THROW_EXCEPTION(::PropertyService::MultipleExceptions());
    }
  }
}


PropertyService::PropertyModeType
PropertySetDef_impl::get_property_mode(const char* property_name)
  throw(
    ::CORBA::SystemException,
    ::PropertyService::PropertyNotFound,
    ::PropertyService::InvalidPropertyName)

{
  PSVectorIterator i = get_iterator(property_name);
  if (i != _properties.end()) {
    return (*i)->property_mode;
  }

  DOORS_THROW_EXCEPTION(::PropertyService::PropertyNotFound());
  return PropertyService::undefined;
}


CORBA::Boolean
PropertySetDef_impl::get_property_modes( 
        const PropertyService::PropertyNames& property_names, 
	PropertyService::PropertyModes_out property_modes )
  throw(
    ::CORBA::SystemException)

{

  // Zero length name sequence is invalid
  CORBA::ULong len = property_names.length();
  if (len == 0) {
    DOORS_THROW_EXCEPTION(::CORBA::SystemException());
    return FALSE;
  }

  // Create new PropertyModes sequence with the size of
  // number of requested properties.
  property_modes = new PropertyService::PropertyModes;
  property_modes->length(len);

  // Go through the properties and save PropertyMode to the
  // sequence. If some property is not found, it is flagged
  // as undefined and this method returns FALSE.
  CORBA::Boolean allfound = TRUE;
  for (register CORBA::ULong i = 0; i < len; i++) {
    // Find property
    PSVectorIterator iter = get_iterator(property_names[i]);
    // If property is found, save the mode
    if (iter != _properties.end()) {
      PropertyService::PropertyMode mode = { "", (*iter)->property_mode };
      property_modes[i] = mode;
    } else {
      // If property is not found, set mode as undefined
      PropertyService::PropertyMode mode = { "", PropertyService::undefined };
      property_modes[i] = mode;
      allfound = FALSE;
    }      
  }

  return allfound;
}


void
PropertySetDef_impl::set_property_mode(const char* property_name, 
                        PropertyService::PropertyModeType property_mode)
  throw(
    ::CORBA::SystemException,
    ::PropertyService::InvalidPropertyName,
    ::PropertyService::PropertyNotFound,
    ::PropertyService::UnsupportedMode)

{
  // If zero length name, throw
  if (!strlen(property_name)) {
    DOORS_THROW_EXCEPTION(::PropertyService::InvalidPropertyName());
    return;
  }

  if (property_mode == PropertyService::undefined) {
    DOORS_THROW_EXCEPTION(::PropertyService::UnsupportedMode());
  }
    
  // Find property
  PSVectorIterator i = get_iterator(property_name);

  // If not found, throw
  if (i == _properties.end()) {
    DOORS_THROW_EXCEPTION(::PropertyService::PropertyNotFound());
    return;
  }

  // Check if property is allowed 
  if (!is_property_allowed(property_name, 
			   (*i)->property_value,
			   (*i)->property_mode)) {
    DOORS_THROW_EXCEPTION(::PropertyService::UnsupportedMode());
  }

  // Set mode
  (*i)->property_mode = property_mode;
}


void
PropertySetDef_impl::set_property_modes(
		   const PropertyService::PropertyModes& property_modes)
  throw(
	::CORBA::SystemException,
	::PropertyService::MultipleExceptions)

{
  CORBA::ULong len = property_modes.length();
  if (len == 0) {
    return;
  }
  
  CORBA::Boolean allok = TRUE;
  for (register CORBA::ULong i = 0; i < len; i++) {
    try {
      set_property_mode(property_modes[i].property_name,
			property_modes[i].property_mode);
    } catch ( ... ) {
      allok = FALSE;
    }
  }

  if (!allok) {
    DOORS_THROW_EXCEPTION(::PropertyService::MultipleExceptions());
  }

}


// Implementation for interface PropertyNamesIterator

PropertyNamesIterator_impl::PropertyNamesIterator_impl()
{
  _properties = (PSVector*)0;
  _pos = -1;
}

PropertyNamesIterator_impl::PropertyNamesIterator_impl(
	       PropertySet_impl* property_set, CORBA::ULong pos)
{
  // If properties is not set, just set _properties to
  // NULL and return
  if (!property_set) {
    _properties = (PSVector*)0;
    _pos = -1;
    return;
  }
  _properties = property_set->get_vector();
  if (!_properties) {
    // TODO: Handle this
  }

  // Reset pointer
  _pos = pos;

}

void
PropertyNamesIterator_impl::reset()
  throw(
    ::CORBA::SystemException)

{
  if (_properties) {
    _pos = 0;
  }
}


CORBA::Boolean
PropertyNamesIterator_impl::next_one( CORBA::String_out property_name )
  throw(
    ::CORBA::SystemException)

{
  // Check if no properties or in the end of sequence
  if (_pos < 0) {
    property_name = CORBA::string_dup("");
    return FALSE;
  }
  if ((_pos < 0) || (_pos >= _properties->size())) {
    property_name = CORBA::string_dup("");
    return FALSE;
  }
  
  // Copy property name, it is the 'first' in vector
  property_name = 
    CORBA::string_dup((*_properties)[_pos]->property_name);
  
  // Add one to position
  _pos++;

  return TRUE;
}


CORBA::Boolean
PropertyNamesIterator_impl::next_n( CORBA::ULong how_many, 
                      PropertyService::PropertyNames_out property_names )
  throw(
    ::CORBA::SystemException)

{
  // Check if no properties or in the end of sequence
  if (!_properties) {
    return FALSE;
  }
  CORBA::ULong len = _properties->size();
  if ((_pos < 0) || (_pos >= len) || (how_many == 0)) {
    property_names = new PropertyService::PropertyNames();
    return FALSE;
  }

  // Create a new PropertyNames sequence
  property_names = new PropertyService::PropertyNames ();

  // Check how_many. If it runs beyond the length, reduce it
  if ((_pos + how_many) > len) {
    how_many = len - _pos;
  }

  property_names->length(how_many);

  for (register CORBA::ULong i = 0; i < how_many; i++) {
    (*property_names)[i] = 
      CORBA::string_dup((*_properties)[i + _pos]->property_name);
  }

  // Advance position
  _pos += how_many;

  return TRUE;
}


void
PropertyNamesIterator_impl::destroy()
  throw(
    ::CORBA::SystemException)

{
  _properties = (PSVector*)0;
  _pos = -1;
  CORBA::release(this);
}


// Implementation for interface PropertiesIterator

PropertiesIterator_impl::PropertiesIterator_impl()
{
  _properties = (PSVector*)0;
  _pos = -1;
}

PropertiesIterator_impl::PropertiesIterator_impl(
				      PSVector* properties,
				      CORBA::ULong pos)
{
  // If properties is not set, just set _properties to
  // NULL and return
  if (!properties) {
    _properties = (PSVector*)0;
    _pos = -1;
    return;
  }
  _properties = properties;
}

void
PropertiesIterator_impl::reset()
  throw(
    ::CORBA::SystemException)

{
  if (_properties) {
    _pos = 0;
  }
}


CORBA::Boolean
PropertiesIterator_impl::next_one( PropertyService::Property_out aproperty )
  throw(
    ::CORBA::SystemException)

{
  // Check if no properties or in the end of sequence
  if (!_properties) {
    return FALSE;
  }

  // Create a new property 
  aproperty = new PropertyService::Property;
  if ((_pos < 0) || (_pos >= _properties->size())) {
    return FALSE;
  }
  
  // Copy property 
  aproperty->property_name = 
    CORBA::string_dup((*_properties)[_pos]->property_name);
  aproperty->property_value = (*_properties)[_pos]->property_value;

  // Add one to position
  _pos++;

  return TRUE;
}


CORBA::Boolean
PropertiesIterator_impl::next_n(CORBA::ULong how_many, 
				PropertyService::Properties_out nproperties)
  throw(
    ::CORBA::SystemException)

{
  // Check if no properties or in the end of sequence
  if (!_properties) {
    return FALSE;
  }
  CORBA::ULong len = _properties->size();
  nproperties = new PropertyService::Properties();
  if ((_pos < 0) || (_pos >= len) || (how_many == 0)) {
    return FALSE;
  }

  // Check how_many. If it runs beyond the length, reduce it
  if ((_pos + how_many) > len) {
    how_many = len - _pos;
  }

  nproperties->length(how_many);

  for (register CORBA::ULong i = 0; i < how_many; i++) {
    (*nproperties)[i].property_name = 
      CORBA::string_dup((*_properties)[i + _pos]->property_name);
    (*nproperties)[i].property_value = 
      (*_properties)[i + _pos]->property_value;
  }

  // Advance position
  _pos += how_many;

  return TRUE;
}

void
PropertiesIterator_impl::destroy()
  throw(
    ::CORBA::SystemException)

{
  _properties = (PSVector*)0;
  _pos = -1;
  CORBA::release(this);
}


