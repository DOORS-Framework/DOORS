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

$Log: smgenerator.C,v $
Revision 1.11  2004/03/03 11:10:49  hartmanj
Added support for multiple Peer- and SAP-files and interfaces.
More error detection also.

Revision 1.10  2004/01/29 14:45:04  hartmanj
Fixed a bug when handling comments inside <interface> element

Revision 1.9  2004/01/28 13:43:49  hartmanj
Fixed horrible indentation

Revision 1.8  2003/05/15 18:59:16  bilhanan
cerr diagnostic changed from "Invalid" to "Unverifiable"

Revision 1.7  2003/05/15 11:43:39  bilhanan
fixes to prevent gcc-compiled binaries from spitting out empty .h files

Revision 1.6  2002/07/25 14:43:12  hartmanj
Replaced IncludeFiles with HIncludeFiles and CIncludeFiles attributes

Revision 1.5  2002/07/23 17:50:11  hartmanj
Development

Revision 1.4  2002/06/19 09:31:10  hartmanj
Fixed a bug in processPeerFile() and processSAPFile()

Revision 1.3  2002/06/18 14:01:13  ik
Missing ">".

Revision 1.2  2002/06/18 12:25:41  ik
string->std::string etc fixes.

Revision 1.1  2002/06/17 13:29:56  bilhanan
*** empty log message ***

Revision 1.5  2002/06/17 08:26:16  hartmanj
#include <doors/sm.h> -> #include <doors/xcgsm.h>

Revision 1.4  2002/06/14 13:41:39  hartmanj
Changed #includes

Revision 1.3  2002/06/14 13:27:20  hartmanj
Reordering includefiles

Revision 1.2  2002/06/07 12:19:09  hartmanj
Full licence clause added


*/



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <doors/smgenerator.h>
#include <doors/xmldocument.h>
#include <doors/xcgsm.h>
#include <doors/state.h>
#include <doors/event.h>


const int HINCLUDE = 0;
const int CINCLUDE = 1;

//  SMGenerator::SMGenerator() : sm_(0), doc_(0), hh_file_(std::cout), cc_file_(std::cout), hh_fname_("")
//  {
//  }

SMGenerator::SMGenerator( XMLDocument *doc, std::ofstream &hh_file, std::string hh_fname, 
                          std::ofstream &cc_file )
     : sm_(0), doc_(doc), hh_file_(hh_file), hh_fname_(hh_fname), cc_file_(cc_file)
{
}

SMGenerator::~SMGenerator()
{
}

int SMGenerator::generateSM()
{
     doc_->getRootNode();
     while( doc_->getNodeName() == "comment" ) {
          doc_->getNextNode();
     }

     if( !Strcmp(doc_->getNodeName(),"SM") )
     {
          // check if PTask attribute has been defined
          std::string ptaskname_ = getPTaskNameFromAttributes();
          std::string smname_ = getSMNameFromAttributes();
          if(smname_ == "") {
               std::cerr << "Warning: No Name specified for SM! Using DefaultSM" << std::endl;
               smname_ = "DefaultSM";
          }
          if(ptaskname_ == "") {
               if(smname_ != "DefaultSM") {
                    std::cerr << "Warning: No PTask specified for SM! Using "
                              << smname_ << "PTask" << std::endl;
                    ptaskname_ = smname_;
                    ptaskname_ += "PTask";
               } else {
                    std::cerr << "Warning: No PTask specified for SM! Using DefaultPTask" << std::endl;
                    ptaskname_ = "DefaultPTask";
               }
          }

          // create new SM object to hold the following SM definition
          sm_ = new SM( smname_, ptaskname_ );

          // get the include definitions
          getIncludes();

          doc_->getChildNode();
          doc_->getNextNode();
          // process the SAP-File element

          while( Strcmp( doc_->getNodeName(), "From" ) && Strcmp( doc_->getNodeName(), "State" ) ) {
               if(!Strcmp(doc_->getNodeName(),"SAP-File")) {
                    processSAPFile();
                    doc_->getNextNode();
                    doc_->getNextNode();
               } else if(!Strcmp(doc_->getNodeName(),"Peer-File")) {
                    processPeerFile();
                    doc_->getNextNode();
                    doc_->getNextNode();
               } else {
                    doc_->getNextNode();
                    doc_->getNextNode();
               }
          }

          // read the From elements and add the interfaces to SM
          while( !Strcmp( doc_->getNodeName(), "From" ) )
          {
               processFrom();
               // move to next From-element
               doc_->getNextNode();
               doc_->getNextNode();
          }


          // go through the states
          while( !Strcmp( doc_->getNodeName(), "State" ) )
          {
               processState();
               doc_->getNextNode();
               doc_->getNextNode();
          }
      
          // generator was successful
          return 0;
      
     }
  
     std::cerr << "Error processing SM XML! No SM element found or xml-file corrupted!" << std::endl;
     std::cerr << "doc_->getNodeName(): " << doc_->getNodeName() << std::endl;

     return 0;
}

