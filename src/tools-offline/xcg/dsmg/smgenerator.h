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

$Log: smgenerator.h,v $
Revision 1.6  2004/03/03 11:10:49  hartmanj
Added support for multiple Peer- and SAP-files and interfaces.
More error detection also.

Revision 1.5  2003/05/15 11:43:39  bilhanan
fixes to prevent gcc-compiled binaries from spitting out empty .h files

Revision 1.4  2002/07/25 14:43:12  hartmanj
Replaced IncludeFiles with HIncludeFiles and CIncludeFiles attributes

Revision 1.3  2002/07/23 17:50:11  hartmanj
Development

Revision 1.2  2002/06/18 12:25:41  ik
string->std::string etc fixes.

Revision 1.1  2002/06/17 13:29:56  bilhanan
*** empty log message ***

Revision 1.3  2002/06/14 13:27:20  hartmanj
Reordering includefiles

Revision 1.2  2002/06/07 12:19:09  hartmanj
Full licence clause added


*/



#ifndef SMGENERATOR_HH
#define SMGENERATOR_HH


#include <iostream>
#include <string>
#include <fstream>

#include <doors/xmldocument.h>
#include <doors/xcgsm.h>

class SMGenerator {


public:
	
//  	SMGenerator();
	SMGenerator( XMLDocument *doc, std::ofstream &hh_file, std::string hh_fname,
				 std::ofstream &cc_file );
	~SMGenerator();
	
	int generateSM();
	
	void createHeaderFile();
	void createImplFile();
	
private:
	
	std::string getSMNameFromAttributes();
	std::string getPTaskNameFromAttributes();
	void getIncludes(void); // reads attributes of doc_ and adds IncludeFiles to includes_
	void addToIncludes(std::string s, int includevec);
	void processFrom();
	void processState();
	void processSAPFile();
	void processPeerFile();
	bool isDoorsInc(std::string inc) const;
        bool verifyInterfaceName( const std::string &ifaceName );
        bool verifyInterfaceType( const std::string &ifaceType );
	std::string cutWSpace( std::string s ) const;
	
	SM *sm_;
	
	XMLDocument *doc_;
	
	std::ofstream &hh_file_;
	std::ofstream &cc_file_;

	std::string hh_fname_;
	
        std::vector<std::string> sapnames_, peernames_, ifacenames_;
	
	std::vector<std::string> hincludes_;
	std::vector<std::string> cincludes_;

};




#endif
