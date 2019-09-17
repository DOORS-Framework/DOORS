
  //  Copyright 2000
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
  
$Log: mc6joinent.h,v $
Revision 1.1  2003/11/17 12:37:35  hartmanj
Initial commit


*/

#ifndef _mc6joinent_h_
#define _mc6joinent_h_

#include <string>
#include <doors/udp6task.h>
#include "mc6joinsap.h"

class MC6JoinerEntitySM;

class MC6JoinerEntity : public PTask {

public:

     MC6JoinerEntity( std::string n, Scheduler *s, MC6JoinerEntitySM *esm,
                      const InetAddr6 addr, Udp6Task *udp );

     ~MC6JoinerEntity();

     bool ready_default(Message *msg );
     bool OpenMC6Group ( Message *msg );

     MC6JoinerSAP :: Provider up;
     Udp6SAP::User down;
     
private:

     InetAddr6 addr_;
     Udp6Task *udp6task_;
     MC6JoinerEntitySM *esm_;
     
};


#endif
