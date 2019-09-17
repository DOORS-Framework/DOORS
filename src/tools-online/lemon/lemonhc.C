//Editor-Info: -*- C++ -*-
//

#ifdef HAVE_CONFIG_H
#include <doors/doorsconf.h>
#endif

#include <doors/lemonhc.h>
#include <doors/lemonhand.h>
#include <string>

void LEMonHandlerCreator :: createHandler (Otask *, Symbol *s) 
{
}


void LEMonHandlerCreator :: createHandler (EventTask *t, Symbol *s) {

#if _MT_ENABLED
    s->setHandler (new MTEventTaskHandler (s, "EventTask", "Event task"), LEMON_HANDLER);
#else
    s->setHandler (new EventTaskHandler (s, "EventTask", "Event task"), LEMON_HANDLER);
#endif

    _MT_SET_LOCKPOINTER(s->handler(LEMON_HANDLER), t);
    _MT_SET_LOCKPOINTER(s, t);
}


void LEMonHandlerCreator :: createHandler (PTask *t, Symbol *s) {

    s->setHandler (new PTaskHandler (s, "Protocol task"), LEMON_HANDLER);
    _MT_SET_LOCKPOINTER(s->handler(LEMON_HANDLER), t);
    _MT_SET_LOCKPOINTER(s, t);
    _MT_UPDATE(s);
}


void LEMonHandlerCreator :: createHandler (DebugScheduler *t, Symbol *s) {

    s->setHandler (new DebugSchHandler (s, "Debugging scheduler"), LEMON_HANDLER);
}


void LEMonHandlerCreator :: createHandler (EnvTask *t, Symbol *s) {

    s->setHandler (new EventTaskHandler (s, "EnvTask", "Environment task"), LEMON_HANDLER);
}


void LEMonHandlerCreator :: createHandler (LinkList *t, Symbol *s) {

    s->setHandler (new LinkListHandler (s, "Message queue object"), LEMON_HANDLER);
}

void LEMonHandlerCreator :: createHandler (Port *t, Symbol *s) {

    s->setHandler (new PortHandler (s, "Port", "Port object"), LEMON_HANDLER);
}


void LEMonHandlerCreator :: createHandler (Timer *t, Symbol *s) {

    s->setHandler (new TimerHandler (s, "Timer object"), LEMON_HANDLER);
}


void LEMonHandlerCreator :: createHandler (Iface *t, Symbol *s) {

    s->setHandler (new IfaceHandler (s, "Interface object"), LEMON_HANDLER);
}


void LEMonHandlerCreator :: createHandler (Peer *t, Symbol *s) {

    s->setHandler (new IfaceHandler (s, "Peer interface object"), LEMON_HANDLER);
}


void LEMonHandlerCreator :: createHandler (Message *t, Symbol *s) {

    s->setHandler (new MessageHandler (s, ""), LEMON_HANDLER);
}


void LEMonHandlerCreator :: createHandler (UnixAddr *t, Symbol *s) {

    s->setHandler (new FundTypeHandler<UnixAddr> (s, "UnixAddr"), LEMON_HANDLER);
}


void LEMonHandlerCreator :: createHandler (InetAddr *t, Symbol *s) {

    s->setHandler (new FundTypeHandler<InetAddr> (s, "InetAddr"), LEMON_HANDLER);
}

#if HAVE_IPV6
void LEMonHandlerCreator :: createHandler (InetAddr6 *t, Symbol *s) {

    s->setHandler (new FundTypeHandler<InetAddr6> (s, "InetAddr6"), LEMON_HANDLER);
}
#endif

void LEMonHandlerCreator :: createHandler (bool *t, Symbol *s) {

    s->setHandler (new FundTypeHandler<bool> (s, "bool"), LEMON_HANDLER);
}

void LEMonHandlerCreator :: createHandler (Sint8 *t, Symbol *s) {

    s->setHandler (new FundTypeHandler<Sint8> (s, "Sint8"), LEMON_HANDLER);
}

void LEMonHandlerCreator :: createHandler (Sint16 *t, Symbol *s) {

    s->setHandler (new FundTypeHandler<Sint16> (s, "Sint16"), LEMON_HANDLER);
}

void LEMonHandlerCreator :: createHandler (Sint32 *t, Symbol *s) {

    s->setHandler (new FundTypeHandler<Sint32> (s, "Sint32"), LEMON_HANDLER);
}

void LEMonHandlerCreator :: createHandler (Uint8 *t, Symbol *s) {

    s->setHandler (new FundTypeHandler<Uint8> (s, "Uint8"), LEMON_HANDLER);
}

void LEMonHandlerCreator :: createHandler (Uint16 *t, Symbol *s) {

    s->setHandler (new FundTypeHandler<Uint16> (s, "Uint16"), LEMON_HANDLER);
}

void LEMonHandlerCreator :: createHandler (Uint32 *t, Symbol *s) {

    s->setHandler (new FundTypeHandler<Uint32> (s, "Uint32"), LEMON_HANDLER);
}

void LEMonHandlerCreator :: createHandler (std::string *t, Symbol *s) {

    s->setHandler (new FundTypeHandler<std::string> (s, "String"), LEMON_HANDLER);
}

void LEMonHandlerCreator :: createHandler (signed long *t, Symbol *s) {

    s->setHandler (new FundTypeHandler<int> (s, "int"), LEMON_HANDLER);
}

void LEMonHandlerCreator :: createHandler (char **t, Symbol *s) {

    s->setHandler (new FundTypeHandler<char *> (s, "char *"), LEMON_HANDLER);
}

void LEMonHandlerCreator :: createHandler (Frame *t, Symbol *s) {

    s->setHandler (new FundTypeHandler<Frame> (s, "Frame"), LEMON_HANDLER);
}


void LEMonHandlerCreator :: createHandler (MemoryManager *t, Symbol *s) {

    // !!!
}

void LEMonHandlerCreator :: createHandler (StatPlainMemManager *t, Symbol *s) {

    s->setHandler (new StatPlainMMHandler (s, "Access to statistics of memory manager."), LEMON_HANDLER);
}

void LEMonHandlerCreator :: createHandler (StatBlockManager *t, Symbol *s) {

    s->setHandler (new StatBlockMHandler (s, "Access to statistics of memory manager."), LEMON_HANDLER);
}

void LEMonHandlerCreator :: createHandler (StatBlockMemManager *t, Symbol *s) {

    s->setHandler (new StatBlockMMHandler (s, "Access to statistics of memory manager."), LEMON_HANDLER);
}

#if _CF_ASN1

void LEMonHandlerCreator :: createHandler (AsnType *t, Symbol *s) {

    s->setHandler (new AsnTypeHandler (s, "AsnType"), LEMON_HANDLER);
}

#endif









