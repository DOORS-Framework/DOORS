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

$Log: symiter.C,v $
Revision 1.3  2004/01/19 11:14:58  bilhanan
#include fixed.

Revision 1.2  2002/07/23 14:25:18  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:46:16  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.3  2001/06/08 08:39:07  bilhanan
Full licence clause added into doors 0.1


*/



#include <doors/symiter.h>


//
//Function: sameName
//
//Parameters: Symbol *s, 
//            const void  *n
//
//Return: bool
//
//Description: 
//  Compares name of the symbol given as a first
//  parameter to the string given as a second 
//  parameter.
//
//  This function is used in tree-searcher's get
//  function when it compares symbols by their 
//  names. 
//
//  Returns non-zero value if symbol's isMyName
//  function with second parameter returns non-zero 
//  value. Otherwise the return value is zero.
//

bool sameName (Symbol *s, const void  *n) {

    return s->isMyName ((const char *) n);
}


//
//Function: sameObject
//
//Parameters: Symbol *s, 
//            const void  *n
//
//Return: bool
//
//Description: 
//  Compares object of the symbol given as a first
//  parameter to the object given as a second 
//  parameter.
//
//  This function is used in tree-searcher's get
//  function when it compares symbols by their 
//  objects.
//
//  Returns non-zero value if symbol's isMyObject
//  function with second parameter returns non-zero 
//  value. Otherwise the return value is zero.
//

bool sameObject (Symbol *s, const void *o) {

    return (s->isMyObject (o)) ? true : false;
}


//
//Function: sameParent
//
//Parameters: Symbol *s, 
//            const void  *n
//
//Return: bool
//
//Description: 
//  Compares parent object of the symbol given as a 
//  first parameter to the parent object given as a 
//  second parameter.
//
//  This function is used in tree-searcher's get
//  function when it compares symbols by their 
//  parent objects.
//
//  Returns non-zero value if symbol's isMyParent
//  function with second parameter returns non-zero 
//  value. Otherwise the return value is zero.
//

bool sameParent (Symbol *s, const void *o) {
    
    bool rc = false;

    rc = ((const Symbol *) o)->isMyParent (s) ? true : false;

    return rc;
}


//
//Constructor: TreeSearcher
//
//Parameters: Symbol *r, 
//            bool (*cf) (Symbol *s, const void *k)
//
//Description: 
//  Creates a new tree-searcher and initializes it by
//  given parameters.
//
//  The parameter r is a root symbol. The searcher will
//  seek only sub-symbols of the root.
//
//  The parameter cf is a compare function used to test
//  matching. There is three different compare function
//  defined in hsi module, but it is possible to define
//  them more if needed in other modules.
//
//  If the second parameter was not given a sameObject
//  function will be used as a default.
//

TreeSearcher :: TreeSearcher (Symbol *r, bool (*cf) (Symbol *s, const void *k)) 
: root (r), compareFunc (cf ? cf : sameObject), deep (0), currentLevel (0) {
    
}


//
//Destructor: TreeSearcher
//
//Description: 
//  Deletes this tree-searcher.
//

TreeSearcher :: ~TreeSearcher (void) {

}


//
//Function: reset
//
//Member-Of: TreeSearcher
//
//Parameters: Symbol *r, 
//            bool (*cf) (Symbol *s, const void *k)
//
//Description: 
//  Re-initializes this tree-searcher.
//
//  The parameter r is a root symbol. The searcher will
//  seek only sub-symbols of the root.
//
//  The parameter cf is a compare function used to test
//  matching. The parameter has default value zero which
//  means that this function remains its present comparing
//  function.
//
//  There is three different compare function defined in 
//  hsi module, but it is possible to define others if 
//  needed in other modules.
//

void TreeSearcher :: reset (Symbol *r, bool (*cf) (Symbol *s, const void *k)) {

    root = r;
    if (cf != 0) {
	compareFunc = cf;
    }
    deep = 0;
    currentLevel = 0;
}


//
//Function: get
//
//Member-Of: TreeSearcher
//
//Parameters: const void *k, 
//            int level
//
//Return: bool
//
//Description: 
//  Gets first sub-symbol of the root which satisfy the 
//  compare function.
//
//  The parameter k is a compared value used in compare 
//  function. 
//  
//  The parameter level tells how deep the seeking process
//  will go in the symbol tree.
//
//  Returns a symbol if found.
//

Symbol * TreeSearcher :: get (const void *k, int level) {

    if (k == 0) {
	return 0;
    }

    Symbol *s = 0;
    
    deep = level;

    if (root) {
	if (compareFunc (root, k)) {
	    s = root;
	} else {
	    s = getChildren (root, k);
	}
	// s = (compareFunc (root, k)) ? root : getChildren (root, k);
    }

    return s;
}


//
//Function: getAll
//
//Member-Of: TreeSearcher
//
//Parameters: const void *k, 
//            int level
//
//Return: SymbolList * 
//
//Description: 
//  Gets all sub-symbol of the root that satisfy the 
//  compare function.
//
//  The parameter k is a compared value used in compare 
//  function. 
//  
//  The parameter level tells how deep the seeking process
//  will go in the symbol tree.
//
//  Returns a list of the symbols found.
//

