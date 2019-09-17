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

$Log: unixaddr.C,v $
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

#include <string>
#include <doors/unixaddr.h>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>


//
//Constructor: UnixAddr
//
//Description:
//   Creates a new instance of unix address with zero value.
//   

UnixAddr :: UnixAddr (void): Address (AF_UNIX, sizeof unixAddr) {

    (void) ::memset ((void *) &unixAddr, 0, sizeof unixAddr);
}


//
//Constructor: UnixAddr
//
//Parameters:
//   const UnixAddr &oth
//
//Description:
//   Creates a new instance of unix address by the given
//   address.
//   
//   Parameter oth is other unix address to be cloned.
//   

UnixAddr :: UnixAddr (const UnixAddr &oth) : Address (AF_UNIX, oth.getSize ()) {

    size_t size = oth.getSize ();

    // Add one extra byte to account for the NUL at the end of the pathname.
    if (size < sizeof unixAddr) {
	size = oth.getSize () + 1;
    }

    unixAddr.sun_family = AF_UNIX;
    strcpy (unixAddr.sun_path, oth.unixAddr.sun_path);
}


void UnixAddr :: set (const sockaddr_un *un, int len) {

    (void) ::memset ((void *) &unixAddr, 0, sizeof unixAddr);
    unixAddr.sun_family = AF_UNIX;
    strcpy (unixAddr.sun_path, un->sun_path);
    baseSet (AF_UNIX, len);
}


//
//Constructor: UnixAddr
//
//Parameters:
//   const sockaddr_un *, int len
//
//Description:
//   Creates a new instance of unix address by the given
//   address value.
//   
//   Parameter sockaddr_un is the C-structure used to hold
//   address value.
//   
//   Parameter len is the length of the address value.
//   

UnixAddr :: UnixAddr (const sockaddr_un *un, int len) {

    set (un, len);
}


//
//Function: set
//
//Member-Of: UnixAddress
//
//Parameters:
//   std::string rendezvous_point
//
//Description:
//   Re-initializes this instance of unix address by the
//   given address value.
//   
//   Parameter rendezvous_point is the value of the address.
//   

void UnixAddr :: set (std::string rendezvous_point) {

    (void) ::memset ((void *) &unixAddr, 0, sizeof unixAddr);
    int len = rendezvous_point.length();

    unixAddr.sun_family = AF_UNIX;

    if (len >= sizeof unixAddr.sun_path) {
	/* At this point, things are screwed up, so let's make sure we 
	   don't crash. */
	(void) ::strncpy (unixAddr.sun_path, rendezvous_point.c_str(), sizeof unixAddr.sun_path);
	len = sizeof unixAddr.sun_path;
	unixAddr.sun_path[len - 1] = '\0';
	len -= 2; /* Don't count the NUL byte at the end of the string. */
    } else {
	(void) ::strcpy (unixAddr.sun_path, rendezvous_point.c_str());
    }

    Address :: baseSet (AF_UNIX, len + sizeof unixAddr.sun_family);
}


//
//Constructor: UnixAddr
//
//Parameters:
//   std::string rendezvous_point
//
//Description:
//   Creates a new instance of unix address by the given
//   address value.
//   
//   Parameter rendezvous_point is the value of the address.
//   

UnixAddr :: UnixAddr (std::string rendezvous_point) {

    set (rendezvous_point);
}


//
//Function: getAddr
//
//Member-Of: UnixAddr
//
//Return: void *
//
//Description:
//   Return address 
//   

void * UnixAddr :: getAddr (void) const {

    return (void *) &unixAddr;
}

//
//Function: addrToString
//
//Member-Of: UnixAddr
//
//Return: int
//
//Description:
//   Create string out of address.
//   

std::string UnixAddr :: addrToString () const {

    return unixAddr.sun_path;
}

//
//Function: operator ==
//
//Member-Of: UnixAddr
//
//Return: bool
//
//Parameters:
//        const Address &
//
//Description:
//   Overloaded compare function.
//   

bool UnixAddr :: operator == (const Address &oth) const {

    return ( ::strncmp (unixAddr.sun_path, 
			((const UnixAddr &) oth).unixAddr.sun_path,
			sizeof unixAddr.sun_path) == 0);
}

//
//Function: operator !=
//
//Member-Of: UnixAddr
//
//Return: bool
//
//Parameters:
//        const Address &
//
//Description:
//   Overloaded compare function.
//   

bool UnixAddr :: operator != (const Address &oth) const {

    return !((*this) == oth);
}

//
//Function: getPathName
//
//Member-Of: UnixAddr
//
//Return: std::string
//
//Description:
//   Return path name.
//   

std::string UnixAddr :: getPathName (void) const {

    return unixAddr.sun_path;
}

