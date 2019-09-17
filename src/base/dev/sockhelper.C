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

$Log: sockhelper.C,v $
Revision 1.5  2004/01/19 11:12:31  bilhanan
#include fixed.

Revision 1.4  2002/05/12 20:00:58  ik
#define fixed.

Revision 1.3  2002/05/12 15:31:12  ik
Conditional lines for setsockopt and getpeername.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.3  2002/02/25 17:06:05  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.

Revision 1.2  2001/06/08 16:46:28  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:25  bilhanan
Full licence clause added into doors 0.1


*/

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <doors/util.h>

#include <doors/sockhelper.h>
#include <doors/unixaddr.h>
#include <doors/inetaddr.h>
#include <doors/sockmsg.h>
#include <doors/ocoremac.h>



extern "C" int shutdown (int, int);


//
//Constructor: SockHelper
//
//Description:
//  Initializes the base class and own variables. 
//

SockHelper :: SockHelper (void): fd (-1) {

}

//
//Destructor: SockHelper
//
//Description:
//  Just to be.
//
SockHelper :: ~SockHelper (void) {

}

//
//Function: setOptions
//
//Member-Of: SockHelper
//
//Parameters:
//      int option, 
//      void *optval, 
//      int optlen)
//Return:
//      int
//Description:
//  Function wrappes the system call setsockoption.
//
//  Parameter option is type of option to be changed, optval is value of 
//  that option and optlen is length of the value
//

int SockHelper ::  setOptions (int option, void *optval, int optlen) {

#if HAVE_INT_GETSOCKOPT
  int o = optlen;
#elif HAVE_SIZE_T_GETSOCKOPT
  size_t o = optlen;
#elif HAVE_SOCKLEN_T_GETSOCKOPT
  socklen_t o = optlen;
#else
#error "No known 4th parameter type for setsockopt"
#endif
  return ::setsockopt (fd, SOL_SOCKET, option, (char *) optval, o);
}

//
//Function: getOptions
//
//Member-Of: SockHelper
//
//Parameters:
//      int option, 
//      void *optval, 
//      int optlen)
//Return:
//      int
//Description:
//  Function wrappes the system call getsockoption.
//
//  Parameter option is type of option to be changed, optval is value of 
//  that option and optlen is length of the value
//
int SockHelper ::  getOptions (int option, void *optval, int *optlen) {
  
  int result = 0;
#if HAVE_INT_GETSOCKOPT
  result = ::getsockopt (fd, SOL_SOCKET, option,(char *) optval, (int *) optlen);
#elif HAVE_SIZE_T_GETSOCKOPT
  result = ::getsockopt (fd, SOL_SOCKET, option,(char *) optval, (size_t *) optlen);
#elif HAVE_SOCKLEN_T_GETSOCKOPT
  result = ::getsockopt (fd, SOL_SOCKET, option,(char *) optval, (socklen_t *) optlen);
#else
#error "No known 4th parameter type for setsockopt"
#endif

  return result;
}






