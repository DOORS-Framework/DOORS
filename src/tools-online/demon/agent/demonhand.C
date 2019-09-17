//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
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
//
//  Subject: DEMON handler methods
//  Author:  Ilkka Karvinen, TTKK/DMI

/*
$Log: demonhand.C,v $
Revision 1.17  2003/01/03 17:10:10  ik
Simpler approach to indexing symbols. Temporary hack.

Revision 1.16  2002/12/30 14:42:27  ik
Support for index in symbols.

Revision 1.15  2002/12/30 13:12:57  ik
Time changed to absolute.

Revision 1.14  2002/12/04 16:16:29  ik
Timer starts from zero.

Revision 1.13  2002/11/25 16:52:48  ik
Added overloaded isEnabled() for MessageHandler.

Revision 1.12  2002/11/22 17:17:41  ik
Filter changes. setEnabled(), setEnabledRecursively(), setFilter()
functions added.

Revision 1.11  2002/11/11 16:27:56  ik
Bug fixes. Added lines for sending whole system symbol.

Revision 1.10  2002/10/30 17:34:13  ik
Changed to s6_addr due to porting issues.

Revision 1.9  2002/10/30 17:06:41  ik
IPv6 address mapped.

Revision 1.8  2002/10/30 15:51:24  ik
Added dummy lines to map IPv6 address in InetAddr6.

Revision 1.7  2002/08/14 09:33:38  bilhanan
ONS_ macros are now renamed. Refer to top-level ChangeLog for details.

Revision 1.6  2002/08/09 11:46:32  ik
Handler for InetAddr6.

Revision 1.5  2002/08/09 09:32:40  ik
Added c_str()'s to std::string type variables because Mico CORBA
doesn't have operator= from std::string to CORBA::String.

Revision 1.4  2002/07/23 14:46:56  ik
Od{Boolean,False,True} -> {bool,false,true}

Revision 1.3  2002/05/24 09:58:26  ik
EventTask symbol value.

Revision 1.2  2002/05/24 08:04:42  ik
Development

Revision 1.1.1.1  2002/05/13 14:54:40  bilhanan
Restructured

Revision 1.3  2002/05/10 10:49:17  ik
Text UI.

Revision 1.2  2002/05/03 13:42:48  ik
Memory handlers made conditional according to configuration.

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.8  2002/02/25 17:08:34  ik
Changes due to UnixAddr/InetAddr structure change.

Revision 1.7  2002/01/22 11:45:57  ik
update() moved to base class because it is always the same.

Revision 1.6  2002/01/21 17:19:01  ik
Changes to remove CORBA::Any from demon.idl.

Revision 1.5  2002/01/14 17:37:51  ik
Changes to make two-direction CORBA calls to work.

Revision 1.4  2001/12/13 10:20:42  ik
Manager moved to root of demon directory due to autoconf structure
problem.

Revision 1.3  2001/12/11 15:56:46  ik
Added Makefile.am for demon. Structure changes.

Revision 1.2  2001/12/11 15:21:00  ik
Added licence for initial demon files. Directory structure changed
due to problems in autoconf.

*/

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <string>
#include "agenttask.h"
#include <doors/symbol.h>
#include <doors/demonhand.h>
#include <doors/sm.h>
#include <doors/llist.h>
#include <doors/inetaddr.h>
#include <doors/inetaddr6.h>

unsigned long DemonHand::indexpool_ = 1;

DemonHand::DemonHand (Symbol *s, const char *tn, Demon::SymbolType t) 
  : SymbolHandler (s), typename_ (tn), type_ (t), enabled_(true),
    index_(indexpool_++)
{
}


DemonHand::~DemonHand (void) 
{
}

Demon::Symbol
DemonHand::createStateSymbol(std::string n, CORBA::ULong number)
{
  Demon::Symbol sym;
  sym.name = n.c_str();
  sym.value.state(number);
  sym.index = getIndex();

  return sym;
}

