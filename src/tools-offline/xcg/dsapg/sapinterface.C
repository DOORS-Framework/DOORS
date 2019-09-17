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

$Log: sapinterface.C,v $
Revision 1.1  2002/06/17 13:29:55  bilhanan
*** empty log message ***

Revision 1.3  2002/06/14 13:41:31  hartmanj
Changed #includes

Revision 1.2  2002/06/07 12:19:08  hartmanj
Full licence clause added


*/


// SAPnterface-class Implementation

//060202 petri Draft version
//100202 petri Print Header Implemented -- quite ready.
//       petri Functions implemented that print implementations..
//250202 petri Commented and made small non-functional changes.
//060402 petri Inherited form interface

#include <cstdlib>
#include <vector>
#include <ctype.h>
#include <doors/sapinterface.h>
#include <doors/sapmessage.h>
#include <doors/xmldocument.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ostream;

SAPInterface::SAPInterface(ifaceType type)
	: Interface(),
	  name_(),
	  parents_(),
	  firstMessage_(0),
	  lastMessage_(0),
	  type_(type)
{
	// Set the default parents (RegInterface or IndInterface)
	// UserInterface is allways inherited atleast from RegInterface
	// and ProviderInterface from InfInterface.
  
    if(type_ == User)
    {
		name_ = "User";
		parents_.push_back("ReqIface");
    }
    else
    {
		name_ = "Provider";
		parents_.push_back("IndIface");
    } 
}
SAPInterface::~SAPInterface()
{
    // Delete the messages related to this interface.
    // loop through the message-list and delete all.
    
    SAPMessage *tmp = firstMessage_;
    
    while(firstMessage_ != 0)
    {
		tmp = firstMessage_->getNextMessage();
		delete firstMessage_;
		firstMessage_ = tmp;
    }
    tmp = 0;
    firstMessage_ = 0;
}
void SAPInterface::addParent(const string &parent)
{
    // Adds Inheritance parent to Message (inheritance type is allways public)
    parents_.push_back(parent);
}

SAPInterface::ifaceType SAPInterface::getType(void) const
{
	return type_;
}

// Get's the Interface's first message.
SAPMessage* SAPInterface::getFirstMessage() const
{
    return firstMessage_;
}
// Sets the Interface's first message.
void SAPInterface::setFirstMessage(SAPMessage *message)
{
    firstMessage_ = message;
}
// Adds message to interface.
void SAPInterface::addMessage(SAPMessage *message)
{
    if(lastMessage_ == 0) // no messages so far.
    {
		firstMessage_ = message;
		lastMessage_ = firstMessage_;
		lastMessage_->setNextMessage(0);
    }
    else if(firstMessage_ != 0)
    { 
		SAPMessage *tmp = lastMessage_;
		lastMessage_ = message;
		tmp->setNextMessage(lastMessage_);
		lastMessage_->setNextMessage(0);
    }
    else
    {
		cout << "Internal generator error! Please send bug report!" << endl;
    }
}
// Prints declaration of Interface's classes to header.
void SAPInterface::printIfaceClass(ostream &header)
{
    // Prints the User or Provider class to header file.
    // Their messages are then printed by printMessageClasses()-method
    
    // class name
    header << TAB() << "class " << name_;
    
    // print the public inheritance parents (c++.style).
    
    unsigned int idx = 0;
    if(!parents_.empty())
    {
		header << " : ";
		while(idx < parents_.size())
		{
			header << "public " << parents_[idx];
	    
			if(idx+1 < parents_.size())
			{ 
				header << " , ";
			}
			idx++;
		}
		idx = 0;
    }
    header << endl;
    header << TAB() << "{" << endl;
    ilevel++;
    header << TAB() << "public:" << endl;
    ilevel++;
    // Print the constructor and destructor.
    // Notice that constructors parametres are hardcoded HERE.

    if( type_ == User) {
		header << TAB() << name_ << "(EventTask *t, Peer *p=0);" << endl;
	} else {
		header << TAB() << name_ << "(EventTask *t);" << endl;
	}
    header << TAB() << "~" << name_ << "(void);" << endl;
    ilevel--;
    ilevel--;
    header << TAB() << "};" << endl << endl;
}