SymbolList * TreeSearcher :: getAll (const void *k, int level) {

    if (k == 0) {
	return 0;
    }

    SymbolList *sl = new SymbolList;
    
    deep = level;

    if (root) {
	if (compareFunc (root, k)) {
	    sl->append (root);
	}
	    
	getAllChildren (root, k, sl);
    }

    return sl;
}


//
//Function: getChildren
//
//View: private
//
//Member-Of: TreeSearcher
//
//Parameters: Symbol *parent, 
//            const void *k
//
//Return: Symbol *
//
//Description: 
//  This function iterates recursively all children of
//  given parent. The parameter k is a compared value 
//  used in compare function. 
//  
//  Get function of this class uses this function to 
//  fulfill seeking recursion.
//  
//  Returns a symbol satisfies the compare function if
//  found.
//

Symbol * TreeSearcher :: getChildren (Symbol *parent, const void *k) {

    Symbol *childPtr = 0;

    currentLevel++;

    if (!isTooDeep ()) {
	if (parent) {
	    SymbolListIterator childIterator (&parent->_children);
	    
	    while (childPtr = childIterator.next ()) {
		
		if (compareFunc (childPtr, k)) {
		    break;
		}
	    }
	    
	    childIterator.reset ();
	    
	    while (childPtr == 0) {
		
		if ((parent = childIterator.next ()) == 0) {
		    break;
		}
		
		childPtr = getChildren (parent, k);
	    }
	}
    }    
    
    currentLevel--;

    return childPtr;
}


//
//Function: getAllChildren
//
//View: private
//
//Member-Of: TreeSearcher
//
//Parameters: Symbol *parent, 
//            const void *k,
//            SymbolList *sl
//
//Description: 
//  This function iterates recursively all children of
//  given parent. The parameter k is a compared value 
//  used in compare function. The parameter sl is
//  a symbol list for symbols satisfie the compare 
//  function.
//
//  GetAll function of this class uses this function to 
//  fulfill seeking recursion.
//  

void TreeSearcher :: getAllChildren (Symbol *parent, const void *k, SymbolList *sl) {

    Symbol *childPtr = 0;

    currentLevel++;

    if (isTooDeep ()) {
	return;
    }

    if (parent) {
	SymbolListIterator childIterator (&parent->_children);
    
	while (childPtr = childIterator.next ()) {
	    
	    if (compareFunc (childPtr, (const void *) k)) {
		sl->append (childPtr);
	    }
	}
	
	childIterator.reset ();
	
	while (childPtr == 0) {
	    
	    if ((parent = childIterator.next ()) == 0) {
		break;
	    }
	    
	    getAllChildren (parent, k, sl);
	}
    }

    currentLevel--;
}


//
//Function: isTooDeep
//
//View: private
//
//Member-Of: TreeSearcher
//
//Return: bool
//
//Description: 
//  Returns non-zero value if the current level in
//  seeking process is deeper than the level got
//  in get function.
//
//  If the deep level is not defined or it is less
//  or equal as  current level zero is returned.
//

bool TreeSearcher :: isTooDeep (void) {

    if (deep == -1) {
	//  means infinite level limiter
	return false;
    }

    if (currentLevel > deep) {
	return true;
    }

    return false;
}


//
//Constructor: SymbolChildIterator
//
//Parameters: Symbol *r
//
//Description: 
//  Creates a new child iterator and initializes it
//  with given value.
//
//  The parameter r is a parent who children will
//  be iterate.
//

SymbolChildIterator :: SymbolChildIterator (Symbol *r) 
: iterator ((r) ? &(r->_children) : 0) {

}


//
//Destructor: SymbolChildIterator
//
//Description: 
//  Deletes this iterator.
//

SymbolChildIterator :: ~SymbolChildIterator (void) {

}


//
//Function: next
//
//Member-Of: SymbolChildIterator
//
//Return: Symbol *
//
//Description: 
//  Returns next child of the parent symbol. If there are
//  no more children the zero is returned.
//
//  Note: The children list should not be updated while
//  iterating. If that is anyway necessary the iterator 
//  must be reseted after updating.
//  

Symbol * SymbolChildIterator :: next (void) {

    return iterator.next ();
}


//
//Function: reset
//
//Member-Of: SymbolChildIterator
//
//Parameters: Symbol *r
//
//Return: Symbol *
//
//Description: 
//  Resets the iterator to the given symbol. If the 
//  parameter r was not given the iterator goes
//  at the beginning of the children list.
//
//  Returns the symbol given as a parameter if 
//  it was found in the children list. Otherwise
//  returns zero.
//

Symbol * SymbolChildIterator :: reset (Symbol *r) {

    Symbol *s = 0;
    iterator.reset ();
    
    if (r) {
	while ((s = iterator.next ()) != r) {
	    if (s == 0) {
		break;
	    }
	}
    }

    return s;
}
