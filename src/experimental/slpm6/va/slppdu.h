
#ifndef _slppeer_h_
#define _slppeer_h_


#include <doors/hsi.h>
#include <doors/ptb.h>
#include <doors/slptypes.h>
#include <doors/slppdu.h>
#include <doors/serviceurl.h>
#include <doors/inetaddr6.h>

class SLPPeer : public Peer
{
  public:

    enum State { srvrqst = 1, srvrply, srvreg, srvdereg, srvack, attrrqst, attrrply, daadvert, srvtyperqst, srvtyperply, saadvert, aaadvert, other };

    class SRVRQST : public PDU, public SLPHeader
    {
      public:
        SRVRQST (void);
        SRVRQST (const SRVRQST &msg);
        ~SRVRQST (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        AddrVector PRList;
        std::string servicetype;
        StringVector scopeList;
        std::string predicate;
        std::string spi;
    };

    class SRVRPLY : public PDU, public SLPHeader
    {
      public:
        SRVRPLY (void);
        SRVRPLY (const SRVRPLY &msg);
        ~SRVRPLY (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        Uint16 errorCode;
        URLVector urlEntries;
    };

    class SRVREG : public PDU, public SLPHeader
    {
      public:
        SRVREG (void);
        SRVREG (const SRVREG &msg);
        ~SRVREG (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        ServiceURL url;
        std::string  serviceType;
        Uint16 errorCode;
        StringVector  scopeList;
        StringVector  attributeList;
        AuthBlockVector  AttrAuthBlocks;
    };

    class SRVDEREG : public PDU, public SLPHeader
    {
      public:
        SRVDEREG (void);
        SRVDEREG (const SRVDEREG &msg);
        ~SRVDEREG (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        StringVector  scopeList;
        ServiceURL url;
        StringVector tagList;
    };

    class SRVACK : public PDU, public SLPHeader
    {
      public:
        SRVACK (void);
        SRVACK (const SRVACK &msg);
        ~SRVACK (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        Uint16 errorCode;
    };

    class ATTRRQST : public PDU, public SLPHeader
    {
      public:
        ATTRRQST (void);
        ATTRRQST (const ATTRRQST &msg);
        ~ATTRRQST (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        AddrVector PRList;
        ServiceURL URL;
        StringVector scopeList;
        StringVector  tagList;
        std::string spiString;
    };

    class ATTRRPLY : public PDU, public SLPHeader
    {
      public:
        ATTRRPLY (void);
        ATTRRPLY (const ATTRRPLY &msg);
        ~ATTRRPLY (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        Uint16 errorCode;
        AttrVector  attributeList;
        StringVector attrAuthBlocks;
    };

    class DAADVERT : public PDU, public SLPHeader
    {
      public:
        DAADVERT (void);
        DAADVERT (const DAADVERT &msg);
        ~DAADVERT (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        Uint16 errorCode;
        Uint32 DABootStamp;
        ServiceURL url;
        StringVector scopeList;
        StringVector  attributeList;
        std::string spi;
        AuthBlockVector attrAuthBlocks;
    };

    class SRVTYPERQST : public PDU, public SLPHeader
    {
      public:
        SRVTYPERQST (void);
        SRVTYPERQST (const SRVTYPERQST &msg);
        ~SRVTYPERQST (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        AddrVector PRList;
        std::string namingA;
        StringVector scopeList;
    };

    class SRVTYPERPLY : public PDU, public SLPHeader
    {
      public:
        SRVTYPERPLY (void);
        SRVTYPERPLY (const SRVTYPERPLY &msg);
        ~SRVTYPERPLY (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        Uint16 errorCode;
        StringVector typeList;
    };

    class SAADVERT : public PDU, public SLPHeader
    {
      public:
        SAADVERT (void);
        SAADVERT (const SAADVERT &msg);
        ~SAADVERT (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        ServiceURL url;
        StringVector  scopeList;
        StringVector  attributeList;
        AuthBlockVector AttrAuthBlocks;
    };

    class AAADVERT : public PDU, public SLPHeader
    {
      public:
        AAADVERT (void);
        AAADVERT (const AAADVERT &msg);
        ~AAADVERT (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        ServiceURL aa_url;
        ServiceURL da_url;
        StringVector  attributeList;
        AuthBlockVector AttrAuthBlocks;
    };

    SLPPeer(EventTask *host, ReqIface *d);
    ~SLPPeer(void);

    Sint16 identify (Message *msg);

  protected:
  private:
};



#endif
