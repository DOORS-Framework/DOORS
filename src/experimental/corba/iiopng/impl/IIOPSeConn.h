
#ifndef IIOPSECONN_H
#define IIOPSECONN_H


#include <doors/connmgr.h>
#include <doors/iioppeer.h>
#include <doors/tran.h>

class IIOPSeConnSM;

class IIOPSeConnTask : public PTask {

 public:

  IIOPSeConnTask (const char *n, IIOPSeEntTask *e, IIOPSeConnSM *clconsm);
  virtual ~IIOPSeConnTask (void);

  bool closed_default (Message *msg);

  bool wfCopen_tcpopen (Message *msg);
  bool wfCopen_tcpfail (Message *msg);
  bool wfCopen_timeout (Message *msg);
  bool wfCresp_default (Message *msg);

  bool active_close_con (Message *msg);
  bool active_mesgerr (Message *msg);
  bool active_req (Message *msg);
  bool active_lreq (Message *msg);
  bool active_cancreq (Message *msg);
  bool active_mesg_err (Message *msg);
  bool active_tcpfail (Message *msg);
  bool idle_default (Message *msg);

  bool wfOpReply_oprep (Message *msg);
  bool wfOpReply_timeout (Message *msg);
  bool wfOpReply_close_con (Message *msg);
  bool wfOpReply_mesgerr (Message *msg);
  bool wfOpReply_cancreq (Message *msg);
  bool wfOpReply_mesg_err (Message *msg);
  bool wfOpReply_tcpfail (Message *msg);
  bool wfOpReply_default (Message *msg);

  bool wfLocReply_locrep (Message *msg);
  bool wfLocReply_timeout (Message *msg);
  bool wfLocReply_close_con (Message *msg);
  bool wfLocReply_mesgerr (Message *msg);
  bool wfLocReply_cancreq (Message *msg);
  bool wfLocReply_mesg_err (Message *msg);
  bool wfLocReply_tcpfail (Message *msg);
  bool wfLocReply_default (Message *msg);

  Uint32 getId (void);

  IIOPSAP :: Provider &up;
  TranSAP :: User &down;
  GIOPPeer & peer;

  Port SeEntity;

 private:

  Timer expirytimer;
  uint32 cid;
};

#endif //IIOPSeConn_H
