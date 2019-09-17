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

$Log: interface.h,v $
Revision 1.1  2002/06/17 13:29:49  bilhanan
*** empty log message ***

Revision 1.2  2002/06/07 12:09:39  hartmanj
Full licence clause added


*/



#ifndef INTERFACE_HH
#define INTERFACE_HH

#include <iostream>
#include <string>

// abstract class Interface

class Interface
{
public:
  
  //constructor and virtual destructor
  Interface();
  virtual ~Interface();
  
  // pure-virtual methods
  
  // Adds Parent to Message.
  virtual void addParent(const std::string &parent) =0;
  
  // Same as headers...but implementation is printed.
  virtual void printIfaceClassImpl(const std::string &SAPname,
				   std::ostream &impl) =0;
  
  virtual void printIfaceMessagesImpl(const std::string &SAPname,
			      std::ostream &impl) =0;
  
  // virtual methods
  
  // Prints the Interface-specific data to header.
  // Default implementation prints nothing.
  virtual void printIfaceClass(std::ostream);
  
  // Prints the Interface's messages to header (SAPMessage.printHeader())
  // Default implementation prints nothing.
  virtual void printMessageClasses(std::ostream);
  
private:
  
#endif // Interface.hh

};
