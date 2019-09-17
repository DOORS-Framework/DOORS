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

/*

$Log: genmac.h,v $
Revision 1.1.1.1  2002/04/30 12:32:52  bilhanan
Development

Revision 1.2  2001/06/08 16:45:31  bilhanan
Full Licence clause into DOORS 0.2

Revision 1.4  2001/06/08 08:39:37  bilhanan
Full licence clause added into doors 0.1


*/


#if ! _genmac_h
#define _genmac_h 1

#include <stdio.h>
#include <stdlib.h>

#ifndef MSG_STREAM
#define MSG_STREAM stderr
#endif

//Name: PASS
//Type: macro
//Context: util
//View: public
//Parameters:
//Description:
//  Pass declares the programs has executed successfylly, and
//  will exit by zero exit status.
//
//  PASS is most used in regression test cases.
//
#define PASS() \
  {exit(0);}

//Name: FAIL
//Type: macro
//Context: util
//View: public
//Parameters: msg
//Description:
//  FAIL declares the program has reached failure point,
//  and shall exit by nowzero exit status.
//
//  The parameter msg is human readable information
//  about the fail situation.
//
//  FAIL is nost used in regression test cases.
//
#define FAIL(msg) \
  {fprintf(MSG_STREAM,"Fail:%s:%d:%s\n",__FILE__,__LINE__,msg); \
  exit(1);}

#endif // _genmac_h


