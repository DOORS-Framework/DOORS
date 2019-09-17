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

$Log: unixaddr.h,v $
Revision 1.4  2005/03/18 15:52:16  bilhanan
comparison checks now return bool instead of int.

Revision 1.3  2002/07/25 10:08:40  ik
Use std::string.

Revision 1.2  2002/06/03 09:19:54  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.1  2002/02/25 17:06:05  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.


*/

#ifndef UNIX_ADDR_H
#define UNIX_ADDR_H

#include <string>
#include <doors/addr.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/un.h>


/** This class defines an interface for using unix socket
  addresses. The class is inherited from abstract Address
  base class of PTB. */
  

class UnixAddr : public Address {
public:
  /** Creates a new instance of unix address with zero value.*/
  UnixAddr (void);
  
  /** Creates a new instance of unix address by the given address.
    @param oth other unix address to be cloned.*/
  UnixAddr (const UnixAddr &oth);
  
  /** Creates a new instance of unix address by the given
    address value.   
    @param rendezvous_point the value of the address.*/
  UnixAddr (std::string rendezvous_point);
  
  /** Creates a new instance of unix address by the given address value.
    @parameter sockaddr_un is the C-structure used to hold address value.
    @param len the length of the address value.*/
  UnixAddr (const sockaddr_un *, int len);
  
  /** Re-initializes this instance of unix address by the
    given address value.    
    @param rendezvous_point is the value of the address.*/
  void set (std::string rendezvous_point);
  
  /** Re-initializes this instance of unix address by the
    given address value.    
    @param sockaddr_un is the C-structure used to hold address value.
    @param len the length of the address value.*/
  void set (const sockaddr_un *, int len);

  /** @return The adress */
  virtual void *getAddr (void) const;

  /** Create string out of address. 
   @param addr the address
   @param size_t the lenght of the adress */
  virtual std::string addrToString () const;

  virtual bool operator == (const Address &oth) const;
  virtual bool operator != (const Address &oth) const;

  /** @return path name */
  std::string getPathName (void) const; 
    
  private:
    sockaddr_un unixAddr;
};

#endif // UNIX_ADDR_H
