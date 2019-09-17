
#include <doors/IIOPClConn.h>
#include <doors/IIOPClEnt.h>
#include <doors/iiopclconnsm.h>


IIOPClConnTask :: IIOPClConnTask (const char *n, Uint32 id, IIOPClEntTask *e, IIOPClConnSM *clconnsm) 
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
}

IIOPClConnTask :: ~IIOPClConnTask (void) {

  if (req)
    delete req;

    DELETE_SYMBOL (this);
}

bool IIOPClConnTask :: closed_opreq (Message *msg) {


    IIOPSAP::Opreq_r *sapmsg = (IIOPSAP::Opreq_r *) msg;

    req = (GIOPPeer::REQUEST *) peer.create (GIOPPeer::request);

    (GIOPPeer:: TCPOPEN) *peermsg = (GIOPPeer::TCPOPEN *) peer.create (GIOPPeer::tcpopen);

  // send open connection message and wait

    peer.putMessage (peermsg);
    expiryTimer.start ();

    state = IIOPClConnSM :: wfCopen;
    return false;

}

bool IIOPClConnTask :: closed_default (Message *msg) {

  return false;

}


bool IIOPClConnTask :: wfCopen_tcpok (Message *msg) {

  GIOPPeer:: TCPOPENOK *peermsg = (GIOPPeer::TCPOPENOK *) msg;

  // excellent. store the lower layer cep and send the request.

  peer.putMessage (req);

  return false;

}

bool IIOPClConnTask :: wfCopen_tcpnok (Message *msg) {

  GIOPPeer:: TCPOPENFAIL *peermsg = (GIOPPeer::TCPOPENFAIL *) msg;

  // woops. something happened. send diagnostic to upper layer.

  IIOPSAP :: TCPFailInd *sapmsg = (IIOPSAP :: TCPFailInd *)up.create(IIOPSAP:: tcpfailind);

  sapmsg->cep = peermsg->cep;
  sapmsg->data = peermsg->data;

  up.putMessage(upmsg);
    state = IIOPClConnSM :: closed;

  return false;

}

bool IIOPClConnTask :: wfCopen_timeout (Message *msg) {

  return false;

}

bool IIOPClConnTask :: wfCopen_default (Message *msg) {

  return false;

}

bool IIOPClConnTask :: active_opreq (Message *msg) {

  return false;

}

bool IIOPClConnTask :: active_locreq (Message *msg) {

  return false;

}

bool IIOPClConnTask :: active_mesgerr (Message *msg) {

  return false;

}

bool IIOPClConnTask :: active_cancreq (Message *msg) {

  return false;

}

bool IIOPClConnTask :: active_closecon (Message *msg) {

  return false;

}

bool IIOPClConnTask :: active_mesg_err (Message *msg) {

  return false;

}

bool IIOPClConnTask :: active_tcpfail (Message *msg) {

  return false;

}

bool IIOPClConnTask :: active_default (Message *msg) {

  return false;

}

bool IIOPClConnTask :: wfOpReply_rep (Message *msg) {

  return false;

}

bool IIOPClConnTask :: wfOpReply_timeout (Message *msg) {

  return false;

}

bool IIOPClConnTask :: wfOpReply_closecon (Message *msg) {

  return false;

}

bool IIOPClConnTask :: wfOpReply_mesg_err (Message *msg) {

  return false;

}

bool IIOPClConnTask :: wfOpReply_tcpfail (Message *msg) {

  return false;

}

bool IIOPClConnTask :: wfOpReply_default (Message *msg) {

  return false;

}

bool IIOPClConnTask :: wfLocReply_lrep (Message *msg) {

  return false;

}

bool IIOPClConnTask :: wfLocReply_timeout (Message *msg) {

  return false;

}

bool IIOPClConnTask :: wfLocReply_closecon (Message *msg) {

  return false;

}

bool IIOPClConnTask :: wfLocReply_mesg_err (Message *msg) {

  return false;

}

bool IIOPClConnTask :: wfLocReply_tcpfail (Message *msg) {

  return false;

}

bool IIOPClConnTask :: wfLocReply_default (Message *msg) {

  return false;

}

Uint32 IIOPClConnTask :: getId (void) {

  return cid;

}
