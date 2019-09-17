
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

$Log: serverrequest.C,v $
Revision 1.1.1.1  2002/05/13 14:38:36  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:46:40  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.8  2001/06/08 08:39:46  bilhanan
Full licence clause added into doors 0.1


*/


#include <doors/serverrequest.h>
#include <doors/namedvalue.h>

namespace CORBA {

  ServerRequest :: ServerRequest (Object_ptr ptr) 
    : Firsttime (CORBA::TRUE){
  }

  ServerRequest :: ServerRequest (String operation, Object_ptr ptr)
  : _operation (operation) {
  }

  ServerRequest :: ServerRequest (const ServerRequest& req){
  }

  ServerRequest :: ~ServerRequest (void){
  }

  String ServerRequest :: operation (void) const{
    return _operation;
  }

  void ServerRequest :: arguments (NVList_ptr& parameters) {

    if (Firsttime){
      Firsttime = CORBA::FALSE;
      addvalues(parameters);
    } // else throw BAD_INV_ORDER();
    return;
  }

  void ServerRequest :: set_result(const Any& value){

    // could this be an appropriate place to
    // compare "IN" values for illegal DIR modifications?

    delete _arguments;
    _arguments = NVList :: _duplicate (_other);
    delete _other;
    _result = value;

  }

  void ServerRequest :: addvalues (NVList_ptr& args){

    // at the moment, no typechecking or compare done, 
    // therefore a straight copy of arguments is done here.
    // this is still legal, though.
    // the problem of how to enforce dir not to modify
    // "IN" types is not solved yet.

    delete args;
    _other = NVList :: _duplicate (_arguments);
    args = _other;
  }

}
