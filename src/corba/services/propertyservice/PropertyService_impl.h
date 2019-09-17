// -*- c++ -*-
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

$Log: PropertyService_impl.h,v $
Revision 1.1.1.1  2002/05/13 14:30:34  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:48  bilhanan
Development

Revision 1.5  2001/08/21 16:51:21  ik
Changes to AVStreams.

Revision 1.4  2001/08/03 15:21:59  ik
PropertyService uses SLP to resolve NamingService and binds itself
there. Added SLP wrapper and CosNaming files temporarily so that
propertyd compiles.

Revision 1.3  2001/07/27 12:24:28  ik
Makefiles added for propertyservice. Some lines of DSI code there.

Revision 1.2  2001/06/08 16:45:22  bilhanan
Full Licence clause into DOORS 0.2


*/
//
// +------------------------------------------------------------------------+
// !  file:     PropertyService_impl.h
// !  date:     24.3.2000
// !  subject:  Implementation headers for DOORS PropertyService
// !  desc:     
// !  author:   ilkka karvinen, TTKK/DMI
// +------------------------------------------------------------------------+

#ifndef __PROPERTYSERVICE_IMPL_H__
#define __PROPERTYSERVICE_IMPL_H__

#include "PropertyService.h"

typedef vector<PropertyService::PropertyDef*> PSVector;

typedef vector<PropertyService::PropertyDef*>::iterator PSVectorIterator;

typedef vector<CORBA::TypeCode_ptr> PSTypeCodeVector;

typedef vector<CORBA::TypeCode_ptr>::iterator PSTypeCodeVectorIterator;

// Implementation for interface PropertySetFactory
class PropertySetFactory_impl : 
  virtual public POA_PropertyService::PropertySetFactory,
  virtual public PropertyService::PropertySetFactory
{
  public:

    PropertySetFactory_impl(PortableServer::POA_ptr _poa);

    ~PropertySetFactory_impl() { }

    PropertyService::PropertySet_ptr create_propertyset();

    PropertyService::PropertySet_ptr 
      create_constrained_propertyset(const PropertyService::PropertyTypes& 
				     allowed_property_types, 
				     const PropertyService::Properties& 
				     allowed_properties );


    PropertyService::PropertySet_ptr create_initial_propertyset( 
	       const PropertyService::Properties& initial_properties);

private:
    PortableServer::POA* _poa;

};


// Implementation for interface PropertySetDefFactory
class PropertySetDefFactory_impl : 
  virtual public POA_PropertyService::PropertySetDefFactory,
  virtual public PropertyService::PropertySetDefFactory
{
  public:

    PropertySetDefFactory_impl(PortableServer::POA_ptr poa);

    ~PropertySetDefFactory_impl() { }

    PropertyService::PropertySetDef_ptr create_propertysetdef()
      throw(
        ::CORBA::SystemException)
    ;

    PropertyService::PropertySetDef_ptr create_constrained_propertysetdef( const PropertyService::PropertyTypes& allowed_property_types, const PropertyService::PropertyDefs& allowed_property_defs )
      throw(
        ::CORBA::SystemException,
        ::PropertyService::ConstraintNotSupported)
    ;

    PropertyService::PropertySetDef_ptr create_initial_propertysetdef( const PropertyService::PropertyDefs& initial_property_defs )
      throw(
        ::CORBA::SystemException,
        ::PropertyService::MultipleExceptions)
    ;

private:
    PortableServer::POA* _poa;
};

