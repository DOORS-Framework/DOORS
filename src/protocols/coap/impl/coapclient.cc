

#include "coapclient.hh"


CoAPClientSM :: CoAPClientSM(void)
: states (2), events(8)
{
  execute [0][0] = &CoAPclientTask :: idle_default;
  execute [1][0] = &CoAPclientTask :: idle_get;
  execute [2][0] = &CoAPclientTask :: idle_put;
  execute [3][0] = &CoAPclientTask :: idle_del;
  execute [4][0] = &CoAPclientTask :: idle_default;
  execute [5][0] = &CoAPclientTask :: idle_default;
  execute [6][0] = &CoAPclientTask :: idle_default;
  execute [7][0] = &CoAPclientTask :: idle_default;
  
  execute [0][1] = &CoAPclientTask :: wait_default;
  execute [1][1] = &CoAPclientTask :: wait_default;
  execute [2][1] = &CoAPclientTask :: wait_default;
  execute [3][1] = &CoAPclientTask :: wait_default;
  execute [4][1] = &CoAPclientTask :: wait_ACK;
  execute [5][1] = &CoAPclientTask :: wait_RST;
  execute [6][1] = &CoAPclientTask :: wait_CON;
  execute [7][1] = &CoAPclientTask :: wait_ACKTimeout;
  
}

CoAPClientSM :: ~CoAPClientSM(void) {

}

bool CoAPClientSM :: transit (EventType t, Message *msg, PTask *h) {
  
  int msgId = 0;

  if (t == SystemEvent) {

    msgId = 7;

  } else

  if (msg->getTarget () == &((CoAPclientTask *) h)->up) {
    switch(msg->getType ()) {
      case CoAPSAP :: del :
        msgId = 3;
        break;

      case CoAPSAP :: put :
        msgId = 2;
        break;

      case CoAPSAP :: get :
        msgId = 1;
        break;

    }

  } else

  if (msg->getTarget () == &((CoAPclientTask *) h)->peer) {
    switch(msg->getType ()) {
      case CoAPPeer :: con :
        msgId = 6;
        break;

      case CoAPPeer :: rst :
        msgId = 5;
        break;

      case CoAPPeer :: ack :
        msgId = 4;
        break;

    }

  }  
  return (((CoAPclientTask*) h)->*execute [msgId][h->getState ()]) (msg);
}

std::string CoAPClientSM :: stateName (PTask *h) {

  const char *rc = 0;

  switch (h->getState ()) {
    case 0 : 
      rc = "Idle";
      break;
    case 1 : 
      rc = "Wait";
      break;
    default:
      rc = 0;
  }
  return rc;
}
