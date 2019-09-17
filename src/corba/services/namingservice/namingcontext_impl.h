//  Copyright 2000 
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

$Log: namingcontext_impl.h,v $
Revision 1.4  2002/08/08 12:24:33  hartmanj
std::vector<LDAPConnection::LDAPAttribute> to LDAPAttributelist and other updates and bugfixes

Revision 1.3  2002/07/01 11:19:22  hartmanj
Development


*/


#ifndef NAMINGCONTEXT_IMPL_HH
#define NAMINGCONTEXT_IMPL_HH


#include <vector>
#include <string>

#include "ldapcon.h"
#include "cosnaming.h"


// Some configure parameters that can be set via config file or command-line
// parameters in the ns.C
typedef struct NSConfig_ {
	string bindas;
	string basedn;
	string passwd;
	string ldaphostname;
	int ldapport;
} NSConfig;


// Class NamingContext_impl implements IDL:omg.org/CosNaming/NamingContext:1.0
// Check OMG's documentation for further information.
//
// The 


class NamingContext_impl : public PortableServer::DynamicImplementation
{
 public:

    NamingContext_impl( CORBA::ORB_var orb );
    ~NamingContext_impl();

	void setConfig( NSConfig &config );
	void ConnectToLDAP(void);

    // For DSI functionality
    virtual void invoke(CORBA::ServerRequest_ptr request);
    // Also for DSI for use with interface repository
    virtual CORBA::RepositoryId _primary_interface(const PortableServer::ObjectId&, PortableServer::POA_ptr);


 private:

    // These functions implement corresponding services.
    // Only invoke can call them.
    // bind_context, rebind and destroy aren't implemented
    void bind( CORBA::ServerRequest_ptr request );
    void rebind( CORBA::ServerRequest_ptr request );
    void bind_context( const CosNaming::Name& n,CosNaming::NamingContext_ptr nc);
    void resolve( CORBA::ServerRequest_ptr request );
    void unbind( CORBA::ServerRequest_ptr request );
    void destroy();



    // sets ior and name to struct ldap_att_ to be used when inserting the
    // binding to ldap-directory
    void set_ior_and_name( CORBA::String_var ior, CosNaming::Name& name );

    // converts the cosname struct to string representation
    std::string NamingContext_impl::cosname_to_string(const CosNaming::Name& n );
    std::string NamingContext_impl::cosname_to_name(const CosNaming::Name& n );
    std::string NamingContext_impl::cosname_to_path(const CosNaming::Name& n );
    std::string NamingContext_impl::cosname_to_dn(const CosNaming::Name& n );

    // we need method strint_to_object atleast
    CORBA::ORB_var orb_;

    // For adding an object to ldap-tree
    // We set the default stuff to ldap_obj_ in constructor and just adjust
    // name and ior with ldap_att_ in each operation
    //    std::vector<LDAPConnection::LDAPAttribute> ldap_obj_;
    //    LDAPConnection::LDAPAttribute ldap_att_;
	LDAPAttributelist ldap_attrs_;

    // For searching the ldap-tree
    std::vector<std::string> attrs_;

    // LDAP specific stuff: connection handle, host parameters
    LDAPConnection *con_;
    std::string ldaphostname_;
    int ldaphostport_;
    std::string ldapusername_;
    std::string ldapuserpasswd_;
    std::string ldapbase_;
    std::string bound_context_;


};

#endif
