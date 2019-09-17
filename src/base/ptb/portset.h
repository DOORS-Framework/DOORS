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

$Log: portset.h,v $
Revision 1.3  2002/07/23 14:25:20  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:50  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:52  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:32  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef PORTSET_H
#define PORTSET_H

#include <doors/core.h>

class Propagator;

//
//Class: MuxPort
//
//Description:
//   MuxPort is a specialized Port which is used in port sets
//   (PortSet) of port multiplexer (PortMux). It acts like a
//   Port but how it routes sent and received messages
//   differs from original Port class. In addition a
//   comparing function can be attached to the MuxPort to
//   determine whether the given message will be sent or not
//   through the port.
//   

class MuxPort : public Port {
  public:
    MuxPort (Propagator *m, Propagator *o, 
	     bool (*cmpFunc) (const EventTask *task, const Message *msg));
    virtual ~MuxPort (void);

    virtual bool putMessage (Message *msg);
    virtual bool getMessage (Message *msg);

    virtual bool disconnectRequest (Port *);

  private:
    Propagator *master;
    Propagator *others;
    bool (*acceptor) (const EventTask *task, const Message *msg);
};


//
//Class: PortSet
//
//Description:
//   This class an abstract base class which defines common
//   interface for all port sets used in message propagators.
//   By using this technique it's possible to change
//   implementations of port sets depending on the case.
//   
//   In the PTB there is one concrete implementation for port
//   sets defined by PortList class. Other implementations
//   are also possible for port vectors and trees for
//   instance if necessary.
//   

class PortSet {
  public:
    virtual bool add (MuxPort *p) = 0;
    virtual MuxPort *remove (MuxPort *p) = 0;
    virtual MuxPort *getFirst (void) = 0;
    virtual MuxPort *getNext (void) = 0;
};


//
//Function: add
//
//Member-Of: PortSet
//
//Parameters:
//   MuxPort *p
//Return:
//   bool
//
//Description:
//   Adds a new port to the port set. Note that there is no
//   implementation for this function.
//   
//   Parameter p is a pointer to the multiplexer port will be
//   added.
//   
//   Returns true if the p was added successfully. Otherwise
//   the return value is false.
//   

//
//Function: remove
//
//Member-Of: PortSet
//
//Parameters:
//   MuxPort *p
//Return:
//   MuxPort *
//
//Description:
//   Removes given multiplexer port from this port set. Note
//   that there is no implementation for this function.
//   
//   Parameter p is a pointer to the multiplexer port will be
//   removed.
//   
//   Returns a pointer to the removed port if found from the
//   port set. Otherwise the return value is zero.
//   

//
//Function: getFirst
//
//Member-Of: PortSet
//
//Return:
//   MuxPort *
//
//Description:
//   This function retrieves first multiplexer port in the
//   port set. Note that there is no implementation for this
//   function.
//   
//   Returns a pointer to the first port in the port set.
//   

//
//Function: getNext
//
//Member-Of: PortSet
//
//Return:
//   MuxPort *
//
//Description:
//   This function retrieves next multiplexer port in the
//   port set. Note that there is no implementation for this
//   function.
//   
//   Returns a pointer to the next port in the port set. If
//   the end of the list is reached the zero value is
//   returned once and after that the list is started at the
//   beginning.
//   


//
//Class: PortList
//
//Description:
//   This class is a concrete implementation for PortSet base
//   class. It defines a port set where the pors are stored
//   in dynamic list.
//   

class PortList : public PortSet {
  public:
    PortList (void);

    bool add (MuxPort *p);
    MuxPort *remove (MuxPort *p);
    MuxPort *getFirst (void);
    MuxPort *getNext (void);

  private:
    TQueue<MuxPort> ports;
    TQueueListIterator<MuxPort> iterator;
};


//
//Class: Propagator
//
//Description:
//   This class is an abstract base class for all propagator
//   types the PortMux class (port multiplexer) uses for its
//   user and provider ends. The purpose of this class is to
//   bind a propagate function for common method to deliver a
//   message to the port set the propagator knows.
//   
//   There are two concrete propagator classes defined in
//   PTB: SinglePropagator for implementing unicast and
//   MultiPropagator for multi- and broadcast purposes. Other
//   propagator types are also possible to use if somebody
//   sees it useful.
//   
//   Deletes this instance. Note that the port set
//   is destroyed as well.
//   

class Propagator {
  public:
    Propagator (PortSet *ps);
    virtual ~Propagator (void);

    virtual int propagate (Message *&msg) = 0;
    PortSet *getPortSet (void) const;

  protected:
    PortSet *ports;
};


//
//Function: propagate
//
//Member-Of: Propagator
//
//Parameters:
//   Message *&msg
//Return:
//   int
//
//Description:
//   This function is called by port multiplexer when it has
//   a message to propagate to the port set this propagator
//   holds.
//   
//   Since this class is abstract there is no implementation
//   for this function in this class but in derived classes
//   (SinglePropagator and MultiPropagator) required actions
//   will take place.
//   
//   Parameter msg is a message to be propagated.
//   
//   Returns a number of ports the message is succesfully
//   propagated.
//   


//
//Class: SinglePropagator
//
//Description:
//   This class implements a concrete message propagator
//   which delivers message for first port of the port set
//   that accepts it. On the other words it can be used to
//   accomplish message unicasting. The class is inherited
//   from the common Propagator base class used for
//   propagators.

class SinglePropagator : public Propagator {
  public:
    SinglePropagator (PortSet *ps);

    virtual int propagate (Message *&msg);
};


//
//Class: MultiPropagator
//
//Description:
//   This class implements a concrete message propagator
//   which delivers message for all ports of the port set
//   that accepts the message. On the other words it can be
//   used to accomplish message multi or broadcasting.
//   Whether the case is multi- or broadcast this class can't
//   determine. It depends on the ports of the port set will
//   their comparing function accept the message or not.
//   
//   The MultiPropagator class is inherited from the common
//   Propagator base class used for propagators.
//   
//   Creates a new instance of this class.
//   
//   The parameter ps is pointer to the abstract port set
//   which will be given to the base class.
//   

class MultiPropagator : public Propagator {
  public:
    MultiPropagator (PortSet *ps);

    virtual int propagate (Message *&msg);
};


#endif  // PORTSET_H
