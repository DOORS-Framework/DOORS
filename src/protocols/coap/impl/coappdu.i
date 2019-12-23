
#include "coappdu.hh"


Message *createCoAPPeerCON (void)
{
  return new CoAPPeer :: CON;
}

Message *createCoAPPeerACK (void)
{
  return new CoAPPeer :: ACK;
}

Message *createCoAPPeerRST (void)
{
  return new CoAPPeer :: RST;
}

Message *(*coAPPeerMessages []) (void) =
{
  createCoAPPeerCON,
  createCoAPPeerACK,
  createCoAPPeerRST,
  0
};

CoAPPeer :: CoAPPeer (EventTask *host, ReqIface *d)
 : Peer (host, coAPPeerMessages, d)
{
}

CoAPPeer :: ~CoAPPeer ()
{
}
CoAPPeer :: CON :: CON (void)
{
  DYNAMIC_SYMBOL (this, "con", 0);
  STATIC_SYMBOL (ip, this);
  STATIC_SYMBOL (VerTOC, this);
  STATIC_SYMBOL (Code, this);
  STATIC_SYMBOL (MsgID, this);
  STATIC_SYMBOL (Options, this);
  STATIC_SYMBOL (Payload, this);
}

CoAPPeer :: CON :: CON(const CON &msg)
: PDU(msg), ip (msg.ip),VerTOC (msg.VerTOC),Code (msg.Code),MsgID (msg.MsgID),Options (msg.Options),Payload (msg.Payload)
{
  DYNAMIC_SYMBOL (this, "con", 0);
  STATIC_SYMBOL (ip, this);
  STATIC_SYMBOL (VerTOC, this);
  STATIC_SYMBOL (Code, this);
  STATIC_SYMBOL (MsgID, this);
  STATIC_SYMBOL (Options, this);
  STATIC_SYMBOL (Payload, this);
}

CoAPPeer :: CON :: ~CON (void)
{
  DELETE_SYMBOL (this);
}

MessageType CoAPPeer :: CON :: getType(void) const
{
  return con;
}
Message *CoAPPeer :: CON :: clone (void) const
{
  return new CON(*this);
}

CoAPPeer :: ACK :: ACK (void)
{
  DYNAMIC_SYMBOL (this, "ack", 0);
  STATIC_SYMBOL (ip, this);
  STATIC_SYMBOL (VerTOC, this);
  STATIC_SYMBOL (Code, this);
  STATIC_SYMBOL (MsgID, this);
}

CoAPPeer :: ACK :: ACK(const ACK &msg)
: PDU(msg), ip (msg.ip),VerTOC (msg.VerTOC),Code (msg.Code),MsgID (msg.MsgID)
{
  DYNAMIC_SYMBOL (this, "ack", 0);
  STATIC_SYMBOL (ip, this);
  STATIC_SYMBOL (VerTOC, this);
  STATIC_SYMBOL (Code, this);
  STATIC_SYMBOL (MsgID, this);
}

CoAPPeer :: ACK :: ~ACK (void)
{
  DELETE_SYMBOL (this);
}

MessageType CoAPPeer :: ACK :: getType(void) const
{
  return ack;
}
Message *CoAPPeer :: ACK :: clone (void) const
{
  return new ACK(*this);
}

CoAPPeer :: RST :: RST (void)
{
  DYNAMIC_SYMBOL (this, "rst", 0);
  STATIC_SYMBOL (ip, this);
  STATIC_SYMBOL (VerTOC, this);
  STATIC_SYMBOL (Code, this);
  STATIC_SYMBOL (MsgID, this);
}

CoAPPeer :: RST :: RST(const RST &msg)
: PDU(msg), ip (msg.ip),VerTOC (msg.VerTOC),Code (msg.Code),MsgID (msg.MsgID)
{
  DYNAMIC_SYMBOL (this, "rst", 0);
  STATIC_SYMBOL (ip, this);
  STATIC_SYMBOL (VerTOC, this);
  STATIC_SYMBOL (Code, this);
  STATIC_SYMBOL (MsgID, this);
}

CoAPPeer :: RST :: ~RST (void)
{
  DELETE_SYMBOL (this);
}

MessageType CoAPPeer :: RST :: getType(void) const
{
  return rst;
}
Message *CoAPPeer :: RST :: clone (void) const
{
  return new RST(*this);
}

