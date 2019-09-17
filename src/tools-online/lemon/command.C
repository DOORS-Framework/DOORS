//Editor-Info: -*- C++ -*-
//

#include <doors/tqueue.h>

#include <doors/uitask.h>
#include <doors/lemonhand.h>
#include <doors/command.h>
#include <doors/lemonhand.h>
#include <doors/core.h>
#include <doors/otl_record.h>
#include <string.h>


Command * Command :: top = new Command ("main menu", 0, 0);
Menu :: TracedSymbol * Menu :: System :: tmpObjects = new Menu :: TracedSymbol ("tmp", 0, 0, 0);
AliasList Command :: aliases;


//
//Function: strapprx
//
//Parameters: 
//       const char *s1, const char *s2
//
//Return: 
//      int
//
//Description: 
//      enchancement for strcmp to approximate if words are equal
//      currently checks if first string is longer and begins with 
//      second string.
//
int strapprx (const char *s1, const char *s2) {

    int len1 = strlen (s1);
    int len2 = strlen (s2);
    
    if (len1 >= len2) {
	return strncmp (s1, s2, len2);
    }
    
    return -1;
}


bool placeForTS (const Command *a, const Command *) {
    
    return (a->type () != Command :: tracedSymbol) ? true : false;
}


CommandList :: CommandList (void) {

}


CommandList :: ~CommandList (void) {

}


bool CommandList :: add (Command *c) 
{
    _DOORS_ERROR(c,"no command");

    if (c->type () == Command :: tracedSymbol) {
	_commands.add (c, placeForTS);	
    } else {
	_commands.insert (c);
    }

    return true;
}


void CommandList :: remove (Command *c) 
{
    _DOORS_ERROR(c,"no command");
    _commands.remove (c);
}


Command * CommandList :: find (const Name *n) {

    TQueueListIterator<Command> iterator (&_commands);
    Command *current = 0;

    while ((current = iterator.next ()) != 0) {       // try exact match
	if (current->isEqual (n)) {
	    return current;
	}
    } 
    
    iterator.reset ();
    
    while ((current = iterator.next ()) != 0) {   // try approximate match
	if (current->isApprx (n)) {
	    break;
	}
    }
    
    return current;
}

//////////////////////////////////////////////////////////////////////

Command :: Command (const char *n, Command *p, const char *h)
    : _name (n), 
      _parent (p), 
      _help (h) 
{
    if (_parent != 0) {
	_parent->subCommands()->add(this);
    }
}


Command :: ~Command (void) {

    if (_parent) {
	_parent->subCommands ()->remove (this);
    }

    if (this == Command :: top ) {
	aliases.removeAll();
    }
}


void Command :: listOptions (void) {

    TQueueListIterator<Command> iterator (_subCommands.list ());
    Command *current = 0;
    
    vout << "( ";
    while ((current = iterator.next ()) != 0) {
	if (!current->name ()->empty())
	    vout << *current->name () << ' ';
    }
    vout << ')' << endv;
}


Command * Command :: execute (WordList *words) 
{
    _DOORS_ERROR(words,"no arguments");

    WordListIterator iterator (words);
    Name *word = iterator.head ();
    
    WordList *replacement = aliases.get (word);

    if (replacement) {                            // alias case
	WordList reverseList;
	Name *tmp = 0;

	delete words->remove ();  // remove alias

	while ((tmp = replacement->remove ()) != 0) {
	    reverseList.insert (tmp);
	}
	
	while ((tmp = reverseList.remove ()) != 0) {
	    words->insert (tmp);
	}
	
	delete replacement;
    }

    Command *newLevel = process (words);
    newLevel->listOptions ();

    return newLevel;
}


Command * Command :: process (WordList *words) 
{
    _DOORS_ERROR(words,"no arguments");

    Name *word = words->remove ();

    if (word == 0) {
	return this;
    }

    Command *executor = find (word);
    
    if (executor == 0) {   // doesn't match any command
	words->insert (word);  // put unmatched word back to list
	return this;
    }

    delete word;

    return executor->process (words);
}


Command * Command :: find (const Name *n) {

    return _subCommands.find (n);
}


