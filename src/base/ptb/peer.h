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

$Log: peer.h,v $
Revision 1.3  2002/07/23 14:25:19  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/06/03 09:19:50  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:51  bilhanan
Development

Revision 1.2  2001/06/08 16:45:50  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:31  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef PEER_H
#define PEER_H

#include <doors/iface.h>

class PDU;
class ReqIface;

//
//Class: Peer
//
//Description:
//   Peer class abstracts a connection from one entity task
//   to its peer entity. Once a task contains an instance of
//   this class it can send and receive PDU's via this
//   object instead of using the lower layer service.
//   Equally, the task "sees" received indications of lower
//   layer as PDU:s coming from its Peer object.
//   
//   The Peer class encapsulates PDU encoding and decoding
//   from the task. It means that the task can send
//   instances of concrete PDU classes without encoding as
//   well as the task will receive messages from peer entity
//   as the instances of concrete PDU classes.
//   
//   Peer class is inherited from Iface class to get
//   functionality of Iface. It also derives Otask class in
//   order to work asynchronously in DOORS system.
//   User inherits this class in his protocol application.
//   In the inherited class he needs to implement an
//   identify member function.
//   

class Peer : public Iface, public Otask {
  public:
    Peer (EventTask *host, MessageCreateFunction *funcs, ReqIface *r);
    virtual ~Peer (void);

    void run (void);
    bool save (Message *msg);

    virtual bool putMessage (Message *msg);
    bool putPersistentMsg (Message *msg); 
    virtual bool getMessage (Message *msg);
    
    virtual Sint16 identify (Message *msg) = 0;

  protected:
    Iface *downiface;
    LinkList messageQueue;

  private:

};


//
//Function: identify
//
//Member-Of: Peer
//
//Parameters:
//   Message *msg
//
//Return:
//   Sint16
//
//Description:
//   This is a pure base function for identifying a type of
//   the PDU from received primitive given as a parameter.
//   It doesn't have implementation in Peer class but user
//   defines it in inherited class. Normally the type of the
//   PDU can be resolved by inspecting the SDU part of the
//   received indication primitive.
//   
//   This method is called in run function of this class
//   when a new PDU is created to represent received
//   indication of lower layer.
//   
//   Parameter msg is an indication primitive received from
//   lower layer.
//   
//   The identify function must return an index for the
//   array of message create function the Peer contains. If
//   the msg doesn't represent any PDU and it should be
//   given directly to the entity task, this function must 
//   return -1.
//   

#endif   // PEER_H
