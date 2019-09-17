//Editor-Info: -*- C++ -*-
//
//  Copyright 2000 
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.
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

/*

$Log: iiopsm.h,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1  2002/01/23 13:10:23  bilhanan
reworking IIOP. Under heavy construction.

Revision 1.8  2001/06/08 08:40:18  bilhanan
Full licence clause added into doors 0.1


*/
//  =========================================================================
//  File:     iiopsm.h
//  Date:     24.03.2000
//  Subject:  IIOP ver 1.2
//  Desc:     
//
//  Author:   Janne Makihonka, TTKK/DMI
//  =========================================================================


#ifndef IIOPSM_H
#define IIOPSM_H


#include <doors/frame.h>
#include <doors/iiopconn.h>
#include <doors/giop.h>
#include <doors/iiopsap.h>
#include <doors/iop.h>
#include <doors/iiop.h>


class IiopSm {

public:
  
  virtual OdBoolean run(Message *msg, Sint16 *iiopstate, 
			IiopConnection* connection)
    {
      DEBUGINFO("IiopSm::run, up");
      return OdFalse;
    };
  
  
  virtual OdBoolean run(Frame *frm, Sint16 *iiopstate, 
			IiopConnection* connection)
    {
      DEBUGINFO("IiopSm::run, peer");
      return OdFalse;
    };
  
protected:
  
  virtual Frame* Encode(Giop::IiopMsg *msg)
    {
      return NULL; 
    };
  
  virtual Giop::IiopMsg* Decode(Frame *msg)
    {
      return NULL; 
    };
  
  
  
  OdBoolean PeekMsgHeader(Frame *frm, Giop::MessageHeader &msghdr)
    {
      Byte *tmpptr = (Byte *)&msghdr;
      for(int i = 0; i < 12; ++i){
	*tmpptr = frm->read(i);
	++tmpptr;
      }
      int val = memcmp(&msghdr,"GIOP",4);
      val += (msghdr.GIOP_version.major == 1 ) ? 0 : 1;
      if(val) {
	ERROR("PeekMsgHeader");
	return OdTrue;
      }
      return OdFalse;
    };
  
  
  IiopConnection *conn;
  Uint16 version;
  Sint16 *state;
  
private:
  
  OdBoolean peerPutMessage(Giop::IiopMsg *msg) 
    {
      DEBUGINFO("IiopSm::peerPutMessage");
      return conn->peerPutMessage(Encode((Giop::IiopMsg *)msg));
    };

};


#endif
