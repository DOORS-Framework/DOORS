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

$Log: symbol.C,v $
Revision 1.4  2002/08/14 09:31:38  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.3  2002/07/25 10:08:38  ik
Use std::string.

Revision 1.2  2002/07/23 14:25:17  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:15  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:06  bilhanan
Full licence clause added into doors 0.1


*/


#include <string>
#include <doors/symbol.h>
#include <doors/symhand.h>
#include <doors/symiter.h>
#include <doors/symtable.h>
#include <doors/symhc.h>
#include <doors/debug.h>
#include <string.h>

//
//Constructor: Symbol
//
//Parameters: 
//  void *o, 
//  std::string n, 
//  void *p, 
//  bool s
//
//Description: 
//  Creates a new symbol with given parameters.
//
//  The parameter o is a pointer to the object of this
//  symbol. The parameter n is a name of the object. 
//  The parameter p is a parent object for o. The symbol 
//  will be dynamic the parameter s is zero, otherwise 
//  it a static symbol will be created.
//
Symbol :: Symbol(void *o, std::string n, void *p, bool s) 
    : _name(n), 
      _object(o), 
      _parentObject(p),
      _parent(0),
      _state(s)  
{
    _MT_RESET_POINTER;
    _MT_SYNCMONITOR;

    for (int i = 0; i < MAX_HANDLER; i++) {
	_handler [i] = 0;
    }
}


//
//Destructor: Symbol
//
//Description: 
//  Deletes this symbol and all its children recursively.
//  If the symbol has a hendler it will be deleted too.
//

Symbol :: ~Symbol (void) {

    _MT_SYNCMONITOR;
    Symbol *s = 0;

    if (_parent) {
	_parent->removeChild (this);
    }

    while (s = _children.remove ()) {
	delete s;
    }

    for (int i = 0; i < MAX_HANDLER; i++) {
	if (_handler [i]) {
	    delete _handler [i];
	}
    }
}

//
//Function: isStatic
//
//Member-Of: Symbol
//
//Return: bool
//
//Description: 
//  Returns non-zero value if this symbol is static type. 
//  Otherwise the return value is true.
//

bool Symbol :: isStatic (void) const {

    _MT_SYNCMONITOR;
    return (_state) ? true : false;
}


//
//Function: isMyName
//
//Member-Of: Symbol
//
//Parameters: std::string n
//
//Return: bool
//
//Description: 
//  Returns non-zero value if the name of the object 
//  of this symbol is same as a parameter n. Otherwise
//  the return value is zero.
//

bool Symbol :: isMyName (std::string n) const {

    _MT_SYNCMONITOR;
    bool rc = false;

    if (!n.empty()) {
	rc = (n != _name);
    } else {
	rc = (_name.empty());
    }

    return rc;
}


//
//Function: isMyObject
//
//Member-Of: Symbol
//
//Parameters: void *o
//
//Return: bool
//
//Description: 
//  Returns non-zero value if the objectof this symbol 
//  is same as given object o. Otherwise the return 
//  value is zero.
//

bool Symbol :: isMyObject (const void *o) const {

    _MT_SYNCMONITOR;
    return (o == _object) ? true : false;
}


//
//Function: isMyParent
//
//Member-Of: Symbol
//
//Parameters: Symbol *s
//
//Return: bool
//
//Description: 
//  Returns non-zero value if the parent object 
//  of this symbol is same as given symbol's 
//  object. Otherwise the return value is zero.
//

bool Symbol :: isMyParent (const Symbol *s) const {

    _MT_SYNCMONITOR;
    bool rc = false;

    rc = (s) ? s->isMyObject (_parentObject) : ((_parentObject == 0) ? true : false);

    return rc;
}

//
//Function: setName
//
//Member-Of: Symbol
//
//Parameters: std::string n
//
//Description: 
//  Sets new name for this symbol.
//  Previous name is destroyed.
//

void Symbol :: setName (std::string n) {

    _MT_SYNCMONITOR;
    _name = n;
}


//
//Function: name
//
//Member-Of: Symbol
//
//Return: std::string 
//
//Description: 
//  Returns a name of this symbol.
//

std::string Symbol :: name (void) const {

    _MT_SYNCMONITOR;
    return _name;
}


//
//Function: object
//
//Member-Of: Symbol
//
//Return: void *
//
//Description: 
//  Returns a pointer to the object of this symbol.
//

void * Symbol :: object (void) const {

    _MT_SYNCMONITOR;
    return _object;
}


//
//Function: setHandler
//
//Member-Of: Symbol
//
//Parameters: SymbolHandler *h
//
//Description: 
//  Attaches a given handler to this symbol.
//
void Symbol :: setHandler (SymbolHandler *h, Uint8 hno) 
{
    _MT_SYNCMONITOR;
    _DOORS_DEBUG(!_handler[hno],"overriding existent handler pointer");
    _handler [hno] = hno < MAX_HANDLER ? h : 0;
}


//
//Function: handler
//
//Member-Of: Symbol
//
//Return: SymbolHandler *
//
//Description: 
//  Returns a pointer to handler of this symbol.
//

SymbolHandler ** Symbol :: handlers (void) {

    _MT_SYNCMONITOR;
    return _handler;
}

