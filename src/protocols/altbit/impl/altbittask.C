// Editor-info: -*- C++ -*-

//  Copyright 2010 
//  	Tampere University of Technology.  All rights reserved.

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

/*
$Log: altbittask.C,v $
Revision 1.4  2010/02/19 08:04:21  bilhanan
*** empty log message ***

Revision 1.3  2004/02/01 23:15:30  bilhanan
bugfixes, cvs diff for details on individual files.

Revision 1.2  2003/08/24 22:27:48  bilhanan
- removed all "delete msg" statements, use function return value for that.
- const char* -> std::string
- #include "..." -> #include <doors/...>

Revision 1.1  2003/07/02 14:37:27  ik
Moved to protocol section.

Revision 1.1  2003/07/02 11:27:52  ik
Initial revision.

*/

#include <doors/altbittask.h>

AltbitTask :: AltbitTask (std::string name, Scheduler* s, AltbitTaskSM* sm, InetAddr addr)
  : PTask(name, s, sm), 
    up(this),           // Connect this task to outgoing SAP
    peer(this, &down),  // Peer this task to other Altbit peer using 'down' UDP
    down(this, &peer),  // Connect the peer to UDP task
    altbit_(false),     // Initially altbit flag is down
    retries_(0),        // Number of send retries
    destaddr_(addr),        // Destination address
    sent_byte_(0),      // Used for retries
    timer_ (this, OTime (15))  // Send timer
{

  // Set these
  STATIC_SYMBOL(up, this);
  STATIC_SYMBOL(down, this);
  STATIC_SYMBOL(peer, this);
  // These are optional but give additional information to LEMon & DEMon
  STATIC_SYMBOL(altbit_, this);
  STATIC_SYMBOL(retries_, this);
  STATIC_SYMBOL(timer_, this);
}

AltbitTask :: ~AltbitTask() {

  // Delete symbols from the hierarchy
  DELETE_SYMBOL(&up);
  DELETE_SYMBOL(&down);
  DELETE_SYMBOL(&peer);
  DELETE_SYMBOL(&altbit_);
  DELETE_SYMBOL(&retries_);
  DELETE_SYMBOL(&timer_);
}

void AltbitTask :: sendACK (bool altbit) {

  // Create message for Altbit peer
  AltbitPeer :: ACK *peer_ack =  (AltbitPeer :: ACK *) peer.create (AltbitPeer :: ack);

  if (!peer_ack) {
    return;
  }

  peer_ack->altbit = altbit;

  // Destination address
  peer_ack->addr = destaddr_;

  peer.putMessage (peer_ack);

}

void AltbitTask :: sendDATA (Byte c) {

  // Create message for Altbit peer
  AltbitPeer :: DATA *peer_data = (AltbitPeer :: DATA *) peer.create (AltbitPeer :: data);

  if (!peer_data) {
    return;
  }

  // Save the alternating bit
  peer_data->altbit = altbit_;

  // Destination address
  peer_data->addr = destaddr_;

  // And finally the data
  peer_data->payload = c;

  // Reset timer
  timer_.start ();

  // Save data if needed for retry
  sent_byte_ = c;

  // Send data to the peer
  peer.putMessage (peer_data);
}

void AltbitTask :: receiveDATA (Message* msg) {

  AltbitPeer :: DATA *peer_data = (AltbitPeer :: DATA *) msg;
  
  // Create a new message for the user task with the data
  AltbitSAP :: Dtind* up_dtind = (AltbitSAP :: Dtind *) up.create (AltbitSAP :: dtind);
  up_dtind->srcIP = peer_data->addr;
  up_dtind->payload = peer_data->payload;
  up_dtind->altbit = peer_data->altbit;

  // Post the message to the user
  up.putMessage (up_dtind);
}

