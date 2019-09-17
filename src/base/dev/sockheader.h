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

$Log: sockheader.h,v $
Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:46  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:19  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef SOCKHEADER_H
#define SOCKHEADER_H

#include <doors/util.h>
#include <doors/nethdr.h>

#define SOCKHDR_LEN   4

/** SockHeader is a header class provided by the module and it is used by
  default with streams if user does not provide one. It adds in the front of
  every 'packet' a 4 byte long header that includes the size of user data. 
  If peer entity uses header of the same format then this class can be used. 
  */
class SockHeader: public NetworkHeader {
 public:
  /** Initialize attribites */
  SockHeader (void);

  /** Destructor */
  ~SockHeader (void);

  /** Adds the 4 byte network header that Isode, and other OSI
    hosts, expect to data frame.
    */
  void putHeader (Frame &frm);

  /** This function removes the header from the beginning of the
    data coming from socket.
    @return The length of expected user data.
    If some errors are noticed, -1 is returned.*/
  Sint32 removeHeader (int fd);

 private:
  Byte isodePktHdr[SOCKHDR_LEN];
  int _headerSize;
};

#endif
