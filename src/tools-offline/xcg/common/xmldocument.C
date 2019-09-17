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

$Log: xmldocument.C,v $
Revision 1.5  2009/04/20 13:46:38  bilhanan
bugfix to allow c-style string manipulation

Revision 1.4  2002/08/23 06:07:46  hartmanj
Added some comments

Revision 1.3  2002/06/19 09:30:30  hartmanj
Added some pointer checks

Revision 1.2  2002/06/18 12:23:08  ik
string->std::string etc fixes.

Revision 1.1  2002/06/17 13:29:49  bilhanan
*** empty log message ***

Revision 1.3  2002/06/14 13:39:18  hartmanj
#include "X" -> #include <doors/X>

Revision 1.2  2002/06/07 12:09:39  hartmanj
Full licence clause added


*/

#include <iostream>
#include <doors/parser.h>
#include <doors/tree.h>
#include <string>
#include <cstring>
#include <ctype.h>
#include <doors/xmldocument.h>



// converts a string to lower case
std::string StrToLower(std::string s)
{
	std::string::iterator i = s.begin();
	std::string temp;

	while(i != s.end()) {
		temp += tolower(*i);
		i++;
	}

	return temp;
}

// case insensitive comparison
int Strcmp(std::string s1, std::string s2)
{
    unsigned int i=0;
    while(i<s1.size()) {
		s1[i] = tolower(s1[i]);
		i++;
    }

    i=0;

    while(i<s2.size()) {
		s2[i] = tolower(s2[i]);
		i++;
    }

    return strcmp(s1.c_str(),s2.c_str());
}

/* Constructor.
 * uses libXML to parse the xml-file
 */
XMLDocument::XMLDocument( std::string filename ) : root_(0), current_(0), attrs_(0)
{
    doc_ = xmlParseFile( filename.c_str() );

    if( doc_ == NULL )
    {
		std::cerr << "Error opening XML-file(" << filename << ")" << std::endl;
		//throw some exception?
    }
}

/*
 * Checks whether the xml-file has been parsed
 * successfully
 */
bool XMLDocument::isGood() const
{
	if( doc_ )
		return true;
	return false;
}	

XMLDocument::XMLDocument() : root_(0), current_(0), doc_(0), attrs_(0)
{
}

/* Destructor:
 * Frees the memory that libXML
 * allocated
 */
XMLDocument::~XMLDocument()
{
    if( doc_ )
		xmlFreeDoc(doc_);
    else if( root_ )
		xmlFreeNode( root_ );
}


/*
 * Set the current node to
 * the root node of the document
 */
bool XMLDocument::getRootNode()
{
	
    if( doc_ && doc_->children )
    {
		root_ = doc_->children;
		current_ = root_;
		return true;
    }

    return false;
}

/*
 * Descends to the child of
 * the current node(if the child is valid).
 * Returns: true if there is a child,
 * false if there isn't
 */
bool XMLDocument::getChildNode()
{
    if( current_ && current_->children )
    {
		current_ = current_->children;
		return true;
    }

    return false;
}    

/*
 * Moves to the next sibling
 * node of the current node.
 * Returns: true if sibling exists,
 * false if it doesn't
 */
bool XMLDocument::getNextNode()
{
    if( current_ && current_->next )
    {
		current_ = current_->next;
		return true;
    }

    return false;
}

/*
 * Moves to the parent node of
 * the current node.
 * Returns: true if parent exists,
 * false if it doesn't (you would
 * exit the tree).
 */
bool XMLDocument::getParentNode()
{
    if( current_ && current_->parent )
    {
		current_ = current_->parent;
		return true;
    }

    return false;
}

/*
 * Moves to the previous sibling node
 * of the current.
 * Returns: as above.
 */
bool XMLDocument::getPreviousNode()
{
    if( current_ && current_->prev )
    {
		current_ = current_->prev;
		return true;
    }

    return false;
}

/*
 * Retrieves and returns
 * the text in the elements
 * (first) text field.
 */
std::string XMLDocument::getNodeText()
{
    if( current_ && current_->children && current_->children->content )
    {
		return strip_ws(std::string((char*)current_->children->content));
    }
    return "";
}

/*
 * Strips white space from beginning and
 * end of a string. Uses copying.
 */
std::string XMLDocument::strip_ws(std::string s) const
{
    int i1=0;
    int i2=s.size()-1;
    
    while(i1<i2 && isspace(s.at(i1))) {
		i1++;
    }
    
    while(i2>i1 && isspace(s.at(i2))) {
		i2--;
    }
    
    return s.substr(i1,i2-i1+1);
}

/*
 * Returns: the name of the current element
 */
std::string XMLDocument::getNodeName()
{
	if(current_ && current_->name ) {
		std::string s = (char*)current_->name;
		return s;
	}

	return "";
}

/*
 * Updates the attrs_ pointer.
 * Used by get*Attribute()-methods.
 * Private function.
 */
void XMLDocument::updateAttrs()
{
	if( current_ ) {
		attrs_ = current_->properties;
	}
}

/*
 * Moves to the first attribute of
 * the current element.
 */
bool XMLDocument::getFirstAttribute()
{
    updateAttrs();
    if( attrs_ != NULL )
		return true;
    return false;
}

/*
 * Goes to the next attribute
 * of the current element.
 * getFirstAttribute() must have
 * been called first.
 */
bool XMLDocument::getNextAttribute()
{
    if( attrs_ && attrs_->next != NULL )
    {
		attrs_ = attrs_->next;
		return true;
    }
    return false;
}

/*
 * Returns the name of the attribute.
 */
std::string XMLDocument::getAttributeName()
{
    if( attrs_ && attrs_->name )
    {
		std::string s = (char*)attrs_->name;
		return s;
    }
    return "";
}

/*
 * Returns the value of the attribute.
 */
std::string XMLDocument::getAttributeValue()
{
    if( attrs_ && attrs_->children
		&& attrs_->children->content )
    {
		std::string s = (char*)attrs_->children->content;
		return s;
    }
    return "";
}

/*
 * Finds and returns the value of attribute att.
 * If not found, returns an empty string
 */
std::string XMLDocument::extractAttribute(std::string att)
{
	for(bool P=this->getFirstAttribute();P;P=this->getNextAttribute())
    {
		if(!strcmp(this->getAttributeName().c_str(),att.c_str()))
			return this->getAttributeValue();
    }

	return "";
}

