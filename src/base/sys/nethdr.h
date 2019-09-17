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

$Log: nethdr.h,v $
Revision 1.2  2002/06/03 09:19:53  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:46  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:18  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef NETHDR_H
#define NETHDR_H

#include <doors/util.h>


/** Networkheader is a abstract class that defines a virtual putHeader and 
  removeHeader functions. It defines a interface for manipulating headers 
  of socket streams. It provides a possibility for user to create own header 
  class that uses a different kind of header in the front of the stream.
  */
class NetworkHeader {
 public:
  /** destructor */
  virtual ~NetworkHeader (void);
  /** PutHeader adds the header in front of package.
   @param frm the frame including the header.*/
  virtual void putHeader (Frame &frm) = 0;

  /** removes the header from the incoming stream. 
   @param fd The filedescriptor of the socket
   @return the length of expectd user data.*/

  virtual Sint32 removeHeader (int fd) = 0;

 protected:
  /** Constructor */
  NetworkHeader (void);
};


#endif
