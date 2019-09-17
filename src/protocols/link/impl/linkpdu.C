// Editor-info: -*- C++ -*-
// 
//  Copyright 1999 
//      Telecom Lab, Lappeenranta University of Technology.  
//      All rights reserved.
 
 
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

//  $Log: linkpdu.C,v $
//  Revision 1.4  2002/07/23 16:34:14  hartmanj
//
//  Development
//
//  Revision 1.3  2002/07/23 06:43:31  ik
//  Syntax error.
//
//  Revision 1.2  2002/07/12 08:47:43  bilhanan
//  linkrandpdu unneeded and therefore removed.
//

#include <doors/linksapbase.h>
#include <doors/linkpdu.h>
#include <doors/linkpdu.i>
#include <doors/phys.h>
#include <doors/linkent.h>


Uint16 LinkPeer :: CR :: decode (Message *msg) {

    Frame b = ((PhysSAP::Dti *) msg)->data;
    type = cr;
    cid = b.getFirst ();
    data = b;

    return 1 + data.length ();
}


Message *LinkPeer :: CR :: encode (void) {

    PhysSAP::Dtr *dtr = new PhysSAP::Dtr;
    Frame b = data;
    b.putFirst (cid);
    b.putFirst (LinkPeer::cr);

    dtr->data = b;

    return dtr;
}


Uint16 LinkPeer :: CC :: decode (Message *msg) {

    Frame b = ((PhysSAP::Dti *) msg)->data;
    type = cc;
    cid = b.getFirst ();
    data = b;

    return 1 + data.length ();
}


Message *LinkPeer :: CC :: encode (void) {

    PhysSAP::Dtr *dtr = new PhysSAP::Dtr;
    Frame b = data;
    b.putFirst (cid);
    b.putFirst (LinkPeer::cc);

    dtr->data = b;

    return dtr;
}


Uint16 LinkPeer :: DATA :: decode (Message *msg) {

    Frame b = ((PhysSAP::Dti *) msg)->data;
    type = data;
    cid = b.getFirst ();
    seq = b.getFirst ();
    udata = b;

    return 2 + udata.length ();
}


Message *LinkPeer :: DATA :: encode (void) {

    PhysSAP::Dtr *dtr = new PhysSAP::Dtr;
    Frame b = udata;
    b.putFirst (seq);
    b.putFirst (cid);
    const char * mmtype= getenv ("NETWORK");
    if (strcmp (mmtype, "down") == 0){
      b.putFirst (LinkPeer::other);

    } else if (strcmp (mmtype, "up") == 0){
      b.putFirst (LinkPeer::data);

    } else if (strcmp (mmtype, "partial") == 0){

      //note that the "partial" option is not documented nor tested

          if (rand()%2){
	    b.putFirst (LinkPeer::other);
	  } else
	    b.putFirst (LinkPeer::data);
    }
    
    dtr->data = b;

    return dtr;
}


Uint16 LinkPeer :: ACK :: decode (Message *msg) {

    Frame b = ((PhysSAP::Dti *) msg)->data;
    type = ack;
    cid = b.getFirst ();
    seq = b.getFirst ();

    return 2;
}


Message *LinkPeer :: ACK :: encode (void) {

    PhysSAP::Dtr *dtr = new PhysSAP::Dtr;
    Frame b;
    b.putFirst (seq);
    b.putFirst (cid);
    b.putFirst (LinkPeer::ack);

    dtr->data = b;

    return dtr;
}


Uint16 LinkPeer :: DR :: decode (Message *msg) {

    Frame b = ((PhysSAP::Dti *) msg)->data;
    type = dr;
    cid = b.getFirst ();

    return 1;
}


Message *LinkPeer :: DR :: encode (void) {

    PhysSAP::Dtr *dtr = new PhysSAP::Dtr;
    Frame b;
    b.putFirst (cid);
    b.putFirst (LinkPeer::dr);

    dtr->data = b;

    return dtr;
}


Sint16 LinkPeer :: identify (Message *msg) {

    Frame &b = ((PhysSAP::Dti *) msg)->data;

    Uint16 type = b.getFirst ();

    return type;
}
