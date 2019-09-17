
#ifndef IIOPCLCONN_H
#define IIOPCLCONN_H


#include <doors/connmgr.h>
#include <doors/iioppeer.h>
include <doors/tran.h>

class IIOPClConnSM;

class IIOPClConnTask : public PTask {

 public:

  IIOPClConnTask (const char *n, Uint32 id, IIOPClEntTask *e, IIOPClConnSM *clconnsm);
  virtual ~IIOPClConnTask (void);

  bool closed_opreq (Message *msg);
  bool closed_default (Message *msg);

  bool wfCopen_tcpok (Message *msg);
  bool wfCopen_tcpnok (Message *msg);
  bool wfCopen_timeout (Message *msg);
  bool wfCopen_default (Message *msg);

  bool active_opreq (Message *msg);
  bool active_locreq (Message *msg);
  bool active_mesgerr (Message *msg);
  bool active_cancreq (Message *msg);
  bool active_closecon (Message *msg);
  bool active_mesg_err (Message *msg);
  bool active_tcpnok (Message *msg);
  bool active_default (Message *msg);

  bool wfOpReply_rep (Message *msg);
  bool wfOpReply_timeout (Message *msg);
  bool wfOpReply_closecon (Message *msg);
  bool wfOpReply_mesg_err (Message *msg);
  bool wfOpReply_tcpnok (Message *msg);
  bool wfOpReply_default (Message *msg);

  bool wfLocReply_lrep (Message *msg);
  bool wfLocReply_timeout (Message *msg);
  bool wfLocReply_closecon (Message *msg);
  bool wfLocReply_mesg_err (Message *msg);
  bool wfLocReply_tcpnok (Message *msg);
  bool wfLocReply_default (Message *msg);

  Uint32 getId (void);

  IIOPSAP :: Provider &up;
  TranSAP :: User &down;
  GIOPPeer & peer;

  Port ClEntity;

 private:

  GIOPPeer::REQUEST *req;
  Timer expirytimer;
  uint32 cid;
};

#endif //IIOPClConn_H
