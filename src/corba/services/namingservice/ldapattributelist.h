
//  Copyright 2001
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.

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
  LDAPAttributelist represents a list of attributes that are passed
  to the slapd in functions ldap_modify and ldap_add.
  This class provides a simple and easy to use interface to the LDAPMod structure
  and is meant to be used instead of that.
  $Log: ldapattributelist.h,v $
  Revision 1.3  2002/08/08 12:34:16  ik
  Default constructor added to make it compile with SunCC.

  Revision 1.2  2002/07/19 06:24:44  hartmanj
  Licence added

*/


#ifndef _ldapattributelist_h_
#define _ldapattributelist_h_

#include <string>
#include <vector>
#include <ldap.h>


class LDAPAttributelist {

 public:

	LDAPAttributelist();
	LDAPAttributelist( int mode );
	~LDAPAttributelist();
	LDAPAttributelist( std::string name, std::string value, int mode = LDAP_MOD_ADD );
	LDAPAttributelist( std::string name, int mode = LDAP_MOD_ADD );
	// Number of _values_ in the list
	int size() const;
	int length() const; // same thing

	// Number of individual attributes
	int numOfAttrs() const;

	// Number of values for attribute att_name
	int numOfValues( std::string &att_name );

	// Add a name-value pair to the list
	void addAttribute( std::string name,
					   std::string value );

	// Delete a specific name-value pair from
	// the attribute list.
	// Returns: number of values removed
	//  (1 if deletion successful, 0 otherwise)
	int delAttribute( std::string name,
					   std::string value );

	// Delete all attribute values with name name
	// Returns: number of values removed
	int delAttribute( std::string name );

	// Creates an LDAPMod struct
	operator LDAPMod** ( void ) const;

 private:

	// For case insensitivity
	std::string StrToLower(std::string &s);

	struct LDAPAttribute {
		LDAPAttribute() { }
		LDAPAttribute( std::string &name_, std::string value_ ) { name = name_; values.push_back(value_); }
		LDAPAttribute( std::string &name_ ) { name = name_; }
		std::string name;
		std::vector<std::string> values;
	};

	std::vector<LDAPAttribute> values_;

	int mode_; // The mode of operation.
	// Can be LDAP_MOD_[ADD|DELETE|REPLACE].

};


#endif