//
//Function: handler
//
//Member-Of: Symbol
//
//Parameters: Uint8
//
//Return: SymbolHandler *
//
//Description: 
//  Returns a pointer to handler specified with hno 
//  of this symbol.
//

SymbolHandler * Symbol :: handler (Uint8 hno) {

    _MT_SYNCMONITOR;
    return _handler [hno];
}

//
//Function: removeHandler
//
//Member-Of: Symbol
//
//Parameters: SymbolHandler * 
//
//Description: 
//  Removes the handler h from the handler table.
//

void Symbol :: removeHandler (SymbolHandler *h) {

    _MT_SYNCMONITOR;
    for (int i = 0; i < MAX_HANDLER; i++) {
	if (_handler [i] && _handler [i] == h) {
	    _handler [i] = 0;
	    break;
	}
    }
}

//
//Function: update
//
//Member-Of: Symbol
//
//Description: 
//  Update information to all hadlers of the
//  table.
//

void Symbol :: update (void) {

    _MT_SYNCMONITOR;
    for (int i = 0; i < MAX_HANDLER; i++) {
        if (_handler [i]) {
            _handler [i]->update ();
        }
    }
}


//
//Function: setParent
//
//Member-Of: Symbol
//
//Parameters: Symbol *s
//
//Description: 
//  Sets a new parent to this symbol. The parent
//  gets also a new child in its children list.
//
//  If there is already parent the symbol will 
//  loose it as well as the parent will loose this 
//  symbol. 
//

void Symbol :: setParent (Symbol *s) {

    _MT_SYNCMONITOR;
    if (_parent) {
	// remove current relation
	_parent->removeChild (this);
    }

    _parent = s;

    if (s) {

      // If s is a symbol for symboltable, _parentObject 
      // is not updated to 0
      if (s->object()) {
        _parentObject = s->object();
      }
      s->addChild (this);
    }
}


//
//Function: parent
//
//Member-Of: Symbol
//
//Return: Symbol *
//
//Description: 
//  Returns a pointer to a parent of this symbol.
//

Symbol * Symbol :: parent (void) {
    
    _MT_SYNCMONITOR;
    return _parent;
}


//
//Function: addChild
//
//Member-Of: Symbol
//
//Parameters: Symbol *s
//
//Description: 
//  Appends a new child to this symbol. In addition
//  the parent of the child given as a parameter is 
//  set to point this symbol. 
//

void Symbol :: addChild (Symbol *s) {

    _MT_SYNCMONITOR;
    if (s) {
	if (s->parent () == this) {
	    _children.append (s);

	} else {
	    s->setParent (this);
	}
    }
}


//
//Function: removeChild
//
//Member-Of: Symbol
//
//Parameters: Symbol *s
//
//Return: Symbol *
//
//Description: 
//  Removes a child given as a parameter from the
//  children list of this symbol. A parent of the
//  removed child is also updated.
//
//  Returns a removed child if found in the children
//  list. Otherwise the return value is zero.
//

Symbol * Symbol :: removeChild (Symbol *s) {

    _MT_SYNCMONITOR;
    Symbol *removed = _children.remove (s);

    if (removed) {
	s->setParent (0);
    }

    return removed;
}


//
//Function: removeParent
//
//Member-Of: Symbol
//
//Return: Symbol *
//
//Description: 
//  Removes a parent of this symbol. Then this symbol 
//  is removed from a children list of the parent
//  symbol.
//
//  Returns a removed parent.
//

Symbol * Symbol :: removeParent (void) {

    _MT_SYNCMONITOR;
    Symbol *removed = _parent;

    if (_parent) {
	_parent->removeChild (this);
	_parent = 0;
    }

    return removed;
}

#if 0
int Symbol::stubCreateToStaticArea(void *object,void *parent,const char *name)
{
    Symbol *symbol = new Symbol(object,name,parent, true);
    symbolTable.add(symbol);
    TQueueListIterator<HandlerCreator> 
	tools(HandlerCreatorPool::instance()->creators());
    HandlerCreator *creator; 
    while(creator = tools.next()) { 
	creator->createHandler(object,symbol); 
    } 
}

int Symbol::stubCreateToDynamicArea(void *object,void *parent,const char *name)
{
    Symbol *symbol = new Symbol((void *)object,name,(void *)parent,false);
    symbolTable.add(symbol);
    TQueueListIterator<HandlerCreator> 
	tools(HandlerCreatorPool::instance()->creators());
    HandlerCreator *creator; 
    while (creator = tools.next()) { 
	creator->createHandler (object, symbol); 
    } 
}

int Symbol::stubDestroy(void *object)
{
    symbolTable.remove(object); 
    return 1;
}

int Symbol::stubUpdate(void *object)
{
    TreeSearcher searcher(symbolTable.getStaticTree()); 
    Symbol *symbol = searcher.get(ptr);
    if(!symbol) {
	searcher.reset(symbolTable.getDynamicTree());
	symbol = searcher.get(ptr)
    }
    if(symbol) {
	symbol->update();
    }
    return 1; // for syntax evaluation
}

int Symbol::stubMove(void *object,void *parent)
{
    symbolTable.updateRelation(object,parent);
    return 1;
}
#endif

_MT_UPDATELOCKS;


