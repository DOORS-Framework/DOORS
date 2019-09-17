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

$Log: uaconfigurator.C,v $
Revision 1.7  2004/06/09 13:29:32  hartmanj
Typo fix

Revision 1.6  2004/03/30 06:07:06  hartmanj
Fixes to rid compile errors

Revision 1.5  2004/03/27 12:45:59  bilhanan
rewritten ua functionality

Revision 1.4  2004/03/11 14:11:37  hartmanj
Variant removed, changes in ConfigReader interface

Revision 1.3  2004/03/09 10:17:15  bilhanan
improved handling in Instance().

Revision 1.2  2004/03/08 10:47:07  hartmanj
Fix ambiguity in a getValue() call

Revision 1.1  2004/03/08 09:03:25  bilhanan
UAConfigurator appears in a puff of smoke.


*/

#include <doors/uaconfigurator.h>
#include <doors/debug.h>

UAConfigurator :: UAConfigurator (const std::string &filename)
  : ConfigReader(filename),slp_DAActiveDiscoveryInterval (20), slp_useMulticast (true),
  slp_passiveDADetection (true) {

    if (hasValue ("net.slp.DAActiveDiscoveryInterval"))
      slp_DAActiveDiscoveryInterval = getIntValue ("net.slp.DAHeartBeat");

   if (hasValue ("net.slp.DAAddresses"))
      slp_DAAddresses = getStrValue ("net.slp.DAAddresses");

    if (hasValue ("net.slp.passiveDADetection")) {
      int passiveDA = getIntValue ("net.slp.passiveDADetection");
      if (passiveDA < 1)
	slp_passiveDADetection = false;
      else
	slp_passiveDADetection = true;
    }
}
  
UAConfigurator :: ~UAConfigurator (void) {

} 

UAConfigurator *UAConfigurator :: Init (const std::string &filename) {
 
  if (_instance == 0){
    
    // UAConfigurator created for the first time
    _DOORS_DEBUG(0, "Launching UAConfigurator");
    _instance = new UAConfigurator (filename);
  } else { 

    // UAConfigurator exists, destroy and recreate
    _DOORS_DEBUG (0, "Re-launching UAConfigurator");
    delete _instance;
    _instance = new UAConfigurator (filename);
  }
    
  return _instance;
}

UAConfigurator *UAConfigurator :: Instance () {

  if (!_instance) {
    _DOORS_DEBUG (0, "Error: No instance exists yet. Create with UAConfigurator::Init(filename) before using UAConfigurator::Instance()");
    _DOORS_DEBUG (0, "Seeing if UACONFIG environment variable exists");
    
    if (getenv ("UACONFIG")) {
      _DOORS_DEBUG (0, "Using config filename in UACONFIG environment variable");
      return (UAConfigurator::Init (getenv ("UACONFIG")));
    } else {
      _DOORS_DEBUG (0, "Error: UACONFIG environment variable not set. Exiting");
      exit (-1);
    }
  }
  return _instance;
}

UAConfigurator *UAConfigurator :: _instance = 0;

