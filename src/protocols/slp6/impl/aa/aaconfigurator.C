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

$Log: aaconfigurator.C,v $
Revision 1.3  2006/11/21 19:52:11  bilhanan
development

Revision 1.2  2006/07/29 21:42:09  bilhanan
AA improvements

Revision 1.1  2004/10/12 11:57:18  bilhanan
AA code merged into main tree from src/experimental

Revision 1.1  2004/05/25 09:39:21  hartmanj
Initial versions


*/

#include <doors/aaconfigurator.h>
#include <doors/configreader.h>
#include <doors/debug.h>

AAConfigurator :: AAConfigurator (const std::string &filename)
  : ConfigReader( filename ), 
    slpm6_AAHeartBeat (5),
    slpm6_AAMinDelay (2),
    _mcast (false),
    _srvproxy (false),
    _notify (false),
    _mesh (false),
    _remdns (false),
    _netcapabilities (0) {

  if (hasValue ("net.slpm6.AAHeartBeat"))
    slpm6_AAHeartBeat = getIntValue ("net.slpm6.AAHeartBeat");

  if (hasValue ("net.slpm6.AAMinDelay"))
    slpm6_AAMinDelay = getIntValue ("net.slpm6.AAMinDelay");

  if (hasValue ("net.slpm6.AAPrefix"))
    slpm6_AAPrefix = getStrValue  ("net.slpm6.AAPrefix");

  if (hasValue ("net.slpm6.Mcast"))
    _mcast = getIntValue ("net.slpm6.Mcast");

  if (hasValue ("net.slpm6.AAProxy"))
    _srvproxy = getIntValue ("net.slpm6.AAProxy");

  if (hasValue ("net.slpm6.RFC3082"))
    _notify =  getIntValue ("net.slpm6.RFC3082");

  if (hasValue ("net.slpm6.RFC3528"))
    _mesh = getIntValue ("net.slpm6.RFC3528");

  if (hasValue ("net.slpm6.RFC3832"))
    _remdns = getIntValue ("net.slpm6.RFC3832");

  if (_mcast == true)
    _netcapabilities |= BIT_0;

   if (_srvproxy == true)
    _netcapabilities |= BIT_1;

   if (_notify == true)
    _netcapabilities |= BIT_2;

   if (_mesh == true)
    _netcapabilities |= BIT_3;

   if (_remdns == true)
    _netcapabilities |= BIT_4;
 

}
  
AAConfigurator :: ~AAConfigurator (void) {

} 

AAConfigurator *AAConfigurator :: Init (const std::string &filename) {
 
  if (_instance == 0){
    
    // AAConfigurator created for the first time
    _DOORS_DEBUG(0, "Launching AAConfigurator");
    _instance = new AAConfigurator (filename);
  } else { 

    // AAConfigurator exists, destroy and recreate
    _DOORS_DEBUG (0, "Re-launching AAConfigurator");
    delete _instance;
    _instance = new AAConfigurator (filename);
  }
  
  return _instance;
}

AAConfigurator *AAConfigurator :: Instance () {
  
  if (!_instance) {
    
    _DOORS_DEBUG (0, "Warning: No instance exists yet. Create with AAConfigurator::Init(filename) before using AAConfigurator::Instance(). ");
    _DOORS_DEBUG (0, "Seeing if AACONFIG environment variable exists");
    
    if (getenv ("AACONFIG")) {
      _DOORS_DEBUG (0, "Using config filename in AACONFIG environment variable");
      return (AAConfigurator::Init (getenv ("AACONFIG")));
    } else {
      _DOORS_DEBUG (0, "Error: AACONFIG environment variable not set. Exiting");
      exit (-1);
    }
  }
  return _instance;
}

AAConfigurator *AAConfigurator :: _instance = 0;

