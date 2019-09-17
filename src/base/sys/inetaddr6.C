// 
//  Copyright 1999 
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
 
 
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

$Log: inetaddr6.C,v $
Revision 1.17  2009/04/20 13:44:23  bilhanan
bugfix to allow c-style string manipulation

Revision 1.16  2005/05/04 11:35:36  bilhanan
bugfix: encode bit was not respected previously

Revision 1.15  2005/03/18 15:52:15  bilhanan
comparison checks now return bool instead of int.

Revision 1.14  2005/02/17 16:41:40  ik
Result value of getaddrinfo() checked for errors.

Revision 1.13  2004/02/01 23:10:41  bilhanan
Improved error diagnostics.

Revision 1.12  2004/01/28 09:18:33  hartmanj
Fixed operator <

Revision 1.11  2003/11/05 09:16:09  hartmanj
InetAddr6.addrToString() to use stringstream to correctly convert port number

Revision 1.10  2003/08/07 10:06:43  bilhanan
Addition:
	getIpAddress() function.
Cleanup:
	redundant sizeof() operation in getHostName() removed.
	redundant #include <string.h> removed.
	*DO NOT* use std::cout in DOORS core code. replaced with
	debug macro in set() function.

Revision 1.9  2002/11/13 14:55:55  kalliosa
minor bugfixes:
inetaddr6::getPortNumber: ntohs added
inetaddr::getPortNumber: ntohs added
inetAddr6::setPort function addes

Revision 1.8  2002/07/25 10:08:40  ik
Use std::string.

Revision 1.7  2002/07/24 14:57:01  ik
Use std::string.

Revision 1.6  2002/06/19 09:21:24  ik
Memory leak in addrToString(). To use snprintf.

Revision 1.5  2002/06/19 09:13:29  ik
addrToString IPv6-style. getHostAddr to use inet_ntop because getaddrinfo
didn't work. Copying IPv6 address in set() fixed.

Revision 1.4  2002/06/17 13:28:10  ik
Constructor and set() for port number only.

Revision 1.3  2002/06/17 10:53:56  ik
#include <sstream> removed and code changed accordingly.

Revision 1.2  2002/06/14 14:56:18  ik
Maintenance commit.

Revision 1.1  2002/06/04 18:21:00  ik
InetAddr6 class.


*/

#include <string>

#include <doors/inetaddr6.h>
#include <doors/debug.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sstream>
#include <cstring>

//
//Constructor: InetAddr6
//
//Description:
//   Creates a new instance of inet address.
//   

InetAddr6 :: InetAddr6 (void) 
  : Address (AF_INET6, sizeof this->inetAddr)
{
}

//
//Function: operator set
//
//Member-Of: InetAddr6
//
//Return: int
//
//Parameters:
//        const InetAddr6 &
//
//Description:
//   Set value for address
//   

int InetAddr6 :: set (const InetAddr6 &sa) 
{

    Address :: baseSet (AF_INET6, sizeof this->inetAddr);
    (void) ::memcpy ((void *) &inetAddr, (void *) &sa.inetAddr, 
		     sizeof inetAddr);
    return 0;
}

//
//Constructor: InetAddr6
//
//Parameters: 
//        const InetAddr6 &
//
//Description:
//   Creates a new instance of inet address with value oth.
//   

InetAddr6 :: InetAddr6 (const InetAddr6 &oth) {

    set (oth);
}

//
//Function: operator set
//
//Member-Of: InetAddr6
//
//Return: int
//
//Parameters:
//        u_short
//
//Description:
//   Set value for address
//   

int InetAddr6 :: setPort (unsigned short port_number)
{
   inetAddr.sin6_port = htons(port_number);
   return 0;
}


int InetAddr6 :: set (unsigned short port_number)
{
    ::memset(&inetAddr, 0, sizeof(inetAddr));
    inetAddr.sin6_family = AF_INET6;
    inetAddr.sin6_flowinfo = 0;
    inetAddr.sin6_port = htons(port_number);
    inetAddr.sin6_addr = in6addr_any;
    return 0;
}

