
#ifndef COAPTASK_H
#define COAPTASK_H

#include <string>
#include <doors/ptb.h>
#include <doors/udp.h>
#include <doors/ptask.h>
#include <doors/inetaddr.h>
#include <doors/timer.h>
#include "coapsap.hh"
#include "coapclient.hh"
#include "coappdu.hh"
#include "coapconstants.hh"
#include <doors/otime.h>


#define MAX_RETRIES 5

class CoAPClientSM;

class CoAPclientTask : public PTask {

public:
    CoAPclientTask (std::string name, Scheduler* s, CoAPClientSM* sm);

    virtual ~CoAPclientTask();

    UdpSAP :: User down;      // SAP to UDP task
    CoAPSAP :: Provider up; // SAP to a provider task
    CoAPPeer peer;   // SAP to CoAP peer

    // Idle state
    bool idle_default (Message* msg);
    bool idle_get (Message* msg);
    bool idle_put (Message* msg);
    bool idle_del (Message* msg);

    // Wait state
    bool wait_default (Message* msg);
    bool wait_ACK (Message* msg);
    bool wait_RST (Message* msg);
    bool wait_CON (Message* msg);
    bool wait_ACKTimeout(Message* msg);

    // Helper functions
private:

      void sendACK (InetAddr ipaddr);
      void sendRST (InetAddr ipaddr);
      Word newMsgID();
      Word getMsgID();
      Byte getToken();
      Byte newToken();
      Word lastMsgID();
      Byte lastToken();
      void handleData(Frame data);
      Byte bitEncode(int Ver, int T, int OC);
      std::string path;
    //  void receiveDATA (Message* msg);
    //  void receiveACK (Message* msg);

    // Class variables
//--> stuff for retransmission
    int retr_type;
    InetAddr retr_ip;
    Byte retr_VerTOC;
    Word retr_MsgID;
    Byte retr_Code;
    Frame retr_Options;
    Frame retr_Payload;

    int retries_;
    Word msgid_;
    Byte token_;
    Frame data_;
    InetAddr addr_;
    char sent_data_;
    Timer timer_;
    Word last_msgid;
    Byte last_token;

};

#endif

