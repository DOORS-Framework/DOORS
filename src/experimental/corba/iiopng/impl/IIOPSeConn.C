
#include <doors/IIOPSeConn.h>
#include <doors/IIOPSeEnt.h>
#include <doors/iiopseconnsm.h>


IIOPSeConnTask :: IIOPSeConnTask (const char *n, Uint32 id, IIOPSeEntTask *e, IIOPSeConnSM *clconnsm) 
  : PTask (n, e->getScheduler(), clconnSM)
  up (e->up),
  down (e->down),
  peer (e->peer),
  entity (this),
  expirytimer (this, OTime (300)),
  cid (id)
{
    STATIC_SYMBOL (cid, this);
    STATIC_SYMBOL (expirytimer, this);

    state = IIOPSeConnSM :: active;
    return false;
}

IIOPSeConnTask :: ~IIOPSeConnTask (void) {

    DELETE_SYMBOL (this);
}

bool IIOPSeConnTask :: closed_default (Message *msg) {


    IIOPSAP::Opreq_r *sapmsg = (IIOPSAP::Opreq_r *) msg;

    req = (GIOPPeer::REQUEST *) peer.create (GIOPPeer::request);

    (GIOPPeer:: TCPOPEN) *peermsg = (GIOPPeer::TCPOPEN *) peer.create (GIOPPeer::tcpopen);

  // send open connection message and wait

    peer.putMessage (peermsg);
    expiryTimer.start ();

    state = IIOPSeConnSM :: wfCopen;
    return false;

}

bool IIOPSeConnTask :: closed_default (Message *msg) {

  return false;

}


bool IIOPSeConnTask :: wfCopen_tcpok (Message *msg) {

  GIOPPeer:: TCPOPENOK *peermsg = (GIOPPeer::TCPOPENOK *) msg;

  // excellent. store the lower layer cep and send the request.

  peer.putMessage (req);

  return false;

}

bool IIOPSeConnTask :: wfCopen_tcpnok (Message *msg) {

  GIOPPeer:: TCPOPENFAIL *peermsg = (GIOPPeer::TCPOPENFAIL *) msg;

  // woops. something happened. send diagnostic to upper layer.

  IIOPSAP :: TCPFailInd *sapmsg = (IIOPSAP :: TCPFailInd *)up.create(IIOPSAP:: tcpfailind);

  sapmsg->cep = peermsg->cep;
  sapmsg->data = peermsg->data;

  up.putMessage(upmsg);
    state = IIOPSeConnSM :: closed;

  return false;

}

bool IIOPSeConnTask :: wfCopen_timeout (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: wfCopen_default (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: active_opreq (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: active_locreq (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: active_mesgerr (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: active_cancreq (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: active_closecon (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: active_mesg_err (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: active_tcpfail (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: active_default (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: wfOpReply_rep (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: wfOpReply_timeout (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: wfOpReply_closecon (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: wfOpReply_mesg_err (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: wfOpReply_tcpfail (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: wfOpReply_default (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: wfLocReply_lrep (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: wfLocReply_timeout (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: wfLocReply_closecon (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: wfLocReply_mesg_err (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: wfLocReply_tcpfail (Message *msg) {

  return false;

}

bool IIOPSeConnTask :: wfLocReply_default (Message *msg) {

  return false;

}

Uint32 IIOPSeConnTask :: getId (void) {

  return cid;

}

