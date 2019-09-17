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

$Log: daconfigurator.h,v $
Revision 1.3  2004/04/06 08:36:17  hartmanj
Removed slp_IPv6Scope and slp_SlpPort from class. They hide ConfigReader::slp_IPv6Scope and ConfigReader::slp_SlpPort

Revision 1.2  2004/03/01 09:05:46  bilhanan
code fixing, introducing Init()

Revision 1.1  2004/02/01 22:58:00  bilhanan
Enter daconfigurator singleton.


*/

#ifndef DACONFIGURATOR_H
#define DACONFIGURATOR_H


#include <string>
#include <doors/configreader.h>

class DAConfigurator : public ConfigReader {

  public:

  static DAConfigurator *Init (const std::string &filename);
  static DAConfigurator *Instance (void);
  virtual ~DAConfigurator (void);
  
  int slp_DAHeartBeat;
  
private:
  
  static DAConfigurator *_instance;
  
  DAConfigurator (const std::string &filename);
  
  
};


#endif // DACONFIGURATOR_H
