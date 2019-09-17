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

$Log: vaconfigurator.h,v $
Revision 1.2  2007/02/23 10:43:10  bilhanan
supporting ll address retrieval from config

Revision 1.1  2006/08/10 15:39:07  bilhanan
initial commit

Revision 1.1  2004/05/12 08:33:20  hartmanj
Development


*/

#ifndef VACONFIGURATOR_H
#define VACONFIGURATOR_H


#include <string>
#include <doors/configreader.h>

class VAConfigurator : public ConfigReader {

  public:

  static VAConfigurator *Init (const std::string &filename);
  static VAConfigurator *Instance (void);
  virtual ~VAConfigurator (void);
  
  int slp_DAActiveDiscoveryInterval;
  int slpm6_AAAdvertTimeoutMax;
  std::string slpm6_HomeAddr;
  std::string slpm6_LinkLocalAddr;
  
private:
  
  static VAConfigurator *_instance;
  
  VAConfigurator (const std::string &filename);
  
  
};


#endif // VACONFIGURATOR_H
