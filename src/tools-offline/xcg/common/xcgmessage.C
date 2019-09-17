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

$Log: xcgmessage.C,v $
Revision 1.2  2002/06/18 12:22:38  ik
Class name changed to XcgMessage due to collision to DOORS Message.

Revision 1.1  2002/06/17 13:29:49  bilhanan
*** empty log message ***

Revision 1.1  2002/06/17 07:23:35  hartmanj
Renamed message.h/C to xcgmessage.h/C

Revision 1.3  2002/06/14 13:39:18  hartmanj
#include "X" -> #include <doors/X>

Revision 1.2  2002/06/07 12:09:39  hartmanj
Full licence clause added


*/



// Message class implementation

#include <string>
#include <cstdlib>
#include <doors/xcgmessage.h>

// constructor
XcgMessage::XcgMessage()
{
}

// destructor
XcgMessage::~XcgMessage()
{
}

// protected-methods : are visible to subclasses

// Returns string that represents the type of desc.
// e.g uint16 age; -> returns uint16
std::string XcgMessage::getVarType(std::string &desc) const
{
  std::string::iterator i = desc.begin();
  std::string type;
  while( i != desc.end() && (*i) != ' ' )
    {
      type += *i;
      i++;
    }
  
  if( (i+1) != desc.end() && ( (*i+1) == '*' || (*i+1) == '&' ) )
    type += (*i+1);
  
    return type;
    
}

// Returns string that is the name of taken out from desc.
// e.g uint16 age; -> returns age
std::string XcgMessage::getVarName(std::string &desc) const
{
    unsigned int idx = desc.find_first_of(" ");
    
    if(desc[idx+1] == '&' || desc[idx+1] == '*')
    {
      return desc.substr(idx+2,std::string::npos);
    }
    
    return desc.substr(idx+1,std::string::npos);
}

std::string XcgMessage::toLower(std::string &S) const
{
  std::string s;
  
  std::string::iterator i = S.begin();
  while( i != S.end() )
    {
      s += tolower(*i);
	i++;
    }
  return s;
}
