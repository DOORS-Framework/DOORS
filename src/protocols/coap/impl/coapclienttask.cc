#include <iostream>
#include <fstream>

#include <doors/obl_dcl.h>
#include "coapclienttask.hh"

using namespace std;

CoAPclientTask :: CoAPclientTask (std::string name, Scheduler* s, CoAPClientSM* sm)
    : PTask(name, s, sm),
      up(this),           // Connect this task to outgoing SAP
      peer(this, &down),  // Peer this task to other CoAP peer using 'down' UDP
      down(this, &peer),  // Connect the peer to UDP task
      retries_(0),        // Number of send retries
      msgid_ (0),
      token_ (0),
      last_msgid (0),
      last_token (0),

      //sent_data_(0),      // Used for retries
      timer_ (this, OTime (RESPONSE_TIMEOUT))  // Send timer
{

    // Set these
    STATIC_SYMBOL(up, this);
    STATIC_SYMBOL(down, this);
    STATIC_SYMBOL(peer, this);
    // These are optional but give additional information to LEMon & DEMon
    STATIC_SYMBOL(retries_, this);
    STATIC_SYMBOL(timer_, this);
}

CoAPclientTask :: ~CoAPclientTask() {

    // Delete symbols from the hierarchy
    DELETE_SYMBOL(&up);
    DELETE_SYMBOL(&down);
    DELETE_SYMBOL(&peer);
    DELETE_SYMBOL(&retries_);
    DELETE_SYMBOL(&timer_);
}

Word CoAPclientTask :: getMsgID(){
    return msgid_;
}

Word CoAPclientTask :: newMsgID(){
    msgid_++;
    return msgid_;
}

Byte CoAPclientTask :: getToken(){
    return token_;
}

Byte CoAPclientTask :: newToken(){
    token_++;
    return token_;
}

Word CoAPclientTask :: lastMsgID(){

    return last_msgid;
}

Byte CoAPclientTask :: lastToken(){
    return last_token;
}

void CoAPclientTask :: sendACK (InetAddr ipaddr) {

    // Create message for CoAP peer
    CoAPPeer :: ACK *ack =  (CoAPPeer :: ACK *) peer.create (CoAPPeer :: ack);

    if (!ack) {
        return;
    }

    ack->VerTOC = bitEncode(VER, TYPE_ACK, 0); // no options
    ack->Code = 0;
    ack->MsgID = newMsgID();

    // Destination address
    ack->ip = ipaddr;

    peer.putMessage (ack);

}

void CoAPclientTask :: sendRST (InetAddr ipaddr) {

    // Create message for CoAP peer
    CoAPPeer :: RST *rst = (CoAPPeer :: RST *) peer.create (CoAPPeer :: rst);

    if (!rst) {
        return;
    }

    rst->VerTOC = bitEncode(VER, TYPE_RST, 0); // no options
    rst->Code = 0;
    rst->MsgID = newMsgID(); // that means we forget the last one

    // Destination address
    rst->ip = ipaddr;
    peer.putMessage (rst);
}


//
// **************** Idle state ****************
//


bool CoAPclientTask :: idle_get (Message* msg) {

    CoAPSAP :: Get *get = (CoAPSAP :: Get *) msg;
    CoAPPeer :: CON *con =  (CoAPPeer :: CON *) peer.create (CoAPPeer :: con);

    // Reset the flags
    retries_ = 0;

    con->ip = get->IPport;
    con->VerTOC = bitEncode(VER, TYPE_CON, OC_GET);
    con->Code = METHOD_GET;
    con->MsgID = newMsgID();

// in case of retransmission

    retr_ip = get->IPport;
    retr_VerTOC = bitEncode(VER, TYPE_CON, OC_GET);
    retr_Code = METHOD_GET;
    retr_MsgID = getMsgID();

    //////////////////
   // Options
   //////////////////

    // Uri-Path
    if(!(get->filePath.empty()))
    {
        path = get->filePath;
        //delta 0x9
        // Option header
        if(get->filePath.length() < 15) // Option length on 4 bits
        {
            Byte option = OPT_URI_PATH << 4 | get->filePath.length();
            con->Options.putLast(option);
        }
        else // Option length on 12 bits
        {
            con->Options.putLast(OPT_URI_PATH << 4 | 0x0f);
            con->Options.putLast(get->filePath.length() - 15);
        }
        // Option value
        con->Options.putLast((const Byte *)get->filePath.data(),
                             get->filePath.length());

    }

    // Token
    // Option header    DELTA                     TOKEN_SIZE = 1B
    Byte option = (OPT_TOKEN - OPT_URI_PATH) << 4 | TOKEN_SIZE;
    con->Options.putLast(option);

    // Option value
    option = newToken();
    con->Options.putLast(option);

    // in case of retransmission
    retr_Options = con->Options.copy();


    peer.putMessage(con);
    // Move to wait state
    state = CoAPClientSM::Wait;
    timer_.start();
    return false;
}

