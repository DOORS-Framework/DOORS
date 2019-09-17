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

$Log: configreader.h,v $
Revision 1.6  2004/03/24 09:15:45  bilhanan
erraneously committed, now fixed.

Revision 1.5  2004/03/24 09:14:00  bilhanan
introducing CONFIG variables.

Revision 1.4  2004/03/11 14:09:53  hartmanj
Removed Variant and introduced a struct and some new methods to reflect the change

Revision 1.3  2004/02/01 22:54:43  bilhanan
class private area changed to protected to support derived classes.

Revision 1.2  2003/06/04 11:09:53  kalliosa
Licence clause added

  
*/

#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <vector>
#include <string>


class ConfigReader
{
   public:
   
      ConfigReader (const std::string &filename);
      ~ConfigReader ();
      bool readFile (const std::string &filename);
      
      bool hasValue( const std::string &name ) const;
      bool isIntValue( const std::string &name ) const;
      bool isStrValue( const std::string &name ) const;
      std::string getStrValue( const std::string &name ) const;
      int getIntValue( const std::string &name ) const;      

      int slp_IPv6Scope;
      int slp_SlpPort;
      std::string slp_locale;
      std::string slp_useScopes;

      int CONFIG_MC_MAX;
      int CONFIG_START_WAIT;
      int CONFIG_RETRY;
      int CONFIG_RETRY_MAX;

   protected:
      
      struct ConfValue {
            unsigned char which;
            int intval;
            std::string strval;
            std::string name;
      };
      unsigned int findIndex( const std::string &name ) const;
      std::vector<ConfValue> values_;   
};

#endif // CONFIGREADER__H
