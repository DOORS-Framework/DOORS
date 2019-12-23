#ifndef COAPSERVERTASK_H
#define COAPSERVERTASK_H

#include <string>

#include <doors/ptb.h>
#include <doors/udp.h>
#include <doors/ptask.h>
#include <doors/inetaddr.h>
#include <doors/timer.h>

#include <map>
#include <fstream>
#include <unistd.h>
#include <dirent.h>

#include "coapsap.hh"
#include "coapserver.hh"
#include "coappdu.hh"
//#include "bitmap.cc"
#include "coapconstants.hh"
#include "options.hh"


class CoAPServerSM;

class CoAPserverTask : public PTask {
  
public:
  CoAPserverTask (std::string name, Scheduler* s, CoAPServerSM* sm);

  virtual ~CoAPserverTask();

  UdpSAP :: User down;      // SAP to UDP task
  CoAPSAP :: Provider up; // SAP to a user task
  CoAPPeer peer;          // SAP to CoAP peer

  // Listen state
  bool listen_default (Message* msg); // Handles recieving RST or ACK in listen state
  bool listen_CON (Message* msg); // Handles recieving CON

  // Wait state
  bool wait_default (Message* msg); // Handles recieving CON in wait state
  bool wait_ACK (Message* msg);
  bool wait_RST (Message* msg);
  bool wait_ACKtimeout (Message* msg);

  // Helper functions
private:

  // Handles the incoming requests
  void handle_request(Byte requestCode, Frame& payload);
  // Retrieves the requested file or the requested directory listing
  bool get_data (std::string path, Frame& data);
  // Overwrites the received file in the directory
  bool put_data(std::string path, Frame& data);
  // Creates the received file in the directory
  bool post_data(std::string path, Frame& data);
  // Deletes the requested file
  bool del_file(std::string path);

  // Sends a ACK Message to client including the ACK and Response
  void sendACK (Byte responseCode, Frame& payload);
  void sendPOSTACK (Byte responseCode, std::string location);
  void sendPUTACK (Byte responseCode);

  
  // Class variables
  
  Uint16 retries_;
  Word msgID_;
  InetAddr clientAddr_;
  Options options_;
  Timer timer_;
  Frame Token;
  Byte Etag;
  Frame uripath;

  // Variables for retransmission

  InetAddr retr_ip;
  Byte retr_VerTOC;
  Word retr_MsgID;
  Byte retr_Code;
  Frame retr_Options;
  Frame retr_Payload;

};

#endif