void Command :: printHelp (void) {

    std::string str = _name;

    vout << Vostream::INFO << str << endv;
    vout.levelIncrement ();
    vout << Vostream::INFO << _help << endv;
    vout.levelDecrement ();
}


void Command :: printSubHelp (void) {

    TQueueListIterator<Command> iterator (_subCommands.list ());
    Command *current = 0;
    std::string str = _name;

    vout << Vostream::INFO << str << endv;
    vout.levelIncrement ();

    while ((current = iterator.next ()) != 0) {
	current->printHelp ();
    }

    vout.levelDecrement ();
}


void Command :: setParent (Command *c) {

    _parent->subCommands ()->remove (this);
    c->subCommands ()->add (this);
    _parent = c;
}


Menu :: Tool :: Tool (Command *p, const char *h) : Command ("tools", p, h) {

}


Menu :: Tool :: ~Tool (void) {

}


Menu :: Tool :: Log :: Log (Command *p, const char *h) : Command ("log", p, h) {

}


Menu :: Tool :: Log :: ~Log (void) {

}


Menu :: Tool :: Log :: Open :: Open (UiTask *t, Command *p, const char *h) 
: Command ("open", p, h), ui (t) {

}


Menu :: Tool :: Log :: Open :: ~Open (void) {

}


Command * Menu :: Tool :: Log :: Open :: process (WordList *words) 
{
    _DOORS_ERROR(words,"no arguments");

    Name *word= words->remove ();

    if (word == 0) {
	vout << Vostream::INFO << "( <filename> . )" << endv;
	return this;
    }
    
    const char* str = word->c_str();

    if (strcmp(str, ".")) {
	new File (ui->getScheduler (), str, _parent, "Opened log file");
    }

    delete word;

    return top;
}


Menu :: Tool :: Log :: Cont :: Cont (Command *p, const char *h) : Command ("cont", p, h) {

}


Menu :: Tool :: Log :: Cont :: ~Cont (void) {

}


Command * Menu :: Tool :: Log :: Cont :: process (WordList *words) {

    ((File *) _parent)->cont ();
    
    return top;
}


Menu :: Tool :: Log :: Hold :: Hold (Command *p, const char *h) : Command ("hold", p, h) {

}


Menu :: Tool :: Log :: Hold :: ~Hold (void) {

}


Command * Menu :: Tool :: Log :: Hold :: process (WordList *words) {

    ((File *) _parent)->hold ();
    
    return top;
}


Menu :: Tool :: Log :: Close :: Close (Command *p, const char *h) 
: Command ("close", p, h) {

}


Menu :: Tool :: Log :: Close :: ~Close (void) {

}


Command * Menu :: Tool :: Log :: Close :: process (WordList *words) {

    delete _parent;  // CAUTION ! This deletes also this object, so let's
                     //           get out right now !
 
    return top;
}


Menu :: Tool :: Log :: File :: File (Scheduler *s, const char *n, Command *p, const char *h) 
: Command (n, p, h), stopped (false) {

    logger = new FileLogger (s, s->getIoHandler (), _name);

    // attach logger to output stream
    vout.putOutput (logger->getPort ());

    new Dot (this);
    new Help (this);
    new Close (this, "Ends logging to the file");
    new Cont (this, "Continues writing to the log file after it is stopped");
    new Hold (this, "Temporarily stops writing to the log file");
}


Menu :: Tool :: Log :: File :: ~File (void) {

    // deattach logger from output stream
    vout.getOutput (logger->getPort ());

    if (logger) {
	delete logger;
    }
}


void Menu :: Tool :: Log :: File :: hold (void) {

    vout << Vostream::INFO << "logHold" << endv;

    if (!stopped) {
	vout.getOutput (logger->getPort ());
	stopped = true;
    }
} 


void Menu :: Tool :: Log :: File :: cont (void) {

    vout << Vostream::INFO << "logCont" << endv;

    if (stopped) {
	vout.putOutput (logger->getPort ());
	stopped = true;
    }
} 


Menu :: Tool :: Macro :: Macro (UiTask *t, Command *p, const char *h) 
: Command ("macro", p, h), recording (false), ui (t) {

}


Menu :: Tool :: Macro :: ~Macro (void) {
    
}


