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

$Log: symhc.C,v $
Revision 1.2  2002/06/06 14:22:43  ik
InetAddr6 symbol.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.3  2002/01/05 18:54:20  ik
Changes to give HandlerCreatorPool a bit more Singleton Design Pattern look.

Revision 1.2  2001/06/08 16:46:16  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:06  bilhanan
Full licence clause added into doors 0.1


*/


#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <doors/symbol.h>
#include <doors/symhc.h>


HandlerCreatorPool * HandlerCreatorPool :: _instance = 0;

HandlerCreatorPool * HandlerCreatorPool :: instance (void) {

    if (_instance == 0) {
      _instance = new HandlerCreatorPool;
    }

    return _instance;
}


HandlerCreatorPtrSeq *HandlerCreatorPool :: creators (void) {

    return &_creators;
}


static void gnu_tmpl_ctor_inst_fix()
{
    HandlerCreatorPtrSeq seq;
}

