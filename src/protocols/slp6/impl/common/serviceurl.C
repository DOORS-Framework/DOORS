//  Copyright 2003
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

$Log: serviceurl.C,v $
Revision 1.23  2006/12/21 13:13:57  bilhanan
devt

Revision 1.22  2004/06/17 08:49:47  hartmanj
Added support for transient service URL:s

Revision 1.21  2004/04/14 09:17:48  hartmanj
Bug fixing

Revision 1.20  2004/04/05 07:07:21  hartmanj
Removed unnecessary #includes

Revision 1.19  2004/03/25 12:38:47  hartmanj
Compile fixes

Revision 1.18  2004/03/25 09:26:23  hartmanj
Development.

Revision 1.17  2004/03/22 11:05:47  hartmanj
Added support for pure servicetype strings such as service:lpr without the URL. Especially helpful in attribute requests.

Revision 1.16  2004/03/15 15:18:53  hartmanj
Added getAbstractType() and fixed isOfServiceType()

Revision 1.15  2004/03/15 09:38:16  hartmanj
Bug fixing


Revision 1.14  2004/03/15 06:50:00  hartmanj
Added service: in front of member serviceType_

Revision 1.13  2004/03/11 14:08:54  hartmanj
Added isOk() and bool errorFlag so ok-ness of Service URL can be determined after parsing

Revision 1.12  2004/03/01 11:39:49  bilhanan
added assignment operator.
added attrs_ in copy constructor.

Revision 1.11  2004/02/26 11:01:04  hartmanj
Now #including serviceurlparser.h instead of .C. Added setLifetime()

Revision 1.10  2004/02/25 13:30:58  hartmanj
Added getPort() and setPort() to parser.

Revision 1.9  2004/02/25 13:01:39  hartmanj
Rewrote parsing and handling of ServiceURL. ServiceType-class no longer needed.

Revision 1.8  2004/02/16 13:35:05  hartmanj
Fixed substringing in ServiceURLparseUrl()

Revision 1.7  2004/02/01 23:14:04  bilhanan
forced commit, typo in previous log: setPort introduced, getPort return val changed.

Revision 1.6  2004/02/01 22:53:44  bilhanan
changed constructors, changed setPort return value, added getPort

Revision 1.5  2004/01/23 22:13:10  bilhanan
constructor ambiguity removed.

Revision 1.4  2003/11/27 12:32:56  bilhanan
bug fixing

Revision 1.3  2003/08/11 23:09:50  bilhanan
added isEmpty() function.

Revision 1.2  2003/06/04 11:09:54  kalliosa
Licence clause added
  
*/


#include <string>
#include <doors/serviceurl.h>

#include <doors/debug.h>


#if defined(STRING) || defined(HEXSTRING) || defined(NUM) || defined(IPSTART) \
    || defined(IPXSTART) || defined(ATSTART) || defined(SERVICESTR)
#error Conflicting #defines!
#endif

#include "serviceurlparser.h"

extern DOORS_ServiceURL_Parser_Class DOORS_ServiceURL_Parser;

ServiceURL :: ServiceURL( const ServiceURL &src )
    : lifetime_(src.lifetime_), port_(src.port_), errorFlag_(src.errorFlag_),
      serviceIsPureType_(src.serviceIsPureType_), is_transient(src.is_transient)
{

   if (!(src.aType_.empty()))
        aType_ = src.aType_;

   if (!(src.cType_.empty()))
        cType_ = src.cType_;

   if (!(src.serviceType_.empty()))
        serviceType_ = src.serviceType_;

   if (!(src.NA_.empty()))
        NA_ = src.NA_;

   if (!(src.url_.empty()))
        url_ = src.url_;

   if (!(src.urlPart_.empty()))
       urlPart_ = src.urlPart_;

   if (!(src.host_.empty()))
        host_ = src.host_;

   if (!(src.attrs_.empty()))
     attrs_ = src.attrs_;  // expensive operation!
}


ServiceURL :: ServiceURL( std::string url, Sint32 lifetime) 
  : url_( url ), lifetime_( lifetime ), errorFlag_(false),serviceIsPureType_(false),
    is_transient(false)
{
     DOORS_ServiceURL_Parser.setURL(url_);

     if( yyparse() != 0 ) {
          std::cerr << "Error parsing service URL: " << url << std::endl;
          errorFlag_ = true;
     } else {
          attrs_ = DOORS_ServiceURL_Parser.getAllAttributes();
          aType_ = DOORS_ServiceURL_Parser.getAbstractType();
          cType_ = DOORS_ServiceURL_Parser.getConcreteType();
         if(aType_.empty()) {
           serviceType_ = "service:" + cType_;
         } else {
           serviceType_ = "service:" + aType_ + ':' + cType_;
         }

         if( DOORS_ServiceURL_Parser.isPureType() ) {
           serviceIsPureType_ = true;
         }

         is_transient = DOORS_ServiceURL_Parser.getTransient();

         if( !is_transient ) {
           host_ = DOORS_ServiceURL_Parser.getHostAddr();
         } else {
             host_ = "coa";
         }

         port_ = DOORS_ServiceURL_Parser.getPort();
         urlPart_ = DOORS_ServiceURL_Parser.getAddrSpec();
     }
}


