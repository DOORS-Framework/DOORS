//Editor-Info: -*- C++ -*-
//
// 

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#if HAVE_IPV6
#include <doors/inetaddr6.h>
#endif

#include <string>
#include <doors/lemonhand.h>
#include <doors/command.h>
#include <doors/dsched.h>
#include <doors/unixaddr.h>
#include <doors/inetaddr.h>

LEMonHandler :: LEMonHandler (Symbol *s, const char *tn, Type t) 
: SymbolHandler (s), _typename (tn), _option (0), _type (t), _funcFlags (0), 
  _current (0) {

    _MT_SYNCMONITOR;
      
}


LEMonHandler ::  ~LEMonHandler (void) {

    _MT_SYNCMONITOR;
    if (_option) {
      delete _option;
    }
}


void LEMonHandler :: initMenu (Uint8 code, const char *h) {

    _MT_SYNCMONITOR;
    _funcFlags = code;

    Command *parentOption = 0;
    if (Command :: top == 0)
      {
	return;
      }

    if (symbol ()->parent ()) {
	if (symbol ()->parent ()->handler (LEMON_HANDLER)) {
	    parentOption = ((LEMonHandler *) symbol ()->parent ()->handler (LEMON_HANDLER))->option ();
	} else {
	    parentOption = Menu :: System :: tmpObjects;
	}
    } else {
	if (symbol ()->isStatic ()) {
	    parentOption = Command :: top;
	} else {
	    return; 
	}
    }

    _option = new Menu :: TracedSymbol (symbol ()->name ().c_str(), 
					parentOption, 
					(h) ? h : "Traced object", this);

    new Menu :: Dot (_option);
    new Menu :: Help (_option);

    if (code & FF_trace) {
	new Menu :: System :: Trace (_option, "Sets tracing flags of tasks.");
    }
    if (code & FF_print) {
	new Menu :: System :: Print (_option, "Prints status of the selected object.");
    }
    if (code & FF_set) {
	new Menu :: System :: Set (_option, "Sets values of the selected object.");
    }
    if (code & FF_show) {
	new Menu :: System :: Show (_option, "Shows types of the selected object.");
    }
    if (code & FF_structure) {
	new Menu :: System :: Struct (_option, "Shows structure of selected object.");
    }

    Menu :: System :: tmpObjects->updateChildren (_option);
}


LEMonHandler *LEMonHandler :: currentSet (LEMonHandler *) {

    _MT_SYNCMONITOR;
    LEMonHandler *rc = 0;

    if (_current) {
	SymbolChildIterator iterator (symbol ());
	Symbol *ptr = 0;
	for (ptr = iterator.next (); ptr != _current && ptr; ptr = iterator.next ());
	_current = iterator.next ();
	if (_current == 0) {
	    if (symbol ()->parent ()) {
		rc = ((LEMonHandler *) symbol ()->parent ()->handler (LEMON_HANDLER))->currentSet (this);
	    } else {
		rc = 0;
	    }
	} else {
	    rc = this;
	}
    }
	
    return rc;
}


void LEMonHandler :: show (void) {

    _MT_SYNCMONITOR;
    if (_funcFlags & FF_show) {
	SymbolChildIterator iterator (symbol ());
	Symbol *current = 0;
	
	vout << Vostream::VAR << symbol ()->name () << "::= " << typeName ();
	if (iterator.next ()) {
	    iterator.reset ();
	    vout << " = {" << endv;
	    vout.levelIncrement ();
	    
	    while ((current = iterator.next ()) != 0) {
		((LEMonHandler *) current->handler (LEMON_HANDLER))->show ();
	    }
	    vout.levelDecrement (); 
	    vout << Vostream::VAR << "}";
	}
	vout << endv;
    }
}


