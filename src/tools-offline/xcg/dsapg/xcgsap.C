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

$Log: xcgsap.C,v $
Revision 1.2  2002/07/23 17:50:11  hartmanj
Development

Revision 1.1  2002/06/17 13:29:55  bilhanan
*** empty log message ***

Revision 1.2  2002/06/17 07:33:23  hartmanj
Renamed sap.h/C to xcgsap.h/C

Revision 1.1  2002/06/17 07:32:33  hartmanj
Renamed sap.h/C to xcgsap.h/C

Revision 1.3  2002/06/14 13:41:30  hartmanj
Changed #includes

Revision 1.2  2002/06/07 12:19:08  hartmanj
Full licence clause added


*/


// SAP Implementation

//060202 petri Draft version
//100202 petri Print Header implemented -- quite ready.
//             Printing implementation implemented...
//110202 petri Class-name changed: OwnMain -> SAP
//250202 petri Commented and made some non.functional changes.
//070402 petri Inherited from Protocol Layer functionality
//             baseclass Structure.

#include <cstdlib>
#include <doors/xcgsap.h>
#include <doors/sapinterface.h>
#include <doors/xmldocument.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ostream;

string TAB()
{
    string s;
    for(int i=0;i<ilevel;i++)
		s.append("  ");
    return s;
}


SAP::SAP(const string &name) : Structure(), name_(name), parents_(), user_(0), provider_(0)
{
    // nothing to do.
}
SAP::~SAP()
{
    // Delete the User and Provider Interface's (classes).
    
    if(user_ != 0)
    {
		delete user_;
    }
    if(provider_ != 0)
    {
		delete provider_;
    }
}
void SAP::addParent(const string &parent)
{
    // Add the new parent at the end of the parents vector.
    parents_.push_back(parent);
}
void SAP::setUserInterface(SAPInterface *user)
{
    // Links the new UserInterface to SAP, replaces the possible old one.
    user_ = user;
}
void SAP::setProviderInterface(SAPInterface *provider)
{
    // Links the new ProviderInterface to SAP, replaces the possible old one.
    provider_ = provider;
}
SAPInterface* SAP::getUser() const
{
    // Returns pointer to UserInterface.
    return user_;
}
SAPInterface* SAP::getProvider() const
{
    // Returns pointer to ProviderInterface
    return provider_;
}
void SAP::printHeader(ostream &header)
{
    // Prints the class 
    //and all it's subclasses(SAPInterfaces and their messages to header)
 
    // Print's declaration of Ovops specifig classes.
    header << TAB() << "class Report;" << endl;
    header << TAB() << "class SrcDesc;" << endl << endl;
    
    // Specifies the used indentation
    unsigned int idx = 0;
	while(idx < parents_.size() ) {
		header << TAB() << "class "
			   << parents_[idx] << ';' << std::endl;
	}
    
    // class name
    header << TAB() << "class " << name_;

    // print the public inheritance parents.
    // If multiple parents, then type, name pairs are separated by comma (C++)
    
	idx=0;
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
    header << TAB() << "public:" << endl << endl;
    ilevel++;
    // Print user-interface class specific data..
    // Done by calling the SAPInterface.PrintIfaceClass() methods.
    if(user_)
    {
		user_->printIfaceClass(header);
    }
    if(provider_)
    {
		provider_->printIfaceClass(header);
    }
    
    // Print the messages of each interface (User, Provider)
    if(user_)
    {
		user_->printMessageClasses(header);
    }
    
    // print the Provider-iterface classes.
    if(provider_)
    {
		provider_->printMessageClasses(header);
    }
    ilevel--;
    header << TAB() << "protected:" << endl;
    header << TAB() << "private:" << endl;
    ilevel--;
    header << TAB() << "};" << endl << endl;;


    // Print the getMessageType inline function implementations.
        
    
    SAPMessage *tmp = user_->getFirstMessage();
    
    while(tmp != 0)
    {
		if( tmp->getName() != "Req" ) {
			header << TAB() << "inline MessageType " << name_ << " :: " 
				   << tmp->getName() << " :: getType (void) const" << endl;
			header << TAB() << "{" << endl;
			ilevel++;
			header << TAB() << "return " << StrToLower(tmp->getName()) << ";" << endl;
			ilevel--;
			header << TAB() << "}" << endl << endl;
		}

		tmp = tmp->getNextMessage();
    }

    tmp = provider_->getFirstMessage();
    
    while(tmp != 0)
    {
		if( tmp->getName() != "Ind" ) {
			header << TAB() << "inline MessageType " << name_ << " :: " 
				   << tmp->getName() << " :: getType (void) const" << endl;
			header << TAB() << "{" << endl;
			ilevel++;
			header << TAB() << "return " << StrToLower(tmp->getName()) << ";" << endl;
			ilevel--;
			header << TAB() << "}" << endl << endl;
		}

		tmp = tmp->getNextMessage();
    }
	ilevel=0;
}
void SAP::printImpl(ostream &impl)
{
    // Prints the class and all it's subclasses
    // implementations to .cc file
    
    // 1. Print general info about interfaces.
    if(user_)
    {
		user_->printIfaceClassImpl(name_, impl);
    }
    if(provider_)
    {
		provider_->printIfaceClassImpl(name_, impl);
    }
    // 2. Print each of Interface-Class Messages implementation
    if(user_)
    {
		user_->printIfaceMessagesImpl(name_, impl);
    }
    if(provider_)
    {
		provider_->printIfaceMessagesImpl(name_, impl);
    }
}
