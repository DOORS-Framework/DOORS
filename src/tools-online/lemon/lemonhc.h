//Editor-Info: -*- C++ -*-
//

#ifndef LEMONHC_H
#define LEMONHC_H

#include <doors/symhc.h>
#include <doors/doorsconf.h>
#include <string>

#if HAVE_IPV6
#include <doors/inetaddr6.h>
#endif

class LEMonHandlerCreator : public HandlerCreator {
  public:
    
    void createHandler (Otask *, Symbol *s);
    void createHandler (EventTask *, Symbol *s);
    void createHandler (PTask *, Symbol *s);
    void createHandler (LinkList *, Symbol *s);
    void createHandler (DebugScheduler *, Symbol *s);
    void createHandler (EnvTask *, Symbol *s);
    void createHandler (Port *, Symbol *s);
    void createHandler (Timer *, Symbol *s);
    void createHandler (Iface *, Symbol *s);
    void createHandler (Peer *, Symbol *s);
    void createHandler (Message *, Symbol *s);
    void createHandler (UnixAddr *, Symbol *s);
    void createHandler (InetAddr *, Symbol *s);
#if HAVE_IPV6
    void createHandler (InetAddr6 *, Symbol *s);
#endif
    void createHandler (bool *, Symbol *s);
    void createHandler (Sint8 *, Symbol *s);
    void createHandler (Sint16 *, Symbol *s);
    void createHandler (Sint32 *, Symbol *s);
    void createHandler (Uint8 *, Symbol *s);
    void createHandler (Uint16 *, Symbol *s);
    void createHandler (Uint32 *, Symbol *s);
    void createHandler (std::string *, Symbol *s);
    void createHandler (Load *, Symbol *s);
    void createHandler (char **, Symbol *s);
    void createHandler (Frame *, Symbol *s);
    
    void createHandler (MemoryManager *, Symbol *s);
    void createHandler (StatPlainMemManager *, Symbol *s);
    void createHandler (StatBlockManager *, Symbol *s);
    void createHandler (StatBlockMemManager *, Symbol *s);
};


#endif  // LEMONHC_H