void LEMonHandler :: structure (void) {

    _MT_SYNCMONITOR;
    if (_funcFlags & FF_structure) {
	SymbolChildIterator iterator (symbol ());
	Symbol *current = 0;
	
	vout << Vostream::VAR << typeName () << " '" << symbol ()->name () << "'";
	vout << " = {" << endv;
	vout.levelIncrement ();
	
	while ((current = iterator.next ()) != 0) {
	    ((LEMonHandler *) current->handler (LEMON_HANDLER))->structure ();
	}
	vout.levelDecrement ();
	vout << Vostream::VAR << "}" << endv;
    }
}


LEMonHandler *LEMonHandler :: set (WordList *words) {

    _MT_SYNCMONITOR;
    LEMonHandler *next = 0;

    if (_funcFlags & FF_set) {
	vout << Vostream::VAR << "asking " << symbol ()->name () << endv;

	SymbolChildIterator iterator (symbol ());
	
	if (_current) {
	    for (Symbol *ptr = iterator.next (); ptr != _current && ptr; 
		 ptr = iterator.next ());
	} else {
	    _current = iterator.next ();
	}

	while (_current) {
	    next = ((LEMonHandler *) _current->handler (LEMON_HANDLER))->set (words);
	    
	    if (next == this) {
		if (_current == 0) {
		    next = (LEMonHandler *) symbol ()->parent ()->handler (LEMON_HANDLER);
		    return next->currentSet (this);
		}
	    } else {
		return next;
	    }
	}
    } else {
	next = (LEMonHandler *) symbol ()->parent ()->handler (LEMON_HANDLER);
	next = next->currentSet (this);
    }

    return next;
}


void LEMonHandler :: print (void) {

    _MT_SYNCMONITOR;
    if (_funcFlags & FF_print) {
	SymbolChildIterator iterator (symbol ());
	Symbol *current = 0;
	
	vout << Vostream::VAR << typeName () 
	    << " '" << symbol ()->name () << "'";
	vout << " = {" << endv;
	vout.levelIncrement ();
	
	while ((current = iterator.next ()) != 0) {
	    ((LEMonHandler *) current->handler (LEMON_HANDLER))->print ();
	}
	vout.levelDecrement ();
	vout << Vostream::VAR << "}" << endv;
    }
}


SystemHandler :: SystemHandler (Symbol *s, const char *h) 
: LEMonHandler (s, "System", system), object ((Menu :: System *) s->object ()) {

    _MT_SYNCMONITOR;
    symbol()->setName ("system");
    initMenu (FF_structure | FF_show | FF_set | FF_print | FF_trace, h);
}

SystemHandler :: ~SystemHandler (void) {

    _MT_SYNCMONITOR;
    symbol()->setName ("");
}


EventTaskHandler :: EventTaskHandler (Symbol *s, const char *tn, const char *h) 
: LEMonHandler (s, tn, eventtask), object ((EventTask *) s->object ()) {

    _MT_SYNCMONITOR;
    initMenu (FF_structure | FF_show | FF_set | FF_print | FF_trace, h);
}


void EventTaskHandler :: update (void) {

    _MT_SYNCMONITOR;
    const char *moment;
    bool printTask = false;

    DebugScheduler *runner = (DebugScheduler *) object->getScheduler ();
    if (runner->beforeRun () && option ()->isTrace (TF_before)) {
        moment = "BEFORE";
        printTask = true;
    } else {
        if (option ()->isTrace (TF_after)) {
            moment = "AFTER";
            printTask = true;
        }
    }
    if (printTask) {
        vout << "***** " << symbol ()->name () << " " << moment << " run ";
        if (object->getIoHandler () && object->getTimerTask ()) {
            OTime tm = object->getTimerTask ()->getLocalTime ();
            vout << formatDate (tm) << endv;
        }
        print ();
    }
}

const char * EventTaskHandler :: formatDate (OTime &currentTime) {

    _MT_SYNCMONITOR;
    static char buf[80];
    time_t t = currentTime.getSeconds ();
    struct tm *ts = localtime (&t);
    sprintf (buf, "%2d:%02d:%02d.%06d", ts->tm_hour, ts->tm_min, ts->tm_sec,
             currentTime.getMicros ());
    return buf;
}

