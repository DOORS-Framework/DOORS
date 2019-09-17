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

$Log: aaconfigurator.h,v $
Revision 1.3  2006/11/21 19:52:11  bilhanan
development

Revision 1.2  2006/07/29 21:42:09  bilhanan
AA improvements

Revision 1.1  2004/10/12 11:57:18  bilhanan
AA code merged into main tree from src/experimental

Revision 1.1  2004/05/25 09:39:21  hartmanj
Initial versions



*/

#ifndef AACONFIGURATOR_H
#define AACONFIGURATOR_H


#include <string>
#include <doors/configreader.h>
#include <doors/obl_dcl.h>

#define BIT_7       1
#define BIT_6       2
#define BIT_5       4
#define BIT_4       8
#define BIT_3      16
#define BIT_2      32
#define BIT_1      64
#define BIT_0     128

class AAConfigurator : public ConfigReader {

  public:

  static AAConfigurator *Init (const std::string &filename);
  static AAConfigurator *Instance (void);
  virtual ~AAConfigurator (void);
  
  int slpm6_AAHeartBeat;
  int slpm6_AAMinDelay;

  std::string slpm6_AAPrefix;
  bool _mcast;
  bool _srvproxy;
  bool _notify;
  bool _mesh;
  bool _remdns;
  Uint8 _netcapabilities;

private:
  
  static AAConfigurator *_instance;
  
  AAConfigurator (const std::string &filename);
  
  
};


#endif // AACONFIGURATOR_H
