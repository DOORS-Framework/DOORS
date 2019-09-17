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

$Log: peermessage.C,v $
Revision 1.7  2003/05/08 08:01:55  bilhanan
test and fix for empty parent tag.

Revision 1.6  2002/10/24 10:30:10  kalliosa
Added copy constructor calls to messages' copyconstructor.

Revision 1.5  2002/08/08 12:38:59  hartmanj
we really must print getType()s to .i file also with --nomacros

Revision 1.4  2002/07/23 14:17:40  hartmanj
Fixed pointer variable handling

Revision 1.3  2002/07/23 13:13:10  hartmanj
Fixed nomacros bug

Revision 1.2  2002/06/18 12:23:32  ik
string->std::string etc fixes.

Revision 1.1  2002/06/17 13:29:54  bilhanan
*** empty log message ***

Revision 1.3  2002/06/14 13:41:23  hartmanj
Changed #includes

Revision 1.2  2002/06/07 12:19:07  hartmanj
Full licence clause added


*/


// PeerMessage-class Implementation

//260202 petri Draft version


#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctype.h>
#include <doors/peermessage.h>
#include <doors/xmldocument.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ostream;

// Global-variable, that indicates if macros should be printed or not.
extern int no_macros;
extern int ilevel;
extern string TAB(void);

