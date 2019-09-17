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


//  $Log: tcp6muxconn.C,v $
//  Revision 1.1  2004/08/18 06:22:16  bilhanan
//  TCP IPv6 multiplexer
//

#include <map>
#include <doors/hsi.h>
#include <doors/util.h>
#include <doors/core.h>
#include "tcp6muxconn.h"
#include "tcp6muxcsm.h"

//
//Constructor: Tcp6muxConnection
//
//Parameters:
//      IoHandler *ioh,
//      Uint16 id,
//      Tcp6muxEnt *e,
//      Tcp6muxConnSM *csm 
//
//Description:
//  Initializes the base class and own variables. 
//
//  Parameter id is a file descriptor of excisting 
//  socket connection, e is a pointer to controlling task, csm is pointer to
//  state automaton and hdr is header that is used for encoding and decoding
//  of a header in a front of a socket stream.
//
//  This constructor is used when connection is established by server.
//

Tcp6muxConnection :: Tcp6muxConnection (IoHandler *io, int id, Tcp6muxEnt *e,

Tcp6muxConnSM *csm, Uint32 idletimeout): 
PTask ("", e->getScheduler (), csm), 
connection (this),
device(io, this),
entity (e),
respTimer (this, OTime (60)),
idleTimer (this, OTime (idletimeout)),
cep (id),
up (e->up)
{

  char n[40];
  snprintf (n, 40, "%s.conn.%d", entity->getName().c_str(), cep); 
  name = (n);

  connection.connect(&device.iFace);
  device.connect(id);
  DYNAMIC_SYMBOL (this, n, e);
  STATIC_SYMBOL (connection, this);
  STATIC_SYMBOL (cep, this);
  STATIC_SYMBOL (respTimer, this);
  _MT_SCHEDULE (&device);
}

//
//Constructor: Tcp6muxConnection
//
//Parameters:
//      IoHandler *ioh,
//      TranEnt *e,
//      TranConnSM *csm 
//
//Description:
//  Initializes the base class and own variables. 
//
//  Parameter e is a pointer to controlling task, csm is pointer to 
//  state automaton and hdr is header that is used for encoding and
//  decoding of a header in a front of a socket stream.
//
//  This constructor is used when instance is created before address 
//  of the peer entity is known. The connection is done later with 
//  connect function.
//
//

Tcp6muxConnection :: Tcp6muxConnection (IoHandler *io, Tcp6muxEnt *e,

Tcp6muxConnSM *csm, Uint32 idletimeout): // was idletimeout=300
PTask ("", e->getScheduler (), csm), 
connection (this),
device ( io, this),
entity (e),
respTimer (this, OTime (60)),
idleTimer (this, OTime (idletimeout)),
cep (-1),
up (e->up)
{

  char n[40];
  connection.connect(&device.iFace);
  cep = device.getFd();
  snprintf (n, 40, "%s.conn.%d", entity->getName().c_str(), cep); 
  name = (n);

  DYNAMIC_SYMBOL (this, n, e);
  STATIC_SYMBOL (connection, this);
  STATIC_SYMBOL (cep, this);
  STATIC_SYMBOL (respTimer, this);
}

//
//Destructor: Tcp6muxConnection
//
//Description:
//  Delete symbols.
//

Tcp6muxConnection :: ~Tcp6muxConnection (void) {
    
  DELETE_SYMBOL (this);
  device.close();
}

//
//Function: closed_Creq
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in closed state and incoming
//  message is type of Tcp6muxSAP::Creq.
//
//  There should be no data included in the Creq primitive

bool Tcp6muxConnection :: closed_Creq (Message *msg) {

  Tcp6muxSAP :: Creq *cr = (Tcp6muxSAP :: Creq *) msg;
  reqici = cr->ici;
  respTimer.start ();

  device.connect(&(cr->addr));
  device.readBytes(StreamDevice :: immediate);

  state = Tcp6muxConnSM :: wfSopen;
  return false;
}

//Function: closed_Conn
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in closed state and incoming
//  message is type of DevIf :: Open.
//
//  Connect indication is sent up and state isupdated to wfCresp.
//

bool Tcp6muxConnection :: closed_Conn (Message *msg) {


  Tcp6muxSAP :: Cind *ci = (Tcp6muxSAP :: Cind *) up.create (Tcp6muxSAP :: cind);
  myaddr =  *(InetAddr6 *)device.getRemoteAddress();
  ci->cep = cep;
  ci->addr = myaddr;
  up.putMessage(ci);

  state = Tcp6muxConnSM :: wfCresp;
  return false;
}



//
//Function: closed_default
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in closed state and source
//  of the message is not known.
//
//  In this implementation nothing is done.
//
bool Tcp6muxConnection :: closed_default (Message *msg) {

  return false;
}


