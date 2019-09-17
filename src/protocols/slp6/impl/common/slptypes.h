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

$Log: slptypes.h,v $
Revision 1.5  2004/04/07 13:43:34  hartmanj
slpheader.h steps in

Revision 1.4  2004/02/03 14:43:41  bilhanan
printFrame() removed.

Revision 1.3  2003/10/03 14:30:03  bilhanan
fixed header includes. removed "using namespace" usage.

Revision 1.2  2003/06/04 11:09:56  kalliosa
Licence clause added

  
*/

 // typedefs for DOORS' SLP component

#ifndef SLPTYPES_H
#define SLPTYPES_H

#include <doors/debug.h>

#include <vector>
#include <string>
#include <set>
#include <doors/inetaddr6.h>

class ServiceURL;
class ServiceType;

// constants
//const int RQST_RETRANS = 10; // request retransmission time
const int CONFIG_MC_MAX =15;
const int CONFIG_START_WAIT = 3;
const int CONFIG_RETRY = 2;
const int CONFIG_RETRY_MAX = 15;

  
// own datatypes
typedef std::string ServiceLocationAttribute;
typedef std::string AuthBlock;


typedef std::vector <AuthBlock> AuthBlockVector;
typedef std::vector <std::string> StringVector;
typedef std::vector <ServiceURL> URLVector;
typedef std::set <InetAddr6> AddrVector;
typedef std::vector <ServiceLocationAttribute> AttrVector;


#endif // SLPTYPES_H
