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

$Log: inetaddr.C,v $
Revision 1.8  2005/03/18 15:52:15  bilhanan
comparison checks now return bool instead of int.

Revision 1.7  2003/11/10 07:53:23  bilhanan
consistency fix from InetAddr6.C: addrToString() to use
stringstream to correctly convert port number

Revision 1.6  2002/11/13 14:55:55  kalliosa
minor bugfixes:
inetaddr6::getPortNumber: ntohs added
inetaddr::getPortNumber: ntohs added
inetAddr6::setPort function addes

Revision 1.5  2002/08/09 07:34:41  hartmanj
Port comparison back to == operator

Revision 1.4  2002/07/25 10:08:39  ik
Use std::string.

Revision 1.3  2002/07/24 14:57:01  ik
Use std::string.

Revision 1.2  2002/06/03 09:19:52  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.4  2002/03/19 14:21:30  bilhanan
slightly faster "<" comparison implementation.

Revision 1.3  2002/03/01 14:33:43  bilhanan
Added "less than" relational operator overload.

Revision 1.2  2002/02/26 07:59:26  bilhanan
added equality check for port number in "==" operator.

Revision 1.1  2002/02/25 17:06:05  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.


*/

#include <string>
#include <doors/inetaddr.h>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sstream>


//
//Constructor: InetAddr
//
//Description:
//   Creates a new instance of inet address.
//   

InetAddr :: InetAddr (void) : Address (AF_INET, sizeof this->inetAddr) {

    (void) ::memset ((void *) &inetAddr, 0, sizeof inetAddr);
}

//
//Function: operator set
//
//Member-Of: InetAddr
//
//Return: int
//
//Parameters:
//        const InetAddr &
//
//Description:
//   Set value for address
//   

int InetAddr :: set (const InetAddr &sa) {

    Address :: baseSet (AF_INET, sizeof this->inetAddr);
    (void) ::memcpy ((void *) &inetAddr, (void *) &sa.inetAddr, 
		     sizeof inetAddr);
    return 0;
}

//
//Constructor: InetAddr
//
//Parameters: 
//        const InetAddr &
//
//Description:
//   Creates a new instance of inet address with value oth.
//   