#if _MT_ENABLED

MTEventTaskHandler :: MTEventTaskHandler (Symbol *s, const char *tn, const char *h) 
: EventTaskHandler (s, tn, h), _beforeRun(true) {

    _MT_SYNCMONITOR;
}

void MTEventTaskHandler :: update (void) {

    _MT_SYNCMONITOR;
    const char *moment;
    bool printTask = false;

    if (_beforeRun && option ()->isTrace (TF_before)) {
        moment = "BEFORE";
        printTask = true;
	_beforeRun = false;
    } else {
        if (option ()->isTrace (TF_after)) {
            moment = "AFTER";
            printTask = true;
	    _beforeRun = true;	    
        }
    }
    if (printTask) {
        vout << "***** " << symbol ()->name () << " " << moment << " run ";
        if (object->getIoHandler () && object->getTimerTask ()) {
            OTime tm = object->getTimerTask ()->getLocalTime ();
            vout << formatDate (tm) << endv;
        }
        print ();
    }
}

#endif

PTaskHandler :: PTaskHandler (Symbol *s, const char *h) 
: ETaskHandler (s, "PTask", h) 
{
    _MT_SYNCMONITOR;
}


void PTaskHandler :: print (void) {

    _MT_SYNCMONITOR;
    SymbolChildIterator iterator (symbol ());
    Symbol *current = 0;
    
    vout << Vostream::VAR << typeName () << " '" << symbol ()->name () << "'";
    vout << " = {" << endv;
    vout.levelIncrement ();
	
    while ((current = iterator.next ()) != 0) {
	if (strcmp (current->name ().c_str(), "state")) {
	    ((LEMonHandler *) current->handler (LEMON_HANDLER))->print ();
	} else {
	    StateMachine *sm = ((PTask *) object)->getStateMachine ();
	    if (sm) {
		vout << Vostream::VAR << "state = " << sm->stateName ((PTask *) object) <<
		    " (" << ((PTask *) object)->getState () << ')' << endv;
	    }
	}
    }

    vout.levelDecrement ();
    vout << Vostream::VAR << "}" << endv;
}


DebugSchHandler :: DebugSchHandler (Symbol *s, const char *h) 
: LEMonHandler (s, "Scheduler", debugsch), object ((DebugScheduler *) s->object ()) {

    _MT_SYNCMONITOR;
    initMenu (FF_show | FF_set | FF_print, h);
}


Command *DebugSchHandler :: process (WordList *words) {

    _MT_SYNCMONITOR;
    Command *rc = 0;
    Name *word = words->remove ();

    // if value is supported process it
    if (word) {
	const char *str = word->c_str ();

	if (!strapprx ("?", str)) {
	    option ()->printSubHelp ();
	    vout.levelIncrement ();
	    vout << Vostream::INFO << "start" << endv;
	    vout.levelIncrement ();
	    vout << Vostream::INFO << "Restarts stopped scheduler." << endv;
	    vout.levelDecrement ();
	    vout << Vostream::INFO << "stop" << endv;
	    vout.levelIncrement ();
	    vout << Vostream::INFO << "Stops running scheduler." << endv;
	    vout.levelDecrement ();
	    vout << Vostream::INFO << "next" << endv;
	    vout.levelIncrement ();
	    vout << Vostream::INFO << "Runs stopped scheduler one turn." << endv << endv;
	    vout.levelDecrement ();
	    vout.levelDecrement ();
	    rc = option ();
	} else if (!strapprx ("start", str)) {
	    object->start ();
	    delete word;
	    rc = Command :: top;
	} else if (!strapprx ("stop", str)) { 
	    object->stop ();
	    delete word;
	    rc = Command :: top;
	} else if (!strapprx ("next", str)) {
	    object->step ();
	    delete word;
	    rc = Command :: top;
	} else {
	    words->insert (word);
	    rc = 0;
	}
    }

    return rc;
}


