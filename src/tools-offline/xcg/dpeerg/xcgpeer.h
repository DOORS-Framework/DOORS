//Editor-Info: -*- C++ -*-

//  Copyright 2000
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.

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

$Log: xcgpeer.h,v $
Revision 1.1  2002/06/17 13:29:54  bilhanan
*** empty log message ***

Revision 1.1  2002/06/17 08:14:52  hartmanj
Renamed peer.h/C to xcgpeer.h/C

Revision 1.3  2002/06/14 13:41:22  hartmanj
Changed #includes

Revision 1.2  2002/06/07 12:19:07  hartmanj
Full licence clause added


*/


// Peer -class

// 260202 petri Draft version.
// 070402 petri Inherited from Protocol Functionality baseclass
//              Structure

// Class descripes the peer functionality of Protocol Layer

#ifndef PEER_HH
#define PEER_HH

#include <vector>
#include <doors/structure.h>
#include <doors/peerinterface.h>
#include <doors/peermessage.h>

extern int ilevel;

class Peer : public Structure
{
public:
  
  Peer(const std::string &name);
  virtual ~Peer();
    
  // addParent adds inheritance parents to Peer-class.
  // Peer-class can have as many parents as needed.
  virtual void addParent(const std::string &parent);
  
  // Links the PDUInterface to this Peer Class
  void setPeerInterface(PeerInterface *PDUs);
  
  // Returns the PDUInterface, that contains all the PDUclasses.
  PeerInterface* getPeerInterface() const;
  
  // Prints the Peer header file.
  // Prints "recursevely" PeerInterface and PeerPDU headers as well.
  virtual void printHeader(std::ostream &header);
  
  // Prints the Peer implementation file.
  // Prints "recursevely" PeerInterface and PeerPDU-function's
  // implementations  as well.
  virtual void printImpl(std::ostream &impl);
  
private:
  
  // Peer-name
  std::string name_;
  // Peer's parents as a vector of strings.
  std::vector<std::string> parents_;
  // Peer's PDUInterface
  PeerInterface *PDU_;
    
};
#endif // Peer_HH
