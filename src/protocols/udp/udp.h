
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
 
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

$Log: udp.h,v $
Revision 1.2  2003/05/10 18:22:55  bilhanan
Obsolete references to OVOPS removed.

Revision 1.1.1.1  2002/04/30 12:32:50  bilhanan
Development

Revision 1.3  2002/02/25 17:06:05  ik
sockaddr.* split to inetaddr.* and unixaddr.* and the depending
files changed accordingly.

Revision 1.2  2001/06/08 16:46:06  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:28  bilhanan
Full licence clause added into doors 0.1


*/

 

//Description:
//  This file declares a classes of UdpSAP service
//  access point.
//


#ifndef _udp_h_
#define _udp_h_


#include <doors/ptb.h>
#include <doors/inetaddr.h>



//
//Class: UdpSAP
//
//Description:
//   Inside this class a service access point of
//   UdpSAP defined.
//

class UdpSAP {
  public:

//
//Class: User
//
//Inherited: ReqIface
//
//Description:
//   This is a service user's interface which can
//   be used to send request primitives of this SAP.
//

    class User : public ReqIface {
      public:
        User (EventTask *t, Peer *p = 0);
        ~User (void);
    };


//
//Class: Provider
//
//Inherited: IndIface
//
//Description:
//   This is a service provider's interface which can
//   be used to send indication primitives of this SAP.
//

    class Provider : public IndIface {
      public:
        Provider (EventTask *t);
        ~Provider (void);
    };

    enum ReqType { dtreq = 1, setup };

//
//Class: Dtreq
//
//Inherited: Message
//
//Description:
//  This is one of the request primitives this
//  SAP defines.
//
    
class Dtreq : public Message {
      public:
        Dtreq (void);
        Dtreq (const Dtreq &msg);
        ~Dtreq (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        InetAddr dest;
        Frame data;
    };

//
//Class: Setup
//
//Inherited: Message
//
//Description:
//  This is one of the request primitives this
//  SAP defines.
//
    
class Setup : public Message {
      public:
        Setup (void);
        Setup (const Setup &msg);
        ~Setup (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        InetAddr ownIp;
    };

    enum IndType { dtind = 1, errind };

//
//Class: Dtind
//
//Inherited: Message
//
//Description:
//  This is one of the request primitives this
//  SAP defines.
//
    
class Dtind : public Message {
      public:
        Dtind (void);
        Dtind (const Dtind &msg);
        ~Dtind (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        InetAddr source;
        InetAddr dest;
        Frame data;
    };

//
//Class: Errind
//
//Inherited: Message
//
//Description:
//  This is one of the request primitives this
//  SAP defines.
//
    
class Errind : public Message {
      public:
        Errind (void);
        Errind (const Errind &msg);
        ~Errind (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 code;
        Frame data;
    };

  protected:
  private:
};


inline MessageType UdpSAP :: Dtreq :: getType (void) const {

  return dtreq;
}


inline MessageType UdpSAP :: Setup :: getType (void) const {

  return setup;
}


inline MessageType UdpSAP :: Dtind :: getType (void) const {

  return dtind;
}


inline MessageType UdpSAP :: Errind :: getType (void) const {

  return errind;
}




#endif