bool CoAPclientTask :: idle_put (Message* msg) {

    CoAPSAP :: Put *put = (CoAPSAP :: Put *) msg;
    CoAPPeer :: CON *con =  (CoAPPeer :: CON *) peer.create (CoAPPeer :: con);

    // Reset the flags
    retries_ = 0;

    con->ip = put->IPport;
    con->VerTOC = bitEncode(VER, TYPE_CON, OC_PUT);
    con->Code = METHOD_PUT;
    con->MsgID = newMsgID();

    // in case of retransmission
        retr_ip = put->IPport;
        retr_VerTOC = bitEncode(VER, TYPE_CON, OC_GET);
        retr_Code = METHOD_GET;
        retr_MsgID = getMsgID();

   //////////////////
   // Options
   //////////////////

    // Uri-Path
    if(!(put->filePath.empty()))
    {
        // putta 0x9
        // Option header
        if(put->filePath.length() < 15) // Option length on 4 bits
        {
            Byte option = OPT_URI_PATH << 4 | put->filePath.length();
            con->Options.putLast(option);
        }
        else // Option length on 12 bits
        {
            con->Options.putLast(OPT_URI_PATH << 4 | 0x0f);
            con->Options.putLast(put->filePath.length() - 15);
        }
        // Option value
        con->Options.putLast((const Byte *)put->filePath.data(),
                             put->filePath.length());
    }

    // Tokens
    // Option header    DELTA                   TOKEN_SIZE = 1B
    Byte option = (OPT_TOKEN - OPT_URI_PATH) << 4 | TOKEN_SIZE;
    con->Options.putLast(option);

    // Option value
    option = newToken();
    con->Options.putLast(option);
    con->Payload = put->payload.copy();


    //in case of retransmisson
    retr_Options = con->Options.copy();
    retr_Payload = con->Payload.copy();
    peer.putMessage(con);

    // Move to wait state
    state = CoAPClientSM::Wait;

    //start timer
    timer_.start ();

    return false;
}

bool CoAPclientTask :: idle_del (Message* msg) {

    CoAPSAP :: Del *del = (CoAPSAP :: Del *) msg;
    CoAPPeer :: CON *con =  (CoAPPeer :: CON *) peer.create (CoAPPeer :: con);

    // Reset the flags
    retries_ = 0;

    con->ip = del->IPport;
    con->VerTOC = bitEncode(VER, TYPE_CON, OC_DEL);
    con->Code = METHOD_DELETE;
    con->MsgID = getMsgID();

    // in case of retransmission

        retr_ip = del->IPport;
        retr_VerTOC = bitEncode(VER, TYPE_CON, OC_GET);
        retr_Code = METHOD_GET;
        retr_MsgID = newMsgID();

    //////////////////
   // Options
   //////////////////

    // Uri-Path
    if(!(del->filePath.empty()))
    {
        //delta 0x9
        // Option header
        if(del->filePath.length() < 15) // Option length on 4 bits
        {
            Byte option = OPT_URI_PATH << 4 | del->filePath.length();
            con->Options.putLast(option);
        }
        else // Option length on 12 bits
        {
            con->Options.putLast(OPT_URI_PATH << 4 | 0x0f);
            con->Options.putLast(del->filePath.length() - 15);
        }
        // Option value
        con->Options.putLast((const Byte *)del->filePath.data(),
                             del->filePath.length());
    }

    // Token
    // Option header    DELTA                     TOKEN_SIZE = 1B
    Byte option = (OPT_TOKEN - OPT_URI_PATH) << 4 | TOKEN_SIZE;
    con->Options.putLast(option);

    // Option value
    option = getToken();
    con->Options.putLast(option);

    //in case of retransmission
    retr_Options = con->Options.copy();

    peer.putMessage(con);

    // Move to wait state
    state = CoAPClientSM::Wait;

    //start timer
    timer_.start ();

    return false;
}

