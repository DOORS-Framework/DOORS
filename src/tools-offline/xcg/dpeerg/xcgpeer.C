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

$Log: xcgpeer.C,v $
Revision 1.1  2002/06/17 13:29:54  bilhanan
*** empty log message ***

Revision 1.2  2002/06/17 08:16:00  hartmanj
#include <doors/peer.h> -> #include <doors/xcgpeer.h>

Revision 1.1  2002/06/17 08:14:51  hartmanj
Renamed peer.h/C to xcgpeer.h/C

Revision 1.3  2002/06/14 13:41:22  hartmanj
Changed #includes

Revision 1.2  2002/06/07 12:19:07  hartmanj
Full licence clause added


*/


// Peer-class implementation

//260202 petri Draft version


#include <cstdlib>
#include <vector>

#include <doors/xcgpeer.h>
#include <doors/peerinterface.h>

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


Peer::Peer(const string &name)
	: Structure(),
	  name_(name), parents_()
{

	parents_.push_back("Peer");
	// nothing to do.

}

Peer::~Peer()
{
    // Delete all possible interfaces..now only PDUInterface.
    
    if(PDU_ != 0)
    {
		delete PDU_;
    }
}
void Peer::addParent(const string &parent)
{
    // Add the new parent at the end of the parents vector.
    parents_.push_back(parent);
}
void Peer::setPeerInterface(PeerInterface *PDUs)
{
    // Links the new PDUInterface to Peer, replaces the possible old one.
    PDU_ = PDUs;
}
PeerInterface* Peer::getPeerInterface() const
{
    // Returns pointer to PeerInterface.
    return PDU_;
}

void Peer::printHeader(ostream &header)
{
    // Prints the class header
    //and all it's subclasses(PeerInterfaces and their messages) to header
    
    // class name
    header << TAB() << "class " << name_;
    
    // print the public inheritance parents.
    // If multiple parents, then type, name pairs are separated by comma (C++)
    
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
    header << TAB() << "public:" << endl << endl;
    ilevel++;
    
    // Printing Interface specific-data is strongly optional!
    // It might come in use if there are need of other types of interfaces
    // that have some things that need to bee printed. (Like SAPInterfaces)
    
    // Here in PeerPDUInterface this is not needed and not done, because
    // PeerPDUinterfaces don't even have printIfaceClass() methods implemented
    
    // Print PDUinterface class specific data..
    // Done by calling the PeerInterface.PrintIfaceClass() methods.
    
    // Print the messages of each interface (PDU)
    if(PDU_)
    {
		PDU_->printMessageClasses(header);
    }

    // Print the constructor and destructor.
    // Notice that constructors parametres are hardcoded HERE.

    header << TAB() << name_ << "(EventTask *host, ReqIface *d);" << endl;
    header << TAB() << "~" << name_ << "(void);" << endl << endl;
    
    header << TAB() << "Sint16 identify (Message *msg);" << endl << endl;
    
    ilevel--;
    header << TAB() << "protected:" << endl;
    header << TAB() << "private:" << endl;
    ilevel--;
    header << TAB() << "};" << endl << endl;;
 
}

void Peer::printImpl(ostream &impl)
{
    // 1. Print the createMessage methods
    // and the function pointer things...
    
    if(PDU_)
    {
		PDU_->printIfaceClassImpl(name_, impl);
    }
    
    // Print the Peer-class constructor and destructor.
    
    impl << TAB() << name_ << " :: " << name_ << " (EventTask *host, ReqIface *d)"
		 << endl;
    impl << TAB() << " : " << parents_[0] << " (host, "
		 << (char)tolower(name_[0]) << name_.substr(1,name_.size())
		 << "Messages, d)" << endl;
    impl << TAB() << "{" << endl;
    ilevel++;
    ilevel--;
    impl << TAB() << "}" << endl << endl;
    impl << TAB() << name_ << " :: ~" << name_ << " ()" << endl;
    impl << TAB() << "{" << endl;
    ilevel++;
    ilevel--;
    impl << TAB() << "}" << endl;
    
    if(PDU_)
    {
		PDU_->printIfaceMessagesImpl(name_, impl);
    }
}