// Fetches the name of the SAP from SAP-File
void SMGenerator::processSAPFile()
{
     std::string sapfile_ = doc_->extractAttribute("Name");

     if(sapfile_ != "" )
     {
          XMLDocument *sapdoc_ = new XMLDocument(sapfile_);

          if( sapdoc_->getRootNode() == false ) {
               delete sapdoc_;
               return;
          }

          while( sapdoc_->getNodeName() == "comment" ) {
               sapdoc_->getNextNode(); // Take away possible comment
          }

          if( Strcmp(sapdoc_->getNodeName(),"SAP") ) // No SAP
          {
               std::cerr << "Error: No SAP element found in SAP-File(" << sapfile_ << ")." << std::endl;
          }
          else { // all good so far
               sapnames_.push_back(sapdoc_->extractAttribute("Name"));
               if(!sapnames_.empty() && sapnames_.back() == "" ) {
                    std::cerr << "Error: No name attribute found in SAP element in " << sapfile_ << std::endl;
               }
          }

          delete sapdoc_;
     }
}

// Fetches the name of the Peer from Peer-File
void SMGenerator::processPeerFile()
{
     std::string peerfile_ = doc_->extractAttribute("Name");
     if(peerfile_ != "" )
     {
          XMLDocument *peerdoc_ = new XMLDocument(peerfile_);

          if( peerdoc_->getRootNode() == false ) {
               delete peerdoc_;
               return;
          }

          while( peerdoc_->getNodeName() == "comment" ) {
               peerdoc_->getNextNode(); // Take away possible comment
          }

          if(Strcmp(peerdoc_->getNodeName(),"Peer")) // Didn't find Peer!
          {
               std::cerr << "Error: No Peer element found in Peer-File(" << peerfile_ << ")." << std::endl;
          }
          else { // all good so far
               peernames_.push_back( peerdoc_->extractAttribute("Name") );
               if(!peernames_.empty() && peernames_.back() == "") {
                    std::cerr << "Error: No name attribute found in Peer element in " << peerfile_ << std::endl;
               }
          }

          delete peerdoc_;
     }

}

std::string SMGenerator::getSMNameFromAttributes()
{
     for(bool P=doc_->getFirstAttribute();P;P=doc_->getNextAttribute())
          if(!Strcmp(doc_->getAttributeName(),"Name"))
               return doc_->getAttributeValue();

     return "";
}

std::string SMGenerator::getPTaskNameFromAttributes()
{
     for(bool P=doc_->getFirstAttribute();P;P=doc_->getNextAttribute())
          if(!Strcmp(doc_->getAttributeName(),"PTask"))
               return doc_->getAttributeValue();

     return "";
}

