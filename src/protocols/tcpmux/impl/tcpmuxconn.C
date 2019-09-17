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


//  $Log: tcpmuxconn.C,v $
//  Revision 1.4  2009/04/20 13:42:11  bilhanan
//  slightly longer timeouts
//
//  Revision 1.3  2005/03/02 14:32:58  bilhanan
//  development updates
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
#include <doors/tcpmuxconn.h>
#include <doors/tcpmuxcsm.h>

//
//Constructor: TcpmuxConnection
//
//Parameters:
//      IoHandler *ioh,
//      Uint16 id,
//      TcpmuxEntity *e,
//      TcpmuxConnSM *csm 
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

TcpmuxConnection :: TcpmuxConnection (IoHandler *io, int id, TcpmuxEntity *e, TcpmuxConnSM *csm): 
  PTask ("", e->getScheduler (), csm), 
  connection (this),
  device(io, this),
  entity (e),
  idleTimer (this, OTime (100)),
  respTimer (this, OTime (300)),
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
//Destructor: TcpmuxConnection
//
//Description:
//  Delete symbols.
//

TcpmuxConnection :: ~TcpmuxConnection (void) {
    
  DELETE_SYMBOL (this);
  device.close();
}

//
//Function: closed_Creq
//
//Member-Of: TcpmuxConnection
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
//  message is type of TcpmuxSAP::Creq.
//
//  There should be no data included in the Creq primitive

bool TcpmuxConnection :: closed_Creq (Message *msg) {

  TcpmuxSAP :: Creq *cr = (TcpmuxSAP :: Creq *) msg;
  reqici = cr->ici;
  respTimer.start ();

  device.connect(&(cr->addr));
  device.readBytes(StreamDevice :: immediate);

  state = TcpmuxConnSM :: wfSopen;
  return false;
}

//Function: closed_Conn
//
//Member-Of: TcpmuxConnection
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
 
bool TcpmuxConnection :: closed_Conn (Message *msg) {

  TcpmuxSAP :: Cind *ci = (TcpmuxSAP :: Cind *) up.create (TcpmuxSAP :: cind);
  myaddr =  *(InetAddr*)device.getRemoteAddress();
  ci->cep = cep;
  ci->addr = myaddr;
  up.putMessage(ci);
  device.readBytes (StreamDevice :: immediate);
  state = TcpmuxConnSM :: wfCresp;
  respTimer.start ();
  return false;
}



//
//Function: closed_default
//
//Member-Of: TcpmuxConnection
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
bool TcpmuxConnection :: closed_default (Message *msg) {

  return false;
}


//
//Function: wfSopen_conn
//
//Member-Of: TcpmuxConnection
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

bool TcpmuxConnection :: wfSopen_Conn (Message *msg) {


  DevIf :: Open *tmp = (DevIf :: Open *) msg;    

  cep = tmp->fd;
  myaddr =  *(InetAddr*)device.getRemoteAddress();

  std::ostringstream cname;
  cname << entity->getName () << ".conn." << cep; 
  name = cname.str();
  RENAME_SYMBOL (this, cname.str());

  TcpmuxSAP :: Cconf *cc = (TcpmuxSAP :: Cconf *) up.create (TcpmuxSAP :: cconf);
  cc->cep = cep;
  cc->addr = myaddr;
  cc->ici = reqici; // return the data passed during Creq

  up.putMessage (cc);
  
  msg->lock();
  entity->save (msg);

  respTimer.stop ();

  state = TcpmuxConnSM :: open;
  idleTimer.start ();

  return true;
}

//
//Function: wfSopen_Disconn
//
//Member-Of: TcpmuxConnection
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
//  TcpmuxSAP::Dind message is sent to upper layer and state ischanged 
//  to closed.
//  

bool TcpmuxConnection :: wfSopen_Disconn (Message *msg) {

  DevIf :: Close *sDisc = (DevIf :: Close *) msg;

  TcpmuxSAP :: Dind *di = (TcpmuxSAP :: Dind *) up.create (TcpmuxSAP :: dind);
  di->cep = cep;
  up.putMessage (di);

  msg->lock ();
  entity->save (msg);

  state = TcpmuxConnSM :: closed;
  return true;
}

//
//Function: wfSopen_timeout
//
//Member-Of: TcpmuxConnection
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

