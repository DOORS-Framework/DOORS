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

//  $Log: request.h,v $
//  Revision 1.6  2003/04/17 12:55:25  bilhanan
//  charstring to c++ string changes
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
//  Revision 1.2  2002/07/25 19:28:21  bilhanan
//  Initial revision
//
//  Revision 1.1  2002/07/25 15:48:52  bilhanan
//  *** empty log message ***
//

#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <doors/corba_dcl.h>
#include <doors/ptb.h>
#include <doors/orbtask.h>
#include <doors/tran.h>
#include <doors/gioppeer.h>

class ORBTask;
class RequestTaskSM;

namespace CORBA {
  class RequestTask : public PTask {
    
  public:

    virtual ~RequestTask (void);
    
    Tran::User &down;
    GiopPeer &peer;
    
    Port entity;
    
    
    Any& add_in_arg(); 
    Any& add_in_arg(const String name); 
    Any& add_inout_arg();
    Any& add_inout_arg(const String name); 
    Any& add_out_arg(); 
    Any& add_out_arg(const String name); 
    void set_return_type(TypeCode_ptr tc); 
    Any& return_value(); 
    
    Object_ptr target() const; 
    const String *operation() const; 
    NVList_ptr arguments(); 
    NamedValue_ptr result(); 
    
    void invoke (some callback stuff);
    void send_oneway (no callback stuff);
    
    void get_response(); 
    CORBA::Boolean poll_response(); 
    
    static Request_ptr _duplicate(Request_ptr req);
    static Request_ptr _nil(void);

  private:

    friend class Object;
    friend class ORB;
    friend class RequestTaskSM;
  
    Uint16 _reqid;
    Sint16 _tcpid;
    
    Timer reqTimer;
    
    Context_ptr _context;
    String _operation;
    NVList_ptr _arguments;
    NamedValue_ptr _result;
    Object_ptr _target;

    IIOP :: ProfileBody_1_1 prf;
    CORBA :: Boolean response_expected;

    RequestTask (std::string n, String operation);
    RequestTask (std::string n, Context_ptr ctx, String operation, NVList_ptr arg_list, NamedValue_ptr result);
 
    RequestTask (const RequestTask& req); // needed for the _duplicate()

    bool closed_default (Message *msg);
    
    bool wfTCPOpen_default (Message *msg);
    bool wfTCPOpen_Cconf (Message *msg);
    bool wfTCPOpen_Dind (Message *msg);
    
    bool wfReply_default (Message *msg);
    bool wfReply_Reply (Message *msg);
    bool wfReply_Close (Message *msg);
    bool wfReply_Error (Message *msg);
    bool wfReply_Dind (Message *msg);
    bool wfReply_timeout (Message *msg);
    
    bool wfLocReply_default (Message *msg);
    bool wfLocReply_LocReply (Message *msg);
    bool wfLocReply_Close (Message *msg);
    bool wfLocReply_Error (Message *msg);
    bool wfLocReply_Dind (Message *msg);
    bool wfLocReply_timeout (Message *msg);

    Message *createRequest ();
    Message *createLocateRequest ();

    // Boolean callingback;  <-- we'll see if this is needed later.
  };
  
  /*  

 class Request_out { 

  public: 
    
    Request_out(Request_ptr& p) 
      : ptr_(p) { 
      ptr_ = Request::_nil(); 
    } 

    Request_out(Request_out& a) 
      : ptr_(a.ptr_) {} 

    Request_out& operator=(Request_out& a) { 
      ptr_ = a.ptr_; return *this; 
    } 

    Request_out& operator=(Request_ptr p) { 
      ptr_ = p; 
      return *this; 
    } 

    operator Request_ptr&() { 
      return ptr_; 
    } 

    Request_ptr& ptr() { 
      return ptr_; 
    } 

    Request_ptr operator->() { 
      return ptr_; 
    } 

  private: 
    Request_ptr& ptr_; 

  };

  */

  CORBA::Boolean is_nil(Request_ptr ptr);
  void release(Request_ptr ptr);
  

}

#endif