bool DebugSchHandler :: listOptions (void) {

    _MT_SYNCMONITOR;
    vout << "( start stop next ";

    TQueueListIterator<Command> iterator (option ()->subCommands ()->list ());
    Command *current = 0;
    
    while ((current = iterator.next ()) != 0) {
	vout << *current->name () << ' ';
    }
    vout << ')' << endv;

    return true;
}


LinkListHandler :: LinkListHandler (Symbol *s, const char *h) 
: LEMonHandler (s, "LinkList", linklist), object ((LinkList *) s->object ()) {

    _MT_SYNCMONITOR;
    initMenu (FF_print, h);
}


void LinkListHandler :: print (void) {

    _MT_SYNCMONITOR;
    LinkListIterator iterator (object);

    vout << Vostream::VAR << symbol ()->name () << ": " << iterator.length () << endv;
}


PortHandler :: PortHandler (Symbol *s, const char *n, const char *h) 
: LEMonHandler (s, n, LEMonHandler :: port), object ((Port *) s->object ()) {

    _MT_SYNCMONITOR;
    initMenu (FF_structure | FF_show, h);
}


void PortHandler :: structure (void) {

    _MT_SYNCMONITOR;
    vout << Vostream::VAR << typeName () << "'" << symbol ()->name () << "' -> "; 
    
    Port *port = object->getOther ();

    if (port ) {
	TreeSearcher searcher (symbolTable.getStaticTree ());
	Symbol *otherPort = searcher.get (port);
	Symbol *otherParent = (otherPort) ? otherPort->parent () : 0;

	vout << (otherParent ? otherParent->name () : "Unknown") << ":";
	vout << (otherPort ? otherPort->name () : "Unknown");
    } else {
	vout  << "Unconnected";
    }
    vout << endv;
}


void PortHandler :: show (void) {

    _MT_SYNCMONITOR;
    structure ();
}


TimerHandler :: TimerHandler (Symbol *s, const char *h) 
: LEMonHandler (s, "Timer", LEMonHandler :: timer), object ((Timer *) s->object ()) {

    _MT_SYNCMONITOR;
    initMenu (FF_structure | FF_show, h);
}


void TimerHandler :: show (void) {

    _MT_SYNCMONITOR;
    vout << Vostream::VAR << symbol ()->name() << "::= " << typeName () << endv;
}


LEMonHandler * TimerHandler :: set (WordList *words) {

    _MT_SYNCMONITOR;
    Name *word = words->remove ();
    
    // if value is supported process it
    if (word) {
	const char *str = word->c_str ();
	OTime time (atoi (str), 0);
	object->setInitial (time);
	delete word;
	return 0;
    }

    vout << Vostream::INFO << "( <seconds> )" << endv;
    return this;
}


void TimerHandler :: print (void) {

    _MT_SYNCMONITOR;
    vout << Vostream::VAR << "Timer '" << symbol ()->name () << "' = ";

    if (object->isTicking ()) {
	OTime t = object->delta ();
	vout << t << " / ";	
    }
    vout << object->getInitial () << endv;
}


IfaceHandler :: IfaceHandler (Symbol *s, const char *h) 
: PortHandler (s, "Iface", h), object ((Iface *) s->object ()) {

    _MT_SYNCMONITOR;
}


void IfaceHandler :: show (void) {

    _MT_SYNCMONITOR;
    PortHandler :: structure ();

    vout.levelIncrement (); 
    vout << Vostream::VAR << "( ";

    Message *msg = 0;
    for (int i = 1; (msg = object->create (i)) != 0; i++) {
	TreeSearcher searcher (symbolTable.getDynamicTree ());
	Symbol *symbol = searcher.get (msg);
	if (symbol) {
	    vout << symbol->name () << ' ';
	    DELETE_SYMBOL (symbol);
	}
	delete msg;
    }
    vout << Vostream::VAR << ")" << endv;
    vout.levelDecrement ();
}


