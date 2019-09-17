//Editor-Info: -*- C++ -*-
//
 //  Copyright 1999 
//      Telecom Lab, Lappeenranta University of Technology.  
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

//  $Log: physent.h,v $
//  Revision 1.4  2003/08/11 23:52:44  bilhanan
//  const char* in constructor -> std::string
//
//  Revision 1.3  2002/07/23 16:38:32  ik
//  Od{Boolean,True,False} -> {bool,true,false}
//
//  Revision 1.2  2002/07/22 22:33:38  bilhanan
//  Licence clause added.
//


#ifndef PHYSENT_H
#define PHYSENT_H

#include <string>
#include <doors/ptb.h>
#include <doors/phys.h>

class PhysSM;

class PhysEntity : public PTask {
  public:
    PhysEntity (std::string n, Scheduler *s, PhysSM *psm);
    virtual ~PhysEntity (void);
    
    bool upDtr (Message *msg);
    bool leftDtr (Message *msg);
    bool rightDtr (Message *msg);

    PhysSAP::Provider up;
    Port left;
    Port right;
};


#endif  // PHYSENT_H

