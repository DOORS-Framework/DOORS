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

$Log: peergenerator.h,v $
Revision 1.7  2003/05/15 11:43:38  bilhanan
fixes to prevent gcc-compiled binaries from spitting out empty .h files

Revision 1.6  2002/07/25 15:31:39  hartmanj
Changed CIncludeFiles to IIncludeFiles

Revision 1.5  2002/07/25 14:43:11  hartmanj
Replaced IncludeFiles with HIncludeFiles and CIncludeFiles attributes

Revision 1.4  2002/07/23 17:50:10  hartmanj
Development

Revision 1.3  2002/07/09 11:54:50  hartmanj
Changed the 'grammar' a bit. Peer is described now like this: <Peer> <Message Name=MsgName> <Parent>...</Parent> <Field>...</Field> ... </Message> ... </Peer>

Revision 1.2  2002/06/18 12:23:32  ik
string->std::string etc fixes.

Revision 1.1  2002/06/17 13:29:54  bilhanan
*** empty log message ***

Revision 1.4  2002/06/17 08:16:00  hartmanj
#include <doors/peer.h> -> #include <doors/xcgpeer.h>

Revision 1.3  2002/06/14 13:40:32  hartmanj
void generatePeer() to int generatePeer()

Revision 1.2  2002/06/07 12:19:07  hartmanj
Full licence clause added


*/



#ifndef PEERGENERATOR_HH
#define PEERGENERATOR_HH

#include <fstream>
#include <vector>
#include <string>
#include <doors/xmldocument.h>
#include <doors/xcgpeer.h>


class PeerGenerator {


public:

//      PeerGenerator();
    PeerGenerator( XMLDocument *doc, std::ofstream &hh_file,
				   std::string hh_fname, std::ofstream &i_file );
    ~PeerGenerator();


    // Process the xml and create a "parse tree" to be used with printing
    int generatePeer(void);

    // Actually generate the files. generatePeer() must be called first
    void createHeaderFile();
    void createImplFile();


private:


    void processPeer();
    void processMessage();
    void processAttributes();
    void put_hh_headers() const;
    void put_hh_endings() const;

	std::string getMessageName(void);
	std::string cutWSpace( std::string s ) const;

	void addToIncludes(std::string s, int includevec);
	bool isDoorsInc(std::string inc) const;

    std::string classname_;
	std::string hh_fname_;

    std::ofstream &hh_file_;
    std::ofstream &i_file_;

    XMLDocument *doc_;

    std::vector<std::string> hincludes_;
    std::vector<std::string> iincludes_;

    Peer *peer_; // The Peer construction class
    PeerInterface *peeri_; // Construction class for the interface of pdus
};




#endif
