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

$Log: sapgenerator.C,v $
Revision 1.6  2003/10/08 14:34:35  hartmanj
Fixed a seg fault, which occured when input file was not a SAP-file

Revision 1.5  2003/05/14 15:06:54  bilhanan
std::ostream members should really be std::ofstream

Revision 1.4  2002/07/25 14:43:12  hartmanj
Replaced IncludeFiles with HIncludeFiles and CIncludeFiles attributes

Revision 1.3  2002/07/23 17:50:11  hartmanj
Development

Revision 1.2  2002/06/18 12:25:22  ik
string->std::string etc fixes.

Revision 1.1  2002/06/17 13:29:55  bilhanan
*** empty log message ***

Revision 1.3  2002/06/14 13:41:30  hartmanj
Changed #includes

Revision 1.2  2002/06/07 12:19:08  hartmanj
Full licence clause added


*/


#include <fstream>
#include <iostream>
#include <cstdio>
#include <ctype.h>

#include <doors/sapgenerator.h>

const int CINCLUDE = 0;
const int HINCLUDE = 1;

SAPGenerator::SAPGenerator( XMLDocument *doc, std::ofstream &hh_file,
							std::string hh_fname,std::ofstream &cc_file )
    : doc_(doc), hh_file_(hh_file), cc_file_(cc_file), msg_base_name_("Message"),
	  hh_fname_(hh_fname)
{
    sap_ = NULL;
    useri_ = NULL;
    provi_ = NULL;
    classname_ = "";
}

SAPGenerator::~SAPGenerator()
{
}

int SAPGenerator::generateSAP( )
{
     doc_->getRootNode();
     while( doc_->getNodeName() == "comment" ) {
          doc_->getNextNode();
     }
     
     if( !Strcmp(doc_->getNodeName(),"SAP") )
          // Lets see if we find <SAP> in the tree
     {
          // Process attributes of <SAP>
          processAttributes( );
          processSAP();
     }
     else {
          //throw SAPNotFound("No SAP element found");
          std::cerr << "No SAP element found!" << std::endl;
          return 1;
     }

    return 0;
}

void SAPGenerator::createHeaderFile() const
{
    put_hh_headers();
    // print include statements
    hh_file_ << "#include <doors/ptb.h>" << std::endl;

	unsigned int i=0;
    while(i<hincludes_.size()) {
		if( isDoorsInc(hincludes_[i]) ) {
			hh_file_ << "#include <" << cutWSpace(hincludes_[i++]) << ">" << std::endl;
		} else {
			hh_file_ << "#include \"" << cutWSpace(hincludes_[i++]) << "\"" << std::endl;
		}
	}

    hh_file_ << std::endl;

    sap_->printHeader(hh_file_);
    put_hh_endings();
}


void SAPGenerator::createImplFile() const
{
    unsigned int i=0;
	cc_file_ << std::endl;
    cc_file_ << "#include <doors/hsi.h>" << std::endl;
    while(i<cincludes_.size()) {
		if( isDoorsInc(cincludes_[i]) ) {
			cc_file_ << "#include <" << cutWSpace(cincludes_[i++]) << ">" << std::endl;
		} else {
			cc_file_ << "#include \"" << cutWSpace(cincludes_[i++]) << "\"" << std::endl;
		}
	}

	cc_file_ << "#include \"" << hh_fname_ << '"' << std::endl;

	cc_file_ << std::endl;

    sap_->printImpl(cc_file_);
}

std::string SAPGenerator::cutWSpace(std::string s) const
{
	// Indices of the first and last char of string(take away white space in beg and end
	unsigned int not_wspace_first=0, not_wspace_last=s.size()-1;

	while(not_wspace_first < s.size() && isspace(s[not_wspace_first]) ) {
		not_wspace_first++;
	}

	while( not_wspace_last > 0 && isspace(s[not_wspace_last]) ) {
		not_wspace_last--;
	}

	return s.substr(not_wspace_first, not_wspace_last-not_wspace_first+1);
}

bool SAPGenerator::isDoorsInc( std::string inc ) const
{
	if( cutWSpace(inc).substr(0,6) == "doors/" ) {
		return true;
	}

	return false;
}

void SAPGenerator::processAttributes()
{
    hh_file_ << std::endl;

    bool P = doc_->getFirstAttribute();

    while( P )
    {
		if( !Strcmp( doc_->getAttributeName(), "Name") )
		{
			// save the classname to member variable
			classname_ = doc_->getAttributeValue();
			break;
		}

		P = doc_->getNextAttribute();
    }

    P = doc_->getFirstAttribute();

    while( P )
    {
		if( !Strcmp( doc_->getAttributeName(), "CIncludeFiles") )
		{
			addToIncludes(doc_->getAttributeValue(), CINCLUDE);
		} else if( !Strcmp( doc_->getAttributeName(), "HIncludeFiles") ){
			addToIncludes(doc_->getAttributeValue(), HINCLUDE);
		} else if( !Strcmp( doc_->getAttributeName(), "IncludeFiles") ){
			std::cerr << "Warning: Deprecated attribute IncludeFiles."
				" Use HIncludeFiles and CIncludeFiles instead!" << std::endl;
			addToIncludes(doc_->getAttributeValue(), CINCLUDE);
		}

		P = doc_->getNextAttribute();
    }

    if( classname_ == "" )
    {
		std::cerr << "Critical error: No SAP NAME defined!\n" << std::endl;
		exit(1);
    }
}