LEMonHandler * IfaceHandler :: currentSet (LEMonHandler *c) {

    _MT_SYNCMONITOR;
    Message *msg = ((MessageHandler *) c)->message ();
    MOVE_SYMBOL (msg, 0);

    object->putMessage (msg);

    return 0;
}


void IfaceHandler :: deleteMessages (void) {
    
    _MT_SYNCMONITOR;
    TQueueListIterator<Command> commands (option ()->subCommands ()->list ());
    Command *cmd;
    
    while ((cmd = commands.next ()) != 0) {
	if (cmd->type () == Command :: tracedSymbol) {
	    delete ((Menu :: TracedSymbol *) cmd)->handler ()->symbol ();
	    delete cmd;
	    commands.reset ();
	}
    }
}


bool IfaceHandler :: listOptions (void) {

    _MT_SYNCMONITOR;
    Message *msg = 0;

    vout << "( ";
    for (int i = 1; (msg = object->create (i)) != 0; i++) {
	TreeSearcher searcher (symbolTable.getDynamicTree ());
	Symbol *sbl = searcher.get (msg);
	if (sbl) {
	    vout << sbl->name () << ' ';
	}
	delete sbl;
    }

    TQueueListIterator<Command> iterator (option ()->subCommands ()->list ());
    Command *current = 0;
    
    while ((current = iterator.next ()) != 0) {
	vout << *current->name () << ' ';
    }
    vout << ')' << endv;

    return true;
}


Command *IfaceHandler :: find (const Name *n) {

    _MT_SYNCMONITOR;
    Message *msg = 0;
    
    for (int i = 1; (msg = object->create (i)) != 0; i++) {
	TreeSearcher searcher (symbolTable.getDynamicTree ());
	Symbol *sbl = searcher.get (msg);
	if (sbl && strapprx (sbl->name ().c_str(), n->c_str ()) == 0) {
	    symbolTable.updateRelation (msg, object);

	    return ((LEMonHandler *) sbl->handler (LEMON_HANDLER))->option ();
	} else {
	    DELETE_SYMBOL (sbl);
	    delete msg;
	}
    }
    
    return 0;
}


MessageHandler :: MessageHandler (Symbol *s, const char *h) 
: LEMonHandler (s, "Message", LEMonHandler :: message), object ((Message *) s->object ()) {

    _MT_SYNCMONITOR;
    initMenu (FF_set | FF_print, h);
}


void MessageHandler :: print (void) {
    
    _MT_SYNCMONITOR;
    TreeSearcher searcher (symbolTable.getStaticTree ());
    Port *target = object->getTarget ();
    Symbol *targetSymbol = searcher.get (target); 
    Symbol *targetTaskSymbol = (targetSymbol) ? targetSymbol->parent () : 0;

    if (targetTaskSymbol && ((LEMonHandler *) targetTaskSymbol->handler (LEMON_HANDLER))->option ()->isTrace (TF_message)) { 
	vout << "*** Message from ";
	Port *sender = object->getSender ();
	if (sender) {
	    Symbol *senderSymbol = searcher.get (sender); 
	    Symbol *senderTaskSymbol = (senderSymbol) ? senderSymbol->parent () : 0;
	    vout << (senderTaskSymbol ? senderTaskSymbol->name () : "Unknown") << ":";
	    vout << (senderSymbol ? senderSymbol->name () : "Unknown");
	} else {
	    vout  << "Unknown source";
	}
    
	vout << " to ";
	vout << targetTaskSymbol->name () << ":";
	vout << targetSymbol->name ();

	vout << " ***" << endv;
	vout.levelIncrement ();
	LEMonHandler :: print ();
	vout.levelDecrement ();
    }
}

Vostream &operator << (Vostream &s, std::string &a) {

    s << (std::string) a;

    return s;
}


