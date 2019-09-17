//Editor-Info: -*- C++ -*-
//
// 



#ifndef ENVTASK_H
#define ENVTASK_H

#include <doors/ptb.h>

class Iface;

class EnvTask : public EventTask {
  public:
    EnvTask (Scheduler *s);
    virtual ~EnvTask (void);
    
    void add (Iface *i, const char *name);
    void remove (Iface *i);
    void remove (void);

    virtual bool execute (EventType t, Message *msg);

  private:
    TQueue<Iface> ifaces;
};


#endif  // ENVTASK_H

