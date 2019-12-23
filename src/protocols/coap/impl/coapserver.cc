

#include "coapserver.hh"


CoAPServerSM :: CoAPServerSM(void)
: states (2), events(5)
{
  execute [0][0] = &CoAPserverTask :: listen_default;
  execute [1][0] = &CoAPserverTask :: listen_CON;
  execute [2][0] = &CoAPserverTask :: listen_default;
  execute [3][0] = &CoAPserverTask :: listen_default;
  execute [4][0] = &CoAPserverTask :: listen_default;
  
  execute [0][1] = &CoAPserverTask :: wait_default;
  execute [1][1] = &CoAPserverTask :: wait_default;
  execute [2][1] = &CoAPserverTask :: wait_ACK;
  execute [3][1] = &CoAPserverTask :: wait_RST;
  execute [4][1] = &CoAPserverTask :: wait_ACKtimeout;
  
}

CoAPServerSM :: ~CoAPServerSM(void) {

}

bool CoAPServerSM :: transit (EventType t, Message *msg, PTask *h) {
  
  int msgId = 0;

  if (t == SystemEvent) {

    msgId = 4;

  } else

  if (msg->getTarget () == &((CoAPserverTask *) h)->peer) {
    switch(msg->getType ()) {
      case CoAPPeer :: rst :
        msgId = 3;
        break;

      case CoAPPeer :: ack :
        msgId = 2;
        break;

      case CoAPPeer :: con :
        msgId = 1;
        break;

    }

  }  
  return (((CoAPserverTask*) h)->*execute [msgId][h->getState ()]) (msg);
}

std::string CoAPServerSM :: stateName (PTask *h) {

  const char *rc = 0;

  switch (h->getState ()) {
    case 0 : 
      rc = "Listen";
      break;
    case 1 : 
      rc = "Wait";
      break;
    default:
      rc = 0;
  }
  return rc;
}
