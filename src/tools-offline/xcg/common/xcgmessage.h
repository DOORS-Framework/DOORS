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

$Log: xcgmessage.h,v $
Revision 1.2  2002/06/18 12:22:38  ik
Class name changed to XcgMessage due to collision to DOORS Message.

Revision 1.1  2002/06/17 13:29:49  bilhanan
*** empty log message ***

Revision 1.1  2002/06/17 07:23:35  hartmanj
Renamed message.h/C to xcgmessage.h/C

Revision 1.2  2002/06/07 12:09:39  hartmanj
Full licence clause added


*/

// this is base class for all messages.

//060402 petri draft version

#ifndef XCGMESSAGE_HH
#define XCGMESSAGE_HH

#include <iostream>
#include <string>

// abstract class Message

class XcgMessage
{
public:
  
  //constructor and virtual destructor
  XcgMessage();
  virtual ~XcgMessage();

  // pure-virtual methods
  
  // Adds Parent to Message.
  virtual void addParent(const std::string &parent) =0;
  
  // Adds Member data to Message.
  virtual void addMemberData(const std::string &member) =0;
  
  // Returns the name of the message.
  virtual std::string getName() const =0;
  
  // Print the declaration of message-class to header-stream.
  virtual void printHeader(std::ostream &header) =0;
  
  // Print implementation of message-class to implementation stream.
  // Parameter name is eg. SAPname or PeerName -> name of generator class
  // message belongs
  virtual void printImpl(const std::string name, std::ostream &impl) =0;

protected:
  
  // some helping functions that are not public interface
  // put are needed in inherited classes.

  // string is in format type name
  // Functions return substring that matches to type or name.
  std::string getVarType(std::string &desc) const;
  std::string getVarName(std::string &desc) const;
  // Converts the given string to lowercase.
  std::string toLower(std::string &S) const;
  
  
private:
  

};

#endif // XcgMessage.hh
