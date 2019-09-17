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

$Log: configreader.C,v $
Revision 1.9  2006/07/31 21:01:59  bilhanan
bugfix: configreader confused by mixed string values starting with numbers

Revision 1.8  2004/03/25 12:38:47  hartmanj
Compile fixes

Revision 1.7  2004/03/24 09:14:00  bilhanan
introducing CONFIG variables.

Revision 1.6  2004/03/12 14:05:15  hartmanj
Meat inside isStrValue()

Revision 1.5  2004/03/12 14:01:24  bilhanan
removing struct keyword, adding false return value to isStrValue

Revision 1.4  2004/03/11 14:09:53  hartmanj
Removed Variant and introduced a struct and some new methods to reflect the change

Revision 1.3  2003/11/26 09:06:48  bilhanan
bugfix for null character read into ifstream.

Revision 1.2  2003/06/04 11:09:53  kalliosa
Licence clause added

  
*/

#include <doors/configreader.h>
#include <fstream>

#include <doors/debug.h>

ConfigReader :: ConfigReader (const std::string &filename)
  : slp_IPv6Scope (5),
    slp_SlpPort (427),
    slp_locale ("en"),
    slp_useScopes ("DEFAULT"), 
    CONFIG_MC_MAX (15),
    CONFIG_START_WAIT (3),
    CONFIG_RETRY (2),
    CONFIG_RETRY_MAX (15){

  if( readFile (filename) ) {

    if (hasValue ("net.slp.IPv6Scope")) {
      slp_IPv6Scope = getIntValue ("net.slp.IPv6Scope");
      if (slp_IPv6Scope > 5)
	slp_IPv6Scope = 5;
      else if (slp_IPv6Scope < 1)
	slp_IPv6Scope = 1;    
    }

    if (hasValue ("net.slp.SlpPort")) {
      slp_SlpPort = getIntValue ("net.slp.SlpPort");
      if (slp_SlpPort > 65535)      // 65K should be enuff for everybody
	slp_SlpPort = 65535;
      else if (slp_SlpPort < 427)  // I'm just assuming sanity here, testing 
	slp_SlpPort = 427;         // for < 1 seemed silly.  -Bill
    }
  
    if (hasValue ("net.slp.useScopes"))
      slp_useScopes = getStrValue ("net.slp.useScopes");

    if (hasValue ("net.slp.locale"))
      slp_locale = getStrValue ("net.slp.locale");

    if (hasValue ("CONFIG_MC_MAX"))
      CONFIG_MC_MAX = getIntValue ("CONFIG_MC_MAX");

    if (hasValue ("CONFIG_START_WAIT"))
      CONFIG_START_WAIT = getIntValue ("CONFIG_START_WAIT");

    if (hasValue ("CONFIG_RETRY"))
      CONFIG_RETRY = getIntValue ("CONFIG_RETRY");

    if (hasValue ("CONFIG_RETRY_MAX"))
      CONFIG_RETRY_MAX = getIntValue ("CONFIG_RETRY_MAX");

  } else {

    _DOORS_DEBUG(0, "Warning, Configurator unable to open " << filename << ", using default values." );

  }
  
}


ConfigReader :: ~ConfigReader( )
{
   // nothing to clean
}

bool ConfigReader :: readFile( const std::string &filename )
{
   std::ifstream file;
   file.open( filename.c_str() );
   std::string  tmp;
   if( !file.fail() )
   {      
      int linenum = 0;
      while( !file.eof() )
      {
         char name[100];
         char value[100];
         // skip comment line
         while( file.peek()  =='#' ) {
            linenum++;
            file.getline( name, 100 , '\n' );
         }

         // read till '='
         linenum++;
         while(isspace(file.peek())) {
            if(file.get() == EOF) {
               return true;
            }
         }
         file.getline( name, 100, '=' );
         if(file.fail()) {
            std::cout << "Warning: Missing '=' in config file " << filename << " line " << linenum << std::endl;
            continue;
         }
         _DOORS_DEBUG(0, "Config item read from file: " << name );
         
         if (name[0] != '\0') {
            // read rest of the line
            file.getline( value, 100 );
            _DOORS_DEBUG(0, "value read: " << value );
            std::cout << "value read: " << value << std::endl;

            char **endptr = new char*;
            int intval = strtol(value, endptr, 10);
            values_.push_back(ConfValue());
            values_[values_.size()-1].name = std::string(name);

	    if (*endptr[0] != '\0') {
               values_[values_.size()-1].which = 1;
               values_[values_.size()-1].strval = std::string(value);
               values_[values_.size()-1].intval = 0;
            } else {
               values_[values_.size()-1].which = 0;
               values_[values_.size()-1].intval = intval;
               values_[values_.size()-1].strval = "";
            }
         }
      }
      // eof
      return true;
	 
   }
   // open failed
   return false;
}

bool ConfigReader :: isIntValue( const std::string &name ) const
{
   unsigned int idx = findIndex( name );
   if( idx < values_.size() ) {
      if( values_[idx].which == 0 ) {
         return true;
      }
   }

   return false;
}

unsigned int ConfigReader :: findIndex( const std::string &name ) const
{
   for(unsigned int i=0;i<values_.size();i++) {
      if( values_[i].name == name )
         return i;
   }

   return values_.size()+1;
}

bool ConfigReader :: isStrValue( const std::string &name ) const
{
   unsigned int idx = findIndex( name );
   if( idx < values_.size() ) {
      if( values_[idx].which == 1 ) {
         return true;
      }
   }

  return false;
}

bool ConfigReader :: hasValue( const std::string &name ) const
{
   unsigned int idx = findIndex( name );

   if( idx < values_.size() )
      return true;

   return false;
}

int ConfigReader :: getIntValue( const std::string &name ) const
{
   unsigned int idx = findIndex( name );

   if( idx < values_.size() )
      return values_[idx].intval;

   return 0;
}

std::string ConfigReader :: getStrValue( const std::string &name ) const
{
   unsigned int idx = findIndex( name );

   if( idx < values_.size() )
      return values_[idx].strval;

   return 0;   
}