void Menu :: Tool :: Macro :: record (const char *n) {

    ui->macroStart (n);
}


void Menu :: Tool :: Macro :: unrecord (void) {

    ui->macroEnd ();
}


bool Menu :: Tool :: Macro :: isRecording (void) {

    return recording;
}


void Menu :: Tool :: Macro :: run (const char *n) {

    ui->addInput (n);
}


Menu :: Tool :: Macro :: Make :: Make (Command *p, const char *h) 
: Command ("make", p, h) {

}


Menu :: Tool :: Macro :: Make :: ~Make (void) {

}


Command * Menu :: Tool :: Macro :: Make :: process (WordList *words) 
{
    _DOORS_ERROR(words,"no arguments");

    Name *word = words->remove ();

    if (!word) {
	vout << Vostream::INFO << "( <macroname> . )" << endv;
	return this;
    }
    
    const char *str = word->c_str();    

    if (str && strcmp (str, ".")) {
	new End (_parent, "Ends recording of macro.");
	((Macro *) _parent)->record (str);
	delete this;    // remove 'make' option from the menu
    }

    delete word;

    return top;
}


Menu :: Tool :: Macro :: End :: End (Command *p, const char *h) : Command ("end", p, h) {

}


Menu :: Tool :: Macro :: End :: ~End (void) {

}


Command * Menu :: Tool :: Macro :: End :: process (WordList *words) {

    ((Macro *) _parent)->unrecord ();

    new Make (_parent, "Begins recording of macro.");
    delete this;    // remove 'end' option from the menu

    return top;
}


Menu :: Tool :: Macro :: Run :: Run (Command *p, const char *h) 
: Command ("run", p, h) {

}


Menu :: Tool :: Macro :: Run :: ~Run (void) {

}


Command * Menu :: Tool :: Macro :: Run :: process (WordList *words) {

    Name *word= words->remove ();

    if (word == 0) {
	vout << "( macroname . )" << endv;
	return this;
    }
    
    const char *str = word->c_str();    

    if (strcmp (str, ".")) {
	((Macro *) _parent)->run (str);
    }

    delete word;

    return top;
}


Menu :: TracedSymbol :: TracedSymbol (const char *n, Command *p, const char *h, LEMonHandler *t) 
: Command (n, p, h), traceFlags (~0), _handler (t) {

}


Menu :: TracedSymbol :: ~TracedSymbol (void) {

  if (_handler) {
    _handler->setOption (0);
  }
}


void Menu :: TracedSymbol :: updateChildren (Menu :: TracedSymbol *c) {

    TQueueListIterator<Command> iterator (_subCommands.list ());
    Command *current = 0;
    Symbol *candidate = c->handler ()->symbol ();

    while ((current = iterator.next ()) != 0) {
	if (current->type () == tracedSymbol) {
	    Menu :: TracedSymbol *s = (Menu :: TracedSymbol *) current;
	    if (s->handler ()->symbol ()->isMyParent (candidate)) {
		current->setParent (c);
		iterator.reset ();
	    }
	}
    }
}


void Menu :: TracedSymbol :: setTrace (Uint16 c) {

    TQueueListIterator<Command> iterator (_subCommands.list ());
    Command *current = 0;

    while ((current = iterator.next ()) != 0) {
	if (current->type () == tracedSymbol) {
	    ((Menu :: TracedSymbol *) current)->setTrace (c);
	}
    }

    traceFlags = c;
}


void Menu :: TracedSymbol :: addTrace (Uint16 c) {

    TQueueListIterator<Command> iterator (_subCommands.list ());
    Command *current = 0;

    while ((current = iterator.next ()) != 0) {
	if (current->type () == tracedSymbol) {
	    ((Menu :: TracedSymbol *) current)->addTrace (c);
	}
    }
    traceFlags ^= c;
}


bool Menu :: TracedSymbol :: isTrace (Uint16 c) {

    return (c & traceFlags) ? true: false;
}


void Menu :: TracedSymbol :: printTraceFlags (void) {

    if (traceFlags & TF_before) {
	vout << "Before ";
    }
    if (traceFlags & TF_message) {
	vout << "Message ";
    }
    if (traceFlags & TF_after) {
	vout << "After ";
    }
}    