PeerMessage::PeerMessage(const string &name) : XcgMessage(), name_(name), parents_(),
	members_(), nextMessage_(0)
{
    // Message is always inherited from class PDU.
    parents_.push_back("PDU");
    // nothing else to do.
}
PeerMessage::~PeerMessage()
{
    // Nothing to do...
    // This dynamic memory is deleted at the destructor of PeerInterface.
}
// Adds parent to class.
void PeerMessage::addParent(const string &parent)
{
  if (parent.empty() == true) {
    // an empty parent tag here, do nothing
    return;
  }
    parents_.push_back(parent);
}
// Returns the next message in list.
PeerMessage* PeerMessage::getNextMessage() const
{
    return nextMessage_;
}
// Sets the message next to this one.
void PeerMessage::setNextMessage(PeerMessage *message)
{
    nextMessage_ = message;
}
// Adds member data to this message.
void PeerMessage::addMemberData(const string &member)
{
    members_.push_back(member);
}
// Returns the name of this message.
string PeerMessage::getName() const
{
    return name_;
}
// Print declaration of this message to header-stream.
void PeerMessage::printHeader(ostream &header)
{
    // class name
    header << TAB() << "class " << name_ ;  
    
    // print the public inheritance parents.
    
    unsigned int idx = 0;
    if(!parents_.empty())
    {
		header << " : ";
		while(idx < parents_.size())
		{
			header << "public " << parents_[idx];
	    
			if(idx+1 < parents_.size())
			{ 
				header << ", ";
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
    // print constructor without parametres, desctructor (not virtual) and
    // copyconstructor.
    
    header << TAB() << name_ << " (void);" << endl;
    header << TAB() << name_ << " (const " << name_ << " &msg);" << endl;
    header << TAB() << "~" << name_ << " (void);" << endl << endl;
    
    // print Clone-method;
    header << TAB() << "Message *clone (void) const;" << endl << endl;
    // print getType-method
    header << TAB() << "MessageType getType (void) const;" << endl << endl;
    // print decode-method
    header << TAB() << "Uint16 decode (Message *msg);" << endl << endl;
    // print encode-method
    header << TAB() << "Message *encode (void);" << endl << endl;
    
    // print datamembers as a private.
    
    // idx = 0 taas
    if(!members_.empty())
    {
		while(idx < members_.size())
		{
			header << TAB() << members_[idx] << ";" << endl;
			idx++;
		}
		idx = 0;
    }
    ilevel--;
    ilevel--;
    
    header << TAB() << "};" << endl << endl;
}
// Print the implementation of message-class to implementation-stream.
void PeerMessage::printImpl(const string name, ostream &impl)
{
    unsigned int idx = 0;
    
    // Structure is dependendt of no_macros-global variable.
    // If no_macros is set -> no DYNAMIC or STATIC symbols are printed.
    // Otherwise they are.
    
    if(!no_macros)
    {
		// Message's Constructor ...
	
		impl << TAB() << name << " :: " << name_ << " :: " << name_ 
			 << " (void)" << endl;
		impl << TAB() << "{" << endl;
		ilevel++;
		impl << TAB() << "DYNAMIC_SYMBOL (this, \"" << toLower(name_)
			 << "\", 0);" << endl;
	
		while( idx < members_.size())
		{
			if(members_[idx].find("*") == string::npos)
			{
			  impl << TAB() << "STATIC_SYMBOL (" << getVarName(members_[idx]) 
				   << ", this);" << endl; 
			}
			idx++;
		}
		idx = 0;
	
		// Is this needed in Peer??
	
		// Initialize possible pointer-type members to zero.
		while( idx < members_.size())
		{
			if(members_[idx].find("*") != string::npos)
			{
				impl << TAB() << getVarName(members_[idx]) << " = 0;" << endl; 
			}
			idx++;
		}
		idx = 0;
		ilevel--;
		impl << TAB() << "}" << endl << endl;
	
		// Message's copyconstructor ...
	
		impl << name << " :: " << name_ << " :: " << name_ << "(const " 
			 << name_ << " &msg)" << endl;
       
       if( !parents_.empty() || !members_.empty() )
       {		
	  impl << ": ";	  
	  while( idx < parents_.size() )
	  {
	     impl << parents_[idx] << "(msg)";
	     if( idx+1 < parents_.size() )
	       impl << ",";
	     idx++;
	  }
	  idx = 0;
	  if(!members_.empty())
		{
		   impl << ", ";
			while(idx < members_.size())
			{
				impl << getVarName(members_[idx]) << " (msg." 
					 << getVarName(members_[idx]) << ")";
				if(idx+1 < members_.size())
				{
					impl << ",";
				}
				idx++;
			}
			impl << endl;
			idx = 0;
		}
	  
       }     
       
                impl << TAB() << "{" << endl;
		ilevel++;
		impl << TAB() << "DYNAMIC_SYMBOL (this, \"" << toLower(name_) 
			 << "\", 0);" << endl;
	
		while( idx < members_.size())
		{
			if(members_[idx].find("*") == string::npos)
			{
			  impl << TAB() << "STATIC_SYMBOL (" << getVarName(members_[idx]) 
				   << ", this);" << endl; 
			}
			idx++;
		}
		idx = 0;
		// Initialize possible pointer-type members to zero.
		while( idx < members_.size())
		{
			if(members_[idx].find("*") != string::npos)
			{
				impl << TAB() << getVarName(members_[idx]) << " = 0;" << endl; 
			}
			idx++;
		}
		idx = 0;
		ilevel--;
		impl << TAB() << "}" << endl << endl;
	
		// Message's destructor
	
		impl << TAB() << name << " :: " << name_ << " :: ~" << name_ 
			 << " (void)" << endl;
		impl << TAB() << "{" << endl;
		ilevel++;
		impl << TAB() << "DELETE_SYMBOL (this);" << endl;
	
	
		// delete dynamic memory at the end of pointers...
	
		while( idx < members_.size())
		{
			if(members_[idx].find("*") != string::npos)
			{
				impl << TAB() << "delete " << getVarName(members_[idx])
					 << ';' << std::endl;
			}
			idx++;
		}
		idx = 0;
	
		ilevel--;
		impl << TAB() << "}" << endl << endl;

		// Message's getType() function
		impl << TAB() << "MessageType " << name << " :: " << name_
			 << " :: getType(void) const" << endl;
		impl << TAB() << "{" << endl;
		ilevel++;
		// StrToLower(name_) is actually enum name of name_(message class)
		impl << TAB() << "return " << StrToLower(name_) << ";" << endl;
		ilevel--;
		impl << TAB() << "}" << endl;

		// Message's clone function.
	
		impl << TAB() << "Message *" << name << " :: " << name_ 
			 << " :: clone (void) const" << endl;
	
		impl << TAB() << "{" << endl;
		ilevel++;
		impl << TAB() << "return new " << name_ << "(*this);" << endl;
		ilevel--;
		impl << TAB() << "}" << endl << endl;
    }   
    else
    {
		// Now macros [DYNAMIC_SYMBOLS, STATIC_SYMBOLS, are not printed]
	
		// Message's Constructor ...
	
		impl << TAB() << name << " :: " << name_ << " :: " << name_ 
			 << " (void)" << endl;
		impl << TAB() << "{" << endl;
		ilevel++;
		// Is this needed in Peer??
	
		// Initialize possible pointer-type members to zero.
		while( idx < members_.size())
		{
			if(members_[idx].find("*") != string::npos)
			{
				impl << TAB() << getVarName(members_[idx]) << " = 0;" << endl; 
			}
			idx++;
		}
		idx = 0;
		ilevel--;
		impl << TAB() << "}" << endl << endl;
	
		// Message's copyconstructor ...
	
		impl << name << " :: " << name_ << " :: " << name_ << "(const " 
			 << name_ << " &msg)" << endl;

       if( !parents_.empty() || !members_.empty() )
       {		
	  impl << ": ";	  
	  while( idx < parents_.size() )
	  {
	     impl << parents_[idx] << "(msg)";
	     if( idx+1 < parents_.size() )
	       impl << ",";
	     idx++;
	  }
	  idx = 0;
	  if(!members_.empty())
		{
		   impl << ", ";

			while(idx < members_.size())
			{
				impl << getVarName(members_[idx]) << " (msg." 
					 << getVarName(members_[idx]) << ")";
				if(idx+1 < members_.size())
				{
					impl << ",";
				}
				idx++;
			}
			impl << endl;
			idx = 0;
		}
       }
       
		impl << TAB() << "{" << endl;
		ilevel++;
		ilevel--;
		impl << TAB() << "}" << endl << endl;
	
		// Message's destructor
	
		impl << TAB() << name << " :: " << name_ << " :: ~" << name_ 
			 << " (void)" << endl;
		impl << TAB() << "{" << endl;
		ilevel++;
		// delete dynamic memory at the end of pointers...
	
		while( idx < members_.size())
		{
			if(members_[idx].find("*") != string::npos)
			{
				impl << TAB() << "delete " << getVarName(members_[idx]) 
					 << ";" << std::endl;
			}
			idx++;
		}
		idx = 0;
		ilevel--;
		impl << TAB() << "}" << endl << endl;

		// Message's getType() function
		impl << TAB() << "MessageType " << name << " :: " << name_
			 << " :: getType(void) const" << endl;
		impl << TAB() << "{" << endl;
		ilevel++;
		// StrToLower(name_) is actually enum name of name_(message class)
		impl << TAB() << "return " << StrToLower(name_) << ";" << endl;
		ilevel--;
		impl << TAB() << "}" << endl;

		impl << std::endl;

		// Message's clone funktion.
	
		impl << TAB() << "Message *" << name << " :: " << name_ 
			 << " :: clone (void) const" << endl;
	
		impl << TAB() << "{" << endl;
		ilevel++;
		impl << TAB() << "return new " << name_ << "(*this);" << endl;
		ilevel--;
		impl << TAB() << "}" << endl << endl;
    }
}
