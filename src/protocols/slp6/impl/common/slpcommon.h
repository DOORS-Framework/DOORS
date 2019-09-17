//  Copyright 2003
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

$Log: slpcommon.h,v $
Revision 1.13  2004/07/15 09:33:03  hartmanj
Added another getMCAddr, now with scope parameter

Revision 1.12  2004/07/10 11:21:36  hartmanj
Added typedef AttributeV

Revision 1.11  2004/03/25 09:16:19  hartmanj
Added solicitDirectoryAgent()

Revision 1.10  2004/03/22 11:07:11  hartmanj
Moved createAttributeList() to SLPCommon

Revision 1.9  2004/03/19 11:20:44  hartmanj
Changes in API: joinMCGroup() and getMCAddr() changed.

Revision 1.8  2004/03/09 09:09:33  bilhanan
I lied, readConfig() declaration is now really removed. :)

Revision 1.7  2004/03/09 08:43:26  bilhanan
changes in constructors, destructor, removed DAActiveDiscoveryInterval_,
removed readConfig, removed uncessary #includes, code cleanup.

Revision 1.6  2004/02/10 18:08:54  bilhanan
rewrote joinMCGroup and removed joinMCGroups

Revision 1.5  2004/02/10 16:29:00  hartmanj
Changed XID handling abit

Revision 1.4  2003/11/27 12:32:56  bilhanan
bug fixing

Revision 1.3  2003/08/24 22:13:11  bilhanan
merged in code for mcast group address calculation.

Revision 1.2  2003/06/04 11:09:55  kalliosa
Licence clause added

  
*/

#ifndef SLPCOMMON_H
#define SLPCOMMON_H

#include <doors/ptb.h>
#include <doors/udp6.h>
#include <string>
#include <map>
#include <doors/inetaddr6.h>
#include <sys/socket.h>

class SLPCommon
{
   
 public:
  SLPCommon (Udp6SAP :: User &udp6sap);
  virtual ~SLPCommon (void) = 0;
  
   Uint16 getNewXID();
   int getNewHandle();
   int slpPort_;
   int ip6scope_;

   std::string getMCAddr (std::string sType);
   std::string getMCAddr (std::string sType, int scope);
   bool joinMCGroup (std::string sType);   

    
    struct Attribute {
        std::string AttributeName;
        std::string AttributeValue;
    };

    typedef std::vector<Attribute> AttributeV;

    void createAttributeList( std::vector<Attribute> &attrVec, const std::vector<std::string> &strVec );

    void solicitDirectoryAgent( std::string scope = "DEFAULT" );

protected:

    Udp6SAP :: User &udp6down;

 private:

   Uint32 HandleCounter_;
   static Uint16 XIDCounter_;
   static bool FirstXID_;
};

#endif // SLPCOMMON_H
