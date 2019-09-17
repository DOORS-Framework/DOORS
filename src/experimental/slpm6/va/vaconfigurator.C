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

$Log: vaconfigurator.C,v $
Revision 1.1  2004/05/12 08:33:20  hartmanj
Development


*/

#include "vaconfigurator.h"
#include <doors/configreader.h>
#include <doors/debug.h>

VAConfigurator :: VAConfigurator (const std::string &filename)
  : ConfigReader( filename ),
    slp_DAActiveDiscoveryInterval (20), slpm6_AAAdvertTimeoutMax(600) {


    if (hasValue ("net.slp.DAActiveDiscoveryInterval"))
      slp_DAActiveDiscoveryInterval = getIntValue ("net.slp.DAHeartBeat");
    if (hasValue ("net.slpm6.AAAdvertTimeoutMax"))
      slp_DAActiveDiscoveryInterval = getIntValue ("net.slpm6.AAAdvertTimeoutMax");
    if (hasValue ("net.slpm6.slpm6_HomeAddr"))
      slpm6_HomeAddr = getStrValue ("net.slpm6.slpm6_HomeAddr");
}
  
VAConfigurator :: ~VAConfigurator (void) {

} 

VAConfigurator *VAConfigurator :: Init (const std::string &filename) {
 
  if (_instance == 0){
    
    // VAConfigurator created for the first time
    _DOORS_DEBUG(0, "Launching VAConfigurator");
    _instance = new VAConfigurator (filename);
  } else { 

    // VAConfigurator exists, destroy and recreate
    _DOORS_DEBUG (0, "Re-launching VAConfigurator");
    delete _instance;
    _instance = new VAConfigurator (filename);
  }
  
  return _instance;
}

VAConfigurator *VAConfigurator :: Instance () {
  
  if (!_instance) {
    
    _DOORS_DEBUG (0, "Warning: No instance exists yet. Create with VAConfigurator::Init(filename) before using VAConfigurator::Instance(). ");
    _DOORS_DEBUG (0, "Seeing if VACONFIG environment variable exists");
    
    if (getenv ("VACONFIG")) {
      _DOORS_DEBUG (0, "Using config filename in VACONFIG environment variable");
      return (VAConfigurator::Init (getenv ("VACONFIG")));
    } else {
      _DOORS_DEBUG (0, "Error: VACONFIG environment variable not set. Exiting");
      exit (-1);
    }
  }
  return _instance;
}

VAConfigurator *VAConfigurator :: _instance = 0;

