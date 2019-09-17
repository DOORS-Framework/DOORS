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

$Log: sapgenerator.h,v $
Revision 1.5  2003/05/14 15:06:55  bilhanan
std::ostream members should really be std::ofstream

Revision 1.4  2002/07/25 14:43:12  hartmanj
Replaced IncludeFiles with HIncludeFiles and CIncludeFiles attributes

Revision 1.3  2002/07/23 17:50:11  hartmanj
Development

Revision 1.2  2002/06/18 12:25:22  ik
string->std::string etc fixes.

Revision 1.1  2002/06/17 13:29:55  bilhanan
*** empty log message ***

Revision 1.4  2002/06/17 07:33:23  hartmanj
Renamed sap.h/C to xcgsap.h/C

Revision 1.3  2002/06/14 13:41:31  hartmanj
Changed #includes

Revision 1.2  2002/06/07 12:19:08  hartmanj
Full licence clause added


*/



#ifndef SAPGENERATOR_HH
#define SAPGENERATOR_HH

#include <fstream>
#include <iostream>
#include <doors/xmldocument.h>
#include <doors/xcgsap.h>

class SAPGenerator {

public:

    SAPGenerator( XMLDocument *doc, std::ofstream &hh_file, std::string hh_fname, 
				  std::ofstream &cc_file );
    ~SAPGenerator();

    int generateSAP();
    void createHeaderFile() const;
    void createImplFile() const;

private:
    
    // Functions to process different elements in XML-tree
    void processSAP();
    // Helper functions to process User and Provider sections
    void processUser();
    void processProvider();

    // Helper function to process SAP's attributes(#include etc)
    void processAttributes();
    void processField();
    void processMessage( SAPInterface *sapiface );

	std::string cutWSpace( std::string s ) const;

    void put_hh_headers() const;
    void put_hh_endings() const;

	bool isDoorsInc( std::string inc ) const;

	// parses a comma separated list of include files
	void addToIncludes(std::string s, int includevec);

    XMLDocument *doc_;
    std::string classname_;
	std::string hh_fname_;

    std::ofstream &hh_file_;
    std::ofstream &cc_file_;

    SAP *sap_;

    SAPInterface *useri_, *provi_;

    std::vector<std::string> hincludes_;
    std::vector<std::string> cincludes_;

	std::string msg_base_name_;
};

#endif
