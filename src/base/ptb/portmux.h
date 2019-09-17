// 
//  Copyright 1999
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
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

$Log: portmux.h,v $
Revision 1.4  2002/08/14 09:31:40  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 14:25:19  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:50  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:52  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:31  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef PORTMUX_H
#define PORTMUX_H

#include <doors/portset.h>


//
//Class: PortMux
//
//Description:
//   PortMux class is a multiplexer which can be used when
//   user wants to connect several ports to one or multiple
//   other ports. So, it is a generic multiplexer for 1-to-N
//   or N-to-N connections. PortMux object can be imagined
//   between the port pairs. Instead of connect port pair to
//   each other user connects one port to user end and other
//   port to provider end of multiplexer. Though PortMux
//   class defines user and provider ends it is completely
//   symmetrical regarding its behavior.
//   
//   PortMux class contains two instances of Propagator
//   classes that are given by constructor or init function.
//   By the propagators user can adjust behaviour of the
//   multiplexer. The PortMux class can be used in uni-,
//   multi-, and broadcast purposes as far as the user
//   initializes it with proper propagators and defines
//   comparing function used in port creation functions
//   working in that way.
//   

class PortMux {
  public:
    PortMux (void);
    PortMux (Propagator *u, Propagator *p);
    virtual ~PortMux (void);
    
    bool init (Propagator *u, Propagator *p);

    Port *getUser (void) const;
    Port *getProvider (void) const;

    Port *createUser (bool (*cmpFunc) (const EventTask *t, const Message *m) = 0);
    Port *createProvider (bool (*cmpFunc) (const EventTask *t, const Message *m) = 0);

    _MT_GETASYNCLOCK
    Propagator *getProviderPropagator(void);
    Propagator *getUserPropagator(void);

  protected:

  private:
    Propagator *users;
    Propagator *providers;
    _MT_ASYNCLOCK
};

#endif  // PORTMUX_H
