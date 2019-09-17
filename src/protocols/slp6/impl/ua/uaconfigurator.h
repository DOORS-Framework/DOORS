//  Editor-Info: -*- C++ -*-

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

$Log: uaconfigurator.h,v $
Revision 1.3  2004/03/30 06:07:06  hartmanj
Fixes to rid compile errors

Revision 1.2  2004/03/27 12:45:59  bilhanan
rewritten ua functionality

Revision 1.1  2004/03/08 09:03:25  bilhanan
UAConfigurator appears in a puff of smoke.


*/

#ifndef UACONFIGURATOR_H
#define UACONFIGURATOR_H


#include <string>
#include <doors/configreader.h>

class UAConfigurator : public ConfigReader {

  public:

  static UAConfigurator *Init (const std::string &filename);
  static UAConfigurator *Instance (void);
  virtual ~UAConfigurator (void);
  
  bool slp_passiveDADetection;
  bool slp_useMulticast;
  int slp_DAActiveDiscoveryInterval;
  int slp_multicastMaximumWait;
  int slp_maxResults;
  std::string slp_DAAddresses;

private:
  
  static UAConfigurator *_instance;
  
  UAConfigurator (const std::string &filename);
  
  
};


#endif // SACONFIGURATOR_H
