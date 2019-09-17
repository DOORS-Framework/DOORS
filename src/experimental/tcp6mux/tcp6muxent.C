//Editor-Info: -*- C++ -*-

//  Copyright 2002 
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


//  $Log: tcp6muxent.C,v $
//  Revision 1.1  2004/08/18 06:22:16  bilhanan
//  TCP IPv6 multiplexer
//

#include <string>
#include <doors/util.h>
#include <doors/hsi.h>
#include "tcp6mux.h"
#include "tcp6muxent.h"
#include "tcp6muxconn.h"
#include "tcp6muxesm.h"
#include "tcp6muxcsm.h"

//
//Constructor: Tcp6muxEntity
//
//Parameters:
//      std::string n,
//      Scheduler *s,
//      IoHandler *ioh,
//      Address *a
//
//Description:
//  Initializes the base class and own variables. 
//
//  Parameter n is a name of the task, a is a address that server will start
//  to listen and hdr is header that is used for encoding and decoding of 
//  a header in a front of a socket stream.
//

Tcp6muxEntity :: Tcp6muxEntity( std::string n,
			 Scheduler *s,
			 IoHandler *io, 
			 Address *a, 
			 Tcp6muxEntitySM *entSM,
			 Uint32 idletimeout) // was == 300
    : PTask (n, s, entSM),  
      up (this),
      entity (this), 
      connSM (new Tcp6muxConnSM),
      device(io, this),
	  itimeout(idletimeout)
{
    STATIC_SYMBOL (up, this);
    entity.connect(&device.iFace);
    device.connect(a);
    STATIC_SYMBOL (entity, this);
    memset (connections, 0, sizeof (connections)); 
}

//
//Destructor: Tcp6muxEntity
//
//Description:
//  Delete symbols and the possible Tcp6muxConnection instances.
//

Tcp6muxEntity :: ~Tcp6muxEntity (void) {

  DELETE_SYMBOL (this);

  for (int i = 0; i < 256; i++) {
    if (connections [i] != 0) {
         delete connections [i];
      connections [i] = 0;
    }
  }
}

//
//Function: getConnection
//
//Member-Of: Tcp6muxEntity
//
//Returns: Tcp6muxConnection *
//
//Parameters: 
//      Uint32 i
//
//Description:
//  Returns the connection identified with the parameter i
//  which is same as connectios CEP number.
//
Tcp6muxConnection *Tcp6muxEntity :: getConnection (int i) {

  return connections [i];
}

//
//Function: ready_Creq
//
//Member-Of: Tcp6muxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in ready state and incoming
//  message is type of Tcp6muxSAP::Creq.
//
//  New connection is created if necessary and it is put temperarily 
//  into the TQueue list. The connection instance will be put later 
//  in the correct place into the connections table. Notice that also 
//  the message is saved for later use.
//
bool Tcp6muxEntity :: ready_Creq (Message *msg) {
	Tcp6muxSAP :: Creq *cr = (Tcp6muxSAP :: Creq *) msg;

	//check connection table here.

	// if connection exists, 
	// return the cep and data in a cconf

	if (conntable.count(cr->addr)){
	  // cout << "Connection exists, returning cep "<< conntable [cr->addr] << endl;
		Tcp6muxSAP :: Cconf *cc = (Tcp6muxSAP :: Cconf *) up.create (Tcp6muxSAP :: cconf);
		cc->cep = conntable [cr->addr];
		cc->addr = cr->addr;
		cc->ici = cr->ici;
		Message *bMsg = (Message *) cc;
		up.putMessage (bMsg);

		return true;
	}
	
	Tcp6muxConnection *conn = 
	  new Tcp6muxConnection (getIoHandler(), this, connSM, itimeout);
	
	msg->lock();
	conn->save (msg);
	return true;
}

//
//Function: ready_Cresp
//
//Member-Of: Tcp6muxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in ready state and incoming
//  message is type of Tcp6muxSAP::Cresp.
//
//  If the connection with required SAP is availeble the message
//  is only given to that connection to handle. Otherwise the 
//  message is ignored.
//
bool Tcp6muxEntity :: ready_Cresp (Message *msg) {
  Uint16 tempcep = 0;
  //Tcp6muxSAPBase *req = (Tcp6muxSAPBase *) msg;
  Tcp6muxSAP::Dtreq *dtreq = 0;
  Tcp6muxSAP::Cresp *cresp = 0;
  Tcp6muxSAP::Dreq *dreq = 0;

  if( (dtreq = dynamic_cast<Tcp6muxSAP::Dtreq*>(msg)) ) {
	  tempcep = dtreq->cep;
  } else if( (dreq = dynamic_cast<Tcp6muxSAP::Dreq*>(msg)) ) {
	  tempcep = dreq->cep;
  } else if( (cresp = dynamic_cast<Tcp6muxSAP::Cresp*>(msg)) ) {
	  tempcep = cresp->cep;
  }

  if (tempcep <256 && connections[tempcep] != 0) {
    msg->lock ();
    connections[tempcep]->save (msg);
  }
  return true;
}

