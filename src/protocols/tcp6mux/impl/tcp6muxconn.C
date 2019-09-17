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
//  Revision 1.3  2005/03/02 14:32:41  bilhanan
//  development updates
//
//  Revision 1.2  2005/02/21 16:30:41  bilhanan
//  clean port of tcpmux to ipv6
//
//  Revision 1.2  2005/02/21 13:59:20  bilhanan
//  development
//
//  Revision 1.1  2004/10/12 12:01:22  bilhanan
//  The protocol formerly known as tranmux
//
//  Revision 1.8  2003/05/15 11:48:11  bilhanan
//  fixes for std c++ compliance
//
//  Revision 1.7  2002/09/13 12:56:50  kalliosa
//  fixed problem with default value in constructor
//
//  Revision 1.6  2002/08/16 09:55:57  hartmanj
//  Added idletimeout to constructor
//
//  Revision 1.5  2002/08/15 08:34:14  hartmanj
//  ONS taken away
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

#include <map>
#include <string>
#include <sstream>
#include <doors/hsi.h>
#include <doors/util.h>
#include <doors/core.h>
#include <doors/tcp6muxconn.h>
#include <doors/tcp6muxcsm.h>

//
//Constructor: Tcp6muxConnection
//
//Parameters:
//      IoHandler *ioh,
//      Uint16 id,
//      Tcp6muxEntity *e,
//      Tcp6muxConnSM *csm 
//
//Description:
//  Initializes the base class and own variables. 
//
//  The parameter id is a file descriptor value passed from
//  entity task, e is a pointer to entity task, csm is pointer to
//  state automaton.
//
//  The idleTimer is set to 1000 seconds
//  The response Timer is set to 30 seconds 

