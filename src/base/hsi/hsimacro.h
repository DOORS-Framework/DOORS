// 
//  Copyright 1999 
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
 
 
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

$Log: hsimacro.h,v $
Revision 1.4  2004/01/19 10:29:57  bilhanan
#include fixed.

Revision 1.3  2002/08/14 09:31:37  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/08/09 06:46:28  hartmanj
#include <ocoremac.h> to #include <doors/ocoremac.h>

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:34  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:14  bilhanan
Full licence clause added into doors 0.1


*/

#ifndef HSIMACRO_H
#define HSIMACRO_H

#include <doors/doorsconf.h>

#if _MT_ENABLED
#if _CF_TRACE
#include <doors/symtable.h>

#define _MT_STABLE_MONITOR\
\
   FuncMonitor _DOORS_stableMonitor (symbolTable.getSyncLock()); \
\

#else
#define _MT_STABLE_MONITOR
#endif

#else
#define _MT_STABLE_MONITOR

#include <doors/ocoremac.h>

#endif
#endif


