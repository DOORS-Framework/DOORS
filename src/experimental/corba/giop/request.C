//Editor-Info: -*- C++ -*-
//
//  Copyright 2002 
//      Telecom Lab, Tampere University of Technology.  
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

//  $Log: request.C,v $
//  Revision 1.8  2003/04/17 12:55:26  bilhanan
//  charstring to c++ string changes
//
//  Revision 1.7  2003/01/23 14:44:17  bilhanan
//  development
//
//  Revision 1.6  2003/01/21 13:59:14  bilhanan
//  devt
//
//  Revision 1.5  2002/09/23 09:33:52  bilhanan
//  development
//
//  Revision 1.4  2002/09/06 09:08:08  bilhanan
//  *** empty log message ***
//
//  Revision 1.3  2002/08/15 10:48:43  bilhanan
//  development.
//
//  Revision 1.2  2002/08/13 10:10:41  bilhanan
//  Development
//
//  Revision 1.1  2002/08/02 13:39:11  bilhanan
//  development
//

#include <doors/hsi.h>
#include <doors/typecode.h>
#include <doors/namedvalue.h>
#include <doors/orb.h>
#include <doors/object.h>
#include <doors/transap.h>
#include <doors/typecode.h>
#include <doors/request.h>
#include <doors/reqtasksm.h>


namespace CORBA {

  CORBA::Boolean is_nil(Request_ptr ptr){
    return ptr == 0;
  }

  void release(Request_ptr ptr){
    if (ptr) delete ptr;
  }

  RequestTask :: RequestTask (std::string n, String operation) 
    : PTask (n, Scheduler :: Instance (), new RequestTaskSM()),
      down (CORBA::ORBTask::Instance()->down),
      peer (CORBA::ORBTask::Instance()->peer),
      _reqid (0),
      _tcpid (-1),
      reqTimer (this, OTime (60)),
      _context (0),
      _operation (operation)
      _arguments (0),
      _result (0),
      _target (0){
    
    state = requestTaskSM :: wfTCPOpen;
  }

  RequestTask :: RequestTask (std::string n, Context_ptr ctx, String operation, NVList_ptr arg_list, NamedValue_ptr result) 
    : PTask (n, Scheduler :: Instance (), new RequestTaskSM()),
      down (CORBA::ORBTask::Instance()->down),
      peer (CORBA::ORBTask::Instance()->peer),
      _reqid (0),
      _tcpid (-1),
      reqTimer (this, OTime (60)),
      _context (ctx),
      _operation (operation), 
      _arguments (NVList :: _duplicate (arglist)), 
      _result (result), 
      _target (0){
  
    state = requestTaskSM :: wfTCPOpen;
  }
  
  RequestTask :: RequestTask (const RequestTask& req)
    : PTask (req._operation, Scheduler :: Instance (), req.sm),
      down (CORBA::ORBTask::Instance()->down),
      peer (CORBA::ORBTask::Instance()->peer),
      _reqid ( CORBA::ORBTask::Instance()->get_reqcnt()),
      _tcpid (req->_tcpid),
      reqTimer (this, OTime (60)),
      _context (ctx),
      _operation (req._operation),
      _arguments (NVList::_duplicate (req._arguments)),
      _result (NamedValue::_duplicate (req._result)),
      _target (Object::_duplicate (req._target)) {

    state = requestTaskSM :: wfTCPOpen;
  
  }
  
  RequestTask :: ~RequestTask (void) {

    if (_arguments) delete _arguments;
    if (_result) delete _result;
    if (_target) delete _target;
  }


  Any& RequestTask :: add_in_arg(void){
    NamedValue_ptr ptr = _arguments-> add(ARG_IN);
    return *(ptr->value());
  }
 
  Any& RequestTask :: add_in_arg(const String name){
    NamedValue_ptr ptr = _arguments-> add_item(name, ARG_IN);
    return *(ptr->value());
  }

  Any& RequestTask :: add_inout_arg(void){
    NamedValue_ptr ptr = _arguments-> add(ARG_INOUT);
    return *(ptr->value());
  }

  Any& RequestTask :: add_inout_arg(const String name){
    NamedValue_ptr ptr = _arguments-> add_item(name, ARG_INOUT);
    return *(ptr->value());
  }

  Any& RequestTask :: add_out_arg(void){ 
    NamedValue_ptr ptr = _arguments-> add(ARG_OUT);
    return *(ptr->value());
  }

  Any& RequestTask :: add_out_arg(String name){ 
    NamedValue_ptr ptr = _arguments-> add_item(name, ARG_OUT);
    return *(ptr->value());
  }

