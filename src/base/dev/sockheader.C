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

$Log: sockheader.C,v $
Revision 1.2  2002/08/14 09:31:36  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:23  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:18  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/sockheader.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <doors/debug.h>
//
//Constructor: SockHeader
//
//Parameters: 
//
//Description: 
//  Initialize attributes.
//
SockHeader :: SockHeader (void) 
    : _headerSize(0)
{

  // from RFC 983 by Cass & Rose (1986) 
  isodePktHdr[0] = (SOCKHDR_LEN - 1);
  isodePktHdr[1] = (0);
  isodePktHdr[2] = (0);
  isodePktHdr[3] = (0);
}

//
//Destructor: SockHeader
//
//Description: 
//  Just to be.
//
SockHeader :: ~SockHeader (void) {

}

//
//Function: putHeader
//
//Member-Of: SockHeader
//
//Returns:
//
//Parameters:
//      Frame &frame
//
//Description:
//      Adds the 4 byte network header that Isode, and other OSI
//      hosts, expect to data frame.
//

void SockHeader :: putHeader (Frame &frame) {

  FrameSize len = 0;
  
  if (len = frame.length()) {
    // total length = packet header + frame length
    len += SOCKHDR_LEN;
    isodePktHdr[2] = (Byte) ((len >> 8) & 0xff);
    isodePktHdr[3] = (Byte) (len & 0xff);
    
    frame.putFirst (isodePktHdr, SOCKHDR_LEN);
  }
}

//
//Function: removeHeader
//
//Member-Of: SockHeader
//
//Returns:
//  Sint32
//
//Parameters:
//  int fd
// 
//Description:
//  This function removes the header from the beginning of the
//  data coming from socket and returns the length of expected user data.
//  If some errors are noticed, -1 is returned.
//
Sint32 SockHeader :: removeHeader (int fd) 
{
    int nbytes;
    if((nbytes = recv(fd, (char*)(isodePktHdr + _headerSize), 
		      SOCKHDR_LEN - _headerSize, 0)) < 0) {
	perror("get header");
	return -1;
    }
    if(nbytes == 0) {
	_DOORS_DEBUG(0,"read header fd:" << fd << " closed:");
	return -1;
    }
    _headerSize += nbytes;
    if(_headerSize < SOCKHDR_LEN) {
	return 0; // try again
    }
    // now we have a header
    _headerSize = 0; // zero for a next header
    if (isodePktHdr[0] != (SOCKHDR_LEN - 1) || isodePktHdr[1] != 0) {
	// Error in network packet !
	return -1;
    }
    // decode network packet
    return ((isodePktHdr[2] << 8) | isodePktHdr[3]) - SOCKHDR_LEN;
}







