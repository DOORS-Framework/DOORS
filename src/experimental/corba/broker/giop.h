//Editor-Info: -*- C++ -*-
//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
//
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

$Log: giop.h,v $
Revision 1.1.1.1  2002/05/13 14:38:35  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:31  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:40:18  bilhanan
Full licence clause added into doors 0.1


*/
//  =========================================================================
//  File:     giop.h
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================


#ifndef GIOP_H
#define GIOP_H


#include <doors/bastypes.h>


class Giop {
public:
  
  struct Version { 
    CORBA::Octet major; 
    CORBA::Octet minor; 
  };
  
  class MessageHeader  {
  public:
    CORBA::Octet magic[4];
    Version GIOP_version;
    CORBA::Boolean byte_order;
    CORBA::Octet message_type;
    CORBA::ULong message_size;
  };
  
  class IiopMsg  {
  public:
    MessageHeader msghdr;
  };

  
  class RequestHeader {
  public:
  };

  class Request : public IiopMsg {
  public:
  };
  
  class ReplyHeader {
  public:
  };

  class Reply: public IiopMsg  {
  public:
  };
  
  class CancelRequestHeader {
  public:
  };

  class CancelRequest : public IiopMsg  {
  public:
  };

  class LocateRequestHeader{
  public:
  };

  class LocateRequest : public IiopMsg {
  public:
  };
  
  class LocateReplyHeader {
  public:
  };

  class LocateReply : public IiopMsg{
  public:
  };
  
  class CloseConnection : public IiopMsg {
  public:
  };
  
  class MessageError : public IiopMsg {
  public:
  };
 
  
  inline void RemoveLeading(Frame& frm, CORBA::ULong firstSize) {
    if (frm.length() > 0) {
      CORBA::ULong byteCounter,len;
      byteCounter = * (CORBA::ULong*) frm.getFirst(sizeof(CORBA::ULong));
      /* here we could only deal with first parameter
	 of size 0 1 2 4 8 */
      len = byteCounter;
      if (firstSize == 0);
      else if (firstSize == 1);
      else if (firstSize == 2)
	len += len % 2;
      else if (firstSize == 4) {
	len += 3;
	len -= len % 4;
    }
      else if (firstSize == 8) {
	len += 7;
	len -= len % 8;
      }
      else
	throw "first Size must be 0, 1, 2, 4 or 8";
      frm.getFirst(len - byteCounter);
    }
  };
};

#endif
