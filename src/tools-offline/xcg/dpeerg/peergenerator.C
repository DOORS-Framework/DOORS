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

$Log: peergenerator.C,v $
Revision 1.8  2003/05/15 11:43:38  bilhanan
fixes to prevent gcc-compiled binaries from spitting out empty .h files

Revision 1.7  2002/08/23 13:06:24  hartmanj
Development

Revision 1.6  2002/07/25 15:31:39  hartmanj
Changed CIncludeFiles to IIncludeFiles

Revision 1.5  2002/07/25 14:43:11  hartmanj
Replaced IncludeFiles with HIncludeFiles and CIncludeFiles attributes

Revision 1.4  2002/07/23 17:50:10  hartmanj
Development

Revision 1.3  2002/07/09 11:54:49  hartmanj
Changed the 'grammar' a bit. Peer is described now like this: <Peer> <Message Name=MsgName> <Parent>...</Parent> <Field>...</Field> ... </Message> ... </Peer>

Revision 1.2  2002/06/18 12:23:31  ik
string->std::string etc fixes.

Revision 1.1  2002/06/17 13:29:54  bilhanan
*** empty log message ***

Revision 1.3  2002/06/14 13:40:32  hartmanj
void generatePeer() to int generatePeer()

Revision 1.2  2002/06/07 12:19:07  hartmanj
Full licence clause added


*/

#include <fstream>
#include <vector>
#include <string>

#include <doors/xmldocument.h>
#include <doors/peergenerator.h>


const int HINCLUDE = 0;
const int IINCLUDE = 1;

//  PeerGenerator::PeerGenerator() : hh_file_(std::cout), i_file_(std::cout)
//  {
//  }

PeerGenerator::PeerGenerator( XMLDocument *doc, std::ofstream &hh_file,
							  std::string hh_fname, std::ofstream &i_file )
  : hh_file_(hh_file), i_file_(i_file), doc_(doc), peer_(NULL), peeri_(NULL),
  hh_fname_(hh_fname)
{
}

PeerGenerator::~PeerGenerator()
{
}


int PeerGenerator::generatePeer()
{
	doc_->getRootNode();
	while( doc_->getNodeName() == "comment" ) {
		doc_->getNextNode();
	}

    if( !Strcmp(doc_->getNodeName(), "Peer") )
    {
		processAttributes();
		processPeer();
		return 0;
    }

    std::cerr << "No Peer element found in XML!(or xml-file corrupted or not accessible)" << std::endl;

	return 1;
}

void PeerGenerator::processAttributes()
{
    hh_file_ << std::endl;

    bool P = doc_->getFirstAttribute();

    while( P )
    {
		if( !Strcmp( doc_->getAttributeName(), "Name") )
		{
			// save the classname to member variable
			classname_ = doc_->getAttributeValue();
		}

		P = doc_->getNextAttribute();
    }

    P = doc_->getFirstAttribute();

    while( P )
    {
		if( !Strcmp( doc_->getAttributeName(), "HIncludeFiles") ) {
			addToIncludes(doc_->getAttributeValue(), HINCLUDE);
		} else if( !Strcmp( doc_->getAttributeName(), "IIncludeFiles") ) {
			addToIncludes(doc_->getAttributeValue(), IINCLUDE);
		} else if( !Strcmp( doc_->getAttributeName(), "IncludeFiles") ){
			std::cerr << "Warning: Deprecated attribute IncludeFiles."
				" Use HIncludeFiles and IIncludeFiles instead!" << std::endl;
			addToIncludes(doc_->getAttributeValue(), IINCLUDE);
		}


		P = doc_->getNextAttribute();
    }

    if( classname_ == "" )
    {
		std::cerr << "Critical error: No Peer NAME defined!\n" << std::endl;
		exit(1);
    }
}

