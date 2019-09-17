

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



// Implementation for class LDAPConnection
// Declarations are in ldapcon.h

#include <lber.h>
#include <ldap.h>
#include <vector>
#include <string>
#include <cstdio>
#include <doors/ldapcon.h>
#include <doors/ldapattributelist.h>


const int LDAPConnection::oma_default_port = LDAP_PORT;


LDAPConnection::LDAPConnection( std::string host, int port )
	: result_(0), entry_(0), ptr_(0), values_(0),
	  value_offset_(0), values_size_(0)
{
	con_ = 0;
	// avataan yhteys kohteeseen
	std::cout << "LDAPConnection(): ldap_init(" << host << ", port)=";
	con_ = ldap_init( host.c_str(), port );
	int version = LDAP_VERSION3;
	ldap_set_option( con_, LDAP_OPT_PROTOCOL_VERSION, &version );
	std::cout << con_ << std::endl;
}

LDAPConnection::~LDAPConnection()
{
	if( ptr_ != 0 )
		ber_free(ptr_,0);
	ldap_unbind( con_ );
}

LDAPConnection::LDAPConnection()
	: result_(0), entry_(0), ptr_(0), values_(0),
	  value_offset_(0), values_size_(0)
{
}

void LDAPConnection::unbind(void)
{
	ldap_unbind(con_);
	con_ = 0;
}

bool LDAPConnection::isOpen(void) const
{
	if(con_)
		return true;
	return false;
}

bool LDAPConnection::initCon( std::string host, int port )
{
	if( con_ ) // vanha yhteys on paalla, suljetaan se ensin
    {
		if( ptr_ != 0 )
			ber_free(ptr_,0);
		ldap_unbind( con_ );
		con_ = 0;
    }
  
	con_ = ldap_init( host.c_str(), port);

	return ( con_ != 0 ? true : false ); // onko yhteys muodostettu vai not
}


int LDAPConnection::bind( std::string dn, std::string passwd )
{
	std::cout << "LDAPConnection: ldap_bind: " << dn << ", " << passwd << std::endl;
	return ldap_bind_s( con_, dn.c_str(),
						passwd.c_str(), LDAP_AUTH_SIMPLE );
}

int LDAPConnection::search( std::string base, int scope,std::string filter,
							std::vector<std::string> &attrs, bool attrsonly )
{
	char **temp;
	int return_value;

	// Muutetaan vector -> char **
	if( attrs.size() )
    {
		temp = new char*[attrs.size()+1];
		unsigned int i=attrs.size();

		while( i-- )
		{
			temp[i] = new char[attrs[i].length()+1];
			strcpy( temp[i], attrs[i].c_str());
		}
		temp[attrs.size()] = NULL;
    }
	else
		temp = NULL;

	return_value = ldap_search_s( con_,  base.c_str(), 
								  scope, filter.c_str(),
								  temp, attrsonly, &result_ );

	if( temp != NULL )
    {
		unsigned int i=attrs.size();

		while( i-- )
			delete [] temp[i];

		delete [] temp;
    }

	return return_value;
}

int LDAPConnection::compare( std::string dn, std::string att, std::string value ) const
{
	return ldap_compare_s(con_, dn.c_str(),
						  att.c_str(),
						  value.c_str() );
}

int LDAPConnection::modify_dn( std::string olddn, std::string newdn )
{
	//TODO: Deprecated ldap_modrdn2_s was used here which caused problems
	//      when compiling on Solaris. This function needs implementation.
	return 0;
} 

int LDAPConnection::replace(std::string dn, const std::string &att, const std::string &newvalue )
{
	LDAPAttributelist temp;

	temp.addAttribute(att, newvalue);

	LDAPMod **mods = (LDAPMod**)temp;

	int return_value = ldap_modify_s( con_, dn.c_str(), mods );
	ldap_mods_free(temp,0); // Vapautetaan muisti

	return return_value;
}

int LDAPConnection::add_entry(std::string dn,LDAPAttributelist &attrs)
{
	int return_value=0;

	LDAPMod** mods = (LDAPMod**)(attrs);

	return_value = ldap_add_s(con_, dn.c_str(), mods);

	ldap_mods_free(mods, 1);

	return return_value;
}

int LDAPConnection::del(std::string dn)
{
	return ldap_delete_s( con_, dn.c_str() );
}

int LDAPConnection::del(std::string dn, std::string att)
{
	int return_value = 0;

	LDAPAttributelist temp(att);

	LDAPMod** mods = (LDAPMod**)temp;

	return_value = ldap_modify_s( con_, dn.c_str(), mods );
	ldap_mods_free(mods,0); // Vapautetaan muisti

	return return_value;
}

int LDAPConnection::del(std::string dn, std::string att, std::string value)
{
	int return_value = 0;

	LDAPAttributelist temp(att, value, LDAP_MOD_DELETE);

	LDAPMod** mods = (LDAPMod**)temp;

	return_value = ldap_modify_s( con_, dn.c_str(), mods );
	ldap_mods_free(mods,0);
  
	return return_value;
}

bool LDAPConnection::first_entry( void )
{
	entry_ = ldap_first_entry( con_, result_ );
	if( entry_ == NULL )
		return false;

	return true;
}

bool LDAPConnection::next_entry( void )
{
	entry_ = ldap_next_entry( con_, entry_ );

	if( entry_ == NULL )
		return false;

	return true;
}

std::string LDAPConnection::first_attribute( void )
{
	if( ptr_ != 0 )
		ber_free( ptr_,0 );
  
	char *temp = ldap_first_attribute( con_, entry_, &ptr_ );

	if( temp != NULL )
    {
		att_ = temp;
		return att_;
    }

	return "";
}

std::string LDAPConnection::next_attribute( void )
{
	char *temp = ldap_next_attribute( con_, entry_, ptr_ );

	if( temp != NULL )
    {
		att_ = temp;
		return att_;
    }

	return "";
}

std::string LDAPConnection::first_value( void )
{
	values_ = ldap_get_values( con_, entry_, att_.c_str() );
	value_offset_ = 0; // aloitetaan selaaminen alusta

	// lasketaan, kuinka monta arvoa loytyi
	values_size_=0;
	while( values_[values_size_] != NULL )
		values_size_++;

	if( values_[0] != NULL )
		return values_[0];

	return "";
}

std::string LDAPConnection::next_value( void )
{
	// siirrytaan yksi eteenpain ja tarkistetaan samalla ollaanko viela taulussa
	if( ++value_offset_ < values_size_ )
    {
		return values_[value_offset_];
    }

	return "";
}

std::string LDAPConnection::value( unsigned int offset ) const
{
	if( offset < values_size_ )
		return values_[offset];

	return "";
}

std::string LDAPConnection::get_dn(void) const
{
	if( entry_ )
		return ldap_get_dn(con_,entry_);

	return "";
}

void LDAPConnection::Error( std::string operation )
{
	ldap_perror( con_, operation.c_str() );
}
