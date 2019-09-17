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

$Log: sapmessage.C,v $
Revision 1.8  2004/05/25 13:23:30  hartmanj
Fixed copyconstructor generation when message has parents.

Revision 1.7  2004/05/25 13:21:43  hartmanj
Fixed horrible indentation. No changes in code

Revision 1.6  2004/04/29 05:30:28  hartmanj
Added an explicit call to Message(&msg) in copyconstructor.

Revision 1.5  2003/05/07 13:21:18  bilhanan
Modification to perform better symbol handling in parent classes.
Code fix to detect empty XML parent tags.

Revision 1.4  2002/07/30 12:25:22  hartmanj
--nomacros was broken and produced badbad code

Revision 1.3  2002/07/23 14:17:40  hartmanj
Fixed pointer variable handling

Revision 1.2  2002/06/18 12:25:22  ik
string->std::string etc fixes.

Revision 1.1  2002/06/17 13:29:55  bilhanan
*** empty log message ***

Revision 1.3  2002/06/14 13:41:31  hartmanj
Changed #includes

Revision 1.2  2002/06/07 12:19:08  hartmanj
Full licence clause added


*/


// SAPMessage-class Implementation

// Updates before file was inserted to DOORS-CVS
//060202 petri Draft version
//100202 petri PrintHeader implemented -- quite ready.
//       petri PrintImpl implemented
//110202 petri Class-name changed: OwnMessage -> SAPMessage.
//250202 petri Commented and made some non-functional changes.
//060402 petri inherited from baseclass Message

#include <cstdlib>
#include <ctype.h>
#include <doors/sapmessage.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ostream;

// Global-variable, that indicates if macros should be printed or not.
extern int no_macros;

SAPMessage::SAPMessage(const std::string &name, const std::string base_class)
    : XcgMessage(), name_(name), parents_(), members_(), nextMessage_(0)
{
    // SAP Message is always inherited from class Message unless parents are given
    // User interface messages derive from Req and Provider messages from Ind
    parents_.push_back(base_class); // is this always Message? Let's hope so.
    parenttag = false; // initial assumption is non existence of parent tag.
    // nothing to do.
}

SAPMessage::~SAPMessage()
{
    // Nothing to do...
    // This dynamic memory is deleted at the destructor of SAPInterface.
}

// Adds parent to class.
void SAPMessage::addParent(const std::string &parent)
{
    // aha! Developer defines parents, let's take some action

    if (parent.empty() == true) {
        // an empty parent tag here, do nothing
        return;
    }

    if (parenttag == true) {
        // If parenttag has already been set, just add
        parents_.push_back(parent);

    } else {   // first delete DOORS Message public inheritence

        std::string teststring = "Message";
        if ((teststring.compare (*parents_.begin())) == 0) {
            parents_.erase (parents_.begin());
        }  
        parents_.push_back(parent);
        parenttag = true;
    }

}
// Returns the next message in list.
SAPMessage* SAPMessage::getNextMessage() const
{
    return nextMessage_;
}

// Sets the message next to this one.
void SAPMessage::setNextMessage(SAPMessage *message)
{
    nextMessage_ = message;
}

// Adds member date to this message.
void SAPMessage::addMemberData(const std::string &member)
{
    members_.push_back(member);
}

// Returns the name of this message.
std::string SAPMessage::getName() const
{
    return name_;
}

