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

$Log: portset.C,v $
Revision 1.5  2003/11/14 10:07:05  bilhanan
bugfix in getFirst(), checks added in singleprogator's propagate()

Revision 1.4  2003/10/15 10:49:50  bilhanan
singlepropagator's propogate able to distinguish between unicastand
broadcast message.

Revision 1.3  2002/07/23 14:25:19  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:50  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:33  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:31  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/portset.h>


//
//Constructor: MuxPort
//
//Parameters:
//   Propagator *m,
//   Propagator *o,
//   Boolean (*cmpFunc) (const EventTask *task, const Message *msg)
//
//Description:
//   Creates a new instance of MuxPort class.
//   
//   First parameter m is a message propagater where
//   this port will belong to.
//   
//   Second parameter o is another propagater where the
//   received messages will be propagated.
//   
//   Third parameter cmpFunc is a user defined comparing
//   function. It should return true or false concerning will
//   the message m be given to the task t or not. The message
//   m is coming from a provider end of the multiplexer. The
//   task t is a host of the other end of the port created.
//   

MuxPort :: MuxPort (Propagator *m, Propagator *o, 
		    bool (*cmpFunc) (const EventTask *task, const Message *msg))
: Port (0), master (m), others (o), acceptor (cmpFunc) {

}


//
//Destructor: MuxPort
//
//Description:
//   Deletes an instance of this class.
//   

MuxPort :: ~MuxPort (void) {

}


//
//Function: putMessage
//
//Member-Of: PortList
//
//Parameters:
//   Message *msg
//
//Return:
//   bool
//
//Description:
//   This function will send the message given as a parameter
//   to the other end of this port if the comparing function
//   attached to this class returns true.
//   
//   Parameter msg is a message to be sent.
//   
//   Returns zero if the msg was sent successfully. Otherwise
//   the value is non-zero.
//   

bool MuxPort :: putMessage (Message *msg) {

    bool rc = false;

    if (acceptor == 0 || acceptor (other->getTask (), msg)) {
	rc = Port :: putMessage (msg);
    }

    return rc;
}


//
//Function: getMessage
//
//Member-Of: PortList
//
//Parameters:
//   Message *msg
//
//Return:
//   bool
//
//Description:
//   This function calls the propagate function of the
//   another message propagator given as a second parameter
//   of the constructor.
//   
//   Parameter msg is a message to be sent.
//   
//   Returns a number of the ports the message was
//   propagated.
//   

bool MuxPort :: getMessage (Message *msg) {

    return (others->propagate (msg)) ? true : false;
}


//
//Function: disconnectRequest
//
//Member-Of: PortList
//
//Parameters:
//   Port *
//
//Return:
//   bool
//
//Description:
//   This function is called when the other end of this port
//   disconnects. It acts like disconnectRequest-function of
//   Port class but it also removes itself from the port set
//   of the propagator where it belongs to (first parameter
//   of the constructor).
//   
//   Parameter of this function is useless in this case.
//   
//   Returns zero if the msg was sent successfully. Otherwise
//   the value is non-zero.
//   

bool MuxPort :: disconnectRequest (Port *) {

    if (master && master->getPortSet ()) {
	master->getPortSet ()->remove (this);
    }

    Port :: disconnectRequest (0);
    delete this;
    return true;
}


//
//Constructor: PortList
//
//Description:
//   Creates a new instance of MuxList class.
//

PortList :: PortList (void) 
: iterator (&ports) {

}


//
//Function: add
//
//Member-Of: PortList
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

bool PortList :: add (MuxPort *p) {

    ports.append (p, false);
    return true;
}


//
//Function: remove
//
//Member-Of: PortList
//
//Parameters:
//   MuxPort *p
//Return:
//   MuxPort *
//
//Description:
//   Removes given multiplexer port from this port
//   set. Note that there is no implementation for this
//   function.
//   
//   Parameter p is a pointer to the multiplexer port will be
//   removed.
//   
//   Returns a pointer to the removed port if found from the
//   port set. Otherwise the return value is zero.
//   

MuxPort * PortList :: remove (MuxPort *p) {

    return ports.remove (p);
}


