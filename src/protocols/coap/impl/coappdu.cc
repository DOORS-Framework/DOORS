#include "doors/udp.h"
#include "coappdu.hh"
#include <iostream>

// Include some implementations 
#include "coappdu.i"

Message* CoAPPeer :: CON :: encode (void)
{
    Byte zero = 0;

    UdpSAP::Dtreq *dtreq = new UdpSAP::Dtreq;

    dtreq->data.putLast(VerTOC);
    dtreq->data.putLast(Code);
    dtreq->data.putLast((Byte)(MsgID >> 8));
    dtreq->data.putLast((Byte)MsgID);
    //dtreq->data.combine (Options);
    // alternative for using comibne
    int opLength = Options.length();
    for (int i = 0; i < opLength; i++){
        dtreq->data.putLast(Options.getFirst());

    }
    // hopefully it works
    if (Payload.length() != 0)
      dtreq->data.combine (Payload);


    dtreq->dest = ip;

    return dtreq;
}


// remember that CON can have a piggybacked ACK
Uint16 CoAPPeer :: CON :: decode (Message* msg)
{
    int oc;
    int optionLength = 0;
    Byte temp;
    Word NetMsgID = 0;
    // UdpSAP::Dtind provides the whole UDP PDU in its 'data' attribute
    Frame frame = ((UdpSAP::Dtind *) msg)->data;

    ip = ((UdpSAP::Dtind *) msg)->source;

    VerTOC = frame.getFirst();
    Code = frame.getFirst();
    temp = frame.getFirst();
    MsgID = (temp << 8) | frame.getFirst();

    oc = VerTOC & 0x0f;

    for (int i = 0; i < oc; i++) {
      temp = frame.read(optionLength++) & 15;

      if (temp < 15) {
	optionLength += temp;
      } else {	
	temp = frame.read(optionLength++);
	if (temp < 255) {
	  optionLength += (temp + 15);
	} else {
	  temp = frame.read(optionLength++);
	  optionLength += (temp + 270);
	}
      }
    }

    Options = frame.split ((FrameSize)optionLength);

    Payload = frame;
    return 0;
}



Message* CoAPPeer :: ACK :: encode (void)
{
    Byte zero = 0;
    // Create a new udp packet to dtreq message
    UdpSAP::Dtreq *dtreq = new UdpSAP::Dtreq;

    // MsgID, lower part (google says type casting should work for this)
    dtreq->data.putFirst((Byte)MsgID);

    // MsgID, high part
    dtreq->data.putFirst((Byte)(MsgID >> 8));

    dtreq->data.putFirst(Code);

    dtreq->data.putFirst(VerTOC);

    dtreq->dest = ip;
    //std::cout<<"<---------- ACK encoded ---------->"<<std::endl;

    return dtreq;
}

Uint16 CoAPPeer :: ACK :: decode (Message* msg)
{
    Byte temp;
    MsgID = 0;
    // UdpSAP::Dtind provides the whole UDP PDU in its 'data' attribute
    Frame frame = ((UdpSAP::Dtind *) msg)->data;
    ip = ((UdpSAP::Dtind *) msg)->source;


    VerTOC = frame.getFirst();
    Code = frame.getFirst();
    temp = frame.getFirst();
    MsgID = (temp << 8) | frame.getFirst();
    //std::cout<<"<---------- ACK decoded ---------->"<<std::endl;
    return 5; // Return number of bytes decoded from dtind payload
}


Message* CoAPPeer :: RST :: encode (void)
{
    Byte zero = 0;
    // Create a new udp packet to dtreq message
    UdpSAP::Dtreq *dtreq = new UdpSAP::Dtreq;

    dtreq->data.putLast(VerTOC);
    dtreq->data.putLast(Code);
    // MsgID, high part
    dtreq->data.putLast((Byte)(MsgID >> 8));

    // MsgID, lower part (google says type casting should work for this)
    dtreq->data.putLast((Byte)MsgID);

    dtreq->dest = ip;
    //std::cout<<"<---------- RST encoded ---------->"<<std::endl;

    return dtreq;
}

Uint16 CoAPPeer :: RST :: decode (Message* msg)
{
    Byte temp;
    MsgID = 0;
    // UdpSAP::Dtind provides the whole UDP PDU in its 'data' attribute
    Frame frame = ((UdpSAP::Dtind *) msg)->data;
    ip = ((UdpSAP::Dtind *) msg)->source;


    VerTOC = frame.getFirst();
    Code = frame.getFirst();
    temp = frame.getFirst();
    MsgID = (temp << 8) | frame.getFirst(); // make sure that works!
    //std::cout<<"<---------- RST decoded ---------->"<<std::endl;

    return 5; // Return number of bytes decoded from dtind payload
}

// Identifies the received PDU
Sint16 
CoAPPeer :: identify (Message *msg) 
{
    // //std::cout<<"<---------- Identify in motion ---------->"<<std::endl;
    // int type = (((UdpSAP::Dtind *) msg)->data.read(0) & 0x30) >> 4;




    // int code = ((UdpSAP::Dtind *) msg)->data.read(1);

    // // check if the ACK is piggybacked, if yes, it's a CON
    // if ((type == ack) && (code == RES_CODE_CONTENT))
    // {
    //     type = con;
    // }
    // //std::cout<<"<--------------------------------------->"<<std::endl;

    // //std::cout << "type check (1- CON, 2 - ANK, 3 - RST)..." << type <<std::endl;
    // //std::cout << "code check..." << code << std::endl;
    // //std::cout<<"<--------------------------------------->"<<std::endl;

    // return type;
  UdpSAP::Dtind *downmsg = (UdpSAP::Dtind *) msg;
  Byte coapfield = downmsg->data.read(0);
  Byte coapmsgtype = coapfield & 0x30;
  switch (coapmsgtype) {
  case (0):
    return CoAPPeer::con; 
  // case (16):
  //   return CoAPPeer::non;
  case (32):
    return CoAPPeer::ack;
  case (48):
    return CoAPPeer::rst;
  }
  return other;

}



