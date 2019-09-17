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

$Log: dsmg.C,v $
Revision 1.6  2009/04/20 13:46:45  bilhanan
bugfix to allow c-style string manipulation

Revision 1.5  2003/05/15 11:43:38  bilhanan
fixes to prevent gcc-compiled binaries from spitting out empty .h files

Revision 1.4  2002/07/25 14:43:12  hartmanj
Replaced IncludeFiles with HIncludeFiles and CIncludeFiles attributes

Revision 1.3  2002/06/19 12:17:31  hartmanj
Fixed generateHH/CCFilename functions

Revision 1.2  2002/06/18 12:25:41  ik
string->std::string etc fixes.

Revision 1.1  2002/06/17 13:29:56  bilhanan
*** empty log message ***

Revision 1.5  2002/06/17 06:39:24  hartmanj
Fixed generateHH/CCFilename functions

Revision 1.4  2002/06/14 13:41:39  hartmanj
Changed #includes

Revision 1.3  2002/06/14 13:27:20  hartmanj
Reordering includefiles

Revision 1.2  2002/06/07 12:19:09  hartmanj
Full licence clause added


*/


#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstring>
#include <unistd.h>

#include <doors/smgenerator.h>
#include <doors/xmldocument.h>


// some command line arguments
int no_hhfile=0;
int no_ccfile=0;
int no_macros=0;
int ilevel=0;

// Not part of SAPGenerator!!
void Process( std::string xmlfilename, std::string cc_filename, std::string hh_filename )
{
    /* Let's test if xmlfile really exists */
    if(access(xmlfilename.c_str(), R_OK) != 0) {
		std::cerr << "Error opening " << xmlfilename << " for reading! Aborting!" << std::endl;
		return;
    }

    XMLDocument *doc = new XMLDocument(xmlfilename);

    if( doc == NULL || !doc->isGood() ) // XML file was malformed
    {
		//throw new ParseError("Error in XML syntax");
		std::cerr << "Error opening XML file" << std::endl;
		return;
    }

    if( hh_filename != "" && cc_filename != "" ) {
		std::ofstream header_file, impl_file;

		if(no_hhfile==0)
			header_file.open(hh_filename.c_str());
		if(no_ccfile==0)
			impl_file.open(cc_filename.c_str());
	
		SMGenerator smg = SMGenerator( doc, header_file, hh_filename, impl_file );
		if(	smg.generateSM() ) {
		  //    header_file.close();  commented: why the need to close the 
		  //	impl_file.close();    commented: ofstreams here ? -Bill
			return;
		}

		if(no_hhfile==0) {
			smg.createHeaderFile();
			header_file.close();
		}
		if(no_ccfile==0) {
			smg.createImplFile();
			impl_file.close();
		}
    }
    else {
		std::cerr << "Unexpected error! Check your files and filenames!" << std::endl << std::endl;
    }
}

std::string generateHHFilename(std::string xmlfname, std::string hh_suffix)
{
    unsigned int i = 0;

	while( xmlfname.at(i) != '.' && i<xmlfname.size() ) {
		i++;
	}


    return xmlfname.substr(0,i) + "." + hh_suffix;
}

std::string generateCCFilename(std::string xmlfname, std::string cc_suffix)
{
    unsigned int i = 0;

	while( xmlfname.at(i) != '.' && i<xmlfname.size() ) {
		i++;
	}


    return xmlfname.substr(0,i) + "." + cc_suffix;
}

void Usage(void)
{
    std::cout << "Usage: dsmg [<options>] <xmlfile>" << std::endl;
    std::cout << "Possible <options> are:" << std::endl;
    std::cout << "\t--help" << std::endl;
    std::cout << "\t--version" << std::endl;
    std::cout << "\t--nomacros" << std::endl;
    std::cout << "\t--no-hhfile" << std::endl;
    std::cout << "\t--no-ccfile" << std::endl;
    std::cout << "\t--cc-file <filename>" << std::endl;
    std::cout << "\t--hh-file <filename>" << std::endl;
    std::cout << "\t--hh-suffix <header file suffix>\t" << std::endl;
    std::cout << "\t--cc-suffix <implementation file suffix>\t" << std::endl << std::endl;
    std::cout << "See dsapg(1) for more details." << std::endl;
}

int main( int argc, char *argv[] )
{
    std::string hh_filename = "";
    std::string cc_filename = "";
    std::string cc_suffix = "cc";
    std::string hh_suffix = "hh";
    std::string xmlfilename = "";

    if( argc == 1 ) {
		Usage();
		return 0;
    }

    int i=1;
    while( i < argc )
    {
		if( !strcmp(argv[i], "--nomacros") ) {
			no_macros = 1;
		}
		else if( !strcmp(argv[i], "--hh-file") ) {
			if(i<argc-2) {
				hh_filename = argv[i+1];
				i++;
			} else {
				std::cout << "Error: Invalid arguments. Missing .hh file name." << std::endl;
				Usage();
				return 0;
			}
		}
		else if( !strcmp(argv[i], "--cc-file") ) {
			if(i<argc-2) {
				cc_filename = argv[i+1];
				i++;
			} else {	
				std::cout << "Error: Invalid arguments. Missing .cc file name." << std::endl;
				Usage();
				return 0;
			}

		}
		else if( !strcmp(argv[i], "--hh-suffix") ) {
			if(i<argc-2) {
				hh_suffix = argv[i+1];
				i++;
			} else {
				std::cout << "Error: Invalid arguments. Missing .hh suffix." << std::endl;
				Usage();
				return 0;
			}
		}
		else if( !strcmp(argv[i], "--cc-suffix") ) {
			if(i<argc-2) {
				cc_suffix = argv[i+1];
				i++;
			} else {
				std::cout << "Error: Invalid arguments. Missing .cc suffix." << std::endl;
				Usage();
				return 0;
			}
		}
		else if( !strcmp(argv[i], "--help") ) {
			Usage();
			return 0;
		}	
		else if( !strcmp(argv[i], "--no-hhfile") ) {
			no_hhfile=1;
		}
		else if( !strcmp(argv[i], "--no-ccfile") ) {
			no_ccfile=1;
		}
		else if( !strcmp(argv[i], "--version") || !strcmp(argv[i], "-v") ) {
			std::cout << "dsmg version 1.0" << std::endl;
			return 0;
		}
		else if( i < argc - 1 )
		{
			std::cout << "Invalid parameter: " << argv[i] << std::endl;
			Usage();
			return 0;
		}
		else if( i == argc - 1 )
		{
			xmlfilename = argv[i];
		}
		i++;
    }



    if( cc_filename == "" ) {
		cc_filename = generateCCFilename(xmlfilename, cc_suffix);
    }
    if( hh_filename == "" ) {
		hh_filename = generateHHFilename(xmlfilename, hh_suffix);
    }


    /* Go for the real stuff */

    Process(xmlfilename, cc_filename, hh_filename);


    return 0;
}