  void RequestTask :: set_return_type(TypeCode_ptr tc){
    _result->value()->replace(TypeCode::_duplicate(tc), 0);
  }

  Any& RequestTask :: return_value(void) {
    return *(_result->value());
  }


  Object_ptr RequestTask :: target() const {
    return _target;
  }
  
  const String* RequestTask :: operation() const {
    return &_operation;
  }

  NVList_ptr RequestTask :: arguments (void){
    return _arguments;
  }
  
  NamedValue_ptr RequestTask :: result(void){   

    return _result;
  }

  void RequestTask :: invoke (void) {
    response_expected = 1;
   this->closed_default();
  }

  void RequestTask :: send_oneway (void) {
    response_expected = 0;
   this->closed_default();
  }

  Request_ptr RequestTask :: _duplicate(Request_ptr req){
    if (!is_nil(req)){
      return new RequestTask(*req);
    }
    
  }

  Request_ptr RequestTask :: _nil(void){ 
    return 0;
  }

  bool RequestTask :: closed_default (Message *msg) { 

    // InetAddr addr =  get inetaddr somewhere;
    IOP::IOR *destior = _target->my_ior;

    if (ior.profiles[0].tag = IOP::TAG_INTERNET_IOP) {

      // let's get the endpoint & port

      Frame *profile_data = destior->profiles[0].profile_data;
      Uint32 len;
      DecodeProfileBody_1_1 (prf, profile_data, 0, len);
      
      InetAddr addr (prf.port, prf.host.Data());
      
      TranSAP :: Creq *cr = (TranSAP :: Creq*)down.create(TranSAP :: creq);
      cr->addr = addr;
      cr->cep = -1; // not yet confirmed, let's use -1.
      cr->ici = _reqid;
      down.putMessage(cr);
      
      state = requestTaskSM :: wfTCPOpen;

    }

  return false; 
  }
  
  bool RequestTask :: wfTCPOpen_default (Message *msg) { 

    return false; 
  }

  bool RequestTask :: wfTCPOpen_Cconf (Message *msg) { 

    TranSAP :: Cconf *cconf = (TranSAP :: Cconf *) msg;
    _tcpid = cconf->cep;

    GiopPeer::Request *req = (GiopPeer::Request *) peer.create (GiopPeer::request);
    req->tcp_cep = _tcpid;

    req->giophdr.GIOP_version.major = 1;
    req->giophdr.GIOP_version.minor = 1;
    req->giophdr.flags = LOCAL_BYTE_ORDER;
    req->giophdr.message_type = GIOP :: Request;

    //    req.giophdr.message_size can't be determined here

    req->reqhdr.service_context = "";
    req->reqhdr.request_id = _reqid;
    req->reqhdr.response_expected = response_expected;
    req->reqhdr.object_key = prf.object_key;
    req->reqhdr.operation = _operation;

    req->args = _arguments;
    peer.putMessage (req);

    state = RequestTaskSM :: wfReply;

    return false; 
  }
 
  bool RequestTask :: wfTCPOpen_Dind (Message *msg) { 

    return false; 
  }
  
  bool RequestTask :: wfReply_default (Message *msg) { 

    return false; 
  }
  
  bool RequestTask :: wfReply_Reply (Message *msg) { 

    return false; 
  }
 
  bool RequestTask :: wfReply_Close (Message *msg) { 

    return false; 
  }

  bool RequestTask :: wfReply_Error (Message *msg) { 

    return false; 
  }
  
  bool RequestTask :: wfReply_Dind (Message *msg) { 

    return false; 
  }
  
  bool RequestTask :: wfReply_timeout (Message *msg) { 

    return false; 
  }
  
  bool RequestTask :: wfLocReply_default (Message *msg) { 

    return false; 
  }
  
  bool RequestTask :: wfLocReply_LocReply (Message *msg) { 

    return false; 
  }
  
  bool RequestTask :: wfLocReply_Close (Message *msg) { 

    return false; 
  }
  
  bool RequestTask :: wfLocReply_Error (Message *msg) { 

    return false; 
  }
  
  bool RequestTask :: wfLocReply_Dind (Message *msg) { 

    return false; 
  }

  bool RequestTask :: wfLocReply_timeout (Message *msg) { 

    return false; 
  }

  Message * RequestTask :: createRequest (){

    GiopPeer::Request *req = (GiopPeer::Request *) peer.create (GiopPeer::request);
    req->tcpid = _tcpid;

    return req;
  }

  Message * RequestTask :: createLocateRequest (){

    GiopPeer::LocateRequest *lreq = (GiopPeer::LocateRequest *) peer.create (GiopPeer::locaterequest);
    lreq->tcpid = _tcpid;

    return lreq;
  }

   
}
