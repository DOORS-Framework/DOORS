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

$Log: serviceurl.h,v $
Revision 1.16  2006/12/21 13:13:57  bilhanan
devt

Revision 1.15  2004/06/17 08:49:47  hartmanj
Added support for transient service URL:s

Revision 1.14  2004/04/05 07:07:21  hartmanj
Removed unnecessary #includes

Revision 1.13  2004/03/25 09:26:23  hartmanj
Development.

Revision 1.12  2004/03/22 11:05:47  hartmanj
Added support for pure servicetype strings such as service:lpr without the URL. Especially helpful in attribute requests.

Revision 1.11  2004/03/15 15:19:05  hartmanj
Added getAbstractType()

Revision 1.10  2004/03/11 14:08:54  hartmanj
Added isOk() and bool errorFlag so ok-ness of Service URL can be determined after parsing

Revision 1.9  2004/03/01 11:39:49  bilhanan
added assignment operator.
added attrs_ in copy constructor.

Revision 1.8  2004/02/26 11:01:38  hartmanj
Added setLifetime()

Revision 1.7  2004/02/25 13:01:39  hartmanj
Rewrote parsing and handling of ServiceURL. ServiceType-class no longer needed.

Revision 1.6  2004/02/01 22:52:37  bilhanan
reduced number of data members, changed constructor, introduced setPort

Revision 1.5  2004/01/23 22:13:11  bilhanan
constructor ambiguity removed.

Revision 1.4  2003/11/27 12:32:56  bilhanan
bug fixing

Revision 1.3  2003/08/11 23:09:50  bilhanan
added isEmpty() function.

Revision 1.2  2003/06/04 11:09:55  kalliosa
Licence clause added

  
*/


#ifndef SERVICEURL_H
#define SERVICEURL_H

#include <string>
#include <map>

#include <doors/slptypes.h>


#define  SRVURL_NO_PORT 0
#define  SRVURL_LIFETIME_NONE 0
#define  SRVURL_LIFETIME_DEFAULT 10800
#define  SRVURL_LIFETIME_MAXIMUM 65535
#define  SRVURL_LIFETIME_PERMANENT -1


class ServiceType;

class ServiceURL 
{
 public:

     ServiceURL( std::string url, Sint32 lifetime = SRVURL_LIFETIME_DEFAULT); 
     ServiceURL( const ServiceURL &src );
     ServiceURL( void );
     ~ServiceURL();
     std::string getServiceType( void ) const ;
     std::string getAbstractType( void ) const ;
     std::string getURL( void ) const ;
     std::string getURLPart( void ) const ;
     std::string getHost( void ) const;
     InetAddr6  getHostAddr( void ) const;
     Uint16 getPort( void ) const;
     void setPort (Uint16 port);
     Sint32 getLifetime( void ) const;
     void setLifetime( Sint32 ltime );
     void setServiceType( std::string newType );
     void updateAddr( const std::string &new_addr );
     bool isTransient() const;
  
     bool isOfServiceType( const std::string &type ) const;
     bool isOfType( const ServiceURL &u ) const;
     bool isPureType( ) { return serviceIsPureType_; };
     bool operator==( const ServiceURL &u ) const ;
     bool operator!=( const ServiceURL &u ) const ;
     void operator = (const ServiceURL &src );
     bool isEmpty (void) const;
     bool isOk( void );
private:
     bool isValidAddr( const std::string &str );
     void parseUrl( void );
     std::string url_;
     std::string urlPart_;
     Sint32 lifetime_;
     Uint16 port_;
     std::string aType_;
     std::string cType_;
     std::string serviceType_;
     std::string NA_;
     std::string host_;

     bool is_transient; // if the service: URL is transient (automatic address expansion)
     bool errorFlag_;
     
     std::map<std::string,std::string> attrs_;

     bool serviceIsPureType_;
};

#endif // SERVICEURL_H   
   
