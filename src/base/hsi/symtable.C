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

$Log: symtable.C,v $
Revision 1.4  2002/08/14 09:31:38  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/25 22:41:09  ik
Fixes for std::string changes.

Revision 1.2  2002/07/23 14:25:18  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:17  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:07  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/symtable.h>
#include <doors/symhc.h>
#include <doors/symiter.h>

#include <doors/debug.h>

//
//Constructor: SymbolTable
//
//Description: 
//  Creates a new symbol table.
//
//  The root symbols are intialized in the symbol table.
//  A name of the root symbol of the static tree is system.
//  A name of the root symbol of the dynamic tree is temp.
//

SymbolTable :: SymbolTable (void) 
    : symbolTree (0, "", 0, true), 
      tempTree (0, "", 0, true) 
{
    _DOORS_DEBUG(0,"SymbolTable: create");
    _MT_SYNCMONITOR;
}

//
//Destructor: SymbolTable
//
//Description: 
//  Deletes this symbol table.
//
SymbolTable :: ~SymbolTable (void) 
{
    _DOORS_DEBUG(0,"SymbolTable: destroy");
    _MT_SYNCMONITOR;
}

//
//Function: getStaticTree
//
//Member-Of: SymbolTable
//
//Return: Symbol *
//
//Description: 
//  Returns a pointer to the root symbol of the 
//  static tree.
//

Symbol *SymbolTable :: getStaticTree (void) {

  _MT_SYNCMONITOR;
  return &symbolTree;
}

    
//
//Function: getDynamicTree
//
//Member-Of: SymbolTable
//
//Return: Symbol *
//
//Description: 
//  Returns a pointer to the root symbol of the 
//  dynamic tree.
//

Symbol *SymbolTable :: getDynamicTree (void) {

  _MT_SYNCMONITOR;
  return &tempTree;
}

    
//
//Function: add
//
//Member-Of: SymbolTable
//
//Parameters: Symbol *s
//
//Description: 
//  The macro interface will call this method when
//  it wants to add a new symbol to the symbol 
//  table.  
//
//  First, the method picks all symbols from dynamic
//  tree whose parent is a symbol to be added and 
//  sets them as its children. 
//
//  Then, if the symbol is static it is added into 
//  static tree, otherwise into dynamic tree.
//

void SymbolTable :: add (Symbol *s) {

  _MT_SYNCMONITOR;

  SymbolChildIterator iterator (&tempTree);
  Symbol *current = iterator.next ();
  Symbol *next = 0;
  
  while (current) {
    if (current->isMyParent (s)) {
      next = iterator.next ();
      
      tempTree.removeChild (current);
      s->addChild (current);
      
      current = iterator.reset (next);
      
    } else {
      current = iterator.next ();
    }
  }
  
  if (s->isMyParent (0)) {
    // global symbol
    if (s->isStatic ()) {
      symbolTree.addChild (s);
    } else {
      // belongs somebody not created yet
      tempTree.addChild (s);
    }
    
  } else {
    TreeSearcher searcher (&symbolTree, sameParent);
    Symbol *symbol = 0;
    
    if ((symbol = searcher.get (s)) == 0) {
      searcher.reset (&tempTree, sameParent);
      symbol = searcher.get (s);
    }
    
    if (symbol) {
      symbol->addChild (s);
    } else {
      tempTree.addChild (s);
    }
  }
}


//
//Function: updateRelation
//
//Member-Of: SymbolTable
//
//Parameters: void *ch, 
//            void *par
//
//Return: bool
//
//Description:
//  Moves a symbol to new location in the symbol tree.
//  The symbol and its parent can be either in static
//  or dynamic tree.
//
//  The macro interface will call this method when
//  there is MOVE_SYMBOL macro used.
//
//  The parameter ch is an object of the symbol to be
//  moved. The parameter par is an object of the new
//  parent symbol for the symbol to be moved. If it
//  is zero the symbol will be moved as a global
//  symbol either to the static tree or dynamic tree
//  according to the symbol.
//
//  Returns non-zero value if the operation success.
//  If there is no such symbol or its parent zero
//  will be returned.
//

bool SymbolTable :: updateRelation (void *ch, void *par) {

  _MT_SYNCMONITOR;

  TreeSearcher searcher (&tempTree, sameObject);
  
  Symbol *child = 0, *parent = 0;
  
  if ((child = searcher.get (ch)) == 0) {
    searcher.reset (&symbolTree, sameObject);
    
    if ((child = searcher.get (ch)) == 0) {
      return false;
    }
  }
  
  searcher.reset (&symbolTree, sameObject);
  
  if (par) {
    if ((parent = searcher.get (par)) == 0) {
      searcher.reset (&tempTree, sameObject);
      
      if ((parent = searcher.get (par)) == 0) {
	return false;
      }
    }
  } else {
    parent = (child->isStatic ()) ? &symbolTree : &tempTree;
  }
  
  child->removeParent ();
  parent->addChild (child);
  
  return true;
}
    

//
//Function: remove
//
//Member-Of: SymbolTable
//
//Parameters: void *s
//
//Return: bool
//
//Description: 
//  The macro interface will call this method when
//  it wants to delete symbol from the symbol table.  
//
//  First the corresponding symbol for the parameter 
//  s will be searched from the dynamic tree, then
//  from the static tree.
//
//  If such symbol was found it will bew rmoved 
//  from the symbol tree and deleted.
//
//  Returns non-zero value if the symbol was 
//  succesfully deleted. It there is no symbol
//  for given object the return value is zero.
//

bool SymbolTable :: remove (void *s) {

  _MT_SYNCMONITOR;

  TreeSearcher searcher (&tempTree, sameObject);
  
  Symbol *symbol = 0;
  
  if ((symbol = searcher.get (s)) == 0) {
    searcher.reset (&symbolTree, sameObject);
    
    if ((symbol = searcher.get (s)) == 0) {
      return false;
    }
  }
  
  symbol->removeParent ();
  delete symbol;
  
  return true;
}


SymbolTable symbolTable;
