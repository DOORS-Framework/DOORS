#include <doors/obl_dcl.h>
#include "coapservertask.hh"

CoAPserverTask :: CoAPserverTask (std::string name, Scheduler* s, CoAPServerSM* sm)
  : PTask(name, s, sm), 
    up(this),
    peer(this, &down),
    down(this, &peer),
    msgID_(0),
    retries_(0),                 // Number of send retries
    options_(),
    timer_ (this, OTime (RESPONSE_TIMEOUT))    // Set timer
{


  STATIC_SYMBOL(up, this);
  STATIC_SYMBOL(down, this);
  STATIC_SYMBOL(peer, this);

  //For information to LEMon
  STATIC_SYMBOL(retries_, this);
  STATIC_SYMBOL(timer_, this);
}

CoAPserverTask :: ~CoAPserverTask() {

  DELETE_SYMBOL(&up);
  DELETE_SYMBOL(&down);
  DELETE_SYMBOL(&peer);

  DELETE_SYMBOL(&retries_);
  DELETE_SYMBOL(&timer_);
}

//
// **************** Helper Functions ****************
//

void CoAPserverTask::handle_request(Byte requestCode, Frame& payload)
{
  Frame data, pathfrm;
  FrameSize stringbytesread;
  std::string path;
  pathfrm = options_.getOption(OPT_URI_PATH);
  stringbytesread = pathfrm.getString (path, pathfrm.length());
  std::string clientstring = clientAddr_.addrToString ();
  std::string resource = path + "/" + clientstring;
  switch (requestCode)
    {
       case METHOD_GET:
      if (get_data(path, data))
         sendACK(RES_CODE_CONTENT, data);
      else
         sendACK(RES_CODE_NOT_FOUND, data);
      break;

       case METHOD_PUT:
      if (put_data(resource, payload))
         sendPUTACK(RES_CODE_CHANGED);
      else
         sendACK(RES_CODE_UNAUTHORIZED, data);
      break;

       case METHOD_POST:
      if (post_data(resource, payload))
	sendPOSTACK(RES_CODE_CREATED, resource);
      else
         sendACK(RES_CODE_UNAUTHORIZED, data);
      break;

       case METHOD_DELETE:
      if (del_file(path))
         sendACK(RES_CODE_DELETED, data);
      else
         sendACK(RES_CODE_NOT_FOUND, data);
      break;

       default: // If the request is any other than get,put or del
      sendACK(RES_CODE_NOT_IMPLEMENTED, data);
      break;
    };

}

bool CoAPserverTask::get_data (std::string path, Frame& data)
{
    // If the path ends with a '/', we return the directory listing
    if(path[path.length()-1] == '/')
    {
        DIR* dir;
        struct dirent* listing;
        dir= opendir(path.c_str());
        if(dir)
        {
            while(listing = readdir(dir))
            {
                data.putLast((const Byte*) listing->d_name, strlen(listing->d_name));
                data.putLast(',');
            }
            closedir(dir);
        }
        else // returning false if opening directory wasn't possible
            return false;
    }
    else // read the requested file
    {
        std::ifstream ifile(path.c_str());
        if(ifile.is_open())
        {
            while(!ifile.eof())
            {
                data.putLast((Byte) ifile.get());
            }
            ifile.close();
        }
        else // Return false if unable to open the requested file
            return false;

    }
    return true;
}

// Sends a piggy-backed response to the client
void CoAPserverTask::sendACK(Byte responseCode, Frame& payload)
{
  CoAPPeer::CON *pdu = (CoAPPeer::CON *) peer.create(CoAPPeer::con);

  // Creating the header

  pdu->VerTOC = (VER << 6) + (TYPE_ACK << 4) + 1;


  // Storing the response code
  pdu->Code = responseCode;

  // Storing the MsgID
  pdu->MsgID= msgID_;


  Byte option = (OPT_TOKEN << 4) | TOKEN_SIZE;

  pdu->Options.putLast(option);
  //  pdu->Options.putLast((const Byte*) options_.getOption(OPT_TOKEN).data(),options_.getOption(OPT_TOKEN).length()); //Make sure this works
  pdu->Payload.combine(payload);

  pdu->ip = clientAddr_;

  peer.putMessage(pdu);

  //Store the last pdu values for re transmitting if required
  retr_ip = pdu->ip;
  retr_VerTOC = pdu->VerTOC;
  retr_MsgID = pdu->MsgID;
  retr_Code = pdu->Code;
  retr_Options = pdu->Options.copy();
  retr_Payload = pdu->Payload.copy();


}

