// Editor-info: -*- C++ -*-
// 
//  Copyright 1999 
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
//  THIS SOFTWARE IS PROVIDED BY TAMPERE UNIVERSITY OF TECHNOLOGY AND 
//  CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
//  BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//  FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY 
//  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// $Log: linkconn.C,v $
// Revision 1.8  2003/11/14 10:20:24  bilhanan
// CloseAll message handling and general cleanup.
//
// Revision 1.7  2003/08/12 07:25:01  bilhanan
// typo
//
// Revision 1.6  2003/08/11 23:47:29  bilhanan
// const char* in constructor -> std::string
//
// Revision 1.5  2002/07/23 16:38:31  ik
// Od{Boolean,True,False} -> {bool,true,false}
//
// Revision 1.4  2002/07/23 16:34:11  hartmanj
//
// Development
//
// Revision 1.3  2002/07/12 08:47:43  bilhanan
// linkrandpdu unneeded and therefore removed.
//
// Revision 1.2  2002/07/12 06:31:46  bilhanan
// Licence clause and Log keyword added
//

#include <string>
#include <doors/linksapbase.h>
#include <doors/linkconn.h>
#include <doors/linkcsm.h>
#include <doors/linkent.h>
#include <doors/link.h>

LinkTask::LinkTask(std::string n, Uint16 id, LinkEntity *e, LinkTaskSM *lsm) 
    : PTask (n, e->getScheduler (), lsm), 
      up (e->up),
      down (e->down),
      peer (e->peer),
      entity (this),
      retTimer (this, OTime (15)), 
      crespTimer (this, OTime (30)), 
      cid (id), 
      seqNumber (0), 
      sendNumber (0), 
      recNumber (0) 
{
    STATIC_SYMBOL (cid, this);
    STATIC_SYMBOL (seqNumber, this);
    STATIC_SYMBOL (sendNumber, this);
    STATIC_SYMBOL (recNumber, this);
    STATIC_SYMBOL (retTimer, this);
    STATIC_SYMBOL (crespTimer, this);
}


LinkTask :: ~LinkTask (void) {
    
    DELETE_SYMBOL (this);
}


bool LinkTask :: closed_Creq (Message *msg) {

    LinkSAP::Creq *lcr = (LinkSAP::Creq *) msg;

    LinkPeer::CR *cr = (LinkPeer::CR *) peer.create (LinkPeer::cr);
    cr->data = lcr->data;
    cid = cr->cid = lcr->cid;
    peer.putMessage (cr);

    crespTimer.start ();

    state = LinkTaskSM::wfCC;
    return false;
}


bool LinkTask :: closed_CR (Message *msg) {

    LinkPeer::CR *cr = (LinkPeer::CR *) msg;

    LinkSAP::Cind *lci = (LinkSAP::Cind *) up.create (LinkSAP::cind);
    cid = lci->cid = cr->cid;
    lci->data = cr->data;

    up.putMessage (lci);

    crespTimer.start ();

    state = LinkTaskSM::wfCresp;
    return false;
}


bool LinkTask :: closed_default (Message *msg) {

    return false;
}


bool LinkTask :: wfCresp_Cresp (Message *msg) {

    LinkSAP::Cresp *lcr = (LinkSAP::Cresp *) msg;
    
    LinkPeer::CC *cc = (LinkPeer::CC *) peer.create (LinkPeer::cc);
    cc->cid = lcr->cid;
    cc->data = lcr->data;

    peer.putMessage (cc);
    crespTimer.stop ();

    state = LinkTaskSM::idle;
    return false;
}


bool LinkTask :: wfCresp_timeout (Message *msg) {

    state = LinkTaskSM::closed;
    delete this;
    return false;
}


bool LinkTask :: wfCresp_default (Message *msg) {

    return false;
}


bool LinkTask :: wfCC_CC (Message *msg) {

    LinkPeer::CC *cc = (LinkPeer::CC *) msg;
    LinkSAP::Cconf *lcc = (LinkSAP::Cconf *) up.create (LinkSAP::cconf);
    lcc->cid = cc->cid;
    lcc->data = cc->data;

    up.putMessage (lcc);
    crespTimer.stop ();

    state = LinkTaskSM::idle;
    return false;
}


bool LinkTask :: wfCC_timeout (Message *msg) {

    state = LinkTaskSM::closed;
    delete this;
    return false;
}


bool LinkTask :: wfCC_default (Message *msg) {

    return false;
}


bool LinkTask :: idle_Dtreq (Message *msg) {

    LinkSAP::Dtreq *dtr = (LinkSAP::Dtreq *) msg;
    LinkPeer::DATA *data = (LinkPeer::DATA *) peer.create (LinkPeer::data);
    data->cid = cid;
    data->seq = seqNumber = sendNumber;

    Frame &queue = dtr->data.copy();
    buffer.add (&queue);
    Frame &frm = buffer.get ()->copy ();
    data->udata = frm;
    delete &frm;
    peer.putMessage (data);

    incSendNumber ();    
    retTimer.start ();
    
    state = LinkTaskSM :: wfACK;
    return false;
}