Vostream &operator << (Vostream &s, UnixAddr &a) {

    s << a.addrToString();

    return s;
}


Vostream &operator << (Vostream &s, InetAddr &a) {

    s << a.addrToString();

    return s;
}

#if HAVE_IPV6
Vostream &operator << (Vostream &s, InetAddr6 &a) {

    s << a.addrToString();

    return s;
}
#endif

#if _MT_ENABLED

Vostream &operator << (VostreamProxy &s, std::string &a) {

  Vostream &stream = s.getStream ();
  
  stream << (std::string)a;

  return stream;
}

Vostream &operator << (VostreamProxy &s, UnixAddr &a) {

  Vostream &stream = s.getStream ();
  
  stream << a;

  return stream;
}

Vostream &operator << (VostreamProxy &s, InetAddr &a) {

  Vostream &stream = s.getStream ();
  
  stream << a;

  return stream;
}

#if HAVE_IPV6
Vostream &operator << (VostreamProxy &s, InetAddr6 &a) {

  Vostream &stream = s.getStream ();
  
  stream << a;

  return stream;
}
#endif

#endif

void MemStruct :: init (MemoryStatistic *ms) {
    
    totalAllocAmount = ms->getTotalAllocAmount ();
    currentAllocAmount = ms->getCurrentAllocAmount ();
    totalAllocSize = ms->getTotalAllocSize ();
    meanAllocSize = ms->getMeanAllocSize ();
    largestAllocSize = ms->getLargestAllocSize ();
    statisticError = ms->isStatError ();
}


void BlockStruct :: init (BlockMemoryStatistic *ms) {

    StatBlockManager *sbm = (StatBlockManager *) ms;
    blocks = sbm->getAmount ();
    blockSize = sbm->getSize ();
    poolSize = blocks * blockSize;
    currentTotalAllocSize = ms->getCurrentTotalAllocSize ();
    largestTotalAllocSize = ms->getLargestTotalAllocSize ();
    wantedCurrentTotalAllocSize = ms->getWantedCurrentTotalAllocSize ();
    wantedLargestTotalAllocSize = ms->getWantedLargestTotalAllocSize ();
    wantedTotalAllocSize = ms->getWantedTotalAllocSize ();
    wantedMeanAllocSize = ms->getWantedMeanAllocSize ();
    wantedLargestAllocSize = ms->getWantedLargestAllocSize ();
    memoryError = ms->isMemError ();

    MemStruct :: init ((MemoryStatistic *) ms);
}


void BlockMemStruct :: init (StatBlockMemManager *sbm) {

    int i = 0;

    for (i = 0; i < sbm->getBlockManagerAmount (); i++) {
	BlockStruct *bs = new BlockStruct;
	managerList.insert (bs);
    }

    BlockStruct :: init ((BlockMemoryStatistic *) sbm);

    LinkListIterator mli (&sbm->managerList);
    TQueueListIterator<BlockStruct> bli (&managerList);
    StatBlockManager *mPtr = 0;
    BlockStruct *bPtr = 0;
    blocks = 0;
    blockSize = 0;
    poolSize = 0;

    while ((mPtr = (StatBlockManager *) mli.next ()) != 0) {
	bPtr = bli.next ();
	bPtr->init (mPtr); 
	blocks += mPtr->getAmount ();
	poolSize += mPtr->getAmount () * mPtr->getSize ();
	bPtr->fragmentation = mPtr->getFragmentation ();
    }
}


MMHandler :: MMHandler (Symbol *s, const char *t, const char *h) 
: LEMonHandler (s, t, LEMonHandler :: memoryManager),
  object ((MemoryManager *) s->object ()) {
    
    _MT_SYNCMONITOR;
    Name n ("tools");
    Command *cmd = Command :: top->subCommands ()->find (&n);

    if (cmd) {
	setOption (new Menu :: TracedSymbol (symbol ()->name ().c_str(), cmd, 
					     (h) ? h : "Traced object", this));
	
	new Menu :: Dot (option ());
	new Menu :: Help (option ());

	new Menu :: System :: Print (option (), "Prints statistical information of current memory usage.");
	new Menu :: System :: Show (option (), "Shows type of memory manager.");
    }

    s->removeParent ();
}