InetAddr :: InetAddr (const InetAddr &oth) {

    set (oth);
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
//        Uint32
//        int
//
//Description:
//   Set value for address
//   

int InetAddr :: set (unsigned short port_number, Uint32 inet_address, int encode) {

    Address :: baseSet (AF_INET, sizeof inetAddr);
    (void) :: memset ((void *) &inetAddr, 0, sizeof inetAddr);
    inetAddr.sin_family = AF_INET;
    inetAddr.sin_port = encode ? htons (port_number) : port_number;
    
    (void)  :: memcpy ((void *) &inetAddr.sin_addr, (void *) &inet_address,
		       sizeof inetAddr.sin_addr);
    return 0;
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
//        int
//
//Description:
//   Set value for address
//   

int InetAddr :: set (unsigned short port_number, std::string host_name, 
		     int encode) {

    hostent *server_info;  
    Uint32 addr;
    
    Address :: baseSet (AF_INET, sizeof inetAddr);
    (void) ::memset ((void *) &inetAddr, 0, sizeof inetAddr);

    // Yow, someone gave us a NULL host_name!
    if (host_name.empty()) {
	return -1;
    } 
    if ((addr = ::inet_addr (host_name.c_str())) != -1 || 
	(host_name == std::string("255.255.255.255"))) { // Broadcast addresses are weird...
	return set (port_number, addr, encode);
    }
    if ((server_info =  :: gethostbyname (host_name.c_str())) != 0) {
	(void) ::memcpy ((void *) &addr, server_info->h_addr, server_info->h_length);
	return set (port_number, addr, encode);
    }
    
    return -1;   // We must return something !
}

//
//Function: operator set
//
//Member-Of: InetAddr
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

int InetAddr :: set (std::string port_name, std::string host_name) 
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
//        std::string
//        Uint32
//
//Description:
//   Set value for address
//   

int InetAddr :: set (std::string port_name, Uint32 inet_address) 
{

    servent *sp;

    if ((sp = ::getservbyname(port_name.c_str(), "tcp")) == 0) {
	return -1;
    }
    return set (sp->s_port, inet_address, 0);
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

InetAddr :: InetAddr (int port_number, std::string host_name) 
{

    if (set (port_number, host_name) == -1) {
	// LM_ERROR ((LOG_ERROR, "InetAddr :: InetAddr"));
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
//        const sockaddr_in
//        int
//
//Description:
//   Set value for address
//   

int InetAddr :: set (const sockaddr_in *addr, int len) 
{

    Address :: baseSet (AF_INET, len);
    ::memcpy ((void *) &inetAddr, (void *) addr, len);

    return 0;
}

//
//Constructor: InetAddr
//
//Parameters: 
//        sockaddr_in
//        int
//
//Description:
//   Creates a new instance of inet address with value addr.
//   

InetAddr :: InetAddr (const sockaddr_in *addr, int len) 
{

    set (addr, len);
}

//
//Constructor: InetAddr
//
//Parameters: 
//        u_short
//        Uint32
//
//Description:
//   Creates a new instance of inet address with value addr.
//   

InetAddr :: InetAddr (int port_number, Uint32 inet_address) 
{

    if (set (port_number, inet_address) == -1) {
      // LM_ERROR ((LOG_ERROR, "InetAddr :: InetAddr"));
    }
}

//
//Constructor: InetAddr
//
//Parameters: 
//        std::string
//        std::string
//
//Description:
//   Creates a new instance of inet address with value addr.
//   

InetAddr :: InetAddr (std::string port_name, std::string host_name) 
{

    if (set (port_name, host_name) == -1) {
	// LM_ERROR ((LOG_ERROR, "InetAddr :: InetAddr"));
    }
}

//
//Constructor: InetAddr
//
//Parameters: 
//        const char *
//        Uint32
//
//Description:
//   Creates a new instance of inet address with value addr.
//   

InetAddr :: InetAddr (std::string port_name, Uint32 inet_address) 
{

    if (set (port_name, inet_address) == -1) {
	// LM_ERROR ((LOG_ERROR, "InetAddr :: InetAddr"));
    }
}

//
//Function: getAddr
//
//Member-Of: InetAddr
//
//Return: void *
//
//Description:
//   Return address 
//   

void * InetAddr :: getAddr (void) const 
{

    return (void *) &inetAddr;
}

//
//Function: addrToString
//
//Member-Of: InetAddr
//
//Return: std::string
//
//
//Description:
//   Create string out of address.
//   

std::string InetAddr :: addrToString () const 
{
  std::string result = getHostAddr();
  result += ":";
  std::stringstream port_stream;
  port_stream << (int)getPortNumber();
  result += port_stream.str();
  return result;
}

//
//Function: operator ==
//
//Member-Of: InetAddr
//
//Return: bool
//
//Parameters:
//        const Address &
//
//Description:
//   Overloaded compare function.
//   

bool InetAddr :: operator == (const Address &oth) const 
{
  return  ((:: memcmp ((void *) &inetAddr.sin_addr, 
					   (void *) &((InetAddr &) oth).inetAddr.sin_addr,
					   sizeof (inetAddr.sin_addr)) == 0)
		   && (inetAddr.sin_port == ((InetAddr&)oth).inetAddr.sin_port));

}

//
//Function: operator !=
//
//Member-Of: InetAddr
//
//Return: bool
//
//Parameters:
//        const Address &
//
//Description:
//   Overloaded compare function.
//   

bool InetAddr :: operator != (const Address &oth) const 
{

    return !((*this) == oth);
}


//
//Function: operator <
//
//Member-Of: InetAddr
//
//Return: bool
//
//Parameters:
//        const Address &
//
//Description:
//   Overloaded compare function.
// 

bool InetAddr :: operator < (const InetAddr &oth) const 
{

  if  (this->inetAddr.sin_addr.s_addr < oth.inetAddr.sin_addr.s_addr)
    return true;

  if (this->inetAddr.sin_addr.s_addr == oth.inetAddr.sin_addr.s_addr) {
    if (this->inetAddr.sin_port < oth.inetAddr.sin_port)
      return true;
  }

  return false;

}

//
//Function: operator getHostName
//
//Member-Of: InetAddr
//
//Return: std::string
//
//Description:
//   Return the name of host.
//   

std::string InetAddr :: getHostName (void) const 
{

    hostent *hp;
    int a_len = sizeof inetAddr.sin_addr.s_addr;

    if ((hp = ::gethostbyaddr ((char *) &inetAddr.sin_addr, a_len, addrType)) == 0) {
	return 0;
    }
    return hp->h_name;
}

//
//Function: operator getHostAddr
//
//Member-Of: InetAddr
//
//Return: std::string
//
//Description:
//   Return the address of host.
//   

std::string InetAddr :: getHostAddr (void) const 
{

    return inet_ntoa (inetAddr.sin_addr);  
}

//
//Function: operator getIpAddress
//
//Member-Of: InetAddr
//
//Return: Uint32
//
//Description:
//   Return the address.
//   

Uint32 InetAddr :: getIpAddress (void) const 
{

    return Uint32 (inetAddr.sin_addr.s_addr);
}

//
//Function: operator getPortNumber
//
//Member-Of: InetAddr
//
//Return: u_short
//
//Description:
//   Return the port number.
//   

unsigned short InetAddr :: getPortNumber (void) const {

    return ntohs( inetAddr.sin_port);
}
