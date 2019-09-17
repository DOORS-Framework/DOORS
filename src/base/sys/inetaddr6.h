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

$Log: inetaddr6.h,v $
Revision 1.6  2005/03/18 15:52:15  bilhanan
comparison checks now return bool instead of int.

Revision 1.5  2003/08/07 09:55:42  bilhanan
getIpAddress() added.

Revision 1.4  2002/11/13 14:55:55  kalliosa
minor bugfixes:
inetaddr6::getPortNumber: ntohs added
inetaddr::getPortNumber: ntohs added
inetAddr6::setPort function addes

Revision 1.3  2002/07/24 14:57:01  ik
Use std::string.

Revision 1.2  2002/06/17 13:28:10  ik
Constructor and set() for port number only.

Revision 1.1  2002/06/04 18:21:00  ik
InetAddr6 class.


*/

#ifndef INET_ADDR6_H
#define INET_ADDR6_H

#include <string>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/un.h>

#include <doors/addr.h>


/** This class defines an interface for using internet socket
  addresses. The class is inherited from abstract Address
  base class of PTB.
  */

class InetAddr6 : public Address {
  public:
  /** Creates a new instance of inet address. */
  InetAddr6 (void);

  /** Creates a new instance of inet address with value oth */
  InetAddr6 (const InetAddr6 &);

  /** Creates a new instance of inet address with value addr.
    @param addr the structure of adress
    @param len the lenght of adress structure*/
  InetAddr6 (const sockaddr_in6 *, int len);

  /** Creates a new instance of inet address with value addr.
    @param addr the structure of adress
    @param len the lenght of adress structure*/
  InetAddr6 (int port_number, std::string host_name);

  /** Creates a new instance of inet address with value port.
    @param addr the structure of adress
    @param len the lenght of adress structure*/
  InetAddr6 (int port_number);

  /** Creates a new instance of inet address with value addr.
    @param port_name the name of socket port
    @param host_name the name of host*/   
  InetAddr6 (std::string port_name, std::string host_name);

  /** Set value for address 
    @param sa the adress value*/   
  int set (const InetAddr6 &);

  /** Set value for address 
    @param port_number the socket port number
    @param host_name the name of host */   
  int set (unsigned short port_number, std::string host_name, int encode = 1);

  /** Set value for address 
    @param port_number the socket port number */
  int set (unsigned short port_number);

  /** Set value for address 
    @param port_name the name of socket port
    @param host_name the name of host*/   
  int set (std::string port_name, std::string host_name);
  
  /** Set value for address 
    @param addr is the socket address structure 
    @param len is the lenght of adress.*/   
  int set (const sockaddr_in6 *, int len);

   /** Set port number
    Sets only the port number */   

  int setPort (unsigned short port_number);
   
   
  /**@return the address */
  virtual void *getAddr (void) const;

  /** Create string out of address. 
   @param s is the table for the string
   @return zero*/
  virtual std::string addrToString () const;

  virtual bool operator == (const Address &oth) const;
  virtual bool operator != (const Address &oth) const;
  virtual bool operator < (const InetAddr6 &oth) const; 
    
  /** @return the name of host.*/
  std::string getHostName (void) const;

  /** @return the address of host.*/
  std::string getHostAddr (void) const;

  /** @return the IP address of host.*/
  void getIpAddress (Frame *ipaddr);

  /** @return the port number */
  unsigned short getPortNumber (void) const;
    
  private:
    struct sockaddr_in6 inetAddr;
};


#endif  // INET_ADDR6_H
