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

$Log: symtable.h,v $
Revision 1.4  2004/01/19 10:34:11  bilhanan
#include fixed.

Revision 1.3  2002/08/14 09:31:38  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/07/23 14:25:18  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:05  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:08  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <doors/symbol.h>

class TreeSearcher;


//
//Class: SymbolTable
//
//Description: 
//  This class declares a symbol table. It consists
//  of two hierarhical ordered trees for static and
//  dynamic symbols. 
//
//  The symbol table provides methods to two directions.
//  The macro interface calls the methods of this 
//  class when updating the symbol table and the manager
//  will also need services of the symbol table.
// 

class SymbolTable {
  public:
    SymbolTable (void);
    virtual ~SymbolTable (void);

    void add (Symbol *s);
    bool updateRelation (void *ch, void *par);
    bool remove (void *o);

    Symbol *getStaticTree (void);
    Symbol *getDynamicTree (void);

    _MT_GETSYNCLOCK
  protected:
    _MT_SYNCLOCK
  private:
    Symbol symbolTree;
    Symbol tempTree;  
};


extern SymbolTable symbolTable;


#endif  // SYMTABLE_H



