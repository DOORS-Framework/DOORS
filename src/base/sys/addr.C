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

$Log: addr.C,v $
Revision 1.4  2005/03/18 15:52:15  bilhanan
comparison checks now return bool instead of int.

Revision 1.3  2002/07/24 14:57:00  ik
Use std::string.

Revision 1.2  2002/06/03 09:19:52  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:18  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:10  bilhanan
Full licence clause added into doors 0.1


*/

#include <string>
#include <doors/addr.h>


//
//Function: baseSet
//
//Member-Of: Address
//
//Parameters:
//   int type,
//   int size
//
//Description:
//   Converts address information to the human readable
//   string format.
//   
//   Parameter addr is a pointer to the octet string where
//   the address conversion can be copied.
//   
//   Parameter s is the maximum length of the octet string to
//   be copied.
//   

void Address :: baseSet (int type, int size) {

  addrType = type;
  addrSize = size;
}


//
//Constructor: Address
//
//Description:
//   Initializes data members of this base class with zero
//   values.
//   

Address :: Address (void) {

    baseSet (0, 0);  // Note that 0 is an unspecified protocol family type... 
}


//
//Constructor: Address
//
//Parameters:
//   int type,
//   int size
//Description:
//   Initializes data members of this base class with given
//   values.
//   
//   Parameter type is a type of the address. At the moment
//   there are two values reserved for type identification
//   defined in unix system libraries: AF_INET for
//   InetAddress class and AF_UNIX for UnixAddress class.
//   
//   Parameter size is relates a size of the address in
//   bytes.
//   

Address :: Address (int type, int size) {

    baseSet (type, size);
}


//
//Function: getAddr
//
//Member-Of: Address
//
//Return:
//   void *
//
//Description:
//   Gives an untyped pointer to the address information
//   stored by a derived class. In this base class case
//   the function returns zero.
//   

void * Address :: getAddr (void) const {

    return 0;
}


//
//Function: addrToString
//
//Member-Of: Address
//
//Parameters:
//   char addr[],
//   size_t s
//
//Return:
//   void *
//
//Description:
//   Converts address information to the human readable
//   string format.
//   
//   Parameter addr is a pointer to the octet string where
//   the address conversion can be copied.
//   
//   Parameter s is the maximum length of the octet string to
//   be copied.
//   
//   Returns non-zero value if the address is successfully
//   copied. Otherwise the return value is zero.
//   

std::string Address :: addrToString () const {

    return "";
}


//
//Function: operator ==
//
//Member-Of: Address
//
//Parameters:
//   const Address &oth
//
//Return:
//   bool
//
//Description:
//   Returns true if the address given as a
//   parameter is identical with this address. Otherwise the
//   return value is false.
//   

bool Address :: operator == (const Address &sap) const {
 
    return sap.getType () == 0;
}


//
//Function: operator !=
//
//Member-Of: Address
//
//Parameters:
//   const Address &oth
//
//Return:
//   bool
//
//Description:
//   Returns true if the address given as a parameter
//   is identical with this address. Otherwise the return
//   value is false.
//   

bool Address :: operator != (const Address &sap) const {

    return sap.getType () != 0;
}


//
//Function: getSize
//
//Member-Of: Address
//
//Return:
//   int
//
//Description:
//   Returns a size of the address in bytes.
//   

int Address :: getSize (void) const {

  return addrSize;
}


//
//Function: setSize
//
//Member-Of: Address
//
//Parameters:
//   int size
//
//Description:
//   Re-initializes the size of this address.
//   
//   Parameter size is a new size of this address.
//   

void Address :: setSize (int size) {

  addrSize = size;
}


//
//Function: getType
//
//Member-Of: Address
//
//Return:
//   int
//
//Description:
//   Returns an integer which corresponds to a type of the
//   address.
//   

int Address :: getType (void) const {

  return addrType;
}


//
//Function: setType
//
//Member-Of: Address
//
//Parameters:
//   int type
//
//Description:
//   Re-initializes the type of this address.
//   
//   Parameter type is a new type of this address.
//   

void Address :: setType (int type) {

  addrType = type;
}
