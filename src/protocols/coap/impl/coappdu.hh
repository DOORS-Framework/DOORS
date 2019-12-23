
#ifndef _coappeer_h_
#define _coappeer_h_


#include <doors/hsi.h>
#include <doors/ptb.h>
#include "coapconstants.hh"

class CoAPPeer : public Peer
{
  public:

    enum State { con = 1, ack, rst, other };

    class CON : public PDU
    {
      public:
        CON (void);
        CON (const CON &msg);
        ~CON (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        InetAddr ip;
        Byte VerTOC;
        Byte Code;
        Word MsgID;
        Frame Options;
        Frame Payload;
    };

    class ACK : public PDU
    {
      public:
        ACK (void);
        ACK (const ACK &msg);
        ~ACK (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        InetAddr ip;
        Byte VerTOC;
        Byte Code;
        Word MsgID;
    };

    class RST : public PDU
    {
      public:
        RST (void);
        RST (const RST &msg);
        ~RST (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);

        Message *encode (void);

        InetAddr ip;
        Byte VerTOC;
        Byte Code;
        Word MsgID;
    };

    CoAPPeer(EventTask *host, ReqIface *d);
    ~CoAPPeer(void);

    Sint16 identify (Message *msg);

  protected:
  private:
};



#endif
