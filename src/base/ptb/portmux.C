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

$Log: portmux.C,v $
Revision 1.4  2002/08/14 09:31:40  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

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


#include <doors/portmux.h>


//
//Constructor: PortMux
//
//Description:
//   Creates a new instance of PortMux class with no
//   user of provider ports.
//   

PortMux :: PortMux (void) : users (0), providers (0) {
  
    _MT_ASYNCMONITOR;
}


//
//Constructor: PortMux
//
//Parameters:
//   Propagator *u,
//   Propagator *p
//
//Description:
//   Creates a new instance of PortMux class with no user of
//   provider ports.
//   
//   The parameter u is a propagator used in user end of the
//   multiplexer. It can be instance of SinglePropagator or
//   MultiPropagator.
//   
//   The parameter p is a propagator used in provider end of
//   the multiplexer. It can be instance of SinglePropagator
//   or MultiPropagator.
//   

PortMux :: PortMux (Propagator *u, Propagator *p) : users (u), providers (p) {

    _MT_ASYNCMONITOR;
}


//
//Destructor: PortMux
//
//Description:
//   Deletes this instance of PortMux class. Note that all
//   ports the instance contains will be destroyed.
//   

PortMux :: ~PortMux (void) {

    _MT_ASYNCMONITOR;
    MuxPort *p = 0;

    if (users) {	    
	while ((p = users->getPortSet ()->getFirst ()) != 0) {
	    users->getPortSet ()->remove (p);
	    delete p;
	}
	delete users;
    }

    if (providers) {	    
	while ((p = providers->getPortSet ()->getFirst ()) != 0) {
	    providers->getPortSet ()->remove (p);
	    delete p;
	}
	delete providers;
    }
}

//
//Function: init
//
//Member-Of: PortMux
//
//Parameters:
//   Propagator *u
//   Propagator *p
//
//Return:
//   bool
//
//Description:
//   Function initializes user and provider propagators.
//   

bool PortMux :: init (Propagator *u, Propagator *p) {
  
    _MT_ASYNCMONITOR;
    bool rc = true;

    if (users == 0) {
	users = u;
    } else {
	rc = false;
    }

    if (providers == 0) {
	providers = p;
    } else {
	rc = false;
    }

    return rc;
}


//
//Function: getUser
//
//Member-Of: PortMux
//
//Return:
//   Port *
//
//Description:
//   This function retrieves first port created on user end
//   of multiplexer.
//   
//   Returns a pointer to the first port created on user end.
//   

Port * PortMux :: getUser (void) const {
    
    _MT_ASYNCMONITOR;
    return users->getPortSet ()->getFirst ();
}


//
//Function: getProvider
//
//Member-Of: PortMux
//
//Return:
//   Port *
//
//Description:
//   This function retrieves first port created on provider
//   end of multiplexer.
//   
//   Returns a pointer to the first port created on provider
//   end.
//   

Port * PortMux :: getProvider (void) const {

    _MT_ASYNCMONITOR;
    return providers->getPortSet ()->getFirst ();
}

//
//Function: getProviderPropagator
//
//Member-Of: PortMux
//
//Return:
//   Propagator *
//
//Description:
//   This function returns provider propagator.
//   

Propagator *PortMux :: getProviderPropagator (void) {
  
    _MT_ASYNCMONITOR;
    return providers;
}

//
//Function: getUserPropagator
//
//Member-Of: PortMux
//
//Return:
//   Propagator *
//
//Description:
//   This function returns user propagator.
//   

Propagator *PortMux :: getUserPropagator (void) {
  
    _MT_ASYNCMONITOR;
    return users;
}


//
//Function: createUser
//
//Member-Of: PortMux
//
//Parameters:
//   Boolean (*cmpFunc) (const EventTask *t, const Message *m)
//Return:
//   Port *
//
//Description:
//   This function creates a new port to user end of
//   multiplexer. User can associate particular comparing
//   function to be used when determine if the task on the
//   other end of the port should get the message traveling
//   through the multiplexer.
//   
//   
//   The parameter cmpFunc is a user defined comparing
//   function. It should return true or false concerning will
//   the message m be given to the task t or not. The message
//   m is coming from a provider end of the multiplexer. The
//   task t is a host of the other end of the port created.
//   
//   The parameter is optional. If user did not give a it or
//   give zero, the message will always be sent by port
//   created.
//   
//   Returns a new port created in this function by given
//   comparing function.
//   

Port * PortMux :: createUser (bool (*cmpFunc) (const EventTask *t, const Message *d)) {

    _MT_ASYNCMONITOR;
    MuxPort *p = new MuxPort (users, providers, cmpFunc);
    users->getPortSet ()->add (p);

    return p;
}


//
//Function: createProvider
//
//Member-Of: PortMux
//
//Parameters:
//   Boolean (*cmpFunc) (const EventTask *t, const Message *m)
//Return:
//   Port *
//
//Description:
//   This function works exactly like createUser but for the
//   provider end of this multiplexer (see description of
//   createUser function of this class).
//   
//   The parameters and return value are the same as in
//   createUser function.
//   

Port * PortMux :: createProvider (bool (*cmpFunc) (const EventTask *t, const Message *d)) {

    _MT_ASYNCMONITOR;
    MuxPort *p = new MuxPort (providers, users, cmpFunc);
    providers->getPortSet ()->add (p);

    return p;
}

    
