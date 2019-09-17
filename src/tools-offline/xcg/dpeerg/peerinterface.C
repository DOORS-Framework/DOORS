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

$Log: peerinterface.C,v $
Revision 1.1  2002/06/17 13:29:54  bilhanan
*** empty log message ***

Revision 1.3  2002/06/14 13:41:22  hartmanj
Changed #includes

Revision 1.2  2002/06/07 12:19:07  hartmanj
Full licence clause added


*/


// Peernterface-class Implementation

//260202 petri Draft version
//060402 petri inherited from Interface

#include <cstdlib>
#include <vector>
#include <ctype.h>
#include <doors/peerinterface.h>
#include <doors/peermessage.h>
#include <doors/xmldocument.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ostream;

extern string TAB(void);
extern int ilevel;

PeerInterface::PeerInterface(ifaceType type) 
  : Interface(),
    name_(),
    parents_(),
    firstMessage_(0),
    lastMessage_(0),
    type_(type)
{
    // Nothing to do...
}

PeerInterface::~PeerInterface()
{
    // Delete the messages related to this interface.
    // loop through the message-list and delete all.
    
    PeerMessage *tmp = firstMessage_;
    
    while(firstMessage_ != 0)
    {
	tmp = firstMessage_->getNextMessage();
	delete firstMessage_;
	firstMessage_ = tmp;
    }
    tmp = 0;
    firstMessage_ = 0;
}
void PeerInterface::addParent(const string &parent)
{
    // Adds Inheritance parent to Message (inheritance type is allways public)
    parents_.push_back(parent);
}
// Get's the Interface's first message.
PeerMessage* PeerInterface::getFirstMessage() const
{
    return firstMessage_;
}
// Sets the Interface's first message.
void PeerInterface::setFirstMessage(PeerMessage *message)
{
    firstMessage_ = message;
}
// Adds message to interface.
void PeerInterface::addMessage(PeerMessage *message)
{
    if(lastMessage_ == 0) // no messages so far.
    {
	firstMessage_ = message;
	lastMessage_ = firstMessage_;
	lastMessage_->setNextMessage(0);
    }
    else if(firstMessage_ != 0)
    { 
	PeerMessage *tmp = lastMessage_;
	lastMessage_ = message;
	tmp->setNextMessage(lastMessage_);
	lastMessage_->setNextMessage(0);
    }
    else
    {
	cout << "tannne ei kai tulla" << endl;
    }
}

// Print the Interface's message-classes headers...
void PeerInterface::printMessageClasses(ostream &header)
{
    // first print enumeration for states of peer-class
    // States are the lowercase names of the PeerMessages + "other".
    
    if(firstMessage_ != 0)
    {
	PeerMessage *tmp = firstMessage_->getNextMessage();
	
	header << TAB() << "enum State { " << StrToLower(firstMessage_->getName()) << " = 1";
	
	while(tmp != 0)
	{
	    header << ", " << StrToLower(tmp->getName());
	    tmp = tmp->getNextMessage();
	}
	
	header << ", other };" << endl << endl;
    }
    
    // Second, print the class-declarations of each message belonging to this
    // interface.
    
    PeerMessage *tmp = firstMessage_;
    while(tmp != 0)
    {
	tmp->printHeader(header);
	tmp = tmp->getNextMessage();
    }
}
// Prints the class and all it's subclasses implementations to .cc file
void PeerInterface::printIfaceClassImpl(const string &peerName, ostream &impl)
{
    // Print the create methods for each message.
    
    PeerMessage *tmp = firstMessage_;
    
    while( tmp != 0)
    {
	impl << TAB() << "Message *create" << peerName << tmp->getName() 
	     << " (void)" << endl;
	
	impl << TAB() << "{" << endl;
	ilevel++;
	impl << TAB() << "return new " << peerName << " :: " << tmp->getName() 
	     << ";" << endl;
	ilevel--;
	impl << TAB() << "}" << endl << endl;
	tmp = tmp->getNextMessage();
    }
    
    tmp = 0;
    
    // Second print the functionpointer thing, of each message belonging
    // to this Interface.
    // (jh: OVOPS hack; if(peerName=="LinkPeer") -> print "linkPeerMessages)

    impl << TAB() << "Message *(*" << (char)tolower(peerName[0])
		 << peerName.substr(1,peerName.size())
		 << "Messages []) (void) =" 
	 << endl;
    impl << TAB() << "{" << endl;
    ilevel++;
    tmp = firstMessage_;
    
    while(tmp != 0)
    {
	impl << TAB() << "create" << peerName << tmp->getName() 
	     << "," << endl;
	
	tmp = tmp->getNextMessage();
    }
    
    impl << TAB() << "0" << endl;
    ilevel--;
    impl << TAB() << "};" << endl << endl;
}

void PeerInterface::printIfaceMessagesImpl(const std::string &peerName,
					  std::ostream &impl)
{
    // Print the implementations of messages belonging to this interface.
    
    // 1. Print the message implementations.
    
    PeerMessage *tmp = firstMessage_;
    
    while(tmp != 0)
    {
	tmp->printImpl(peerName, impl);
	tmp = tmp->getNextMessage();
    }
}
