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

$Log: sockmsg.h,v $
Revision 1.2  2002/06/03 09:19:48  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:01  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:25  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef SOCKMSG_H
#define SOCKMSG_H

#include <doors/util.h>
#include <doors/core.h>

class SocketDevice;

//
//Class: SocketMessage
//
//Inherited: Message
//
//Description:
// SocketMessage is inherited from Message and it is a abstract base 
// class for different messages that are used by socket module.

class SocketMessage: public Message {
 public:
  enum Type {Conn, Disconn, Data};

  SocketMessage (void);
  ~SocketMessage (void);

  virtual MessageType getType (void) const = 0;
};


//
//Class: SocketData
//
//Inherited: SocketMessage
//
//Description:
// SocketData is inherited from SocketMessage and it is used for 
// transferring data between SocketConnection and its controlling task.

class SocketData: public SocketMessage {
 public:
  SocketData (void);
  ~SocketData (void);

  MessageType getType (void) const;

  Frame data;
  SocketDevice *device;
};


//
//Class: SocketConn
//
//Inherited: SocketMessage
//
//Description:
// SocketConn is inherited from SocketMessage and it is used for informing 
// controlling task that incoming connection request has arrived.

class SocketConn: public SocketMessage {
 public:
  SocketConn (void);
  ~SocketConn (void);

  MessageType getType (void) const;

  int fd;
  SocketDevice *device;
};


//
//Class: SocketDisconn
//
//Inherited: SocketMessage
//
//Description:
// SocketDisconn is inherited from SocketMessage and it is used for informing 
// controlling task that connection has closed for some reason.

class SocketDisconn: public SocketMessage {
 public:
  SocketDisconn (void);
  ~SocketDisconn (void);

  MessageType getType (void) const;

  int fd;
  SocketDevice *device;
};

#endif