// Recursively set enabled flag on this symbol and all of its children
// to state given in 'enabled' flag.
void
DemonHand::setEnabledRecursive (bool enabled)
{
  setEnabled(enabled);
  std::cout << "Setting '" << symbol()->name() << "' is " << enabled << std::endl;

  SymbolChildIterator iterator (symbol ());
  Symbol *current = 0;
  // Recurse to children
  while ((current = iterator.next ()) != 0) {
    DemonHand* handler = (DemonHand *) current->handler (DEMON_HANDLER);
    if (handler) {
      handler->setEnabledRecursive(enabled);
    }
  }
}

// Update filter on this symbol and all of its children
bool
DemonHand::setFilter (Demon::Filter& filter)
{
  Demon::Symbol sym = getSymbol();

  if (sym.index != filter.index) {
    return false;
  }

  std::cout << "Symbol name = " << sym.name << ", state = " << filter.enabled << std::endl;

  // This filter is made for this symbol, so set enabled flag
  setEnabled(filter.enabled);

  // If there are children for this filter, try them on the children
  // symbols
  int len = filter.children.length();
  if (len > 0) {
    SymbolChildIterator iterator (symbol ());
    Symbol *current = 0;
    int index = 0;
    // Run on the children
    while ((current = iterator.next ()) != 0) {
      for (int i = 0; i < len; i++) {
	// Try with this child and if it fits, true is returned and
	// we break the loop since for every symbol there is only one
	// filter item.
	DemonHand* handler = (DemonHand *) current->handler (DEMON_HANDLER);
	if (handler && handler->setFilter(filter.children[i])) {
	  break;
	}
      }
    }
  }

  return true;
}


void 
DemonHand::update (void)
{
  if (isEnabled()) {
    struct timeval tv;
    gettimeofday(&tv, 0);
    Demon::TimeStamp time;
    time.seconds = tv.tv_sec;
    time.useconds = tv.tv_usec;
    AgentTask::Instance()->sendSymbol(time, getSymbol());
  }
}

// Handlers

DemonSystemHandler::DemonSystemHandler (Symbol* s, const char* n)
  : DemonHand(s, n, Demon::stSystem)
{
}

void 
DemonSystemHandler::update (void)
{
  std::cout << "DemonSystemHandler::update() called." << std::endl;
  AgentTask::Instance()->setSystem(getSymbol());
}

Demon::Symbol
DemonSystemHandler::getSymbol (void)
{
  Demon::Symbol sym;
  sym.value.system(0);
  sym.name = "system";
  sym.index = getIndex();

  // Recurse to children
  SymbolChildIterator iterator (symbol ());
  Symbol *current = 0;
  int index = 0;
  sym.children.length(100);
  while ((current = iterator.next ()) != 0) {
    // Retrieve symbol
    sym.children[index++] = 
      ((DemonHand *) current->handler (DEMON_HANDLER))->getSymbol ();
  }
  sym.children.length(index);

  return sym;
}

DemonLinkListHandler::DemonLinkListHandler (Symbol *s, const char *n)
  : DemonHand (s, n, Demon::stLinkList), object_ ((LinkList *) s->object ()) 
{
}

Demon::Symbol
DemonLinkListHandler::getSymbol (void)
{
  Demon::Symbol tmp;

  LinkListIterator iterator (object_);
  tmp.value.linklist(iterator.length());
  tmp.name = symbol ()->name().c_str();
  tmp.index = getIndex();

  return tmp;
}

DemonEventTaskHandler::DemonEventTaskHandler (Symbol *s, const char *n, 
					      Demon::SymbolType t)
  : DemonHand (s, n, t), object_ ((EventTask *) s->object ()) 
{
}