//
//Function: wfSopen_conn
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in wfSopen state and incoming
//  message is type of DevIf :: Open and state ischanged to open.
//
//  The message is also given to the entity for registration purpose.

bool Tcp6muxConnection :: wfSopen_Conn (Message *msg) {

  char buf[40];
  DevIf :: Open *tmp = (DevIf :: Open *) msg;    

  cep = tmp->fd;
  myaddr =  * (InetAddr6 *)device.getRemoteAddress();
  snprintf (buf, 40, "%s.conn.%d", getName ().c_str(), cep);
  name = (buf);
  Tcp6muxSAP :: Cconf *cc = (Tcp6muxSAP :: Cconf *) up.create (Tcp6muxSAP :: cconf);
  cc->cep = cep;
  cc->addr = myaddr;
  cc->ici = reqici; // return the data passed during Creq
  Message *bMsg = (Message *) cc;
  up.putMessage (bMsg);
  
   if (!entity->conntable.count(myaddr)){
     //     std::cout << "populating conntable " << myaddr.getHostAddr()<< ":" << myaddr.getPortNumber() << " , cep "<< cep <<std::endl; 
     entity->conntable [myaddr] = cep;


   }
  msg->lock();
  entity->save (msg);

  respTimer.stop ();
  idleTimer.start ();

  //  std::cout << "Idle timer started. " << std::endl;
  state = Tcp6muxConnSM :: open;

  return true;
}

//
//Function: wfSopen_Disconn
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in wfSopen state and incoming
//  message is type of DevIf :: Close.
//
//  Tcp6muxSAP::Dind message is sent to upper layer and state ischanged 
//  to closed.
//  

bool Tcp6muxConnection :: wfSopen_Disconn (Message *msg) {

  DevIf :: Close *sDisc = (DevIf :: Close *) msg;

  Tcp6muxSAP :: Dind *di = (Tcp6muxSAP :: Dind *) up.create (Tcp6muxSAP :: dind);
  di->cep = cep;
  up.putMessage (di);

  msg->lock ();
  entity->save (msg);

  if (entity->conntable.count(myaddr)){
    // std::cout << "unpopulating conntable " << myaddr.getHostAddr()<< ":" << myaddr.getPortNumber() << " , cep "<< entity->conntable [myaddr]<<std::endl; 
     entity->conntable.erase (myaddr);
   }
   
  state = Tcp6muxConnSM :: closed;
  return true;
}

//
//Function: wfSopen_timeout
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in wfSopen state and incoming
//  message is type of DevIf :: Timeout.
//
//  DevIf :: Close message is created and used as parameter for
//  wfSopen_Disconn function.
//  

bool Tcp6muxConnection :: wfSopen_timeout (Message *msg) {


  DevIf :: Close *sMsg = new DevIf :: Close;
  sMsg->fd = cep;
  sMsg->device = &device;
  sMsg->mark (&device.iFace, &device.iFace);
  sMsg->lock ();
  Message *bMsg = (Message *) sMsg;

  return wfSopen_Disconn (bMsg);
}

//
//Function: wfSopen_default
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//
//  In this inplemtation nothing is done.
//  

bool Tcp6muxConnection :: wfSopen_default (Message *msg) {
  return false;
}

//
//Function: wfCresp_Cresp
//
//Member-Of: Tcp6muxConnection
//
//Returns:  bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in wfCresp state and incoming
//  message is type of Tcp6muxSAP::Cresp.
//

bool Tcp6muxConnection :: wfCresp_Cresp (Message *msg) {

  
  Tcp6muxSAP::Cresp *cr = (Tcp6muxSAP::Cresp *) msg;
  respTimer.stop ();
  device.readBytes (StreamDevice :: immediate);
  
  if (!entity->conntable.count(myaddr)){
    // std::cout << "populating conntable " << myaddr.getHostAddr()<< ":" << myaddr.getPortNumber() << " , cep "<< cep <<std::endl; 
	entity->conntable [myaddr] = cep;
  }  
  state = Tcp6muxConnSM::open;
  
  return false;
}

//
//Function: wfCresp_Dreq
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool 
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in wfCresp state and incoming
//  message is type of Tcp6muxSAP::Dreq.
//
//  Same thing is done as if in open state.
//
bool Tcp6muxConnection :: wfCresp_Dreq (Message *msg) {

  return open_Dreq (msg);
}

