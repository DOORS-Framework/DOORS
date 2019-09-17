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

$Log: symiter.h,v $
Revision 1.3  2004/01/19 10:33:33  bilhanan
#include fixed.

Revision 1.2  2002/07/23 14:25:18  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:04  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:07  bilhanan
Full licence clause added into doors 0.1


*/



#ifndef SYMITER_H
#define SYMITER_H

#include <doors/symbol.h>


extern bool sameObject (Symbol *s, const void *o);

//
//Class: TreeSearcher
//
//Description: 
//  This class can be used to search symbol(s) from
//  the symbol tree. It iterates the tree recursively
//  from given root symbol.
//
//  The iterating algorithm seeks firstly root's 
//  children and then first children's children and 
//  so on. 
//

class TreeSearcher {
  public:
    TreeSearcher (Symbol *r, bool (*cf) (Symbol *s, const void *k) = sameObject);
    virtual ~TreeSearcher (void);

    void reset (Symbol *r, bool (*cf) (Symbol *s, const void *k) = 0);

    Symbol *get (const void *k, int level = -1);
    SymbolList *getAll (const void *k, int level = -1);

  private:
    Symbol *getChildren (Symbol *parent, const void *k);
    void getAllChildren (Symbol *parent, const void *k, SymbolList *sl);
    
    bool isTooDeep (void);

    Symbol *root;
    bool (*compareFunc) (Symbol *s, const void *k);

    int deep, currentLevel;
};


//
//Class: SymbolChildIterator
//
//Description: 
//  This class can be used to iterate children of
//  given symbol. It uses TQueueListIterator class
//  of the util module.
//

class SymbolChildIterator {
  public:
    SymbolChildIterator (Symbol *r);
    virtual ~SymbolChildIterator (void);

    Symbol *next (void);
    Symbol *reset (Symbol *r = 0);

  private:
    SymbolListIterator iterator;
};


extern bool sameName (Symbol *s, const void  *n);

extern bool sameObject (Symbol *s, const void *o);

extern bool sameParent (Symbol *s, const void *o);

#endif // SYMITER_H