Demon::Symbol
DemonEventTaskHandler::getSymbol (void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();

  Scheduler *runner = (Scheduler *) object_->getScheduler ();
  if (runner->beforeRun()) {
    sym.value.etaskbefore(0);
  } else {
    sym.value.etaskafter(0);
  }

  SymbolChildIterator iterator (symbol ());
  Symbol *current = 0;
  int index = 0;

  sym.children.length(100);
  while ((current = iterator.next ()) != 0) {
    // Retrieve symbol
    sym.children[index++] = 
      ((DemonHand *) current->handler (DEMON_HANDLER))->getSymbol ();
  }
  sym.children.length(index);

  return sym;
}

DemonPTaskHandler::DemonPTaskHandler (Symbol *s, const char *n)
  : DemonEventTaskHandler (s, n, Demon::stPTask), 
    object_ ((PTask *) s->object ()) 
{
}

// Symbol 
//    name = PTask
//    value = Demon::Task
//
Demon::Symbol
DemonPTaskHandler::getSymbol (void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();

  SymbolChildIterator iterator (symbol ());
  Symbol *current = 0;
  int index = 0;

  sym.children.length(100);
  while ((current = iterator.next ()) != 0) {
    if (std::string(current->name()) != std::string("state")) {
      // Retrieve symbol
      sym.children[index++] = 
	((DemonHand *) current->handler (DEMON_HANDLER))->getSymbol ();
    } else {
      StateMachine *sm = ((PTask *) object_)->getStateMachine ();
      if (sm) {
	sym.children[index++] = 
	  createStateSymbol(sm->stateName ((PTask *) object_),
			    (CORBA::Long)((PTask *) object_)->getState ());
      }
    }
  }
  sym.children.length(index);

  sym.value.ptask(0); // Flags not used

  return sym;
}

DemonPortHandler::DemonPortHandler (Symbol *s, const char *n,
				    Demon::SymbolType t)
  : DemonHand (s, n, t), object_ ((Port *) s->object ()) 
{
}

Demon::Symbol
DemonPortHandler::getSymbol (void)
{
  Demon::Symbol sym;
  sym.name = symbol()->name ().c_str();
  sym.index = getIndex();

  Port *port = object_->getOther ();

  Demon::Port demonport;
  if (port ) {
    TreeSearcher searcher (symbolTable.getStaticTree ());
    Symbol *otherPort = searcher.get (port);
    Symbol *otherParent = (otherPort) ? otherPort->parent () : 0;
    
    demonport.parentname = (otherParent ? otherParent->name ().c_str() : "Unknown");
    demonport.otherport = (otherPort ? otherPort->name ().c_str() : "Unknown");
  } else {
    demonport.parentname = "Unconnected";
    demonport.otherport = "Unconnected";
  }
  
  sym.value.port(demonport);

  return sym;
}

DemonTimerHandler::DemonTimerHandler (Symbol *s, const char *n)
  : DemonHand (s, n, Demon::stTimer), object_ ((Timer *) s->object ()) 
{
}

Demon::Symbol
DemonTimerHandler::getSymbol (void)
{
  Demon::Symbol sym;
  Demon::Timer timer;
  OTime t;
  if (object_->isTicking()) {
    timer.is_ticking = true;
  }
  t = object_->delta();
  timer.delta.seconds = t.getSeconds();
  timer.delta.useconds = t.getMicros();
  t = object_->getInitial();
  timer.initial.seconds = t.getSeconds();
  timer.initial.useconds = t.getMicros();
  sym.name = CORBA::string_dup(symbol ()->name ().c_str());
  sym.index = getIndex();
  sym.value.timer(timer);

  return sym;
}

DemonIfaceHandler::DemonIfaceHandler (Symbol *s, const char *n)
  : DemonPortHandler (s, n, Demon::stIface), object_ ((Iface *) s->object ())
{
}