//
//Function: operator set
//
//Member-Of: InetAddr6
//
//Return: int
//
//Parameters:
//        u_short
//        std::string
//        int
//
//Description:
//   Set value for address
//   

int InetAddr6 :: set (unsigned short port_number, std::string host_name, 
		      int encode) 
{
    char port_number_buf[16];
    int result = -1;
    if (!encode) {
      port_number = htons(port_number);
    }

    Address :: baseSet (AF_INET6, sizeof inetAddr);
    (void) ::memset ((void *) &inetAddr, 0, sizeof inetAddr);

    // You could at least try!
    if (host_name.empty()) {
	return -1;
    } 

    // getaddrinfo() implementations may differ. On Solaris 8/9,
    // you may need to add line: "ipnodes: files dns" to
    // /etc/nsswitch.conf to make IPv6 lookups to work.
    // The other feature in Solaris: getaddrinfo() refers to
    // /etc/services so that if ai_socktype is zero (not set),
    // only the ports listed in /etc/services are valid. If
    // ai_socktype is set, any numeric port is valid but if service
    // name is used (like "tftp"), its socktype is checked and fails
    // you you try to offer, for example, SOCK_STREAM type and
    // tftp service which would require SOCK_DGRAM socktype.
    snprintf(port_number_buf, 16, "%d", port_number);
    struct addrinfo hints, *res = NULL, *res0 = NULL;
    int aires = 0;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    if ((aires = getaddrinfo(host_name.c_str(), 
			     port_number_buf, &hints, &res0)) != 0) {
      _DOORS_DEBUG (0, "getaddrinfo returns error.");
      return -1;
    }
    if (!res0) {
      _DOORS_DEBUG (0, "getaddrinfo returns no results.");
      return -1;
    }
    for (res = res0; res; res = res->ai_next) {
      if (res->ai_family == AF_INET6) {
	// Found an IPv6 address; we use this and forget all the rest
	(void)  ::memcpy ((void *) &inetAddr, 
			  (void *) res->ai_addr, sizeof inetAddr);
	result = 0;
	break;
      }
    }
    freeaddrinfo(res0);

    return result;
}

//
//Function: operator set
//
//Member-Of: InetAddr6
//
//Return: int
//
//Parameters:
//        std::string
//        std::string
//
//Description:
//   Set value for address
//   

int InetAddr6 :: set (std::string port_name, std::string host_name) 
{

    servent *sp;
    
    if ((sp = ::getservbyname (port_name.c_str(), "tcp")) == 0) {
	return -1;
    }
    return set (sp->s_port, host_name, 0);
}

//
//Function: operator set
//
//Member-Of: InetAddr
//
//Return: int
//
//Parameters:
//        u_short
//        std::string
//
//Description:
//   Set value for address
//   

InetAddr6 :: InetAddr6 (int port_number, std::string host_name) 
{

    if (set (port_number, host_name) == -1) {
      _DOORS_DEBUG (0, "Error setting port_number " << port_number << ", host_name " << host_name);
    }
}

//
//Function: operator set
//
//Member-Of: InetAddr
//
//Return: int
//
//Parameters:
//        u_short
//
//Description:
//   Set value for address using port number only
//   

InetAddr6 :: InetAddr6 (int port_number) 
{
    set(port_number);
}

//
//Function: operator set
//
//Member-Of: InetAddr6
//
//Return: int
//
//Parameters:
//        const sockaddr_in6
//        int
//
//Description:
//   Set value for address
//   

int InetAddr6 :: set (const sockaddr_in6 *addr, int len) 
{

    Address :: baseSet (AF_INET6, len);
    ::memcpy ((void *) &inetAddr, (void *) addr, len);

    return 0;
}

//
//Constructor: InetAddr6
//
//Parameters: 
//        sockaddr_in6
//        int
//
//Description:
//   Creates a new instance of inet address with value addr.
//   

InetAddr6 :: InetAddr6 (const sockaddr_in6 *addr, int len) 
{

    set (addr, len);
}

//
//Constructor: InetAddr6
//
//Parameters: 
//        std::string
//        std::string
//
//Description:
//   Creates a new instance of inet address with value addr.
//   