Tcp6muxConnection :: Tcp6muxConnection (IoHandler *io, int id, Tcp6muxEntity *e, Tcp6muxConnSM *csm): 
  PTask ("", e->getScheduler (), csm), 
  connection (this),
  device(io, this),
  entity (e),
  idleTimer (this, OTime (1000)),
  respTimer (this, OTime (30)),
  cep (id),
  muxcount (1),
  up (e->up) {
  
  std::ostringstream cname;
  cname << entity->getName () << ".conn." << cep; 
  
  name = cname.str();
  
  connection.connect(&device.iFace);
  if (id > 0)  
    device.connect(id);  // we connect if id value is valid
  
  DYNAMIC_SYMBOL (this, cname.str(), e);
  STATIC_SYMBOL (connection, this);
  STATIC_SYMBOL (cep, this);
  STATIC_SYMBOL (muxcount, this);
  STATIC_SYMBOL (idleTimer, this);
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
//  This function runs when task is in closed state and incoming
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
//  This function runs when task is in closed state and incoming
//  message is type of DevIf :: Open.
//
//  Connect indication is sent up and state is updated to wfCresp.
//  Response timer is started.
 
bool Tcp6muxConnection :: closed_Conn (Message *msg) {

  Tcp6muxSAP :: Cind *ci = (Tcp6muxSAP :: Cind *) up.create (Tcp6muxSAP :: cind);
  myaddr =  *(InetAddr6*)device.getRemoteAddress();
  ci->cep = cep;
  ci->addr = myaddr;
  up.putMessage(ci);
  device.readBytes (StreamDevice :: immediate);
  state = Tcp6muxConnSM :: wfCresp;
  respTimer.start ();
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
//  This function runs when task is in closed state and source
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
//  This function runs when task is in wfSopen state and incoming
//  message is type of DevIf :: Open and state is changed to open.
//
//  The message is also given to the entity for registration purpose.

bool Tcp6muxConnection :: wfSopen_Conn (Message *msg) {


  DevIf :: Open *tmp = (DevIf :: Open *) msg;    

  cep = tmp->fd;
  myaddr =  *(InetAddr6*)device.getRemoteAddress();

  std::ostringstream cname;
  cname << entity->getName () << ".conn." << cep; 
  name = cname.str();
  RENAME_SYMBOL (this, cname.str());

  Tcp6muxSAP :: Cconf *cc = (Tcp6muxSAP :: Cconf *) up.create (Tcp6muxSAP :: cconf);
  cc->cep = cep;
  cc->addr = myaddr;
  cc->ici = reqici; // return the data passed during Creq

  up.putMessage (cc);
  
  msg->lock();
  entity->save (msg);

  respTimer.stop ();

  state = Tcp6muxConnSM :: open;
  idleTimer.start ();

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
//  This function runs when task is in wfSopen state and incoming
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
//  This function runs when task is in wfSopen state and incoming
//  message is type of DevIf :: Timeout.
//
//  DevIf :: Close message is created and used as parameter for
//  wfSopen_Disconn function.
//  

bool Tcp6muxConnection :: wfSopen_timeout (Message *msg) {

  return wfCresp_timeout (msg);
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
//  In this implementation nothing is done.
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
//  This function runs when task is in wfCresp state and incoming
//  message is type of Tcp6muxSAP::Cresp. The state is changed to
//  Tcp6muxConnSM::open and the idle timer is started
//

bool Tcp6muxConnection :: wfCresp_Cresp (Message *msg) {

  Tcp6muxSAP::Cresp *cr = (Tcp6muxSAP::Cresp *) msg;
  respTimer.stop ();
  device.write (cr->data); 
  state = Tcp6muxConnSM::open;
  idleTimer.start ();
  
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
//  This function runs when task is in wfCresp state and incoming
//  message is type of Tcp6muxSAP::Dreq.
//
//
bool Tcp6muxConnection :: wfCresp_Dreq (Message *msg) {

  bool rc = false;
  respTimer.stop();

  if (device.writeBuffer() == 0) {
    device.close();
  } else {
    msg->lock();
    save (msg);
    rc = true;
  }

  return rc;

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
//  This function runs when task is in wfCresp state and incoming
//  message is type of DevIf :: Close.
//
//  Same thing is done as if in closed state.
//
bool Tcp6muxConnection :: wfCresp_Disconn (Message *msg) {

  DevIf :: Close *sDisc = (DevIf :: Close *) msg;

  respTimer.stop();
  Tcp6muxSAP :: Dind *di = (Tcp6muxSAP :: Dind *) up.create (Tcp6muxSAP :: dind);
  di->cep = cep;

  up.putMessage (di);

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
//  This function runs when task is in wfCresp state and timeout has
//  occurred.
//
//  Service user has not accepted connection in time and DevIf :: Close
//  message is sent to entity task which should close socket
//  connection and delete this instance.
//  
bool Tcp6muxConnection :: wfCresp_timeout (Message *msg) {

  bool rc = false;

  if (device.writeBuffer() == 0) {
    device.close();
  } else {
    msg->lock();
    save (msg);
    rc = true;
  }

  return rc;

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
//  This function runs when task is in wfCresp state and source
//  of the message is not known.
//
//  In this implementation nothing is done.
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
//  This function runs when task is in open state and message
//  type is Tcp6muxSAP::Dtreq
//
//  The data in the message is moved into the DevIf :: Read message
//  The idle timer is restarted.
//
bool Tcp6muxConnection :: open_Dtreq (Message *msg) {

  Tcp6muxSAP::Dtreq *dtr = (Tcp6muxSAP::Dtreq *) msg;
  device.write(dtr->data);

  state = Tcp6muxConnSM :: open;
  idleTimer.start ();
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
//  This function runs when task is in open state and message
//  type is Tcp6muxSAP::Dreq. If there aren't multiple tasks
//  using this TCP6 connection, the connection task is deleted
//  by the entity task. Otherwise, the multiplex usage count is
//  simply decremented.
//

bool Tcp6muxConnection :: open_Dreq (Message *msg) {

  muxcount--;

  if (muxcount) {
    return false;
  }
  else {
    return wfCresp_Dreq (msg);
  }

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
//  This function runs when task is in open state and message
//  type is DevIf :: Read. The idle timer is restarted.
//

bool Tcp6muxConnection :: open_Data (Message *msg) {

    device.readBytes(StreamDevice :: immediate);

    DevIf :: Read *sData = (DevIf :: Read *) msg;
    Tcp6muxSAP :: Dtind *dti = (Tcp6muxSAP :: Dtind *) up.create (Tcp6muxSAP :: dtind);
    dti->cep = cep;
    dti->data = sData->data;
    up.putMessage (dti);
	
    state = Tcp6muxConnSM :: open;
    idleTimer.start ();

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
//  This function runs when task is in open state and incoming
//  message is type of DevIf :: Close.
//
//  Same thing is done as if in wfSopen state.
//
bool Tcp6muxConnection :: open_Disconn (Message *msg) {

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
//  timeout occurs as a result of no data. 
//

bool Tcp6muxConnection :: open_timeout (Message *msg) {

  return wfCresp_timeout (msg);

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
//  This function runs when task is in open state and source
//  of the message is not known.
//
//  In this implementation nothing is done.
//
bool Tcp6muxConnection :: open_default (Message *msg) {
  return false;
}