// Print declaration of this message to header-stream.
void SAPMessage::printHeader(ostream &header)
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

    header << TAB() << "~" << name_ << " (void);" << endl << endl;

    if( name_ != "Req" && name_ != "Ind" ) {
        header << TAB() << name_ << " (const " << name_ << " &msg);" << endl;
        // print Clone-method;
        header << TAB() << "Message *clone (void) const;" << endl << endl;
        // print getType-method
        header << TAB() << "MessageType getType (void) const;" << endl << endl;
    }

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
void SAPMessage::printImpl(const std::string name, ostream &impl)
{
    unsigned int idx = 0;
    
    // Structure is dependendt of no_macros-global variable.
    // If no_macros is set -> no DYNAMIC or STATIC symbols are printed.
    // Otherwise they are.
    
    if(!no_macros)
    {
        // Message's Constructor ...
	
        impl << name << " :: " << name_ << " :: " << name_ 
             << " (void)" << endl;
        impl << "{" << endl;
        ilevel++;

        if( name_ != "Req" && name_ != "Ind" ) {
            impl << TAB() << "DYNAMIC_SYMBOL (this, \"" << toLower(name_)
                 << "\", 0);" << endl;
        } else { impl << TAB() << "cid = 0;" << endl; }

        while( idx < members_.size())
        {
            if(members_[idx].find("*") == std::string::npos)
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
            if(members_[idx].find("*") != std::string::npos)
            {
                impl << TAB() << getVarName(members_[idx]) << " = 0;" << endl; 
            }
            idx++;
        }
        idx = 0;
	
        impl << "}" << endl << endl;
        ilevel--;

        // Message's copyconstructor ...

        if( name_ != "Req" && name_ != "Ind" ) {
            impl << name << " :: " << name_ << " :: " << name_ << "(const " 
                 << name_ << " &msg)" << endl;

            if( !parenttag ) {
                impl << ": Message(msg)";
            } else {
                impl << ": " << parents_[0] << "(msg)";
                for( unsigned int i=1;i<parents_.size();i++) {
                    impl << ", " << parents_[i] << "(msg)";
                }
            }

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
		  
            impl << "{" << endl;
            ilevel++;
            impl << TAB() << "DYNAMIC_SYMBOL (this, \"" << toLower(name_) 
                 << "\", 0);" << endl;
		
            while( idx < members_.size())
            {
                if(members_[idx].find("*") == std::string::npos)
                {
                    impl << TAB() << "STATIC_SYMBOL (" << getVarName(members_[idx]) 
                         << ", this);" << endl; 
                }
                idx++;
            }
            idx = 0;
		
            impl << "}" << endl << endl;
            ilevel--;
        }
        // Message's clone function.
        if( name_ != "Req" && name_ != "Ind" ) {
            impl << "Message *" << name << " :: " << name_ 
                 << " :: clone (void) const" << endl;

            impl << "{" << endl;
            ilevel++;
            impl << TAB() << "return new " << name_ << "(*this);" << endl;
            impl << "}" << endl << endl;
            ilevel--;
        }

        // Message's destructor

        impl << name << " :: " << name_ << " :: ~" << name_ << " (void)" << endl;
        impl << "{" << endl;
        ilevel++;
        if(name_!="Req" && name_!="Ind") {
            impl << TAB() << "DELETE_SYMBOL (this);" << endl;
        }
        // delete dynamic memory at the end of pointers...
		
        while( idx < members_.size())
        {
            if(members_[idx].find("*") != std::string::npos)
            {
                impl << TAB() << "delete " << getVarName(members_[idx]) << ';' << endl;
            }
            idx++;
        }
        idx = 0;
		
        impl << "}" << endl << endl;
        ilevel--;

    }
    else
    {
        // Now macros [DYNAMIC_SYMBOLS, STATIC_SYMBOLS, are not printed]
	
        // Message's Constructor ...
	
        impl << name << " :: " << name_ << " :: " << name_ << " (void)" 
             << endl;
        impl << "{" << endl;
        ilevel++;

        // Initialize possible pointer-type members to zero.
	
        while( idx < members_.size())
        {
            if(members_[idx].find("*") != std::string::npos)
            {
                impl << TAB() << getVarName(members_[idx]) << " = 0;" << endl; 
            }
            idx++;
        }
        idx = 0;

        ilevel--;

        impl << TAB() << '}' << endl << endl;

        // Message's copyconstructor ...
        impl << name << " :: " << name_ << " :: " << name_ << "(const " 
             << name_ << " &msg)" << endl;
		
        if(!members_.empty())
        {
            impl << ": ";
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
		
        impl << "{" << endl;
        ilevel++;
		
        // delete dynamic memory at the end of pointers...
        while( idx < members_.size())
        {
            if(members_[idx].find("*") != std::string::npos)
            {
                impl << TAB() << "delete " << getVarName(members_[idx]) << ';' << endl;
            }
            idx++;
        }
        idx = 0;
		
        impl << "}" << endl << endl;
        ilevel--;

        // end of copyconstructor
		
        // Message's clone function.
		
        impl << "Message *" << name << " :: " << name_ 
             << " :: clone (void) const" << endl;
		
        impl << "{" << endl;
        ilevel++;
        impl << TAB() << "return new " << name_ << "(*this);" << endl;
        impl << "}" << endl << endl;
        ilevel--;

        // Message's destructor
		
        impl << name << " :: " << name_ << " :: ~" << name_ << " (void)" 
             << endl;
        impl << TAB() << "{" << endl;
        impl << TAB() << '}' << endl << endl;

    }
}