// Implementation for interface PropertySet
class PropertySet_impl : 
  virtual public POA_PropertyService::PropertySet,
  virtual public PropertyService::PropertySet
{
  public:

    PropertySet_impl() { }

    PropertySet_impl(PortableServer::POA* poa) { _poa = poa; }

    ~PropertySet_impl() { }

    PropertySet_impl( PortableServer::POA* poa, 
                      const PropertyService::Properties& 
		      initial_properties );

    // Local invocations
    void define_property(const char* property_name, 
			 const CORBA::Any& property_value)
    throw(
	  ::CORBA::SystemException,
	  ::PropertyService::InvalidPropertyName,
	  ::PropertyService::ConflictingProperty,
	  ::PropertyService::UnsupportedTypeCode,
	  ::PropertyService::UnsupportedProperty,
	  ::PropertyService::ReadOnlyProperty)
      ;

    void define_properties( const PropertyService::Properties& nproperties )
      throw(
        ::CORBA::SystemException,
        ::PropertyService::MultipleExceptions)
    ;

    CORBA::ULong get_number_of_properties()
      throw(
        ::CORBA::SystemException)
    ;

    void get_all_property_names( CORBA::ULong how_many, 
		       PropertyService::PropertyNames_out property_names,
		       PropertyService::PropertyNamesIterator_out rest)
      throw(
        ::CORBA::SystemException)
    ;

    CORBA::Any* get_property_value( const char* property_name )
      throw(
        ::CORBA::SystemException,
        ::PropertyService::InvalidPropertyName,
        ::PropertyService::PropertyNotFound)
    ;

    CORBA::Boolean get_properties( 
             const PropertyService::PropertyNames& property_names,
	     PropertyService::Properties_out nproperties)
      throw(
        ::CORBA::SystemException)
    ;

    void get_all_properties( CORBA::ULong how_many,
			     PropertyService::Properties_out nproperties,
			     PropertyService::PropertiesIterator_out rest)
      throw(
        ::CORBA::SystemException)
    ;

    void delete_property( const char* property_name )
      throw(
        ::CORBA::SystemException,
        ::PropertyService::InvalidPropertyName,
        ::PropertyService::PropertyNotFound,
        ::PropertyService::FixedProperty)
    ;

    void delete_properties( 
                 const PropertyService::PropertyNames& property_names)
      throw(
        ::CORBA::SystemException,
        ::PropertyService::MultipleExceptions)
    ;

    CORBA::Boolean delete_all_properties()
      throw(
        ::CORBA::SystemException)
    ;

    CORBA::Boolean is_property_defined( const char* property_name )
      throw(
        ::CORBA::SystemException,
        ::PropertyService::InvalidPropertyName)
    ;

    PSVector* get_vector() { return &_properties; };
    PortableServer::POA* get_poa() { return _poa; }

protected:
  // PropertyDef is used as a hash-value for both PropertySet and
  // PropertySetDef. PropertySet just uses only property_mode=normal.
  PSVector _properties;
  PSVector _allowed_properties;
  PSTypeCodeVector _typecodes;
  PortableServer::POA* _poa;

  // Local version of define_property that supports also mode
  void _define_property(const char *property_name,
			const CORBA::Any &property_value,
			const PropertyService::PropertyModeType &mode_type = 
			  PropertyService::normal);

  CORBA::Boolean is_fixed (PSVectorIterator i);
  CORBA::Boolean is_fixed (CORBA::Long i);
  CORBA::Boolean is_readonly (PSVectorIterator i);
  CORBA::Boolean is_type_allowed (CORBA::TypeCode_ptr property_type);
  CORBA::Boolean is_property_allowed (const char *name,
                          const CORBA::Any &value,
                          const PropertyService::PropertyModeType &mode_type);
  CORBA::Long get_index (const char* property_name);
  PSVectorIterator get_iterator(const char* property_name);

};


