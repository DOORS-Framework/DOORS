//Editor-Info: -*- C++ -*-
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

//  $Log: physent.C,v $
//  Revision 1.5  2003/08/11 23:52:44  bilhanan
//  const char* in constructor -> std::string
//
//  Revision 1.4  2002/08/14 09:32:29  bilhanan
//  ONS_ macros are now renamed. Refer to top-level ChangeLog for details.
//
//  Revision 1.3  2002/07/23 16:38:31  ik
//  Od{Boolean,True,False} -> {bool,true,false}
//
//  Revision 1.2  2002/07/22 22:33:38  bilhanan
//  Licence clause added.
//
//
//

#include <string>
#include <doors/ocoremac.h>
#include <doors/physent.h>
#include <doors/physsm.h>
#include <doors/hsi.h>


PhysEntity :: PhysEntity (std::string n, Scheduler *s, PhysSM *psm) 
: PTask (n, s, psm), up (this), left (this), right (this) {

    _MT_SYNCMONITOR;
    STATIC_SYMBOL (up, this);
    STATIC_SYMBOL (left, this);
    STATIC_SYMBOL (right, this);
}


PhysEntity :: ~PhysEntity (void) {
  
    _MT_SYNCMONITOR;
}


bool PhysEntity :: upDtr (Message *msg) {

    _MT_SYNCMONITOR;
    Message *clone = 0;

    if (right.connected ()) {
	clone = msg->clone ();
	right.putMessage (clone);
    }
    
    if (left.connected ()) {
	clone = msg->clone ();
	left.putMessage (clone);
    }

    return false;
}


bool PhysEntity :: leftDtr (Message *msg) {

    _MT_SYNCMONITOR;
    Message *clone = 0;

    if (up.connected ()) {
	PhysSAP :: Dti *dti = (PhysSAP :: Dti *) up.create (PhysSAP :: dti);
	PhysSAP::Dtr *dtr = (PhysSAP::Dtr*)msg;
	Frame &copy = dtr->data.copy ();
	dti->data = copy; // move
	delete &copy;
	up.putMessage(dti);
    }

    if (right.connected ()) {
	clone = msg->clone ();
	right.putMessage(clone);
    }

    return false;
}


bool PhysEntity :: rightDtr (Message *msg) {

    _MT_SYNCMONITOR;
    Message *clone = 0;

    if (up.connected ()) {
	PhysSAP :: Dti *dti = (PhysSAP :: Dti *) up.create (PhysSAP :: dti);
	Frame &copy = ((PhysSAP :: Dtr *) msg)->data.copy ();
	dti->data = copy;
	delete &copy;
	up.putMessage(dti);
    }

    if (left.connected ()) {
	clone = msg->clone ();
	left.putMessage(clone);
    }

    return false;
}
