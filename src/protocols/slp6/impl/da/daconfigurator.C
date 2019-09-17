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

$Log: daconfigurator.C,v $
Revision 1.6  2004/03/25 12:50:58  hartmanj
Removed redundancies

Revision 1.5  2004/03/11 14:10:30  hartmanj
Variant removed, changes in ConfigReader interface

Revision 1.4  2004/03/09 10:17:30  bilhanan
improved handling in Instance().

Revision 1.3  2004/03/01 16:49:31  bilhanan
uncommented debug output.

Revision 1.2  2004/03/01 09:05:46  bilhanan
code fixing, introducing Init()

Revision 1.1  2004/02/01 22:58:00  bilhanan
Enter daconfigurator singleton.


*/

#include <doors/daconfigurator.h>
#include <doors/configreader.h>
#include <doors/debug.h>





DAConfigurator :: DAConfigurator (const std::string &filename)
  : ConfigReader(filename),
    slp_DAHeartBeat (600) {

  if (hasValue ("net.slp.DAHeartBeat"))
    slp_DAHeartBeat = getIntValue ("net.slp.DAHeartBeat");

  if (hasValue ("net.slp.IPv6Scope"))
    slp_IPv6Scope = getIntValue ("net.slp.IPv6Scope");

  if (hasValue ("net.slp.SlpPort"))
    slp_SlpPort = getIntValue ("net.slp.SlpPort");

}
  
DAConfigurator :: ~DAConfigurator (void) {

} 

DAConfigurator *DAConfigurator :: Init (const std::string &filename) {
 
  if (_instance == 0){
    
    // DAConfigurator created for the first time
    _DOORS_DEBUG(0, "Launching DAConfigurator");
    _instance = new DAConfigurator (filename);
  } else { 

    // DAConfigurator exists, destroy and recreate
    _DOORS_DEBUG (0, "Re-launching DAConfigurator");
    delete _instance;
    _instance = new DAConfigurator (filename);
  }
    
  return _instance;
}

DAConfigurator *DAConfigurator :: Instance () {
  
  if (!_instance) {
    _DOORS_DEBUG (0, "Error: No instance exists yet. Create with DAConfigurator::Init(filename) before using DAConfigurator::Instance()");
    _DOORS_DEBUG (0, "Seeing if DACONFIG environment variable exists");
    
    if (getenv ("DACONFIG")) {
      _DOORS_DEBUG (0, "Using config filename in DACONFIG environment variable");
      return (DAConfigurator::Init (getenv ("DACONFIG")));
    } else {
      _DOORS_DEBUG (0, "Error: DACONFIG environment variable not set. Exiting");
      exit (-1);
    }
  }
  return _instance;
}

DAConfigurator *DAConfigurator :: _instance = 0;

