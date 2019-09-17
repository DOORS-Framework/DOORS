//
//  Copyright 1999
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
//
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

$Log: errmgmt.C,v $
Revision 1.3  2002/07/25 10:08:40  ik
Use std::string.

Revision 1.2  2002/06/03 09:19:55  bilhanan
DOORS header file amendments.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:36  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.5  2001/06/08 08:39:36  bilhanan
Full licence clause added into doors 0.1


*/


#include <string>
#include <iostream>
#include <doors/errmgmt.h>
#include <stdlib.h>

uint16 DefaultErrorManager::maxErrors = 16;

void DefaultErrorManager::info(std::string str)
{
  if(flags.info) {
    std::cerr << "info: " << str << std::endl;
  }
}

void DefaultErrorManager::warn(std::string str)
{
  if(flags.warn) {
    std::cerr << "warning: " << str << std::endl;
  }
}

void DefaultErrorManager::error(std::string str)
{
    std::cerr << "error: " << str << std::endl;
    _errors++;
    if(_errors >= maxErrors) {
        std::cerr << "too many errors: quitting" << std::endl;
	exit(1);
    }
}

void DefaultErrorManager::fatal(std::string str)
{
    std::cerr << "fatal: " << str << std::endl;
    exit(1);
}

void DefaultErrorManager::hideInfo()
{
    flags.info = 0;
}

void DefaultErrorManager::showInfo()
{
    flags.info = 1;
}

void DefaultErrorManager::hideWarn()
{
    flags.warn = 0;
}

void DefaultErrorManager::showWarn()
{
    flags.warn = 1;
}

//Name: em
//Type: instance
//View: public
//Context: ErrorManager
ErrorManager *em = new DefaultErrorManager();



