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

$Log: xmldocument.h,v $
Revision 1.2  2002/06/18 12:23:08  ik
string->std::string etc fixes.

Revision 1.1  2002/06/17 13:29:49  bilhanan
*** empty log message ***

Revision 1.3  2002/06/14 13:39:18  hartmanj
#include "X" -> #include <doors/X>

Revision 1.2  2002/06/07 12:09:40  hartmanj
Full licence clause added


*/

#ifndef XMLDOCUMENT_HH
#define XMLDOCUMENT_HH

#include <doors/parser.h>
#include <doors/tree.h>
#include <string>


int Strcmp(std::string s1, std::string s2);
std::string StrToLower(std::string s);

/* Abstract interface class to libXML to be used with generators.
 * Note that this implementation limits some features of libxml
 * and is kind of a hack. Notice the order of child-elements:
 * element1 -> text -> element2 -> text -> ..
 * And also the order of NextNodes:
 * element1 -> text -> element2 -> text -> ..
 * I wouldn't mind a better system.
 */



class XMLDocument {

public:

    XMLDocument( std::string filename );
    XMLDocument( );
    ~XMLDocument();

    bool isGood() const;

    // for walking the tree
    bool getRootNode();
    bool getChildNode();
    bool getNextNode();
    bool getPreviousNode();
    bool getParentNode();
    std::string getNodeText();
    std::string getNodeName();
    
    // sets current attribute to first att
    bool getFirstAttribute();
    bool getNextAttribute();
    std::string getAttributeName();
    std::string getAttributeValue();

  std::string extractAttribute(std::string att);    


private:

    std::string strip_ws(std::string s) const;

    void updateAttrs();

    xmlNodePtr root_;
    xmlNodePtr current_;
    xmlDocPtr doc_;

    xmlAttrPtr attrs_;
};
    
#endif