// Implementation for interface PropertySetDef
class PropertySetDef_impl : 
  virtual public PropertySet_impl,
  virtual public POA_PropertyService::PropertySetDef,
  virtual public PropertyService::PropertySetDef
{
  public:

    PropertySetDef_impl(PortableServer::POA* poa) { _poa = poa; }

    ~PropertySetDef_impl() { }

    PropertySetDef_impl(PortableServer::POA* poa,
          const PropertyService::PropertyDefs& initial_property_defs )
      throw(
        ::CORBA::SystemException,
        ::PropertyService::MultipleExceptions )
      ; 

    void get_allowed_property_types( 
                PropertyService::PropertyTypes_out property_types )
      throw(
        ::CORBA::SystemException)
    ;

    void get_allowed_properties( 
                     PropertyService::PropertyDefs_out property_defs)
      throw(
        ::CORBA::SystemException)
    ;

    void define_property_with_mode( const char* property_name, const CORBA::Any& property_value, PropertyService::PropertyModeType property_mode )
      throw(
        ::CORBA::SystemException,
        ::PropertyService::InvalidPropertyName,
        ::PropertyService::ConflictingProperty,
        ::PropertyService::UnsupportedTypeCode,
        ::PropertyService::UnsupportedProperty,
        ::PropertyService::UnsupportedMode,
        ::PropertyService::ReadOnlyProperty)
    ;

    void define_properties_with_modes( const PropertyService::PropertyDefs& property_defs )
      throw(
        ::CORBA::SystemException,
        ::PropertyService::MultipleExceptions)
    ;

    PropertyService::PropertyModeType get_property_mode( const char* property_name )
      throw(
        ::CORBA::SystemException,
        ::PropertyService::PropertyNotFound,
        ::PropertyService::InvalidPropertyName)
    ;

    CORBA::Boolean get_property_modes( const PropertyService::PropertyNames& property_names, PropertyService::PropertyModes_out property_modes )
      throw(
        ::CORBA::SystemException)
    ;

    void set_property_mode( const char* property_name, PropertyService::PropertyModeType property_mode )
      throw(
        ::CORBA::SystemException,
        ::PropertyService::InvalidPropertyName,
        ::PropertyService::PropertyNotFound,
        ::PropertyService::UnsupportedMode)
    ;

    void set_property_modes( const PropertyService::PropertyModes& property_modes )
      throw(
        ::CORBA::SystemException,
        ::PropertyService::MultipleExceptions)
    ;

};


// Implementation for interface PropertyNamesIterator
class PropertyNamesIterator_impl : 
  virtual public POA_PropertyService::PropertyNamesIterator,
  virtual public PropertyService::PropertyNamesIterator
{
  public:

    PropertyNamesIterator_impl();

    // Create iterator for PropertyNames sequence
    // 'first' is the first in sequence (default is 0, first)
    // 'how_many' is how many properties are wanted (def is 0, all)
    PropertyNamesIterator_impl(PropertySet_impl* property_set,
			       CORBA::ULong pos = 0);

    ~PropertyNamesIterator_impl() { }

    void reset()
      throw(
        ::CORBA::SystemException)
    ;

    CORBA::Boolean next_one( CORBA::String_out property_name )
      throw(
        ::CORBA::SystemException)
    ;

    CORBA::Boolean next_n( CORBA::ULong how_many, PropertyService::PropertyNames_out property_names )
      throw(
        ::CORBA::SystemException)
    ;

    void destroy()
      throw(
        ::CORBA::SystemException)
    ;

private:
    PSVector* _properties;
    CORBA::Long _pos;
};


// Implementation for interface PropertiesIterator
class PropertiesIterator_impl : 
  virtual public POA_PropertyService::PropertiesIterator,
  virtual public PropertyService::PropertiesIterator
{
  public:

    PropertiesIterator_impl();

    PropertiesIterator_impl(PSVector *properties, 
			    CORBA::ULong pos);

    ~PropertiesIterator_impl() { }

    void reset()
      throw(
        ::CORBA::SystemException)
    ;

    CORBA::Boolean next_one( PropertyService::Property_out aproperty )
      throw(
        ::CORBA::SystemException)
    ;

    CORBA::Boolean next_n( CORBA::ULong how_many, 
			   PropertyService::Properties_out nproperties )
      throw(
        ::CORBA::SystemException)
    ;

    void destroy()
      throw(
        ::CORBA::SystemException)
    ;

private:
    PSVector* _properties;
    CORBA::Long _pos;
};


#endif