void PeerGenerator::addToIncludes(std::string s, int includevec)
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
		if(includevec == IINCLUDE) {
		  iincludes_.push_back(temp);
		} else {
		  hincludes_.push_back(temp);
		}
		temp = "";
		if(i != s.end() ) {
			i++; // skip ','
		}
	}
}

void PeerGenerator::processPeer()
{
    // create the Peer class
    peer_ = new Peer(classname_);

    bool P = doc_->getChildNode();

	if( doc_->getNodeName() == "text" ) {
		doc_->getNextNode();
	}

	// Create a PeerInterface in peer_
	peeri_ = new PeerInterface(PeerInterface::PDU);
	peer_->setPeerInterface(peeri_);

	// Go through the messages in the Peer-element
    while( P )
    {
		// Element should be <Message> now
		processMessage();

		// Next message (skip text-element first)
		P = doc_->getNextNode();
		P = doc_->getNextNode();
    }
}

void PeerGenerator::processMessage()
{
	if( !Strcmp( doc_->getNodeName(), "Message" ) ) {
		PeerMessage *msg = new PeerMessage(getMessageName());
		bool P=doc_->getChildNode();
		// if there is something in this branch
		while( P )
		{
			// add fields and parents (and ignore possible empty decls
			if( !Strcmp( doc_->getNodeName(), "Field" ) && doc_->getNodeText() != "" )
				msg->addMemberData( doc_->getNodeText() );
			if( !Strcmp( doc_->getNodeName(), "Parent" ) && doc_->getNodeText() != "" )
				msg->addParent( doc_->getNodeText() );
			
			P = doc_->getNextNode();
		}
		peeri_->addMessage(msg);
		doc_->getParentNode();
	}
}

std::string PeerGenerator::getMessageName(void)
{
	bool P = doc_->getFirstAttribute();

	while( P ) {

		if( !Strcmp( doc_->getAttributeName(), "Name" ) ) {
			return doc_->getAttributeValue();
		}

		P = doc_->getNextAttribute();
	}

	std::cerr << "Warning: Name-attribute missing from a <Message> element!" << std::endl;

	return "NoName";
}

void PeerGenerator::createHeaderFile()
{
    put_hh_headers();
    // print include statements
    hh_file_ << "#include <doors/hsi.h>" << std::endl;
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
    peer_->printHeader(hh_file_);
    put_hh_endings();
}


void PeerGenerator::createImplFile()
{
    unsigned int i=0;

	i_file_ << std::endl;

    while(i<iincludes_.size()) {
		if( isDoorsInc(iincludes_[i]) ) {
			i_file_ << "#include <" << cutWSpace(iincludes_[i++]) << ">" << std::endl;
		} else {
			if(cutWSpace(iincludes_[i]).at(0)=='<') {
				i_file_ << "#include " << cutWSpace(iincludes_[i++]) << std::endl;
			} else {
				i_file_ << "#include \"" << cutWSpace(iincludes_[i++]) << "\"" << std::endl;
			}
		}
	}

	// Include the generated header file
	i_file_ << "#include \"" << hh_fname_ << '"' << std::endl;

	i_file_ << std::endl;
	i_file_ << std::endl;

    peer_->printImpl(i_file_);
}

std::string PeerGenerator::cutWSpace(std::string s) const
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

bool PeerGenerator::isDoorsInc( std::string inc ) const
{
	if( cutWSpace(inc).substr(0,6) == "doors/" ) {
		return true;
	}

	return false;
}

void PeerGenerator::put_hh_headers() const
{
    // here should go the Copyright

    // print the stuff in the beginning of the hh file
    hh_file_ << "#ifndef _";
	hh_file_ << StrToLower(classname_);
    hh_file_ << "_h_" << std::endl;

    hh_file_ << "#define _";
	hh_file_ << StrToLower(classname_);
    hh_file_ << "_h_\n\n" << std::endl;

}

void PeerGenerator::put_hh_endings() const
{
    hh_file_ << "\n\n#endif" << std::endl;
}
