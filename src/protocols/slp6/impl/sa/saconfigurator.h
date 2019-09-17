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

$Log: saconfigurator.h,v $
Revision 1.2  2004/04/06 08:36:17  hartmanj
Removed slp_IPv6Scope and slp_SlpPort from class. They hide ConfigReader::slp_IPv6Scope and ConfigReader::slp_SlpPort

Revision 1.1  2004/03/02 21:05:47  bilhanan
Enter the SAConfigurator.


*/

#ifndef SACONFIGURATOR_H
#define SACONFIGURATOR_H


#include <string>
#include <doors/configreader.h>

class SAConfigurator : public ConfigReader {

  public:

  static SAConfigurator *Init (const std::string &filename);
  static SAConfigurator *Instance (void);
  virtual ~SAConfigurator (void);
  
  int slp_DAActiveDiscoveryInterval;
  
private:
  
  static SAConfigurator *_instance;
  
  SAConfigurator (const std::string &filename);
  
  
};


#endif // SACONFIGURATOR_H
