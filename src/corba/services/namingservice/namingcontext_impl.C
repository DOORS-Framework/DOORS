
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

$Log: namingcontext_impl.C,v $
Revision 1.4  2002/08/08 12:24:31  hartmanj
std::vector<LDAPConnection::LDAPAttribute> to LDAPAttributelist and other updates and bugfixes

Revision 1.3  2002/07/01 11:19:22  hartmanj
Development


*/

#include <string>
#include <vector>
#include <cstdlib>

#include "namingcontext_impl.h"
#include "slpwrap.h"
#include "ldapcon.h"
#include "cosnaming.h"


std::string ldaphost = "";
int ldapport = 389;
std::string ldapuser = "";
std::string ldappasswd = "";
std::string ldapbase = "";


void NamingContext_impl::setConfig( NSConfig &config )
{
	ldapusername_ = config.bindas;
	ldapuserpasswd_ = config.passwd;
	ldaphostname_ = config.ldaphostname;
	ldaphostport_ = config.ldapport;
	ldapbase_ = config.basedn;
	bound_context_ = ldapbase_;
}


void NamingContext_impl::invoke(CORBA::ServerRequest_ptr request)
{
    std::cout <<  request->op_name() << " requested." << std::endl;
    try {
		// If the shoe fits
		if( !strcmp(request->op_name(), "bind") )
			bind(request);
		else if(!strcmp(request->op_name(), "resolve") )
			resolve(request);
		else if(!strcmp(request->op_name(), "unbind") )
			unbind(request);
		else if(!strcmp(request->op_name(), "rebind") )
			rebind(request);
		else if(!strcmp(request->op_name(), "destroy") )
			std::cout << "Fix me, add some code here for destroy!" << std::endl;
    }
    catch(CosNaming::NamingContext::NotFound &ex) {
		std::cout << "EXCEPTION NotFound! Passing on!" << std::endl;
		CORBA::Any exc;
		exc <<= ex;
        request->set_exception(exc);
    }
    catch(CosNaming::NamingContext::AlreadyBound &ex) {
		std::cout << "EXCEPTION AlreadyBound! Passing on!" << std::endl;
		CORBA::Any exc;
		exc <<= ex;
        request->set_exception(exc);
    }
    catch(CosNaming::NamingContext::InvalidName &ex) {
      	std::cout << "EXCEPTION InvalidName! Passing on!" << std::endl;
		CORBA::Any exc;
		exc <<= ex;
        request->set_exception(exc);
    }
    catch(CosNaming::NamingContext::CannotProceed &ex) {
		std::cout << "EXCEPTION CannotProceed! Passing on!" << std::endl;
		CORBA::Any exc;
		exc <<= ex;
        request->set_exception(exc);
    }
    catch(...) {   // Some weird exp caught
		std::cout << "NamingContext_impl::invoke() caught an unknown "
				  << "exception!" << std::endl;

		CORBA::Any exc;
		CORBA::UserException ex;
		exc <<= ex;
		request->set_exception(exc);
    }
}

CORBA::RepositoryId NamingContext_impl::_primary_interface(const PortableServer::ObjectId&, PortableServer::POA_ptr)
{
    return "IDL:omg.org/CosNaming/NamingContext:1.0";
}

void NamingContext_impl::ConnectToLDAP(void)
{
    // If no port was defined in SLP
    if( ldaphostport_ == 0 )
		ldaphostport_ = 389;


    // Open con to the ldap-server

    if((con_ = new LDAPConnection( ldaphostname_, ldaphostport_ )) != NULL)
    {
		// Bind to the LDAP server
		int res;
		if((res = con_->bind( ldapusername_, ldapuserpasswd_ )) != 0 )
		{
			con_->Error("ldap_bind");
			std::cerr << "Couldn't bind to server. " << res << std::endl;
		}
	} else {
		std::cerr << "Couldn't initialize a connection to server." << std::endl;
	}       
}

NamingContext_impl::NamingContext_impl( CORBA::ORB_var orb )
    : con_(0), ldaphostname_(ldaphost), ldaphostport_(ldapport),
      ldapusername_(ldapuser),ldapuserpasswd_(ldappasswd), ldapbase_(ldapbase),
      bound_context_(ldapbase)
{
    orb_ = orb->_duplicate(orb);

    // Resolve where the ldap-server is
    // We'll use SLP first and if that fails, use defaults
    /*    std::string ldap="ldap";
		  if( !FindServiceHost(ldap, ldaphostname_, ldaphostport_) )
		  {
		  ldaphostname_ = ldaphost;
		  ldaphostport_ = ldapport;
		  }
    */

    // Announce yourself somewhere(slp)?
    // Check if other namingcontexts are serving?

	ldap_attrs_.addAttribute("objectClass", "corbaObjectReference");
	//ldap_attrs_.addAttribute("objectClass", "corbaObject");
	ldap_attrs_.addAttribute("objectClass","top");
	ldap_attrs_.addAttribute("objectClass","corbaContainer");
	ldap_attrs_.addAttribute("description","CORBA object bound with DOORS NS");
}

