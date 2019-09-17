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

$Log: sockhelper.h,v $
Revision 1.2  2004/01/14 13:54:15  bilhanan
#include fixed.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.3  2002/02/25 17:06:05  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.

Revision 1.2  2001/06/08 16:46:01  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:25  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef SOCKHELPER_H
#define SOCKHELPER_H

#include <doors/core.h>

#include <doors/unixaddr.h>
#include <doors/inetaddr.h>

// Berzerkeley Socket Stuff
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/stat.h>


/** Helper class for Operating Systems socket usage. */

class SockHelper {
public:
  /** Function wrappes the system call getsockoption */
  int getOptions (int option, void *optval, int *optlen);

  /** Function wrappes the system call setsockoption */
  int setOptions (int option, void *optval, int optlen);
  
protected:
  /** Initializes the base class and own variables.  */
  SockHelper (void);

  /** Empty destructor */
  ~SockHelper (void);
  
  int fd;
};

#endif // SOCKHELPER_H


