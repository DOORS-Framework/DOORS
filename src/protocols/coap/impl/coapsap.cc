
#include <doors/hsi.h>
#include "coapsap.hh"
#include "string"
#include "coapsap.hh"

Message *createCoAPSAPGet (void)
{
  return new CoAPSAP :: Get;
}

Message *createCoAPSAPPut (void)
{
  return new CoAPSAP :: Put;
}

Message *createCoAPSAPDel (void)
{
  return new CoAPSAP :: Del;
}

Message *(*coAPSAPUserMessages []) (void) =
{
  createCoAPSAPGet,
  createCoAPSAPPut,
  createCoAPSAPDel,
  0
};

Message *createCoAPSAPConfirm (void)
{
  return new CoAPSAP :: Confirm;
}

Message *createCoAPSAPError (void)
{
  return new CoAPSAP :: Error;
}

Message *(*coAPSAPProviderMessages []) (void) =
{
  createCoAPSAPConfirm,
  createCoAPSAPError,
  0
};

CoAPSAP :: User :: User (EventTask *t, Peer *p)
: ReqIface (t, coAPSAPUserMessages, p)
{
}

CoAPSAP :: User :: ~User(void)
{
}
CoAPSAP :: Get :: Get (void)
{
  DYNAMIC_SYMBOL (this, "get", 0);
  STATIC_SYMBOL (IPport, this);
  STATIC_SYMBOL (filePath, this);
}

CoAPSAP :: Get :: Get(const Get &msg)
: Message(msg), IPport (msg.IPport),filePath (msg.filePath)
{
  DYNAMIC_SYMBOL (this, "get", 0);
  STATIC_SYMBOL (IPport, this);
  STATIC_SYMBOL (filePath, this);
}

Message *CoAPSAP :: Get :: clone (void) const
{
  return new Get(*this);
}

CoAPSAP :: Get :: ~Get (void)
{
  DELETE_SYMBOL (this);
}

CoAPSAP :: Put :: Put (void)
{
  DYNAMIC_SYMBOL (this, "put", 0);
  STATIC_SYMBOL (IPport, this);
  STATIC_SYMBOL (filePath, this);
  STATIC_SYMBOL (payload, this);
}

CoAPSAP :: Put :: Put(const Put &msg)
: Message(msg), IPport (msg.IPport),filePath (msg.filePath),payload (msg.payload)
{
  DYNAMIC_SYMBOL (this, "put", 0);
  STATIC_SYMBOL (IPport, this);
  STATIC_SYMBOL (filePath, this);
  STATIC_SYMBOL (payload, this);
}

Message *CoAPSAP :: Put :: clone (void) const
{
  return new Put(*this);
}

CoAPSAP :: Put :: ~Put (void)
{
  DELETE_SYMBOL (this);
}

CoAPSAP :: Del :: Del (void)
{
  DYNAMIC_SYMBOL (this, "del", 0);
  STATIC_SYMBOL (IPport, this);
  STATIC_SYMBOL (filePath, this);
}

CoAPSAP :: Del :: Del(const Del &msg)
: Message(msg), IPport (msg.IPport),filePath (msg.filePath)
{
  DYNAMIC_SYMBOL (this, "del", 0);
  STATIC_SYMBOL (IPport, this);
  STATIC_SYMBOL (filePath, this);
}

Message *CoAPSAP :: Del :: clone (void) const
{
  return new Del(*this);
}

CoAPSAP :: Del :: ~Del (void)
{
  DELETE_SYMBOL (this);
}

CoAPSAP :: Provider :: Provider(EventTask *t)
: IndIface (t, coAPSAPProviderMessages)
{
}

CoAPSAP :: Provider :: ~Provider(void)
{
}
CoAPSAP :: Confirm :: Confirm (void)
{
  DYNAMIC_SYMBOL (this, "confirm", 0);
  STATIC_SYMBOL (confirm_code, this);
  STATIC_SYMBOL (data, this);
}

CoAPSAP :: Confirm :: Confirm(const Confirm &msg)
: Message(msg), confirm_code (msg.confirm_code),data (msg.data)
{
  DYNAMIC_SYMBOL (this, "confirm", 0);
  STATIC_SYMBOL (confirm_code, this);
  STATIC_SYMBOL (data, this);
}

Message *CoAPSAP :: Confirm :: clone (void) const
{
  return new Confirm(*this);
}

CoAPSAP :: Confirm :: ~Confirm (void)
{
  DELETE_SYMBOL (this);
}

CoAPSAP :: Error :: Error (void)
{
  DYNAMIC_SYMBOL (this, "error", 0);
  STATIC_SYMBOL (error_code, this);
}

CoAPSAP :: Error :: Error(const Error &msg)
: Message(msg), error_code (msg.error_code)
{
  DYNAMIC_SYMBOL (this, "error", 0);
  STATIC_SYMBOL (error_code, this);
}

Message *CoAPSAP :: Error :: clone (void) const
{
  return new Error(*this);
}

CoAPSAP :: Error :: ~Error (void)
{
  DELETE_SYMBOL (this);
}

