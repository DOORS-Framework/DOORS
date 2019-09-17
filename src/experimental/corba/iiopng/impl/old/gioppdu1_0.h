

//
//File: gioppdu1_0.h
//
//Date: Thu Aug 23 13:29:18 2001
//
//Commentary:
//  This is a machine generated file produced by 
// compiler. Editing is not recommended.
//
//Description:
//  This file declares a PDU classes of GIOP_1_0_Peer
//  interface.
//


#ifndef _gioppdu1_0_h_
#define _gioppdu1_0_h_


#include <doors/hsi.h>
#include <doors/ptb.h>
#include <doors/giop1_0.h>



//
//Class: GIOP_1_0_Peer
//
//Description:
//   This class defines a peer interface for a task.
//   Inside this class all PDUs are defined as well.
//

class GIOP_1_0_Peer : public Peer {
  public:
    enum State { request = 1, reply, cancelrequest, locaterequest, locatereply, closeconnection, messageerror, other };

//
//Class: Request
//
//Inherited: GIOP:: GIOP_1_0_Msg
//
//Description:
//  This is one of the PDUs this peer contains.
//

    class Request : public GIOP:: GIOP_1_0_Msg {
      public:
        Request (void);
        Request (const Request &msg);
        ~Request (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);
        Message *encode (void);

        GIOP::RequestHeader_1_0 reqhdr;
        Frame body;
    };

//
//Class: Reply
//
//Inherited: GIOP:: GIOP_1_0_Msg
//
//Description:
//  This is one of the PDUs this peer contains.
//

    class Reply : public GIOP:: GIOP_1_0_Msg {
      public:
        Reply (void);
        Reply (const Reply &msg);
        ~Reply (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);
        Message *encode (void);

        GIOP::ReplyHeader_1_0 rephdr;
        Frame body;
    };

//
//Class: CancelRequest
//
//Inherited: GIOP:: GIOP_1_0_Msg
//
//Description:
//  This is one of the PDUs this peer contains.
//

    class CancelRequest : public GIOP:: GIOP_1_0_Msg {
      public:
        CancelRequest (void);
        CancelRequest (const CancelRequest &msg);
        ~CancelRequest (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);
        Message *encode (void);

        GIOP::CancelRequestHeader reqhdr;
    };

//
//Class: LocateRequest
//
//Inherited: GIOP:: GIOP_1_0_Msg
//
//Description:
//  This is one of the PDUs this peer contains.
//

    class LocateRequest : public GIOP:: GIOP_1_0_Msg {
      public:
        LocateRequest (void);
        LocateRequest (const LocateRequest &msg);
        ~LocateRequest (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);
        Message *encode (void);

        GIOP::LocateRequestHeader_1_0 reqhdr;
    };

//
//Class: LocateReply
//
//Inherited: GIOP:: GIOP_1_0_Msg
//
//Description:
//  This is one of the PDUs this peer contains.
//

    class LocateReply : public GIOP:: GIOP_1_0_Msg {
      public:
        LocateReply (void);
        LocateReply (const LocateReply &msg);
        ~LocateReply (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);
        Message *encode (void);

        GIOP::LocateReplyHeader_1_0 rephdr;
        Frame body;
    };

//
//Class: CloseConnection
//
//Inherited: GIOP:: GIOP_1_0_Msg
//
//Description:
//  This is one of the PDUs this peer contains.
//

    class CloseConnection : public GIOP:: GIOP_1_0_Msg {
      public:
        CloseConnection (void);
        CloseConnection (const CloseConnection &msg);
        ~CloseConnection (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);
        Message *encode (void);

        ;
    };

//
//Class: MessageError
//
//Inherited: GIOP:: GIOP_1_0_Msg
//
//Description:
//  This is one of the PDUs this peer contains.
//

    class MessageError : public GIOP:: GIOP_1_0_Msg {
      public:
        MessageError (void);
        MessageError (const MessageError &msg);
        ~MessageError (void);

        Message *clone (void) const;

        MessageType getType (void) const;

        Uint16 decode (Message *msg);
        Message *encode (void);

        ;
    };

    GIOP_1_0_Peer (EventTask *host, ReqIface *d);
    ~GIOP_1_0_Peer (void);

    Sint16 identify (Message *msg);

  protected:
  private:
};


#endif
