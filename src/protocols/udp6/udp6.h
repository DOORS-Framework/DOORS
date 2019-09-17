
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

$Log: udp6.h,v $
Revision 1.5  2003/08/11 12:24:48  bilhanan
bugfix: header guard name was previously the same as udp.h

Revision 1.4  2003/05/10 18:22:55  bilhanan
Obsolete references to OVOPS removed.

Revision 1.3  2002/08/20 12:32:53  kalliosa
Added JoinMcast to Udp6Sap

Revision 1.2  2002/06/06 21:53:14  ik
Fixes to IPv6 UDP.

Revision 1.1  2002/06/06 12:52:55  ik
Initial files for IPv6 UdpTask.


*/

 

//Description:
//  This file declares a classes of UdpSAP service
//  access point.
//


#ifndef _udp6_h_
#define _udp6_h_


#include <doors/ptb.h>
#include <doors/inetaddr6.h>



//
//Class: Udp6SAP
//
//Description:
//   Inside this class a service access point of
//   Udp6SAP defined.
//

class Udp6SAP {
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

    enum ReqType { dtreq = 1, setup, joinmcast, leavemcast };

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

        InetAddr6 dest;
        Frame data;
    };
//
//Class: JoinMcast
//
//Inherited: Message
//
//Description:
//  This is one of the request primitives this
//  SAP defines. 
//
    
class JoinMcast : public Message {
      public:
        JoinMcast (void);
        JoinMcast (const JoinMcast &msg);
        ~JoinMcast (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        InetAddr6 addr;
    };

//
//Class: LeaveMcast
//
//Inherited: Message
//
//Description:
//  This is one of the request primitives this
//  SAP defines.
//
    
class LeaveMcast : public Message {
      public:
        LeaveMcast (void);
        LeaveMcast (const LeaveMcast &msg);
        ~LeaveMcast (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        InetAddr6 addr;
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

        InetAddr6 ownIp;
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

        InetAddr6 source;
        InetAddr6 dest;
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


inline MessageType Udp6SAP :: Dtreq :: getType (void) const {

  return dtreq;
}

inline MessageType Udp6SAP :: JoinMcast :: getType (void) const {

  return joinmcast;
}
inline MessageType Udp6SAP :: LeaveMcast :: getType (void) const {

  return leavemcast;
}


inline MessageType Udp6SAP :: Setup :: getType (void) const {

  return setup;
}


inline MessageType Udp6SAP :: Dtind :: getType (void) const {

  return dtind;
}


inline MessageType Udp6SAP :: Errind :: getType (void) const {

  return errind;
}




#endif