// Prints the Users or Providers Message Classes to header.
void SAPInterface::printMessageClasses(ostream &header)
{
    // first print enumeration for these messages
    
    if(firstMessage_ != 0)
    {
		SAPMessage *tmp = firstMessage_;

		if(tmp->getName() == "Req" || tmp->getName() == "Ind") {
			tmp = tmp->getNextMessage();
		}
	
		if(type_ == User)
		{
			header << TAB() << "enum ReqType { " << StrToLower(tmp->getName()) << " = 1";
		}
		else
		{
			header << TAB() << "enum IndType { " << StrToLower(tmp->getName()) << " = 1";
		}

		tmp = tmp->getNextMessage();

		while(tmp != 0)
		{
			header << ", " << StrToLower(tmp->getName());
			tmp = tmp->getNextMessage();
		}

		header << " };" << endl << endl;
    }
    
    // Second, print the class-declarations of each message belonging to this
    // interface.
    
    SAPMessage *tmp = firstMessage_;
    while(tmp != 0)
    {
		tmp->printHeader(header);
		tmp = tmp->getNextMessage();
    }
}
// Prints the class and all it's subclasses implementations to .cc file
void SAPInterface::printIfaceClassImpl(const string &SAPname, ostream &impl)
{
    // Print the create methods for each message.

    SAPMessage *tmp = firstMessage_;
    
    while( tmp != 0)
    {
		if(tmp->getName() != "Req" && tmp->getName() != "Ind" ) {
			impl << TAB() << "Message *create" << SAPname << tmp->getName() 
				 << " (void)" << endl;
			
			impl << TAB() << "{" << endl;
			ilevel++;
			impl << TAB() << "return new " << SAPname << " :: " << tmp->getName() 
				 << ";" << endl;
			ilevel--;
			impl << TAB() << "}" << endl << endl;
		}

		tmp = tmp->getNextMessage();
    }

    tmp = 0;
    
    // Second print the functionpointer thing, of each message belonging
    // to this Interface.
    
    impl << "Message *(*"
		 << (char)tolower(SAPname[0]) << SAPname.substr(1,SAPname.size())
		 << name_ << "Messages []) (void) =" 
		 << endl;
    impl << "{" << endl;
    ilevel++;

    tmp = firstMessage_;
    
    while(tmp != 0)
    {
		if(tmp->getName() != "Req" && tmp->getName() != "Ind" ) {
			impl << TAB() << "create" << SAPname << tmp->getName() 
				 << "," << endl;
		}
	
		tmp = tmp->getNextMessage();
    }
    
    impl << TAB() << "0" << endl;
    impl << "};" << endl << endl;
	ilevel--;
}
void SAPInterface::printIfaceMessagesImpl(const std::string &SAPname,
										  std::ostream &impl)
{
    // print the interface's constructor and destructor implementations
    // and implementations of messages belonging to this interface.
    
    // 1. Print the constructor's and destructor's implementation 
    
    impl << SAPname << " :: " << name_ << " :: " << name_;
    
    // Constructor is slightly different depending on interface type

    if(type_ == User)
    {
		impl << " (EventTask *t, Peer *p)" << endl;
		impl << ": " << parents_[0] << " (t, " << (char)tolower(SAPname[0])
			 << SAPname.substr(1,SAPname.size())
			 << name_ << "Messages, p)" << endl;
		impl << "{" << endl;
		impl << "}" << endl;
    }
    else
    {
		impl << "(EventTask *t)" << endl;
		impl << ": " << parents_[0] << " (t, " << (char)tolower(SAPname[0])
			 << SAPname.substr(1,SAPname.size())
			 << name_ << "Messages)" << endl;
		impl << "{" << endl;
		impl << "}" << endl;
    }
    
    impl << endl;
    
    impl << SAPname << " :: " << name_ << " :: ~" << name_ << "(void)" << endl;
    impl << "{" << endl;
    impl << "}" << endl;
    
    // 2. Print the message implementations.
    
    SAPMessage *tmp = firstMessage_;
    
    while(tmp != 0)
    {
		tmp->printImpl(SAPname, impl);
		tmp = tmp->getNextMessage();
    }
}
