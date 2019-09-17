//Editor-Info: -*- C++ -*-
// 


#include <doors/alias.h>
#include <doors/core.h>
#include <doors/debug.h>
#include <string.h>

AliasList :: AliasList (void) {}

AliasList :: ~AliasList (void) {}

WordList * AliasList :: get (const Name *n) {

    if (n == 0) {
	return 0;
    }

    TQueueListIterator<Alias> iterator (&aliases);
    Alias *current = 0;

    while ((current = iterator.next ()) != 0) {
	if (*current == n) {
	    break;
	}
    }

    return (current) ? current->getReplacement () : 0;
}


bool AliasList :: add (Name *n, WordList *rpl) 
{
    _DOORS_FAULT(n && rpl,"funny things don't happen now");

    if (get (n)) {    // this alias exists already 
	delete new Alias (n, rpl);    // easy way to delete n and rpl
	return false;
    }
    
    Alias *a = new Alias (n, rpl);
    aliases.append (a);

    return true;
}


bool AliasList :: remove (const Name *n) {

    TQueueListIterator<Alias> iterator (&aliases);
    Alias *current = 0, *synonym = 0;

    while ((current = iterator.next ()) != 0) {
	if (*current == n) {
	    synonym = aliases.remove (current);
	    delete synonym;
    	    return true;
	}
    }

    return false;
}

void AliasList :: removeAll (void) {

    TQueueListIterator<Alias> iterator (&aliases);
    Alias *current = 0;

    while ((current = iterator.next ()) != 0) {
	delete aliases.remove (current);
	iterator.reset ();
    }
}


void AliasList :: print (void) const {

    TQueueListIterator<Alias> iterator (&aliases);
    Alias *current = 0;
    
    vout << Vostream::INFO << "Aliases:" << endv;
    vout.levelIncrement ();

    while ((current = iterator.next ()) != 0) {
	current->print ();
    }

    vout.levelDecrement ();
}


Alias :: Alias (Name *a, WordList *rpl) 
{
    _DOORS_FAULT(a && rpl,"no name and no arguments");

    alias = *a;
    Name *ptr = 0;

    while ((ptr = rpl->remove ()) != 0) {
	replacement.append (ptr);
    }
}


Alias :: ~Alias (void) {

}

    
void Alias :: print (void) const {

    vout << Vostream::INFO << "'" << alias << "' = '";

    WordListIterator nameIterator(&replacement);
    Name *node = nameIterator.next ();

    if (node) { 
	vout << *node;
    }

    while (node = nameIterator.next ()) {
	vout << " " << *node;
    }

    vout << "'" << endv;
}


Name * Alias :: getName (void) {

    return &alias;
}

WordList * Alias :: getReplacement (void) {

  // HPUX hack
    WordList *list;
    list = new WordList;

    WordListIterator nameIterator(&replacement);
    Name *node = 0;

    while (node = nameIterator.next ()) {
	list->append (new Name (*node));
    }

    return list;
}

bool Alias :: operator == (const Name *n) 
{
    return (n == 0 || (*n != alias)) ? false : true;
}

bool Alias :: operator == (Alias &a) 
{
    return (*a.getName () != alias) ? false : true;
}