Command * Menu :: TracedSymbol :: find (const Name *n) {

    Command *rc = (_handler) ? _handler->find (n) : 0;

    if (rc == 0) {
	rc =  Command :: find (n);
    }

    return rc;
}


void Menu :: TracedSymbol :: show (void) {

    if (_handler) {
	_handler->show ();
    }
}


Command *Menu :: TracedSymbol :: set (WordList *words) {

    LEMonHandler *rc = 0;

    if (_handler) {
	rc = _handler->set (words);
    }
    
    return rc ? rc->option () : top;
}


void Menu :: TracedSymbol :: print (void) {

    if (_handler) {
	_handler->print ();
    }
}


void Menu :: TracedSymbol :: structure (void) {

    if (_handler) {
	_handler->structure ();
    }
}


void Menu :: TracedSymbol :: listOptions (void) {

    if (_handler && _handler->listOptions ()) {
	return;
    }

    Command :: listOptions ();
}


Command * Menu :: TracedSymbol :: process (WordList *words) {

    Command *rc = 0;

    if (_handler) {
	if ((rc = _handler->process (words)) == 0) {
	    rc = Command :: process (words);
	}
    }

    return rc;
}


Menu :: System :: System (Command *p, const char *h, LEMonHandler *t) 
: TracedSymbol ("system", p, h, t) {

}


Menu :: System :: ~System (void) {

}


Menu :: System :: Object :: Object (Command *p, const char *h, LEMonHandler *t) 
: TracedSymbol (t->symbol ()->name ().c_str(), p, h, t) {

}


Menu :: System :: Struct :: Struct (Command *p, const char *h) 
: Command ("struct", p, h) {

}


Menu :: System :: Struct :: ~Struct (void) {

}


Command * Menu :: System :: Struct :: process (WordList *words) {

    ((TracedSymbol *) _parent)->structure ();
    
    return top;
}


Menu :: System :: Show :: Show (Command *p, const char *h) 
: Command ("show", p, h) {

}


Menu :: System :: Show :: ~Show (void) {

}


Command * Menu :: System :: Show :: process (WordList *words) {

    ((TracedSymbol *) _parent)->show ();
    
    return top;
}


Menu :: System :: Set :: Set (Command *p, const char *h) 
: Command ("set", p, h) {

}


Menu :: System :: Set :: ~Set (void) {

}


Command * Menu :: System :: Set :: process (WordList *words) {

    Command *rc = ((TracedSymbol *) _parent)->set (words);

    if (rc != top) {
	Name n ("set");
	rc = rc->subCommands ()->find (&n);	
    }

    return rc;
}


Menu :: System :: Print :: Print (Command *p, const char *h) 
: Command ("print", p, h) {

}


Menu :: System :: Print :: ~Print (void) {

}


Command * Menu :: System :: Print :: process (WordList *words) {

    ((TracedSymbol *) _parent)->print ();
    
    return top;
}


Menu :: System :: Trace :: Trace (Command *p, const char *h) 
: Command ("trace", p, h) {

}


Menu :: System :: Trace :: ~Trace (void) {

}


Command * Menu :: System :: Trace :: process (WordList *words) 
{
    _DOORS_ERROR(words,"no arguments");
    
    Name *word = words->remove ();
    bool inform = true;

    // if value is supported process it
    if (word) {
	const char *str = word->c_str();

	if (!strcmp (str, ".")) { 	// cancel
	    // do nothing
	    inform = false;
	} else if (!strcmp (str, "?")) { 	// help
	    vout << Vostream::INFO << "trace" << endv;
	    vout.levelIncrement ();
	    vout << Vostream::INFO << _help << endv;
	    printHelp ();
	    vout.levelDecrement ();
	    vout << endv;
	    inform = false;
	} else if (!strapprx ("all", str)) { 	// all
	    ((TracedSymbol *) _parent)->setTrace ((Uint8) ~0);
	} else if (!strapprx ("off", str)) { 	// off
	    ((TracedSymbol *) _parent)->setTrace ((Uint8) 0);
	} else if (!strapprx ("show", str)) {
	    // do nothing
	} else if (!strapprx ("before", str)) {
	    ((TracedSymbol *) _parent)->addTrace (TF_before);
	} else if (!strapprx ("message", str)) {
	    ((TracedSymbol *) _parent)->addTrace (TF_message);
	} else if (!strapprx ("after", str)) {
	    ((TracedSymbol *) _parent)->addTrace (TF_after);
	}

	if (inform) {
	    vout << Vostream::VAR << "Trace: ";
	    ((TracedSymbol *) _parent)->printTraceFlags ();
	    vout << endv;
	}

    } else {
	// otherwise set up for asking
	vout << Vostream::INFO << "( all off before message after show ? . )" << endv;
	return this;
    }
    
    return top;   
}