//
//Function: getFirst
//
//Member-Of: PortList
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

MuxPort * PortList :: getFirst (void) {

    iterator.reset ();

    return iterator.next ();
}


//
//Function: getNext
//
//Member-Of: PortList
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

MuxPort * PortList :: getNext (void) {

    return iterator.next ();
}


//
//Constructor: Propagator
//
//Parameters:
//   PortSet *ps
//
//Description:
//   Initializes data members of this base class.
//   
//   The parameter ps is pointer to the abstract port set
//   which contains the ports the propagator informs with the
//   message to be delivered. Due to the abstractness of port
//   set user can create propagators fith different type port
//   sets depending on the case.
//   

Propagator :: Propagator (PortSet *ps) : ports (ps) {

}


//
//Destructor: Propagator
//
//Description:
//   Deletes an instance of this class.
//   

Propagator :: ~Propagator (void) {

    delete ports;
}


//
//Function: getPortSet
//
//Member-Of: Propagator
//
//Return:
//   PortSet * 
//
//Description:
//   Returns a pointer to the abstract port set ot this 
//   class.
//   

PortSet * Propagator :: getPortSet (void) const {

    return ports; 
}


//
//Constructor: SinglePropagator
//
//Parameters:
//   PortSet *ps
//
//Description:
//   Creates a new instance of this class.
//   
//   The parameter ps is pointer to the abstract port set
//   which will be given to the base class.
//

SinglePropagator :: SinglePropagator (PortSet *ps) : Propagator (ps) {

}


//
//Function: propagate
//
//Member-Of: SinglePropagator
//
//Parameters:
//   Message *&msg
//Return:
//   bool
//
//Description:
//   This function is called by port multiplexer when it has
//   a message to propagate to the port set this propagator
//   holds.
//   
//   In this class case the implementation goes through the
//   port set tries put the message given as a parameter to
//   every port of the set. Once one port accepts the message
//   the execution is interrupted and the function returns.
//   
//   Parameter msg is a message to be propagated.
//   
//   Returns a number of ports the message is succesfully
//   propagated. In this case it can be zero or one.
//   

int SinglePropagator :: propagate (Message *&msg) {

    MuxPort *current = ports->getFirst ();
    int rc = 0;

    if (msg->isForMany()) {   // send to all
      msg->lock();
	while (current) {
	if (current->putMessage (msg) == true) {
	  msg->increment_rcpt_count();
	  rc++;
	}
	if (msg->isForMany())  // in case msg has somehow become unicast
	  current = ports->getNext ();
	else 
	  break;
	}
    }

    else {   // stop sending after the correct receipient
      while (current) {
	if (current->putMessage (msg) == true) {
	  rc = 1;
	  break;
	}
	current = ports->getNext ();
      }
    }
    return rc;
}


//
//Constructor: MultiPropagator
//
//Parameters:
//   PortSet *ps
//
//Description:
//   Creates a new instance of this class.
//   
//   The parameter ps is pointer to the abstract port set
//   which will be given to the base class.
//

MultiPropagator :: MultiPropagator (PortSet *ps) : Propagator (ps) {

}


//
//Function: propagate
//
//Member-Of: MultiPropagator
//
//Parameters:
//   Message *&msg
//Return:
//   bool
//
//Description:
//   This function is called by port multiplexer when it has
//   a message to propagate to the port set this propagator
//   holds.
//   
//   In this class case the implementation goes through the
//   port set tries put the message given as a parameter to
//   every port of the set. Once one port accepts the message
//   the execution is interrupted and the function returns.
//   
//   Parameter msg is a message to be propagated.
//   
//   Returns a number of ports the message is succesfully
//   propagated. In this case it can be zero or one.
//   

int MultiPropagator :: propagate (Message *&msg) {

    MuxPort *current = ports->getFirst ();
    int rc = 0;
    Message *clone = msg->clone ();

    while (current) {
	if (current->putMessage (clone) == true) {
	    clone = msg->clone ();
	    rc++;
	}
	current = ports->getNext ();
    }
    
    delete clone;
    return rc;
}