MMHandler :: ~MMHandler (void) {
  
    _MT_SYNCMONITOR;
    if (symbol ()) {
	symbol ()->removeHandler (this);
	delete symbol ();
    }
}


void MMHandler :: show (void) {

    _MT_SYNCMONITOR;
    vout << Vostream::VAR << symbol ()->name() << "::= " << typeName () << endv;
}


void MMHandler :: printMS (MemStruct *ms) {

    _MT_SYNCMONITOR;
    vout << Vostream::VAR << "Total no. of allocations   = " 
	<< ms->totalAllocAmount  << endv;

    vout << Vostream::VAR << "Unreleased allocations     = " 
	<< ms->currentAllocAmount  << endv;

    vout << Vostream::VAR << "Cumulative memory usage    = "
	<< ms->totalAllocSize << endv;

    vout << Vostream::VAR << "Average unit size          = "
	<< ms->meanAllocSize  << endv;

    vout << Vostream::VAR << "Largest unit size          = "
	<< ms->largestAllocSize  << endv;

    if (ms->statisticError) {
	vout << Vostream::VAR << "Statistical overflow occured"  << endv;
    }
}


StatPlainMMHandler :: StatPlainMMHandler (Symbol *s, const char *h) 
: MMHandler (s, "StatPlain", h) {

    _MT_SYNCMONITOR;
}


void StatPlainMMHandler :: print (void) {

    _MT_SYNCMONITOR;
    StatPlainMemManager *mm = (StatPlainMemManager *) object;

    MemStruct ms;
    ms.init ((MemoryStatistic *) mm);

    vout << Vostream::VAR << typeName () << " '" << symbol ()->name () << "' = {" << endv;
    vout.levelIncrement ();
    MMHandler :: printMS (&ms);
    vout.levelDecrement ();
    vout << Vostream::VAR << "}" << endv;
}


StatBlockHandler :: StatBlockHandler (Symbol *s, const char *t, const char *h) 
: MMHandler (s, t, h) {

    _MT_SYNCMONITOR;
}


void StatBlockHandler :: printBS (BlockStruct *bs) {

    _MT_SYNCMONITOR;
    vout << Vostream::VAR << "Memory pool size           = ";

    if (bs->blockSize != 0) { 
	vout << bs->blockSize << " x " << bs->blocks << " = " << bs->poolSize << endv;
    } else {
	vout << bs->poolSize << endv;
    }

    vout << Vostream::VAR << "Memory currently allocated = " 
	<< bs->currentTotalAllocSize << " (" << bs->wantedCurrentTotalAllocSize  
	    << ')' << endv;

    vout << Vostream::VAR << "Peak memory allocated      = " 
	<< bs->largestTotalAllocSize << " (" << bs->wantedLargestTotalAllocSize
	    << ')' << endv;

    vout << Vostream::VAR << "Cumulative memory usage    = "
	<< bs->totalAllocSize << " (" << bs->wantedTotalAllocSize
	    << ')' << endv;

    vout << Vostream::VAR << "Average unit size          = "
	<< bs->meanAllocSize << " (" << bs->wantedMeanAllocSize
	    << ')' << endv;

    vout << Vostream::VAR << "Largest unit size          = "
	<< bs->largestAllocSize << " (" << bs->wantedLargestAllocSize
	    << ')' << endv;
    
    vout << Vostream::VAR << "Total no. of allocations   = " 
	<< bs->totalAllocAmount  << endv;

    vout << Vostream::VAR << "Unreleased allocations     = " 
	<< bs->currentAllocAmount  << " of " << bs->blocks << endv;

    if (bs->memoryError) {
	vout << Vostream::VAR << "There were memory error(s)"  << endv;
    } 

    if (bs->statisticError) {
	vout << Vostream::VAR << "Statistical overflow occured"  << endv;
    }
}


