
#ifndef _coapsap_h_
#define _coapsap_h_


#include <doors/ptb.h>
#include <string>

class Report;
class SrcDesc;

class CoAPSAP
{
  public:

    class User : public ReqIface
    {
      public:
        User(EventTask *t, Peer *p=0);
        ~User(void);
    };

    class Provider : public IndIface
    {
      public:
        Provider(EventTask *t);
        ~Provider(void);
    };

    enum ReqType { get = 1, put, del };

    class Get : public Message
    {
      public:
        Get (void);
        ~Get (void);

        Get (const Get &msg);
        Message *clone (void) const;

        MessageType getType (void) const;

        InetAddr IPport;
        std::string filePath;
    };

    class Put : public Message
    {
      public:
        Put (void);
        ~Put (void);

        Put (const Put &msg);
        Message *clone (void) const;

        MessageType getType (void) const;

        InetAddr IPport;
        std::string filePath;
        Frame payload;
    };

    class Del : public Message
    {
      public:
        Del (void);
        ~Del (void);

        Del (const Del &msg);
        Message *clone (void) const;

        MessageType getType (void) const;

        InetAddr IPport;
        std::string filePath;
    };

    enum IndType { confirm = 1, error };

    class Confirm : public Message
    {
      public:
        Confirm (void);
        ~Confirm (void);

        Confirm (const Confirm &msg);
        Message *clone (void) const;

        MessageType getType (void) const;

        int confirm_code;
        Frame data;
    };

    class Error : public Message
    {
      public:
        Error (void);
        ~Error (void);

        Error (const Error &msg);
        Message *clone (void) const;

        MessageType getType (void) const;

        int error_code;
    };

  protected:
  private:
};

inline MessageType CoAPSAP :: Get :: getType (void) const
{
  return get;
}

inline MessageType CoAPSAP :: Put :: getType (void) const
{
  return put;
}

inline MessageType CoAPSAP :: Del :: getType (void) const
{
  return del;
}

inline MessageType CoAPSAP :: Confirm :: getType (void) const
{
  return confirm;
}

inline MessageType CoAPSAP :: Error :: getType (void) const
{
  return error;
}



#endif
