//Editor-Info: -*- C++ -*-
//  Copyright 2000 
//  	Telecoms Lab, Tampere University of Technology.  All rights reserved.

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
//
//  Subject: DEMON core support header file 
//  Author:  Ilkka Karvinen, TTKK/DMI

/*
$Log: demoncore.h,v $
Revision 1.1.1.1  2002/05/13 14:54:40  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.5  2001/12/13 10:24:41  ik
Structure change.

Revision 1.4  2001/12/12 13:49:56  ik
Demon modified to use Scheduler::Instance().

Revision 1.3  2001/12/12 13:19:41  ik
Log tag added.

*/

#ifndef DEMONCORE_H
#define DEMONCORE_H

#include <doors/demonhand.h>
#include <doors/demonhc.h>

#define DEMON_INIT { TQueue<HandlerCreator> *creators = \
                          HandlerCreatorPool::instance()->creators (); \
			  creators->insert(new DemonHandlerCreator); }

#define DEMON_DESTROY { \
  SymbolHandler *th = symbolTable.getStaticTree ()->handler (DEMON_HANDLER); \
  if (th) { delete th; th = 0; } }

#endif