StatBlockMHandler :: StatBlockMHandler (Symbol *s, const char *h) 
: StatBlockHandler (s, "StatBlock", h) { 

    _MT_SYNCMONITOR;
}


void StatBlockMHandler :: print (void) {

    _MT_SYNCMONITOR;
    StatBlockManager *bm = (StatBlockManager *) object;

    vout << Vostream::VAR << typeName () << " '" << symbol ()->name () << "' = {" << endv;
    vout.levelIncrement ();

    BlockStruct bs;
    bs.init (bm);
    bs.fragmentation = bm->getFragmentation ();

    StatBlockHandler :: printBS (&bs);
    vout << Vostream::VAR << "Fragmentation rate         = " << 
	bs.fragmentation << " %" << endv;

    vout.levelDecrement ();
    vout << Vostream::VAR << "}" << endv;
}


StatBlockMMHandler :: StatBlockMMHandler (Symbol *s, const char *h) 
: StatBlockHandler (s, "StatBlockMem", h) {

    _MT_SYNCMONITOR;
}


void StatBlockMMHandler :: print (void) {

    _MT_SYNCMONITOR;
    StatBlockMemManager *mm = (StatBlockMemManager *) object;

    BlockMemStruct bs;
    bs.init (mm);
    bs.blockManagerAmount = mm->getBlockManagerAmount ();

    vout << Vostream::VAR << typeName () << " '" << symbol ()->name () << "' = {" << endv;
    vout.levelIncrement ();
    vout << Vostream::VAR << "Memory blocks              = " << 
	bs.blockManagerAmount << endv;
    
    StatBlockHandler :: printBS (&bs);
    vout << endv;

    BlockStruct *current = 0;
    TQueueListIterator<BlockStruct> iterator (&bs.managerList);

    while((current = iterator.next ()) != 0) {
	vout << Vostream::VAR << "StatBlock = {" << endv;
	vout.levelIncrement ();

	StatBlockHandler :: printBS (current);
	vout << Vostream::VAR << "Fragmentation rate         = " << 
	current->fragmentation << " %" << endv;

	vout.levelDecrement ();
	vout << Vostream::VAR << "}" << endv;
    }

    vout.levelDecrement ();
    vout << Vostream::VAR << "}" << endv;
}

#ifdef __GNUG__
template class TQueue<BlockStruct>;
#endif

//
// g++ 2.7.2 template hack
//
/*
#ifdef __GNUG__
static void gnu_tmpl_ctor_inst_fix(void) 
{
    Symbol *s = 0;
    const char *o = 0;
    FundTypeHandler<InetAddr> a_inst(s,o);
    FundTypeHandler<InetAddr6> a_inst(s,o);
    FundTypeHandler<UnixAddr> b_inst(s,o);
    FundTypeHandler<char *> c_inst(s,o);
    FundTypeHandler<unsigned long> d_inst(s,o);
    FundTypeHandler<unsigned short> e_inst(s,o);
    FundTypeHandler<unsigned char> f_inst(s,o);
    FundTypeHandler<Frame> g_inst(s,o);
    FundTypeHandler<char *> h_inst(s,o);
    FundTypeHandler<Frame> i_inst(s,o);
    FundTypeHandler<bool> j_inst(s,o);
    FundTypeHandler<int> k_inst(s,o);
    FundTypeHandler<long> l_inst(s,o);
    FundTypeHandler<short> m_inst(s,o);
    FundTypeHandler<char> n_inst(s,o);
    FundTypeHandler<bool> o_inst(s,o);
    FundTypeHandler<std::string> q_inst (s,o);
    TQueue<BlockStruct> p_inst;
}
#endif

*/











