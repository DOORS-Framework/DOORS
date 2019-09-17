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


//  $Log: tcpmuxent.C,v $
//  Revision 1.3  2005/03/02 14:32:58  bilhanan
//  development updates
//
//  Revision 1.2  2005/02/21 13:59:20  bilhanan
//  development
//
//  Revision 1.1  2004/10/12 12:01:22  bilhanan
//  The protocol formerly known as tranmux
//
//  Revision 1.10  2003/08/11 23:57:07  bilhanan
//  constructor code documentaton rectified: const char* -> std::string
//
//  Revision 1.9  2003/05/15 11:48:12  bilhanan
//  fixes for std c++ compliance
//
//  Revision 1.8  2002/09/13 12:56:50  kalliosa
//  fixed problem with default value in constructor
//
//  Revision 1.7  2002/08/19 08:11:16  hartmanj
//  Added missing comma
//
//  Revision 1.6  2002/08/16 09:55:58  hartmanj
//  Added idletimeout to constructor
//
//  Revision 1.5  2002/08/15 08:31:45  hartmanj
//  development
//
//  Revision 1.4  2002/08/08 13:42:04  hartmanj
//  development
//
//  Revision 1.3  2002/07/23 16:38:34  ik
//  Od{Boolean,True,False} -> {bool,true,false}
//
//  Revision 1.2  2002/07/23 07:30:44  bilhanan
//  licence added, include headers getting fixed..
//

#include <string>
#include <doors/util.h>
#include <doors/hsi.h>
#include <doors/tcpmux.h>
#include <doors/tcpmuxent.h>
#include <doors/tcpmuxconn.h>
#include <doors/tcpmuxesm.h>
#include <doors/tcpmuxcsm.h>

//
//Constructor: TcpmuxEntity
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
//  Parameter n is a name of the task, a is an address that server listens
//  to listen and entSM points to the EntityTask's statemachine.
//

TcpmuxEntity :: TcpmuxEntity( std::string n,
			 Scheduler *s,
			 IoHandler *io, 
			 Address *a, 
			 TcpmuxEntitySM *entSM)
    : PTask (n, s, entSM),  
      up (this),
      entity (this), 
      connSM (new TcpmuxConnSM),
      device(io, this),
      unknown (0)
{
    STATIC_SYMBOL (up, this);
    entity.connect(&device.iFace);
    if (a)
	device.connect(a);

    STATIC_SYMBOL (entity, this);
    memset (connections, 0, sizeof (connections)); 
}

//
//Destructor: TcpmuxEntity
//
//Description:
//  Delete symbols and the possible TcpmuxConnection instances.
//

