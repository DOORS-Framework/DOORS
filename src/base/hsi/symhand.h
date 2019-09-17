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

$Log: symhand.h,v $
Revision 1.3  2004/01/19 10:32:47  bilhanan
#include fixed.

Revision 1.2  2002/08/14 09:31:38  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:04  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:06  bilhanan
Full licence clause added into doors 0.1


*/



#ifndef SYMHAND_H
#define SYMHAND_H

#include <doors/symbol.h>

//
//Class: SymbolHandler
//
//Description: 
//  This class is an abstract base class for all symbol 
//  handlers. It has a pointer to the symbol who owns it.
//  
//  When user wants to define his own handlers he should 
//  derive them from this class.
//

class SymbolHandler {
  public:
    SymbolHandler (void);
    SymbolHandler (Symbol *s);
    virtual ~SymbolHandler (void);

    Symbol *symbol (void) const {return _symbol;}

    virtual void update (void) = 0;

    _MT_SETSLOCK_POINTER
    _MT_GETSLOCK_POINTER
    
  private:
    Symbol *_symbol;
    _MT_POINTER_TO_SLOCK
};


#endif  // SYMHAND_H