bool TcpmuxConnection :: wfSopen_timeout (Message *msg) {

  return wfCresp_timeout (msg);
}

//
//Function: wfSopen_default
//
//Member-Of: TcpmuxConnection
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

bool TcpmuxConnection :: wfSopen_default (Message *msg) {
  return false;
}

//
//Function: wfCresp_Cresp
//
//Member-Of: TcpmuxConnection
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
//  message is type of TcpmuxSAP::Cresp. The state is changed to
//  TcpmuxConnSM::open and the idle timer is started
//

bool TcpmuxConnection :: wfCresp_Cresp (Message *msg) {

  TcpmuxSAP::Cresp *cr = (TcpmuxSAP::Cresp *) msg;
  respTimer.stop ();
  device.write (cr->data); 
  state = TcpmuxConnSM::open;
  idleTimer.start ();
  
  return false;
}

//
//Function: wfCresp_Dreq
//
//Member-Of: TcpmuxConnection
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
//  message is type of TcpmuxSAP::Dreq.
//
//
bool TcpmuxConnection :: wfCresp_Dreq (Message *msg) {

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
//Member-Of: TcpmuxConnection
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
bool TcpmuxConnection :: wfCresp_Disconn (Message *msg) {

  DevIf :: Close *sDisc = (DevIf :: Close *) msg;

  respTimer.stop();
  TcpmuxSAP :: Dind *di = (TcpmuxSAP :: Dind *) up.create (TcpmuxSAP :: dind);
  di->cep = cep;

  up.putMessage (di);

  msg->lock ();
  entity->save (msg);

  state = TcpmuxConnSM :: closed;

  return true;
}

//
//Function: wfCresp_timeout
//
//Member-Of: TcpmuxConnection
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
bool TcpmuxConnection :: wfCresp_timeout (Message *msg) {

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
//Member-Of: TcpmuxConnection
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
bool TcpmuxConnection :: wfCresp_default (Message *msg) {
  return false;
}

//
//Function: open_Dtreq
//
//Member-Of: TcpmuxConnection
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
//  type is TcpmuxSAP::Dtreq
//
//  The data in the message is moved into the DevIf :: Read message
//  The idle timer is restarted.
//
bool TcpmuxConnection :: open_Dtreq (Message *msg) {

  TcpmuxSAP::Dtreq *dtr = (TcpmuxSAP::Dtreq *) msg;
  device.write(dtr->data);

  state = TcpmuxConnSM :: open;
  idleTimer.start ();
  return false;
}

//
//Function: open_Dreq
//
//Member-Of: TcpmuxConnection
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
//  type is TcpmuxSAP::Dreq. If there aren't multiple tasks
//  using this TCP connection, the connection task is deleted
//  by the entity task. Otherwise, the multiplex usage count is
//  simply decremented.
//

bool TcpmuxConnection :: open_Dreq (Message *msg) {

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
//Member-Of: TcpmuxConnection
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

bool TcpmuxConnection :: open_Data (Message *msg) {

    device.readBytes(StreamDevice :: immediate);

    DevIf :: Read *sData = (DevIf :: Read *) msg;
    TcpmuxSAP :: Dtind *dti = (TcpmuxSAP :: Dtind *) up.create (TcpmuxSAP :: dtind);
    dti->cep = cep;
    dti->data = sData->data;
    up.putMessage (dti);
	
    state = TcpmuxConnSM :: open;
    idleTimer.start ();

    return false;

}

//
//Function: open_Disconn
//
//Member-Of: TcpmuxConnection
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
bool TcpmuxConnection :: open_Disconn (Message *msg) {

  return wfSopen_Disconn (msg);
}

//
//Function: open_timeout
//
//Member-Of: TcpmuxConnection
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

bool TcpmuxConnection :: open_timeout (Message *msg) {

  /*
  DevIf :: Close *sMsg = new DevIf :: Close;
  sMsg->fd = cep;
  sMsg->device = &device;
  sMsg->mark (&device.iFace, &device.iFace);
  sMsg->lock ();

  return wfSopen_Disconn (sMsg);
  */

  return wfCresp_timeout (msg);
}

//
//Function: open_default
//
//Member-Of: TcpmuxConnection
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
bool TcpmuxConnection :: open_default (Message *msg) {
  return false;
}