NamingContext_impl::~NamingContext_impl()
{
    std::cout << "Someone just destroyed me..." << std::endl;
}

void NamingContext_impl::bind( CORBA::ServerRequest_ptr request )
{
    // To be used with actual binding-stuff
    CosNaming::Name n;
    CORBA::Object_ptr obj;

    // This much shit just to get params
    CORBA::NVList_ptr args;
    orb_->create_list(0, args);
    CORBA::Any a,b;
    a.replace(CosNaming::_tc_Name, 0);
    b.replace(CORBA::_tc_Object, 0);
    std::cout << "NamingContext_impl::bind initiated" << std::endl;
    args->add_value("n", a, CORBA::ARG_IN);
    args->add_value("obj", b, CORBA::ARG_IN);
    
    // We want the params of the call!
    request->params(args);
    
    
    CORBA::Any& input1_any = *(args->item(0)->value());
    CORBA::Any& input2_any = *(args->item(1)->value());
    
    // Convert the any params to correct types
    input1_any >>= n;
    input2_any >>= obj;

    // Process the object to string
    CORBA::String_var ref = orb_->object_to_string( obj );


    // Add the ior to ldap-tree in appropriate place
    // with name n. Only cn is set to the simple name of the object.
    // The compound name exists in the path of the object.
    //set_ior_and_name(ref,n);
	LDAPAttributelist temp;
	temp = ldap_attrs_; // Hope that the default copy constructor works
	temp.addAttribute( (std::string)n[n.length()-1].kind, (std::string)n[n.length()-1].id );
	temp.addAttribute( "corbaIor", (std::string)ref );

    std::string dn = cosname_to_dn(n) + "," + ldapbase_;

	std::cout << "NS::bind: " << dn << std::endl;

    int v;
    if( (v=con_->add_entry(dn, temp ) ) != 0 ) {
		std::cout << "Addition failed... "; std::cout.flush();
		con_->Error("ldap_add");
	}

    // An object with such name already exists -> throw AlreadyBound
    if( v == LDAP_ALREADY_EXISTS )
		throw CosNaming::NamingContext::AlreadyBound();

    if( v == LDAP_NO_SUCH_OBJECT )
		throw CosNaming::NamingContext::NotFound();

    if( v != 0 )
		throw CosNaming::NamingContext::CannotProceed();

    std::cout << "NamingContext_impl::bind end" << std::endl;
}

// You have to put current context after this function to get the whole dn
std::string NamingContext_impl::cosname_to_dn(const CosNaming::Name& name )
{
    std::string dn = "";

    // Make a string like "id[n].kind=id[n].id,...,id[0].kind=id[0].id"
    for(unsigned int i=0; i<name.length(); i++)
    {
		dn = static_cast<std::string>(name[i].kind) + "=" +
			static_cast<std::string>(name[i].id) + dn;
		if( i < name.length()-1 )
			dn = ',' + dn;
	
    }

    return dn;
}

// Sets ior and name to ldap-addition vector
void NamingContext_impl::set_ior_and_name(CORBA::String_var ior, CosNaming::Name& name)
{
    // Now, this is _very_ ugly..
/*
    ldap_obj_.pop_back();
    ldap_obj_.pop_back();

    ldap_att_.name = name[name.length()-1].kind;
    ldap_att_.value = name[name.length()-1].id;
    ldap_obj_.push_back(ldap_att_);

    ldap_att_.name = "corbaIor";
    ldap_att_.value = ior;
    ldap_obj_.push_back(ldap_att_);
*/
}

void NamingContext_impl::resolve( CORBA::ServerRequest_ptr request )
{
    // To be used with actual binding-stuff
    CosNaming::Name n;

    // This much shit just to get params
    CORBA::NVList_ptr args;
    orb_->create_list(0, args);
    CORBA::Any a;
    a.replace(CosNaming::_tc_Name, 0);
    args->add_value("n", a, CORBA::ARG_IN);
    
    // We want the param of the call!
    request->params(args);
    
    CORBA::Any& input1_any = *(args->item(0)->value());
    
    // Convert the any param to CN::Name
    input1_any >>= n;

    // Reverse-engineer the Name to string
    std::string dn = cosname_to_dn(n);

    // Filter in all corba-objects
    std::string filt = "objectClass=corbaObjectReference";
    std::cout << "dn: " << dn << "," << bound_context_ << std::endl;
    int res;
    if((res=con_->search( dn + "," + bound_context_,
						  LDAP_SCOPE_BASE,filt, attrs_, false)) != 0 )
    {
		std::cout << "Search failed..." << std::endl;
		throw CosNaming::NamingContext::CannotProceed();
    }



    // Check for results
    if( !con_->first_entry() )
    {
        std::cout << "None were found.." << std::endl;
		throw CosNaming::NamingContext::NotFound();
    }
    
    // We did find something
    // OK, let's just take the first one
    CORBA::String_var string_ref = "";

    // Step through attributes till corbaIor is found
    for(std::string att = con_->first_attribute(); att!="";
		att = con_->next_attribute())
		if( att == "corbaIor" )
			string_ref = con_->first_value().c_str();

    //    std::cout << string_ref << std::endl;
    
    // If ior found -> string_to_object
    if( string_ref != CORBA::string_dup("") )
    {
		// Stick the obj_ptr to request::return_val
		CORBA::Object_ptr obj = orb_->string_to_object(string_ref);
		CORBA::Any any_obj;
		any_obj <<= obj;
		request->set_result(any_obj);
		std::cout << "resolve exiting normally" << std::endl;
		return;
    }

    // Else raise some exception(notfound, invalidname..)
    // This is always a good one to throw :)
    throw CosNaming::NamingContext::CannotProceed();
}

