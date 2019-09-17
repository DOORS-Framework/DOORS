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

$Log: addr.h,v $
Revision 1.4  2005/03/18 15:52:15  bilhanan
comparison checks now return bool instead of int.

Revision 1.3  2004/01/19 10:35:38  bilhanan
#include fixed.

Revision 1.2  2002/07/24 14:57:00  ik
Use std::string.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:22  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:10  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef ADDR_H
#define ADDR_H

#include <doors/util.h>


/**
   Address class defines common interface for different type
   of addresses used in application. Usually,  the
   addresses are unique for every application area which
   means that user probably need to define a special class
   to carry address information. In that case the user
   defined address should be inherited from this class if
   possible to achieve a standard basic interface for
   address handling in DOORS.<p>
   
   In PTB there are two concrete address types defined:
   UnixAddress for socket addresses in same host and
   InetAddress for internet addressing. Both classes
   derives this abstract base class.<p>

   */
   

class Address {
public:
  /**
    Initializes data members of this base class with zero
    values.
    */
  Address (void);
  /**
    Initializes data members of this base class with given
    values.
    @param type is a type of the address. At the moment
    there are two values reserved for type identification
    defined in unix system libraries: AF_INET for
    InetAddress class and AF_UNIX for UnixAddress class.
    @param size is relates a size of the address in
    bytes.
    */
  Address (int type, int size);
  /**
    @return size of the address in bytes
    */
  int getSize (void) const;
  /**
    Re-initializes the size of this address.
    @param size is a new size of this address.
    */
  void setSize (int size);
  /**
  @return Integer corresponding to a type of the address.
  */
  int getType (void) const;
  /**
  Re-initializes the type of this address.
  @param type is a new type of this address.
  */
  void setType (int type);
  /**  
    Gives an untyped pointer to the address information
    stored by a derived class. In this base class case
    the function returns zero.
   */
  virtual void *getAddr (void) const;
  /**
    Converts address information to the human readable
    string format.
    @return non-zero length string if the address is successfully
    copied.
    */
  virtual std::string addrToString () const;
  /**
    Compares address given as a parameter to this address.
    @return non-zero value if the address given as a
    parameter is identical with this address. Otherwise the
    return value is zero.
    */
  virtual bool operator == (const Address &oth) const;
  /**
    Compares address given as a parameter to this address.
    @return nzero value if the address given as a
    parameter is identical with this address. Otherwise the
    return value is non-zero.
    */
  virtual bool operator != (const Address &oth) const;
  /**Converts address information to the human readable
    string format.  
    @param addr is a pointer to the octet string where
    the address conversion can be copied.  
    @param size is the maximum length of the octet string to
    be copied.
    */
  void baseSet (int type, int size);

protected:
  int addrType; // e.g. AF_UNIX or AF_INET. 
  int addrSize; // Number of bytes in the address.
};

#endif // ADDR_H
