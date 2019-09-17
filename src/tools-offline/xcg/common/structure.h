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

$Log: structure.h,v $
Revision 1.1  2002/06/17 13:29:49  bilhanan
*** empty log message ***

Revision 1.2  2002/06/07 12:09:39  hartmanj
Full licence clause added


*/

// this is base class for all Protocol Layer functionalities.
// eg. SAP, Peer, StateMachine
// This interface defines keymethods
// for implementiong generator data structure.

//070402 petri draft version

#ifndef STRUCTURE_HH
#define STRUCTURE_HH

#include <iostream>
#include <string>

// abstract class Structure

class Structure
{
public:
  
  //constructor and virtual destructor
  Structure();
  virtual ~Structure();

  // pure-virtual methods
  
  // Adds Parent to Protocol layer functionality class eg. SAP.
  virtual void addParent(const std::string &parent) =0;
  
  // PrintHeader print the dataStructure in header-format
  // -> generates C++headerfiles to functionality eg. SAP
  virtual void printHeader(std::ostream &header) =0;
  
  // PrintImpl prints the dataStructure in
  // implementation-format
  // -> generates C++implementation files
  // to functionality eg. SAP
  virtual void printImpl(std::ostream &impl) =0;

private:
  
#endif // Structure.hh

};
