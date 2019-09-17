
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

$Log: exceptions.h,v $
Revision 1.1.1.1  2002/05/13 14:38:35  bilhanan
Restructured

Revision 1.1.1.1  2002/04/30 12:32:49  bilhanan
Development

Revision 1.2  2001/06/08 16:45:29  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:42  bilhanan
Full licence clause added into doors 0.1


*/


#ifndef _doors_exceptions_h_
#define _doors_exceptions_h_

#include <stdexcept>
 
#define ex_body {unsigned long minor; completion_status completed;} 

enum completion_status {COMPLETED_YES, COMPLETED_NO, COMPLETED_MAYBE}; 
enum exception_type {NO_EXCEPTION, USER_EXCEPTION, SYSTEM_EXCEPTION};

exception UNKNOWN ex_body;       // the unknown exception 
exception BAD_PARAM ex_body;     // an invalid parameter was passed 
exception NO_MEMORY ex_body;     // dynamic memory allocation failure
exception IMP_LIMIT ex_body;     // violated implementation limit 
exception COMM_FAILURE ex_body;  // communication failure 
exception INV_OBJREF ex_body;    // invalid object reference 
exception NO_PERMISSION ex_body; // no permission for attempted op. 
exception INTERNAL ex_body;      // ORB internal error 
exception MARSHAL ex_body;       // error marshalling param/result 
exception INITIALIZE ex_body;    // ORB initialization failure 
exception NO_IMPLEMENT ex_body;  // operation implementation unavailable 
exception BAD_INV_ORDER ex_body; // bad DIR argument call
exception BAD_TYPECODE ex_body;  // bad typecode 
exception BAD_OPERATION ex_body; // invalid operation 
exception NO_RESOURCES ex_body;  // insufficient resources for req. 
exception NO_RESPONSE ex_body;   // response to req. not yet available
exception PERSIST_STORE ex_body; // persistent storage failure 
exception BAD_INV_ORDER ex_body; // routine invocations out of order
exception TRANSIENT ex_body;     // transient failure - reissue request  
exception FREE_MEM ex_body;      // cannot free memory 
exception INV_IDENT ex_body;     // invalid identifier syntax 
exception INV_FLAG ex_body;      // invalid flag was specified 
exception INTF_REPOS ex_body;    // error accessing interface repository 
exception BAD_CONTEXT ex_body;   // error processing context object 
exception OBJ_ADAPTER ex_body;   // failure detected by object adapter 
exception DATA_CONVERSION ex_body;  // data conversion error 
exception OBJECT_NOT_EXIST ex_body; // non-existent object, delete reference 
exception TRANSACTION_REQUIRED ex_body;   // transaction required 
exception TRANSACTION_ROLLEDBACK ex_body; // transaction rolled back 
exception INVALID_TRANSACTION ex_body;    // invalid transaction

#endif
