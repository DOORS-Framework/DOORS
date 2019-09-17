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

$Log: peermessage.h,v $
Revision 1.2  2002/06/18 12:23:32  ik
string->std::string etc fixes.

Revision 1.1  2002/06/17 13:29:54  bilhanan
*** empty log message ***

Revision 1.2  2002/06/07 12:19:07  hartmanj
Full licence clause added


*/


// PeerMessage -class

//260202 petri Draft version

// Class defines message, belonging to some PeerInterface

#ifndef PEERMESSAGE_HH
#define PEERMESSAGE_HH

#include <string>
#include <vector>
#include <doors/xcgmessage.h>

class PeerMessage : public XcgMessage
{
public:
  
  PeerMessage(const std::string &name);
  virtual ~PeerMessage();
  
  // Adds Parent to Message.
  virtual void addParent(const std::string &parent);
  
  // Adds Member data to Message.
  virtual void addMemberData(const std::string &member);
  
  // Links message as a next to this.
  void setNextMessage(PeerMessage *message);
  
  // Returns the next message in list.
  PeerMessage* getNextMessage() const;
  
  // Returns the name of the message.
  virtual std::string getName() const;
    
  // Print the declaration of message-class to header-stream.
  virtual void printHeader(std::ostream &header);
  
  // Print implementation of message-class to implementation stream.
  virtual void printImpl(const std::string PeerName, std::ostream &impl);
    
private:
    
  // Message name
  std::string name_;
  // Parents as a vector of strings
  std::vector<std::string> parents_;
  // Members as a vector of strings
  std::vector<std::string> members_;
  // Links to make an linked list of messages.
  PeerMessage *nextMessage_;
  
};

#endif //PeerMESSAGE_HH
