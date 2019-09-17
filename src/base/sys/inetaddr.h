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

$Log: inetaddr.h,v $
Revision 1.4  2005/03/18 15:52:15  bilhanan
comparison checks now return bool instead of int.

Revision 1.3  2002/07/24 14:57:01  ik
Use std::string.

Revision 1.2  2002/06/03 09:19:53  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.4  2002/03/01 14:33:05  bilhanan
added "less than" relational check.

Revision 1.3  2002/02/26 12:43:55  ik
Unwanted declarations removed.

Revision 1.2  2002/02/25 17:52:40  ik
sockaddr.* modifications.

Revision 1.1  2002/02/25 17:06:05  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.


*/

#ifndef INET_ADDR_H
#define INET_ADDR_H

#include <string>
#include <doors/addr.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/un.h>


/** This class defines an interface for using internet socket
  addresses. The class is inherited from abstract Address
  base class of PTB.
  */

class InetAddr : public Address {
  public:
  /** Creates a new instance of inet address. */
  InetAddr (void);

  /** Creates a new instance of inet address with value oth */
  InetAddr (const InetAddr &);

  /** Creates a new instance of inet address with value addr.
    @param addr the structure of adress
    @param len the lenght of adress structure*/
  InetAddr (const sockaddr_in *, int len);

  /** Creates a new instance of inet address with value addr.
    @param addr the structure of adress
    @param len the lenght of adress structure*/
  InetAddr (int port_number, std::string host_name);

  /** Creates a new instance of inet address with value addr.
    @param port_number the socket port number
    @param inet_address the ip adress*/   
  InetAddr (int port_number, Uint32 ip_addr = INADDR_ANY);

  /** Creates a new instance of inet address with value addr.
    @param port_name the name of socket port
    @param host_name the name of host*/   
  InetAddr (std::string port_name, std::string host_name);

  /** Creates a new instance of inet address with value addr.
    @param port_name the socket port name.
    @param inet_address the ip adress.*/   
  InetAddr (std::string port_name, Uint32 ip_addr = INADDR_ANY);
  
  /** Set value for address 
    @param sa the adress value*/   
  int set (const InetAddr &);

  /** Set value for address 
    @param port_number the socket port number
    @param host_name the name of host */   
  int set (unsigned short port_number, std::string host_name, int encode = 1);

  /** Set value for address 
    @param port_number the socket port number
    @param inet_address the ip adress*/   
  int set (unsigned short port_number, Uint32 ip_addr = INADDR_ANY, int encode = 1);

  /** Set value for address 
    @param port_name the name of socket port
    @param host_name the name of host*/   
  int set (std::string port_name, std::string host_name);
  
  /** Set value for address 
    @param port_name the socket port name.
    @param inet_address the ip adress.*/   
  int set (std::string port_name, Uint32 ip_addr = INADDR_ANY);

  /** Set value for address 
    @param addr is the socket address structure 
    @param len is the lenght of adress.*/   
  int set (const sockaddr_in *, int len);

  /**@return the address */
  virtual void *getAddr (void) const;

  /** Create string out of address. 
   @param s is the table for the string
   @return zero*/
  virtual std::string addrToString () const;

  virtual bool operator == (const Address &oth) const;
  virtual bool operator != (const Address &oth) const;
  virtual bool operator < (const InetAddr &oth) const; 
    
  /** @return the name of host.*/
  std::string getHostName (void) const;

  /** @return the address of host.*/
  std::string getHostAddr (void) const;

  /** @return the IP address of host.*/
  Uint32 getIpAddress (void) const;

  /** @return the port number */
  unsigned short getPortNumber (void) const;
    
  private:
    struct sockaddr_in inetAddr;
};


#endif  // INET_ADDR_H
