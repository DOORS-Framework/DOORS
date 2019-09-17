// -*- C++ -*-

//  Copyright 2000 
//  	Telecoms Lab, Tampere University of Technology.  All rights reserved.

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

$Log: serverrequest.h,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:59  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.8  2001/06/08 08:39:46  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef _doors_serverrequest_
#define _doors_serverrequest_

#include <doors/datatypes.h>
#include <doors/any.h>

namespace CORBA {

  class NVList;
  typedef NVList* NVList_ptr;
  class ServerRequest;
  typedef ServerRequest* ServerRequest_ptr;

  class ServerRequest { 

    friend class ORB;
    
    ServerRequest (Object_ptr ptr);
    ServerRequest (String operation, Object_ptr ptr);
    ServerRequest (const ServerRequest& req);

    void addvalues (NVList_ptr& args); 
    String _operation;
    NVList_ptr _arguments;
    Any _result;
    Object_ptr _target;
    NVList_ptr _other;
    CORBA::Boolean Firsttime;

  public: 

    virtual ~ServerRequest ();
    String operation() const;
    void arguments (NVList_ptr& parameters);
    // Context_ptr ctx();
    void set_result(const Any& value);
    // void set_exception (const Any& value);

    /*  Hmmm should these be implemented?   
    static ServerRequest_ptr _duplicate(ServerRequest_ptr req);
    static ServerRequest_ptr _nil(void);
    */

  };


  CORBA::Boolean is_nil(ServerRequest_ptr ptr){
    return ptr == 0;
  }

  void release(ServerRequest_ptr ptr){
    if (ptr) delete ptr;
  }


}

#endif