
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

  $Log: ldapattributelist.C,v $
  Revision 1.3  2002/08/09 09:39:43  ik
  Default argument value removed from implementation.

  Revision 1.2  2002/07/19 06:24:44  hartmanj
  Licence added

*/


#include <ldap.h>
#include <string>
#include <vector>
#include "ldapattributelist.h"

LDAPAttributelist::LDAPAttributelist()
{
	mode_ = LDAP_MOD_ADD;
}

LDAPAttributelist::LDAPAttributelist( int mode ) : mode_(mode)
{
	if( mode != LDAP_MOD_ADD && mode != LDAP_MOD_DELETE && mode != LDAP_MOD_REPLACE ) {
		mode = LDAP_MOD_ADD;
	}
}

LDAPAttributelist::~LDAPAttributelist()
{
}

LDAPAttributelist::LDAPAttributelist( std::string name, std::string value, int mode ) : mode_(mode)
{
	values_.push_back( LDAPAttribute(name,value) );
}

LDAPAttributelist::LDAPAttributelist( std::string name, int mode )
{
	values_.push_back( LDAPAttribute(name) );
}

int LDAPAttributelist::size() const
{
	int i=0, result=0;
	while(i<values_.size()) {
		result += values_[i].values.size();
		i++;
	}

	return result;
}


int LDAPAttributelist::length() const
{
	return this->size();
}

int LDAPAttributelist::numOfValues( std::string &att_name )
{
	int i=0, result=0;
	while(i<values_.size()) {
		if(StrToLower(values_[i].name) == StrToLower(att_name)) {
			result++;
		}
		i++;
	}

	return result;
}

int LDAPAttributelist::numOfAttrs() const
{
	return values_.size();
}

void LDAPAttributelist::addAttribute(std::string name, std::string value )
{
	int i=0;
	while( i < values_.size() ) {
		if(StrToLower(values_[i].name) == StrToLower(name)) {
			values_[i].values.push_back(value);
			return;
		}
		i++;
	}

	values_.push_back( LDAPAttribute(name,value) );
}

int LDAPAttributelist::delAttribute( std::string name, std::string value )
{
	std::vector<LDAPAttribute>::iterator iter = values_.begin();

	while( iter != values_.end() ) {
		if( StrToLower(name) == StrToLower((*iter).name) ) {
			std::vector<std::string>::iterator iter2 = (*iter).values.begin();
			while( iter2 != (*iter).values.end() ) {
				if( StrToLower(value) == StrToLower((*iter2)) ) {
					(*iter).values.erase(iter2);
					return 1;
				}
			}

			return 0;
		}
		iter++;
	}


	return 0;
}

int LDAPAttributelist::delAttribute( std::string name )
{
	std::vector<LDAPAttribute>::iterator iter = values_.begin();

	while( iter != values_.end() ) {
		if( StrToLower(name) == StrToLower((*iter).name) ) {
			int values=(*iter).values.size();
			values_.erase(iter);
			return values;
		}
		iter++;
	}

	return 0;
}


// Memory is reserved with calloc so that free_mods_list() can be called.
// This is perfectly legal but a bit ugly.
LDAPAttributelist::operator LDAPMod**(void) const
{
	LDAPMod **temp = (LDAPMod**)calloc(numOfAttrs()+1, sizeof(LDAPMod*));

	// Copy names and values to LDAPMod
	for(unsigned int i=0; i < numOfAttrs(); i++ ) {
		temp[i] = (LDAPMod*)calloc(1, sizeof(LDAPMod));
		temp[i]->mod_op = mode_;
		temp[i]->mod_type = (char*)calloc(values_[i].name.size(), 1);
		temp[i]->mod_bvalues = NULL;

		strcpy( temp[i]->mod_type, values_[i].name.c_str() );

		// Use calloc, so if LDAP_MOD_DELETE is used and the entire attribute is to be
		// deleted, then LDAPMod.mod_type==NULL
		temp[i]->mod_values = (char**)calloc(values_[i].values.size()+1, sizeof(char*));

		// Copy values of attribute to char**
		for(unsigned int ii=0; ii < values_[i].values.size(); ii++) {
			temp[i]->mod_values[ii] = (char*)calloc(values_[i].values[ii].size(), 1);
			strcpy( temp[i]->mod_values[ii], values_[i].values[ii].c_str() );
		}

		// null-terminate the array, just to be sure
		temp[i]->mod_values[values_[i].values.size()] = NULL;
	}

	// null-terminate the array, just to be sure
	temp[numOfAttrs()] = NULL;

	return temp;
}

std::string LDAPAttributelist::StrToLower( std::string &s )
{
        std::string::iterator i = s.begin();
        std::string temp;

        while(i != s.end()) {
                temp += tolower(*i);
                i++;
        }

        return temp;
}