void SAPGenerator::addToIncludes(std::string s, int includevec)
{
	std::string temp="";
	std::string::iterator i = s.begin();

	// go through the include list
	while( i != s.end() ) {
		// extract one include and add it to includes_
		while( (*i) != ',' && i != s.end() ) {
			temp += (*i);
			i++;
		}

		if( includevec == CINCLUDE ) {
			cincludes_.push_back(temp);
		} else {
			hincludes_.push_back(temp);
		}

		temp = "";
		if(i != s.end() ) {
			i++; // skip ','
		}
	}
}

void SAPGenerator::processSAP()
{
    // create the SAP class
    sap_ = new SAP(classname_);

    bool P = doc_->getChildNode();

    while( P )
    {
		if( !Strcmp( doc_->getNodeName(), "User" ) )
		{
			// We found User access point
			useri_ = new SAPInterface(SAPInterface::User);
			sap_->setUserInterface(useri_);
			processUser();
		}
		if( !Strcmp( doc_->getNodeName(), "Provider" ) )
		{
			// We found Provider access point
			provi_ = new SAPInterface(SAPInterface::Provider);
			sap_->setProviderInterface(provi_);
			processProvider();
		}
		// Might be more access points..
		P = doc_->getNextNode();

    }
}

void SAPGenerator::processUser()
{
    // Go beyond the User
    if(doc_->getChildNode())
    {
		// while we don't exit the branch back to User
		while( Strcmp(doc_->getNodeName(), "User" ) )
		{
			if( !Strcmp( doc_->getNodeName(), "Message" ) )
			{
				processMessage(useri_);
			} else if( !Strcmp( doc_->getNodeName(), "Address" ) ) {
				// Add a father class for messages to interface.
				// This is an OVOPS-compatibility hack
				SAPMessage *m_ = new SAPMessage("Req", "Message");
				m_->addMemberData(doc_->getNodeText());
				useri_->addMessage(m_);
				msg_base_name_="Req";
			}
	    
			// Choose the next node
			if( !doc_->getNextNode() )
				doc_->getParentNode();
		}
    }
}

void SAPGenerator::processMessage( SAPInterface *sapiface )
{
    std::string mesname = "";
    if( doc_->getFirstAttribute() )
    {
		if( !Strcmp( doc_->getAttributeName(), "Name" ) )
			mesname = doc_->getAttributeValue();
    }

    // Create a new message
	SAPMessage *message=0;
	message = new SAPMessage(mesname, msg_base_name_);

	sapiface->addMessage(message);

    // go through the member fields of the message definition
    bool P = doc_->getChildNode();
    while( P )
    {
		if( !Strcmp( doc_->getNodeName(), "Field" ) )
		{
			message->addMemberData(doc_->getNodeText());
		}


		if( !Strcmp( doc_->getNodeName(), "Parent" ) )
		{
			message->addParent(doc_->getNodeText());
		}

		P = doc_->getNextNode();
    }

    // add the message to the interface
    sapiface->addMessage(message);

    // Return to the correct level
    doc_->getParentNode();

}

void SAPGenerator::processProvider()
{
	msg_base_name_ = "Message";
    doc_->getChildNode();
    while( Strcmp(doc_->getNodeName(), "Provider" ) )
    {
		if( !Strcmp( doc_->getNodeName(), "Message" ) ) {
			processMessage(provi_);
		} else if( !Strcmp( doc_->getNodeName(), "Address" ) ) {
			// Add a father class for messages to interface.
			// This is an OVOPS-compatibility hack
			SAPMessage *m_ = new SAPMessage("Ind", "Message");
			m_->addMemberData(doc_->getNodeText());
			provi_->addMessage(m_);
			msg_base_name_="Ind";
		}

		// Choose the next node
		if( !doc_->getNextNode() )
			doc_->getParentNode();
    }
}

void SAPGenerator::put_hh_headers() const
{
    // here should go the Copyright

    // print the stuff in the beginning of the hh file
    hh_file_ << "#ifndef _";
    unsigned int i=0;
    while( i < classname_.size() )
		hh_file_ << (char)tolower(classname_.at(i++));

    hh_file_ << "_h_" << std::endl;

    hh_file_ << "#define _";
    i=0;
    while( i < classname_.size() )
		hh_file_ << (char)tolower(classname_.at(i++));

    hh_file_ << "_h_\n\n" << std::endl;

}

void SAPGenerator::put_hh_endings() const
{
    hh_file_ << "\n\n#endif" << std::endl;
}