Demon::Symbol
DemonIfaceHandler::getSymbol (void)
{
  Demon::Symbol sym;
  sym.name = symbol()->name ().c_str();
  sym.index = getIndex();

  Port *port = object_->getOther ();

  Demon::Iface demonport;
  if (port) {
    TreeSearcher searcher (symbolTable.getStaticTree ());
    Symbol *otherPort = searcher.get (port);
    Symbol *otherParent = (otherPort) ? otherPort->parent () : 0;
    
    demonport.parentname = (otherParent ? otherParent->name ().c_str() : "<unknown>");
    demonport.otherport = (otherPort ? otherPort->name ().c_str() : "<unknown>");
  } else {
    demonport.parentname = "<unknown>";
    demonport.otherport = "<unknown>";
  }

  // TODO: This is actually needed only when sending the whole system symbol
  Message *msg = 0;
  demonport.messages.length(100);
  int index = 0;
  for (int i = 1; (msg = object_->create (i)) != 0; i++) {
    TreeSearcher searcher (symbolTable.getDynamicTree ());
    Symbol *symbol = searcher.get (msg);
    if (symbol) {
      demonport.messages[i - 1] = symbol->name().c_str();
      index++;
      DELETE_SYMBOL (symbol);
    }
    delete msg;
  }
  demonport.messages.length(index);

  sym.value.iface(demonport);

  return sym;
}

DemonMessageHandler::DemonMessageHandler (Symbol *s, const char *n)
  : DemonHand (s, n, Demon::stMessage), object_ ((Message *) s->object ()) 
{
}

bool 
DemonMessageHandler::isEnabled() const
{
  TreeSearcher searcher (symbolTable.getStaticTree ());

  Port *sender = object_->getSender ();
  if (sender) {
    Symbol *senderSymbol = searcher.get (sender); 
    if (senderSymbol) {
     // Return status of the sender symbol as our status
     // (=If the sender Task is filtered out, the messages
     // from that Task are not sent.)
     return ((DemonHand *) senderSymbol->handler (DEMON_HANDLER))->isEnabled();
    }
  }

  return true;
}

Demon::Symbol
DemonMessageHandler::getSymbol (void)
{
  Demon::Symbol sym;
  TreeSearcher searcher (symbolTable.getStaticTree ());

  Port *target = object_->getTarget ();
  Symbol *targetSymbol = searcher.get (target); 
  Symbol *targetTaskSymbol = (targetSymbol) ? targetSymbol->parent () : 0;

  Demon::Message message;
  Port *sender = object_->getSender ();
  if (sender) {
    Symbol *senderSymbol = searcher.get (sender); 
    Symbol *senderTaskSymbol = (senderSymbol) ? senderSymbol->parent () : 0;
    message.source.parentname = 
      (senderTaskSymbol ? senderTaskSymbol->name ().c_str() : "<unknown>");
    message.source.otherport = 
      (senderTaskSymbol ? senderTaskSymbol->name ().c_str() : "<unknown>");
  } else {
    message.source.parentname = "<unknown>";
    message.source.otherport = "<unknown>";
  }

  if (targetTaskSymbol) {
    message.target.parentname = targetTaskSymbol->name ().c_str();
    message.target.otherport = targetSymbol->name ().c_str();
  } else {
    message.target.parentname = "<unknown>";
    message.target.otherport = "<unknown>";
  }

  sym.value.message(message);

  // Get symbol list
  SymbolChildIterator iterator (symbol ());
  Symbol *current = 0;
  int index = 0;
  sym.children.length(100);
  while ((current = iterator.next ()) != 0) {
    sym.children[index++] = 
      ((DemonHand *) current->handler (DEMON_HANDLER))->getSymbol ();
  }
  sym.children.length(index);
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();

  return sym;
}

DemonDebugSchHandler::DemonDebugSchHandler (Symbol *s, const char *n)
  : DemonHand (s, n, Demon::stDebugScheduler), 
    object_ ((Message *) s->object ()) 
{
}

Demon::Symbol 
DemonDebugSchHandler::getSymbol(void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();

  //TODO: Just a dummy event for now
  sym.value.debugsch(1);

  return sym;
}

