//  Copyright 2002
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

$Log: sapentity.h,v $
Revision 1.6  2003/08/11 13:20:50  bilhanan
now with ipv6 goodness.

Revision 1.5  2003/07/09 15:57:54  bilhanan
general cleanup and code rewrite.

Revision 1.4  2003/04/17 09:04:29  bilhanan
Forced commit, now properly supports multiple sessions, 1 udptask per session.

Revision 1.3  2003/04/17 08:54:32  bilhanan
charstring to std::string conversion

Revision 1.2  2002/07/23 16:38:34  ik
Od{Boolean,True,False} -> {bool,true,false}

Revision 1.1  2002/06/17 06:05:37  hartmanj
Initial revision


*/

#ifndef _sapent_h_
#define _sapent_h_

#include <string>
#include <doors/ptb.h>
#include <doors/sapentitysap.h>

class SAPEntitySM;
class SAPSessionSM;

class SAPEntity : public PTask {
	
 public:

	SAPEntity( std::string n, Scheduler *s, SAPEntitySM *esm,
		   const InetAddr *dest, const InetAddr6 *dest6);

	virtual ~SAPEntity();


	bool ready_default( Message *msg );
	bool SetupSession( Message *msg );

	void createConnection (Uint16 cid, Uint8 ipversion);

	SAPEntSAP::Provider up;
	InetAddr dest_;
	InetAddr6 dest6_;


  private:

	Port connections;
	PortMux connMux;
	SAPSessionSM *connSM;
	

};

#endif