void AltbitTask :: receiveACK (Message* msg) {

  AltbitPeer :: ACK *peer_ack = (AltbitPeer :: ACK *) msg;

  // Create a new message for the user task with the altbit
  if (peer_ack->altbit) {
    AltbitSAP :: Ack1* up_ack = (AltbitSAP :: Ack1 *) up.create (AltbitSAP :: ack1);
    up.putMessage (up_ack);
  } else {
    AltbitSAP :: Ack0* up_ack = (AltbitSAP :: Ack0 *) up.create (AltbitSAP :: ack0);
    up.putMessage (up_ack);
  }

  // Acknowledge packet received from a peer. Take actions according to
  // the altbit.
  // If the altbit flag received in the ACK packet is the same
  // as in the previous DATA packet, send a new DATA packet and flip the flag.
  // If the string is empty, send a packet with 'EOT' (null character) and
  // return to idle state.

  if (peer_ack->altbit == altbit_) {
    altbit_ = (altbit_ == false) ? true : false; // toggle albit value
    if (octets_.length() == 0) {
      // Send 'EOT' packet to the other peer
      sendDATA(0);
      // Go back to idle state and reset the flags
      retries_ = 0;
      altbit_ = false;
      state = AltbitTaskSM::Idle;
    } else {
      // Valid ACK received so reset the retry count
      retries_ = 0;
      // Send a new packet and continue waiting
      sendDATA (octets_.getFirst());
    }
  } // else ignore the packet and keep on waiting

}

//
// **************** Idle state ****************
//

bool AltbitTask :: idle_Dtreq (Message* msg) {

  AltbitSAP :: Dtreq *up_dtreq = (AltbitSAP :: Dtreq *) msg;
  
  // Check for data sanity
  if (up_dtreq->octets.length() == 0) {
    return false;
  }

  // Reset the flags
  altbit_ = false;
  retries_ = 0;

  // Save the data
  octets_ = up_dtreq->octets;

  // Get first octet from the frame and send it. Function getFirst()
  // removes the octet and moves the pointer to the next octet.
  sendDATA (octets_.getFirst());

  // Move to wait state
  state = AltbitTaskSM::Wait;
  
  return false;
}

bool AltbitTask :: idle_Default (Message* msg) { 

  // just drop nessage
  return false; 
}

bool AltbitTask :: idle_Ackreq (Message* msg) {

  AltbitSAP :: Ackreq *up_ackreq = (AltbitSAP :: Ackreq *) msg;

  sendACK(up_ackreq->altbit);

  return false;
}

bool AltbitTask :: idle_DATA(Message* msg) {

  receiveDATA(msg);

  return false;
}

bool AltbitTask :: idle_ACK(Message* msg) {

  // ACK received when idle; do nothing

  return false;
}

//
// **************** Wait state ****************
//

bool AltbitTask :: wait_Default (Message* msg) { 

  // just drop nessage
  return false; 
}

bool AltbitTask :: wait_Dtreq(Message* msg) {

  // Send message received from the 'upper task' during the wait state.
  // Append this data to the end of the data frame.

  AltbitSAP :: Dtreq *up_dtreq = (AltbitSAP :: Dtreq *) msg;

  octets_.combine(up_dtreq->octets);

  return false;
}

bool AltbitTask :: wait_Ackreq(Message* msg) {

  AltbitSAP :: Ackreq *up_ackreq = (AltbitSAP :: Ackreq *) msg;

  sendACK(up_ackreq->altbit);

  return false;
}

bool AltbitTask :: wait_ACK(Message* msg) {

  receiveACK(msg);

  return false;
}

bool AltbitTask :: wait_DATA(Message* msg) {

  // Incoming data from a peer during Wait state. Identical to idle_DATA;
  // just grab the data and push it to the upper state.

  receiveDATA(msg);

  return false;
}

bool AltbitTask :: wait_Timeout(Message* msg) {

  // Increase the retry count
  retries_++;
  
  if (retries_ > MAX_RETRIES) {
    // Give up; reset the attributes and go back to idle state
    octets_.destroy();
    retries_ = 0;
    altbit_ = false;
    state = AltbitTaskSM::Idle;
  } else {
    sendDATA(sent_byte_);
  }

  return false;
}