DemonUserTypeHandler::DemonUserTypeHandler (Symbol *s, const char *t,
					    Demon::SymbolType st)
  : DemonHand (s, t, st)
{
}

Demon::Symbol 
DemonSint8TypeHandler::getSymbol(void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();
  sym.value.sint8(*object_);

  return sym;
}

Demon::Symbol 
DemonUint8TypeHandler::getSymbol(void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();
  sym.value.uint8(*object_);

  return sym;
}

Demon::Symbol 
DemonSint16TypeHandler::getSymbol(void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();
  sym.value.sint16(*object_);

  return sym;
}

Demon::Symbol 
DemonUint16TypeHandler::getSymbol(void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();
  sym.value.uint16(*object_);

  return sym;
}

Demon::Symbol 
DemonSint32TypeHandler::getSymbol(void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();
  sym.value.sint32(*object_);

  return sym;
}

Demon::Symbol 
DemonUint32TypeHandler::getSymbol(void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();
  sym.value.uint32(*object_);

  return sym;
}

Demon::Symbol 
DemonInetAddrTypeHandler::getSymbol(void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();
  sym.value.inetaddr4(object_->getIpAddress());

  return sym;
}

Demon::Symbol 
DemonInetAddr6TypeHandler::getSymbol(void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();
  struct sockaddr_in6* addr = (struct sockaddr_in6*)object_->getAddr();
  Demon::InetAddr6 value;
  value.length(16);
  for (int i = 0; i < 16; i++) {
    value[i] = addr->sin6_addr.s6_addr[i];
  }
  sym.value.inetaddr6(value);

  return sym;
}

Demon::Symbol 
DemonUnixAddrTypeHandler::getSymbol(void)
{
  Demon::Symbol sym;
  //TODO: Check out for changes in UnixAddr elsewhere
#if 0
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();
  Demon::UnixAddr value;
  const char* path = object_->getPathName();
  int len = strlen(path);
  value.length(len);
  for (int i = 0; i < len; i++) {
    value[i] = path[i];
  }
  sym.value.unixaddr(value);
#endif

  return sym;
}

Demon::Symbol 
DemonboolTypeHandler::getSymbol(void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();
  sym.value.boolval(*object_);

  return sym;
}

Demon::Symbol 
DemonIntTypeHandler::getSymbol(void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();
  sym.value.intval(*object_);

  return sym;
}

Demon::Symbol 
DemonStringTypeHandler::getSymbol(void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();
  sym.value.str(*object_);

  return sym;
}

Demon::Symbol 
DemonFrameTypeHandler::getSymbol(void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();
  Demon::Frame value;
  int len = object_->length();
  if (len > 0) {
    value.length(len);
    for (int i = 0; i < len; i++) {
      value[i] = object_->getFirst();
    }
    sym.value.frame(value);
  }

  return sym;
}

#if _CF_MEM

DemonMMHandler::DemonMMHandler (Symbol *s, const char *n, const char *t) 
  : DemonHand (s, n, Demon::stMemoryManager), 
    object_ ((MemoryManager *) s->object ()), type_(t)
{
}

Demon::Symbol
DemonMMHandler::getSymbol (void)
{
  Demon::Symbol sym;
  sym.name = symbol ()->name ().c_str();
  sym.index = getIndex();

  return sym;
}

DemonStatPlainMMHandler::DemonStatPlainMMHandler (Symbol *s, const char *n) 
  : DemonMMHandler (s, n, "StatPlain") 
{
}

DemonStatBlockHandler::DemonStatBlockHandler (Symbol *s, const char *n, const char *t)
  : DemonMMHandler (s, n, t) 
{
}

DemonStatBlockMHandler::DemonStatBlockMHandler (Symbol *s, const char *n)
  : DemonStatBlockHandler (s, n, "StatBlock") 
{
}

DemonStatBlockMMHandler::DemonStatBlockMMHandler (Symbol *s, const char *n)
  : DemonStatBlockHandler (s, n, "StatBlockMem") 
{
}

#endif

