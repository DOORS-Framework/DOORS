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

$Log: sap.h,v $
Revision 1.4  2002/08/14 09:31:40  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 14:25:20  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:52  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:58  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:33  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef SAP_H
#define SAP_H

#include <doors/iface.h>
#include <doors/peer.h>

#define CLASS_IFACE 0

/**
  ReqIface class is a special class of Iface. It is meant
  to be used as a request side interface to entry service
  access point. For that reason it is normally used as a
  down interface of a protocol layer. It differs from
  normal Iface class in a way how the received messages
  (indications in this case) are handled. Once normal
  Iface class gives received messages to its host task
  this class delivers them to Peer object for decoding.
  */ 
class ReqIface : public Iface {
public:
    
    /** Creates a new instance of ReqIface with given parameters.
      @parm host First parameter host is an "owner" of the ReqIface.
      @param Second parameter funcs is an array of message create
      functions the Iface needs to implement its create
      function. The array determines the messages the Iface
      is meant to deliver. If the second parameter is null
      the create function will return always null too.
      @param peer Third parameter peer is a pointer to the Peer object of
      the host task. It is the object that will get all
      messages this ReqIface receives
      */
    ReqIface (EventTask *host, MessageCreateFunction *funcs, Peer *peer = 0);

    virtual ~ReqIface (void);

    /**
      When the ReqIface class receives an indication this
      function is called. If this object is attached to a
      Peer object (see the constructor),
      the function will deliver the indication to that Peer
      object for decoding it into proper PDU. Otherwise it
      acts like a getMessage function of the Iface class.
      @param msg Parameter msg is an indication message received from
      the lower layer.
      @return Returns zero if the msg has been received successfully.
      Otherwise the return value is nonzero.
      */
    virtual bool getMessage (Message *msg);

    /** Return true if this is acctached to coding peer implementation. */
    virtual inline bool hasDecoder (Message *msg);

protected:
    Peer *decoder;
};


#if CLASS_IFACE

/**
  IndIface class is a special class of Iface. It is meant
  to be used as an indication side interface to entry
  service access point. For that reason it is normally
  used as a upper interface of a protocol layer. IndIface
  class doesn't differ in any way from normal Iface class
  in the present PTB. However it is modeled as an own
  class to keep consistency with ReqIface class.
  */
class IndIface : public Iface {
public:

    /** Creates a new instance of IndIface with given
      parameters.
      @param host First parameter host is an "owner" of the IndIface.
      @param funcs Second parameter funcs is an array of message create
      functions the Iface needs to implement its create
      function. The array determines the messages the Iface
      is meant to deliver. If the second parameter is null
      the create function will return always null too.
      */
    IndIface (EventTask *host, MessageCreateFunction *funcs);

    virtual ~IndIface (void);
};

#else

typedef Iface IndIface;

#endif  // CLASS_IFACE

inline bool ReqIface :: hasDecoder (Message *msg) {

    _MT_FORWARDLOCK_ASMONITOR(mon, task);
    return (decoder) ? true : false;
}


#endif  // SAP_H