ServiceURL :: ServiceURL( void )
  : lifetime_ (SRVURL_LIFETIME_DEFAULT), errorFlag_(true),serviceIsPureType_(false),
    is_transient(false)
{
}

bool ServiceURL :: isOk( void )
{
  return errorFlag_;
}

bool ServiceURL :: isValidAddr( const std::string &str )
{
   std::string s = str;
   _DOORS_DEBUG(0,"checking string: '" << str << "'" );
   int block = 0; // number of blocks separated by ':'
   int chars = 0; // number of cahrs in blocks
   
   bool rv = false;
   //remove brackets
   if( s[0] == '[' )   
   {  
      s.erase( s.begin() );
      if( s[s.size() - 1] != ']' )
      {	
	 _DOORS_DEBUG(0,"asd" );
	 return false;
      }     
      else
	s.erase( s.end() -1 );            
   }
   
   // check rest of address
   for( std::string::const_iterator i=s.begin(); i!=s.end();++i )
   {
      if( *i == ':')
      {
	 chars = 0;
	 ++block;
	 if( i != s.begin() && *(i - 1) != ':' && block == 0 )
	 {
	    _DOORS_DEBUG(0, "asd" );
	    return false;
	 }      
      }
      
      else if( !isdigit(*i)  && !( (((char)(*i)) < (char)'a' )
		&&( ((char)(*i)) > (char)'f' )) )
				  
      {
//	 _DOORS_DEBUG(0,"true " << (*i));
	 rv = true;
      }
      else
	++chars;
      
      if(chars > 4)
      {
	 _DOORS_DEBUG(0,"too many chars");
	 return false;
      }
      
      if( block > 7) 
      {
	 _DOORS_DEBUG(0, "too many blocks");
	 return false;
      }
      
      
   }
   return rv;   

}


ServiceURL :: ~ServiceURL()
{
   // nothing to clean up
}

std::string ServiceURL :: getURL( void ) const 
{
   return url_;
}

std::string ServiceURL :: getServiceType( void ) const 
{
     return serviceType_;
}

std::string ServiceURL :: getAbstractType( void ) const 
{
     return std::string("service:" + aType_);
}

void ServiceURL :: updateAddr( const std::string &new_addr ) {
    if( is_transient ) {
        std::string old_host = host_;
        host_ = new_addr;
        int idx = url_.find( old_host );
        if( idx != std::string::npos ) {
            url_.replace( idx, old_host.size(), host_ );
        }
    }
}

bool ServiceURL :: isTransient( ) const {
    return is_transient;
}

std::string ServiceURL :: getHost( void ) const 
{
   return host_;
}

InetAddr6  ServiceURL :: getHostAddr( void ) const
{
     InetAddr6 addr (getPort(), host_ ); 
     
     return addr;
}

Uint16 ServiceURL :: getPort( void ) const
{
   return port_;
}

void ServiceURL :: setServiceType( std::string newType )
{
   serviceType_ = newType;
}

void ServiceURL :: setPort (Uint16 port)
{
  port_ = port;
}

Sint32  ServiceURL :: getLifetime( void ) const
{
   return lifetime_;
}

void  ServiceURL :: setLifetime( Sint32 ltime )
{
     lifetime_ = ltime;
}

std::string ServiceURL :: getURLPart() const 
{
   return urlPart_;
}

bool ServiceURL :: isOfType( const ServiceURL &u ) const
{
     if( u.aType_ == this->aType_ && u.cType_ == this->cType_)
          return true;

     if( u.isOfServiceType( serviceType_ ) )
        return true;
     
     return false;
}

bool ServiceURL :: isOfServiceType( const std::string &type ) const
{

  if( type == serviceType_ )
    return true;

  if( !aType_.empty() && type == std::string("service:" + aType_))
    return true;

  return false;
}

bool ServiceURL :: operator==( const ServiceURL &u ) const 
{
   if( url_ == u.url_ )
     return true;
   else 
     return false;
}

bool ServiceURL :: operator!=( const  ServiceURL &u ) const
{
   return !operator==( u );
}

void ServiceURL :: operator = (const  ServiceURL &src) 
{
    if (!(src.aType_.empty()))
        aType_ = src.aType_;

   if (!(src.cType_.empty()))
        cType_ = src.cType_;

   if (!(src.serviceType_.empty()))
        serviceType_ = src.serviceType_;

   if (!(src.NA_.empty()))
        NA_ = src.NA_;

   if (!(src.url_.empty()))
        url_ = src.url_;

   if (!(src.urlPart_.empty()))
       urlPart_ = src.urlPart_;

   if (!(src.host_.empty()))
        host_ = src.host_;
  
   if (!(src.attrs_.empty()))
     attrs_ = src.attrs_;  // expensive operation!
   
   lifetime_ = src.lifetime_;
   port_ = src.port_;
   is_transient = src.is_transient;
}

bool ServiceURL :: isEmpty (void) const 
{
   if( url_ == "" )
     return true;
   else 
     return false;
}
