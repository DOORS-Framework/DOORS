// -*- C++ -*-
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

$Log: iface.h,v $
Revision 1.2  2002/06/03 09:19:52  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:35  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:16  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef IFACE_H
#define IFACE_H

#include <doors/port.h>

/** Type of function to create a interface message. */
typedef Message *(*MessageCreateFunction) (void);

/**
  Iface class is a special class of Port. It provides
  exactly same service like a port but furthermore user
  can attach a set of messages the Iface is intended to
  send to. After instance construction of this class with
  proper array of message create functions, it's possible
  to create new messages for sending by calling a create
  function of this class with corresponding index.
  <p>
  Note: It's necessary for tools like user interfaces to
  know what type of messages certain interface is
  intended to send in order to retrieve that information
  to user. Thus, if you like to get that capability you
  must give that array of message create functions when
  calling a constructor. It's also possible to use this
   class without the array which will mean that the create
   function returns always null.
   */
class Iface : public Port {
public:

    /** Creates a new Iface instance by given parameters.
      @param host First parameter host is an "owner" of the Iface who
      will get the messages the Iface receives from its other
      end where it has been connected to.
      @param funcs Second parameter funcs is an array of message create
      functions the Iface needs to implement the create
      function. The array determines the messages the Iface
      is meant to deliver. If the second parameter is null
      the create function will return always null too.
      */
    Iface (EventTask *host, MessageCreateFunction *funcs);

    virtual ~Iface (void);

    /** By calling this function user can get a new instance of
      message wanted to send via this Iface. Naturally, this
      requires that user has constructed the Iface by proper
      array of message
      create functions.
      @param type The type number of message to create from defined
      interface message set.
      @return Returns a corresponding message create function of
      given parameter.
      */
    Message *create (int type);

private:
    MessageCreateFunction *functions;
};



#endif  // IFACE_H
