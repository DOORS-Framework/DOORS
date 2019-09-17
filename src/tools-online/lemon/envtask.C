//Editor-Info: -*- C++ -*-
//

#include <doors/envtask.h>
#include <doors/hsitif.h>


EnvTask :: EnvTask (Scheduler *s) 
: EventTask ("Environment", s) {

    EnvTask &env = *this;
    STATIC_SYMBOL (env, 0);
}


EnvTask :: ~EnvTask (void) {

    DELETE_SYMBOL (this);
}
    

void EnvTask :: add (Iface *i, const char *nm) {

    ifaces.insert (i, false);

    DYNAMIC_SYMBOL (i, nm, this);
}

void EnvTask :: remove (Iface *i) {

    Iface *tmp = ifaces.remove (i);
    delete tmp;
}

void EnvTask :: remove (void) {

    Iface *tmp = 0;
    while ((tmp = ifaces.remove ()) != 0) {
	/* Deleted already in ifaces.remove? (Joona) */
	//delete tmp;
    }
}


bool EnvTask :: execute (EventType t, Message *msg) {

    TRACE_MSG (msg);

    delete msg;
    return true;
}