bool LinkTask :: idle_DATA (Message *msg) {

    LinkPeer::DATA *data = (LinkPeer::DATA *) msg;

    if ((Byte) recNumber == data->seq) {
	LinkSAP::Dtind *dti = (LinkSAP::Dtind *) up.create (LinkSAP::dtind);
	dti->cid = cid;
	dti->data = data->udata;
	up.putMessage (dti);    

	LinkPeer::ACK *ack = (LinkPeer::ACK *) peer.create (LinkPeer::ack);
	ack->cid = cid;
	ack->seq = data->seq;
	peer.putMessage (ack);
	incRecNumber (); 

    } else { 
	if ((Byte) recNumber > data->seq) {
	    LinkPeer::ACK *ack = (LinkPeer::ACK *) peer.create (LinkPeer::ack);
	    ack->seq = data->seq;
	    peer.putMessage (ack);
	}
    }

    return false;
}


bool LinkTask :: idle_Dreq (Message *msg) {

    LinkSAP::Dreq *ldr = (LinkSAP::Dreq *) msg;

    LinkPeer::DR *dr = (LinkPeer::DR *) peer.create (LinkPeer::dr);
    dr->cid = cid;
    peer.putMessage (dr);

    retTimer.start ();

    state = LinkTaskSM :: wfDR;
    return false;
}


// code is just cutnpaste mostly from idle_Dreq

bool LinkTask :: idle_CloseAll (Message *msg) {

    LinkSAP::CloseAll *lca = (LinkSAP::CloseAll *) msg;

    LinkPeer::DR *dr = (LinkPeer::DR *) peer.create (LinkPeer::dr);
    dr->cid = cid;
    peer.putMessage (dr);

    retTimer.start ();

    state = LinkTaskSM :: wfDR;
    return false;
}

bool LinkTask :: idle_DR (Message *msg) {

    LinkPeer::DR *dr = (LinkPeer::DR *) peer.create (LinkPeer::dr);
    dr->cid = cid;
    peer.putMessage (dr);
    
    LinkSAP::Dind *di = (LinkSAP::Dind *) up.create (LinkSAP::dind);
    di->cid = cid;
    up.putMessage (di);

    state = LinkTaskSM::closed;
    delete this;
    return false;
}


bool LinkTask :: idle_default (Message *msg) {

    return false;
}


bool LinkTask :: wfACK_Dtreq (Message *msg) {

    LinkSAP::Dtreq *dtr = (LinkSAP::Dtreq *) msg;

    buffer.add (&dtr->data);
    return false;
}


bool LinkTask :: wfACK_ACK (Message *msg) {

    LinkPeer::ACK *ack = (LinkPeer::ACK *) msg;

    if ((Byte) seqNumber == ack->seq) { 
	retTimer.stop ();
	buffer.remove ();

	if (buffer.get ()) {
	    LinkPeer::DATA *data = (LinkPeer::DATA *) peer.create (LinkPeer::data);
	    data->seq = sendNumber;
	    Frame &frm = buffer.get ()->copy ();
	    data->udata = frm;
	    delete &frm;
	    peer.putMessage (data);
    
	    incSendNumber ();    
	    retTimer.start ();
	} else {
	    state = LinkTaskSM::idle;
	}
    }

    return false;
}


bool LinkTask :: wfACK_DATA (Message *msg) {

    return idle_DATA (msg);
}


bool LinkTask :: wfACK_DR (Message *msg) {

    return idle_DR (msg);
}    


bool LinkTask :: wfACK_timeout (Message *msg) {

    if (buffer.get ()) {
	LinkPeer::DATA *data = (LinkPeer::DATA *) peer.create (LinkPeer::data);
	data->seq = sendNumber;
	Frame &frm = buffer.get ()->copy ();
	data->udata = frm;
	delete &frm;
	peer.putMessage (data);
	retTimer.start ();
    }

    return false;
}


bool LinkTask :: wfACK_Dreq (Message *msg) {

    return idle_Dreq (msg);
}


bool LinkTask :: wfACK_default (Message *msg) {

    return false;
}


bool LinkTask :: wfDR_DR (Message *msg) {

    LinkSAP::Dind *dind = (LinkSAP::Dind *) up.create (LinkSAP::dind);
    dind->cid = cid;
    up.putMessage (dind);

    retTimer.stop ();

    state = LinkTaskSM::closed;
    delete this;
    return false;
}


bool LinkTask :: wfDR_timeout (Message *msg) {

    LinkPeer::DR *dr = (LinkPeer::DR *) peer.create (LinkPeer::dr);
    dr->cid = cid;
    peer.putMessage (dr);

    retTimer.start ();
    return false;
}


bool LinkTask :: wfDR_default (Message *msg) {

    return false;
}


Uint16 LinkTask :: getId (void) {

    return cid;
}


void LinkTask :: incSendNumber (void) {

    if (sendNumber < 254) {
	sendNumber ++;
    } else {
	sendNumber = 0; 
    }
}


void LinkTask :: incRecNumber (void) {
    
    if (recNumber < 254) {
	recNumber ++;
    } else {
	recNumber = 0; 
    }
}