void CoAPserverTask::sendPOSTACK(Byte responseCode, std::string location)
{
  CoAPPeer::CON *pdu = (CoAPPeer::CON *) peer.create(CoAPPeer::con);

 
  // Storing the response code
  pdu->Code = responseCode;

  // Storing the MsgID
  pdu->MsgID= msgID_;

  // Let's get the options filled: Content Type, ETAG, LocationPath, Token
  Byte optcount = 0;
  Uint32 optlen = 0;
  Byte optbyte;

  //  OPT_CONTENT_TYPE;
  Frame contentval = options_.getOption (OPT_CONTENT_TYPE);
  Byte contentlen = (Byte) contentval.length();
   if (contentlen < 15) 
    optbyte = (OPT_CONTENT_TYPE << 4) | contentlen;
  
  pdu->Options.putLast (optbyte);
  pdu->Options.combine(contentval);

  optcount++;

  // OPT_ETAG;
  Frame etagval = options_.getOption (OPT_ETAG);
  Byte etaglen = (Byte) etagval.length();
  if (etaglen < 15) 
    optbyte = ((OPT_ETAG - OPT_CONTENT_TYPE) << 4) | etaglen;
  
  pdu->Options.putLast (optbyte);
  pdu->Options.combine (etagval);
  
  optcount++;

  // OPT_LOCATION_PATH;

  std::string rdopt = "rd";
  std::string idopt = clientAddr_.addrToString ();
  Byte rdoptlen = rdopt.length ();
  Uint32 idoptlen = idopt.length ();

  optbyte = ((OPT_LOCATION_PATH - OPT_ETAG) << 4) | 2;
  pdu->Options.putLast (optbyte); 
  pdu->Options.putLast ((const Byte*) rdopt.data(), rdoptlen);
  
  optcount++;

  if (idoptlen < 15) {
    Byte idoptlenbyte = (Byte) idoptlen;
     pdu->Options.putLast (idoptlenbyte);
  } else {
    optbyte = 15;
    pdu->Options.putLast (optbyte);
    if (idoptlen < 270) {
      optbyte = (Byte) idoptlen - 15;
      pdu->Options.putLast (optbyte);
    }
  }
      
  pdu->Options.putLast ((const Byte*) idopt.data(), idoptlen);
  
  optcount++;

  // Then the token 

  Frame tokenval = options_.getOption(OPT_TOKEN);
  Byte toklen = (Byte) tokenval.length();
  if (toklen < 15) 
    optbyte = ((OPT_TOKEN - OPT_LOCATION_PATH) << 4) | toklen;
  
  pdu->Options.putLast (optbyte);
  pdu->Options.combine (tokenval);
  
  optcount++;
  
  pdu->ip = clientAddr_;

 // Creating the header

  pdu->VerTOC = BYTE_VERSION | BYTE_ACK | optcount;


  peer.putMessage(pdu);

 }

void CoAPserverTask::sendPUTACK(Byte responseCode)
{
  CoAPPeer::CON *pdu = (CoAPPeer::CON *) peer.create(CoAPPeer::con);

  // Creating the header

  pdu->VerTOC = (VER << 6) + (TYPE_ACK << 4) + 1;


  // Storing the response code
  pdu->Code = responseCode;

  // Storing the MsgID
  pdu->MsgID= msgID_;

  // Then the token option

  //  std::string tokenval = options_.getOption(OPT_TOKEN);
  std::string tokenval;
  Byte toklen = (Byte) tokenval.length();
  Byte tokopt = ((OPT_TOKEN - OPT_LOCATION_PATH) << 4) | toklen;

  pdu->Options.putLast(tokopt);
  pdu->Options.putLast((const Byte*) tokenval.data(), toklen);

  pdu->ip = clientAddr_;

  peer.putMessage(pdu);

  //Store the last pdu values for re transmitting if required
  retr_ip = pdu->ip;
  retr_VerTOC = pdu->VerTOC;
  retr_MsgID = pdu->MsgID;
  retr_Code = pdu->Code;
  retr_Options = pdu->Options.copy();

}