void Menu :: System :: Trace :: printHelp (void) {

    vout << Vostream::INFO << "all" << endv;
    vout.levelIncrement ();
    vout << Vostream::INFO << "Sets full tracing" << endv;
    vout.levelDecrement ();
    vout << Vostream::INFO << "off" << endv;
    vout.levelIncrement ();
    vout << Vostream::INFO << "Sets tracing off" << endv;
    vout.levelDecrement ();
    vout << Vostream::INFO << "before" << endv;
    vout.levelIncrement ();
    vout << Vostream::INFO << "Switch tracing printed before task's execution" << endv;
    vout.levelDecrement ();
    vout << Vostream::INFO << "message" << endv;
    vout.levelIncrement ();
    vout << Vostream::INFO << "Switch message tracing" << endv;
    vout.levelDecrement ();
    vout << Vostream::INFO << "after" << endv;
    vout.levelIncrement ();
    vout << Vostream::INFO << "Switch tracing printed after task's execution" << endv;
    vout.levelDecrement ();
    vout << Vostream::INFO << "show" << endv;
    vout.levelIncrement ();
    vout << Vostream::INFO << "Shows current status of tracing" << endv;
    vout.levelDecrement ();
}


Menu :: Exit :: Exit (UiTask *t, Command *p, const char *h) 
: Command ("Exit", p, h), ui (t) {
    
}


Menu :: Exit :: ~Exit (void) {

}


Command * Menu :: Exit :: process (WordList *words) {

    ui->terminate ();  // I'll be back !

    return top;
}


Menu :: Alias :: Alias (Command *p, const char *h) : Command ("alias", p, h) {

}


Command * Menu :: Alias :: process (WordList *words) {

    Name *word = words->remove ();

    if (word == 0) {  // print aliases
	aliases.print ();
	vout << endv;

    } else {          // set new alias
	aliases.add (word, words);
    }
  
    delete word;
    return top;
}


Menu :: Unalias :: Unalias (Command *p, const char *h) : Command ("unalias", p, h) {

}


Menu :: Unalias :: ~Unalias (void) {

}


Command * Menu :: Unalias :: process (WordList *words) {

    Name *n = words->remove ();
    aliases.remove (n);
    delete n;
    return top;
}


Menu :: Shell :: Shell (Command *p, const char *h) : Command ("shell", p, h) {

}


Menu :: Shell :: ~Shell (void) {

}


Command * Menu :: Shell :: process (WordList *words) {

    char buffer [256];
    buffer [0] = '\0';
    int len = 0;
    Name *tmp = 0;
    while (tmp = words->remove ())  {
	len += strlen (tmp->c_str()) + 1;
	if (len > 256) {
	    break;
	}
	strcat (buffer, tmp->c_str());
	strcat (buffer, " ");
        delete tmp;
    }
    system (buffer);
    vout << endv;
    
    return top;
}


Menu :: Help :: Help (Command *p, const char *h) 
: Command ("?", p, h ? h : "Prints help for this command level.") {

}


Menu :: Help :: ~Help (void) {

}


Command * Menu :: Help :: process (WordList *words) {

    if (_parent) {
	_parent->printSubHelp ();
	vout << endv;
	return _parent;
    }
    return top;
}


Menu :: Dot :: Dot (Command *p, const char *h) 
: Command (".", p, h ? h : "Returns to main menu") {

}


Menu :: Dot :: ~Dot (void) {

}


Command * Menu :: Dot :: process (WordList *words) {

    return top;
}


