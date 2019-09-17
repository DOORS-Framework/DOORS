//  Copyright 2004
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

$Log: mobilitysap.h,v $
Revision 1.2  2004/05/19 13:55:01  hartmanj
License and log tag added



  
*/

#ifndef _mobilitysap_h_
#define _mobilitysap_h_


#include <doors/ptb.h>

class Report;
class SrcDesc;

class MobilitySAP
{
  public:

    class User : public ReqIface
    {
      public:
        User(EventTask *t, Peer *p=0);
        ~User(void);
        bool notify();
        bool forget();
    };

    class Provider : public IndIface
    {
      public:
        Provider(EventTask *t);
        ~Provider(void);
    };

    enum ReqType { movement = 1 };

    class Movement : public Message
    {
      public:
        Movement (void);
        ~Movement (void);

        Movement (const Movement &msg);
        Message *clone (void) const;

        MessageType getType (void) const;

        InetAddr6 coa;
        InetAddr6 ha;
    };

  protected:
  private:
};

inline MessageType MobilitySAP :: Movement :: getType (void) const
{
  return movement;
}



#endif
