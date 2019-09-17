
//  Copyright 2000 
//  	Telecoms Lab, Tampere University of Technology.  All rights reserved.

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

$Log: udp6app.h,v $
Revision 1.2  2003/05/10 18:22:54  bilhanan
Obsolete references to OVOPS removed.

Revision 1.1  2002/06/14 22:04:05  ik
Development. Initial commit for udp6 application.


*/

//Description:
//  This file declares a classes of AppSAP service
//  access point.
//


#ifndef _udp6app_h_
#define _udp6app_h_


#include <doors/ptb.h>
#include <doors/inetaddr6.h>



//
//Class: Udp6AppSAP
//
//Description:
//   Inside this class a service access point of
//   Udp6AppSAP defined.
//

class Udp6AppSAP {
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

    enum ReqType { start = 1 };

//
//Class: Start
//
//Inherited: Message
//
//Description:
//  This is one of the request primitives this
//  SAP defines.
//
    
class Start : public Message {
      public:
        Start (void);
        Start (const Start &msg);
        ~Start (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        InetAddr6 destaddr;
    };

    enum IndType { infomsg = 1 };

//
//Class: Infomsg
//
//Inherited: Message
//
//Description:
//  This is one of the request primitives this
//  SAP defines.
//
    
class Infomsg : public Message {
      public:
        Infomsg (void);
        Infomsg (const Infomsg &msg);
        ~Infomsg (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Frame info;
    };

  protected:
  private:
};


inline MessageType Udp6AppSAP :: Start :: getType (void) const {

  return start;
}


inline MessageType Udp6AppSAP :: Infomsg :: getType (void) const {

  return infomsg;
}




#endif
