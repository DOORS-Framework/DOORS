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

$Log: sapinterface.h,v $
Revision 1.2  2002/06/18 12:25:22  ik
string->std::string etc fixes.

Revision 1.1  2002/06/17 13:29:55  bilhanan
*** empty log message ***

Revision 1.3  2002/06/14 13:41:31  hartmanj
Changed #includes

Revision 1.2  2002/06/07 12:19:08  hartmanj
Full licence clause added


*/


// SAPInterface -class

//060202 petri Draft version
//250202 petri Commented and made small non-functional changes.

// Class descripes the Service Access Point's user and provider interfaces.

#ifndef SAPINTERFACE_HH
#define SAPINTERFACE_HH

#include <string>
#include <vector>
#include <doors/interface.h>
#include <doors/sapmessage.h>

extern int ilevel;
extern std::string TAB();

class SAPInterface : public Interface
{
public:
  
  // Typedef for Interface types.
  typedef enum {User = 0, Provider} ifaceType;
  
  SAPInterface(ifaceType type);
  virtual ~SAPInterface();
  
  // Adds Inheritance parents to Interface (can have as many as possible)
  // Multiple inheritance, dealed in c++ way.
  // (type, name) pairs separated by comma
  virtual void addParent(const std::string &parent);

  // return the type of the interface
  ifaceType getType() const;
  
  // Returns pointer to first message of interface.
  SAPMessage* getFirstMessage() const;
  
  // Sets the first message of interface.
  void setFirstMessage(SAPMessage *message);
  
  // Adds new message to interface.
  void addMessage (SAPMessage *message);
  
  // Prints the Interface-specifig data to header.
  virtual void printIfaceClass(std::ostream &header);
  // Prints the Interface's messages to header (SAPMessage.printHeader())
  virtual void printMessageClasses(std::ostream &header);
  
  // Same as headers...but implementation is printed.
  virtual void printIfaceClassImpl(const std::string &SAPname,
				   std::ostream &impl);
  virtual void printIfaceMessagesImpl(const std::string &SAPname,
				      std::ostream &impl);
  
private:
  
  // Interface-name
  std::string name_;
  // Parents as a vector of strings.
  std::vector<std::string> parents_;
  // FirstMessage
  SAPMessage *firstMessage_;
  // LastMessage
  SAPMessage *lastMessage_;
  // Type of interface.
  ifaceType type_;
};

#endif // SAPINTERFACE_HH