TcpmuxEntity :: ~TcpmuxEntity (void) {

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
//Member-Of: TcpmuxEntity
//
//Returns: TcpmuxConnection *
//
//Parameters: 
//      Uint32 i
//
//Description:
//  Returns the connection identified with the parameter i
//  which is same as connection's CEP number.
//
TcpmuxConnection *TcpmuxEntity :: getConnection (int i) {

  return connections [i];
}

//
//Function: ready_Creq
//
//Member-Of: TcpmuxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function runs when task is in ready state and incoming
//  message is type of TcpmuxSAP::Creq.
//
//  If a connection to the requested endpoint exists, it is returned
//  immediately, giving the cep. Otherwise new connection is created 
//  and it is temporarily given a negative cep. The connection 
//  instance will be inserted later in the correct place into 
//  the connections array.
//

bool TcpmuxEntity :: ready_Creq (Message *msg) {
	TcpmuxSAP :: Creq *cr = (TcpmuxSAP :: Creq *) msg;

	// if found, return cep, increase muxcount

	if (activemap.count(cr->addr)){
	  TcpmuxSAP :: Cconf *cc = (TcpmuxSAP :: Cconf *) up.create (TcpmuxSAP :: cconf);
	  int conn_cep = activemap [cr->addr];
	  cc->cep = conn_cep;
	  cc->addr = cr->addr;
	  cc->ici = cr->ici;
	  connections[conn_cep]->increment_muxcount();
	  up.putMessage (cc);
	  return false;
	}

	// if not found
	TcpmuxConnection *conn = 
	  new TcpmuxConnection (getIoHandler(), --unknown, this, connSM);
	
	msg->lock();
	conn->save (msg);
	return true;
}

//
//Function: ready_Cresp
//
//Member-Of: TcpmuxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function runs when task is in ready state and incoming
//  message is type of TcpmuxSAP::Cresp.
//
//  If the connection with required SAP is available the message
//  is only given to that connection to handle. Otherwise the 
//  message is ignored.
//
bool TcpmuxEntity :: ready_Cresp (Message *msg) {
  Uint16 tempcep = 0;
  //Tcpmuxbase *req = (Tcpmuxbase *) msg;
  TcpmuxSAP::Dtreq *dtreq = 0;
  TcpmuxSAP::Cresp *cresp = 0;
  TcpmuxSAP::Dreq *dreq = 0;

  if( (dtreq = dynamic_cast<TcpmuxSAP::Dtreq*>(msg)) ) {
	  tempcep = dtreq->cep;
  } else if( (dreq = dynamic_cast<TcpmuxSAP::Dreq*>(msg)) ) {
	  tempcep = dreq->cep;
  } else if( (cresp = dynamic_cast<TcpmuxSAP::Cresp*>(msg)) ) {
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
//Member-Of: TcpmuxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function runs when task is in ready state and incoming
//  message is type of TcpmuxSAP::Dtreq.
//
//  The functionality is the same as if incoming message was type of
//  TcpmuxSAP::Cresp so that function can be called.
//
bool TcpmuxEntity :: ready_Dtreq (Message *msg) {

  return ready_Cresp (msg);
}

//
//Function: ready_Dreq
//
//Member-Of: TcpmuxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function runs when task is in ready state and incoming
//  message is type of TcpmuxSAP::Dreq.
//
//  The functionality is the same as if incoming message was type of
//  TcpmuxSAP::Cresp so that function can be called.
//
bool TcpmuxEntity :: ready_Dreq (Message *msg) {

  return ready_Cresp (msg);
}

//
//Function: ready_default
//
//Member-Of: TcpmuxEntity
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
//  message is coming from a connection task.
//
//  The connections will send the entity either DevIf :: Open or 
//  DevIf :: Close messages so the entity must either add or remove
//  the connection instances to and from the connections array. The
//  map containing the active connections is also updated.
//

bool TcpmuxEntity :: ready_default (Message *msg) {

  // Message is coming from some TcpmuxConnection task

  InetAddr connaddr;
  TcpmuxConnection *conntask;

  switch (msg->getType ()) {
   case (DevIf :: open) : {   

     DevIf::Open *tmp = (DevIf::Open*) msg;
     conntask = (TcpmuxConnection *) tmp->device->getTask();
     connaddr = conntask->myaddr;

     if (connections [tmp->fd] == 0) {
       // everything is OK and instance is moved to the table
       connections [tmp->fd] = conntask;
       // update active map
       if (!activemap.count(connaddr)){
	 activemap [connaddr] = tmp->fd;
       }  
     } else {
       // error, this should never happen
     }
     break;
   }

   case (DevIf :: close) : { 
    
    DevIf :: Close *tmp = (DevIf :: Close *) msg;      
     conntask = (TcpmuxConnection *) tmp->device->getTask();
     connaddr = conntask->myaddr;

     TcpmuxSAP :: Dind *di = (TcpmuxSAP :: Dind *) up.create (TcpmuxSAP :: dind);
     di->cep = tmp->fd;
     up.putMessage (di);

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

    // remove from map
    if (activemap.count(connaddr)){
       activemap.erase (connaddr);
    }
    
   }
  }   
  return false;
}

//
//Function: ready_Data
//
//Member-Of: TcpmuxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function runs when task is in ready state and incoming
//  message is coming from SocketEntity and the message type is
//  DevIf :: Read. In this case this combination will never occur.
//
bool TcpmuxEntity :: ready_Data (Message *msg) {

  return false;
}

//
//Function: ready_Conn
//
//Member-Of: TcpmuxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function runs when task is in ready state and incoming
//  message is coming from SocketEntity and the message type is
//  DevIf :: Open.
//
//  New file descriptor is received and with it a new connection
//  is created. Because the correct place in the connections array
//  is known it is directly inserted without any need to assign 
//  a temporary cep. The message is passed to connection task.
//
bool TcpmuxEntity :: ready_Conn (Message *msg) {

  DevIf::Open *tmp = (DevIf::Open *) msg;

  connections [tmp->fd] = new TcpmuxConnection (getIoHandler(), tmp->fd, this, connSM); 

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
//Member-Of: TcpmuxEntity
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function runs when task is in ready state and incoming
//  message is coming from SocketEntity and the message type is
//  DevIf :: Close. 
//
//  This would mean that an error has occured in the listening 
//  server socket. In this implementation nothing is done about 
//  that. Connection tasks behave normally and TcpMux acts only
//  as a client.

bool TcpmuxEntity :: ready_Disconn (Message *msg) {

  return false;
}








