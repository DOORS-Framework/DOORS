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

$Log: peer.C,v $
Revision 1.4  2002/08/14 09:31:39  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/23 14:25:19  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:50  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:46:32  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:30  bilhanan
Full licence clause added into doors 0.1


*/



#include <doors/ocoremac.h>
#include <doors/peer.h>
#include <doors/hsi.h>
#include <doors/pdu.h>
#include <doors/sap.h>


//
//Constructor: Peer
//
//Parameters:
//   EventTask *host,
//   MessageCreateFunction *funcs,
//   ReqIface *r
//
//Description:
//   Creates a new instance of Peer with given parameters.
//   
//   First parameter host is an entity task this class
//   belongs to.
//   
//   Second parameter funcs is an array of message create
//   functions the Peer needs to implement the create
//   function of Iface class. The array determines PDU:s the
//   Peer is meant to send and receive. Note, that the array
//   is obligatory because it is called in a getMessage
//   function this class.
//   
//   Third parameter r is a pointer to request interface of
//   lower layer. By using that pointer the Peer can send
//   PDU:s by primitives of the lower layer.
//   

Peer :: Peer (EventTask *host, MessageCreateFunction *funcs, ReqIface *r)
: Iface (host, funcs), Otask ("peer", host->getScheduler ()) {
    
    _MT_SYNCMONITOR;

    downiface = r;
}


//
//Destructor: Peer
//
//Description:
//   Deletes this Peer object.
//   

Peer :: ~Peer (void) {

    _MT_SYNCMONITOR;
}


//
//Function: putMessage
//
//Member-Of: Peer
//
//Parameters:
//   Message * msg
//
//Return:
//   bool
//
//Description:
//   A task can send PDU:s to the peer entity by
//   calling this function. Before user should call it, he
//   must be sure that the PDU is initialized completely
//   with correct values of data members. The ICI structure
//   - if needed in primitive constructing of lower layer -
//   must also have been attached to the PDU.
//   
//   Parameter msg is a PDU to be sent to the peer entity.
//   
//   Returns false if the PDU has been sent successfully.
//   Otherwise the return value is true.
//   

bool Peer :: putMessage (Message *pdu) {

    _MT_SYNCMONITOR;

    bool rc = false;
    Message *msg = 0;

    if ((msg = ((PDU *) pdu)->encode ()) != 0) {
	rc = downiface->putMessage (msg);
    }
    delete pdu;
    
    return rc;
}

//
//Function: putPersistentMessage
//
//Member-Of: Peer
//
//Parameters:
//   Message * msg
//
//Return:
//   bool
//
//Description:
//   A task can send PDU:s to the peer entity by
//   calling this function. Before user should call it, he
//   must be sure that the PDU is initialized completely
//   with correct values of data members. The ICI structure
//   - if needed in primitive constructing of lower layer -
//   must also have been attached to the PDU.
//   
//   Parameter msg is a PDU to be sent to the peer entity.
//   
//   Returns false if the PDU has been sent successfully.
//   Otherwise the return value is true.
//   
//   This function does the same thing as putMessage, except the
//   Message parameter does not get deleted. This allows flexibility
//   eg to encode multiple messages to multiple peers efficiently.
//   However it remains the programmer's responsibility to delete the
//   Message parameter

bool Peer :: putPersistentMsg (Message *pdu) {
 
    _MT_SYNCMONITOR;
 
    bool rc = false;
    Message *msg = 0;
 
    if ((msg = ((PDU *) pdu)->encode ()) != 0) {
        rc = downiface->putMessage (msg);       
    }
    
    return rc;
}
 

//
//Function: getMessage
//
//Member-Of: Peer
//
//Parameters:
//   Message *msg
//
//Return:
//   bool
//
//Description:
//   Saves the incoming message.
//   
//   Parameter msg is a PDU to be sent to the peer entity.
//   
//   Returns false if the PDU has been sent successfully.
//   Otherwise the return value is true.
//   

bool Peer :: getMessage (Message *msg) {

    _MT_SYNCMONITOR;

    return save (msg);
}



//
//Function: run
//
//Member-Of: Peer
//
//Description:
//   This function is called by scheduler when the Peer
//   object has received a primitive from lower layer. It
//   creates a new PDU instance, decodes the primitive into
//   the PDU and call the save function of the host task to
//   store the PDU.
//   
//   Note that the primitive of lower layer is deleted at
//   the end of this function.
//   

void Peer :: run (void) {

    _MT_SYNCMONITOR;
    _MT_RELEASE_SYNCMONITOR; 
    _MT_STABLE_MONITOR; 
    _MT_ACQUIRE_SYNCMONITOR;
    _MT_TRACE_TASK;

    Message *msg = 0;
    PDU *pdu = 0;

    if (msg = (Message *) messageQueue.remove ()) {
	TRACE_MSG (msg);
	Sint16 pduType = identify (msg);
	if (pduType != -1) {
	    if ((pdu = (PDU *) create (pduType)) != 0) {
		pdu->decode (msg);
		pdu->mark (this, this);
		task->save (pdu);
	    }
	    delete msg;
	} else {
	    task->save (msg);
	}
	decrementLoad ();
    }
    _MT_TRACE_TASK;
}

//
//Function: save
//
//Member-Of: Peer
//
//Parameters:
//   Message * msg
//
//Return: bool
//
//Description:
//   This function is called by ReqIface object when it gets
//   the primitive from lower layer. The function saves the
//   primitive into the message queue the Peer object
//   contains for later processing in its own execution
//   turn.
//   
//   Parameter msg is a primitive received from lower layer.
//   

bool Peer :: save (Message *msg) {

    _MT_ASYNCMONITOR;
    messageQueue.append (msg);
	
    // Inform the scheduler that this task need to be executed in near future
    if (scheduler && getLoad () == 0) {
        scheduler->request (this, priority);
    }

    // we keep track of the number of messages we have
    incrementLoad ();
    _MT_SIGNAL_RUNLOCK;

    // when the message is saved or deleted nobody should no more 
    // have any references to it.
    return true;
}






