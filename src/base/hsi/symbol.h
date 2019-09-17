//
// $Id: symbol.h,v 1.5 2002/08/14 09:31:38 bilhanan Exp $
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

$Log: symbol.h,v $
Revision 1.5  2002/08/14 09:31:38  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.4  2002/07/25 10:08:38  ik
Use std::string.

Revision 1.3  2002/07/23 14:25:17  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.2  2002/05/05 21:24:10  ik
class-keyword added to friends.

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.3  2001/12/12 12:52:34  ik
Added Demon handler number.

Revision 1.2  2001/06/08 16:46:03  bilhanan
Full Licence clause into DOORS 0.2


*/



#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <doors/obl_string.h>
#include <doors/tqueue.h>
#include <doors/ocoremac.h>

class Symbol;
class SymbolHandler;
class TreeSearcher;
class SymbolChildIterator;

// 
//Immune: SymbolList
// 
//Description: 
//  A template list of symbols
// 

typedef TQueue<Symbol> SymbolList;

// 
//Immune: SymbolListIterator
// 
//Description: 
//  An iterator for Template list of symbols
// 

typedef TQueueListIterator<Symbol> SymbolListIterator;

#define LEMON_HANDLER  0
#define GPT_HANDLER    1
#define DEMON_HANDLER  2
#define MAX_HANDLER    3

//
//Class: Symbol
//
//Description: 
//  This class declares a symbol. The symbol is an entity
//  who has a reference to object. The object can be any
//  kind of c++ type. 
//
//  A main function of the symbol is carry a name of the
//  object, parent of the symbol and children if any.
//  


class Symbol {
    friend class TreeSearcher;
    friend class SymbolChildIterator;

  public:
    Symbol (void *o, std::string n, void *p, bool s);
    virtual ~Symbol (void);

    bool isStatic (void) const;
    bool isMyName (std::string n) const;
    bool isMyObject (const void *o) const;
    bool isMyParent (const Symbol *s) const;

    void setName (std::string n);
    std::string name (void) const;
    void *object (void) const;
    
    void setHandler (SymbolHandler *h, Uint8 hno);
    void removeHandler (SymbolHandler *h);
    SymbolHandler *handler (Uint8 hno);
    SymbolHandler ** handlers (void);
    void update (void);

    void setParent (Symbol *s);
    Symbol *parent (void);

    void addChild (Symbol *s);
    Symbol *removeChild (Symbol *s);
    Symbol *removeParent (void);

    _MT_SETSLOCK_POINTER
    _MT_GETSLOCK_POINTER
    _MT_UPDATELOCKS_DEC 

  private:
    std::string _name;       // symbol name
    void *_object;           // object pointer symbol is representing
    void *_parentObject;     // before parent symbol is created this 
                             // remembers the parent
    Symbol *_parent;         // symbols parent symbol in tree
    SymbolList _children;    // symbols children 
    SymbolHandler *_handler [MAX_HANDLER]; // handler of the objects
    bool _state;        // static or dynamic allocation marker

    _MT_POINTER_TO_SLOCK
};

#endif  // SYMBOL_H