void SMGenerator::getIncludes(void)
{
     for(bool P=doc_->getFirstAttribute();P;P=doc_->getNextAttribute()) {
          if(!Strcmp(doc_->getAttributeName(),"HIncludeFiles")) {
               addToIncludes(doc_->getAttributeValue(), HINCLUDE);
          } else if(!Strcmp(doc_->getAttributeName(),"CIncludeFiles")) {
               addToIncludes(doc_->getAttributeValue(), CINCLUDE);
          } else if( !Strcmp( doc_->getAttributeName(), "IncludeFiles") ){
               std::cerr << "Warning: Deprecated attribute IncludeFiles."
                    " Use HIncludeFiles and CIncludeFiles instead!" << std::endl;
               addToIncludes(doc_->getAttributeValue(), CINCLUDE);
          }
     }
}

void SMGenerator::addToIncludes(std::string s, int includevec)
{
     std::string temp="";
     std::string::iterator i = s.begin();

     // go through the include list
     while( i != s.end() ) {
          // extract one include and add it to [c|h]includes_
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

void SMGenerator::processFrom()
{
     // some upwards compatibility here..
     for(bool P=doc_->getFirstAttribute();P;P=doc_->getNextAttribute())
     {
          std::string interfaceType = doc_->getAttributeName();
          std::string interfaceID = doc_->getAttributeValue();
          bool verified = verifyInterfaceType( interfaceType );
          if( !verified ) {
               std::cerr << "Warning: Unverifiable interface name in From element: "
                         << interfaceType << std::endl;
          }
          ifacenames_.push_back( interfaceID );
          sm_->addIface( interfaceType, interfaceID );
     }
}

bool SMGenerator :: verifyInterfaceName( const std::string &ifaceName )
{
     bool verified = false;
     // check here that _attribute name_ is found in SAP-File or Peer-File
     for(unsigned int ifaceIdx = 0; ifaceIdx < ifacenames_.size() && !verified; ifaceIdx++) {
          
          if( !Strcmp(ifaceName,ifacenames_[ifaceIdx])) {
               verified = true;
          }
     }

     return verified;
}

bool SMGenerator :: verifyInterfaceType( const std::string &ifaceType )
{
     bool verified = false;
     // check here that _attribute name_ is found in SAP-File or Peer-File
     for(unsigned int sapIdx = 0; sapIdx < sapnames_.size() && !verified; sapIdx++) {
          
          if( !Strcmp(ifaceType,sapnames_[sapIdx])) {
               verified = true;
          }
     }
     for(unsigned int peerIdx = 0; peerIdx < peernames_.size() && !verified; peerIdx++) {
          
          if( !Strcmp(ifaceType,peernames_[peerIdx] ) ) {
               verified = true;
          }
     }

     return verified;
}
void SMGenerator::processState()
{
     // do we have a State definition here
     if(!Strcmp(doc_->getNodeName(), "State"))
     {
          // get the State Name and possible default action
          std::string state_name="", default_action;
          for(bool P=doc_->getFirstAttribute();P;P=doc_->getNextAttribute())
          {
               if(!Strcmp(doc_->getAttributeName(),"Name"))
                    state_name=doc_->getAttributeValue();
               if(!Strcmp(doc_->getAttributeName(),"Default"))
                    default_action=doc_->getAttributeValue();
          }

          State *state=0;
          if(state_name!="")
               state = new State(state_name);
          else
               std::cerr << "Warning: Empty State Name found!" << std::endl;

          sm_->addState(state);

          // set the possible default action
          if(default_action!=""&&state) {
               state->setDefaultAction(default_action);
               state->setSystemAction(default_action);
          }

          doc_->getChildNode();
          doc_->getNextNode();

          // go through the child nodes in state. interfaces[+timer]
          while(Strcmp(doc_->getNodeName(),"State"))
          {
               if(!Strcmp(doc_->getNodeName(),"Interface"))
               {
                    std::string iface_name="";
                    // get the interface name
                    for(bool P=doc_->getFirstAttribute();P;P=doc_->getNextAttribute())
                    {
                         if(!Strcmp(doc_->getAttributeName(),"Name")) {
                              iface_name=doc_->getAttributeValue();

                              if( !verifyInterfaceName( iface_name ) ) {
                                   std::cerr << "Undeclared Interface used in Interface declaration: "
                                             << iface_name << std::endl;
                                   std::cerr << "Adding Interface anyway." << std::endl;
                                   sm_->addIface( "NULL", iface_name );
                                   ifacenames_.push_back(iface_name);
                              }
                         }
                    }

                    doc_->getChildNode();
                    //doc_->getNextNode();

                    // get data from input. create an Event object for every input-action pair
                    while(doc_->getNextNode())
                    {
                         if(!Strcmp(doc_->getNodeName(), "comment")) {
                              doc_->getNextNode();
                              continue;
                         }
                         std::string input_name;
                         for(bool P=doc_->getFirstAttribute();P;P=doc_->getNextAttribute()) {
                              if(!Strcmp(doc_->getAttributeName(),"Name")) {
                                   input_name=doc_->getAttributeValue();
                              }
                         }
                         Event *event = new Event(iface_name,input_name);
                         std::string event_action=doc_->getNodeText();
                         if(event_action=="") { event_action = default_action; }
                         event->setAction(event_action);
                         sm_->addEvent(state->getName(),iface_name,event);

                         doc_->getNextNode();
                         //doc_->getNextNode();
                    }

                    doc_->getParentNode();
               }
               if(!Strcmp(doc_->getNodeName(),"Timer"))
               {
                    state->setSystemAction(doc_->getNodeText());
               }
               doc_->getNextNode();
               if(!doc_->getNextNode() || !Strcmp(doc_->getNodeName(),"text"))
                    doc_->getParentNode();
	  
          }
     }
}

void SMGenerator::createHeaderFile()
{
     // print the stuff in the beginning of the hh file
     hh_file_ << "#ifndef _";
     unsigned int i=0;
     std::string classname_ = sm_->getName();
     while( i < classname_.size() )
          hh_file_ << (char)tolower(classname_.at(i++));

     hh_file_ << "_h_" << std::endl;

     hh_file_ << "#define _";
     i=0;
     while( i < classname_.size() )
          hh_file_ << (char)tolower(classname_.at(i++));

     hh_file_ << "_h_\n\n" << std::endl;

     // print #include <ptb.h>
     hh_file_ << "#include <doors/ptb.h>\n\n" << std::endl;

     i=0;
     while( i < hincludes_.size()) {
          if( isDoorsInc(hincludes_[i]) ) {
               hh_file_ << "#include <" << cutWSpace(hincludes_[i++]) << ">" << std::endl;
          } else {
               hh_file_ << "#include \"" << cutWSpace(hincludes_[i++]) << "\"" << std::endl;
          }
     }

     // then the actual stuff from sm_ *SM;
     sm_->printHeader(hh_file_);


     // put #endif
     hh_file_ << "\n#endif" << std::endl;
}

std::string SMGenerator::cutWSpace(std::string s) const
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

bool SMGenerator::isDoorsInc( std::string inc ) const
{
     if( cutWSpace(inc).substr(0,6) == "doors/" ) {
          return true;
     }

     return false;
}

void SMGenerator::createImplFile()
{
     cc_file_ << "\n\n";
     // print #includes
     unsigned int i=0;

     cc_file_ << "#include \"" << hh_fname_ << '"' << std::endl;
     while( i < cincludes_.size()) {
          if( isDoorsInc(cincludes_[i]) ) {
               cc_file_ << "#include <" << cutWSpace(cincludes_[i++]) << ">" << std::endl;
          } else {
               cc_file_ << "#include \"" << cutWSpace(cincludes_[i++]) << "\"" << std::endl;
          }
     }

     cc_file_ << "\n" << std::endl;

     // then the actual mechine
     sm_->printImpl(cc_file_);
}