//
//Function: ready_Dtreq
//
//Member-Of: Tcp6muxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in ready state and incoming
//  message is type of Tcp6muxSAP::Dtreq.
//
//  The functionality is the same as if incoming message was type of
//  Tcp6muxSAP::Cresp so that function can be called.
//
bool Tcp6muxEntity :: ready_Dtreq (Message *msg) {

  return ready_Cresp (msg);
}

//
//Function: ready_Dreq
//
//Member-Of: Tcp6muxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in ready state and incoming
//  message is type of Tcp6muxSAP::Dreq.
//
//  The functionality is the same as if incoming message was type of
//  Tcp6muxSAP::Cresp so that function can be called.
//
bool Tcp6muxEntity :: ready_Dreq (Message *msg) {

  return ready_Cresp (msg);
}

//
//Function: ready_default
//
//Member-Of: Tcp6muxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in ready state and incoming
//  message is coming from some connection so the message type is
//  DevIf.
//
//  The connections will send the entity either DevIf :: Open or 
//  DevIf :: Close messages so the entity must either add or remove
//  the connection instances from and to the table.
//
bool Tcp6muxEntity :: ready_default (Message *msg) {

  // Message is coming from some Tcp6muxConnection task

  switch (msg->getType ()) {
   case (DevIf :: open) : {   
     DevIf::Open *tmp = (DevIf::Open*) msg;
     
     if (connections [tmp->fd] == 0) {
       // everything is OK and instance is moved to the table
       connections [tmp->fd] = (Tcp6muxConnection *) tmp->device->getTask();
     } else {
       // error, this should never happen
     }
     break;
   }

   case (DevIf :: close) : { 
    
    DevIf :: Close *tmp = (DevIf :: Close *) msg;      
    if (tmp->fd > 0 && connections [tmp->fd]) {
      // normal closing of the connection
      DELETE_SYMBOL (connections [tmp->fd]);
      delete connections [tmp->fd];
      connections [tmp->fd] = 0;
    } else if (tmp->fd <0 ){
      // error in socket and file descriptor is no longer valid.
      // we must check every connection to find the one to be
      // deleted.
      for (int i = 0; i < 256; i++) {
	if (connections [i] == (tmp->device->getTask())) {
	  DELETE_SYMBOL (connections [i]);
	  delete connections [i];
	  connections [i] = 0;
	}
      }
    } else {
      Otask *t = tmp->device->getTask();
      if (t) {
	DELETE_SYMBOL (t);
	delete t;
      }
      break;
    }
   }
  }   
  return false;
}

//
//Function: ready_Data
//
//Member-Of: Tcp6muxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in ready state and incoming
//  message is coming from SocketEntity and the message type is
//  DevIf :: Read. In this case this combination will never occur.
//
bool Tcp6muxEntity :: ready_Data (Message *msg) {

  return false;
}

//
//Function: ready_Conn
//
//Member-Of: Tcp6muxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in ready state and incoming
//  message is coming from SocketEntity and the message type is
//  DevIf :: Open. 
//
//  New file descriptor is received and with it a new connection
//  is created. Because the correct place in the connections table
//  is known it is put in there and not in any temporary list.
//
bool Tcp6muxEntity :: ready_Conn (Message *msg) {

  DevIf::Open *tmp = (DevIf::Open *) msg;

  connections [tmp->fd] = new Tcp6muxConnection (getIoHandler(), tmp->fd, this, connSM, itimeout); 
  if (connections[tmp->fd] != 0) {
    Port *t = msg->getTarget();
    msg->mark (t, &connections[tmp->fd]->connection);
    msg->lock ();
    connections[tmp->fd]->save (msg);
  }

  return true;
}

//
//Function: ready_Disconn
//
//Member-Of: Tcp6muxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in ready state and incoming
//  message is coming from SocketEntity and the message type is
//  DevIf :: Close. 
//
//  This would mean that error has happened in the listening server socket.
//  In this implementation nothing is done about that.
//
bool Tcp6muxEntity :: ready_Disconn (Message *msg) {

  return false;
}