InetAddr6 :: InetAddr6 (std::string port_name, std::string host_name) 
{

    if (set (port_name, host_name) == -1) {
      _DOORS_DEBUG (0, "Error setting port_name " << port_name << ", host_name " << host_name);
    }
}

//
//Function: getAddr
//
//Member-Of: InetAddr6
//
//Return: void *
//
//Description:
//   Return address 
//   

void * InetAddr6 :: getAddr (void) const 
{

    return (void *) &inetAddr;
}

//
//Function: addrToString
//
//Member-Of: InetAddr6
//
//Return: std::string
//
//Description:
//   Create string out of address.
//   

std::string InetAddr6 :: addrToString () const 
{
  std::string result = "[";
  result += getHostAddr();
  result += "]:";
  std::stringstream port_stream;
  port_stream << (int)getPortNumber();
  result += port_stream.str();
  return result;
}

//
//Function: operator ==
//
//Member-Of: InetAddr6
//
//Return: bool
//
//Parameters:
//        const Address &
//
//Description:
//   Overloaded compare function.
//   

bool InetAddr6 :: operator == (const Address &oth) const 
{

    return  ((:: memcmp ((void *) &inetAddr.sin6_addr, 
		       (void *) &((InetAddr6 &) oth).inetAddr.sin6_addr,
		       sizeof (inetAddr.sin6_addr)) == 0)
	     && (inetAddr.sin6_port == ((InetAddr6&)oth).inetAddr.sin6_port));
}

//
//Function: operator !=
//
//Member-Of: InetAddr6
//
//Return: bool
//
//Parameters:
//        const Address &
//
//Description:
//   Overloaded compare function.
//   

bool InetAddr6 :: operator != (const Address &oth) const 
{

    return !((*this) == oth);
}


//
//Function: operator <
//
//Member-Of: InetAddr6
//
//Return: bool
//
//Parameters:
//        const Address &
//
//Description:
//   Overloaded compare function.
// 

bool InetAddr6 :: operator < (const InetAddr6 &oth) const 
{
     unsigned char addr_idx = 0;

     while(addr_idx < 16 && this->inetAddr.sin6_addr.s6_addr[addr_idx] ==
           oth.inetAddr.sin6_addr.s6_addr[addr_idx])
          addr_idx++;

     if( addr_idx < 16 )
          return (this->inetAddr.sin6_addr.s6_addr[addr_idx] <
                  oth.inetAddr.sin6_addr.s6_addr[addr_idx]);

     return (this->inetAddr.sin6_port < oth.inetAddr.sin6_port);
}

//
//Function: operator getHostName
//
//Member-Of: InetAddr6
//
//Return: std::string
//
//Description:
//   Return the name of host.
//   

std::string InetAddr6 :: getHostName (void) const 
{
    const int ADDRLEN = 256;
    int a_len = sizeof inetAddr.sin6_addr.s6_addr;
    char buf[ADDRLEN];
    ::getnameinfo((struct sockaddr*)&inetAddr, a_len, buf, ADDRLEN, 0, 0, 0); 
    return buf;
}

//
//Function: operator getHostAddr
//
//Member-Of: InetAddr6
//
//Return: std::string
//
//Description:
//   Return the address of host.
//   

std::string InetAddr6 :: getHostAddr (void) const 
{
    const int ADDRLEN = 256;
    char buf[ADDRLEN];
    ::inet_ntop(AF_INET6, (const void *) &inetAddr.sin6_addr.s6_addr,
		buf, ADDRLEN);
    return buf;
}

//Function: operator getIpAddress
//
//Member-Of: InetAddr6
//
//Return: void
//
//Description:
//   Passes the 128-bit address into the given frame.
//   

void InetAddr6 :: getIpAddress (Frame *ipaddr) 
{

  ipaddr->putLast((const Byte *)inetAddr.sin6_addr.s6_addr, 16);
  return;
}

//
//Function: operator getPortNumber
//
//Member-Of: InetAddr6
//
//Return: u_short
//
//Description:
//   Return the port number.
//   

unsigned short InetAddr6 :: getPortNumber (void) const 
{

    return ntohs( inetAddr.sin6_port );
}
