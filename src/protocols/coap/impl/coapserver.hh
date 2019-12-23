#ifndef _coapserversm_h_
#define _coapserversm_h_


#include <doors/ptb.h>


#include "coapservertask.hh"
class CoAPserverTask;

class CoAPServerSM : public StateMachine
{
  public:

    enum State { Listen = 0, Wait, other };

    CoAPServerSM(void);
    virtual ~CoAPServerSM(void);

    std::string stateName(PTask *h);
    bool transit (EventType t, Message *msg, PTask *host);

  protected:
  private:

    int states;
    int events;
    bool (CoAPserverTask :: *execute [5][2]) (Message *msg);
};

#endif
