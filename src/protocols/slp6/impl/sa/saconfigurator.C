//Editor-Info: -*- C++ -*-

//  Copyright 2004
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

$Log: saconfigurator.C,v $
Revision 1.4  2004/03/25 13:00:23  hartmanj
Removed redundancies

Revision 1.3  2004/03/11 14:11:13  hartmanj
Variant removed, changes in ConfigReader interface

Revision 1.2  2004/03/09 10:17:00  bilhanan
improved handling in Instance().

Revision 1.1  2004/03/02 21:05:47  bilhanan
Enter the SAConfigurator.


*/

#include <doors/saconfigurator.h>
#include <doors/configreader.h>
#include <doors/debug.h>

SAConfigurator :: SAConfigurator (const std::string &filename)
  : ConfigReader( filename ),
    slp_DAActiveDiscoveryInterval (20) {

    if (hasValue ("net.slp.DAActiveDiscoveryInterval"))
      slp_DAActiveDiscoveryInterval = getIntValue ("net.slp.DAHeartBeat");
}
  
SAConfigurator :: ~SAConfigurator (void) {

} 

SAConfigurator *SAConfigurator :: Init (const std::string &filename) {
 
  if (_instance == 0){
    
    // SAConfigurator created for the first time
    _DOORS_DEBUG(0, "Launching SAConfigurator");
    _instance = new SAConfigurator (filename);
  } else { 

    // SAConfigurator exists, destroy and recreate
    _DOORS_DEBUG (0, "Re-launching SAConfigurator");
    delete _instance;
    _instance = new SAConfigurator (filename);
  }
  
  return _instance;
}

SAConfigurator *SAConfigurator :: Instance () {
  
  if (!_instance) {
    
    _DOORS_DEBUG (0, "Warning: No instance exists yet. Create with SAConfigurator::Init(filename) before using SAConfigurator::Instance(). ");
    _DOORS_DEBUG (0, "Seeing if SACONFIG environment variable exists");
    
    if (getenv ("SACONFIG")) {
      _DOORS_DEBUG (0, "Using config filename in SACONFIG environment variable");
      return (SAConfigurator::Init (getenv ("SACONFIG")));
    } else {
      _DOORS_DEBUG (0, "Error: SACONFIG environment variable not set. Exiting");
      exit (-1);
    }
  }
  return _instance;
}

SAConfigurator *SAConfigurator :: _instance = 0;

