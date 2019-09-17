//Editor-Info: -*- C++ -*-

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



// Declarations for class LDAPConnection.
// Implementation in ldapcon.cc

#ifndef LDAPCON_HH
#define LDAPCON_HH

#include <string>
#include <vector>
#include <lber.h>
#include <ldap.h>
#include "ldapattributelist.h"


class LDAPConnection {

public :

  // Default port number
  static const int oma_default_port;

  // Structure used with add_entry
/*
  struct LDAPAttribute {
	  std::string name;
	  std::string value;
  };
*/

  // Constructor and destructor
  LDAPConnection( std::string host, int port );
  LDAPConnection(); // sets all to zero
  ~LDAPConnection();


  // Class methods

  // Performs a simple bind operation to the server
  int bind( std::string dn, std::string passwd );

  // Breaks the connection and tells the server to free the memory
  // for this session
  void unbind(void);

  // Searches for dn:s passing through filter within the scope
  // and only picks attrs-attributes(empty vector selects all).
  // If types_only equals true, only attribute types are grabbed.
  int search(std::string base,int scope,std::string filter,
	     std::vector<std::string> &attrs,bool types_only);

  // Determines whether dn includes an attribute att with value value
  int compare(std::string dn, std::string att, std::string value) const;

  // Replaces attribute att from dn with value newvalue
  int replace(std::string dn, const std::string &att, const std::string &newvalue );

  // Changes the dn of olddn to newdn
  int modify_dn(std::string olddn, std::string newdn);

  // Adds an entry with dn to the tree. attributes are in attrs
  int add_entry(std::string dn, LDAPAttributelist &attrs);

  // First deletes the whole entry, second deletes whole attribute
  // from dn and the third deletes just a single value of an attribute
  // specified by value.
  int del(std::string dn);
  int del(std::string dn, std::string att);
  int del(std::string dn, std::string att, std::string value);

  // Whether theres a connection. (more specifically whether connection
  // has been defined, but possibly no communication to the server
  // exists. See man ldap_init)
  bool isOpen(void) const;

  // Initialize the object and prepare for communications to the server.
  // No actual communication to the server. (See man ldap_init)
  bool initCon(std::string host, int port);

  // Get the first entry of search result. Returns false if not successfull.
  bool first_entry(void);
  // Get the next entry of search result. Returns false if not successfull.
  bool next_entry(void);

  // Retrieves the first attribute from search result from current entry.
  std::string first_attribute(void);
  // Retrieves the next attribute from search result. "" returned if failed.
  std::string next_attribute(void);

  // Retrieves first value of current attribute of current entry.
  // "" if failed(no more left).
  std::string first_value(void);
  std::string next_value(void);

  // Retrieves value of offset offset and returns it.
  std::string value( unsigned int offset ) const;

  // Returns the dn of the current entry of search result.
  std::string get_dn(void) const;

	void Error( std::string operation );

private :
  
  LDAP *con_; // Connection handle
  LDAPMessage *result_; // Result set
  LDAPMessage *entry_;  // Entry set
  BerElement *ptr_; // Pointer for entry browsing
  std::string att_; // Attribute string
  char **values_; // Attribute values
  unsigned int value_offset_;
  unsigned int values_size_;
};


#endif
