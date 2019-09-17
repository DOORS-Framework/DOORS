// Editor-info: -*- C++ -*-
//  Copyright 2003
//      Telecoms Lab, Tampere University of Technology.  All rights reserved.

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
$Log: uaproxy.C,v $
Revision 1.1  2004/10/12 11:57:18  bilhanan
AA code merged into main tree from src/experimental

Revision 1.5  2004/08/03 05:36:41  hartmanj
Bug fixes

Revision 1.4  2004/05/25 09:41:32  hartmanj
Typo-fixes, removed use of deprecated classes/methods, general development.

Revision 1.3  2003/08/20 11:48:07  bilhanan
va<->aa<->network srv req/rply functionality coded.

Revision 1.2  2003/08/19 12:38:42  bilhanan
development & retransmit capability removed.

Revision 1.1  2003/08/18 10:54:25  bilhanan
initial version.

*/

#include <string>
#include <doors/uaproxy.h>
#include <doors/uaproxysm.h>
#include <doors/aaent.h>


#define ULOS(x) std::cout << x << std::endl

UAProxy :: UAProxy (std::string n, Uint16 AA_XID, Uint16 VA_XID, InetAddr6 source, AAEntity *e, UAProxySM *sm) 
  : PTask (n, e->getScheduler(), sm),
    peer (e->peer),
    entity (this),
    va_msg(0),
    va_type_msg(0),
    ent(e),
    aa_XID (AA_XID),
    va_XID (VA_XID),
    va_addr (source),
    timer( this, OTime(3)),
    time_used(0), max_time_try(10) {

  state = UAProxySM :: Ready;

}

UAProxy :: ~UAProxy (void) {
  if( va_msg )
    delete va_msg;
  if( va_type_msg )
    delete va_type_msg;
}

bool UAProxy :: ready_srvTypeRqst (Message *msg){
ULOS("UAProxy :: ready_srvTypeRqst");
  // AAEntity has already tested positive this is from VA

  SLPPeer::SRVTYPERQST *va_rqst = (SLPPeer::SRVTYPERQST *) msg;

  if( !va_type_msg )
    va_type_msg = (SLPPeer :: SRVTYPERQST*)va_rqst->clone();

  if (this->va_XID == va_rqst->XID){
    SLPPeer::SRVTYPERQST *aa_rqst = new SLPPeer::SRVTYPERQST (*va_rqst);
    aa_rqst->source.set (ent->ownAddr_);

    if (ent->dafinder.isDAPresent()) {
      InetAddr6 DA_Addr = ent->dafinder.getDA();
      aa_rqst->dest.set (DA_Addr);

    } else {
      InetAddr6 dest_addr(ent->slpPort_, ent->getMCAddr ("SVRLOC"));
      aa_rqst->dest.set (dest_addr);
    }

    aa_rqst->XID = aa_XID;
    peer.putMessage (aa_rqst);
    state = UAProxySM :: wfSrvTypeRply;
  }

  OTime ot(3);
  timer.setInitial(ot);
  timer.start();

  return false;
}


bool UAProxy :: ready_srvRqst (Message *msg){
ULOS("UAProxy :: ready_srvRqst");
  // AAEntity has already tested positive this is from VA

  SLPPeer::SRVRQST *va_rqst = (SLPPeer::SRVRQST *) msg;
  msg->unlock();

  if( !va_msg )
    va_msg = (SLPPeer :: SRVRQST*)va_rqst->clone();

  if (this->va_XID == va_rqst->XID){
    SLPPeer::SRVRQST *aa_rqst = new SLPPeer::SRVRQST (*va_rqst);
    //aa_rqst->source.set (ent->ownAddr_);

    //if (ent->dafinder.isDAPresent()) {
    //  InetAddr6 DA_Addr = ent->dafinder.getDA();
    //  aa_rqst->dest.set (DA_Addr);

    //} else {
      InetAddr6 dest_addr(ent->slpPort_, ent->getMCAddr (aa_rqst->servicetype));
      aa_rqst->dest.set (dest_addr);
    //}
    
    aa_rqst->XID = aa_XID;
    peer.putMessage (aa_rqst);
    state = UAProxySM :: wfSrvRply;
  }

  OTime ot(3);
  timer.setInitial(ot);
  timer.start();

  return false;
}

bool UAProxy :: ready_default (Message *msg){
ULOS("UAProxy :: ready_default");
   return false;

}

bool UAProxy :: wfSrvTypeRply_srvTypeRply (Message *msg){

   SLPPeer :: SRVTYPERPLY *site_rply = (SLPPeer :: SRVTYPERPLY *) msg;
   // check XID

   if (site_rply->XID == this->aa_XID) {
     SLPPeer::SRVTYPERPLY *aa_rply = new SLPPeer::SRVTYPERPLY (*site_rply);
     aa_rply->source.set (ent->ownAddr_);
     aa_rply->dest.set (va_addr);
     aa_rply->XID = va_XID;
     peer.putMessage (aa_rply);
     delete this;
   }

   return false;
}

bool UAProxy :: wfSrvRply_srvRply (Message *msg){

   SLPPeer :: SRVRPLY *site_rply = (SLPPeer :: SRVRPLY *) msg;
   // check XID

   if (site_rply->XID == this->aa_XID) {
     SLPPeer::SRVRPLY *aa_rply = new SLPPeer::SRVRPLY (*site_rply);
     aa_rply->source.set (ent->ownAddr_);
     aa_rply->dest.set (va_addr);
     aa_rply->XID = va_XID;
     peer.putMessage (aa_rply);
     delete this;
   }

   return false;
}

bool UAProxy :: wfSrvTypeRply_timeout(Message *msg) {

  time_used += timer.getInitial().getSeconds();

  if( time_used > max_time_try )
    return false;

  return ready_srvTypeRqst (va_type_msg);
}


bool UAProxy :: wfSrvRply_timeout(Message *msg) {

  time_used += timer.getInitial().getSeconds();

  if( time_used > max_time_try )
    return false;

  return ready_srvRqst (va_msg);
}

bool UAProxy :: wfSrvRply_default (Message *msg){

   return false;

}


