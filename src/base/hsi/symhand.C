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

$Log: symhand.C,v $
Revision 1.3  2004/01/19 11:14:28  bilhanan
#include fixed.

Revision 1.2  2002/08/14 09:31:38  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:16  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:06  bilhanan
Full licence clause added into doors 0.1


*/




#include <doors/symhand.h>


//
//Constructor: SymbolHandler
//
//Description:
//  Creates a new symbol handler with no symbol.
//

SymbolHandler :: SymbolHandler (void) 
    : _symbol (0)
{
    _MT_RESET_POINTER;
    _MT_SYNCMONITOR;
}


//
//Constructor: SymbolHandler
//
//Parameters: Symbol *s
//
//Description:
//  Creates a new symbol handler for given symbol s.
//

SymbolHandler :: SymbolHandler (Symbol *s)
    : _symbol (s)
{
    _MT_RESET_POINTER;
    _MT_SYNCMONITOR;
}


//
//Destructor: SymbolHandler
//
//Description:
//  Deletes this symbol handler. The link from the
//  corresponding symbol to this object is set to null.
//

SymbolHandler :: ~SymbolHandler (void)
{
    _MT_SYNCMONITOR;
    _symbol->removeHandler (this);
}