bool CoAPserverTask::put_data(std::string path, Frame& data)
{
   std::ofstream oFile;

   //Open the file , if it exists overwrite it
   oFile.open(path.c_str(), std::ios_base::trunc);
   if(oFile.is_open())
   {
       std::string fileData = "";

       for (int i = 0; i < data.length(); ++i)
       {
           fileData += data.read(i);
       }

       oFile << fileData << std::endl;

       oFile.close();

       return true;

   }
   else // If the file could not be opened
       return false;

}

bool CoAPserverTask::post_data(std::string path, Frame& data)
{
   std::ofstream oFile;

   //Open the file , if it exists overwrite it
   oFile.open(path.c_str(), std::ios_base::trunc);
   if(oFile.is_open())
   {
       std::string fileData = "";

       for (int i = 0; i < data.length(); ++i)
       {
           fileData += data.read(i);
       }

       oFile << fileData << std::endl;

       oFile.close();

       return true;

   }
   else // If the file could not be opened
       return false;

}

bool CoAPserverTask::del_file(std::string path)
{
   std::string system_command = "rm " + path;

   if(system(system_command.c_str())) // if the file is succesfully deleted
       return true;
   else
       return false;

}


//
// **************** Listen state ****************
//

bool CoAPserverTask :: listen_default (Message* msg) {

   // If anything else than CON is recieved
  // Ignore the Message
  return false;
}


bool CoAPserverTask :: listen_CON (Message* msg)
{


    // Create PDU with the msg recieved
    CoAPPeer::CON *pdu = (CoAPPeer::CON *)msg;

    //Store the msgID for the response
    msgID_ = pdu->MsgID;
    // Store the host address and port of client for replying
    clientAddr_ = pdu->ip;
    //Read the options from the recieved Msg
    options_.readOptions(pdu->Options,(pdu->VerTOC & 0x0f));

    handle_request(pdu->Code,pdu->Payload);

    // Start the wait state timer
//    timer_.start();

    // Move to wait state
    //state = CoAPServerSM::Wait; // Not Required yet since only piggybacked responses are used for now


  return false;
}

//
// **************** Wait state ****************
//

bool CoAPserverTask :: wait_default (Message* msg) {

  // Ignore the message recieved
  return false; 
}

bool CoAPserverTask::wait_RST (Message* msg)
{

    state = CoAPServerSM::Listen; // Move to Listen State

    return false;
}

bool CoAPserverTask::wait_ACKtimeout (Message* msg)
{
    // Increase the retry count
    retries_++;

    if (retries_ > MAX_RETRANSMIT)
    {
        // Give up; reset the attributes and go back to idle state
        OTime timeVar(RESPONSE_TIMEOUT);
        timer_.stop();
        timer_.setTimeout(timeVar);
        retries_ = 0;
        state = CoAPServerSM::Listen;
    } else {

        CoAPPeer :: CON *con =  (CoAPPeer :: CON *) peer.create (CoAPPeer :: con);
        con->ip = retr_ip;
        con->VerTOC = retr_VerTOC;
        con->Code = retr_Code;
        con->MsgID = retr_MsgID;
        con->Options = retr_Options;
        con->Payload = retr_Payload;
        //Set the timer with new value and start it again
        OTime timeVar(retries_ * RESPONSE_TIMEOUT * 2);
        timer_.stop();
        timer_.setTimeout(timeVar);
        //timer_.start();

        peer.putMessage(con);

    }

    return false;

}

bool CoAPserverTask::wait_ACK (Message* msg)
{
    CoAPPeer :: ACK *con = (CoAPPeer :: ACK *) msg;

    //Check if the message ID in the ACK is the same as last sent PDU
    if(con->MsgID == retr_MsgID)
    {

         state = CoAPServerSM::Listen;
    }

    return false;
}
