#ifndef _coapclientsm_h_
#define _coapclientsm_h_


#include <doors/ptb.h>


#include "coapclienttask.hh"
class CoAPclientTask;

class CoAPClientSM : public StateMachine
{
  public:

    enum State { Idle = 0, Wait, other };

    CoAPClientSM(void);
    virtual ~CoAPClientSM(void);

    std::string stateName(PTask *h);
    bool transit (EventType t, Message *msg, PTask *host);

  protected:
  private:

    int states;
    int events;
    bool (CoAPclientTask :: *execute [8][2]) (Message *msg);
};

#endif
