// -*- C++ -*-
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

$Log: macroif.h,v $
Revision 1.4  2005/02/01 10:13:36  bilhanan
Added RENAME_SYMBOL macro

Revision 1.3  2002/08/14 09:31:37  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.2  2002/07/23 14:25:17  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:40  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:05  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef MACROIF_H
#define MACROIF_H

#include <doors/symbol.h>

//+ OBL_SYMBOL_CREATE_STATIC
//.  Creates a new static symbol for an object. The symbol 
//.  gets a handler to handle a symbol. Then it is added 
//.  into a global symbol table. 
//.
//.  The first parameter object is a object for a new symbol
//.  to be created. The second parameter mother is a parent 
//.  of the new symbol. If it is zero the symbol will be set
//.  as a global symbol.
//-
#define OBL_SYMBOL_CREATE_STATIC(object,mother) \
do { \
    Symbol *symbol = new Symbol((void *)&object,#object,(void *)mother,true); \
    symbolTable.add(symbol); \
    TQueueListIterator<HandlerCreator> tools(HandlerCreatorPool::instance()->creators()); \
    HandlerCreator *creator; \
    while (creator = tools.next()) { \
	creator->createHandler (&object, symbol); \
    } \
} while(0)

//+ OBL_SYMBOL_CREATE_DYNAMIC
//.  Creates a new dynamic symbol for an object. The symbol 
//.  gets a handler to handle a symbol. Then it is added 
//.  into a global symbol table. 
//.
//.  The first parameter object is a object of a new symbol
//.  to be created. The second parameter mother is a parent 
//.  of the new symbol. If it is zero the symbol will be set
//.  into a temporary symbol tree.
//-
#define OBL_SYMBOL_CREATE_DYNAMIC(object,name,mother) \
do { \
    Symbol *symbol = new Symbol((void *)object,name,(void *)mother,false); \
    symbolTable.add(symbol); \
    TQueueListIterator<HandlerCreator> tools(HandlerCreatorPool::instance()->creators()); \
    HandlerCreator *creator; \
    while (creator = tools.next ()) { \
	creator->createHandler(object, symbol); \
    } \
} while(0)

//+ OBL_SYMBOL_MOVE
//.  Moves a symbol to a given location in the symbol tree
//.
//.  The first parameter object is a object of the symbol
//.  to be moved. The second parameter mother is a new parent 
//.  to the symbol. If it is zero the symbol will be set
//.  into a temporary symbol tree (dynamic symbol) or as a 
//.  global symbol (static symbol).
//-
#define OBL_SYMBOL_MOVE(object,mother) \
do { \
    symbolTable.updateRelation((void*)object,(void*)mother); \
} while(0);

//+ OBL_SYMBOL_DESTROY
//. Destroy symbol from symboltree and subsequent symbolhandlers.
//-
#define OBL_SYMBOL_DESTROY(object) \
do { \
    symbolTable.remove((void*)object); \
} while(0)

//+ OBL_SYMBOL_UPDATE
//. Tell to symbolhandler system to update the change of the state of the
//. symbol.
//-
#define OBL_SYMBOL_UPDATE(ptr) \
do { \
    TreeSearcher searcher(symbolTable.getStaticTree()); \
    Symbol *symbol = searcher.get(ptr); \
    if(!symbol) { \
         searcher.reset(symbolTable.getDynamicTree()); \
         symbol = searcher.get(ptr); \
    } \
    if(symbol) symbol->update(); \
} while(0)

#define RENAME_SYMBOL(ptr,name) \
do { \
    TreeSearcher searcher(symbolTable.getStaticTree()); \
    Symbol *symbol = searcher.get(ptr); \
    if(!symbol) { \
         searcher.reset(symbolTable.getDynamicTree()); \
         symbol = searcher.get(ptr); \
    } \
    if(symbol) symbol->setName(name); \
} while(0)

// synonyms
#define _MT_TRACE_TASK                  OBL_SYMBOL_UPDATE(this)
#define TRACE_TASK(task)                   OBL_SYMBOL_UPDATE(task)
#define TRACE_MSG(msg)                     OBL_SYMBOL_UPDATE(msg)
#define DELETE_SYMBOL(object)              OBL_SYMBOL_DESTROY(object)
#define MOVE_SYMBOL(object,mother)         OBL_SYMBOL_MOVE(object,mother)
#define DYNAMIC_SYMBOL(object,name,mother) OBL_SYMBOL_CREATE_DYNAMIC(object,name,mother)
#define STATIC_SYMBOL(object,mother)       OBL_SYMBOL_CREATE_STATIC(object,mother)

#endif  // MACROIF_H