//
//Function: wfCresp_Disconn
//
//Member-Of: Tcp6muxConnection
//
//Returns:  bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in wfCresp state and incoming
//  message is type of DevIf :: Close.
//
//  Same thing is done as if in closed state.
//
bool Tcp6muxConnection :: wfCresp_Disconn (Message *msg) {

  DevIf :: Close *sDisc = (DevIf :: Close *) msg;
  Message *bMsg = 0;

  Tcp6muxSAP :: Dind *di = (Tcp6muxSAP :: Dind *) up.create (Tcp6muxSAP :: dind);
  di->cep = cep;

  bMsg = (Message *) di;
  up.putMessage (bMsg);

  msg->lock ();
  entity->save (msg);

  state = Tcp6muxConnSM :: closed;

  return true;
}

//
//Function: wfCresp_timeout
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in wfCresp state and timeout has
//  occurred.
//
//  Service user has not accepted connection in time and DevIf :: Close
//  message is sent to controlling user which should close socket
//  connection and delete this instance.
//  
bool Tcp6muxConnection :: wfCresp_timeout (Message *msg) {

  DevIf :: Close *sMsg = new DevIf :: Close;
  sMsg->fd = cep;
  sMsg->device = &device;
  sMsg->mark (&device.iFace, &device.iFace);
  sMsg->lock ();
  Message *bMsg = (Message *) sMsg;

  return wfSopen_Disconn (bMsg);
}

//
//Function: wfCresp_default
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool 
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in wfCresp state and source
//  of the message is not known.
//
//  In this inplemtation nothing is done.
//
bool Tcp6muxConnection :: wfCresp_default (Message *msg) {
  return false;
}

//
//Function: open_Dtreq
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool 
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in open state and message
//  type is Tcp6muxSAP::Dtreq
//
//  The data in the message is moved into the DevIf :: Read message
//  and it is given to DevIf :: Open.
//
bool Tcp6muxConnection :: open_Dtreq (Message *msg) {

  Tcp6muxSAP::Dtreq *dtr = (Tcp6muxSAP::Dtreq *) msg;
  device.write(dtr->data);

  idleTimer.start ();
  state = Tcp6muxConnSM :: open;
  return false;
}

//
//Function: open_Dreq
//
//Member-Of: Tcp6muxConnection
//
//Returns:  bool
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in open state and message
//  type is Tcp6muxSAP::Dreq
//

bool Tcp6muxConnection :: open_Dreq (Message *msg) {

  _MT_SYNCMONITOR;

  bool rc = false;

  if (device.writeBuffer() == 0) {
    _MT_KILLTHREAD(&device);
    _MT_WAIT_EXITBARRIER(&device);
    device.close();
  } else {
    msg->lock();
    save (msg);
    rc = true;
  }

  if (entity->conntable.count(myaddr)){
    // std::cout << "unpopulating conntable " << myaddr.getHostAddr()<< ":" << myaddr.getPortNumber() << " , cep "<< entity->conntable [myaddr]<<std::endl; 
    entity->conntable.erase (myaddr);
  }
  return rc;

}

//
//Function: open_Data
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool 
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in open state and message
//  type is DevIf :: Read
//

bool Tcp6muxConnection :: open_Data (Message *msg) {

    _MT_SYNCMONITOR;

    device.readBytes(StreamDevice :: immediate);

    DevIf :: Read *sData = (DevIf :: Read *) msg;
    Tcp6muxSAP :: Dtind *dti = (Tcp6muxSAP :: Dtind *) up.create (Tcp6muxSAP :: dtind);
    dti->cep = cep;
    dti->data = sData->data;
//    Byte *tmp = dti->data;
    up.putMessage (dti);
	
	idleTimer.start ();
	// std::cout << "Idle timer started. " << std::endl;
    state = Tcp6muxConnSM :: open;

    return false;

}

//
//Function: open_Disconn
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool 
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in open state and incoming
//  message is type of DevIf :: Close.
//
//  Same thing is done as if in closed state.
//
bool Tcp6muxConnection :: open_Disconn (Message *msg) {

  _MT_SYNCMONITOR;
  // std::cout << "Disconn received for " << cep << std::endl;

  return wfSopen_Disconn (msg);
}

//
//Function: open_timeout
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool 
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in open state and idle
//  timeout occurs as a result of no data. Die, task, die.
//

bool Tcp6muxConnection :: open_timeout (Message *msg) {

  return open_Dreq(msg);
}

//
//Function: open_default
//
//Member-Of: Tcp6muxConnection
//
//Returns: bool 
//
//Parameters: 
//      Message *msg
//
//Description:
//  Function provides the functionality of the state automaton.
//  
//  This function is run when task is in open state and source
//  of the message is not known.
//
//  In this inplemtation nothing is done.
//
bool Tcp6muxConnection :: open_default (Message *msg) {
  return false;
}


//
//Function: getCep
//
//Member-Of: Tcp6muxConnection
//
//Returns: int
//
//Parameters: 
//
//Description:
//  Returns the connection identifier CEP number.
//
int Tcp6muxConnection :: getCep (void) {

  return cep;
}