bool CoAPclientTask :: idle_default (Message* msg) {

    // just drop nessage
    return false;
}


//
// **************** wait state ****************
//

bool CoAPclientTask :: wait_default (Message* msg) {

    // just drop nessage
    return false;
}

bool CoAPclientTask :: wait_ACK(Message* msg) {

    // Send message received from the 'upper task' during the wait state.
    // Append this data to the end of the data frame.
    CoAPPeer :: ACK *ack = (CoAPPeer :: ACK *) msg;
    if (getMsgID() == ack->MsgID){

        // should create a message to the user to pass him the code.
        last_msgid = getMsgID();
        last_token = getToken();
        timer_.stop();
        state = CoAPClientSM::Idle;

    }
    else {

        sendRST(ack->ip);

        CoAPSAP :: Error* error = (CoAPSAP :: Error *) up.create (CoAPSAP :: error);
        error->error_code = ERR_GOTRST;
        up.putMessage(error);

        state = CoAPClientSM::Idle;
    }
    return false;
}

bool CoAPclientTask :: wait_RST(Message* msg) {

    CoAPSAP :: Error* error = (CoAPSAP :: Error *) up.create (CoAPSAP :: error);
    error->error_code = ERR_SENTRST;
    up.putMessage(error);

    state = CoAPClientSM::Idle;

    return false;
}

bool CoAPclientTask :: wait_CON(Message* msg) {
    // we do simplest case for now so this should come with a piggybacked ACK, thus msgid is the same
    CoAPPeer :: CON *con = (CoAPPeer :: CON *) msg;
    CoAPSAP :: Confirm *conf =  (CoAPSAP :: Confirm *) up.create (CoAPSAP :: confirm);
    conf->confirm_code = CONF_RECV;
    conf->data = con->Payload;

    up.putMessage(conf); // since we dont really know what the upper layer is we handle the file/dir listing
    // handle data
    timer_.stop();
    handleData(conf->data);


    state = CoAPClientSM::Idle;

    return false;
}

bool CoAPclientTask :: wait_ACKTimeout(Message* msg) {

    // Increase the retry count
    retries_++;
    //std::cout << "Timeout" << std::endl;

    if (retries_ > MAX_RETRANSMIT) {
        OTime timeVar(RESPONSE_TIMEOUT);
        timer_.stop();
        timer_.setTimeout(timeVar);
        // Give up; reset the attributes and go back to idle state
        retries_ = 0;
        state = CoAPClientSM::Idle;
        //std::cout << "<----- No more timeouts, too bad ------>" << std::endl;
        //timer_.setTimeout(timeVar);
        timer_.stop();



    } else {
        OTime timeVar(++retries_ * RESPONSE_TIMEOUT * 2);
        CoAPPeer :: CON *con =  (CoAPPeer :: CON *) peer.create (CoAPPeer :: con);
        timer_.setTimeout(timeVar);
        con->ip = retr_ip;
        con->VerTOC = retr_VerTOC;
        con->Code = retr_Code;
        con->MsgID = retr_MsgID;
        con->Options = retr_Options.copy();
        con->Payload = retr_Payload.copy();

        peer.putMessage(con);
        //std::cout << "<----- Retransmitted ----->" << std::endl;
        timer_.setTimeout(timeVar);
        timer_.start();

    }

    return false;


}


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//-*-*-*-*        HELP ME FUNCTIONS       -*-*-*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*


void CoAPclientTask :: handleData(Frame data){
    if(data.read(data.length()-1) == ','){ // its a dir listing
        for (int i=0; i<data.length(); i++){
            if (data.read(0) == ',') std::cout << std::endl;
            else std::cout << data.getFirst();
        }
    }
    else{ //its a file
        std::ofstream file;
        char c;
        file.open (path.c_str());

        for(int i=0; i<data.length()-1; i++){
            c = data.read(i);
            file << c;
        }
        file.close();
    }
}

Byte CoAPclientTask::bitEncode(int Ver, int T, int OC){
return (Ver << 6) + (T << 4) + OC;

}





