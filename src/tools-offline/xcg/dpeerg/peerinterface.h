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

$Log: peerinterface.h,v $
Revision 1.3  2007/09/03 12:27:42  bilhanan
extra qualifiers removed for G++ 4.x compatibility.

Revision 1.2  2002/06/18 12:23:32  ik
string->std::string etc fixes.

Revision 1.1  2002/06/17 13:29:54  bilhanan
*** empty log message ***

Revision 1.3  2002/06/14 13:41:23  hartmanj
Changed #includes

Revision 1.2  2002/06/07 12:19:07  hartmanj
Full licence clause added


*/


// PeerInterface -class

//260202 petri Draft version
//060402 petri inherited from Interface

// PeerInterface class. [Normally only one interface in use,
// but can be as many as needed.

#ifndef PEERINTERFACE_HH
#define PEERINTERFACE_HH

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <doors/interface.h>
#include <doors/peermessage.h>

class PeerInterface : public Interface
{
  
public:
  
  // Typedef for Interface types.
  typedef enum {PDU = 0, types} ifaceType;
  
  PeerInterface(ifaceType type);
  virtual ~PeerInterface();
  
  // Adds Inheritance parents to Interface (can have as many as possible)
  // Multiple inheritance, dealed in c++ way.
  // (type, name) pairs separated by comma
  virtual void addParent(const std::string &parent);
  
  // Returns pointer to first PDU or other class linked to interface.
  PeerMessage* getFirstMessage() const;
  
  // Sets the first PDU of interface.
  void setFirstMessage(PeerMessage *message);
  
  // Adds new message to interface.
  void addMessage (PeerMessage *message);
  
  // Prints the Interface's messages to header (PeerMessage.printHeader())
  virtual void printMessageClasses(std::ostream &header);
  
  // Print the message implementations to for all messages belonging
  // to this interface.
  virtual void printIfaceMessagesImpl(const std::string &PeerName,
				      std::ostream &impl);
  
  virtual void printIfaceClassImpl(const std::string &peerName, std::ostream &impl);
  
private:
  
  // Interface-name
  std::string name_;
  // Parents as a vector of strings.
  std::vector<std::string> parents_;
  // FirstMessage
  PeerMessage *firstMessage_;
  // LastMessage
  PeerMessage *lastMessage_;
  // Type of interface.
  ifaceType type_;
};

#endif // PeerINTERFACE_HH