std::string NamingContext_impl::cosname_to_string( const CosNaming::Name& n )
{
    std::string name;
    for(unsigned int i=0;i<n.length(); i++)
    {
        name += n[i].id;
        if( strcmp(n[i].kind,"") )
            name += "." + static_cast<std::string>(n[i].kind);

        if( i < n.length() - 1 )
            name += "/";
    }

    return name;
}

std::string NamingContext_impl::cosname_to_name( const CosNaming::Name& n )
{
    std::string name =  static_cast<std::string>(n[n.length()-1].kind) + "=" +
		static_cast<std::string>(n[n.length()-1].id);
    return name;
}

std::string NamingContext_impl::cosname_to_path( const CosNaming::Name& n )
{
    std::string name;

    for(unsigned int i=0;i<n.length()-1; i++)
    {
        name += n[i].kind;
		name += "=" + static_cast<std::string>(n[i].id);

        if(i < n.length() - 2)
            name += ",";
    }

    return name;
}

void NamingContext_impl::unbind( CORBA::ServerRequest_ptr request )
{
    // To be used with actual binding-stuff
    CosNaming::Name n;

    // This much shit just to get params
    CORBA::NVList_ptr args;
    orb_->create_list(0, args);
    CORBA::Any a;
    a.replace(CosNaming::_tc_Name, 0);
    args->add_value("n", a, CORBA::ARG_IN);
    
    // We want the param of the call!
    request->params(args);
    
    CORBA::Any& input1_any = *(args->item(0)->value());
    
    // Convert the any param to CN::Name
    input1_any >>= n;

    // corba-name to ldap-entry
    std::string dn = cosname_to_dn(n) + "," + ldapbase_;

    int res;
    if(( res = con_->del(dn)) != 0 ) {
		std::cout << "No can do unbind of " << dn << ". Code " << res << std::endl;
		con_->Error("ldap_delete");
	}

    if( res == LDAP_NO_SUCH_OBJECT )
		throw CosNaming::NamingContext::NotFound();
}

void NamingContext_impl::destroy()
{
    // Still a bit undone
}

void NamingContext_impl::rebind( CORBA::ServerRequest_ptr request )
{
    std::cout << "NamingContext_impl::rebind() initiated" << std::endl;
    // To be used with actual binding-stuff
    CosNaming::Name n;
    CORBA::Object_ptr obj;

    // This much shit just to get params
    CORBA::NVList_ptr args;
    orb_->create_list(0, args);
    CORBA::Any a,b;
    a.replace(CosNaming::_tc_Name, 0);
    b.replace(CORBA::_tc_Object, 0);
    args->add_value("n", a, CORBA::ARG_IN);
    args->add_value("obj", b, CORBA::ARG_IN);

    // We want the params of the call!
    request->params(args);

    CORBA::Any& input1_any = *(args->item(0)->value());
    CORBA::Any& input2_any = *(args->item(1)->value());
    
    // Convert the any params to correct types
    input1_any >>= n;
    input2_any >>= obj;

    // Remove old binding, if such exists
    std::string dn = cosname_to_dn(n);

    // We'll read the given dn and check if it's of type corbaObjectReference
    int res = con_->search(dn+","+bound_context_,LDAP_SCOPE_BASE,
						   "objectClass=corbaObjectReference", attrs_,false);

    if( res == 0 && con_->first_entry() )
    {
		// A binding of type nobject was found, we can rebind it then.
		std::cout << "replacing attributes ";
		std::cout << con_->replace(dn+","+bound_context_, "corbaIor", orb_->object_to_string(obj)) << std::endl;
    }
    else
		throw CosNaming::NamingContext::NotFound();

    std::cout << "NamingContext_impl::rebind() exiting" << std::endl;
}

void NamingContext_impl::bind_context( const CosNaming::Name& n,
									   CosNaming::NamingContext_ptr nc)
{
    // Whatta heck should this do?
}


